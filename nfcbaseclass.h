#ifndef NFCBASECLASS_H
#define NFCBASECLASS_H

#include <QNearFieldManager>
#include <QNearFieldTarget>
#include <QNdefMessage>
#include <QString>
#include <QDebug>
#include <iostream>

/**
 * @brief Базовый класс, содержащий код для запуска и
 * прерывания взаимодействия с метками
 */
class NFCBaseClass:public QObject
{
     Q_OBJECT
public:
    /**
     * @brief Конструктор класса NFCBaseClass
     * @param parent - ссылка на родителя
     */
    NFCBaseClass(QObject *parent = nullptr);
    /**
     * @brief Запуск поиска метки
     */
    void StartMarkDetection();

    /**
     * @brief Остановка поиска метки
     */
    void StopMarkDetection();

protected slots:

    /**
     * @brief Запуск взаимодействия с меткой
     * @param target - ссылка на объект метки
     */
     virtual void StartMarkInteraction(QNearFieldTarget *target)=0;

    /**
     * @brief Функция, что выполняет действия при
     * успешном соединении
     * @param target - ссылка на объект метки
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
