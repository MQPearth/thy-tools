#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_thytools.h"

class thytools : public QMainWindow
{
    Q_OBJECT

public:
    thytools(QWidget *parent = nullptr);
    ~thytools();

private:
    Ui::thytoolsClass ui;
};

