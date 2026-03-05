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
     * @brief Считать информацию с метки
     */
    void ReadMark();
    /**
     * @brief Функция перев
     */
    void TranslateNDEFToNFCInfo();
    /**
     * @brief Запуск взаимодействия с меткой
     */
    void StartMarkInteraction() override;
};

#endif // NFCREADER_H
