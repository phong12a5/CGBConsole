#ifndef SCREENINFO_H
#define SCREENINFO_H

#include <QObject>
#include "screennode.h"

class ScreenInfo : public QObject
{
    Q_OBJECT
public:
    explicit ScreenInfo(QObject *parent = nullptr);
    ~ScreenInfo();

    bool isContains(ScreenInfo &screenInfo);
    int count();
    void addNode(ScreenNode *screenNode);
    static ScreenInfo* fromJson(QJsonArray arr);
    QJsonArray toJson();
    QJsonArray toDefinedJson();

signals:
private:
    QList<ScreenNode*> m_listNode;
};

#endif // SCREENINFO_H
