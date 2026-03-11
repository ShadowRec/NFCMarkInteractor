#ifndef NFCMODULE_H
#define NFCMODULE_H

#include <QObject>
#include <nfcreader.h>
#include <nfcwritter.h>
#include <QObject>
#include <QEventLoop>


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
    NFCModule(QObject *parent);

    /**
    * @brief Начать чтение марки
    */
   void StartReading();

   /**
    * @brief Начать запись на марку
    */
   void StartWriting();

   /**
   * @brief Отменить чтение марки
   */
   void CancelWriting();

   /**
    * @brief Отменить запись на марку
    */
   void CancelReading();

   /**
    * @brief Возвращает указатель на поле
    * _nfcInfo
    */
    NFCInfo* GetNfcInfoPointer();

private:

   /**
    * @brief Остановить запись на марку
    */
   void StopWriting();

   /**
   * @brief Остановить чтение марки
   */
   void StopReading();

     NFCInfo *_nfcInfo;
     NFCWritter *_nfcWritter;
     NFCReader *_nfcReader;


signals:
     /**
     * @brief Сигнал для отмены записи
     */
    void CancelWritingProcess();
    /**
     * @brief Сигнал для отмены чтения
     */
    void CancelReadingProcess();
    /**
     * @brief Сигнал, что зажигается при успешной
     *  Записи
     */
    void WritingComplete();
    /**
     * @brief Сигнал, что зажигаеться при успешном
     *  чтении
     */
    void ReadingComplete();

    /**
     * @brief Сигнал, что зажигаеться при ошибке
     *  при чтении
     */
    void ReadingFail();

    /**
     * @brief Сигнал, что зажигаеться при ошибке
     *  при записи
     */
    void WrittinfFail();

    /**
     * @brief Сигнал, что зажигаеться при отмене
     *  чтения
     */
    void ReadingCanceled();
    /**
     * @brief Сигнал, что зажигается при отмене
     *  Записи
     */
    void WritingCanceled();
};



#endif // NFCMODULE_H
