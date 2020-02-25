#-------------------------------------------------
#
# Project created by QtCreator 2018-12-23T10:43:06
#
#-------------------------------------------------

VERSION = 0.1.0

# Check Qt version.
requires(greaterThan(QT_MAJOR_VERSION, 4))
equals(QT_MAJOR_VERSION, 4) {
    requires(greaterThan(QT_MINOR_VERSION, 5))
    smallerThan(QT_MINOR_VERSION, 9):message("Using Qt version < 5.9 is untested!.")
}

QT += core gui multimedia multimediawidgets xml widgets


TARGET = beamerpresenter
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# Define the application version.
#DEFINES += APP_VERSION=\\\"$${VERSION}\\\"
# Set git version for more precise version info.
DEFINES += APP_VERSION="\\\"$${VERSION}-$(shell git -C \""$$_PRO_FILE_PWD_"\" rev-list --count HEAD ).$(shell git -C \""$$_PRO_FILE_PWD_"\" rev-parse --short HEAD )\\\""

# If the following define is uncommented, BeamerPresenter will check whether a compatible QPA platform is used.
# It will then emit warning on untested systems and try to avoid blocking you window manager.
# If this is commented out, all checks will be omitted.
DEFINES += CHECK_QPA_PLATFORM

# Define a path where the icon will be placed (don't forget the trailing /).
ICON_PATH = "/usr/share/icons/hicolor/scalable/apps/"
DEFINES += ICON_PATH=\\\"$${ICON_PATH}\\\"

CONFIG += c++14 qt
unix {
    # Enable better debugging.
    CONFIG(debug):QMAKE_LFLAGS += -rdynamic
    # Enable embedded applications. This allows for X-embedding of external applications if running in X11.
    DEFINES += EMBEDDED_APPLICATIONS_ENABLED
}
linux {
    # Drop down menus can cause problems in wayland.
    # This defines activates a patch to replace drop down menus in wayland.
    # With wayland 1.18.0 and Qt 5.14.1 the issue is mostly fixed.
    #DEFINES += USE_WAYLAND_SUBMENU_PATCH
    # TODO: drop down menues only partially visible in Wayland.
}

# Disable debugging message if debugging mode is disabled.
CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT

SOURCES += \
        src/main.cpp \
        src/pdf/pdfdoc.cpp \
        src/pdf/externalrenderer.cpp \
        src/pdf/basicrenderer.cpp \
        src/pdf/singlerenderer.cpp \
        src/pdf/cachemap.cpp \
        src/pdf/cachethread.cpp \
        src/screens/controlscreen.cpp \
        src/screens/presentationscreen.cpp \
        src/slide/previewslide.cpp \
        src/slide/mediaslide.cpp \
        src/slide/drawslide.cpp \
        src/slide/presentationslide.cpp \
        src/draw/pathoverlay.cpp \
        src/draw/drawpath.cpp \
        src/gui/timer.cpp \
        src/gui/pagenumberedit.cpp \
        src/gui/toolbutton.cpp \
        src/gui/toolselector.cpp \
        src/gui/tocbox.cpp \
        src/gui/tocbutton.cpp \
        src/gui/tocaction.cpp \
        src/gui/overviewframe.cpp \
        src/gui/overviewbox.cpp \
        src/slide/media/videowidget.cpp

HEADERS += \
        src/enumerates.h \
        src/names.h \
        src/pdf/pdfdoc.h \
        src/pdf/externalrenderer.h \
        src/pdf/basicrenderer.h \
        src/pdf/singlerenderer.h \
        src/pdf/cachemap.h \
        src/pdf/cachethread.h \
        src/screens/controlscreen.h \
        src/screens/presentationscreen.h \
        src/slide/previewslide.h \
        src/slide/mediaslide.h \
        src/slide/drawslide.h \
        src/slide/presentationslide.h \
        src/draw/pathoverlay.h \
        src/draw/drawpath.h \
        src/gui/timer.h \
        src/gui/pagenumberedit.h \
        src/gui/toolbutton.h \
        src/gui/toolselector.h \
        src/gui/tocbox.h \
        src/gui/tocbutton.h \
        src/gui/tocaction.h \
        src/gui/overviewframe.h \
        src/gui/overviewbox.h \
        src/slide/media/videowidget.h

contains(DEFINES, EMBEDDED_APPLICATIONS_ENABLED) {
    SOURCES += src/slide/media/embedapp.cpp
    HEADERS += src/slide/media/embedapp.h
}

FORMS += \
        src/ui/controlscreen.ui

unix {
    INCLUDEPATH += /usr/include/poppler/qt5
    LIBS += -L /usr/lib/ -lpoppler-qt5
}
macx {
    ## Please configure this according to your poppler installation.
    ## Installation on Mac is untested.
    #INCLUDEPATH += ...
    #LIBS += ...
}
win32 {
    ## Please configure this according to your poppler installation.
    ## The configuration will probably have the following form:
    #INCLUDEPATH += C:\...\poppler-0.??.?-win??
    #LIBS += -LC:\...\poppler-0.??.?-win?? -lpoppler-qt5
}

unix {
    # Commands needed for make install
    # Include man pages and default configuration in make install.

    man1.path = /usr/share/man/man1/
    man1.extra = gzip -9 man/$${TARGET}.1 || true
    man1.CONFIG = no_check_exists
    man1.files = man/$${TARGET}.1.gz

    man5.path = /usr/share/man/man5/
    man5.extra = gzip -9 man/$${TARGET}.conf.5 || true
    man5.CONFIG = no_check_exists
    man5.files = man/$${TARGET}.conf.5.gz

    configuration.path = /etc/$${TARGET}/
    configuration.files = config/$${TARGET}.conf config/pid2wid.sh

    icon.path = $${ICON_PATH}
    icon.files = src/icons/beamerpresenter.svg

    target.path = /usr/bin/

    INSTALLS += man1 man5 configuration icon target
}
