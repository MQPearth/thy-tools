#include <QApplication>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QIcon>
#include <QMessageBox>
#include "thytools.h"

int main(int argc, char* argv[]) {
	QApplication a(argc, argv);

	// 创建托盘图标
	QSystemTrayIcon* trayIcon = new QSystemTrayIcon();

	trayIcon->setIcon(QIcon(":/thytools/tray.png"));

	// 创建托盘菜单
	QMenu* menu = new QMenu();

	QAction* showMsgAction = new QAction("显示消息");
	QAction* exitAction = new QAction("退出");

	menu->addAction(showMsgAction);
	menu->addSeparator();
	menu->addAction(exitAction);

	// 设置托盘菜单
	trayIcon->setContextMenu(menu);

	// 托盘提示信息（可选）
	trayIcon->showMessage("托盘启动", "程序已最小化到托盘", QSystemTrayIcon::Information, 3000);

	// 显示托盘图标
	trayIcon->show();

	// 连接槽函数
	QObject::connect(showMsgAction, &QAction::triggered, []() {
		QMessageBox::information(nullptr, "提示", "你点击了显示消息");
		});

	QObject::connect(exitAction, &QAction::triggered, [&]() {
		trayIcon->hide();
		QApplication::quit();
		});

	// 是否在退出所有窗口后关闭进程
	a.setQuitOnLastWindowClosed(false);


	return a.exec();
}