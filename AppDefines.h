#ifndef APPDEFINES_H
#define APPDEFINES_H

#include <QDebug>
#include <QEventLoop>
#include <QTimer>

#define LOG qDebug() << "[" << __FUNCTION__ << "][" << __LINE__ << "] "

#define FARM_PACKAGE_NAME       "xyz.autofarmer.app"
#define CONFIG_FILE_NAME        "config.json"
#define ENDSCRIPT_PATH          "/sdcard/DCIM/"
#define ENDSCRIPT_FILENAME      "endScript.st"
#define INITSCRIPT_FILENAME     "/sdcard/DCIM/initApp.st"
#define APK_FILENAME            "xyz.autofarmer.app.%1.apk"
#define API_SERVER              "https://api.autofarmer.xyz/api/v2/"
#define EMULATOR_NAME_PREFIX    "CGBDevice"
#define ORIGIN_DEVICE_NAME      "OrgDevice"

#define DEFAULT_MAX_VM_THREAD   3
#define DEFAULT_MAX_VM_COUNT    20
#define DEFAULT_OPENAPK_TIME    20 // Unit seconds

#define INSTALL_FOLDER_PROP_KEY "installation_folder"
#define TOKEN_PROP_KEY          "farm_token"
#define APP_NAME_PROP_KEY       "appName"
#define DEVICE_COUNT_KEY        "device_count"
#define THREAD_COUNT_KEY        "thread_count"

#define APP_DATA_FOLDER         "/sdcard/xyz.autofarmer.app/"

typedef struct app_config_struct {
    int timeout;
    int reset_3g;
    QString debug_mode;
    int user_type;
    int m_mobileCloneCount;
    int m_maxVmCount;
    int m_maxVmThread;
    int m_ldCloneCount;
    int m_balance;
    int m_openApkAfterNSeconds;
    int m_android_versioncode;

    app_config_struct(){
        timeout = 30;
        reset_3g = 10;
        debug_mode = "test";
        user_type = 0;
        m_mobileCloneCount = 0;
        m_maxVmCount = DEFAULT_MAX_VM_COUNT;
        m_maxVmThread = DEFAULT_MAX_VM_THREAD;
        m_ldCloneCount = 0;
        m_balance = -1;
        m_openApkAfterNSeconds = DEFAULT_OPENAPK_TIME;
        m_android_versioncode = 8;
    }
} APP_CONFIG;

inline void delay(int ms){
    QEventLoop even;
    QTimer timer;
    timer.setInterval(ms);
    timer.setSingleShot(true);
    QObject::connect(&timer, SIGNAL(timeout()), &even, SLOT(quit()));
    timer.start();
    even.exec();
}

#endif // APPDEFINES_H
