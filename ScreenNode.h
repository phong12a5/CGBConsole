#ifndef SCREENNODE_H
#define SCREENNODE_H

#include <QObject>

class ScreenNode : public QObject
{
    Q_OBJECT
public:
    explicit ScreenNode(QObject *parent = nullptr);
    ~ScreenNode();

    QString className() const;
    void setClassName(const QString &className);

    bool checked() const;
    void setChecked(bool checked);

    bool clickable() const;
    void setClickable(bool clickable);

    QString contentDescription() const;
    void setContentDescription(const QString &contentDescription);

    QString text() const;
    void setText(const QString &text);

    bool selected() const;
    void setSelected(bool selected);

    int height() const;
    void setHeight(int height);

    int width() const;
    void setWidth(int width);

    int x() const;
    void setX(int x);

    int y() const;
    void setY(int y);

    static ScreenNode* fromJson(QJsonObject arr);
    QJsonObject toJson();
    QJsonObject toDefinedJson();

    bool operator== (ScreenNode &other);
    bool isEqual(ScreenNode &other);

signals:
private:
    QString m_className = "";
    bool m_checked = false;
    bool m_clickable = false;
    QString m_contentDescription = "";
    QString m_text = "";
    bool m_selected = false;
    int m_height = 0;
    int m_width = 0;
    int m_x = 0;
    int m_y = 0;

};

#endif // SCREENNODE_H
