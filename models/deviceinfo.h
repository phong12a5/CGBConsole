#ifndef DEVICEINFO_H
#define DEVICEINFO_H

#include <QJsonObject>
#include <QObject>

class DeviceInfo : public QObject
{
    Q_OBJECT
public:
    explicit DeviceInfo(QObject *parent = nullptr);

    QString modelName() const;
    void setModelName(const QString &modelName);

    QString manufacturer() const;
    void setManufacturer(const QString &manufacturer);

    QString imei() const;
    void setImei(const QString &imei);

    QString imsi() const;
    void setImsi(const QString &imsi);

    QString simserial() const;
    void setSimserial(const QString &simserial);

    QString androidId() const;
    void setAndroidId(const QString &androidId);

    QString phoneNumber() const;
    void setPhoneNumber(const QString &phoneNumber);

    QString mac() const;
    void setMac(const QString &mac);

    static DeviceInfo* fromJson(QJsonObject obj);

    QJsonObject toJson();

    QString resolution() const;
    void setResolution(const QString &resolution);

    int cpuCount() const;
    void setCpuCount(int cpuCount);

    int memory() const;
    void setMemory(int memory);

    QJsonObject xposeData() const;
    void setXposedData(const QJsonObject &xposeData);

    void updateGeneral(DeviceInfo &device);

signals:

private:
    QString m_resolution = "320,480,120";
    int m_cpuCount = 1;
    int m_memory = 1024;
    QString m_modelName = "";
    QString m_manufacturer = "";
    QString m_imei = "";
    QString m_imsi = "";
    QString m_simserial = "";
    QString m_androidId = "";
    QString m_phoneNumber = "";
    QString m_mac = "";
    QJsonObject m_xposedData = QJsonObject();
};

#endif // DEVICEINFO_H
