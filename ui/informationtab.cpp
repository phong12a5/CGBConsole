#include "informationtab.h"
#include "ui_informationtab.h"

InformationTab::InformationTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InformationTab)
{
    ui->setupUi(this);
}

InformationTab::~InformationTab()
{
    delete ui;
}
