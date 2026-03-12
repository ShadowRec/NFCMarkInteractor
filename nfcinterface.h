#ifndef NFCINTERFACE_H
#define NFCINTERFACE_H

#include <QMainWindow>
#include <QTimer>
#include <QResizeEvent>
#include <QStyle>
#include <nfcmodule.h>
#include <nfcinfo.h>


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
    //Слот для имитации обнаружения метки
    void SimulatedTagDetected();

    void on_pushButtonRead_pressed();

    void on_pushButtonRead_released();

    void on_pushButtonWrite_pressed();

    void on_pushButtonWrite_released();

    void onNFCReadComplete();
    void onNFCReadFailed();
    void onNFCWriteComplete();
    void onNFCWriteFailed();
    void onNFCTimeout();

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

    NFCModule *_nfcModule;
    NFCInfo *_nfcInfo;


};


#endif // NFCINTERFACE_H
