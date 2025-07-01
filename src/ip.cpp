#include "ip.h"

#include <QtConcurrent>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrl>
#include <QDebug>
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


void ip_tool::handleUpdateSignal(const QJsonObject value) {

	qDebug() << "Parsed JSON:" << value;

	QJsonDocument doc(value);
	QString jsonStr = doc.toJson(QJsonDocument::Indented);
	this->ui.textEdit->clear();
	this->ui.textEdit->insertPlainText(jsonStr);
}


void ip_tool::sendRequest(const QString& ip) {

	QUrl url(QString("https://pro.ip-api.com/json/%1?fields=66842623&key=ipapiq9SFY1Ic4&lang=zh-CN").arg(ip));
	QNetworkRequest request(url);

	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
	request.setRawHeader("origin", "https://members.ip-api.com");
	request.setRawHeader("referer", "https://members.ip-api.com/");

	QNetworkAccessManager* manager = ConfigManager::manager();
	QNetworkReply* reply = manager->get(request);

	QObject::connect(reply, &QNetworkReply::finished, [=]() {
		if (reply->error() == QNetworkReply::NoError) {
			QByteArray responseData = reply->readAll();
			qDebug() << "Raw response:" << responseData;

			QJsonParseError parseError;
			QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData, &parseError);
			if (parseError.error == QJsonParseError::NoError && jsonDoc.isObject()) {
				QJsonObject jsonObj = jsonDoc.object();
				qDebug() << "Parsed JSON:";
				emit updateSignal(jsonObj);
			}
			else {
				qDebug() << "JSON 解析失败:" << parseError.errorString();
			}
		}
		else {
			qDebug() << "请求失败:" << reply->errorString();
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

