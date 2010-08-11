TEMPLATE = app
DESTDIR = bin
OBJECTS_DIR = obj
DEPENDPATH += . src src/item
INCLUDEPATH += . src src/item

CONFIG += debug windows qt
QT = core

LIBS += -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf

unix {
	QMAKE_CXXFLAGS += `sdl-config --cflags`
	QMAKE_CXXFLAGS += -Wall -W
	QMAKE_CFLAGS += `sdl-config --cflags`
	QMAKE_CFLAGS += -Wall -W
}

HEADERS += src/*.h src/item/*.h
SOURCES += src/*.cpp src/*.c src/item/*.cpp

