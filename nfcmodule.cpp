#include "nfcmodule.h"

NFCModule::NFCModule(QObject *parent, NFCInfo &info)
    :QObject(parent)
    ,_nfcInfo(&info)
    ,_nfcReader(new NFCReader(*_nfcInfo,this))
    ,_nfcWritter(new NFCWritter(*_nfcInfo,this))
{
    connect(_nfcReader, &NFCReader::InteractionComplete,
            this, [this](){
        this->StopReading();
        emit ReadingComplete();
    });

    connect(_nfcWritter,&NFCWritter::InteractionComplete,
            this, [this](){
        this->StopWriting();
        emit WritingComplete();
    });

    connect(_nfcReader, &NFCReader::InteractionError,
            this, [this](){
        this->StopReading();
        emit ReadingFail();
    });

    connect(_nfcWritter,&NFCWritter::InteractionError,
            this, [this](){
        this->StopWriting();
        emit WrittinfFail();
    });

    connect(this, &NFCModule::CancelReadingProcess,
            this, [this](){
        this->StopReading();
        emit ReadingCanceled();
    });

    connect(this, &NFCModule::CancelWritingProcess,
            this, [this](){
        this->StopWriting();
        emit WritingCanceled();
    });
     std::cout<<"Создан NFC модуль";
}

void NFCModule::StartReading()
{
    _nfcReader->StartMarkDetection();
}

void NFCModule::StopReading()
{
   _nfcReader->StopMarkDetection();
}

void NFCModule::StartWriting()
{
   _nfcWritter->StartMarkDetection();
}

void NFCModule::StopWriting()
{
    _nfcWritter->StopMarkDetection();
}

void NFCModule::CancelReading()
{
    emit CancelReadingProcess();
}

void NFCModule::CancelWriting()
{
    emit CancelWritingProcess();
}

 NFCInfo* NFCModule::GetNfcInfoPointer()
{
    return _nfcInfo;
}
