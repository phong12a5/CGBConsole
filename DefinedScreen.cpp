#include "DefinedScreen.h"

#include <QJsonArray>
#include <QJsonObject>

DefinedScreen::DefinedScreen(QObject *parent) : QObject(parent)
{
    m_listInfo = QList<ScreenInfo*>();
}

DefinedScreen::~DefinedScreen()
{
    for(int i=0;i<m_listInfo.size();i++){
        if(m_listInfo[i] != nullptr){
            delete m_listInfo[i];
        }
    }
    m_listInfo.clear();
}

QString DefinedScreen::screenId() const
{
    return m_screenId;
}

void DefinedScreen::setScreenId(const QString &screenId)
{
    m_screenId = screenId;
}

bool DefinedScreen::acceptScreen(ScreenInfo &rawScreen)
{
    for(int i=0;i<count();i++){
        if(rawScreen.isContains(*m_listInfo[i])){
            return true;
        }
    }

    return false;
}

void DefinedScreen::addInfo(ScreenInfo *screenInfo)
{
    m_listInfo.append(screenInfo);
}

DefinedScreen* DefinedScreen::fromJson(QJsonObject obj)
{
    DefinedScreen *defined = new DefinedScreen();
    if(obj.contains("screenId")){
        defined->setScreenId(obj["screenId"].toString());
        QJsonArray defineds = obj["elements"].toArray();
        for(int i=0;i<defineds.size();i++){
            defined->addInfo(ScreenInfo::fromJson(defineds[i].toArray()));
        }
    }

    return defined;

}

QJsonObject DefinedScreen::toJson()
{
    QJsonObject obj;
    obj.insert("screenId",m_screenId);
    QJsonArray listDefined;
    for(int i=0;i<m_listInfo.size();i++){
        listDefined.append(m_listInfo[i]->toDefinedJson());
    }
    obj.insert("elements",listDefined);
    return obj;
}

int DefinedScreen::count()
{
    return m_listInfo.count();
}
