#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QJsonObject>
#include <QNetworkAccessManager>


class ConfigManager : public QObject
{
	Q_OBJECT

public:
	static bool loadConfig();

	static bool saveConfig();

	static bool isFirst();

	static QJsonObject& jsonConfig();

	static const QJsonObject& jsonConfigConst();

	static QNetworkAccessManager* manager();

	static void initNetworkManager(QApplication* app);
private:
	ConfigManager();

	static ConfigManager& instance();

	QString configFilePath() const;

	QJsonObject m_config;

	static QNetworkAccessManager* m_manager;

	bool m_first;
};

#endif
