#ifndef NFCMODULE_H
#define NFCMODULE_H


/**
 * @brief Класс модуля, что отвечает за взаимодействия
 * с NFC модулем телефона  
 */
class NFCModule
{
public:
    /**
     * @brief Конструктор класса
     */
    NFCModule();
    /**
     * @brief Запуск процесса взаимодействия 
     */
    void StartInteraction();
    /**
     * @brief Запуск процесса поиска метки
     */
    void StarFieldScanning();
    /**
     * @brief Считывание информации с метки
     */
    void ParseInfo();
    /**
     * @brief Запись на метку
     */
    void WriteInfo();
    /**
     * @brief Перевод из NDEF сообщения в класс NFCInfo
     */
    void TranslateNDEFToNFCInfo();
    

private:
    // NFCWritter _nfcWritter;
    // NFCReader _nfcReader;
};



#endif // NFCMODULE_H
