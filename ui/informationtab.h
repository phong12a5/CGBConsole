#ifndef INFORMATIONTAB_H
#define INFORMATIONTAB_H

#include <QWidget>

namespace Ui {
class InformationTab;
}

class InformationTab : public QWidget
{
    Q_OBJECT

public:
    explicit InformationTab(QWidget *parent = nullptr);
    ~InformationTab();

private:
    Ui::InformationTab *ui;
};

#endif // INFORMATIONTAB_H
