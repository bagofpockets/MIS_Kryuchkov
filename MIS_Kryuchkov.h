#pragma once

#include <QtWidgets/QMainWindow>
#include "chart.h"
#include "chartview.h"
#include "ui_MIS_Kryuchkov.h"

QT_CHARTS_USE_NAMESPACE

class MIS_Kryuchkov : public QMainWindow
{
    Q_OBJECT

public:
    MIS_Kryuchkov(QWidget *parent = Q_NULLPTR);
    ~MIS_Kryuchkov();

private:
    Ui::MIS_KryuchkovClass ui;
    Chart* chart;
    ChartView* chartView;
    int i = 0;

    //chartWidget* chartWindow;

private slots:
    void on_showChart_clicked();
    void on_tab_currentChanged(int);
};
