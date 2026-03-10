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

    // Настройка окна для Android
    setWindowTitle("NFC Метка");

    // Убираем showMaximized() и делаем адаптивный размер
    QRect screenGeometry = QApplication::primaryScreen()->availableGeometry();
    resize(screenGeometry.size());

    // Настройка таймера
    detectionTimer->setSingleShot(true);
    detectionTimer->setInterval(5000);

    connect(detectionTimer, &QTimer::timeout,
            this, &NFCInterface::SimulatedTagDetected);

    // Установка текущей даты
    ui->lineEditDate->setText(QDate::currentDate().toString("dd.MM.yyyy"));

    // Подключение сигналов от кнопок
    connect(ui->pushButtonRead, &QPushButton::pressed,
            this, &NFCInterface::OnPushButtonReadPressed);
    connect(ui->pushButtonRead, &QPushButton::released,
            this, &NFCInterface::OnPushButtonReadReleased);
    connect(ui->pushButtonWrite, &QPushButton::pressed,
            this, &NFCInterface::OnPushButtonWritePressed);
    connect(ui->pushButtonWrite, &QPushButton::released,
            this, &NFCInterface::OnPushButtonWriteReleased);

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

void NFCInterface::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    AdjustLayout();  // При изменении размера перестраиваем
}

void NFCInterface::AdjustLayout()
{
    int w = width();
    int h = height();

    // Отступы от краев
    int margin = w * 0.03;  // 3% от ширины

    // Высота полей ввода (примерно 1/12 экрана на каждое поле)
    int formHeight = h * 0.5;  // 50% высоты под форму

    // Устанавка геометрии для формы с параметрами
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

    qDebug() << "Layout adjusted - Buttons at y:" << buttonY
             << "Button height:" << buttonHeight;
}

void NFCInterface::ApplyStyles()
{
    int w = width();
    int h = height();

    // Базовые размеры шрифтов (относительно экрана)
    int baseFontSize = qMax(12, h / 40);  // Минимум 12px
    int buttonFontSize = qMax(14, h / 35);

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
            font-size: %3px;
            font-weight: bold;
            border: 2px solid black;
            border-radius: %4px;
            background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,
                                             stop: 0 #f6f7fa, stop: 1 #dadbde);
        }

        QPushButton:pressed {
            background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,
                                             stop: 0 #dadbde, stop: 1 #f6f7fa);
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
            border: 1px solid black;
            border-radius: %4px;
            padding: %5px;
            background-color: white;
        }

        QLabel {
            font-size: %2px;
            font-weight: bold;
        }
    )")
    .arg(h / 100)           // %1: радиус статуса
    .arg(baseFontSize)      // %2: базовый размер шрифта
    .arg(buttonFontSize)    // %3: размер шрифта кнопок
    .arg(w / 80)            // %4: радиус скругления
    .arg(h / 150);          // %5: padding

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
    isReadingPressed = false;
    isWritingPressed = false;
    detectionTimer->stop();
    UpdateStatus("Готов к работе");

    // Сбрасываем стили кнопок
    ui->pushButtonRead->setStyleSheet("");
    ui->pushButtonWrite->setStyleSheet("");
}

void NFCInterface::OnPushButtonReadPressed()
{
    if (isWritingPressed)
    {
        UpdateStatus("Сначала отпустите кнопку ЗАПИСЬ", true);
        return;
    }

    qDebug() << "Кнопка чтения нажата";
    isReadingPressed = true;
    UpdateStatus("Поднесите устройство к NFC метке для чтения");

    // Визуальный эффект нажатия
    ui->pushButtonRead->setStyleSheet("background-color: #1976D2;");

    detectionTimer->start();
}

void NFCInterface::OnPushButtonReadReleased()
{
    if (!isReadingPressed) return;

    qDebug() << "Кнопка чтения отпущена";

    // Возвращаем обычный стиль
    ui->pushButtonRead->setStyleSheet("");

    ResetToDefault();
}

void NFCInterface::OnPushButtonWritePressed()
{
    if (isReadingPressed)
    {
        UpdateStatus("Сначала отпустите кнопку ЧТЕНИЕ", true);
        return;
    }

    qDebug() << "Кнопка записи нажата";

    if (!ValidateInputs())
    {
        return;
    }

    isWritingPressed = true;
    UpdateStatus("Поднесите устройство к NFC метке для записи");

    // Визуальный эффект нажатия
    ui->pushButtonWrite->setStyleSheet("background-color: #388E3C;");

    detectionTimer->start();
}

void NFCInterface::OnPushButtonWriteReleased()
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

    if (isReadingPressed)
    {
        UpdateStatus("Метка обнаружена, чтение данных...");
        // Здесь будет реальное чтение
    }
    else if (isWritingPressed)
    {
        UpdateStatus("Метка обнаружена, запись данных...");
        // Здесь будет реальная запись
    }

    QTimer::singleShot(500, this, &NFCInterface::ResetToDefault);
}
