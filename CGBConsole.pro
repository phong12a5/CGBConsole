QT += gui \
      core \
      widgets \
      network

CONFIG += c++11

DEFINES += USE_FILE_STEADOF_ADB


# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS \
#            QT_NO_DEBUG_OUTPUT\
#            QT_NO_WARNING_OUTPUT

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ConfigHelper.cpp \
    DefinedScreen.cpp \
    LDPlayer.cpp \
    LDPlayerHelper.cpp \
    LDWorker.cpp \
    MainWindow.cpp \
    ScreenInfo.cpp \
    ScreenNode.cpp \
    Utility.cpp \
    main.cpp \
    AppMain.cpp \
    AppModel.cpp \
    AppController.cpp \
    LDCommand.cpp \
    PerformanceReader.cpp \
    LDService.cpp \
    models/asyncnetworkmanager.cpp \
    models/autofarmerapi.cpp \
    models/backuprestoremanager.cpp \
    models/backuprestorepackage.cpp \
    models/deviceinfo.cpp \
    models/facebookaccount.cpp \
    models/passwdgenerator.cpp \
    tasks/facebookaction.cpp \
    ui/informationtab.cpp

FORMS += \
    mainwindow.ui \
    ui/informationtab.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    AppMain.h \
    AppModel.h \
    AppDefines.h \
    AppController.h \
    ConfigHelper.h \
    DefinedScreen.h \
    LDCommand.h \
    LDPlayer.h \
    LDPlayerHelper.h \
    LDWorker.h \
    MainWindow.h \
    PerformanceReader.h \
    CPdhQuery.h \
    LDService.h \
    ScreenInfo.h \
    ScreenNode.h \
    ScreenReferences.h \
    Utility.h \
    models/asyncnetworkmanager.h \
    models/autofarmerapi.h \
    models/backuprestoremanager.h \
    models/backuprestorepackage.h \
    models/deviceinfo.h \
    models/facebookaccount.h \
    models/passwdgenerator.h \
    tasks/facebookaction.h \
    ui/informationtab.h

LIBS += -L$$PWD/libs/ -lchilkat-9.5.0 -lws2_32 -lcrypt32 -ldnsapi
LIBS += $$PWD/libs/Pdh.Lib
LIBS += -L$$PWD/libs -lQGoogleAuthenticator


INCLUDEPATH += $$PWD/include/chilkat_mingw/
INCLUDEPATH += $$PWD/include/googleauthenticator

RC_ICONS = Logo.ico

win32:CONFIG(release, debug|release): {
#    QMAKE_POST_LINK += mt -manifest $$PWD/manifest.xml -outputresource:$$OUT_PWD/release/$$TARGET".exe" $$escape_expand(\n\t)
    CONFIG += console
##    DEFINES += RELEASE_MODE
    DESTDIR = $$PWD/../CGBConsoleApp
}
else:win32:CONFIG(debug, debug|release): {
#    CONFIG += console
#    QMAKE_POST_LINK += mt -manifest $$PWD/manifest.xml -outputresource:$$OUT_PWD/debug/$$TARGET".exe" $$escape_expand(\n\t)
    DESTDIR = $$PWD/../CGBConsoleAppd
}

QMAKE_POST_LINK += $$quote(cp $${PWD}/screendefine.json $${DESTDIR} $$escape_expand(\n\t))

