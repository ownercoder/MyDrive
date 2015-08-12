APP_NAME = MyDrive

CONFIG += qt warn_on cascades10

QT += core network
LIBS += -lbbdata -lbbdevice -lbbsystem -lbbcascadespickers -lbb -lbbplatform

include(config.pri)
