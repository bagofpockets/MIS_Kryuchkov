#include <cstdlib>
#include <ctime>
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
#include "warehouse.h"
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
        {5, QString::fromUtf8(u8"распределения Эрланга")},
        {6, QString::fromUtf8(u8"работы оптового склада")} };

    //Установка интерфейса
    ui.setupUi(this);

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

    //Установка чарта для склада
    curves = new std::vector<QwtPlotCurve*>;
    for (int i = 0; i < 6; i++)
    {
        QwtPlotCurve* curve = new QwtPlotCurve(QString::fromUtf8(u8"Товар %1").arg(i+1));
        curve->setPen(QPen(QColor::fromHsv(i * 60, 255, 255), 3));
        curve->attach(ui.mPlot);
        curves->push_back(curve);
    }

    mgrid = new QwtPlotGrid();
    mgrid->enableXMin(true);
    mgrid->enableYMin(true);
    mgrid->setMajorPen(QPen(Qt::black));
    mgrid->setMinorPen(QPen(Qt::gray));

    ui.mPlot->setAxisTitle(QwtPlot::xBottom, QString::fromUtf8(u8"День"));
    ui.mPlot->setAxisTitle(QwtPlot::yLeft, QString::fromUtf8(u8"Удовлетворено заявок"));
    ui.mPlot->setAxisScale(QwtPlot::yLeft, 0.0, 10.0);
    ui.mPlot->setAxisScale(QwtPlot::xBottom, 0.0, 10.0);
    ui.mPlot->setAxisAutoScale(QwtPlot::yLeft, true);
    ui.mPlot->setAxisAutoScale(QwtPlot::xBottom, true);

    // Легенда
    legend = new QwtLegend();
    ui.mPlot->insertLegend(legend, QwtPlot::TopLegend);

    mgrid->attach(ui.mPlot);
    ui.mPlot->replot();

    //Установка текста кнопки построения и названия чарта
    on_gTabs_currentChanged(ui.gTabs->currentIndex());
}

MIS_Kryuchkov::~MIS_Kryuchkov()
{
    delete grid;
    delete mgrid;
    delete histogram;
    delete legend;
    for (std::vector<QwtPlotCurve*>::iterator it = curves->begin(); it != curves->end(); it++)
    {
        delete *it;
    }
    delete curves;
}

void MIS_Kryuchkov::on_showPlot_clicked()
{
    const int numValues = ui.valuesNum->value();
    const int intervalValues = ui.intervalsNum->value();


    switch (ui.gTabs->currentIndex())
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
    case 6:
    {
        break;
    }
    default:
        return;
    }
}

void MIS_Kryuchkov::on_showMPlot_clicked()
{
    const int days = ui.spinBox6_0->value();

    std::vector<int> bottomLine;
    bottomLine.push_back(ui.spinBox6_1_1->value());
    bottomLine.push_back(ui.spinBox6_1_2->value());
    bottomLine.push_back(ui.spinBox6_1_3->value());
    bottomLine.push_back(ui.spinBox6_1_4->value());
    bottomLine.push_back(ui.spinBox6_1_5->value());
    bottomLine.push_back(ui.spinBox6_1_6->value());

    std::vector<int> delivery;
    delivery.push_back(ui.spinBox6_2_1->value());
    delivery.push_back(ui.spinBox6_2_2->value());
    delivery.push_back(ui.spinBox6_2_3->value());
    delivery.push_back(ui.spinBox6_2_4->value());
    delivery.push_back(ui.spinBox6_2_5->value());
    delivery.push_back(ui.spinBox6_2_6->value());

    Warehouse simul_warehouse(bottomLine, delivery, days);

    std::pair<double, std::vector<std::pair<QVector<double>, QVector<double>>>> result;
    if (ui.radioButton6_1->isChecked()) //Равномерное
    {
        result = simul_warehouse.run_model(1);
    }
    else if (ui.radioButton6_2->isChecked()) //Треугольное
    {
        result = simul_warehouse.run_model(2);
    }
    else if (ui.radioButton6_3->isChecked()) //Нормальное
    {
        result = simul_warehouse.run_model(3);
    }

    std::vector<std::pair<QVector<double>, QVector<double>>> xyData(result.second);
    for (int i = 0; i < 6; i++)
    {
        curves->at(i)->setSamples(xyData[i].first, xyData[i].second);
    }
    ui.label6_3_2->setText(QString::fromUtf8(u8"%1%").arg(result.first));
    ui.mPlot->replot();
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

void MIS_Kryuchkov::on_gTabs_currentChanged(int curTab)
{
    ui.plot->setTitle(QString::fromUtf8(u8"Гистограмма %1").arg(tab_names[curTab]));
    ui.showPlot->setText(QString::fromUtf8(u8"Построить диаграмму %1").arg(tab_names[curTab]));
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
        ui.showPlot->setText(QString::fromUtf8(u8"Построить диаграмму гиперэкспоненциального распределения"));
    }
    else on_gTabs_currentChanged(4);
}