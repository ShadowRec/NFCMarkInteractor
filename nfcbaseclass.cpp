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
               this, &NFCBaseClass::EndMarkInteraction);
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

void NFCBaseClass::StopMarkDetection()
{
    if (_NFCmanager->isAvailable())
        {
            _NFCmanager->stopTargetDetection();
        }

    else {
        throw  std::string("NFC_Access_Error");
        }
}


void NFCBaseClass::EndMarkInteraction(QNearFieldTarget *target)
{
    if(_currentMark==target)
    {
        _currentMark=nullptr;
    }
    target->deleteLater();
}

void NFCBaseClass::OnMarkInteractionFail(QNearFieldTarget::Error error,
                                      const QNearFieldTarget::
                                      RequestId &id)
{
    QString errorMessage;
    switch(error) {
       case QNearFieldTarget::NoError:
           errorMessage += "No_error_excep";
           break;
       case QNearFieldTarget::UnsupportedError:
           errorMessage += "Unsupp_op_excep";
           break;
       case QNearFieldTarget::TargetOutOfRangeError:
           errorMessage += "Mark_out_bound_excep";
           break;
       case QNearFieldTarget::NoResponseError:
           errorMessage += "Mark_not_resp_excep";
           break;
       case QNearFieldTarget::ChecksumMismatchError:
           errorMessage += "Check_sum_excep";
           break;
       case QNearFieldTarget::InvalidParametersError:
           errorMessage += "Invalid_param_excep";
           break;
       case QNearFieldTarget::NdefReadError:
           errorMessage += "Read_error_excep";
           break;
        case QNearFieldTarget::NdefWriteError:
            errorMessage += "Write_error_excep";
            break;
       default:
           errorMessage += QString("Unknown_err_code");
       }

       throw std::string(errorMessage.toStdString());
   }
