#ifndef NFCREADER_H
#define NFCREADER_H

#include "nfcbaseclass.h"

class NFCReader: public NFCBaseClass
{
public:
    /**
     * @brief Конструктор класса
     */
    NFCReader();

    /**
     * @brief Функция перевода из NDEF в NFCInfo
     */
    void TranslateNDEFToNFCInfo();
    /**
     * @brief Запуск взаимодействия с меткой
     */
    void StartMarkInteraction(QNearFieldTarget *target) override;
};

#endif // NFCREADER_H
