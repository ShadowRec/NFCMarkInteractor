#include <QApplication>
#include "nfcinterface.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);

    NFCInterface window;
    window.show();

    return app.exec();
}
