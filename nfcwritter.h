#ifndef NFCWRITTER_H
#define NFCWRITTER_H

#include "nfcbaseclass.h"

/**
 * @brief Класс записывающий информацию на метку
 */
class NFCWritter: public NFCBaseClass
{
    Q_OBJECT
public:
    /**
     * @brief Конструктор класса
     */
    NFCWritter(NFCInfo &nfcInfo,QObject *parent);
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
signals:
    /**
     * @brief Сигнал, что зажигаеться при
     * завершении процесса записи на метку
     */
    void WritingComplete();
};

#endif // NFCWRITTER_H
