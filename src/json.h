#pragma once

#include <QtWidgets/QMainWindow>
#include <QString>
#include "ui_json.h"

class json_tool : public QMainWindow
{
    Q_OBJECT

public:
    json_tool(QWidget* parent = nullptr);
    ~json_tool();
    void showWindow();

public slots:
    void handleUpdateSignal();

private:
    Ui::jsonClass ui;
};

