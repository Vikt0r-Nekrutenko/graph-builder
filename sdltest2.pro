TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -L$$PWD/../../../../../SDL2-2.30.3/x86_64-w64-mingw32/lib -lSDL2

INCLUDEPATH += $$PWD/../../../../../SDL2-2.30.3/x86_64-w64-mingw32/include
DEPENDPATH += $$PWD/../../../../../SDL2-2.30.3/x86_64-w64-mingw32/include

SOURCES += \
        app.cpp \
        graph.cpp \
        main.cpp \
        rasterfont.cpp \
        vertex.cpp

HEADERS += \
    app.hpp \
    graph.hpp \
    rasterfont.hpp \
    vertex.hpp
