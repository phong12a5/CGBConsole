#ifndef DEFINEDSCREEN_H
#define DEFINEDSCREEN_H

#include <QObject>
#include "ScreenInfo.h"

class DefinedScreen : public QObject
{
    Q_OBJECT
public:
    explicit DefinedScreen(QObject *parent = nullptr);
    ~DefinedScreen();

    QString screenId() const;
    void setScreenId(const QString &screenId);

    bool acceptScreen(ScreenInfo &rawScreen);

    void addInfo(ScreenInfo *screenInfo);

    static DefinedScreen* fromJson(QJsonObject obj);
    QJsonObject toJson();

    int count();
signals:
private:
    QString m_screenId = "SCREEN_UNKNOWN";
    QList<ScreenInfo*> m_listInfo;
};

#endif // DEFINEDSCREEN_H
