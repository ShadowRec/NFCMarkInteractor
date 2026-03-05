#ifndef NFCBASECLASS_H
#define NFCBASECLASS_H

#include <QNearFieldManager>
#include <QNearFieldTarget>
#include <QNdefMessage>
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
     * @brief Конструктор класса
     */
    NFCBaseClass(QObject *parent = nullptr);
    /**
     * @brief Запуск поиска метки
     */
    void StartMarkDetection();
    /**
     * @brief Запуск взаимодействия с меткой
     */
    virtual void StartMarkInteraction(QNearFieldTarget *target)=0;
    /**
     * @brief Прерывание взаимодействия с меткой
     */
    void EndMarkInteraction(QNearFieldTarget *target);

private:
    /**
     * @brief Поле, содержащее менеджер полей NFC
     */
    QNearFieldManager *_NFCmanager;
    /**
     * @brief Полеб, хранящее текущую метку
     */
    QNearFieldTarget *_currentMark;
};

#endif // NFCBASECLASS_H
