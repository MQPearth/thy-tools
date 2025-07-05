#pragma once
#include <QTextEdit>
#include <QMimeData>

class PlainPasteTextEdit : public QTextEdit {
    Q_OBJECT
public:
    explicit PlainPasteTextEdit(QWidget *parent = nullptr) : QTextEdit(parent) {}

protected:
    void insertFromMimeData(const QMimeData *source) override {
        if (source->hasText()) {
            insertPlainText(source->text());
        }
    }
};
