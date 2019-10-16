QT += quick \
      widgets \
#      testlib

CONFIG += c++11
#DEFINES += TEST_MODE


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
    main.cpp \
    CopyEmulatorWorker.cpp \
    AppMain.cpp \
    AppModel.cpp \
    AppController.cpp \
    LDCommand.cpp \
    LDIntance.cpp \
    LDThread.cpp \
    LDRunner.cpp \
    WebAPI.cpp \
    QAESEncryption.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    AppMain.h \
    AppModel.h \
    AppDefines.h \
    AppController.h \
    CopyEmulatorWorker.h \
    LDCommand.h \
    LDIntance.h \
    LDThread.h \
    LDRunner.h \
    WebAPI.hpp \
    QAESEncryption.hpp

LIBS += -L"$$PWD" \
        -lssl32 \
        -lssl-1_1 \
        -lcrypto-1_1 \
        -leay32

RC_ICONS = Logo.ico
