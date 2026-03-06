#ifndef NFCINFO_H
#define NFCINFO_H

#include <QObject>
#include <QString>
#include <QDate>
#include <iostream>
#include <QJsonObject>
#include <QJsonDocument>

/**
 * @brief Класс, хранящий информацию для NFC метки
 *
 * Данный класс предназначен для хранения и сериализации данных,
 * которые записываются на NFC метку. Поддерживает основные поля
 * для идентификации трубы и даты установки метки.
 */
class NFCInfo: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString _name MEMBER _name WRITE SetName)
    Q_PROPERTY(QString _serialNumber MEMBER _serialNumber WRITE SetSerialNumber)
    Q_PROPERTY(QString _description MEMBER _description WRITE SetDescription)
    Q_PROPERTY(QDate _date MEMBER _date WRITE SetDate)
    Q_PROPERTY(Qstring  _comment MEMBER _comment WRITE SetComment)
public:

    /**
     * @brief MIME-тип для данных метки
     *
     * Используется для идентификации типа данных при обмене
     * через механизмы drag-and-drop или буфер обмена
     */
    static constexpr const char* MIME_TYPE = "application/x-markdata";

    /**
     * @brief Конструктор класса
     * @param parent Родительский QObject объект
     */
    NFCInfo(QObject *parent = nullptr);

    /**
     * @brief Установить наименование
     * @param value Новое наименование
     */
    void SetName(QString value);

    /**
     * @brief Установить заводской номер
     * @param value Заводской номер трубы
     */
    void SetSerialNumber(QString value);

    /**
     * @brief Установить описание
     * @param value Текстовое описание
     */
    void SetDescription(QString value);

    /**
     * @brief Установить дату
     * @param value Дата в строковом формате
     */
    void SetDate(QString value);

    /**
     * @brief Установить комментарий
     * @param value Текст комментария
     */
    void SetComment(QString value);

    /**
     * @brief Сериализовать данные в байтовый массив
     * @return Байтовый массив с сериализованными данными
     *
     * Преобразует все поля класса в формат, пригодный для
     * сохранения в файл или передачи по сети
     */
       QByteArray Serialize() const;

    /**
     * @brief Десериализовать данные из байтового массива
     * @param data Байтовый массив с данными
     * @return true в случае успешной десериализации, false при ошибке
     *
     * Восстанавливает состояние объекта из ранее сериализованных данных
     */
    bool Deserialize(const QByteArray &data);

private:
    /**
     * @brief Наименование объекта
     *
     * Хранит пользовательское наименование трубы или метки
     */
    QString _name;

    /**
     * @brief Заводской номер трубы
     *
     * Уникальный идентификатор трубы, присвоенный производителем
     */
    QString _serialNumber;

    /**
     * @brief Описание
     *
     * Дополнительная информация о трубе или месте установки
     */
    QString _description;

    /**
     * @brief Комментарий
     *
     * Произвольный текстовый комментарий
     */
    QString _comment;

    /**
     * @brief Дата установки метки
     *
     * Дата, когда NFC метка была установлена на трубу
     */
    QDate _date;
};

#endif // NFCINFO_H
