#include "nfcmodule.h"

NFCModule::NFCModule(QObject *parent)
:NFCBaseClass(parent)
{
    _nfcReader = new NFCReader();

}
