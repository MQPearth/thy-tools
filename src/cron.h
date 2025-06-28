#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_cron.h"

class cron_tool : public QMainWindow
{
    Q_OBJECT

public:
    cron_tool(QWidget* parent = nullptr);
    ~cron_tool();

private:
    Ui::cronClass ui;
};

