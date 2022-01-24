CONFIG += qt debug c++98

INCLUDEPATH += lib/eigen
HEADERS += src/main/Analysis/*.h
SOURCES += src/main/Analysis/*.cc
HEADERS += src/main/Saves/*.h
SOURCES += src/main/Saves/*.cc
HEADERS += src/main/UI/*.h
SOURCES += src/main/UI/*.cc
HEADERS += src/main/UI/Components/*.h
SOURCES += src/main/UI/Components/*.cc
SOURCES += src/main/main.cc