#include <QApplication>
#include <QQmlApplicationEngine>

#include <QWindow>
#include <QtQml>

#include "qaxis.h"

#include <stdlib.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    //Register C++ Classes in Qml Enviroment
    qmlRegisterType<QAxis>("com.robinson.qaxis", 1, 0, "QAxis");

    QQmlApplicationEngine *engine = new QQmlApplicationEngine();
    engine->load(QUrl(QStringLiteral("qrc:///main.qml")));

    int result = app.exec();

    delete engine;

    return result;
}
