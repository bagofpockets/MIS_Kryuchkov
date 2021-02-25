#include <cstdlib>
#include <ctime>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QLegend>
#include <QtCharts/QValueAxis>
#include <qdockwidget.h>
#include "MIS_Kryuchkov.h"

MIS_Kryuchkov::MIS_Kryuchkov(QWidget *parent)
    : QMainWindow(parent)
{
    //Названия для кнопки построения и чарта
    tab_names = {
        {0, QString::fromUtf8(u8"равномерного распределения")},
        {1, QString::fromUtf8(u8"распределения Симпсона")},
        {2, QString::fromUtf8(u8"нормального распределения")},
        {3, QString::fromUtf8(u8"экспаненциального распределения")},
        {4, QString::fromUtf8(u8"распределения Эрланга")} };

    //Установка интерфейса
    ui.setupUi(this);

    ui.returnDock->setVisible(false);

    //chartWindow = new chartWidget();

    //Установка чарта
    chart = new Chart();
    chart->setObjectName(QString::fromUtf8("chart"));

    QValueAxis* axisX = new QValueAxis();
    chart->addAxis(axisX, Qt::AlignBottom);

    QValueAxis* axisY = new QValueAxis();
    chart->addAxis(axisY, Qt::AlignLeft);

    chart->legend()->setVisible(false);
    //chart->legend()->setAlignment(Qt::AlignBottom);

    chartView = new ChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setObjectName(QString::fromUtf8("chartView"));

    QSizePolicy chartSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    chartSizePolicy.setHorizontalStretch(0);
    chartSizePolicy.setVerticalStretch(0);
    chartSizePolicy.setHeightForWidth(chartView->sizePolicy().hasHeightForWidth());
    chartView->setSizePolicy(chartSizePolicy);

    ui.gridLayout_2->addWidget(chartView, 0, 0, 1, 1);

    //Установка текста кнопки построения и названия чарта
    on_tabs_currentChanged(ui.tabs->currentIndex());

    //chartView->resize(600, 400);

    //connect(ui.tabs, SIGNAL(currentChanged(int)), this, SLOT(on_tab_currentChanged(int)));
    //connect(ui.showChart, SIGNAL(clicked()), this, SLOT(on_showChart_clicked()));
}

MIS_Kryuchkov::~MIS_Kryuchkov()
{
    //delete chartWindow;
    delete chart;
    delete chartView;
}

void MIS_Kryuchkov::on_dockWidget_topLevelChanged()
{
    if (ui.dockWidget->isFloating())
    {
        //ui.dockWidget->resize(QSize(600, 230));
        ui.returnDock->setVisible(true);
    }
    else
    {
        ui.returnDock->setVisible(false);
    }
}

void MIS_Kryuchkov::on_returnDock_clicked()
{
    ui.dockWidget->setFloating(false);
}

void MIS_Kryuchkov::on_showChart_clicked()
{
    chart->removeAllSeries();

    //QBarSet* valuesSet = new QBarSet("x");
    QSplineSeries* series = new QSplineSeries();

    switch (ui.tabs->currentIndex())
    {
    case 0:
        std::srand(std::time(0));
        if (ui.radioButton0_1->isChecked())
        {
            for (int i = 0; i < ui.spinBox0_1->value(); i++)
            {
                qreal a = qreal((double)rand() / (double)RAND_MAX);
                //chart->setTitle(QString::number(a));
                series->append(i, a);
            }
        }
        else if (ui.radioButton0_2->isChecked())
        {

        }
        else if (ui.radioButton0_3->isChecked())
        {

        }
        break;
    case 1:
        break;
    case 2:
        break;
    case 3:
        break;
    case 4:
        break;
    default:
        delete series;
        return;
    }

    chart->axisY()->setRange(0, 1);
    chart->axisX()->setRange(0, ui.spinBox0_1->value()-1);

    chart->addSeries(series);
    series->attachAxis(chart->axisX());
    series->attachAxis(chart->axisY());

    //chart->setTitle(QString::number(series->barWidth()));
    
    //ui.showChart->setShortcut(QCoreApplication::translate("MIS_KryuchkovClass", "Return", nullptr));
}

void MIS_Kryuchkov::on_tabs_currentChanged(int curTab)
{
    chart->setTitle(QString::fromUtf8(u8"Гистограмма %1").arg(tab_names[curTab]));
    ui.showChart->setText(QString::fromUtf8(u8"Построить диаграмму %1").arg(tab_names[curTab]));
}