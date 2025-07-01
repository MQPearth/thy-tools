#include "configmanager.h"

#include <QStandardPaths>
#include <QFile>
#include <QJsonDocument>
#include <QDir>
#include <QNetworkAccessManager>
#include <QApplication>

QNetworkAccessManager* ConfigManager::m_manager = nullptr;

ConfigManager::ConfigManager()
{
	m_config = QJsonObject();
	m_first = false;
}

ConfigManager& ConfigManager::instance()
{
	static ConfigManager inst;
	return inst;
}

QString ConfigManager::configFilePath() const
{
	QString configDir = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/thy-tools";
	QDir().mkpath(configDir);
	return configDir + "/config.json";
}

bool ConfigManager::loadConfig()
{
    ConfigManager& self = instance();
    QString path = self.configFilePath();
    QFile file(path);

    if (!file.exists()) {
        // 文件不存在，首次使用
        self.m_first = true;
        self.m_config = QJsonObject();

        // 创建空文件并写入 {}
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QJsonDocument doc(self.m_config);
            file.write(doc.toJson(QJsonDocument::Indented));
            file.close();
            return true;
        }
        return false; // 无法写入
    }

    // 文件存在，尝试读取
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(data, &err);
    if (err.error != QJsonParseError::NoError || !doc.isObject()) {
        return false;
    }

    self.m_config = doc.object();
    self.m_first = false;
    return true;
}

bool ConfigManager::saveConfig()
{
	ConfigManager& self = instance();

	QFile file(self.configFilePath());
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
		return false;
	}

	QJsonDocument doc(self.m_config);
	file.write(doc.toJson(QJsonDocument::Indented));
	file.close();
	return true;
}

bool ConfigManager::isFirst() {

    return instance().m_first;
}

QJsonObject& ConfigManager::jsonConfig()
{
	return instance().m_config;
}

const QJsonObject& ConfigManager::jsonConfigConst()
{
	return instance().m_config;
}

QNetworkAccessManager* ConfigManager::manager()
{
    return instance().m_manager;
}


void ConfigManager::initNetworkManager(QApplication* app)
{
    if (!instance().m_manager) {
        ConfigManager& self = instance();
        self.m_manager = new QNetworkAccessManager(app);
    }
}
