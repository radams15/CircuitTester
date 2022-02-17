CONFIG += qt debug c++98

CONFIG:macos += x86_64 x86 ppc

QT += gui

QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.4

QMAKE_LIBFLAGS += -lstdc++

INCLUDEPATH += lib/eigen
HEADERS += src/main/Analysis/*.h
SOURCES += src/main/Analysis/*.cc
HEADERS += src/main/Saves/*.h
SOURCES += src/main/Saves/*.cc src/main/Saves/*.cpp
HEADERS += src/main/UI/*.h
SOURCES += src/main/UI/*.cc
HEADERS += src/main/UI/Components/*.h
SOURCES += src/main/UI/Components/*.cc
SOURCES += src/main/main.cc

RESOURCES += resources/resources.qrc

QMAKE_INFO_PLIST = resources/Info.plist
ICON = resources/CircuitTester.icns
