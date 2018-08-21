/*
	A2S2 - Scratch 2 Launcher by Arduino

*/
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "A2S2.h"
//---------------------------------------------------------------------------
// extra includes

#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <Windows.h>
#include <setupapi.h>
#include <IniFiles.hpp> // to hanlde the initialization files
#include "Detect.h"
#include <registry.hpp>

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
// global variables

// general checks
bool justBooted = true;

// Initialization stream
TIniFile *INI = new TIniFile(ExtractFilePath(Application->ExeName)+ "a2s2.ini");

// Serial port
int port=0;

// paths to software and such
AnsiString path_scratch2="Scratch 2.exe";
AnsiString path_libraries;
AnsiString path_projects;

// lists
TStringList *Libraries = new TStringList;
TStringList *Projects = new TStringList;
TStringList *Extensions1 = new TStringList;

// CONSTRUCTORS
//---------------------------------------------------------------------------
// constructor for the application
// will perform some basic operations, load the init file, and check for the
// serial port

__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
  // get data from definitions
  version = STR(VERSION);
  codename = STR(CODENAME);

  // check whether the INI file exists, create it otherwise
  if (!FileExists(ExtractFilePath(Application->ExeName)+ "a2s2.ini"))
	 createInitFile();
  // read INI file
  port = INI->ReadInteger("COM port", "port", 0);
  path_scratch2 = INI->ReadString("path Scratch2", "path_scratch2", "Scratch 2.exe");
  path_libraries = INI->ReadString("path Libraries", "path_libraries", ExtractFilePath(Application->ExeName) + "libraries\\");
  path_projects = INI->ReadString("path Projects", "path_projects", ExtractFilePath(Application->ExeName) + "projects\\");
  //path_tools = ExtractFilePath(Application->ExeName) + "tools\\";

  // check the value for the serial port (not needed because of autodetection)
  //Edit1->Clear();

  // use the port number from the file to populate the text field on the UI  (not needed because of autodetection)
  //Edit1->Text = IntToStr(port);

  // add the extensions to the Scratch 2 projects
  Extensions1->Add(".sb2");

  // make a search for possible files in the relevant folders
  SearchEx(path_libraries, Extensions1, Libraries,0);
  SearchEx(path_projects, Extensions1, Projects,1);
  delete Extensions1;

  // check whether the board is ready to be used
  // this thread will be running in the background waiting for a board
  // to be plugged or un plugged from the computer
  TestCarte *ThreadTestArduino = new TestCarte(false);
  Edit1->Text="1"; // this needs to be 1 in order to activate the port detection
				   // XXX: make sure 1 is not the only option

  // tell the other methods that we are ready to go
  justBooted = false;

  // clean the console and patch issues with images
  Image1->Width = 307;
  Image1->Height = 29;
  Label2->Caption = " version: " + version + ", codename: " + codename;
  Label2->Top = Image1->Top + Image1->Height + 5;
  Memo1->Text = "Arduino to Scratch 2 (Win)";
  Memo1->Lines->Add("(c) 2018 Arduino LLC, GPLV3");
  Memo1->Lines->Add("based on work by A. Yorinks and S. Canet");

  // add and resize image
  /*Graphics::TBitmap *bmpSomething = new Graphics::TBitmap;

	try {
		bmpSomething->LoadFromFile("test.png");
		Canvas->Draw(10, 10, bmpSomething);
	}
	__finally
	{
		delete bmpSomething;
	}
    */
}

// METHODS
//---------------------------------------------------------------------------
// burn Firmata Plus on the Arduino Uno

void __fastcall TForm1::Button1Click(TObject *Sender)
{
	port=this->Edit1->Text.ToInt();
	Memo1->Lines->Add("We will try to flash Firmata_plus on your Uno");
	INI->WriteInteger("port COM", "port", port);
	ofstream file_flasher("tools/flash_uno.bat", ios::out | ios::trunc);  // create a batch file to call avrdude
	if(file_flasher)
	{
			Memo1->Lines->Add("file_flasher  created");
			file_flasher << "@echo off\nbreak ON\nrem batch forked by Sébastien CANET\nSET currentpath=%~dp1\ncd %currentpath%\ncls\n.\\avrdude -Cavrdude.conf -v -patmega328p -carduino -P\\" << "\\" << ".\\COM" << port << " -b115200 -D -V -Uflash:w:s2a-FirmataPlus.Uno.hex:i" << "\npause";
			file_flasher.close();
	}
	else Memo1->Lines->Add("** Error: cannot create the UNO flashing script");
	//ShowMessage("cannot open the UNO flashing script");
	ShellExecute(0, 0, L"flash_uno.bat", 0, L"tools" , SW_SHOW );
	// XXX clean this up, we want to capture STDOUT and STDERR, so we need
	//     a thread running in parallel that captures stuff and prints it
	/*if (32 < (int) ShellExecute(0, 0, L"flash_uno.bat", 0, 0 , SW_SHOW ))
		Memo1->Lines->Add("flashing failed, check your board");
	else Memo1->Lines->Add("** flashing worked, board ready to use **");
	*/
}
//---------------------------------------------------------------------------
// run the a2s2 batch file in the background to have your
// Arduino board to communicate with Scratch 2

void __fastcall TForm1::Button2Click(TObject *Sender)
{
	port=this->Edit1->Text.ToInt();
	Memo1->Lines->Add("Opening the Arduino to Scratch bridge");
	INI->WriteInteger("port COM", "port", port);
	ofstream file_server("tools/a2s2_cmd.bat", ios::out | ios::trunc);  // create a batch file to including the call to the python code running the server
	if(file_server)
	{
			Memo1->Lines->Add("file_server  created");
			file_server << "@echo off\nbreak ON\nrem batch forked by Sébastien CANET\nSET currentpath=%~dp1\ncd ..\ncd %currentpath%s2a\ncls\n.\\Python\\python.exe s2a.py COM" << port;
			file_server.close();
	}
	else Memo1->Lines->Add("** Error: cannot create the a2s2_cmd script");
	//ShowMessage("cannot open the UNO flashing script");
	ShellExecute(0, 0, L"a2s2_cmd.bat", 0, L"tools" , SW_SHOW );
	// XXX clean this up, we want to capture STDOUT and STDERR, so we need
	//     a thread running in parallel that captures stuff and prints it
	/*if (32 < (int) ShellExecute(0, 0, L"a2s2_cmd.bat", 0, 0 , SW_SHOW ))
		Memo1->Lines->Add("couldn't launch the server");
	else Memo1->Lines->Add("** server launched and ready to use **");
	*/
}
//---------------------------------------------------------------------------
// check the text entry box and verify it is an integer number
// if not, use the default port number
// TODO: add automatic port recognition, eventually

void __fastcall TForm1::Edit1Change(TObject *Sender)
{
	if (Edit1->Text == "1")
	{
		TRegistry *registre1 = new TRegistry();
		TRegistry *registre2 = new TRegistry();
		registre1->RootKey = HKEY_LOCAL_MACHINE;
		registre2->RootKey = HKEY_LOCAL_MACHINE;
		if (registre1->OpenKeyReadOnly("SYSTEM\\CurrentControlSet\\Services\\usbser\\Enum"))
			{
			if (registre1->ValueExists("0"))
				{
					//this->ImgConnect->Picture->LoadFromFile("connect.bmp");
					registre2->OpenKeyReadOnly("HARDWARE\\DEVICEMAP\\SERIALCOMM");
					port=StrToInt(registre2->ReadString("\\Device\\USBSER000").Delete(1,3));
					Edit1->Clear();
					Edit1->Text=IntToStr(port);
					INI->WriteInteger("port COM", "port", port);
					Memo1->Lines->Add("detected Arduino at port: "+ IntToStr(port));
				}
			else
				{
					//this->ImgConnect->Picture->LoadFromFile("unconnect.bmp");
					port=0;
					Edit1->Clear();
					Edit1->Text=IntToStr(port);
					INI->WriteInteger("port COM", "port", port);
					if (justBooted) {
						Memo1->Lines->Add("No Arduino detected, connect your board to the USB port");
					}
					else {
						ShowMessage("Arduino disconnected or port not found");
						Memo1->Lines->Add("board disconnected");
					}
				}
			}
		delete registre1;
		delete registre2;
	}
}
//---------------------------------------------------------------------------
// launch the executable of Scratch 2 using the extension
// that allows Arduino to communicate with it

void __fastcall TForm1::Button3Click(TObject *Sender)
{
	Memo1->Lines->Add("Opening Scratch 2 (offline)");
	ofstream file_scratch("tools/scratch.bat", ios::out | ios::trunc);  // create a batch file to including the call to scratch
	if(file_scratch)
	{
			Memo1->Lines->Add("file_scratch  created");
			file_scratch << "@echo off\nbreak ON\nrem batch forked by Sébastien CANET\ncls\ncd ..\nSET currentpath=%~dp1\nSET dossier_scratch="<< '"' << path_scratch2.c_str() << '"'<< "\nstart %dossier_scratch% " << '"' << path_libraries.c_str() << "a2s2_en.sb2" << '"' ;
			file_scratch.close();
	}
	else Memo1->Lines->Add("** Error: cannot create the file_scratch script");
	//ShowMessage("cannot create the file_scratch script");
	ShellExecute(0, 0, L"scratch.bat", 0, L"tools" , SW_SHOW );
	// XXX clean this up, we want to capture STDOUT and STDERR, so we need
	//     a thread running in parallel that captures stuff and prints it
	/*if (32 < (int) ShellExecute(0, 0, L"scratch.bat", 0, 0 , SW_SHOW ))
		Memo1->Lines->Add("couldn't launch scratch");
	else Memo1->Lines->Add("** scratch launched and ready to use **");
	*/
}
//---------------------------------------------------------------------------

// UTILS
//---------------------------------------------------------------------------
// initilaize the INI file
// this method creates the INI file in case it doesn't exist

void __fastcall TForm1::createInitFile()
{
  // write the different variables to the file
  INI->WriteInteger("COM port", "port", 0);      // serial port by default
  INI->WriteString("path Scratch2", "path_scratch2", "Scratch 2.exe");
  INI->WriteString("path Libraries", "path_libraries", ExtractFilePath(Application->ExeName) + "libraries\\");
  INI->WriteString("path Projects", "path_projects", ExtractFilePath(Application->ExeName) + "projects\\");
}

//---------------------------------------------------------------------------
// search for files in specific folders given a list of extensions

void __fastcall TForm1::SearchEx(AnsiString FilePath, TStringList * Extensions, TStrings * ListeFichiers, int RangMenu)
{
  TSearchRec Infos_fichier;
  TMenuItem *NewItem = new TMenuItem(this);
  if (!FindFirst(FilePath+"*.*", faAnyFile, Infos_fichier))
  do
  {
	for (int i = 0 ; i < Extensions->Count ; i++)
	{
	  if (ExtractFileExt(Infos_fichier.Name).LowerCase() == Extensions->Strings[i].LowerCase())
	  {
		//ajout dans la liste d'une ligne avec le chemin+nom du fichier trouvé
		ListeFichiers->Add(ExpandFileName(Infos_fichier.Name));
		//nettoie le nom de fichier de son extension sb2 pour le nom seul du fichier, sans le chemin
		AnsiString CleanName = StringReplace(Infos_fichier.Name, ".sb2", "",TReplaceFlags() << rfReplaceAll);
		//rajout d'entrées dans le menu
		NewItem = new TMenuItem(this);
		NewItem->Caption = CleanName;
		NewItem->Name = CleanName;
		//stockage du chemin complet + nom de fichier pour la fonction ExempleClick
		NewItem->Hint = FilePath + Infos_fichier.Name.c_str();
		//XXXNewItem->OnClick=ExempleClick;
		//rajout au bon endroit de l'entrée dans le menu
		//XXXMainMenu1->Items->Items[0]->Items[RangMenu]->Add(NewItem);
	  }
	}
  }
  while(!FindNext(Infos_fichier));
  FindClose(Infos_fichier);
}


