#include "nfcmodule.h"

NFCModule::NFCModule(QObject *parent)
    :QObject(parent)
    ,_nfcInfo(new NFCInfo(this))
    ,_nfcReader(new NFCReader(*_nfcInfo,this))
{
     std::cout<<"Создан NFC модуль";
}
