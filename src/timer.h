#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_timer.h"

class timer_tool : public QMainWindow
{
    Q_OBJECT

public:
    timer_tool(QWidget* parent = nullptr);
    ~timer_tool();

private:
    Ui::timerClass ui;
};

