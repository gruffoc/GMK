#include "misura.h"
#include "unistd.h"
#include <iostream>

#include <QString>
#include <QtCharts/QChart>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QAreaSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QPolarChart>
#include <QtCharts/QChartView>
#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace QtCharts;

extern MainWindow *ptr_mainwindow;

extern QScatterSeries *measure;
extern QPolarChart *chartp;
extern QChart *chartl;
extern QChartView *view;
extern QValueAxis *radialAxis;
extern QValueAxis *angularAxis;

extern bool graph_show;
extern int measure_number;
extern double compl_perc;

Misura::Misura()
{

}

Misura::~Misura()
{
    delete this;
}


void Misura::perform_measure()
{
    if(measure_number==0){
        emit start();
        double N_point = 10.0;
        if(!graph_show) measure->clear();

            for(int i=0; i<10; i++){
                measure->append(i*7, (i / 100) * 100 + 8.0);
                std::cout << i << std::endl;
                sleep(1);
                view->update();
                compl_perc = ((double(i)+1.0)/N_point)*100;
                emit update_progress();
            }
            emit finished();
    }else{
        emit start();
        double N_point = 10.0;
        if(!graph_show) measure->clear();

            for(int i=0; i<10; i++){
                measure->append(i*(7+measure_number*10), (i / 100) * 100 + 8.0);
                std::cout << i << std::endl;
                sleep(1);
                measure->setName("Radiation Pattern N."+QString::number(measure_number));
                view->update();
                compl_perc = ((double(i)+1.0)/N_point)*100;
                emit update_progress();
            }
            emit finished();

    }
    measure_number++;

}
