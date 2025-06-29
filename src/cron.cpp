#include "cron.h"
#include "croncpp.h"

#include <QtConcurrent>

#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

cron_tool::cron_tool(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	connect(ui.pushButton, &QPushButton::clicked, this, &cron_tool::run);

	connect(this, &cron_tool::logSignal, this, &cron_tool::handlelogSignal);
}

cron_tool::~cron_tool()
{
}


void cron_tool::handlelogSignal(const QString& value) {
	ui.textEdit->insertPlainText(value);
	ui.textEdit->insertPlainText("\n");
}



void cron_tool::clear() {
	ui.textEdit->clear();
}

std::string formatTimeFromMilliseconds(long long timestampMillis) {
	std::time_t t = static_cast<std::time_t>(timestampMillis);
	std::tm tm = {};
	localtime_s(&tm, &t);
	std::ostringstream oss;
	oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S %a");
	return oss.str();
}

void cron_tool::runInThread(int times, const QString& crontext)
{
    try
    {
        auto cron = cron::make_cron(crontext.toStdString());
        std::time_t now = std::time(0);

        for (size_t i = 0; i < times; i++)
        {
            now = cron::cron_next(cron, now);
            auto nextTime = formatTimeFromMilliseconds(now);
            auto nextStr = QString::fromStdString(nextTime);

            emit logSignal(nextStr);
        }
    }
    catch (cron::bad_cronexpr const& ex)
    {
        emit logSignal(QString("Invalid cron expression: ") + ex.what());
    }
    catch (std::exception const& ex)
    {
        emit logSignal(QString("Error: ") + ex.what());
    }
}

void cron_tool::showWindow() {
    ui.textEdit->clear();
    ui.lineEdit->clear();
    this->show();
}

void cron_tool::run()
{
    this->clear();

    int times = ui.spinBox->value();
    QString crontext = ui.lineEdit->text();

    auto func = std::bind(&cron_tool::runInThread, this, times, crontext);
    QThreadPool::globalInstance()->start(func);
}