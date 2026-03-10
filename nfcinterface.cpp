#include "nfcinterface.h"
#include "ui_nfcinterface.h"
#include <QMessageBox>
#include <QDebug>
#include <QDate>

NFCInterface::NFCInterface(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NFCInterface),
    detectionTimer(new QTimer(this)),
    isReadingPressed(false),
    isWritingPressed(false)

{
    ui->setupUi(this);

    //настройка окна для андройд
    setWindowTitle("NFC Метка");
    showMaximized();

    //Настройка таймера
    detectionTimer->setSingleShot(true);
    detectionTimer->setInterval(5000); //5 секунд имитации поиска

    connect(detectionTimer, &QTimer::timeout, this, &NFCInterface::SimulatedTagDetected);

    //Установка текущей даты в поле "Дата:"
    ui->lineEditDate->setText(QDate::currentDate().toString("dd.MM.yyyy"));

    //Стили если надо
    ApplyStyles();

    //Подключения сигнала от кнопок
    connect(ui->pushButtonRead, &QPushButton::pressed, this, &NFCInterface::OnPushButtonReadPressed);
    connect(ui->pushButtonRead, &QPushButton::released, this, &NFCInterface::OnPushButtonReadReleased);
    connect(ui->pushButtonWrite, &QPushButton::pressed, this, &NFCInterface::OnPushButtonWritePressed);
    connect(ui->pushButtonWrite, &QPushButton::released, this, &NFCInterface::OnPushButtonWriteReleased);

    //Установка базового статуса
    UpdateStatus("Готов к работе");
}

NFCInterface::~NFCInterface()
{
    delete ui;
}

void NFCInterface::ApplyStyles()
{

}

void NFCInterface::UpdateStatus(const QString &message, bool isError)
{
    ui->labelStatus->setText(message);

    ui->labelStatus->setProperty("error", false);
    ui->labelStatus->setProperty("success", false);

    if (isError)
    {
        ui->labelStatus->setProperty("error",true);
    }
    else if (message.contains("успешно") || message.contains("Готов"))
    {
        ui->labelStatus->setProperty("success",true);
    }
}

bool NFCInterface::ValidateInputs()
{
    bool allField = !ui->lineEditName->text().isEmpty() &&
                    !ui->lineEditNumber->text().isEmpty() &&
                    !ui->lineEditComment->text().isEmpty() &&
                    !ui->lineEditDescription->text().isEmpty() &&
                    !ui->lineEditDate->text().isEmpty();

    if (!allField)
    {
        UpdateStatus("Ошибка: Имеются пустые поля", true);
        return false;
    }

    //Проверка номера
    QString number = ui->lineEditNumber->text();
    bool isNumber = true;
    for (QChar c : number)
    {
        if (!c.isDigit())
        {
            isNumber = false;
            break;
        }
    }

    if (!isNumber)
    {
        UpdateStatus("Ошибка: Заводской номер должен содержать только цифры", true);
        return false;
    }

    //Проверка формата даты (дд.мм.гггг)
    QRegExp dateRegex("\\d{2}\\.\\d{2}\\.\\d{4}");
    if (!dateRegex.exactMatch(ui->lineEditDate->text()))
    {
        UpdateStatus("Ошибка: Неверный формат даты", true);
        return false;
    }

    return true;
}

void NFCInterface::ShowResultDialog(const QString &title, const QString &message)
{
    QMessageBox messageBox(this);
    messageBox.setWindowTitle(title);
    messageBox.setText(message);
    messageBox.setIcon(QMessageBox::Information);
    messageBox.setStandardButtons(QMessageBox::Ok);

    //Настройка стиля под телефон
    messageBox.setStyleSheet("QLabel{font-size: 18px; min-width: 300px; min-height: 100px;} QPushButton{font-size: 16px; padding: 10px;}");

    messageBox.exec();
}

void NFCInterface::ResetToDefault()
{
    isReadingPressed = false;
    isWritingPressed = false;
    detectionTimer->stop();
    UpdateStatus("Готов к работе");
}

void NFCInterface::OnPushButtonReadPressed()
{
    if (isWritingPressed)
    {
        UpdateStatus("Отпустите кнопку ЗАПИСЬ", true);
        return;
    }

    qDebug() << "Кнопка чтения нажата";
    isReadingPressed = true;
    UpdateStatus("Поднесите устройство к NFC метке для чтения");
    detectionTimer->start();
}

void NFCInterface::OnPushButtonReadReleased()
{
    if (!isReadingPressed) return;

    qDebug() << "Кнопка чтения отпущена";
    ResetToDefault();
}

void NFCInterface::OnPushButtonWritePressed()
{
    if (isReadingPressed)
    {
        UpdateStatus("Отпустите кнопку ЧТЕНИЕ", true);
        return;
    }

    qDebug() << "Кнопка записи нажата";

    if (!ValidateInputs())
    {
        return;
    }

    isWritingPressed = true;
    UpdateStatus("Поднесите устройство к NFC метче для записи");
    detectionTimer->start();
}

void NFCInterface::OnPushButtonWriteReleased()
{
    if(!isWritingPressed) return;

    qDebug() << "Кнопка записи отпущена";
    ResetToDefault();
}

void NFCInterface::SimulatedTagDetected()
{
    if(!isReadingPressed && !isWritingPressed)
    {
        ResetToDefault();
        return;
    }

    if (isReadingPressed)
    {

    }
    else if (isWritingPressed)
    {

    }

    QTimer::singleShot(500, this, &NFCInterface::ResetToDefault);
}
