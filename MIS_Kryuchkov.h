#pragma once
#include <QtWidgets/QMainWindow>
#include <map>
#include "qwt_plot_histogram.h"
#include "qwt_plot_grid.h"
#include "ui_MIS_Kryuchkov.h"

//QT_CHARTS_USE_NAMESPACE

class MIS_Kryuchkov : public QMainWindow
{
    Q_OBJECT

public:
    MIS_Kryuchkov(QWidget *parent = Q_NULLPTR);
    ~MIS_Kryuchkov();

private:
    std::map<int, QString> tab_names;
    Ui::MIS_KryuchkovClass ui;
    QwtPlotGrid* grid;
    QwtPlotHistogram* histogram;

private slots:
    void on_dockWidget_topLevelChanged();
    void on_returnDock_clicked();
    void on_showChart_clicked();
    void on_tabs_currentChanged(int);
    void on_doubleSpinBox1_1_valueChanged(double);
    void on_doubleSpinBox1_2_valueChanged(double);
    void on_doubleSpinBox1_3_valueChanged(double);
    void on_doubleSpinBox1_4_valueChanged(double);
    void on_doubleSpinBox2_1_valueChanged(double);
    void on_doubleSpinBox2_2_valueChanged(double);
};
