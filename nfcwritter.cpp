#include "nfcwritter.h"

NFCWritter::NFCWritter()
{
    std::cout<<"Создан врайтер.\n";
}
void NFCWritter::StartMarkInteraction(QNearFieldTarget *target)
{
    if(this->_InfoIsNotNull)
    {
          QByteArray jsonData = _nfcInfo->Serialize();

           QNdefRecord record;
           record.setTypeNameFormat(QNdefRecord::Mime);
           record.setType("application/x-markdata");
           record.setPayload(jsonData);

           QNdefMessage message;
           message.append(record);

           QNearFieldTarget::RequestId request =
                 target->writeNdefMessages(QList<QNdefMessage>() << message);

           if (request.isValid()) {
                  qDebug() << "Запись данных начата, размер:" << jsonData.size();
              }
    }

    else
    {
        throw  std::string("NFC_Info_Is_Empty");
    }
}

void NFCWritter::SetNFCInfo(NFCInfo *info)
{
    _nfcInfo = info;
    _InfoIsNotNull=true;
}

void NFCWritter::OnMarkInteractionSuccess(const QNdefMessage &message)
{
    emit WritingComplete();
}
