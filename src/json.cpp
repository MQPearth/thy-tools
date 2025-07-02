#include "json.h"
#include <QString>
#include <QLineEdit>
#include <QJsonParseError>
#include <QJsonDocument>

json_tool::json_tool(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    connect(this->ui.sourceEdit, &QTextEdit::textChanged, this, &json_tool::handleUpdateSignal);
}

json_tool::~json_tool()
{
}


void json_tool::handleUpdateSignal() {
    QString rawText = ui.sourceEdit->toPlainText();
    if (rawText == "") {
        ui.targetEdit->clear();
    }
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(rawText.toUtf8(), &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        ui.targetEdit->setPlainText("非法JSON");
        return;
    }

    QString formatted = doc.toJson(QJsonDocument::Indented);
    ui.targetEdit->setPlainText(formatted);

}


void json_tool::showWindow() {
    ui.sourceEdit->clear();
    ui.targetEdit->clear();
    this->show();
}