#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "user_def.h"
#include "radpat.h"
#include "misura.h"


#include <iostream>
#include <QStringList>
#include <QFileDialog>
#include <QtCharts/QChart>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QAreaSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QPolarChart>
#include <QtCharts/QChartView>
#include <QThread>



using namespace QtCharts;

QThread *tt_mis;
Misura *mis;




extern motor_par mot1;
extern QScatterSeries *measure;
extern QPolarChart *chartp;
extern QChart *chartl;
extern QChartView *view;
extern QValueAxis *radialAxis;
extern QValueAxis *angularAxis;


extern bool graph_show;
extern int measure_number;
extern double compl_perc;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //setting up the main interface
    ui->setupUi(this);
    graph_show = true;
    measure_number = 0;
    compl_perc = 0;

    //setting up the measure thread
    tt_mis = new QThread();
    mis = new Misura();

    mis->moveToThread(tt_mis);

    connect(tt_mis,SIGNAL(started()),mis,SLOT(perform_measure()));
    connect(ui->start_meas,SIGNAL(clicked(bool)),this,SLOT(start_meas_thread()));
    connect(ui->refresh, SIGNAL(clicked(bool)), this, SLOT(refresh_data()));

    connect(mis,SIGNAL(start()),this, SLOT(start_mis()));
    connect(mis,SIGNAL(update_progress()),this,SLOT(update_prog()));
    connect(mis,SIGNAL(finished()),this, SLOT(end_mis()));


    connect(ui->checkBox,SIGNAL(clicked(bool)),this,SLOT(set_overwrite_graph()));
    connect(ui->rdb_polar,SIGNAL(clicked(bool)),this,SLOT(polar_plot()));
    connect(ui->rdb_scat,SIGNAL(clicked(bool)),this,SLOT(scatter_plot()));




    measure->setName("Radiation Pattern");


    for (int i = -100; i <= 100; i += 10)
        measure->append(i, i);

    //setting up radiation pattern plot canvas

    chartp = new QPolarChart();
    chartl = new QChart();
    view = new QChartView();
    radialAxis = new QValueAxis();
    angularAxis = new QValueAxis();

    chartp->addSeries(measure);
    //chartl->addSeries(measure);
    measure->clear();
    view->setChart(chartp);
    view->setRenderHint(QPainter::Antialiasing);
    angularAxis->setTickCount(9); // First and last ticks are co-located on 0/360 angle.
    angularAxis->setLabelFormat("%.1f");
    angularAxis->setShadesVisible(true);
    angularAxis->setShadesBrush(QBrush(QColor(249, 249, 255)));
    chartp->addAxis(angularAxis, QPolarChart::PolarOrientationAngular);
    radialAxis->setTickCount(9);
    radialAxis->setLabelFormat("%d");
    chartp->addAxis(radialAxis, QPolarChart::PolarOrientationRadial);
    measure->attachAxis(radialAxis); // qui forse c'è la risposta al multiplot - fare più assi
    measure->attachAxis(angularAxis);



    //setting up the radiation pattern interface
    g_rad.setFixedSize(600,480);
    g_rad.setCentralWidget(view);
    g_rad.show();
    std::cout << "Show" << std::endl;





    connect(ui->Load_cut, SIGNAL(clicked(bool)), this, SLOT(load_cut()));


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::load_cut(){
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Open File"),"/home/stefanomandelli/CUTS",tr("*.cut"));
    ui->listWidget->addItems(fileNames);
}






void MainWindow::start_meas_thread()
{
    tt_mis->start();
}


void MainWindow::refresh_data()
{
    measure->clear();
    view->update();
}



void MainWindow::start_mis()
{
    ui->meas_log->setText("Grabbing Data");
}

void MainWindow::update_prog()
{
    ui->progressBar->setValue(compl_perc);
}

void MainWindow::end_mis()
{
    tt_mis->exit();
}


void MainWindow::set_overwrite_graph()
{
    if(ui->checkBox->isChecked()){
        graph_show = true;
    }else{
        graph_show = false;
    }
}


void MainWindow::scatter_plot()
{
    angularAxis->setRange(0, 100);
    angularAxis->setTickCount(1);
    angularAxis->setLabelFormat("%.2f");
    chartl->addSeries(measure);
    chartl->createDefaultAxes();
    chartl->axisX()->setRange(0,100);
    chartl->axisY()->setRange(0,100);
    view->setChart(chartl);
    view->update();

}


void MainWindow::polar_plot()
{

    angularAxis->setTickCount(9); // First and last ticks are co-located on 0/360 angle.
    angularAxis->setLabelFormat("%.1f");
    angularAxis->setShadesVisible(true);
    angularAxis->setShadesBrush(QBrush(QColor(249, 249, 255)));
    chartp->addAxis(radialAxis, QPolarChart::PolarOrientationRadial);
    chartp->addSeries(measure);

    radialAxis->setTickCount(9);
    radialAxis->setLabelFormat("%d");

    view->setChart(chartp);
    view->setRenderHint(QPainter::Antialiasing);

}













