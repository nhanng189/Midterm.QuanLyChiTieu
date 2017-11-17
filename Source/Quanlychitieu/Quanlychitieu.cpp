// Notepad.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Quanlychitieu.h"
#include <WindowsX.h>
#include <winuser.h>
#include <commctrl.h>
#include <fstream>
#include <string>
#include <locale>
#include <codecvt>
#include <shlobj.h>
#include <ObjIdl.h>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;
using namespace std;

#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment(lib, "ComCtl32.lib")

#define MAX_LOADSTRING 100

void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
void OnPaint(HWND hwnd);
void OnDestroy(HWND hwnd);
BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct);


// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

												// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_QUANLYCHITIEU, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_QUANLYCHITIEU));

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_QUANLYCHITIEU));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_QUANLYCHITIEU);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	HWND hWnd = CreateWindowW(szWindowClass, L"Quản lý chi tiêu", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, 645, 633, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

struct CHITIEU
{
	wstring Loai[100];
	wstring noiDung[100];
	wstring Tien[100];
	int count = 0;
};

HWND stt;
HWND listView;
HWND cmBLoaiChiTieu;
HWND edCNoiDung;
HWND edCTien;
HWND thongKe0;
HWND thongKe1;
HWND thongKe2;
HWND thongKe3;
HWND thongKe4;
HWND thongKe5;
HWND thongKeSum;

CHITIEU chiTieu;
wchar_t* type[6] = { L"Ăn uống", L"Di chuyển", L"Nhà cửa", L"Xe cộ", L"Nhu yếu phẩm", L"Dịch vụ" };
int batDau[6];
int soDo[6];

// Bảng màu cho biểu đồ
int color1[6] = { 255, 255,  51,  51,  51, 255 };
int color2[6] = { 51,  255, 255, 255,  51,  51 };
int color3[6] = { 51,   51,  51, 255, 255, 255 };

class CShape {
public:
	virtual void Draw(HDC hdc) = 0;
	virtual CShape* Create(int a, int b, int c, int d, int e, int f, int g, int h, int i) = 0;
};
class CPie : public CShape {
private:
	int x;
	int y;
	int width;
	int height;
	int degree1;
	int degree2;
	int color1;
	int color2;
	int color3;
public:
	void Draw(HDC hdc) {
		Graphics* graphics = new Graphics(hdc);
		//int lineWidth = 2;
		//Pen* pen = new Pen(Color(255, 0, 0, 0), lineWidth);
		SolidBrush* solidBrush = new SolidBrush(Color(color1, color2, color3));

		//graphics->DrawPie(pen, x, y, width, height, degree1, degree2);
		graphics->FillPie(solidBrush, x, y, width, height, degree1, degree2);

		//delete pen;
		delete solidBrush;
		delete graphics;
	}

	CPie(int a, int b, int c, int d, int e, int f, int g, int h, int i) {
		x = a;
		y = b;
		width = c;
		height = d;
		degree1 = e;
		degree2 = f;
		color1 = g;
		color2 = h;
		color3 = i;
	}

	CShape* Create(int a, int b, int c, int d, int e, int f, int g, int h, int i) {
		return new CPie(a, b, c, d, e, f, g, h, i);
	}

	CPie() {}
};
CShape* shapes[6];

void taoStt(HWND hWnd, HFONT hFont)
{
	stt = CreateWindowEx(0, L"BUTTON", L"", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 20, 13, 590, 100, hWnd, NULL, hInst, NULL);
	stt = CreateWindowEx(0, L"STATIC", L"Thêm một loại chi tiêu", WS_CHILD | WS_VISIBLE | SS_CENTER, 28, 14, 117, 15, hWnd, NULL, hInst, NULL);
	SendMessage(stt, WM_SETFONT, WPARAM(hFont), TRUE);
	stt = CreateWindowEx(0, L"STATIC", L"Loại chi tiêu:", WS_CHILD | WS_VISIBLE | SS_LEFT, 50, 55, 60, 15, hWnd, NULL, hInst, NULL);
	SendMessage(stt, WM_SETFONT, WPARAM(hFont), TRUE);
	stt = CreateWindowEx(0, L"STATIC", L"Nội dung:", WS_CHILD | WS_VISIBLE | SS_LEFT, 200, 55, 60, 15, hWnd, NULL, hInst, NULL);
	SendMessage(stt, WM_SETFONT, WPARAM(hFont), TRUE);
	stt = CreateWindowEx(0, L"STATIC", L"Số tiền:", WS_CHILD | WS_VISIBLE | SS_LEFT, 350, 55, 60, 15, hWnd, NULL, hInst, NULL);
	SendMessage(stt, WM_SETFONT, WPARAM(hFont), TRUE);

	stt = CreateWindowEx(0, L"BUTTON", L"", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 20, 133, 590, 210, hWnd, NULL, hInst, NULL);
	stt = CreateWindowEx(0, L"STATIC", L"Danh sách các chi tiêu", WS_CHILD | WS_VISIBLE | SS_CENTER, 28, 134, 123, 15, hWnd, NULL, hInst, NULL);
	SendMessage(stt, WM_SETFONT, WPARAM(hFont), TRUE);

	stt = CreateWindowEx(0, L"BUTTON", L"", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 20, 363, 590, 205, hWnd, NULL, hInst, NULL);
	stt = CreateWindowEx(0, L"STATIC", L"Thông tin thống kê", WS_CHILD | WS_VISIBLE | SS_CENTER, 28, 364, 105, 15, hWnd, NULL, hInst, NULL);
	SendMessage(stt, WM_SETFONT, WPARAM(hFont), TRUE);
	stt = CreateWindowEx(0, L"STATIC", L"Ăn uống:", WS_CHILD | WS_VISIBLE | SS_LEFT, 72, 405, 60, 15, hWnd, NULL, hInst, NULL);
	SendMessage(stt, WM_SETFONT, WPARAM(hFont), TRUE);
	stt = CreateWindowEx(0, L"STATIC", L"Di chuyển:", WS_CHILD | WS_VISIBLE | SS_LEFT, 72, 425, 60, 15, hWnd, NULL, hInst, NULL);
	SendMessage(stt, WM_SETFONT, WPARAM(hFont), TRUE);
	stt = CreateWindowEx(0, L"STATIC", L"Nhà cửa:", WS_CHILD | WS_VISIBLE | SS_LEFT, 72, 445, 60, 15, hWnd, NULL, hInst, NULL);
	SendMessage(stt, WM_SETFONT, WPARAM(hFont), TRUE);
	stt = CreateWindowEx(0, L"STATIC", L"Xe cộ:", WS_CHILD | WS_VISIBLE | SS_LEFT, 72, 465, 60, 15, hWnd, NULL, hInst, NULL);
	SendMessage(stt, WM_SETFONT, WPARAM(hFont), TRUE);
	stt = CreateWindowEx(0, L"STATIC", L"Nhu yếu phẩm:", WS_CHILD | WS_VISIBLE | SS_LEFT, 72, 485, 80, 15, hWnd, NULL, hInst, NULL);
	SendMessage(stt, WM_SETFONT, WPARAM(hFont), TRUE);
	stt = CreateWindowEx(0, L"STATIC", L"Dịch vụ:", WS_CHILD | WS_VISIBLE | SS_LEFT, 72, 505, 60, 15, hWnd, NULL, hInst, NULL);
	SendMessage(stt, WM_SETFONT, WPARAM(hFont), TRUE);
	stt = CreateWindowEx(0, L"STATIC", L"Tổng cộng:", WS_CHILD | WS_VISIBLE | SS_LEFT, 50, 525, 60, 15, hWnd, NULL, hInst, NULL);
	SendMessage(stt, WM_SETFONT, WPARAM(hFont), TRUE);
	stt = CreateWindowEx(0, L"STATIC", L"Biểu đồ minh họa:", WS_CHILD | WS_VISIBLE | SS_LEFT, 280, 405, 90, 15, hWnd, NULL, hInst, NULL);
	SendMessage(stt, WM_SETFONT, WPARAM(hFont), TRUE);
}
void taoLoai(HWND hWnd, HFONT hFont)
{
	cmBLoaiChiTieu = CreateWindowEx(0, L"COMBOBOX", L"", WS_CHILD | WS_VISIBLE | CBS_DROPDOWN, 
		49, 74, 120, 30, hWnd, (HMENU)ID_TYPE, hInst, NULL);
	SendMessage(cmBLoaiChiTieu, WM_SETFONT, (WPARAM)hFont, TRUE);
	ComboBox_AddString(cmBLoaiChiTieu, type[0]);
	ComboBox_AddString(cmBLoaiChiTieu, type[1]);
	ComboBox_AddString(cmBLoaiChiTieu, type[2]);
	ComboBox_AddString(cmBLoaiChiTieu, type[3]);
	ComboBox_AddString(cmBLoaiChiTieu, type[4]);
	ComboBox_AddString(cmBLoaiChiTieu, type[5]);
}
void taoNoiDung(HWND hWnd, HFONT hFont)
{
	edCNoiDung = CreateWindowEx(0, L"EDIT", NULL,
		WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL,
		199, 74, 120, 21, hWnd, (HMENU)DT_EDITCONTROL, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);
	SendMessage(edCNoiDung, WM_SETFONT, (WPARAM)hFont, TRUE);
}
void taoTien(HWND hWnd, HFONT hFont)
{
	edCTien = CreateWindowEx(0, L"EDIT", NULL,
		WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL,
		349, 74, 120, 21, hWnd, (HMENU)DT_EDITCONTROL, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);
	SendMessage(edCTien, WM_SETFONT, (WPARAM)hFont, TRUE);
}
void taoListView(HWND hWnd, HFONT hFont)
{
	listView = CreateWindowEx(NULL, WC_LISTVIEWW, L"", WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | WS_BORDER | LVS_REPORT,
		49, 170, 530, 150, hWnd, (HMENU)IDC_LISTVIEW, hInst, NULL);
	SendMessage(listView, WM_SETFONT, (WPARAM)hFont, TRUE);

	// Tạo 3 cột
	LVCOLUMN lvColumn1;
	lvColumn1.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn1.fmt = LVCFMT_LEFT;
	lvColumn1.pszText = L"Loại chi tiêu";
	lvColumn1.cx = 150;
	ListView_InsertColumn(listView, 0, &lvColumn1);

	LVCOLUMN lvColumn2;
	lvColumn2.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn2.fmt = LVCFMT_LEFT;
	lvColumn2.pszText = L"Nội dung";
	lvColumn2.cx = 210;
	ListView_InsertColumn(listView, 1, &lvColumn2);

	LVCOLUMN lvColumn3;
	lvColumn3.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn3.fmt = LVCFMT_LEFT;
	lvColumn3.pszText = L"Số tiền";
	lvColumn3.cx = 170;
	ListView_InsertColumn(listView, 2, &lvColumn3);
}
void taoBtn(HWND hWnd, HFONT hFont)
{
	hWnd = CreateWindowEx(0, L"BUTTON", L"Thêm", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 499, 73, 81, 23, hWnd, (HMENU)IDC_BTNTHEM, hInst, NULL);
	SendMessage(hWnd, WM_SETFONT, WPARAM(hFont), TRUE);
}
void taoThongKe(HWND hWnd, HFONT hFont) {
	thongKe0 = CreateWindowEx(0, L"STATIC", L"000", WS_CHILD | WS_VISIBLE | SS_LEFT, 120, 405, 60, 15, hWnd, NULL, hInst, NULL);
	SendMessage(thongKe0, WM_SETFONT, WPARAM(hFont), TRUE);
	thongKe1 = CreateWindowEx(0, L"STATIC", L"000", WS_CHILD | WS_VISIBLE | SS_LEFT, 128, 425, 60, 15, hWnd, NULL, hInst, NULL);
	SendMessage(thongKe1, WM_SETFONT, WPARAM(hFont), TRUE);
	thongKe2 = CreateWindowEx(0, L"STATIC", L"000", WS_CHILD | WS_VISIBLE | SS_LEFT, 122, 445, 60, 15, hWnd, NULL, hInst, NULL);
	SendMessage(thongKe2, WM_SETFONT, WPARAM(hFont), TRUE);
	thongKe3 = CreateWindowEx(0, L"STATIC", L"000", WS_CHILD | WS_VISIBLE | SS_LEFT, 108, 465, 60, 15, hWnd, NULL, hInst, NULL);
	SendMessage(thongKe3, WM_SETFONT, WPARAM(hFont), TRUE);
	thongKe4 = CreateWindowEx(0, L"STATIC", L"000", WS_CHILD | WS_VISIBLE | SS_LEFT, 149, 485, 60, 15, hWnd, NULL, hInst, NULL);
	SendMessage(thongKe4, WM_SETFONT, WPARAM(hFont), TRUE);
	thongKe5 = CreateWindowEx(0, L"STATIC", L"000", WS_CHILD | WS_VISIBLE | SS_LEFT, 118, 505, 60, 15, hWnd, NULL, hInst, NULL);
	SendMessage(thongKe5, WM_SETFONT, WPARAM(hFont), TRUE);
	thongKeSum = CreateWindowEx(0, L"STATIC", L"000", WS_CHILD | WS_VISIBLE | SS_LEFT, 110, 525, 60, 15, hWnd, NULL, hInst, NULL);
	SendMessage(thongKeSum, WM_SETFONT, WPARAM(hFont), TRUE);
}
void docListView()
{
	// Đọc dữ liệu từ file theo cấu trúc:
	//		<Loại 01
	//		<Nội dung 01
	//		<Tiền 01
	//		<Loại 02 ...

	// Lấy đường dẫn Documents của người dùng
	wchar_t my_documents[255];
	HRESULT result = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, my_documents);

	// Chuyen wchar_t* sang string
	wstring ws(my_documents);
	string fileNameStr(ws.begin(), ws.end());

	// Chuyển đường dẫn đến file text thành định dạng thích hợp
	fileNameStr += "\\Data.txt";
	char add = fileNameStr[2];
	for (int i = 0; i < fileNameStr.length(); i++) {
		if (fileNameStr[i] == add) fileNameStr.insert(i + 1, 1, add);
		i++;
	}

	locale loc(locale(), new codecvt_utf8<wchar_t>); // Hỗ trợ UTF-8
	wifstream file(fileNameStr);
	file.imbue(loc);
	wstring line;
	int num = 1;
	while (getline(file, line)) { // Bắt đầu đọc file
		if (num == 1) {
			chiTieu.Loai[chiTieu.count] = line;
			num = 2;
			goto out;
		}
		if (num == 2) {
			chiTieu.noiDung[chiTieu.count] = line;
			num = 3;
			goto out;
		}
		if (num == 3) {
			chiTieu.Tien[chiTieu.count] = line;
			num = 1;
			chiTieu.count++;
			goto out;
		}
	out:;
	}
	chiTieu.count--;

	// Đưa dữ liệu đọc được vào listView
	LV_ITEM lv;
	for (int i = 0; i <= chiTieu.count; i++) {
		lv.mask = LVIF_TEXT | LVIF_PARAM;
		lv.iItem = i;
		lv.iSubItem = 0;
		lv.pszText = (LPWSTR)chiTieu.Loai[i].c_str();
		ListView_InsertItem(listView, &lv);
		lv.pszText = (LPWSTR)chiTieu.noiDung[i].c_str();
		ListView_SetItemText(listView, i, 1, lv.pszText);
		lv.pszText = (LPWSTR)chiTieu.Tien[i].c_str();
		ListView_SetItemText(listView, i, 2, lv.pszText);
	}

	file.close();
}
void xuatListView() {
	locale loc(locale(), new codecvt_utf8<wchar_t>); // Luu file UTF-8

	// Lấy đường dẫn Documents của người dùng
	wchar_t my_documents[255];
	HRESULT result = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, my_documents);

	// Chuyen wchar_t* sang string
	wstring ws(my_documents);
	string fileNameStr(ws.begin(), ws.end());

	// Chuyển đường dẫn thành định dạng đúng đến tập tin xuất
	fileNameStr += "\\Data.txt";
	char add = fileNameStr[2];
	for (int i = 0; i < fileNameStr.length(); i++) {
		if (fileNameStr[i] == add) fileNameStr.insert(i + 1, 1, add);
		i++;
	}

	wofstream outFile;
	outFile.open(fileNameStr);
	outFile.imbue(loc);
	for (int i = 0; i <= chiTieu.count; i++) {
		wstring outLoai(chiTieu.Loai[i]);
		outFile << outLoai << "\n";
		wstring outNoiDung(chiTieu.noiDung[i]);
		outFile << outNoiDung << "\n";
		wstring outTien(chiTieu.Tien[i]);
		if (i == chiTieu.count) outFile << outTien;
		else outFile << outTien << "\n";
	}
	outFile.close();
}
long long tongTheoLoai(int loai)
{
	long long tong = 0;
	wstring Loai = type[loai];
	for (int i = 0; i <= chiTieu.count; i++)
	{
		if (chiTieu.Loai[i] == Loai) {
			long long convert = stoll(chiTieu.Tien[i]);
			tong += convert;
		}
	}
	return tong;
}
void capNhatThongKe() {
	// Hiển thị tổng Tiền của từng loại
	SetWindowText(thongKe0, to_wstring(tongTheoLoai(0)).c_str());
	SetWindowText(thongKe1, to_wstring(tongTheoLoai(1)).c_str());
	SetWindowText(thongKe2, to_wstring(tongTheoLoai(2)).c_str());
	SetWindowText(thongKe3, to_wstring(tongTheoLoai(3)).c_str());
	SetWindowText(thongKe4, to_wstring(tongTheoLoai(4)).c_str());
	SetWindowText(thongKe5, to_wstring(tongTheoLoai(5)).c_str());

	// Hiển thị Tiền tổng cộng
	long long sum = tongTheoLoai(0) + tongTheoLoai(1) + tongTheoLoai(2) + tongTheoLoai(3) + tongTheoLoai(4) + tongTheoLoai(5);
	SetWindowText(thongKeSum, to_wstring(sum).c_str());

	// Tính các thông số cho việc vẽ biểu đồ
	batDau[0] = -90;
	for (int i = 0; i < 6; i++) {
		if (i != 0) batDau[i] = batDau[i - 1] + soDo[i - 1];
		if (i != 5)soDo[i] = tongTheoLoai(i) * 360 / sum;
		if (i == 5) soDo[i] = 360 - soDo[0] - soDo[1] - soDo[2] - soDo[3] - soDo[4];
	}

	// Vẽ biểu đồ
	for (int i = 0; i < 6; i++) {
		shapes[i] = new CPie(360, 400, 142, 142, batDau[i], soDo[i], color1[i], color2[i], color3[i]);
	}
}

GdiplusStartupInput gdiplusStartupInput;
ULONG_PTR           gdiplusToken;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_MSG(hWnd, WM_CREATE, OnCreate);
		HANDLE_MSG(hWnd, WM_COMMAND, OnCommand);
		HANDLE_MSG(hWnd, WM_PAINT, OnPaint);
		HANDLE_MSG(hWnd, WM_DESTROY, OnDestroy);
	case WM_CLOSE: { // Xuất file trước khi đóng chương trình
		xuatListView();
	}	

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

BOOL OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct)
{
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	
	INITCOMMONCONTROLSEX icc;
	icc.dwSize = sizeof(icc);
	icc.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&icc);

	// Lay font he thong
	LOGFONT lf;
	GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
	HFONT hFont = CreateFont(lf.lfHeight, lf.lfWidth,
		lf.lfEscapement, lf.lfOrientation, lf.lfWeight,
		lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
		lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
		lf.lfPitchAndFamily, lf.lfFaceName);

	taoStt(hWnd, hFont);				// Tạo các static trên cửa sổ
	taoLoai(hWnd, hFont);				// Tạo comboBox Loại chi tiêu
	taoNoiDung(hWnd, hFont);			// Tao editControl Nội dung
	taoTien(hWnd, hFont);				// Tao editControl Tiền
	taoBtn(hWnd, hFont);				// Tạo button
	taoThongKe(hWnd, hFont);			// Tạo static cho thống kê
	taoListView(hWnd, hFont);			// Tao list view
	docListView();						// Đọc vào listView
	capNhatThongKe();					// Cập nhật thống kê
	return true;
}

void OnCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify)
{
	switch (id)
	{
	case IDC_BTNTHEM: // Khi ấn nút THÊM, thêm vào listView
	{
		int i = ComboBox_GetCurSel(cmBLoaiChiTieu); // Đọc loại

		// Đọc nội dung
		int sizeNoiDung = GetWindowTextLength(edCNoiDung);
		wchar_t* getNoiDung = NULL;
		getNoiDung = new wchar_t[sizeNoiDung + 1];
		GetWindowText(edCNoiDung, getNoiDung, sizeNoiDung + 1);
		wstring ws1(getNoiDung);

		// Đọc tiền
		int sizeTien = GetWindowTextLength(edCTien);
		wchar_t* getTien = NULL;
		getTien = new wchar_t[sizeTien + 1];
		GetWindowText(edCTien, getTien, sizeTien + 1); 
		wstring ws2(getTien);

		// Kiểm tra Tiền nhập vào hợp lệ (là số)
		bool ws2CheckNum = TRUE;
		for (int i = 0; i < ws2.length(); i++) {
			if (ws2[i] < 48 || ws2[i]>57) ws2CheckNum = FALSE;
		}

		if (ComboBox_GetCurSel(cmBLoaiChiTieu) != -1 && ws1 != L"" && ws2 != L"" && ws2CheckNum == TRUE) { // Kiểm tra nội dung hợp lệ không
			chiTieu.count++; // Thêm vào struct chiTieu
			chiTieu.Loai[chiTieu.count] = type[i]; // Thêm Loại
			chiTieu.noiDung[chiTieu.count] = ws1; // Thêm nội dung
			chiTieu.Tien[chiTieu.count] = ws2; // Thêm tiền

			// Cập nhật listView
			LV_ITEM lv;
			lv.mask = LVIF_TEXT | LVIF_PARAM;
			lv.iItem = chiTieu.count;
			lv.iSubItem = 0;
			lv.pszText = (LPWSTR)chiTieu.Loai[chiTieu.count].c_str();
			ListView_InsertItem(listView, &lv);
			lv.pszText = (LPWSTR)chiTieu.noiDung[chiTieu.count].c_str();
			ListView_SetItemText(listView, chiTieu.count, 1, lv.pszText);
			lv.pszText = (LPWSTR)chiTieu.Tien[chiTieu.count].c_str();
			ListView_SetItemText(listView, chiTieu.count, 2, lv.pszText);

			capNhatThongKe();
			InvalidateRect(hWnd, NULL, FALSE);
		}
		else {
			MessageBox(hWnd, L"Nội dung không hợp lệ, vui lòng kiểm tra lại !!!", L"Xảy ra lỗi", MB_ICONWARNING);
		}
		// Xóa nội dung vừa nhập
		ComboBox_SetCurSel(cmBLoaiChiTieu, -1);
		SetWindowText(edCNoiDung, L"");
		SetWindowText(edCTien, L"");
	}
	break;
	}
}

void OnPaint(HWND stt)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(stt, &ps);

	Graphics* graphics = new Graphics(hdc);

	SolidBrush* solidBrush = new SolidBrush(Color(color1[0], color2[0], color3[0]));
	graphics->FillRectangle(solidBrush, 50, 403, 15, 15);
	solidBrush = new SolidBrush(Color(color1[1], color2[1], color3[1]));
	graphics->FillRectangle(solidBrush, 50, 423, 15, 15);
	solidBrush = new SolidBrush(Color(color1[2], color2[2], color3[2]));
	graphics->FillRectangle(solidBrush, 50, 443, 15, 15);
	solidBrush = new SolidBrush(Color(color1[3], color2[3], color3[3]));
	graphics->FillRectangle(solidBrush, 50, 463, 15, 15);
	solidBrush = new SolidBrush(Color(color1[4], color2[4], color3[4]));
	graphics->FillRectangle(solidBrush, 50, 483, 15, 15);
	solidBrush = new SolidBrush(Color(color1[5], color2[5], color3[5]));
	graphics->FillRectangle(solidBrush, 50, 503, 15, 15);

	delete solidBrush;
	delete graphics;

	// Vẽ biểu đồ
	for (int i = 0; i < 6; i++) {
		shapes[i]->Draw(hdc);
	}

	EndPaint(stt, &ps);
}

void OnDestroy(HWND hwnd)
{
	GdiplusShutdown(gdiplusToken);
	PostQuitMessage(0);
}