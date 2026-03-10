#ifndef NFCBASECLASS_H
#define NFCBASECLASS_H

#include <QNearFieldManager>
#include <QNearFieldTarget>
#include <QNdefMessage>
#include <QString>
#include <QDebug>
#include <iostream>
#include "nfcinfo.h"

/**
 * @brief Базовый класс, содержащий код для запуска и
 * прерывания взаимодействия с метками
 */
class NFCBaseClass:public QObject
{
     Q_OBJECT
     Q_PROPERTY(NFCInfo *_nfcInfo READ GetNfcInfo)
public:
    /**
     * @brief Конструктор класса NFCBaseClass
     * @param parent - ссылка на родителя
     */
    NFCBaseClass(NFCInfo &nfcInfo, QObject *parent = nullptr);
    /**
     * @brief Запуск поиска метки
     */
    void StartMarkDetection();

    /**
     * @brief Остановка поиска метки
     */
    void StopMarkDetection();

    /**
     * @brief Функция считывания поля _nfcInfo
     */
    NFCInfo* GetNfcInfo();

protected slots:

    /**
     * @brief Запуск взаимодействия с меткой
     * @param target - ссылка на объект метки
     */
     virtual void StartMarkInteraction(QNearFieldTarget *target)=0;
    /**
     * @brief При успехе операции
     * @param message NDEF сообщение
     */
    virtual void OnMarkInteractionSuccess(const QNdefMessage &message)=0;

    /**
     * @brief Функция, что выполняет действия при
     * ошибке во время соединения
     */
    void OnMarkInteractionFail(QNearFieldTarget::Error error,
                                       const QNearFieldTarget::RequestId &id);
    /**
     * @brief Функция, что выполняет действия при
     * выходе метки из зоны поиска
     * @param target - ссылка на объект метки
     */
    void EndMarkInteraction(QNearFieldTarget *target);

protected:
     NFCInfo *_nfcInfo;
private:
    /**
     * @brief Поле, содержащее менеджер полей NFC
     */
    QNearFieldManager *_NFCmanager;
    /**
     * @brief Поле, хранящее текущую метку
     */
    QNearFieldTarget *_currentMark;


};

#endif // NFCBASECLASS_H
