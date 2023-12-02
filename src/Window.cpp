#include "Window.h"

// ------- Helping functions ---------
QString addBinary(const std::string& a, const std::string& b)
{
    std::string ans(8, '0');
    int r = 0;
    for (int i = 7; i >= 0; --i)
    {
        int sum = a[i] + b[i] + r - 2 * '0';
        ans[i] += sum % 2;
        r = sum / 2;
    }
    return QString::fromStdString(ans);
}

bool Window::checkInstruction(QString& instruction)
{
    bool ok = true;
    instruction.toInt(&ok, 16);
    if (instruction.size() != 4 || !ok) {
        showError("Instruction Error", "Invalid Entry " + instruction, "Execution stopped",
                  "You should enter 4 digits per line of valid hexadecimal numbers\nTry Again :)");
        return false;
    }
    return true;
}

QString toStr(unsigned char part1, unsigned char part2)
{
    return QString("%1").arg(part1, 2, 16, QChar('0')) + QString("%1").arg(part2, 2, 16, QChar('0'));
}


// -------------------------------- Window ----------------------------------
Window::Window(QWidget* parent)
    : QWidget(parent), ui(new Ui::Form), input(new InputDialog(this))
{
    ui->setupUi(this);

    retranslateUi();
}
Window::~Window()
{
    delete ui;
    delete input;
}

// --------- GUI settings -----------
void Window::retranslateUi()
{
    // ----- Window
    this->setFixedSize(750, 450);


    // CPU Table
    ui->CpuTable->setColumnWidth(0, 60);
    ui->CpuTable->setColumnWidth(1, 100);
    ui->CpuTable->setColumnWidth(2, 40);

    for (int i = 0; i < 16; ++i) {
        ui->CpuTable->item(i, 0)->setText(QString("%1").arg(i, 2, 16, QChar('0')).toUpper());
        ui->CpuTable->setRowHeight(i, 10);

        // Binary and Hex cells
        ui->CpuTable->setItem(i, 1, new QTableWidgetItem("00000000"));
        ui->CpuTable->setItem(i, 2, new QTableWidgetItem("00"));
        ui->CpuTable->item(i, 1)->setData(Qt::UserRole, "00000000");
        ui->CpuTable->item(i, 2)->setData(Qt::UserRole, "00");
    }

    // MemoryTable
    ui->MemoryTable->setColumnWidth(0, 60);
    ui->MemoryTable->setColumnWidth(1, 100);
    ui->MemoryTable->setColumnWidth(2, 40);

    for (int i = 0; i < 256; ++i) {
        ui->MemoryTable->item(i, 0)->setText(QString("%1").arg(i, 2, 16, QChar('0')).toUpper());
        ui->MemoryTable->setRowHeight(i, 10);

        // Binary and Hex cells
        ui->MemoryTable->setItem(i, 1, new QTableWidgetItem("00000000"));
        ui->MemoryTable->setItem(i, 2, new QTableWidgetItem("00"));
        ui->MemoryTable->item(i, 1)->setData(Qt::UserRole, "00000000");
        ui->MemoryTable->item(i, 2)->setData(Qt::UserRole, "00");
    }

    // Tooltips
    ui->CpuResetBtn->setToolTip("Clear registers to default");
    ui->CpuResetBtn->setToolTipDuration(3000);
    ui->MemoryResetBtn->setToolTip("Clear Memory to default");
    ui->MemoryResetBtn->setToolTipDuration(3000);
    ui->MemoryLoadButton->setToolTip("Open instructions loading window");
    ui->MemoryLoadButton->setToolTipDuration(3000);
    ui->SingleStepBtn->setToolTip("Execute only next upcoming instruction");
    ui->SingleStepBtn->setToolTipDuration(3000);
    ui->RunAllBtn->setToolTip("Execute all instructions until halt");
    ui->RunAllBtn->setToolTipDuration(3000);
    ui->OutResetBtn->setToolTip("Clear output screen");
    ui->OutResetBtn->setToolTipDuration(3000);


    // Connections
    connect(ui->CpuResetBtn, &QPushButton::clicked, this, &Window::clearRegisters);
    connect(ui->MemoryResetBtn, &QPushButton::clicked, this, &Window::clearMemory);
    connect(ui->MemoryLoadButton, &QPushButton::clicked, this, &Window::loadMemory);
    connect(ui->RunAllBtn, &QPushButton::clicked, this, &Window::run);
    connect(ui->SingleStepBtn, &QPushButton::clicked, this, [&]() {Window::run(true); });

    // Cells Connections
    connect(ui->MemoryTable, &QTableWidget::cellChanged, this, &Window::onMemoryCellChanged);
    connect(ui->CpuTable, &QTableWidget::cellChanged, this, &Window::onRegisterChanged);
}

void Window::clearRegisters()
{
    for (int i = 0; i < 16; ++i)
    {
        ui->CpuTable->item(i, 1)->setText("00000000");
        ui->CpuTable->item(i, 2)->setText("00");
    }
}

void Window::clearMemory()
{
    for (int i = 0; i < 256; ++i)
    {
        ui->MemoryTable->item(i, 1)->setText("00000000");
        ui->MemoryTable->item(i, 2)->setText("00");
    }
}

void Window::onMemoryCellChanged(unsigned char row, unsigned char column)
{
    int value;
    bool ok;

    QTableWidgetItem* item = ui->MemoryTable->item(row, column);
    value = (column == 1 ? item->text().toInt(&ok, 2) : item->text().toInt(&ok, 16));
    if (!ok || value > 255)
    {
        QApplication::beep();
        item->setText(item->data(Qt::UserRole).toString());
    }
    else
    {
        ui->MemoryTable->item(row, 1)->setText(QString("%1").arg(value, 8, 2, QChar('0')).toUpper());
        ui->MemoryTable->item(row, 2)->setText(QString("%1").arg(value, 2, 16, QChar('0')).toUpper());
        item->setData(Qt::UserRole, item->text());
    }
}

void Window::onRegisterChanged(unsigned char row, unsigned char column)
{
    int value;
    bool ok;

    QTableWidgetItem* item = ui->CpuTable->item(row, column);
    value = (column == 1 ? item->text().toInt(&ok, 2) : item->text().toInt(&ok, 16));
    if (!ok || value > 255)
    {
        QApplication::beep();
        item->setText(item->data(Qt::UserRole).toString());
    }
    else
    {
        ui->CpuTable->item(row, 1)->setText(QString("%1").arg(value, 8, 2, QChar('0')).toUpper());
        ui->CpuTable->item(row, 2)->setText(QString("%1").arg(value, 2, 16, QChar('0')).toUpper());
        item->setData(Qt::UserRole, item->text());
    }
}

unsigned char Window::getFromMemory(unsigned char address)
{
    QString content = ui->MemoryTable->item(address, 2)->text();
    return content.toInt(0, 16);
}

void Window::updateMemory(unsigned char address, unsigned char content)
{
    ui->MemoryTable->item(address, 1)->setText(QString("%1").arg(content, 8, 2, QChar('0')));
    ui->MemoryTable->item(address, 2)->setText(QString("%1").arg(content, 2, 16, QChar('0')).toUpper());
}

unsigned char Window::getRegister(unsigned char address)
{
    QString content = ui->CpuTable->item(address, 2)->text();
    return content.toInt(0, 16);
}

void Window::updateRegister(unsigned char address, unsigned char content)
{
    ui->CpuTable->item(address, 1)->setText(QString("%1").arg(content, 8, 2, QChar('0')));
    ui->CpuTable->item(address, 2)->setText(QString("%1").arg(content, 2, 16, QChar('0')).toUpper());
}

void Window::updateCounter(unsigned char counter)
{
    ui->CounterField->setText(QString("%1").arg(counter, 2, 16, QChar('0')).toUpper());
}

void Window::incrementCounter()
{
    updateCounter(ui->CounterField->text().toInt(0, 16) + 1);
}

void Window::printToScreen(unsigned char c)
{
    if (c > 127)
        ui->OutputScreen->append("Content: " + QString("1%").arg(c, 2, 16, QChar('0')).toUpper());
    else
        ui->OutputScreen->append(QString(char(c)));
}

// Message Boxes
void Window::showError(const QString& title, const QString& text, const QString& informative, const QString& detailed)
{
    QApplication::beep();
    QMessageBox error(this);
    error.setIcon(QMessageBox::Critical);
    error.setWindowTitle(title);
    error.setText(text);
    error.setInformativeText(informative);
    error.setDetailedText(detailed);
    auto okBtn = error.addButton("OK", QMessageBox::ButtonRole::AcceptRole);
    okBtn->setFixedWidth(100);
    error.exec();
}

void Window::showHalt()
{
    QApplication::beep();
    QMessageBox haltMessage(this);
    haltMessage.setWindowTitle("Done");
    haltMessage.setIconPixmap(QPixmap(":/done.ico"));
    haltMessage.setText("Halt is been called...");
    haltMessage.addButton("OK", QMessageBox::ButtonRole::AcceptRole)->setFixedWidth(100);
    haltMessage.exec();
}

// ---------------------------- Machine -------------------------------------
// --------- Main Functions -----------
void Window::loadMemory()
{
    if (input->exec() == QDialog::Accepted)
    {
        unsigned char counter = ui->CounterField->text().toInt(0, 16);
        QStringList Instructions = input->flushStorage().split('\n');
        for (auto& instruction : Instructions)
        {
            if (!checkInstruction(instruction))
                return;

            if (instruction.size() == 4)
            {
                // Part1
                updateMemory(counter, instruction.mid(0, 2).toInt(0, 16));
                counter++;

                // Part2
                updateMemory(counter, instruction.mid(2, 2).toInt(0, 16));
                counter++;
            }
        }
        // if everything went well
        QApplication::beep();
    }
}

unsigned char Window::fetch()
{
    unsigned char content = getFromMemory(ui->CounterField->text().toInt(0, 16));
    incrementCounter();
    return content;
}

bool Window::execute(unsigned char part1, unsigned char parameters)
{
    // decoding
    char opcode = part1 / 16;
    char reg = part1 % 16;

    switch (opcode)
    {
    case 1:
    {
        unsigned char content = getFromMemory(parameters);
        updateRegister(reg, content);
        break;
    }
    case 2:
    {
        updateRegister(reg, parameters);
        break;
    }
    case 3:
    {
        unsigned char content = getRegister(reg);
        if (parameters == 0)
        {
            printToScreen(content);
        }
        else
        {
            updateMemory(parameters, content);
        }
        break;
    }
    case 4:
    {
        // Extracting R & S
        unsigned char R, S;
        S = parameters % 16;
        R = parameters / 16;

        // Assigning S to R
        unsigned char RContent = getRegister(R);
        updateRegister(S, RContent);
        break;
    }
    case 5:
    {
        // Extracting R & S
        unsigned char R, S;
        S = parameters % 16;
        R = parameters / 16;

        // Addition
        QString bin1 = ui->CpuTable->item(R, 1)->text(), bin2 = ui->CpuTable->item(S, 1)->text();
        QString ans = addBinary(bin1.toStdString(), bin2.toStdString());

        // Assigning back
        updateRegister(reg, ans.toInt(0, 2));
        break;
    }
    case 11:    // B
    {
        if (getRegister(0) == getRegister(reg))
        {
            updateCounter(parameters);
        }
        break;
    }
    case 12:    // C
    {
        if (parameters == 0)
        {
            // halt
            showHalt();
            return false;   // Don't continue
        }
    }
    default:
    {
        showError("Instruction error", "Undefined instruction: " + toStr(part1, parameters), "Execution stopped",
                  "Unkown opcode\nConsider checking the list of valid entries in CS Overview, Appendix C");
        return false;   // Don't continue
    }
    }
    return true;
}

void Window::run(bool oneStep)
{
    while (true)
    {
        // fetching
        auto part1 = fetch();
        auto part2 = fetch();

        // decoding and executing
        bool continued = execute(part1, part2);

        // if return not to continue or only one step
        if (!continued || oneStep)
            break;
    }
}
