#include "Core/ApplicationBootstrap.h"

#include "Core/AppContext.h"
#include "Windows/loginwindow.h"

#include <QApplication>
#include <QCoreApplication>
#include <QLocale>
#include <QTranslator>

namespace {
QWidget *createDefaultInitialWindow(AppContext &appContext)
{
    return new LoginWindow(appContext.authService().get());
}
}

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
