#include "Core/ApplicationBootstrap.h"

#include "Core/AppContext.h"
#include "DAO/UsuarioDAO.h"
#include "Database/DatabaseManager.h"
#include "Database/SchemaInitializer.h"
#include "Services/AuthService.h"
#include "Windows/cadastroadminwindow.h"
#include "Windows/loginwindow.h"

#include <QApplication>
#include <QCoreApplication>
#include <QLocale>
#include <QMessageBox>
#include <QTranslator>

namespace {

LoginWindow *createLoginWindow(AppContext &appContext)
{
    return new LoginWindow(appContext.authService().get());
}

CadastroAdminWindow *createAdminRegistrationWindow(AppContext &appContext)
{
    auto *adminWindow = new CadastroAdminWindow(appContext.authService().get());

    QObject::connect(adminWindow, &CadastroAdminWindow::adminRegistered, adminWindow, [&appContext, adminWindow]() {
        auto *loginWindow = createLoginWindow(appContext);
        loginWindow->setAttribute(Qt::WA_DeleteOnClose, true);
        loginWindow->show();
        adminWindow->close();
    });

    return adminWindow;
}

QWidget *createDefaultInitialWindow(AppContext &appContext)
{
    const auto authService = appContext.authService();
    if (!authService) {
        return new QMessageBox(
            QMessageBox::Critical,
            QStringLiteral("Erro de inicializacao"),
            QStringLiteral("Servico de autenticacao indisponivel."));
    }

    if (authService->checkAdminExists()) {
        return createLoginWindow(appContext);
    }

    if (!authService->lastError().isEmpty()) {
        return new QMessageBox(
            QMessageBox::Critical,
            QStringLiteral("Erro de inicializacao"),
            authService->lastError());
    }

    return createAdminRegistrationWindow(appContext);
}

} // namespace

ApplicationBootstrap::ApplicationBootstrap(AppContext &appContext)
    : m_appContext(appContext)
{
}

void ApplicationBootstrap::configureApplication(QApplication &application)
{
    QCoreApplication::setOrganizationName("PROG3");
    QCoreApplication::setOrganizationDomain("local.armazem");
    QCoreApplication::setApplicationName("Armazem");

    installTranslations(application);
    m_appContext.ensureAppDataDirectory();
    ensureAuthDependencies();
    ensureDefaultInitialWindowFactory();
}

std::unique_ptr<QWidget> ApplicationBootstrap::createInitialWindow()
{
    ensureDefaultInitialWindowFactory();
    return std::unique_ptr<QWidget>(m_appContext.createInitialWindow());
}

void ApplicationBootstrap::installTranslations(QApplication &application)
{
    auto *translator = new QTranslator(&application);
    const QStringList uiLanguages = QLocale::system().uiLanguages();

    for (const QString &locale : uiLanguages) {
        const QString baseName = "Armazem_" + QLocale(locale).name();
        if (translator->load(baseName, ":/i18n")
            || translator->load(baseName, application.applicationDirPath())
            || translator->load(baseName, application.applicationDirPath() + "/i18n")) {
            application.installTranslator(translator);
            return;
        }
    }
}

void ApplicationBootstrap::ensureDefaultInitialWindowFactory()
{
    if (!m_appContext.hasInitialWindowFactory()) {
        m_appContext.setInitialWindowFactory(createDefaultInitialWindow);
    }
}

bool ApplicationBootstrap::ensureAuthDependencies()
{
    if (m_appContext.authService()) {
        return true;
    }

    auto databaseManager = m_appContext.databaseManager();
    if (!databaseManager) {
        databaseManager = std::make_shared<DatabaseManager>(m_appContext.databaseFilePath());
        m_appContext.setDatabaseManager(databaseManager);
    }

    SchemaInitializer schemaInitializer(*databaseManager);
    if (!schemaInitializer.initialize()) {
        return false;
    }

    auto usuarioDao = std::make_shared<UsuarioDAO>(*databaseManager);
    m_appContext.setAuthService(std::make_shared<AuthService>(usuarioDao));
    return true;
}
