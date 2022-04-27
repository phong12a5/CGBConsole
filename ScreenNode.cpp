#include "screennode.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

ScreenNode::ScreenNode(QObject *parent) : QObject(parent)
{

}

ScreenNode::~ScreenNode()
{

}

QString ScreenNode::className() const
{
    return m_className;
}

void ScreenNode::setClassName(const QString &className)
{
    m_className = className;
}

bool ScreenNode::checked() const
{
    return m_checked;
}

void ScreenNode::setChecked(bool checked)
{
    m_checked = checked;
}

bool ScreenNode::clickable() const
{
    return m_clickable;
}

void ScreenNode::setClickable(bool clickable)
{
    m_clickable = clickable;
}

QString ScreenNode::contentDescription() const
{
    return m_contentDescription;
}

void ScreenNode::setContentDescription(const QString &contentDescription)
{
    m_contentDescription = contentDescription;
}

QString ScreenNode::text() const
{
    return m_text;
}

void ScreenNode::setText(const QString &text)
{
    m_text = text;
}

bool ScreenNode::selected() const
{
    return m_selected;
}

void ScreenNode::setSelected(bool selected)
{
    m_selected = selected;
}

int ScreenNode::height() const
{
    return m_height;
}

void ScreenNode::setHeight(int height)
{
    m_height = height;
}

int ScreenNode::width() const
{
    return m_width;
}

void ScreenNode::setWidth(int width)
{
    m_width = width;
}

int ScreenNode::x() const
{
    return m_x;
}

void ScreenNode::setX(int x)
{
    m_x = x;
}

int ScreenNode::y() const
{
    return m_y;
}

void ScreenNode::setY(int y)
{
    m_y = y;
}

ScreenNode* ScreenNode::fromJson(QJsonObject obj)
{
    ScreenNode *node = new ScreenNode();
    if(obj.contains("checked")){
        node->setChecked(obj["checked"].toBool());
    }
    if(obj.contains("className")){
        node->setClassName(obj["className"].toString());
    }
    if(obj.contains("clickable")){
        node->setClickable(obj["clickable"].toBool());
    }
    if(obj.contains("contentDescription")){
        node->setContentDescription(obj["contentDescription"].toString());
    }
    if(obj.contains("height")){
        node->setHeight(obj["height"].toInt());
    }
    if(obj.contains("selected")){
        node->setSelected(obj["selected"].toBool());
    }
    if(obj.contains("text")){
        node->setText(obj["text"].toString());
    }
    if(obj.contains("width")){
        node->setWidth(obj["width"].toInt());
    }
    if(obj.contains("x")){
        node->setX(obj["x"].toInt());
    }
    if(obj.contains("y")){
        node->setY(obj["y"].toInt());
    }

    return node;
}

QJsonObject ScreenNode::toJson()
{
    QJsonObject obj;
    obj.insert("checked",m_checked);
    obj.insert("className",m_className);
    obj.insert("clickable",m_clickable);
    obj.insert("contentDescription",m_contentDescription);
    obj.insert("height",m_height);
    obj.insert("selected",m_selected);
    obj.insert("text",m_text);
    obj.insert("width",m_width);
    obj.insert("x",m_x);
    obj.insert("y",m_y);

    return  obj;
}

QJsonObject ScreenNode::toDefinedJson()
{
    QJsonObject obj;
    obj.insert("checked",m_checked);
    obj.insert("className",m_className);
    obj.insert("clickable",m_clickable);
    obj.insert("contentDescription",m_contentDescription);
    obj.insert("selected",m_selected);
    obj.insert("text",m_text);

    return obj;
}

bool ScreenNode::operator== (ScreenNode &other)
{
    if(other.checked() == m_checked
            && other.className() == m_className
            && other.clickable() == m_clickable
            && other.contentDescription() == m_contentDescription
            && other.selected() == m_selected
            && other.text() == m_text){
        return true;
    }
    return false;
}

bool ScreenNode::isEqual(ScreenNode &other)
{
    if(other.checked() == m_checked
            && other.className() == m_className
            && other.clickable() == m_clickable
            && other.contentDescription() == m_contentDescription
            && other.selected() == m_selected){
        if(other.className().contains("Edit")){
            return true;
        }else if(other.text() == m_text){
            return true;
        }
    }
    return false;
}
