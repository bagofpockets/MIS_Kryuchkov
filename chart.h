#pragma once

#include <QtWidgets/QMainWindow>
#include <QtCharts/qchartview.h>
#include "ui_chart.h"

QT_CHARTS_USE_NAMESPACE

class chartWidget : public QWidget
{
    Q_OBJECT

        
public:
    chartWidget(QWidget* parent = Q_NULLPTR);
    ~chartWidget();

    QChart* chart;

private:
    Ui::chartWidget ui;
    QChartView* chartView;
};
