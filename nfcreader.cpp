#include "nfcreader.h"

NFCReader::NFCReader()
{
    std::cout<<"Создан ридер.\n";
}

void NFCBaseClass::StartMarkInteraction(QNearFieldTarget *target)
{
       connect(target, &QNearFieldTarget::ndefMessageRead,
               this, &NFCBaseClass::OnMarkInteractionSuccess);
       connect(target, &QNearFieldTarget::error,
               this, &NFCBaseClass::OnMarkInteractionFail);

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




