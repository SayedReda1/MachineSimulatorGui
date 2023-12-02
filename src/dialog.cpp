#include <iostream>
#include "dialog.h"

InputDialog::InputDialog(QWidget* parent) : QDialog(parent)
{
    setupUi();
}

void InputDialog::setupUi()
{
    // Window
    this->setObjectName("this");
    gridLayout = new QGridLayout(this);
    gridLayout->setObjectName("gridLayout");


    // Input Field
    inputField = new QPlainTextEdit(this);
    inputField->setObjectName("inputField");

    // Buttons
    loadBtn = new QPushButton("Load", this);
    loadBtn->setObjectName("loadBtn");
    closeBtn = new QPushButton("Close", this);
    closeBtn->setObjectName("closeBtn");
    clearBtn = new QPushButton("Clear", this);
    clearBtn->setObjectName("clearBtn");

    // Spacer
    horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    retranslateUi();
    connectUi();
}

void InputDialog::retranslateUi()
{

    // Window
    this->resize(350, 500);
    this->setWindowTitle(QCoreApplication::translate("this", "Instructions Input", nullptr));

    // Input Field
    inputField->setFont(QFont("calibri", 12));
    gridLayout->addWidget(inputField, 0, 0, 1, 4);
    loadBtn->setDefault(true);
    inputField->setPlaceholderText(QCoreApplication::translate("this", "Enter Instructions", nullptr));

    // Buttons
    gridLayout->addWidget(loadBtn, 1, 2, 1, 1);
    gridLayout->addWidget(clearBtn, 1, 0, 1, 1);
    gridLayout->addWidget(closeBtn, 1, 3, 1, 1);

    // Spacer
    gridLayout->addItem(horizontalSpacer, 1, 1, 1, 1);
}

void InputDialog::connectUi()
{
    QObject::connect(clearBtn, &QPushButton::clicked, inputField, qOverload<>(&QPlainTextEdit::clear));
    QObject::connect(loadBtn, &QPushButton::clicked, this, qOverload<>(&InputDialog::onLoadBtnClick));
    QObject::connect(closeBtn, &QPushButton::clicked, this, qOverload<>(&InputDialog::onCloseBtnClick));
    QMetaObject::connectSlotsByName(this);
}

void InputDialog::onLoadBtnClick()
{
    this->storage = this->inputField->toPlainText();
    this->accept();
}

void InputDialog::onCloseBtnClick()
{
    this->close();
}

QString InputDialog::flushStorage()
{
    QString temp = storage;
    storage.clear();
    //inputField->clear();    // clear input field after loading

    return temp;
}
