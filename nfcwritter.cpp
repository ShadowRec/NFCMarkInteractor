#include "nfcwritter.h"

NFCWritter::NFCWritter(NFCInfo &nfcInfo,QObject *parent)
    : NFCBaseClass(nfcInfo, parent)
{
    qDebug()<<"Создан врайтер.\n";
}


void NFCWritter::StartMarkInteraction(QNearFieldTarget *target)
{
        connect(target, &QNearFieldTarget::ndefMessageRead,
             this, [this](const QNdefMessage &message){
            this->OnMarkInteractionSuccess(message);
    }           , Qt::UniqueConnection);

        connect(target, &QNearFieldTarget::error,
                 this, [this](QNearFieldTarget::Error error,
                           const QNearFieldTarget::RequestId &request) {
                   this->OnMarkInteractionFail(error, request);
               }, Qt::UniqueConnection);
          _currentMark=target;
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
           else
           {
               qDebug()<<"Ошибка при запросе чтения";
           }
}

void NFCWritter::OnMarkInteractionSuccess(const QNdefMessage &message)
{
    qDebug() << "Запись данных завершена";
    emit InteractionComplete();
}
