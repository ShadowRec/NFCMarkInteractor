#include "nfcreader.h"

NFCReader::NFCReader(QObject *parent)
    : NFCBaseClass(parent)
{
    std::cout<<"Создан ридер.\n";
}

void NFCReader::StartMarkInteraction(QNearFieldTarget *target)
{
       connect(target, &QNearFieldTarget::ndefMessageRead,
             this, [this](const QNdefMessage &message){
           this->OnMarkInteractionSuccess(message);
       });

          connect(target, &QNearFieldTarget::error,
                  this, [this](QNearFieldTarget::Error error,
                              const QNearFieldTarget::RequestId &request) {
                      this->OnMarkInteractionFail(error, request);
                  });

        QNearFieldTarget::RequestId request = target->readNdefMessages();

        if (!request.isValid())
        {
                throw  std::string("NFC_Read_Error");
        }
}

void NFCReader::OnMarkInteractionSuccess(const QNdefMessage &message)
{
    std::cout<<"NDEG сообщение, количество записей:"<<message.size();

    for (const QNdefRecord &record : message) {
          // Ищем нашу запись
          if (record.typeNameFormat() == QNdefRecord::Mime &&
              record.type() == "application/x-markdata") {
              _nfcInfo.Deserialize(record.payload());
              emit InfoReadAndFormated();
              return;
          }
      }
}




