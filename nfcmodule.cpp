#include "nfcmodule.h"

NFCModule::NFCModule(QObject *parent, NFCInfo &info)
    :QObject(parent)
    ,_nfcInfo(&info)
    ,_nfcReader(new NFCReader(*_nfcInfo,this))
    ,_nfcWriter(new NFCWritter(*_nfcInfo,this))
{
    connect(_nfcReader, &NFCReader::InteractionComplete,
            this, [this](){
        this->StopReading();
        emit ReadingComplete();
    });

    connect(_nfcWriter,&NFCWritter::InteractionComplete,
            this, [this](){
        this->StopWriting();
        emit WritingComplete();
    });

    connect(_nfcReader, &NFCReader::InteractionError,
            this, [this](){
        this->StopReading();
        emit ReadingFail();
    });

    connect(_nfcWriter,&NFCWritter::InteractionError,
            this, [this](){
        this->StopWriting();
        emit WritingFail();
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
     qDebug()<<"Создан NFC модуль";
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
   _nfcWriter->StartMarkDetection();
}

void NFCModule::StopWriting()
{
    _nfcWriter->StopMarkDetection();
}

void NFCModule::CancelReading()
{
    emit CancelReadingProcess();
}

void NFCModule::CancelWriting()
{
    emit CancelWritingProcess();
}

