#include <cstdlib>
#include <ctime>
#include <math.h>
#include <map>
#include <qdockwidget.h>
#include <qdebug.h>
#include "qwt_interval.h"
#include "qwt_samples.h"
#include "qwt_series_data.h"
#include "qwt_plot_picker.h"
#include "uniform.h"
#include "simpson.h"
#include "normal.h"
#include "exponential.h"
#include "MIS_Kryuchkov.h"

MIS_Kryuchkov::MIS_Kryuchkov(QWidget *parent)
    : QMainWindow(parent)
{
    //Названия для кнопки построения и чарта
    tab_names = {
        {0, QString::fromUtf8(u8"равномерного распределения")},
        {1, QString::fromUtf8(u8"трапецеидального распределения")},
        {2, QString::fromUtf8(u8"треугольного распределения")},
        {3, QString::fromUtf8(u8"нормального распределения")},
        {4, QString::fromUtf8(u8"экспоненциального распределения")},
        {5, QString::fromUtf8(u8"распределения Эрланга")} };

    //Установка интерфейса
    ui.setupUi(this);
    ui.returnDock->setVisible(false);

    //Установка ограничений ввода
    ui.doubleSpinBox2_3->setRange(ui.doubleSpinBox2_1->value(), ui.doubleSpinBox2_2->value());

    //Установка чарта
    histogram = new QwtPlotHistogram();
    histogram->setStyle(QwtPlotHistogram::Columns);
    histogram->setPen(QPen(Qt::black));
    histogram->setBrush(QBrush(Qt::darkCyan));

    grid = new QwtPlotGrid();
    grid->enableXMin(true);
    grid->enableYMin(true);
    grid->setMajorPen(QPen(Qt::black));
    grid->setMinorPen(QPen(Qt::gray));

    ui.plot->setAxisTitle(QwtPlot::yLeft, QString::fromUtf8(u8"Количество"));
    ui.plot->setAxisTitle(QwtPlot::xBottom, QString::fromUtf8(u8"Величина"));
    ui.plot->setAxisScale(QwtPlot::yLeft, 0.0, 10.0);
    ui.plot->setAxisScale(QwtPlot::xBottom, 0.0, 10.0);
    ui.plot->setAxisAutoScale(QwtPlot::yLeft, true);
    ui.plot->setAxisAutoScale(QwtPlot::xBottom, true);

    grid->attach(ui.plot);
    histogram->attach(ui.plot);
    ui.plot->replot();

    //Установка текста кнопки построения и названия чарта
    on_tabs_currentChanged(ui.tabs->currentIndex());
}

MIS_Kryuchkov::~MIS_Kryuchkov()
{
    delete grid;
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
    const int numValues = ui.valuesNum->value();
    const int intervalValues = ui.intervalsNum->value();


    switch (ui.tabs->currentIndex())
    {
    case 0: //Равномерное распределение
    {
        Uniform val(std::time(0));

        if (ui.radioButton0_1->isChecked()) //Стандартный генератор
        {
            generate_distribution<Uniform, &Uniform::standardNext>(&val, numValues, intervalValues);
        }
        else if (ui.radioButton0_2->isChecked()) //Метод простых конгруэнций
        {
            generate_distribution<Uniform, &Uniform::simpleCongruentNext>(&val, numValues, intervalValues);
        }
        else if (ui.radioButton0_3->isChecked()) //Линейная конгруэнтная последовательность Д. Г. Лемера
        {
            generate_distribution<Uniform, &Uniform::linearCongruentNext>(&val, numValues, intervalValues);
        }

        break;
    }
    case 1: //Трапецеидальное распределение
    {
        const double lowerBound = ui.doubleSpinBox1_1->value();
        const double upperBound = ui.doubleSpinBox1_2->value();
        const double modeL = ui.doubleSpinBox1_3->value();
        const double modeR = ui.doubleSpinBox1_4->value();

        Simpson val(std::time(0), lowerBound, upperBound, modeL, modeR);

        generate_distribution<Simpson, &Simpson::next>(&val, numValues, intervalValues, lowerBound, upperBound);

        break;
    }
    case 2: //Треугольное распределение
    {
        const double lowerBound = ui.doubleSpinBox2_1->value();
        const double upperBound = ui.doubleSpinBox2_2->value();
        const double mode = ui.doubleSpinBox2_3->value();

        Simpson val(std::time(0), lowerBound, upperBound, mode, mode);

        generate_distribution<Simpson, &Simpson::next>(&val, numValues, intervalValues, lowerBound, upperBound);

        break;
    }
    case 3: //Нормальное распределение
    {
        Normal val(std::time(0));

        if (ui.radioButton3_1->isChecked()) //Методичка
        {
            generate_distribution<Normal, &Normal::standartNext>(&val, numValues, intervalValues);
        }
        else if (ui.radioButton3_2->isChecked()) //Метод Марсальи
        {
            generate_distribution<Normal, &Normal::marsagliaPolarNext>(&val, numValues, intervalValues);
        }

        break;
    }
    case 4: //Экспоненциальное распределение
    {
        double lambda = ui.doubleSpinBox4_1->value();
        Exponential val(std::time(0), intervalValues, lambda);

        if (ui.checkBox4_1->isChecked()) //Гиперэкспоненциальное распределение
        {
            generate_distribution<Exponential, &Exponential::hyperNext>(&val, numValues, intervalValues, 0.0, 1.0 / lambda);
        }
        else
        {
            generate_distribution<Exponential, &Exponential::standardNext>(&val, numValues, intervalValues, 0.0, 10.0 / lambda);
        }

        break;
    }
    case 5: //Эрланговое распределение
    {
        double lambda = ui.doubleSpinBox5_1->value();
        const int k = ui.spinBox5_1->value();
        Exponential val(std::time(0), intervalValues, lambda, k);

        generate_distribution<Exponential, &Exponential::erlangNext>(&val, numValues, intervalValues, 0.0, 10.0 * sqrt(k) / lambda);

        break;
    }
    default:
        return;
    }
}

template <typename TOwner, double(TOwner::* nextFunc)()>
void MIS_Kryuchkov::generate_distribution(TOwner* val, const int numValues, const int intervalValues, const double lowerBound, const double upperBound)
{
    QVector<QwtIntervalSample> samples(intervalValues);
    std::map<double, int> valIntervals;

    for (uint i = 0; i < intervalValues; i++)
    {
        valIntervals[lowerBound + (double(i) + 1) / double(intervalValues) * (upperBound - lowerBound)] = 0;
    }

    for (uint i = 0; i < numValues; i++)
    {
        double nextVal = (val->*nextFunc)();

        for (std::map<double, int>::iterator it = valIntervals.begin(); it != valIntervals.end(); it++)
        {
            if (nextVal < it->first)
            {
                it->second++;
                break;
            }
        }
    }

    double lastBorder = lowerBound;
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
}

void MIS_Kryuchkov::on_tabs_currentChanged(int curTab)
{
    ui.plot->setTitle(QString::fromUtf8(u8"Гистограмма %1").arg(tab_names[curTab]));
    ui.showChart->setText(QString::fromUtf8(u8"Построить диаграмму %1").arg(tab_names[curTab]));
}

void MIS_Kryuchkov::on_doubleSpinBox1_1_valueChanged(double d)
{
    ui.doubleSpinBox1_2->setMinimum(d);
    ui.doubleSpinBox1_3->setMinimum(d);
    ui.doubleSpinBox1_4->setMinimum(d);
}

void MIS_Kryuchkov::on_doubleSpinBox1_2_valueChanged(double d)
{
    ui.doubleSpinBox1_1->setMaximum(d);
    ui.doubleSpinBox1_3->setMaximum(d);
    ui.doubleSpinBox1_4->setMaximum(d);
}

void MIS_Kryuchkov::on_doubleSpinBox1_3_valueChanged(double d)
{
    ui.doubleSpinBox1_4->setMinimum(d);
}

void MIS_Kryuchkov::on_doubleSpinBox1_4_valueChanged(double d)
{
    ui.doubleSpinBox1_3->setMaximum(d);
}

void MIS_Kryuchkov::on_doubleSpinBox2_1_valueChanged(double d)
{
    ui.doubleSpinBox2_2->setMinimum(d);
    ui.doubleSpinBox2_3->setMinimum(d);
}

void MIS_Kryuchkov::on_doubleSpinBox2_2_valueChanged(double d)
{
    ui.doubleSpinBox2_1->setMaximum(d);
    ui.doubleSpinBox2_3->setMaximum(d);
}

void MIS_Kryuchkov::on_checkBox4_1_toggled(bool c)
{
    if (c)
    {
        ui.plot->setTitle(QString::fromUtf8(u8"Гистограмма гиперэкспоненциального распределения"));
        ui.showChart->setText(QString::fromUtf8(u8"Построить диаграмму гиперэкспоненциального распределения"));
    }
    else on_tabs_currentChanged(4);
}