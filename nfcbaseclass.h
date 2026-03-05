#ifndef NFCBASECLASS_H
#define NFCBASECLASS_H

#include <QNearFieldManager>
#include <QNearFieldTarget>
#include <QNdefMessage>

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
    virtual void StartMarkInteraction()=0;
    /**
     * @brief Прерывание взаимодействия с меткой
     */
    void EndMarkInteraction();

    /**
     * @brief Функция, что выполняет действия при потери
     * соединения с меткой
     */
    void handleTargetLost();

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
