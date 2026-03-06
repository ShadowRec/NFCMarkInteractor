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
    NFCWritter();

    /**
     * @brief Перевод из класса NFCInfo в NDEF сообщение
     */
    void TranslateNFCInfoToNDEF();

protected slots:
    /**
     * @brief Функция, что выполняет действия при
     * успешном соединении
     * @param target - ссылка на объект метки
     */

    using NFCBaseClass::OnMarkInteractionSuccess;

    void OnMarkInteractionSuccess();
    /**
     * @brief Запуск взаимодействия с меткой
     * @param target - ссылка на объект метки
     */
    void StartMarkInteraction(QNearFieldTarget *target)
    override;

signals:
    void WritingComplete();
};

#endif // NFCWRITTER_H
