#include "ConfigHelper.h"

#include <QMessageBox>
#include <QDebug>

int ConfigHelper::releaseVersion = 43;

QString ConfigHelper::configPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)+"\\RegClone\\config.json";

QString ConfigHelper::configKey = "Config";
QString ConfigHelper::configVersion = "version";
int ConfigHelper::currentVersion = 10;
QString ConfigHelper::numThreadKey = "number_thread";
QString ConfigHelper::ldPlayerLibsKey ="ld_player_path";
QString ConfigHelper::accountPathKey = "account_path";
QString ConfigHelper::facebookApkPathKey = "facebook_apk_path";
QString ConfigHelper::adbKeyboardApkPathKey = "adb_keyboard_apk_path";
QString ConfigHelper::screenLogPathKey = "screen_log_path";
QString ConfigHelper::packagePathKey = "package_path";
QString ConfigHelper::autoStartKey = "auto_start";
QString ConfigHelper::vipLikePostModeKey = "vip_like_post_mode";
QString ConfigHelper::regCloneModeKey = "reg_clone_mode";
QString ConfigHelper::buffLikeModeKey = "buff_like_mode";
QString ConfigHelper::buffCommentModeKey = "buff_comment_mode";
QString ConfigHelper::likePageModeKey = "like_page_mode";
QString ConfigHelper::followModeKey = "follow_mode";


ConfigHelper::ConfigHelper(QObject *parent) : QObject(parent)
{

}

//setters
void ConfigHelper::setLDPlayerLibsPath(QString path)
{
    QString key = ldPlayerLibsKey;
    QJsonObject config = getConfig();
    QJsonObject object = config[configKey].toObject();
    QJsonObject contentCopy;
    for (auto it = object.begin(), end=object.end(); it != end; ++it)
    {
        if(it.key() != key){
            contentCopy.insert(it.key(),it.value());
        }else {
            contentCopy.insert(key,path);
        }
    }
    qDebug()<<contentCopy;
    QJsonObject newConfig;
    newConfig.insert(configKey,contentCopy);
    qDebug()<<newConfig;
    updateConfig(newConfig);
}

void ConfigHelper::setNumThread(int numThread)
{
    QString key = numThreadKey;
    QJsonObject config = getConfig();
    QJsonObject object = config[configKey].toObject();
    QJsonObject contentCopy;
    for (auto it = object.begin(), end=object.end(); it != end; ++it)
    {
        if(it.key() != key){
            contentCopy.insert(it.key(),it.value());
        }else {
            contentCopy.insert(key,numThread);
        }
    }
    QJsonObject newConfig;
    newConfig.insert(configKey,contentCopy);
    updateConfig(newConfig);
}

void ConfigHelper::setAccountPath(QString path)
{
    QString key = accountPathKey;
    QJsonObject config = getConfig();
    QJsonObject object = config[configKey].toObject();
    QJsonObject contentCopy;
    for (auto it = object.begin(), end=object.end(); it != end; ++it)
    {
        if(it.key() != key){
            contentCopy.insert(it.key(),it.value());
        }else {
            contentCopy.insert(key,path);
        }
    }
    QJsonObject newConfig;
    newConfig.insert(configKey,contentCopy);
    updateConfig(newConfig);
}

void ConfigHelper::setFacebookApkPath(QString apkPath)
{
    QString key = facebookApkPathKey;
    QJsonObject config = getConfig();
    QJsonObject object = config[configKey].toObject();
    QJsonObject contentCopy;
    for (auto it = object.begin(), end=object.end(); it != end; ++it)
    {
        if(it.key() != key){
            contentCopy.insert(it.key(),it.value());
        }else {
            contentCopy.insert(key,apkPath);
        }
    }
    QJsonObject newConfig;
    newConfig.insert(configKey,contentCopy);
    updateConfig(newConfig);
}

void ConfigHelper::setAdbKeyboardApkPath(QString apkPath)
{
    QString key = adbKeyboardApkPathKey;
    QJsonObject config = getConfig();
    QJsonObject object = config[configKey].toObject();
    QJsonObject contentCopy;
    for (auto it = object.begin(), end=object.end(); it != end; ++it)
    {
        if(it.key() != key){
            contentCopy.insert(it.key(),it.value());
        }else {
            contentCopy.insert(key,apkPath);
        }
    }
    QJsonObject newConfig;
    newConfig.insert(configKey,contentCopy);
    updateConfig(newConfig);
}

void ConfigHelper::setScreenLogPath(QString screenLogPath)
{
    QString key = screenLogPathKey;
    QJsonObject config = getConfig();
    QJsonObject object = config[configKey].toObject();
    QJsonObject contentCopy;
    for (auto it = object.begin(), end=object.end(); it != end; ++it)
    {
        if(it.key() != key){
            contentCopy.insert(it.key(),it.value());
        }else {
            contentCopy.insert(key,screenLogPath);
        }
    }
    QJsonObject newConfig;
    newConfig.insert(configKey,contentCopy);
    updateConfig(newConfig);
}

void ConfigHelper::setPackagePath(QString packagePath)
{
    QString key = packagePathKey;
    QJsonObject config = getConfig();
    QJsonObject object = config[configKey].toObject();
    QJsonObject contentCopy;
    for (auto it = object.begin(), end=object.end(); it != end; ++it)
    {
        if(it.key() != key){
            contentCopy.insert(it.key(),it.value());
        }else {
            contentCopy.insert(key,packagePath);
        }
    }
    QJsonObject newConfig;
    newConfig.insert(configKey,contentCopy);
    updateConfig(newConfig);
}

void ConfigHelper::setAutoStart(bool isAutoStart)
{
    QString key = autoStartKey;
    QJsonObject config = getConfig();
    QJsonObject object = config[configKey].toObject();
    QJsonObject contentCopy;
    for (auto it = object.begin(), end=object.end(); it != end; ++it)
    {
        if(it.key() != key){
            contentCopy.insert(it.key(),it.value());
        }else {
            contentCopy.insert(key,isAutoStart);
        }
    }
    QJsonObject newConfig;
    newConfig.insert(configKey,contentCopy);
    updateConfig(newConfig);
}

void ConfigHelper::setVipLikePost(bool isVipLikePost)
{
    QString key = vipLikePostModeKey;
    QJsonObject config = getConfig();
    QJsonObject object = config[configKey].toObject();
    QJsonObject contentCopy;
    for (auto it = object.begin(), end=object.end(); it != end; ++it)
    {
        if(it.key() != key){
            contentCopy.insert(it.key(),it.value());
        }else {
            contentCopy.insert(key,isVipLikePost);
        }
    }
    QJsonObject newConfig;
    newConfig.insert(configKey,contentCopy);
    updateConfig(newConfig);
}

void ConfigHelper::setRegClone(bool isRegClone)
{
    QString key = regCloneModeKey;
    QJsonObject config = getConfig();
    QJsonObject object = config[configKey].toObject();
    QJsonObject contentCopy;
    for (auto it = object.begin(), end=object.end(); it != end; ++it)
    {
        if(it.key() != key){
            contentCopy.insert(it.key(),it.value());
        }else {
            contentCopy.insert(key,isRegClone);
        }
    }
    QJsonObject newConfig;
    newConfig.insert(configKey,contentCopy);
    updateConfig(newConfig);
}

void ConfigHelper::setBuffLike(bool isBuffLike)
{
    QString key = buffLikeModeKey;
    QJsonObject config = getConfig();
    QJsonObject object = config[configKey].toObject();
    QJsonObject contentCopy;
    for (auto it = object.begin(), end=object.end(); it != end; ++it)
    {
        if(it.key() != key){
            contentCopy.insert(it.key(),it.value());
        }else {
            contentCopy.insert(key,isBuffLike);
        }
    }
    QJsonObject newConfig;
    newConfig.insert(configKey,contentCopy);
    updateConfig(newConfig);
}

void ConfigHelper::setBuffComment(bool isBuffComment)
{
    QString key = buffCommentModeKey;
    QJsonObject config = getConfig();
    QJsonObject object = config[configKey].toObject();
    QJsonObject contentCopy;
    for (auto it = object.begin(), end=object.end(); it != end; ++it)
    {
        if(it.key() != key){
            contentCopy.insert(it.key(),it.value());
        }else {
            contentCopy.insert(key,isBuffComment);
        }
    }
    QJsonObject newConfig;
    newConfig.insert(configKey,contentCopy);
    updateConfig(newConfig);
}


void ConfigHelper::setLikePage(bool isLikePage)
{
    QString key = likePageModeKey;
    QJsonObject config = getConfig();
    QJsonObject object = config[configKey].toObject();
    QJsonObject contentCopy;
    for (auto it = object.begin(), end=object.end(); it != end; ++it)
    {
        if(it.key() != key){
            contentCopy.insert(it.key(),it.value());
        }else {
            contentCopy.insert(key,isLikePage);
        }
    }
    QJsonObject newConfig;
    newConfig.insert(configKey,contentCopy);
    updateConfig(newConfig);
}

void ConfigHelper::setFollow(bool isFollow)
{
    QString key = followModeKey;
    QJsonObject config = getConfig();
    QJsonObject object = config[configKey].toObject();
    QJsonObject contentCopy;
    for (auto it = object.begin(), end=object.end(); it != end; ++it)
    {
        if(it.key() != key){
            contentCopy.insert(it.key(),it.value());
        }else {
            contentCopy.insert(key,isFollow);
        }
    }
    QJsonObject newConfig;
    newConfig.insert(configKey,contentCopy);
    updateConfig(newConfig);
}

//getters
int ConfigHelper::getReleaseVersion()
{
    return releaseVersion;
}

QString ConfigHelper::GetLDPlayerLibsPath()
{
    return getConfig()[configKey].toObject()[ldPlayerLibsKey].toString();
}

int ConfigHelper::getNumThread()
{
    return getConfig()[configKey].toObject()[numThreadKey].toInt();
}

QString ConfigHelper::getAccountPath()
{
    return getConfig()[configKey].toObject()[accountPathKey].toString();
}

int ConfigHelper::getConfigVersion()
{
    if(getConfig()[configKey].toObject()[configVersion].isNull()){
        qDebug()<<"Version is null";
        return 0;
    }
    return getConfig()[configKey].toObject()[configVersion].toInt();
}

QString ConfigHelper::getFacebookApkPath()
{
    return getConfig()[configKey].toObject()[facebookApkPathKey].toString();
}

QString ConfigHelper::getAdbKeyboardApkPath()
{
    return getConfig()[configKey].toObject()[adbKeyboardApkPathKey].toString();
}

QString ConfigHelper::getScreenLogPath()
{
    return getConfig()[configKey].toObject()[screenLogPathKey].toString();
}

QString ConfigHelper::getPackagePath()
{
    return getConfig()[configKey].toObject()[packagePathKey].toString();
}

bool ConfigHelper::isAutoStart()
{
    return getConfig()[configKey].toObject()[autoStartKey].toBool();
}

bool ConfigHelper::isVipLikePost()
{
    return getConfig()[configKey].toObject()[vipLikePostModeKey].toBool();
}

bool ConfigHelper::isRegClone()
{
    return getConfig()[configKey].toObject()[regCloneModeKey].toBool();
}

bool ConfigHelper::isBuffLike()
{
    return getConfig()[configKey].toObject()[buffLikeModeKey].toBool();
}

bool ConfigHelper::isBuffComment()
{
    return getConfig()[configKey].toObject()[buffCommentModeKey].toBool();
}

bool ConfigHelper::isLikePage()
{
    return getConfig()[configKey].toObject()[likePageModeKey].toBool();
}

bool ConfigHelper::isFollow()
{
    return getConfig()[configKey].toObject()[followModeKey].toBool();
}

void ConfigHelper::initConfig()
{
    configPath = Utility::getExecutedPath()+"/config.json";
    QJsonObject content;
    if(QFile(configPath).exists() && getConfigVersion() == currentVersion){
        return;
    }else if(QFile(configPath).exists()){
        content = getConfig()[configKey].toObject();
    }

    if(content.contains(configVersion)){
        content.remove(configVersion);
    }
    content.insert(configVersion,currentVersion);

    if(!content.contains(numThreadKey)){
        content.insert(numThreadKey,1);
    }

    if(!content.contains(ldPlayerLibsKey)){
        content.insert(ldPlayerLibsKey,QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
    }

    if(!content.contains(accountPathKey)){
        content.insert(accountPathKey,"");
    }

    if(!content.contains(facebookApkPathKey)){
        content.insert(facebookApkPathKey,Utility::getExecutedPath()+"/app/facebook.apk");
    }

    if(!content.contains(adbKeyboardApkPathKey)){
        content.insert(adbKeyboardApkPathKey,Utility::getExecutedPath()+"/app/ADBKeyboard.apk");
    }

    if(!content.contains(screenLogPathKey)){
        content.insert(screenLogPathKey,QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)+"\\RegClone\\screenLog.txt");
    }

    if(!content.contains(packagePathKey)){
        content.insert(packagePathKey,Utility::getExecutedPath()+"/packages/package.ifo");
    }

    QString packageDir = content[packagePathKey].toString().mid(0,content[packagePathKey].toString().size()-12);

    if(!QDir(packageDir).exists()){
        QDir().mkdir(packageDir);
    }

    if(!content.contains(autoStartKey)){
        content.insert(autoStartKey,true);
    }

    if(!content.contains(vipLikePostModeKey)){
        content.insert(vipLikePostModeKey,true);
    }

    if(!content.contains(regCloneModeKey)){
        content.insert(regCloneModeKey,false);
    }

    if(!content.contains(buffLikeModeKey)){
        content.insert(buffLikeModeKey,false);
    }

    if(!content.contains(buffCommentModeKey)){
        content.insert(buffCommentModeKey,false);
    }

    if(!content.contains(likePageModeKey)){
        content.insert(likePageModeKey,false);
    }

    if(!content.contains(followModeKey)){
        content.insert(followModeKey,false);
    }

    foreach(QString key,content.keys()){
        if(key != configVersion
                && key != numThreadKey
                && key != ldPlayerLibsKey
                && key != accountPathKey
                && key != facebookApkPathKey
                && key != adbKeyboardApkPathKey
                && key != screenLogPathKey
                && key != packagePathKey
                && key != autoStartKey
                && key != vipLikePostModeKey
                && key != regCloneModeKey
                && key != buffLikeModeKey
                && key != buffCommentModeKey
                && key != likePageModeKey
                && key != followModeKey){
            content.remove(key);
        }
    }

//    qDebug()<<content;

    QJsonObject object;
    object.insert("Config",content);

//    qDebug()<<object;
    updateConfig(object);
}

//private
QJsonObject ConfigHelper::getConfig()
{
    QString txtContent = Utility::read(configPath);
    return Utility::ObjectFromString(txtContent);
}

void ConfigHelper::updateConfig(QJsonObject object)
{
    Utility::write(configPath,QJsonDocument(object).toJson());
}
