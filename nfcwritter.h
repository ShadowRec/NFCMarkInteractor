#ifndef NFCWRITTER_H
#define NFCWRITTER_H

#include "nfcbaseclass.h"

/**
 * @brief Класс записывающий информацию на метку
 */
class NFCWritter: public NFCBaseClass
{
    Q_OBJECT

    Q_PROPERTY(NFCInfo _nfcInfo MEMBER _nfcInfo WRITE SetNFCInfo)
public:
    /**
     * @brief Конструктор класса
     */
    NFCWritter();

    /**
     * @brief Функция для установки значения
     * _nfcInfo
     */
    void SetNFCInfo(NFCInfo *info);

protected slots:

    /**
     * @brief Функция, что выполняет действия при
     * успешном соединении
     * @param target - ссылка на объект метки
     */
    void OnMarkInteractionSuccess(const QNdefMessage &message) override;
    /**
     * @brief Запуск взаимодействия с меткой
     * @param target - ссылка на объект метки
     */
    void StartMarkInteraction(QNearFieldTarget *target)
    override;

private:
    /**
     * @brief Поле хранящее информацию для метки
     */
    NFCInfo* _nfcInfo;

    /**
     * @brief Флаг для проверки на заполненость
     * поля _nfcInfo
     */
    bool _InfoIsNotNull;

signals:
    /**
     * @brief Сигнал, что зажигаеться при
     * завершении процесса записи на метку
     */
    void WritingComplete();
};

#endif // NFCWRITTER_H
