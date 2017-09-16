#This file is part of kptc and was created by Sebastian Martin Dicke in 2017

QT += core gui widgets

QMAKE_CXXFLAGS += -std=c++14
CONFIG += release
TARGET = kptc
TEMPLATE = app

DISTFILES += \
	src/kptc.desktop \
	src/lo16-app-kptc.png \
	src/lo32-app-kptc.png \
	src/mini-kptc.xpm

HEADERS += \
	src/commanddialog.h \
	src/configdata.h \
	src/configdialog.h \
	src/configmachine.h \
	src/cqdialog.h \
	src/cwspeedwidget.h \
	src/fixtextpicker.h \
	src/kptc.h \
	src/modebuttons.h \
	src/modecommander.h \
	src/modem.h \
	src/mylineedit.h \
	src/rttyspeedwidget.h \
	src/statusinfo.h \
	src/update.h \
	src/updatedialog.h \
	src/kled.h

SOURCES += \
	src/commanddialog.cpp \
	src/configdata.cpp \
	src/configdialog.cpp \
	src/configmachine.cpp \
	src/cqdialog.cpp \
	src/cwspeedwidget.cpp \
	src/fixtextpicker.cpp \
	src/kptc.cpp \
	src/main.cpp \
	src/modebuttons.cpp \
	src/modecommander.cpp \
	src/modem.cpp \
	src/mylineedit.cpp \
	src/rttyspeedwidget.cpp \
	src/statusinfo.cpp \
	src/update.cpp \
	src/updatedialog.cpp \
	src/kled.cpp

