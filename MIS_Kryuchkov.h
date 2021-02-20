#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MIS_Kryuchkov.h"
#include "chart.h"

class MIS_Kryuchkov : public QMainWindow
{
    Q_OBJECT

public:
    MIS_Kryuchkov(QWidget *parent = Q_NULLPTR);
    ~MIS_Kryuchkov();

private:
    Ui::MIS_KryuchkovClass ui;
    chartWidget* chartWindow;

private slots:
    void on_showChart_toggled(bool checked);
};
