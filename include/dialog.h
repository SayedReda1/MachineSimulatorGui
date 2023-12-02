#ifndef DIALOG_H
#define DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>


class InputDialog : public QDialog
{
public:
    QGridLayout *gridLayout;
    QPushButton *clearBtn;
    QPushButton *closeBtn;
    QPlainTextEdit *inputField;
    QPushButton *loadBtn;
    QSpacerItem *horizontalSpacer;

    InputDialog(QWidget* parent = nullptr);

    void setupUi();

    void retranslateUi();

    void connectUi();

    QString flushStorage();

// Signals
    void onLoadBtnClick();
    void onCloseBtnClick();

private:
    Q_OBJECT
    QString storage;
};

#endif
