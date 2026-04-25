#ifndef APPLICATIONBOOTSTRAP_H
#define APPLICATIONBOOTSTRAP_H

#include <QWidget>

#include <memory>

class QApplication;
class AppContext;

class ApplicationBootstrap
{
public:
    explicit ApplicationBootstrap(AppContext &appContext);

    void configureApplication(QApplication &application);
    std::unique_ptr<QWidget> createInitialWindow();

private:
    void installTranslations(QApplication &application);
    void ensureDefaultInitialWindowFactory();
    bool ensureAuthDependencies();

    AppContext &m_appContext;
};

#endif // APPLICATIONBOOTSTRAP_H
