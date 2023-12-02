#ifndef Window_H
#define Window_H

#include <QtWidgets/QWidget>
#include <QMessageBox>
#include <iostream>
#include "dialog.h"
#include "ui_mainWindow.h"

class Window : public QWidget
{
public:
    Window(QWidget* parent = nullptr);
    ~Window();

// Helping
    bool checkInstruction(QString&);

// GUI settings
    void retranslateUi();
    unsigned char getFromMemory(unsigned char);
    void updateMemory(unsigned char, unsigned char);
    unsigned char getRegister(unsigned char);
    void updateRegister(unsigned char, unsigned char);
    void updateCounter(unsigned char);
    void incrementCounter();
    void printToScreen(unsigned char);

// Message Dialogs
    void showError(const QString& title, const QString& text, const QString& informative, const QString& detailed);
    void showHalt();


// Main Functionality
    void loadMemory();
    unsigned char fetch();
    bool execute(unsigned char part1, unsigned char parameters);
    void run(bool oneStep = false);

private slots:
    void clearMemory();
    void clearRegisters();
    void onMemoryCellChanged(unsigned char row, unsigned char column);
    void onRegisterChanged(unsigned char row, unsigned char column);

private:
Q_OBJECT
    Ui::Form* ui;
    InputDialog* input;
};

#endif
