#ifndef NFCREADER_H
#define NFCREADER_H

#include "nfcbaseclass.h"


/**
 * @brief Класс отвечающий за чтение метки
 */
class NFCReader: public NFCBaseClass
{
    Q_OBJECT
public:
    /**
     * @brief Конструктор класса
     */
    NFCReader(NFCInfo &nfcInfo,QObject *parent);

protected slots:
    /**
     * @brief Функция, что выполняет действия при
     * успешном соединении
     * @param target - ссылка на объект метки
     */
    void OnMarkInteractionSuccess(const QNdefMessage &message)
    override;

    /**
     * @brief Запуск взаимодействия с меткой
     * @param target - ссылка на объект метки
     */
     void StartMarkInteraction(QNearFieldTarget *target)
     override;

private:

signals:
     void InfoReadAndFormated();
};





#endif // NFCREADER_H
