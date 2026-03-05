#include "nfcbaseclass.h"

NFCBaseClass::NFCBaseClass(QObject *parent)
    : QObject(parent)
        , _NFCmanager(nullptr)
        , _currentMark(nullptr)
{
   _NFCmanager = new QNearFieldManager(this);

       connect(_NFCmanager, &QNearFieldManager::targetDetected,
               this, &NFCBaseClass::StartMarkInteraction);

       connect(_NFCmanager, &QNearFieldManager::targetLost,
               this, &NFCBaseClass::handleTargetLost);
}

void NFCBaseClass::StartMarkDetection()
{
    if (_NFCmanager->isAvailable())
        {
            _NFCmanager->startTargetDetection();
        }

    else {
        throw  std::string("NFC_Access_Error");
        }
}
