//---------------------------------------------------------------------------

#ifndef A2S2H
#define A2S2H

#define VERSION  0.0.1
#define CODENAME Ankara		// codenames --> cities of the world
							// 0 - Ankara
							// 1 - Budapest
							// 2 - ...
#define XSTR(x) #x
#define STR(x) XSTR(x)
#define VER STR(VERSION)
#define CN STR(CODENAME)

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.Imaging.pngimage.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TButton *Button1;
	TButton *Button2;
	TButton *Button3;
	TEdit *Edit1;
	TLabel *Label1;
	TPanel *Panel1;
	TMemo *Memo1;
	TImage *Image1;
	TShape *Shape1;
	TLabel *Label2;
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall Edit1Change(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);
private:	// User declarations
	void __fastcall createInitFile();
	void __fastcall SearchEx(AnsiString FilePath, TStringList * Extensions, TStrings * ListeFichiers, int RangMenu);
public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
	AnsiString version;
	AnsiString codename;
    AnsiString path_tools;
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
