#ifndef NFCMODULE_H
#define NFCMODULE_H

#include <QObject>

/**
 * @brief Класс модуля, что отвечает за взаимодействия
 * с NFC модулем телефона  
 */
class NFCModule:public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Конструктор класса
     */
    NFCModule();

    /**
    * @brief Начать чтение марки
    */
   void StartReading();

   /**
   * @brief Остановить чтение марки
   */
   void StopReading();

   /**
    * @brief Начать запись на марку
    */
   void StartWriting();

   /**
    * @brief Остановить запись на марку
    */
   void StopWriting();


private:
    // NFCWritter _nfcWritter;
    // NFCReader _nfcReader;

signals:
    void StopWritingProcess();

    void StopReadingProcess();
};



#endif // NFCMODULE_H
