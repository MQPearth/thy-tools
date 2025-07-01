#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ip.h"
#include <QJsonObject>

class ip_tool : public QMainWindow
{
    Q_OBJECT

public:
    ip_tool(QWidget* parent = nullptr);
    ~ip_tool();
    void showWindow();

    void query();

    void sendRequest(const QString& ip);
signals:
    void updateSignal(const QString code, const QJsonObject value);

public slots:
    void handleUpdateSignal(const QString code, const QJsonObject value);

private:
    Ui::ipClass ui;
};

