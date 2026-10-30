#include <QApplication>
#include <QIcon>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    // Enable High-DPI scaling and smooth fonts
    QApplication::setApplicationName("Career Navigator");
    QApplication::setOrganizationName("CareerNavigatorEdu");
    QApplication::setApplicationVersion("2.0.0");

    QApplication app(argc, argv);

    MainWindow window;
    window.show();

    return app.exec();
}
