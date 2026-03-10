#include "nfcwritter.h"

NFCWritter::NFCWritter(NFCInfo &nfcInfo,QObject *parent)
    : NFCBaseClass(nfcInfo, parent)
{
    std::cout<<"Создан врайтер.\n";
}


void NFCWritter::StartMarkInteraction(QNearFieldTarget *target)
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

void NFCWritter::OnMarkInteractionSuccess(const QNdefMessage &message)
{
    emit WritingComplete();
}
