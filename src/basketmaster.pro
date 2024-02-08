QT -= gui
QT += opengl
TARGET = BasketMaster_v.0.0.1
TEMPLATE = app
#CONFIG += release

#if you are using qt spesific things (e.g qdebug) this will be needed to use them.
DEFINES += QT_CREATOR

#Select your platform
DEFINES += WIN32
#DEFINES += __linux__
#DEFINES += __APPLE__
#DEFINES += __ANDROID__
#DEFINES += OPENGL_ES

#Enter your own downloaded gameplay folders directory
GAMEPLAY_PATH = "D:/android/GamePlay"
GAMEPLAY_SOURCES = "D:/android/GamePlay/gameplay/src"
GAMEPLAY_SOURCES_RELATIVE = "../../../gameplay/src/"

#common includepath
win32 {
    INCLUDEPATH += \
        #if you are using included gameplay sources
#        "GamePlay/" \
#        "GamePlay/lua" \
        "$$GAMEPLAY_SOURCES" \
        "$$GAMEPLAY_SOURCES/lua" \
        "C:/Program Files (x86)/Microsoft SDKs/Windows/v7.0A/Include" \
        "$$GAMEPLAY_PATH/external-deps/bullet/include" \
        "$$GAMEPLAY_PATH/external-deps/openal/include/AL" \
        "$$GAMEPLAY_PATH/external-deps/glew/include" \
        "$$GAMEPLAY_PATH/external-deps/oggvorbis/include" \
        "$$GAMEPLAY_PATH/external-deps/libpng/include" \
        "$$GAMEPLAY_PATH/external-deps/lua/include" \
        "$$GAMEPLAY_PATH/external-deps/zlib/include" \
        "$$GAMEPLAY_PATH/external-deps/detour/include" \
        "$$GAMEPLAY_PATH/external-deps/detourcrowd/include" \
        "$$GAMEPLAY_PATH/external-deps/freetype2/include" \
        "$$GAMEPLAY_PATH/external-deps/pcre/include" \
        "$$GAMEPLAY_PATH/external-deps/recast/include" \
        "$$GAMEPLAY_PATH/external-deps/tinyxml2/include" \
}

#include x86 libs
win32 {
    LIBS += \
        -L"C:/Program Files (x86)/Microsoft SDKs/Windows/v7.0A/Lib" \
        -L"$$GAMEPLAY_PATH/external-deps/bullet/lib/windows/x86" \
        -L"$$GAMEPLAY_PATH/gameplay/windows/x86/Release" \
        -L"$$GAMEPLAY_PATH/external-deps/openal/lib/windows/x86" \
        -L"$$GAMEPLAY_PATH/external-deps/glew/lib/windows/x86" \
        -L"$$GAMEPLAY_PATH/external-deps/oggvorbis/lib/windows/x86" \
        -L"$$GAMEPLAY_PATH/external-deps/libpng/lib/windows/x86" \
        -L"$$GAMEPLAY_PATH/external-deps/detour/lib/windows/x86" \
        -L"$$GAMEPLAY_PATH/external-deps/detourcrowd/lib/windows/x86" \
        -L"$$GAMEPLAY_PATH/external-deps/freetype2/lib/windows/x86" \
        -L"$$GAMEPLAY_PATH/external-deps/pcre/lib/windows/x86" \
        -L"$$GAMEPLAY_PATH/external-deps/recast/lib/windows/x86" \
        -L"$$GAMEPLAY_PATH/external-deps/tinyxml2/lib/windows/x86" \
        -L"$$GAMEPLAY_PATH/external-deps/zlib/lib/windows/x86" \
        -L"$$GAMEPLAY_PATH/external-deps/lua/lib/windows/x86" \
        -lkernel32 \
        -luser32 \
        -lshell32 \
        -lgameplay \
        -lglew32 \
        -lopenal32 \
        -lbulletcollision \
        -lbulletdynamics \
        -llinearmath \
        -ldetour \
        -ldetourcrowd \
        -lfreetype245 \
        -llibpng14 \
        -llua \
        -llibogg \
        -llibvorbis \
        -llibvorbisfile \
        -lrecast \
        -ltinyxml2 \
        -lzlib \
#        -lcollada14dom \
#        -lfreetype \
#        -lgl \
#        -lz \
}

HEADERS += \
    controller.h \
    menu.h \
    gamebase.h \
    gamescene.h \
    gp_types.h \
    maingame.h

SOURCES += \
    controller.cpp \
    menu.cpp \
    gamebase.cpp \
    gamescene.cpp \
    maingame.cpp
