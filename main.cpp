#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include "ui_nfcinterface.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);

    QMainWindow window;
        Ui::NFCInterface ui;
        ui.setupUi(&window);
        window.show();

    return app.exec();
}
