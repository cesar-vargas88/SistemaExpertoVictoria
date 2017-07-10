// Victoria.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Victoria.h"
#include <stack>
#include <string>
#include <cstdlib>
#include "InferenceEngine.h"
#include "PROPCompiler.h"
#include "PROPNode.h"

using namespace std;

#define MAX_LOADSTRING 100

CPROPCompiler C;
CPROPCompiler* p_C = &C;


// Global Variables:
HINSTANCE hInst;								// current instance
HWND g_hWnd;
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

bool g_bConclusions = false;

// Variables de captura

char *nombre = new char[50];
char *comentario = new char[100];

bool bCheck1;		// Control
bool bCheck2;		// Audio Distribuido
bool bCheck3;		// Video Distribuido
bool bCheck4;		// Iluminación

bool bCheck5;		// Botoneras
bool bCheck6;		// Pantallas tactiles
bool bCheck7;		// Dispositivos moviles

int nCombo1;		// Audio - Calidad
int nCombo2;		// Audio - Zonas
int nCombo3;		// Video - Zonas
int nCombo4;		// Audio - Fuentes
int nCombo5;		// Iluminación
int nCombo6;		// Video - Fuentes
int nCombo7;		// Cantidad Botoneras
int nCombo8;		// Cantidad Pantallas Tactiles
int nCombo9;		// Cantidad Dispositivos móviles

char *cAudioZonas[]				= {"AZ1","AZ2","AZ3","AZ4","AZ5","AZ6","AZ7","AZ8"};
char *cAudioFuentes[]			= {"AF1","AF2","AF3","AF4","AF5","AF6","AF7","AF8"};
char *cAudioCalidad[]			= {"ACMedia","ACAlta","ACProfesional"};

char *cVideoZonas[]				= {"VZ1","VZ2","VZ3","VZ4","VZ5","VZ6","VZ7","VZ8"};
char *cVideoFuentes[]			= {"VF1","VF2","VF3","VF4","VF5","VF6","VF7","VF8"};

char *cIluminacionRegresos[]	= {"IR1","IR2","IR3","IR4","IR5","IR6","IR7","IR8","IR9","IR10","IR11","IR12","IR13","IR14","IR15","IR16","IR17","IR18","IR19","IR20"};

char *cControlBotoneras[]		= {"CB1","CB2","CB3","CB4","CB5","CB6","CB7","CB8","CB9","CB10","CB11","CB12","CB13","CB14","CB15","CB16","CB17","CB18","CB19","CB20"};
char *cControlPantallas[]		= {"CP1","CP2","CP3","CP4","CP5","CP6","CP7","CP8","CP9","CP10","CP11","CP12","CP13","CP14","CP15","CP16","CP17","CP18","CP19","CP20"};
char *cControlMoviles[]		    = {"CM1","CM2","CM3","CM4","CM5","CM6","CM7","CM8","CM9","CM10","CM11","CM12","CM13","CM14","CM15","CM16","CM17","CM18","CM19","CM20"};

char *buffer = new char[500];

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Llamada(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_VICTORIA, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_VICTORIA));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}
//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_VICTORIA));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_VICTORIA);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}
//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   g_hWnd = hWnd;

   return TRUE;
}
//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case IDM_Nuevo_Proyecto:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_Nuevo_Proyecto), hWnd, Llamada);
			break;
		case IDM_Abrir:
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
INT_PTR CALLBACK Llamada(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
		case WM_INITDIALOG:
		{
			HWND hCombo;
			//Calidad Audio
			char *Calidad[] = {"Media", "Alta", "Profesional"};
			for(int i=0; i<3; i++)
			{
				hCombo = GetDlgItem(hDlg, IDC_COMBO1);
				SendMessageA(hCombo, CB_ADDSTRING, 0, (LPARAM)Calidad[i]);
			}
			//Zonas Audio
			char *Zonas[] = {"1","2","3","4","5","6","7","8"};
			for(int i=0; i<8; i++)
			{
				hCombo = GetDlgItem(hDlg, IDC_COMBO2);
				SendMessageA(hCombo, CB_ADDSTRING, 0, (LPARAM)Zonas[i]);
			}
			//Fuentes Audio
			char *Fuentes[] = {"1","2","3","4","5","6","7","8"};
			for(int i=0; i<8; i++)
			{
				hCombo = GetDlgItem(hDlg, IDC_COMBO4);
				SendMessageA(hCombo, CB_ADDSTRING, 0, (LPARAM)Fuentes[i]);
			}
			//Zonas Video
			char *ZonasVideo[] = {"1","2","3","4","5","6","7","8"};
			for(int i=0; i<8; i++)
			{
				hCombo = GetDlgItem(hDlg, IDC_COMBO3);
				SendMessageA(hCombo, CB_ADDSTRING, 0, (LPARAM)ZonasVideo[i]);
			}
			//Fuentes Audio
			char *FuentesVideo[] = {"1","2","3","4","5","6","7","8"};
			for(int i=0; i<8; i++)
			{
				hCombo = GetDlgItem(hDlg, IDC_COMBO6);
				SendMessageA(hCombo, CB_ADDSTRING, 0, (LPARAM)FuentesVideo[i]);
			}
			//Regresos Iluminación
			char *Regresos[] = {"1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16","17","18","19","20"};
			for(int i=0; i<20; i++)
			{
				hCombo = GetDlgItem(hDlg, IDC_COMBO5);
				SendMessageA(hCombo, CB_ADDSTRING, 0, (LPARAM)Regresos[i]);
			}
			//Cantidad de botoneras
			char *CantidadBotoneras[] = {"1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16","17","18","19","20"};
			for(int i=0; i<20; i++)
			{
				hCombo = GetDlgItem(hDlg, IDC_COMBO7);
				SendMessageA(hCombo, CB_ADDSTRING, 0, (LPARAM)CantidadBotoneras[i]);
			}
			//Cantidad de botoneras
			char *CantidadPantallas[] = {"1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16","17","18","19","20"};
			for(int i=0; i<20; i++)
			{
				hCombo = GetDlgItem(hDlg, IDC_COMBO8);
				SendMessageA(hCombo, CB_ADDSTRING, 0, (LPARAM)CantidadPantallas[i]);
			}
			//Cantidad de botoneras
			char *CantidadMoviles[] = {"1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16","17","18","19","20"};
			for(int i=0; i<20; i++)
			{
				hCombo = GetDlgItem(hDlg, IDC_COMBO9);
				SendMessageA(hCombo, CB_ADDSTRING, 0, (LPARAM)CantidadMoviles[i]);
			}
			//si hay conclusiones imprimelas luego luego
			if(g_bConclusions)
			SendMessageA(GetDlgItem(hDlg, IDC_EDIT32), WM_SETTEXT, 0, (LPARAM)buffer);
			g_bConclusions = false;
			return (INT_PTR)TRUE;
		}
		break;
		case WM_COMMAND:
		{
			//////////////////////////////////
			//	   Deshabilita funciones	//
			//////////////////////////////////

			// Audio distribuido
			if(LOWORD(wParam) == IDC_CHECK2)
			{
				bCheck2 = SendMessageA(GetDlgItem(hDlg, IDC_CHECK2), BM_GETCHECK, 0, 0);		
				EnableWindow(GetDlgItem(hDlg, IDC_COMBO1), bCheck2);
				EnableWindow(GetDlgItem(hDlg, IDC_COMBO2), bCheck2);
				EnableWindow(GetDlgItem(hDlg, IDC_COMBO4), bCheck2);
			}
			// Video distribuido
			if(LOWORD(wParam) == IDC_CHECK3)
			{
				bCheck3 = SendMessageA(GetDlgItem(hDlg, IDC_CHECK3), BM_GETCHECK, 0, 0);
				EnableWindow(GetDlgItem(hDlg, IDC_COMBO3), bCheck3);
				EnableWindow(GetDlgItem(hDlg, IDC_COMBO6), bCheck3);
			}
			// Iluminación
			if(LOWORD(wParam) == IDC_CHECK4)
			{
				bCheck4 = SendMessageA(GetDlgItem(hDlg, IDC_CHECK4), BM_GETCHECK, 0, 0);
				EnableWindow(GetDlgItem(hDlg, IDC_COMBO5), bCheck4);
			}
			// Control
			if(LOWORD(wParam) == IDC_CHECK1)
			{
				bCheck1 = SendMessageA(GetDlgItem(hDlg, IDC_CHECK1), BM_GETCHECK, 0, 0);
				EnableWindow(GetDlgItem(hDlg, IDC_CHECK5), bCheck1);
				EnableWindow(GetDlgItem(hDlg, IDC_CHECK6), bCheck1);
				EnableWindow(GetDlgItem(hDlg, IDC_CHECK7), bCheck1);
				bCheck5 = SendMessageA(GetDlgItem(hDlg, IDC_CHECK5), BM_GETCHECK, 0, 0);
				bCheck6 = SendMessageA(GetDlgItem(hDlg, IDC_CHECK6), BM_GETCHECK, 0, 0);
				bCheck7 = SendMessageA(GetDlgItem(hDlg, IDC_CHECK7), BM_GETCHECK, 0, 0);
				EnableWindow(GetDlgItem(hDlg, IDC_COMBO7), bCheck5);
				EnableWindow(GetDlgItem(hDlg, IDC_COMBO8), bCheck6);
				EnableWindow(GetDlgItem(hDlg, IDC_COMBO9), bCheck7);
			}
			// Control - Botoneras
			if(LOWORD(wParam) == IDC_CHECK5)
			{
				bCheck5 = SendMessageA(GetDlgItem(hDlg, IDC_CHECK5), BM_GETCHECK, 0, 0);
				EnableWindow(GetDlgItem(hDlg, IDC_COMBO7), bCheck5);
			}
			// Control - Pantallas
			if(LOWORD(wParam) == IDC_CHECK6)
			{
				bCheck6 = SendMessageA(GetDlgItem(hDlg, IDC_CHECK6), BM_GETCHECK, 0, 0);
				EnableWindow(GetDlgItem(hDlg, IDC_COMBO8), bCheck6);
			}
			// Control - Móviles
			if(LOWORD(wParam) == IDC_CHECK7)
			{
				bCheck7 = SendMessageA(GetDlgItem(hDlg, IDC_CHECK7), BM_GETCHECK, 0, 0);
				EnableWindow(GetDlgItem(hDlg, IDC_COMBO9), bCheck7);
			}
			// Aceptar Nuevo Proyecto
			if (LOWORD(wParam) == IDOK)
			{
				HWND hName;
				hName = GetDlgItem(hDlg, IDC_NAME_PROJECT);

				/////////////////////////////////////////////////////////////////////////
				//	Función mas importante, es el enlace entre visual y el codigo.	   //
				/////////////////////////////////////////////////////////////////////////
				*nombre = 0;
				SendMessageA(hName, /*Destinatario*/
					WM_GETTEXT,     /*Identificador, indique que se va a hacer*/ 
					50,             /*Longitud*/
					(LPARAM)nombre);/*Buffer*/
				if(*nombre == 0)
				{
					MessageBox(hDlg, L"Es necesario introducir el nombre.", 0, 0);
					return (INT_PTR)TRUE;
				}
				hName = GetDlgItem(hDlg, IDC_COMMENTS_PROJECT);
				SendMessageA(hName, /*Destinatario*/
					WM_GETTEXT,     /*Identificador, indique que se va a hacer*/ 
					100,             /*Longitud*/
					(LPARAM)comentario);/*Buffer*/
	
				//finalizar dialogo
				EndDialog(hDlg, LOWORD(wParam));
				//siguiente ventana
				DialogBox(hInst, MAKEINTRESOURCE(IDD_Seleccionar_Sistema), g_hWnd, Llamada);
				return (INT_PTR)TRUE;
			}
			// Aceptar Seleccionar Sistema
			if(LOWORD(wParam) == IDOK2)
			{
				//	Guarda estatus en variables boleanas

				bCheck1 = SendMessageA(GetDlgItem(hDlg, IDC_CHECK1), BM_GETCHECK, 0, 0);
				bCheck2 = SendMessageA(GetDlgItem(hDlg, IDC_CHECK2), BM_GETCHECK, 0, 0);
				bCheck3 = SendMessageA(GetDlgItem(hDlg, IDC_CHECK3), BM_GETCHECK, 0, 0);
				bCheck4 = SendMessageA(GetDlgItem(hDlg, IDC_CHECK4), BM_GETCHECK, 0, 0);
				bCheck5 = SendMessageA(GetDlgItem(hDlg, IDC_CHECK5), BM_GETCHECK, 0, 0);
				bCheck6 = SendMessageA(GetDlgItem(hDlg, IDC_CHECK6), BM_GETCHECK, 0, 0);
				bCheck7 = SendMessageA(GetDlgItem(hDlg, IDC_CHECK7), BM_GETCHECK, 0, 0);

				//	Guarda seleccionado en variables enteras

				nCombo1 = SendMessageA(GetDlgItem(hDlg, IDC_COMBO1), CB_GETCURSEL, 0, 0);	
				nCombo2 = SendMessageA(GetDlgItem(hDlg, IDC_COMBO2), CB_GETCURSEL, 0, 0);	
				nCombo3 = SendMessageA(GetDlgItem(hDlg, IDC_COMBO3), CB_GETCURSEL, 0, 0);	
				nCombo4 = SendMessageA(GetDlgItem(hDlg, IDC_COMBO4), CB_GETCURSEL, 0, 0);	
				nCombo5 = SendMessageA(GetDlgItem(hDlg, IDC_COMBO5), CB_GETCURSEL, 0, 0);	
				nCombo6 = SendMessageA(GetDlgItem(hDlg, IDC_COMBO6), CB_GETCURSEL, 0, 0);	
				nCombo7 = SendMessageA(GetDlgItem(hDlg, IDC_COMBO7), CB_GETCURSEL, 0, 0);	
				nCombo8 = SendMessageA(GetDlgItem(hDlg, IDC_COMBO8), CB_GETCURSEL, 0, 0);	
				nCombo9 = SendMessageA(GetDlgItem(hDlg, IDC_COMBO9), CB_GETCURSEL, 0, 0);	
				
				if( bCheck2 == true & (nCombo1 == -1 || nCombo2 == -1 || nCombo4 == -1))
				{
					MessageBox(hDlg, L"Seleccione todos los campos de Audio Distribuido.", 0, 0);
					return (INT_PTR)TRUE;
				}
				if( bCheck3 == true & (nCombo3 == -1 || nCombo6 == -1))
				{
					MessageBox(hDlg, L"Seleccione todos los campos de Video Distribuido.", 0, 0);
					return (INT_PTR)TRUE;
				}
				if( bCheck4 == true & nCombo5 == -1)
				{
					MessageBox(hDlg, L"Seleccione la cantidad de regresos.", 0, 0);
					return (INT_PTR)TRUE;
				}
				if(bCheck1)
				{
					if( bCheck5 == true & nCombo7 == -1)
					{
						MessageBox(hDlg, L"Seleccione la cantidad de botoneras.", 0, 0);
						return (INT_PTR)TRUE;
					}
					if( bCheck6 == true & nCombo8 == -1)
					{
						MessageBox(hDlg, L"Seleccione la cantidad de Pantallas.", 0, 0);
						return (INT_PTR)TRUE;
					}
					if( bCheck7 == true & nCombo9 == -1)
					{
						MessageBox(hDlg, L"Seleccione la cantidad de Móviles.", 0, 0);
						return (INT_PTR)TRUE;
					}
				}

				//Abre archivo de reglas
				fstream Archivo("..//TextFile1.txt",ios::in);

				if(Archivo.is_open())
				{
					if(C.CompileINF(Archivo))
					{
						CInferenceEngine Inference(C.m_SyntacticTree,p_C);
						Inference.ValidateKnowledge();		// Valiada el conocimiento

						Inference.AllConclusions(Inference.s_Antecedents,Inference.s_Consecuents);
						fstream Knowlegde("..//Significados.txt", ios::in);
						
						//Abre archivo significados
						if(Knowlegde.is_open())
						{
							//Asigna significados
							Inference.AssignRelevantKnowledgeAndMeans(Knowlegde);
						}
						else
						{
							MessageBox(g_hWnd, L"Archivo de significados no encontrado", 0, 0);
						}

						// Agrega datos sensados

						if(bCheck1)
							Inference.TrueAntecedents.push("Control");
						if(bCheck2)
							Inference.TrueAntecedents.push("Audio");
						if(bCheck3)
							Inference.TrueAntecedents.push("Video");
						if(bCheck4)
							Inference.TrueAntecedents.push("Iluminacion");
						if(bCheck5)
							Inference.TrueAntecedents.push("Control_Botoneras");
						if(bCheck6)
							Inference.TrueAntecedents.push("Control_Pantallas");
						if(bCheck7)
							Inference.TrueAntecedents.push("Control_Moviles");
						if(nCombo1 != -1)
							Inference.TrueAntecedents.push(cAudioCalidad[nCombo1]);
						if(nCombo2 != -1)
							Inference.TrueAntecedents.push(cAudioZonas[nCombo2]);
						if(nCombo3 != -1)
							Inference.TrueAntecedents.push(cVideoZonas[nCombo3]);
						if(nCombo4 != -1)
							Inference.TrueAntecedents.push(cAudioFuentes[nCombo4]);
						if(nCombo5 != -1)
							Inference.TrueAntecedents.push(cIluminacionRegresos[nCombo5]);
						if(nCombo6 != -1)
							Inference.TrueAntecedents.push(cVideoFuentes[nCombo6]);
						if(nCombo7 != -1)
							Inference.TrueAntecedents.push(cControlBotoneras[nCombo7]);
						if(nCombo8 != -1)
							Inference.TrueAntecedents.push(cControlPantallas[nCombo8]);
						if(nCombo9 != -1)
							Inference.TrueAntecedents.push(cControlMoviles[nCombo9]);
				
						Inference.VictoriaChaining();				// Aplica fowardchaining
						C.DeleteTree(C.m_SyntacticTree);

						char *c1 = new char[10000];
						int cont = 0;

						sprintf(c1,"Nombre del proyecto: %s\r\n",nombre);
						sprintf(&buffer[cont],"%s", c1);
						cont = cont + strlen(c1);

						sprintf(c1,"Comentarios: %s\r\n\r\n\r\n",comentario);
						sprintf(&buffer[cont],"%s", c1);
						cont = cont + strlen(c1);

						sprintf(c1,"\r\tCantidad\r\t\tDescripción\r\t\t\t\t\t\t\tMarca\r\t\t\t\tModelo\r\n\r\n");
						sprintf(&buffer[cont],"%s", c1);
						cont = cont + strlen(c1);

						// Imprime Conclusiones
						while(Inference.Conclud.size())
						{
							const char *c_str1 = Inference.Conclud.top().c_str( );
							sprintf(c1,"%s\r\n",c_str1);
							sprintf(&buffer[cont],"%s", c1);	
							Inference.Conclud.pop();
							cont = cont + strlen(c1);
						}


						g_bConclusions = true;
					}
				}
				else
					MessageBox(g_hWnd, L"Archivo no encontrado", 0, 0);

				//finalizar dialogo
				EndDialog(hDlg, LOWORD(wParam));
				//siguiente ventana
				DialogBox(hInst, MAKEINTRESOURCE(IDD_Conclusiones), g_hWnd, Llamada);
				return (INT_PTR)TRUE;
			}
			if(LOWORD(wParam) == IDOK3)
			{
				//finalizar dialogo
				EndDialog(hDlg, LOWORD(wParam));
				return (INT_PTR)TRUE;
			}
			if (LOWORD(wParam) == IDCANCEL)
			{
				EndDialog(hDlg, LOWORD(wParam));
				return (INT_PTR)TRUE;
			}
		}
		break;
	}
	return (INT_PTR)FALSE;
}
