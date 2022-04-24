QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mongoose/mongoose.c \
    mqtt/mqttSetting/mqttsetting.cpp \
    mqtt/mqttSvc/mqttAPI.cpp \
    mqtt/subscribe/mqttsubscribe.cpp \
    mqtt/subscribe/mqttsubscribedelegate.cpp \
    networktool.cpp

HEADERS += \
    mongoose/mongoose.h \
    mqtt/mqttSvc/lib/inc/mqttsvc.h \
    mqtt/mqttSetting/mqttsetting.h \
    mqtt/mqttSvc/mqttAPI.h \
    mqtt/subscribe/mqttsubscribe.h \
    mqtt/subscribe/mqttsubscribedelegate.h \
    networktool.h

FORMS += \
    mqtt/mqttSetting/mqttsetting.ui \
    mqtt/subscribe/mqttsubscribe.ui \
    networktool.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


LIBS += -lpthread libwsock32 libws2_32
LIBS += -L$$PWD/mqtt/mqttSvc/lib/ -lmqttSvc


