#include <cstdlib>
#include <ctime>
#include <map>
#include <qdockwidget.h>
#include "qwt_interval.h"
#include "qwt_samples.h"
#include "qwt_series_data.h"
#include "qwt_plot_picker.h"
#include "simple_congruent.h"
#include "linear_congruent.h"
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

    //Установка чарта
    histogram = new QwtPlotHistogram();
    histogram->setStyle(QwtPlotHistogram::Columns);
    histogram->setPen(QPen(Qt::black));
    histogram->setBrush(QBrush(Qt::gray));

    ui.plot->setAxisTitle(QwtPlot::yLeft, QString::fromUtf8(u8"Вероятность"));
    ui.plot->setAxisTitle(QwtPlot::xBottom, QString::fromUtf8(u8"Величина"));
    ui.plot->setAxisScale(QwtPlot::yLeft, 0.0, 10.0);
    ui.plot->setAxisScale(QwtPlot::xBottom, 0.0, 10.0);
    ui.plot->setAxisAutoScale(QwtPlot::yLeft, true);
    ui.plot->setAxisAutoScale(QwtPlot::xBottom, true);
    histogram->attach(ui.plot);
    ui.plot->replot();

    //Установка текста кнопки построения и названия чарта
    on_tabs_currentChanged(ui.tabs->currentIndex());

}

MIS_Kryuchkov::~MIS_Kryuchkov()
{
    delete histogram;
}

void MIS_Kryuchkov::on_dockWidget_topLevelChanged()
{
    if (ui.dockWidget->isFloating())
    {
        ui.dockWidget->resize(QSize(600, 230));
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
    std::srand(std::time(0));

    switch (ui.tabs->currentIndex())
    {
    case 0:
    {
        const int numValues = ui.spinBox0_1->value();
        const int intervalValues = ui.spinBox0_2->value();

        QVector<QwtIntervalSample> samples(intervalValues);
        std::map<double, int> valIntervals;

        for (uint i = 0; i < intervalValues; i++)
        {
            valIntervals[(double(i) + 1) / double(intervalValues)] = 0;
        }

        if (ui.radioButton0_1->isChecked())
        {
            for (uint i = 0; i < numValues; i++)
            {
                double nextVal = (double)rand() / (double)RAND_MAX;

                for (std::map<double, int>::iterator it = valIntervals.begin(); it != valIntervals.end(); it++)
                {
                    if (nextVal < it->first)
                    {
                        it->second++;
                        break;
                    }
                }
            }
        }
        else if (ui.radioButton0_2->isChecked())
        {
            SimpleCongruent val(std::time(0));

            for (uint i = 0; i < numValues; i++)
            {
                double nextVal = val.next();

                for (std::map<double, int>::iterator it = valIntervals.begin(); it != valIntervals.end(); it++)
                {
                    if (nextVal < it->first)
                    {
                        it->second++;
                        break;
                    }
                }
            }

        }
        else if (ui.radioButton0_3->isChecked())
        {
            LinearCongruent val(std::time(0));

            for (uint i = 0; i < numValues; i++)
            {
                double nextVal = val.next();

                for (std::map<double, int>::iterator it = valIntervals.begin(); it != valIntervals.end(); it++)
                {
                    if (nextVal < it->first)
                    {
                        it->second++;
                        break;
                    }
                }
            }
        }

        double lastBorder = 0.0;
        uint i = 0;
        for (std::map<double, int>::iterator it = valIntervals.begin(); it != valIntervals.end(); it++)
        {
            QwtInterval interval(lastBorder, it->first);
            lastBorder = it->first;
            interval.setBorderFlags(QwtInterval::ExcludeMaximum);
            samples[i] = QwtIntervalSample(it->second, interval);
            i++;
        }

        histogram->setData(new QwtIntervalSeriesData(samples));
        ui.plot->replot();

        break;
    }
    case 1:
        break;
    case 2:
        break;
    case 3:
        break;
    case 4:
        break;
    default:
        return;
    }

    /*chart->axisY()->setRange(-0.2, 1.2);
    chart->axisX()->setRange(-1, ui.spinBox0_1->value());

    chart->addSeries(series);
    series->attachAxis(chart->axisX());
    series->attachAxis(chart->axisY());*/

    //chart->setTitle(QString::number(series->barWidth()));
    
    //ui.showChart->setShortcut(QCoreApplication::translate("MIS_KryuchkovClass", "Return", nullptr));
}

void MIS_Kryuchkov::on_tabs_currentChanged(int curTab)
{
    ui.plot->setTitle(QString::fromUtf8(u8"Гистограмма %1").arg(tab_names[curTab]));
    //chart->setTitle(QString::fromUtf8(u8"Гистограмма %1").arg(tab_names[curTab]));
    ui.showChart->setText(QString::fromUtf8(u8"Построить диаграмму %1").arg(tab_names[curTab]));
}