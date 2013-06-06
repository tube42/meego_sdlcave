TEMPLATE = app
QT += core gui

TARGET = sdlcave

LIBS += -lSDLmain -lSDL

contains(MEEGO_EDITION,harmattan) {
    #   QT += opengl
    #    LIBS += -lasound -lpulse-simple -lpulse
    LIBS += -lSDL  -lpthread  -lm -ldl  -lasound -lm -ldl -lpthread -lpulse-simple -lpulse
    LIBS += -lX11 -lXext   -lXfixes -lpvr2d -lpthread -ldbus-1 -lpthread -lrt   -limmvibe
# -lXi
}

OTHER_FILES += \
    10x20.fnt \
    src/SDL_gfx.spec \
    sdlcave.desktop \
    README

SOURCES += src/*.c

HEADERS += src/*.h


contains(MEEGO_EDITION,harmattan) {
    target.path = /opt/sdlcave/bin
    INSTALLS += target

    font.files = *.fnt
    font.path = /opt/sdlcave/
    INSTALLS += font

    icon.files = sdlcave.png
    icon.path = /usr/share/icons/hicolor/80x80/apps
    INSTALLS += icon

    desktop.files = sdlcave.desktop
    desktop.path = /usr/share/applications
    INSTALLS += desktop

}








