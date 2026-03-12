#include "nfcinterface.h"
#include "ui_nfcinterface.h"
#include <QMessageBox>
#include <QDebug>
#include <QDate>
#include <QScreen>

NFCInterface::NFCInterface(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NFCInterface),
    detectionTimer(new QTimer(this)),
    isReadingPressed(false),
    isWritingPressed(false)
{
    ui->setupUi(this);

    _nfcInfo = new NFCInfo(this);
    _nfcModule= new NFCModule(this, *_nfcInfo);

    // Подключаем сигналы от NFC модуля
    connect(_nfcModule, &NFCModule::ReadingComplete,
            this, &NFCInterface::onNFCReadComplete);
    connect(_nfcModule, &NFCModule::ReadingFail,
            this, &NFCInterface::onNFCReadFailed);
    connect(_nfcModule, &NFCModule::WritingComplete,
            this, &NFCInterface::onNFCWriteComplete);
    connect(_nfcModule, &NFCModule::WrittingFail,
            this, &NFCInterface::onNFCWriteFailed);

    // Настройка окна для Android
    setWindowTitle("NFC Метка");

    //Указываем на NFCInfo NFC модуля

    QRect screenGeometry = QApplication::primaryScreen()->availableGeometry();
    setFixedSize(screenGeometry.size());

    // Настройка таймера
    detectionTimer->setSingleShot(true);
    detectionTimer->setInterval(5000);

    connect(detectionTimer, &QTimer::timeout,
            this, &NFCInterface::SimulatedTagDetected);

    // Установка текущей даты
    ui->lineEditDate->setText(QDate::currentDate().toString("dd.MM.yyyy"));

    // Применяем стили
    ApplyStyles();

    // Адаптируем
    AdjustLayout();

    // Установка базового статуса
    UpdateStatus("Готов к работе");
}

NFCInterface::~NFCInterface()
{
    delete ui;
}

void NFCInterface::AdjustLayout()
{
    int w = width();
    int h = height();

    qDebug() << "Adjusting layout for size:" << w << "x" << h;

    // Отступы от краев
    int margin = w * 0.03;  // 3% от ширины

    // Высота полей ввода
    int formHeight = h * 0.5;  // 50% высоты под форму

    // Проверяем, что виджеты существуют
    if (!ui->verticalLayoutWidget || !ui->labelStatus || !ui->horizontalLayoutWidget) {
        qDebug() << "Error: Widgets not found!";
        return;
    }

    // Устанавливаем геометрию для формы с параметрами
    ui->verticalLayoutWidget->setGeometry(
        margin,                  // x
        margin,                  // y
        w - 2 * margin,          // ширина
        formHeight               // высота
    );

    // Статусная строка (располагаем под формой)
    int statusY = margin + formHeight + margin;
    int statusHeight = h * 0.08;  // 8% высоты

    ui->labelStatus->setGeometry(
        margin,                  // x
        statusY,                 // y
        w - 2 * margin,          // ширина
        statusHeight             // высота
    );

    // Кнопки (внизу экрана)
    int buttonY = h - margin - h * 0.1;  // 10% высоты от низа
    int buttonHeight = h * 0.08;          // 8% высоты

    ui->horizontalLayoutWidget->setGeometry(
        margin,                  // x
        buttonY,                 // y
        w - 2 * margin,          // ширина
        buttonHeight             // высота
    );

    qDebug() << "Layout adjusted - Form at y:" << margin
             << "Status at y:" << statusY
             << "Buttons at y:" << buttonY;
}

void NFCInterface::ApplyStyles()
{
    int w = width();
    int h = height();

    // Базовые размеры шрифтов (относительно экрана)
    int baseFontSize = qMax(14, h / 35);  // Минимум 14px
    int buttonFontSize = qMax(16, h / 30);

    QString styleSheet = QString(R"(
        QMainWindow {
            background-color: #f5f5f5;
        }

        QLabel#labelStatus {
            background-color: #E3F2FD;
            border: 2px solid #2196F3;
            border-radius: %1px;
            font-size: %2px;
            font-weight: bold;
            qproperty-alignment: AlignCenter;
            min-height: %3px;
        }

        QLabel#labelStatus[error="true"] {
            background-color: #FFEBEE;
            border-color: #F44336;
            color: #C62828;
        }

        QLabel#labelStatus[success="true"] {
            background-color: #E8F5E9;
            border-color: #4CAF50;
            color: #2E7D32;
        }

        QPushButton {
            font-size: %4px;
            font-weight: bold;
            border: 2px solid #333333;
            border-radius: %5px;
            min-height: %6px;
        }

        QPushButton#pushButtonRead {
            background-color: #2196F3;
            color: white;
        }

        QPushButton#pushButtonRead:pressed {
            background-color: #1976D2;
        }

        QPushButton#pushButtonWrite {
            background-color: #4CAF50;
            color: white;
        }

        QPushButton#pushButtonWrite:pressed {
            background-color: #388E3C;
        }

        QLineEdit {
            font-size: %2px;
            border: 1px solid #333333;
            border-radius: %5px;
            padding: %7px;
            background-color: white;
            min-height: %8px;
        }

        QLabel {
            font-size: %2px;
            font-weight: bold;
            color: #333333;
        }

        QWidget#verticalLayoutWidget, QWidget#horizontalLayoutWidget {
            background-color: transparent;
        }
    )")
    .arg(h / 80)             // %1: радиус статуса
    .arg(baseFontSize)       // %2: базовый размер шрифта
    .arg(h / 20)             // %3: мин высота статуса
    .arg(buttonFontSize)     // %4: размер шрифта кнопок
    .arg(w / 60)             // %5: радиус скругления
    .arg(h / 15)             // %6: мин высота кнопок
    .arg(h / 120)            // %7: padding полей
    .arg(h / 25);            // %8: мин высота полей

    this->setStyleSheet(styleSheet);
}
void NFCInterface::UpdateStatus(const QString &message, bool isError)
{
    ui->labelStatus->setText(message);

    ui->labelStatus->setProperty("error", false);
    ui->labelStatus->setProperty("success", false);

    if (isError)
    {
        ui->labelStatus->setProperty("error", true);
    }
    else if (message.contains("успешно") || message.contains("Готов"))
    {
        ui->labelStatus->setProperty("success", true);
    }

    // Обновляем стиль
    ui->labelStatus->style()->unpolish(ui->labelStatus);
    ui->labelStatus->style()->polish(ui->labelStatus);
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
        UpdateStatus("Ошибка: Заполните все поля!", true);
        return false;
    }

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
        UpdateStatus("Ошибка: Номер должен содержать только цифры", true);
        return false;
    }

    QRegExp dateRegex("\\d{2}\\.\\d{2}\\.\\d{4}");
    if (!dateRegex.exactMatch(ui->lineEditDate->text()))
    {
        UpdateStatus("Ошибка: Неверный формат даты (ДД.ММ.ГГГГ)", true);
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

    // Адаптивный размер диалога
    QRect screenGeometry = QApplication::primaryScreen()->availableGeometry();
    messageBox.setMinimumSize(screenGeometry.width() * 0.8,
                              screenGeometry.height() * 0.3);

    messageBox.setStyleSheet("QLabel{font-size: 18px; min-width: 300px;} QPushButton{font-size: 16px; padding: 10px;}");
    messageBox.exec();
}

void NFCInterface::ResetToDefault()
{
    // Отменяем NFC операции если они ещё выполняются
    if (isReadingPressed) {
        _nfcModule->CancelReading();
    }
    if (isWritingPressed) {
        _nfcModule->CancelWriting();
    }

    isReadingPressed = false;
    isWritingPressed = false;
    detectionTimer->stop();
    UpdateStatus("Готов к работе");

    ui->pushButtonRead->setStyleSheet("");
    ui->pushButtonWrite->setStyleSheet("");
}

void NFCInterface::on_pushButtonRead_pressed()
{
    if (isWritingPressed)
    {
        UpdateStatus("Сначала отпустите кнопку ЗАПИСЬ", true);
        return;
    }

    qDebug() << "Кнопка чтения нажата";
    isReadingPressed = true;
    UpdateStatus("Поднесите устройство к NFC метке для чтения");

    ui->pushButtonRead->setStyleSheet("background-color: #1976D2;");

    // Запускаем поиск метки
    try {
        _nfcModule->StartReading();
        detectionTimer->start(); // Таймер на случай если метка не найдена
    } catch (const std::string &e) {
        UpdateStatus(QString("Ошибка NFC: %1").arg(e.c_str()), true);
        ResetToDefault();
    }
}

void NFCInterface::on_pushButtonRead_released()
{
    if (!isReadingPressed) return;

    qDebug() << "Кнопка чтения отпущена";

    // Возвращаем обычный стиль
    ui->pushButtonRead->setStyleSheet("");

    ResetToDefault();
}

void NFCInterface::on_pushButtonWrite_pressed()
{
    if (isReadingPressed)
    {
        UpdateStatus("Сначала отпустите кнопку ЧТЕНИЕ", true);
        return;
    }

    if (!ValidateInputs())
    {
        return;
    }

    qDebug() << "Кнопка записи нажата";
    isWritingPressed = true;
    UpdateStatus("Поднесите устройство к NFC метке для записи");

    ui->pushButtonWrite->setStyleSheet("background-color: #388E3C;");

    // Запускаем поиск метки
    try {
        _nfcModule->StartWriting();
        detectionTimer->start(); // Таймер на случай если метка не найдена
    } catch (const std::string &e) {
        UpdateStatus(QString("Ошибка NFC: %1").arg(e.c_str()), true);
        ResetToDefault();
    }
}

void NFCInterface::on_pushButtonWrite_released()
{
    if(!isWritingPressed) return;

    qDebug() << "Кнопка записи отпущена";

    // Возвращаем обычный стиль
    ui->pushButtonWrite->setStyleSheet("");

    ResetToDefault();
}

void NFCInterface::SimulatedTagDetected()
{
    if(!isReadingPressed && !isWritingPressed)
    {
        ResetToDefault();
        return;
    }

    // Останавливаем таймер - метка найдена
       detectionTimer->stop();

    if (isReadingPressed)
    {
        UpdateStatus("Метка обнаружена, чтение данных...");

        // Запускаем реальное чтение
        _nfcModule->StartReading();
    }
    else if (isWritingPressed)
    {
        UpdateStatus("Метка обнаружена, запись данных...");

        try {
            _nfcInfo->SetName(ui->lineEditName->text());
            _nfcInfo->SetSerialNumber(ui->lineEditNumber->text());
            _nfcInfo->SetDescription(ui->lineEditDescription->text());
            _nfcInfo->SetComment(ui->lineEditComment->text());
            _nfcInfo->SetDate(QDate::fromString(ui->lineEditDate->text(), "dd.MM.yyyy"));

            _nfcModule->StartWriting();
        }  catch (const std::string &e) {
            UpdateStatus(QString("Ошибка: %1").arg(e.c_str()), true);
            ResetToDefault();
        }



        UpdateStatus("Запись завершена");
    }
}

void NFCInterface::onNFCReadComplete()
{
    // Обновляем поля из прочитанных данных
    ui->lineEditName->setText(_nfcInfo->property("_name").toString());
    ui->lineEditNumber->setText(_nfcInfo->property("_serialNumber").toString());
    ui->lineEditDescription->setText(_nfcInfo->property("_description").toString());
    ui->lineEditComment->setText(_nfcInfo->property("_comment").toString());

    // Дата может требовать специальной обработки
    QDate date = _nfcInfo->property("_date").toDate();
    if (date.isValid()) {
        ui->lineEditDate->setText(date.toString("dd.MM.yyyy"));
    }

    UpdateStatus("Чтение успешно завершено");
    ShowResultDialog("Успех", "Данные успешно прочитаны с NFC метки");
    ResetToDefault();
}

void NFCInterface::onNFCReadFailed()
{
    UpdateStatus("Ошибка чтения метки", true);
    ShowResultDialog("Ошибка", "Не удалось прочитать данные с NFC метки.\n"
                                "Убедитесь, что метка содержит правильные данные.");
    ResetToDefault();
}

void NFCInterface::onNFCWriteComplete()
{
    UpdateStatus("Запись успешно завершена");
    ShowResultDialog("Успех", "Данные успешно записаны на NFC метку");
    ResetToDefault();
}

void NFCInterface::onNFCWriteFailed()
{
    UpdateStatus("Ошибка записи на метку", true);
    ShowResultDialog("Ошибка", "Не удалось записать данные на NFC метку.\n"
                                "Возможно, метка защищена от записи или повреждена.");
    ResetToDefault();
}

void NFCInterface::onNFCTimeout()
{
    if (isReadingPressed) {
        UpdateStatus("Время ожидания метки истекло", true);
        _nfcModule->CancelReading();
    } else if (isWritingPressed) {
        UpdateStatus("Время ожидания метки истекло", true);
        _nfcModule->CancelWriting();
    }

    ShowResultDialog("Информация", "Метка не найдена. Повторите попытку.");
    ResetToDefault();
}
