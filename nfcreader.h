#ifndef NFCREADER_H
#define NFCREADER_H

#include "nfcbaseclass.h"
#include "nfcinfo.h"

/**
 * @brief Класс отвечающий за чтение метки
 */
class NFCReader: public NFCBaseClass
{
    Q_OBJECT

      Q_PROPERTY(NFCInfo _nfcInfo MEMBER _nfcInfo)
public:
    /**
     * @brief Конструктор класса
     */
    NFCReader();

    /**
     * @brief Функция перевода из NDEF в NFCInfo
     */
    void TranslateNDEFToNFCInfo();

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
     NFCInfo _nfcInfo;

signals:
     void InfoReadAndFormated();
};





#endif // NFCREADER_H
