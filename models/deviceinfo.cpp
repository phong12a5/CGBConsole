#include "deviceinfo.h"

#include <QJsonObject>

DeviceInfo::DeviceInfo(QObject *parent) : QObject(parent)
{

}

QString DeviceInfo::modelName() const
{
    return m_modelName;
}

void DeviceInfo::setModelName(const QString &modelName)
{
    m_modelName = modelName;
}

QString DeviceInfo::manufacturer() const
{
    return m_manufacturer;
}

void DeviceInfo::setManufacturer(const QString &manufactuer)
{
    m_manufacturer = manufactuer;
}

QString DeviceInfo::imei() const
{
    return m_imei;
}

void DeviceInfo::setImei(const QString &imei)
{
    m_imei = imei;
}

QString DeviceInfo::imsi() const
{
    return m_imsi;
}

void DeviceInfo::setImsi(const QString &imsi)
{
    m_imsi = imsi;
}

QString DeviceInfo::simserial() const
{
    return m_simserial;
}

void DeviceInfo::setSimserial(const QString &simserial)
{
    m_simserial = simserial;
}

QString DeviceInfo::androidId() const
{
    return m_androidId;
}

void DeviceInfo::setAndroidId(const QString &androidId)
{
    m_androidId = androidId;
}

QString DeviceInfo::phoneNumber() const
{
    return m_phoneNumber;
}

void DeviceInfo::setPhoneNumber(const QString &phoneNumber)
{
    m_phoneNumber = phoneNumber;
}

QString DeviceInfo::mac() const
{
    return m_mac;
}

void DeviceInfo::setMac(const QString &mac)
{
    m_mac = mac;
}

DeviceInfo* DeviceInfo::fromJson(QJsonObject obj)
{
    DeviceInfo *info = new DeviceInfo();
    if(obj.contains("xposedData")){
        info->setXposedData(obj["xposedData"].toObject());
    }
    if(obj.contains("resolution")){
        info->setResolution(obj["resolution"].toString());
    }
    if(obj.contains("cpuCount")){
        info->setCpuCount(obj["cpuCount"].toInt());
    }
    if(obj.contains("memory")){
        info->setMemory(obj["memory"].toInt());
    }
    if(obj.contains("modelName")){
        info->setModelName(obj["modelName"].toString());
    }
    if(obj.contains("manufacturer")){
        info->setManufacturer(obj["manufacturer"].toString());
    }
    if(obj.contains("imei")){
        info->setImei(obj["imei"].toString());
    }
    if(obj.contains("imsi")){
        info->setImsi(obj["imsi"].toString());
    }
    if(obj.contains("simserial")){
        info->setSimserial(obj["simserial"].toString());
    }
    if(obj.contains("androidId")){
        info->setAndroidId(obj["androidId"].toString());
    }
    if(obj.contains("phoneNumber")){
        info->setPhoneNumber(obj["phoneNumber"].toString());
    }
    if(obj.contains("mac")){
        info->setMac(obj["mac"].toString());
    }
    return info;
}

QJsonObject DeviceInfo::toJson()
{
    QJsonObject obj;
    obj.insert("resolution",m_resolution);
    obj.insert("cpuCount",m_cpuCount);
    obj.insert("memory",m_memory);
    obj.insert("modelName",m_modelName);
    obj.insert("manufacturer",m_manufacturer);
    obj.insert("imei",m_imei);
    obj.insert("imsi",m_imsi);
    obj.insert("simserial",m_simserial);
    obj.insert("androidId",m_androidId);
    obj.insert("phoneNumber",m_phoneNumber);
    obj.insert("mac",m_mac);
    obj.insert("xposedData",m_xposedData);
    return obj;
}

QString DeviceInfo::resolution() const
{
    return m_resolution;
}

void DeviceInfo::setResolution(const QString &resolution)
{
    m_resolution = resolution;
}

int DeviceInfo::cpuCount() const
{
    return m_cpuCount;
}

void DeviceInfo::setCpuCount(int cpuCount)
{
    m_cpuCount = cpuCount;
}

int DeviceInfo::memory() const
{
    return m_memory;
}

void DeviceInfo::setMemory(int memory)
{
    m_memory = memory;
}

QJsonObject DeviceInfo::xposeData() const
{
    return m_xposedData;
}

void DeviceInfo::setXposedData(const QJsonObject &xposedData)
{
    m_xposedData = xposedData;
}

void DeviceInfo::updateGeneral(DeviceInfo &device)
{
    m_resolution = device.resolution();
    m_cpuCount = device.cpuCount();
    m_memory = device.memory();
    m_modelName = device.modelName();
    m_manufacturer = device.manufacturer();
    m_imei = device.imei();
    m_imsi = device.imsi();
    m_simserial = device.simserial();
    m_androidId = device.androidId();
    m_phoneNumber = device.phoneNumber();
    m_mac = device.mac();
}
