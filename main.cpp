#include "mqtt/mqttSvc/mqttAPI.h"
#include "networktool.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    initMqttSvc();
    networkTool w;
    w.show();
    return a.exec();
}
