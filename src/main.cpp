#include <QApplication>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QIcon>
#include <QMessageBox>

#include <memory>

#include "cron.h"

int main(int argc, char* argv[]) {
	QApplication a(argc, argv);

	// 创建托盘图标
	QSystemTrayIcon* trayIcon = new QSystemTrayIcon();

	trayIcon->setIcon(QIcon(":/thytools/tray.png"));
	trayIcon->setToolTip("thy-tools");

	// 创建托盘菜单
	QMenu* menu = new QMenu();

	QAction* cronAction = new QAction("Cron");
	QAction* timerAction = new QAction("定时通知");
	QAction* exitAction = new QAction("退出");

	menu->addAction(cronAction);
	menu->addAction(timerAction);
	menu->addSeparator();
	menu->addAction(exitAction);

	trayIcon->setContextMenu(menu);

	trayIcon->show();

	// 托盘提示信息（可选）
	trayIcon->showMessage("thy-tools", "程序已最小化到托盘", QSystemTrayIcon::Information, 3000);

	std::unique_ptr<cron_tool> cron;

	QObject::connect(cronAction, &QAction::triggered, [&cron]() {
		if (!cron) {
			cron = std::make_unique<cron_tool>();
		}
		cron->show();
		});

	QObject::connect(exitAction, &QAction::triggered, [&]() {
		trayIcon->hide();
		QApplication::quit();
		});

	// 是否在退出所有窗口后关闭进程
	a.setQuitOnLastWindowClosed(false);


	return a.exec();
}