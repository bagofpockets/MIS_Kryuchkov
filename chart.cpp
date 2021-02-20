#include "chart.h"

chartWidget::chartWidget(QWidget* parent)
{
    ui.setupUi(this);
    chart = new QChart();
    chart->setObjectName(QString::fromUtf8("chart"));


    chart->setTitle("Simple barchart example");

    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    ui.gridLayout_2->addWidget(chartView, 0, 0, 1, 1);
}

chartWidget::~chartWidget()
{
    delete chart;
    delete chartView;
}