#include "ip.h"

#include <QtConcurrent>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrl>
#include <QDebug>
#include <QClipboard>
#include "configmanager.h"

ip_tool::ip_tool(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	connect(ui.pushButton, &QPushButton::clicked, this, &ip_tool::query);
	connect(this, &ip_tool::updateSignal, this, &ip_tool::handleUpdateSignal);
}

ip_tool::~ip_tool()
{
}


void ip_tool::handleUpdateSignal(const QString code, const QJsonObject value) {

	qDebug() << "Parsed JSON:" << value;

	this->ui.pushButton->setText("查询");
	this->ui.pushButton->setEnabled(true);
	
	this->ui.textEdit->clear();
	this->ui.logEdit->clear();

	QString status = value.value("status").toString();

	if (code == "200" && status == "success") {
		QString ip = value.value("query").toString();

		QClipboard* clipboard = QApplication::clipboard();
		clipboard->setText(ip);

		QString continent = value.value("continent").toString();
		QString country = value.value("country").toString();
		QString isp = value.value("isp").toString();
		QString region = value.value("regionName").toString();
		QString city = value.value("city").toString();
		bool proxy = value.value("proxy").toBool();
		double lat = value.value("lat").toDouble();
		double lon = value.value("lon").toDouble();

		QString address = QString("%1 %2 %3 %4").arg(continent, country, region, city);
		QString locationInfo = QString("IP地址: %1\n运营商: %2\n地址: %3\n经纬度: %4, %5\n代理: %6")
			.arg(ip)
			.arg(isp)
			.arg(address)
			.arg(lon, 0, 'f', 4)
			.arg(lat, 0, 'f', 4)
			.arg(proxy ? "是" : "否");

		this->ui.textEdit->insertPlainText(locationInfo);
		this->ui.textEdit->insertPlainText("\n");

	}
	
	this->ui.logEdit->insertPlainText("状态码: " + code);
	this->ui.logEdit->insertPlainText("\n");
	QJsonDocument doc(value);
	QString jsonStr = doc.toJson(QJsonDocument::Indented);
	this->ui.logEdit->insertPlainText(jsonStr);
}


void ip_tool::sendRequest(const QString& ip) {

	this->ui.pushButton->setText("查询中");
	this->ui.pushButton->setEnabled(false);

	QUrl url(QString("https://pro.ip-api.com/json/%1?fields=66842623&key=ipapiq9SFY1Ic4&lang=zh-CN").arg(ip));
	QNetworkRequest request(url);

	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
	request.setRawHeader("origin", "https://members.ip-api.com");
	request.setRawHeader("referer", "https://members.ip-api.com/");

	QNetworkAccessManager* manager = ConfigManager::manager();
	QNetworkReply* reply = manager->get(request);

	QObject::connect(reply, &QNetworkReply::finished, [=]() {
		QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
		if (reply->error() == QNetworkReply::NoError) {
			QByteArray responseData = reply->readAll();
			qDebug() << "Raw response:" << responseData;

			QJsonParseError parseError;
			QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData, &parseError);
			if (parseError.error == QJsonParseError::NoError && jsonDoc.isObject()) {
				QJsonObject jsonObj = jsonDoc.object();
				emit updateSignal(statusCode.toString(), jsonObj);
			}
			else {
				qDebug() << "JSON 解析失败:" << parseError.errorString();
			}
		}
		else {
			qDebug() << "请求失败:" << reply->errorString();
			emit updateSignal("请求失败", QJsonObject());
		}

		reply->deleteLater();
		});
}


void ip_tool::query() {
	sendRequest(ui.lineEdit->text().trimmed());
}


void ip_tool::showWindow() {
	this->show();
	sendRequest("");
}

