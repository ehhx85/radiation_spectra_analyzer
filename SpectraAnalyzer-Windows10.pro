QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = "MAESTRO Spectra Analysis Tool"
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

include(res/res.pri)
include(gui/gui.pri)
include(spectra/spectra.pri)

SOURCES += main.cpp
