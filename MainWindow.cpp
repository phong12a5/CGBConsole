#include "MainWindow.h"
#include <QWindow>
#include "ui_mainwindow.h"
#include "AppDefines.h"
#include "AppMain.h"
#include <AppModel.h>
#include <QTabWidget>
#include <QTableWidgetItem>
#include <QVBoxLayout>
#include <QGridLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->startBtn->setCheckable(true);
    ui->startBtn->setText("Start");

    connectSignalSlots();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectSignalSlots()
{
    connect(ui->startBtn, &QPushButton::clicked, this, &MainWindow::onStartClicked);

    connect(AppModel::instance(), &AppModel::devicesWidgetMapChanged, this, &MainWindow::onUpdateDevicesView);
}

void MainWindow::onStartClicked()
{
    LOGD("");
    if(ui->startBtn->isChecked()) {
        ui->startBtn->setText("Stop");
        AppModel::instance()->setAppStarted(true);
        AppController::instance()->startLDPlayers();
    } else {
        AppModel::instance()->setAppStarted(false);
        AppController::instance()->stopLDPlayers();
        ui->startBtn->setText("Start");
    }
}

void MainWindow::onUpdateDevicesView()
{
    LOGD("");
    QMap<int, QWidget*> map = AppModel::instance()->devicesWidgetMap();
    foreach(int ldInsId, map.keys()) {
        QWidget* widget = map.value(ldInsId);
        if(ui->gridLayout->indexOf(widget) < 0) {
            ui->gridLayout->addWidget(widget, ldInsId/COL_DEVICE_VIEW, ldInsId%COL_DEVICE_VIEW, 1, 1);
        }
    }
}

