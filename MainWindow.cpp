#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "AppDefines.h"
#include "AppMain.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->startBtn->setCheckable(true);
    ui->startBtn->setText("Start");
    connectGUISignal();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectGUISignal()
{
    connect(ui->startBtn, &QPushButton::clicked, this, &MainWindow::onStartClicked);
}

void MainWindow::onStartClicked()
{
    LOGD("");
    if(ui->startBtn->isChecked()) {
        ui->startBtn->setText("Stop");
        AppController::instance()->startLDPlayers();
    } else {
        AppController::instance()->stopLDPlayers();
        ui->startBtn->setText("Start");
    }
}

