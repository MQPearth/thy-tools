#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_cron.h"
#include <QString>

class cron_tool : public QMainWindow
{
    Q_OBJECT

public:
    cron_tool(QWidget* parent = nullptr);
    ~cron_tool();

    void clear();
    void runInThread(int times, const QString& crontext);
    void showWindow();

signals:
    void logSignal(const QString& value);

public slots:
    void run();
    void handlelogSignal(const QString& value);


private:
    Ui::cronClass ui;
};

