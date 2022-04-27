#include "screeninfo.h"

#include <QJsonArray>
#include <QJsonObject>

ScreenInfo::ScreenInfo(QObject *parent) : QObject(parent)
{
    m_listNode = QList<ScreenNode*>();
}

ScreenInfo::~ScreenInfo()
{
    for(int i=0;i<m_listNode.size();i++){
        if(m_listNode[i] != nullptr){
            delete m_listNode[i];
        }
    }
    m_listNode.clear();
}

bool ScreenInfo::isContains(ScreenInfo &screenInfo)
{
    int foundCount = 0;
    for(int oth=0;oth<screenInfo.count();oth++){
        for(int i=0;i<count();i++){
            if(m_listNode[i]->isEqual(*screenInfo.m_listNode[oth])){
                foundCount++;
                break;
            }
        }
    }
    return foundCount==screenInfo.count();
}

void ScreenInfo::addNode(ScreenNode *screenNode)
{
    m_listNode.append(screenNode);
}

int ScreenInfo::count()
{
    return m_listNode.size();
}

ScreenInfo* ScreenInfo::fromJson(QJsonArray arr)
{
    ScreenInfo *info = new ScreenInfo();
    for(int i=0;i<arr.size();i++){
        info->addNode(ScreenNode::fromJson(arr[i].toObject()));
    }
    return info;
}

QJsonArray ScreenInfo::toJson()
{
    QJsonArray arr;
    for(int i=0;i<m_listNode.size();i++){
        arr.append(m_listNode[i]->toJson());
    }
    return arr;
}

QJsonArray ScreenInfo::toDefinedJson()
{
    QJsonArray arr;
    for(int i=0;i<m_listNode.size();i++){
        arr.append(m_listNode[i]->toDefinedJson());
    }
    return arr;
}
