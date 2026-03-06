#include "nfcinfo.h"

NFCInfo::NFCInfo(QObject *parent):
    QObject(parent)
{
    std::cout<<"Создан класс NFCInfo.\n";
}

void NFCInfo::SetName(QString value)
{
    if(value.length() <40)
    {
        _name=value;
    }
    else
    {
        throw  std::string("Name_Too_Big");
    }
}

void NFCInfo::SetSerialNumber(QString value)
{
    if(value.length() <8)
    {
        _serialNumber=value;
    }
    else
    {
        throw  std::string("SerialNumber_Too_Big");
    }
}

void NFCInfo::SetDescription(QString value)
{
    if(value.length() <150)
    {
        _description=value;
    }
    else
    {
        throw  std::string("Description_Too_Big");
    }
}

void NFCInfo::SetComment(QString value)
{
    if(value.length() <150)
    {
        _comment=value;
    }
    else
    {
        throw  std::string("Comment_Too_Big");
    }
}

QByteArray NFCInfo::Serialize() const
{
    QJsonObject obj;
        obj["name"] = _name;
        obj["serialNumber"] = _serialNumber;
        obj["description"] = _description;
        obj["comment"] = _comment;
        obj["date"] = _date.toString(Qt::ISODate);

        QJsonDocument doc(obj);
        return doc.toJson();
}

bool NFCInfo::Deserialize(const QByteArray &data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isObject()) {
        return false;
    }

    QJsonObject obj = doc.object();

    _name = obj["name"].toString();
    _serialNumber = obj["serialNumber"].toString();
    _description = obj["description"].toString();
    _comment = obj["comment"].toString();

    QString dateStr = obj["date"].toString();
    if (!dateStr.isEmpty()) {
        _date = QDate::fromString(dateStr, Qt::ISODate);
    }

    return true;
}
