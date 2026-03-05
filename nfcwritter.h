#ifndef NFCWRITTER_H
#define NFCWRITTER_H

#include "nfcbaseclass.h"
/**
 * @brief Класс записывающий информацию на метку
 */
class NFCWritter: public NFCBaseClass
{
public:
    /**
     * @brief Конструктор класса
     */
    NFCWritter();

    /**
     * @brief Перевод из класса NFCInfo в NDEF сообщение
     */
    void TranslateNFCInfoToNDEF();
    /**
     * @brief Запуск взаимодействия с меткой
     */
    void StartMarkInteraction(QNearFieldTarget *target) override;
};

#endif // NFCWRITTER_H
