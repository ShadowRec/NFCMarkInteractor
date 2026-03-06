#include "nfcwritter.h"

NFCWritter::NFCWritter()
{
    std::cout<<"Создан врайтер.\n";
}
void NFCBaseClass::StartMarkInteraction(QNearFieldTarget *target)
{

}
void NFCWritter::OnMarkInteractionSuccess()
{
    emit WritingComplete();
}
