#pragma once

#include <QtWidgets/QMainWindow>
#include <map>
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
    std::map<int, QString> tab_names;
    Ui::MIS_KryuchkovClass ui;
    Chart* chart;
    ChartView* chartView;

    //chartWidget* chartWindow;

private slots:
    void on_dockWidget_topLevelChanged();
    void on_returnDock_clicked();
    void on_showChart_clicked();
    void on_tabs_currentChanged(int);
};
