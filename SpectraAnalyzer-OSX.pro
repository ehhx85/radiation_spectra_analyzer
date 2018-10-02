QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = "MAESTRO Spectra Analyzer"
TEMPLATE = app

QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.12

INCLUDEPATH += /opt/local/include/

#DEFINES += QT_DEPRECATED_WARNINGS

include(res/res.pri)
include(gui/gui.pri)
include(spectra/spectra.pri)

SOURCES += main.cpp
