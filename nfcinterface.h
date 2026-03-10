#ifndef NFCINTERFACE_H
#define NFCINTERFACE_H

#include <QMainWindow>
#include <QTimer>
#include <QResizeEvent>
#include <QStyle>

namespace Ui {
class NFCInterface;
}

class NFCInterface : public QMainWindow
{
    Q_OBJECT

public:
    NFCInterface(QWidget *parent = nullptr);
    ~NFCInterface();

private slots:
    //Слоты для нажатия, отпускания кнопок
    void OnPushButtonReadPressed();
    void OnPushButtonReadReleased();
    void OnPushButtonWritePressed();
    void OnPushButtonWriteReleased();

    //Слот для имитации обнаружения метки
    void SimulatedTagDetected();

private:
    Ui::NFCInterface *ui;
    QTimer *detectionTimer;
    bool isReadingPressed;
    bool isWritingPressed;

    void UpdateStatus(const QString &message, bool isError = false);
    bool ValidateInputs();
    void ShowResultDialog(const QString &title, const QString &message);
    void ResetToDefault();

    void AdjustLayout();
    //стили возможно
    void ApplyStyles();

};

#endif // NFCINTERFACE_H
