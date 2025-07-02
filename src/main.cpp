#include <QApplication>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QIcon>
#include <QMessageBox>

#include <memory>

#include "cron.h"
#include "ip.h"
#include "json.h"
#include "configmanager.h"

int main(int argc, char* argv[]) {
	QApplication a(argc, argv);


	ConfigManager::initNetworkManager(&a);

	// 创建托盘图标
	QSystemTrayIcon* trayIcon = new QSystemTrayIcon();

	trayIcon->setIcon(QIcon(":/thytools/tray.png"));
	trayIcon->setToolTip("thy-tools");

	// 创建托盘菜单
	QMenu* menu = new QMenu();

	QAction* cronAction = new QAction("Cron");
	QAction* ipAction = new QAction("IP");
	QAction* jsonAction = new QAction("JSON");
	QAction* timerAction = new QAction("定时");
	QAction* settingsAction = new QAction("设置");
	QAction* exitAction = new QAction("退出");


	menu->addAction(cronAction);
	menu->addAction(ipAction);
	menu->addAction(jsonAction);
	menu->addAction(timerAction);
	menu->addSeparator();
	menu->addAction(settingsAction);
	menu->addAction(exitAction);


	trayIcon->setContextMenu(menu);

	trayIcon->show();

	
	if (!ConfigManager::loadConfig()) {
		QMessageBox::information(
			nullptr,                    
			"提示",                 
			"配置文件读写失败, 无法初始化"    
		);
	}

	if (ConfigManager::isFirst()) {
		trayIcon->showMessage("thy-tools", "程序已最小化到托盘", QSystemTrayIcon::Information, 3000);
	}


	std::unique_ptr<cron_tool> cron;

	QObject::connect(cronAction, &QAction::triggered, [&cron]() {
		if (!cron) {
			cron = std::make_unique<cron_tool>();
		}
		cron->showWindow();
		});

	std::unique_ptr<ip_tool> ip;

	QObject::connect(ipAction, &QAction::triggered, [&ip]() {
		if (!ip) {
			ip = std::make_unique<ip_tool>();
		}
		ip->showWindow();
		});


	std::unique_ptr<json_tool> json;

	QObject::connect(jsonAction, &QAction::triggered, [&json]() {
		if (!json) {
			json = std::make_unique<json_tool>();
		}
		json->showWindow();
		});

	QObject::connect(exitAction, &QAction::triggered, [&]() {
		trayIcon->hide();
		QApplication::quit();
		});

	// 是否在退出所有窗口后关闭进程
	a.setQuitOnLastWindowClosed(false);


	return a.exec();
}