#include "nfcreader.h"

NFCReader::NFCReader(NFCInfo &nfcInfo, QObject *parent)
    : NFCBaseClass(nfcInfo,parent)
{
    qDebug()<<"Создан ридер.\n";
}

void NFCReader::StartMarkInteraction(QNearFieldTarget *target)
{
       qDebug()<<"Начато чтение";
       this->_currentMark = target;
       connect(target, &QNearFieldTarget::ndefMessageRead,
             this, [this](const QNdefMessage &message){
           this->OnMarkInteractionSuccess(message);
       }, Qt::UniqueConnection);

          connect(target, &QNearFieldTarget::error,
                  this, [this](QNearFieldTarget::Error error,
                              const QNearFieldTarget::RequestId &request) {
                      this->OnMarkInteractionFail(error, request);
                  }, Qt::UniqueConnection);

        QNearFieldTarget::RequestId request = target->readNdefMessages();

        if (!request.isValid())
        {
                 qDebug()<<"Ошибка при запросе чтения";
                throw  std::string("NFC_Read_Error");
        }
}

void NFCReader::OnMarkInteractionSuccess(const QNdefMessage &message)
{
    qDebug()<<"NDEG сообщение, количество записей:"<<message.size();

    for (const QNdefRecord &record : message) {
          // Ищем нашу запись
          if (record.typeNameFormat() == QNdefRecord::Mime &&
              record.type() == "application/x-markdata") {
              _nfcInfo->Deserialize(record.payload());
              emit InteractionComplete();
              qDebug()<<"Данные отформатированы";
              return;
          }
      }
}




