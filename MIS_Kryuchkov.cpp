#include <QtCharts/QBarSet>
#include <QtCharts/QBarSeries>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <qdockwidget.h>
#include "MIS_Kryuchkov.h"

MIS_Kryuchkov::MIS_Kryuchkov(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    //chartWindow = new chartWidget();

    chart = new Chart();
    chart->setObjectName(QString::fromUtf8("chart"));

    chart->setTitle("Simple barchart example");

    chartView = new ChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setObjectName(QString::fromUtf8("chartView"));

    QSizePolicy chartSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    chartSizePolicy.setHorizontalStretch(0);
    chartSizePolicy.setVerticalStretch(0);
    chartSizePolicy.setHeightForWidth(chartView->sizePolicy().hasHeightForWidth());
    chartView->setSizePolicy(chartSizePolicy);

    ui.gridLayout_2->addWidget(chartView, 0, 0, 1, 1);
    //chartView->resize(600, 400);

    connect(ui.tabs, SIGNAL(currentChanged(int)), this, SLOT(on_tab_currentChanged(int)));
    connect(ui.showChart, SIGNAL(clicked()), this, SLOT(on_showChart_clicked()));
}

MIS_Kryuchkov::~MIS_Kryuchkov()
{
    //delete chartWindow;
    delete chart;
    delete chartView;
}

void MIS_Kryuchkov::on_showChart_clicked()
{
    //TODO: Почему клик срабатывает дважды?
    QBarSet* set0 = new QBarSet("Jane");
    QBarSet* set1 = new QBarSet("John");
    QBarSet* set2 = new QBarSet("Axel");
    QBarSet* set3 = new QBarSet("Mary");
    QBarSet* set4 = new QBarSet("Samantha");

    *set0 << 1 << 2 << 3 << 4 << 5 << 6;
    *set1 << 5 << 0 << 0 << 4 << 0 << 7;
    *set2 << 3 << 5 << 8 << 13 << 8 << 5;
    *set3 << 5 << 6 << 7 << 3 << 4 << 5;
    *set4 << 9 << 7 << 5 << 3 << 1 << 2;

    QBarSeries* series = new QBarSeries();
    series->append(set0);
    series->append(set1);
    series->append(set2);
    series->append(set3);
    series->append(set4);
    chart->addSeries(series);

    QStringList categories;
    categories << "Jan" << "Feb" << "Mar" << "Apr" << "May" << "Jun";
    QBarCategoryAxis* axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis* axisY = new QValueAxis();
    axisY->setRange(0, 15);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    chartView->show();

    //ui.label->setText(QString::number(i));
    //i++;
    
    //ui.showChart->setShortcut(QCoreApplication::translate("MIS_KryuchkovClass", "Return", nullptr));
}

void MIS_Kryuchkov::on_tab_currentChanged(int curTab)
{
    ui.showChart->setText(QString::number(curTab));
}