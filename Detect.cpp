//---------------------------------------------------------------------------
#include <stdio.h>
#include <vcl.h>
#include <registry.hpp>
#pragma hdrstop

#include "Detect.h"
#include "A2S2.h"
#pragma package(smart_init)

bool EtatDetect = false;
bool EtatDetectAvant = false;
//---------------------------------------------------------------------------

//   Important : les m�thodes et les propri�t�s des objets de la CVL peuvent uniquement
//   �tre utilis�es dans une m�thode appel�e en utilisant Synchronize, comme suit :
//
//      Synchronize(&UpdateCaption);
//
//   o� UpdateCaption serait de la forme :
//
//      void __fastcall TestCarte::UpdateCaption()
//      {
//        Form1->Caption = "Mis � jour dans un thread";
//      }
//---------------------------------------------------------------------------

__fastcall TestCarte::TestCarte(bool CreateSuspended)
	: TThread(CreateSuspended)
{

}
//---------------------------------------------------------------------------
void TestCarte::SetName()
{
	THREADNAME_INFO info;
	info.dwType = 0x1000;
	info.szName = "TestArduino";
	info.dwThreadID = -1;
	info.dwFlags = 0;

	__try
	{
		RaiseException( 0x406D1388, 0, sizeof(info)/sizeof(DWORD),(DWORD*)&info );
	}
	__except (EXCEPTION_CONTINUE_EXECUTION)
	{
	}
}
//---------------------------------------------------------------------------
void __fastcall TestCarte::Execute()
{
SetName();
FreeOnTerminate = true;
while (1)
   {
   if(Terminated) break;
   EtatDetectAvant = EtatDetect;
   Synchronize(&Scan_Arduino);
   Sleep(500);
   if (EtatDetectAvant != EtatDetect)
		{
		Form1->Edit1->Clear(); //modification pour r�cup�ration d'�v�nement dans le Form
		Form1->Edit1->Text="1";
		}
   }
}
//---------------------------------------------------------------------------
 void __fastcall TestCarte::Scan_Arduino()
{
TRegistry *registre = new TRegistry();
registre->RootKey = HKEY_LOCAL_MACHINE;
if (registre->OpenKeyReadOnly("SYSTEM\\CurrentControlSet\\Services\\usbser\\Enum") && registre->ValueExists("0"))
	{
	EtatDetect = true;
	}
	else
		{
		EtatDetect = false;
		}
delete registre;
//valable seulement pour 1 carte Arduino, sinon incr�mente...
//HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\usbser\Enum -> 0 -> USB\VID_2341&PID_0042\6493633303735151D061
//HKEY_LOCAL_MACHINE\HARDWARE\DEVICEMAP\SERIALCOMM -> \Device\USBSER000 -> COMxx
 }