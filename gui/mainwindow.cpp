#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include <QKeyEvent>

#include "gui/config.h"
#include <iostream>
#include <string>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // setup ui elements
    ui->setupUi(this);
    Config::height = this->ui->main->height();
    Config::width =this->ui->main->width();

    // connect ui elements to slots
    connect(this->ui->sliderAnimationSpeed, SIGNAL(valueChanged(int)), this, SLOT(setAnimationSpeed(int)));
    connect(this->ui->checkBoxLocalRotation, SIGNAL(clicked(bool)), this, SLOT(setLocalRotation(bool)));
    connect(this->ui->GlobaleRotation, SIGNAL(clicked(bool)), this, SLOT(setGlobalRotation(bool)));
    connect(this->ui->BeleuchtungSonne, SIGNAL(clicked(bool)), this, SLOT(setLightOn(bool)));
    connect(this->ui->Gitternetz, SIGNAL(clicked(bool)), this, SLOT(setGitter(bool)));
    connect(this->ui->resetCamera, &QPushButton::released, this, &MainWindow::setCameraRest);
    connect(this->ui->spinBox, SIGNAL(valueChanged(int)), this, SLOT(setU(int)));
    connect(this->ui->spinBox_2, SIGNAL(valueChanged(int)), this, SLOT(setV(int)));
    connect(this->ui->spinBox_3, SIGNAL(valueChanged(int)), this, SLOT(setRing(int)));
    connect(this->ui->laser_length, SIGNAL(valueChanged(int)), this, SLOT(setLaserLength(int)));
    connect(this->ui->angle, SIGNAL(valueChanged(int)), this, SLOT(setLaserAngle(int)));
    connect(this->ui->laserActve, SIGNAL(clicked(bool)), this, SLOT(setLaser(bool)));


//    connect(this->ui->checkBoxOrbit, SIGNAL(clicked(bool)), this, SLOT(setOrbit(bool)));
//    connect(this->ui->checkBoxPath, SIGNAL(clicked(bool)), this, SLOT(setPath(bool)));

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::setAnimationSpeed(int value)
{
    Config::animationSpeed = float(value) / 2.0f;
    QString title = QString("Animation: ") + QString::number(Config::animationSpeed, 'x', 1) + "x";
    this->ui->groupBox_4->setTitle(title);
}

void MainWindow::setLocalRotation(bool value)
{
    Config::localRotation = value;
}

void MainWindow::setGitter(bool value)
{
    Config::gitter = value;
}

void MainWindow::setGlobalRotation(bool value)
{
    Config::globalRotation = value;
}


void MainWindow::setLightOn(bool value)
{
    Config::lightOn = value;
}


void MainWindow::setCameraRest()
{
    if( Config::resetCam == false )
    { Config::resetCam  = true;}
    else{ Config::resetCam = false;}
}

void MainWindow::setU(int value)
{
    Config::u = value;
}

void MainWindow::setV(int value)
{
    Config::v = value;
}

void MainWindow::setRing(int value)
{
    Config::ring_res = value;
}

void MainWindow::setLaserLength(int value)
{
    Config::laser_length =value;
}
void MainWindow::setLaser(bool value)
{
    Config::laserActive = value;
}

void MainWindow::setLaserAngle(int value)
{
    Config::angleOfLaser = value;
}




//void setOrbit(bool value);

//void setPath(bool value);

/* Qt mouse and keyboard events */

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    // enable control via keyboard
    switch (event->key()) {
    case Qt::Key_F:
        if (isFullScreen()) {
            showNormal();
        } else {
            showFullScreen();
        }
        break;
    case Qt::Key_Escape:
    case Qt::Key_Q:
        close();
        break;
    }
}

