﻿static char *image_id = 
	"@(#)Copyright (C) H.Shirouzu 2011-2017   image.cpp	Ver4.50";
/* ========================================================================
	Project  Name			: IP Messenger for Win32
	Module Name				: Image
	Create					: 2011-07-24(Mon)
	Update					: 2017-06-12(Mon)
	Copyright				: H.Shirouzu
	Reference				: 
	======================================================================== */

#include "ipmsg.h"
#include <gdiplus.h>
using namespace Gdiplus;

#pragma comment (lib, "msimg32.lib")

//bool GetEncoderClsid(const WCHAR* mime, CLSID* pClsid)
//{
//	ImageCodecInfo* ici = NULL;
//	UINT			num=0, size=0, i=0;
//
//	if (::GetImageEncodersSize(&num, &size) != Ok || size == 0) return false;
//	if (!(ici = (ImageCodecInfo *)malloc(size))) return false;
//
//	if (::GetImageEncoders(num, size, ici) == Ok) {
//		for (i=0; i < num; i++) {
//			if(wcscmp(ici[i].MimeType, mime) == 0) {
//				*pClsid = ici[i].Clsid;
//				break;
//			}
//		}
//	}
//	free(ici);
//
//	return i < num ? true : false;
//}

VBuf *BmpHandleToPngByte(HBITMAP hBmp)
{
	static CLSID	pngid = {0x557cf406,0x1a04,0x11d3, {0x9a,0x73,0x00,0x00,0xf8,0x1e,0xf3,0x2e}};
	VBuf	*vbuf	= NULL;
	IStream *is		= NULL;
	Bitmap	*bmp	= NULL;

	if (!(bmp = Bitmap::FromHBITMAP(hBmp, NULL))) return false;

	if (::CreateStreamOnHGlobal(NULL, TRUE, &is) == S_OK) {
		if (bmp->Save(is, &pngid) == S_OK) {
			LARGE_INTEGER	pos  = {};
			ULARGE_INTEGER	size = {};

			if (is->Seek(pos, STREAM_SEEK_CUR, &size) == S_OK) {
				ULONG	targ_size = size.LowPart;
				vbuf = new VBuf(targ_size);

				is->Seek(pos, STREAM_SEEK_SET, &size);
				if (is->Read(vbuf->Buf(), targ_size, &targ_size) != S_OK) {
					delete vbuf;
					vbuf = NULL;
				}
			}
		}
		is->Release();
	}
	delete bmp;

	return	vbuf;
}

HBITMAP PngByteToBmpHandle(VBuf *vbuf)
{
	HBITMAP	hBmp = NULL;
	GBuf	gbuf(vbuf);
	IStream	*is = NULL;

	if (::CreateStreamOnHGlobal(gbuf.Handle(), FALSE, &is) == S_OK) {
		Bitmap	*bmp = Bitmap::FromStream(is);
		is->Release();

		if (bmp) {
			bmp->GetHBITMAP(Color::White, &hBmp);
			delete bmp;
		}
	}
	return	hBmp;
}

BITMAPINFO *BmpHandleToInfo(HBITMAP hBmp, int *size)
{
	BITMAP		bmp;
	BITMAPINFO	*bmi;
	int			palette;
	int			header_size;
	int			data_size;
	HWND		hWnd = ::GetDesktopWindow();
	HDC			hDc;

	if (!::GetObject(hBmp, sizeof(bmp), &bmp)) return NULL;

	data_size   = (bmp.bmWidth * ALIGN_SIZE(bmp.bmBitsPixel, 8) / 8) * bmp.bmHeight;
	palette     = bmp.bmBitsPixel <= 8 ? 1 << bmp.bmBitsPixel : 0;
	header_size = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * palette;
	*size		= header_size + data_size;

	if (!(bmi = (BITMAPINFO *)malloc(*size))) return NULL;

	memset(bmi, 0, sizeof(BITMAPINFO));
	bmi->bmiHeader.biSize         = sizeof(BITMAPINFOHEADER);
	bmi->bmiHeader.biWidth        = bmp.bmWidth;
	bmi->bmiHeader.biHeight       = bmp.bmHeight;
	bmi->bmiHeader.biPlanes       = 1;
	bmi->bmiHeader.biBitCount     = bmp.bmBitsPixel;
	bmi->bmiHeader.biCompression  = BI_RGB;
	bmi->bmiHeader.biClrUsed      = palette;
	bmi->bmiHeader.biClrImportant = palette;

	if (!(hDc = ::GetDC(hWnd)) ||
		!::GetDIBits(hDc, hBmp, 0, bmp.bmHeight, (char *)bmi + header_size, bmi, DIB_RGB_COLORS)) {
		free(bmi);
		bmi = NULL;
	}
	if (hDc) ::ReleaseDC(hWnd, hDc);

#if 0
	static int counter = 0;
	BITMAPFILEHEADER	bfh;
	bfh.bfType = 0x4d42;
	bfh.bfReserved1 = 0;
	bfh.bfReserved2 = 0;

	bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + header_size;
	bfh.bfSize    = data_size + bfh.bfOffBits;

	FILE *fp = fopen(Fmt("c:\\temp\\a%d.bmp", counter++), "wb");
	fwrite((void *)&bfh, 1, sizeof(bfh), fp);
	fwrite((void *)bmi, 1, *size, fp);
	fclose(fp);
#endif

	return	bmi;
}

HBITMAP BmpInfoToHandle(BITMAPINFO *bmi, int size)
{
	HBITMAP		hBmp;
	HWND		hWnd = ::GetDesktopWindow();
	HDC			hDc;
	int			header_size;

	if (size < sizeof(BITMAPINFOHEADER)) return NULL;

	if (!(hDc = ::GetDC(hWnd))) return NULL;

	header_size = bmi->bmiHeader.biSize;
	if ((bmi->bmiHeader.biBitCount == 16 || bmi->bmiHeader.biBitCount == 32) &&
			bmi->bmiHeader.biCompression == BI_BITFIELDS) {
		header_size += sizeof(DWORD) * 3; // color masks
	} else {
		header_size += bmi->bmiHeader.biClrUsed * sizeof(RGBQUAD);
	}
	hBmp = ::CreateDIBitmap(hDc, (BITMAPINFOHEADER *)bmi, CBM_INIT,
								(char *)bmi + header_size, bmi, DIB_RGB_COLORS);

	if (hDc) ::ReleaseDC(hWnd, hDc);

	return	hBmp;
}

TAreaConfirmDlg::TAreaConfirmDlg(Cfg *_cfg, TImageWin *_parentWin)
	: TDlg(AREACONFIRM_DIALOG, _parentWin)
{
	hAccel = ::LoadAccelerators(TApp::hInst(), (LPCSTR)IPMSG_ACCEL);
	parentWin	= _parentWin;
	cfg			= _cfg;
	useClip		= NULL;
	withSave	= NULL;
	reEdit		= FALSE;
	color		= 0;
	hToolBar	= NULL;
	mode		= MARKER_PEN;
}


/*
	画面キャプチャ用ツールバーウィンドウ
*/
BOOL TAreaConfirmDlg::Create(BOOL *_useClip, BOOL *_withSave, BOOL _reEdit)
{
	useClip		= _useClip;
	withSave	= _withSave;
	reEdit		= _reEdit;
	mode		= MARKER_PEN;
	BOOL ret = TDlg::Create();

	if (reEdit) {
		SetWindowTextU8(LoadStrU8(IDS_REEDIT_IMAGE));
	}

	return ret;
}

BOOL TAreaConfirmDlg::EvCreate(LPARAM lParam)
{
	POINT	pt;
	::GetCursorPos(&pt);
	FitMoveWindow(pt.x - 20, pt.y - 20);

	CheckDlgButton(CLIP_CHECK, *useClip);
	CheckDlgButton(SAVE_CHECK, *withSave);

	TBBUTTON tb_undo = { 0, MARKER_UNDO, 0, TBSTYLE_BUTTON };

	// 戻るボタンでツールバー作成 (iBitmap: 0)
	hToolBar = ::CreateToolbarEx(hWnd, WS_CHILD|WS_VISIBLE|TBSTYLE_TOOLTIPS, AREA_TOOLBAR,
								1, TApp::hInst(), MARKERTB_BITMAP, &tb_undo,
								1, 0, 0, 16, 16, sizeof(TBBUTTON));

	// 赤・緑・青・黄色のビットマップを追加 (iBitmap 1-16)
	for (int i=0; i < COLOR_MAX; i++) {
		TBADDBITMAP tab = { TApp::hInst(), UINT_PTR(MARKERRED_BITMAP + i) };
		::SendMessage(hToolBar, TB_ADDBITMAP, MARKER_TB_MAX, (LPARAM)&tab);
	}

	// マーカーボタン類追加
	for (int i=0; i < MARKER_TB_MAX; i++) {
		TBBUTTON tb = { i+1, MARKER_PEN+i, TBSTATE_ENABLED, TBSTYLE_CHECKGROUP };
		if (i == 0)			tb.fsState |= TBSTATE_CHECKED;
		else if (i == 4)	tb.fsStyle  = TBSTYLE_BUTTON;
		::SendMessage(hToolBar, TB_INSERTBUTTON, i, (LPARAM)&tb);
	}
	// デフォルトで赤色選択
	SetColor(COLOR_RED, BGCOLOR_RED);

	TBBUTTON tb_sep = {0, 0, TBSTATE_ENABLED, TBSTYLE_SEP, 0, 0};
	::SendMessage(hToolBar, TB_INSERTBUTTON, 4, (LPARAM)&tb_sep);
	::SendMessage(hToolBar, TB_INSERTBUTTON, 6, (LPARAM)&tb_sep);

	Show();

	return	TRUE;
}

BOOL TAreaConfirmDlg::EvCommand(WORD wNotifyCode, WORD wID, LPARAM hwndCtl)
{
	switch (wID) {
	case IDOK:
	case IDRETRY:
	case IDCANCEL:
		if (wID == IDOK) {
			if (useClip)  *useClip  = IsDlgButtonChecked(CLIP_CHECK);
			if (withSave) *withSave = IsDlgButtonChecked(SAVE_CHECK);
		}
		if (wID != IDRETRY || !reEdit) {
			EndDialog(wID);
		}
		parentWin->Notify(wID);
		break;

	case MARKER_UNDO:
		parentWin->PopDrawHist();
		Notify();
		break;

	case MARKER_COLOR:
		switch (color) {
		case COLOR_RED:		SetColor(COLOR_GREEN,	BGCOLOR_GREEN);	break;
		case COLOR_GREEN:	SetColor(COLOR_BLUE,	BGCOLOR_BLUE);	break;
		case COLOR_BLUE:	SetColor(COLOR_YELLOW,	BGCOLOR_YELLOW); break;
		default:			SetColor(COLOR_RED,		BGCOLOR_RED);	break;
		}
		break;

	case MARKER_PEN:
		mode = MARKER_PEN;
		parentWin->SetMode(TRUE);
		break;

	case MARKER_LINE:
		mode = MARKER_LINE;
		parentWin->SetMode(TRUE);
		break;

	case MARKER_RECT:
		mode = MARKER_RECT;
		parentWin->SetMode(TRUE);
		break;

	case MARKER_ARROW:
		mode = MARKER_ARROW;
		parentWin->SetMode(TRUE);
		break;

	default:
		break;
	}
	return	TRUE;
}

void TAreaConfirmDlg::SetColor(COLORREF _color, COLORREF _bg_color)
{
	int	image_base = 0;

	color = _color;
	bgColor = _bg_color;

	switch (color) {
	case COLOR_GREEN:	image_base = 1;	break;
	case COLOR_BLUE:	image_base = 2;	break;
	case COLOR_YELLOW:	image_base = 3;	break;
	default:			image_base = 0;	break; // red
	}

	// マーカーボタン色変更
	for (int i=0; i < MARKER_TB_MAX; i++) {
		::SendMessage(hToolBar, TB_CHANGEBITMAP, MARKER_PEN+i, image_base*MARKER_TB_MAX + i + 1);
	}

	parentWin->SetMode(TRUE);
}

BOOL TAreaConfirmDlg::EvNotify(UINT ctlID, NMHDR *pNmHdr)
{
	if (ctlID == AREA_TOOLBAR) {
		if (pNmHdr->code == TBN_GETINFOTIPW) {
			NMTBGETINFOTIPW	*itip = (NMTBGETINFOTIPW *)pNmHdr;
			itip->pszText = NULL;

			switch (itip->iItem) {
			case MARKER_PEN:	itip->pszText = LoadStrW(IDS_MARKER_PEN);	break;
			case MARKER_LINE:	itip->pszText = LoadStrW(IDS_MARKER_LINE);	break;
			case MARKER_RECT:	itip->pszText = LoadStrW(IDS_MARKER_RECT);	break;
			case MARKER_ARROW:	itip->pszText = LoadStrW(IDS_MARKER_ARROW);	break;
			case MARKER_COLOR:	itip->pszText = LoadStrW(IDS_MARKER_COLOR);	break;
			case MARKER_UNDO:	itip->pszText = LoadStrW(IDS_MARKER_UNDO);	break;
			}
			if (itip->pszText) itip->cchTextMax = (int)wcslen(itip->pszText);
			return	TRUE;
		}
	}
	return	FALSE;
}

void TAreaConfirmDlg::Notify()
{
	::SendMessage(hToolBar, TB_SETSTATE, MARKER_UNDO,
		parentWin->DrawHistNum() ? TBSTATE_ENABLED : 0);
}


/*
	画面キャプチャウィンドウ
*/
CursorMap TImageWin::cursorMap;
#define ARROW_ID	RGB(0,0,1) // dummy color
#define CROSS_ID	RGB(0,0,2) // dummy color
#define SIZENS_ID	RGB(0,0,3) // dummy color
#define SIZEWE_ID	RGB(0,0,4) // dummy color
#define SIZENWSE_ID	RGB(0,0,5) // dummy color
#define SIZENESW_ID	RGB(0,0,6) // dummy color

TImageWin::TImageWin(Cfg *_cfg, TSendDlg *_parent)
	: areaDlg(_cfg, this), inputDlg(this), TWin(_parent)
{
	parentWnd	= _parent;
	cfg			= _cfg;
	hSelfDc		= NULL;
	hSelfBmp	= NULL;
	hDarkDc		= NULL;
	hDarkBmp	= NULL;
	reEdit		= NULL;
	useClip		= cfg->CaptureClip;
	withSave	= cfg->CaptureSave;
	memset(areaPts, 0, sizeof(areaPts));
	memset(&lastPts, 0, sizeof(lastPts));
}

TImageWin::~TImageWin()
{
}

void TImageWin::UnInit()
{
	if (hSelfDc) {
		::SelectObject(hSelfDc, hSelfBmpOld);
		::DeleteDC(hSelfDc);
		hSelfDc = NULL;
	}
	if (hSelfBmp) {
		::DeleteObject(hSelfBmp);
		hSelfBmp = NULL;
	}
	if (hDarkDc) {
		::SelectObject(hDarkDc, hDarkBmpOld);
		::DeleteDC(hDarkDc);
		hDarkDc = NULL;
	}
	if (hDarkBmp) {
		::DeleteObject(hDarkBmp);
		hDarkBmp = NULL;
	}
	if (cursorMap[ARROW_ID]) {
		::SetClassLong(hWnd, GCL_HCURSOR, LONG_RDC(cursorMap[ARROW_ID]));
	}
}

BOOL TImageWin::Create(TEditSub *_reEdit)
{
	sX  = ::GetSystemMetrics(SM_XVIRTUALSCREEN);
	sY  = ::GetSystemMetrics(SM_YVIRTUALSCREEN);
	sCx = ::GetSystemMetrics(SM_CXVIRTUALSCREEN);
	sCy = ::GetSystemMetrics(SM_CYVIRTUALSCREEN);

	rect.left	= sX;
	rect.top	= sY;
	rect.right	= sCx + rect.left;
	rect.bottom	= sCy + rect.top;

	HWND	hDesktop   = ::GetDesktopWindow();
	HDC		hDesktopDc = ::GetDC(hDesktop);

	hSelfDc	= ::CreateCompatibleDC(hDesktopDc);

	if ((reEdit = _reEdit)) {
		if (!ReEdit()) {
			UnInit();
			return FALSE;
		}
	}
	else {
		hSelfBmp	= ::CreateCompatibleBitmap(hDesktopDc, sCx, sCy);
		hSelfBmpOld	= (HBITMAP)::SelectObject(hSelfDc, hSelfBmp);

		::BitBlt(hSelfDc, 0, 0, sCx, sCy, hDesktopDc, sX, sY, SRCCOPY);

		hDarkDc		= ::CreateCompatibleDC(hSelfDc);
		hDarkBmp	= ::CreateCompatibleBitmap(hSelfDc, sCx, sCy);
		hDarkBmpOld	= (HBITMAP)::SelectObject(hDarkDc, hDarkBmp);
		MakeDarkBmp(200);
	}
	::ReleaseDC(hDesktop, hDesktopDc);

	if (cursorMap.empty()) {
		cursorMap[CROSS_ID]					  = ::LoadCursor(TApp::hInst(), (LPCSTR)CROSS_CUR);
		cursorMap[ARROW_ID]					  = ::LoadCursor(NULL, (LPCSTR)IDC_ARROW);
		cursorMap[SIZENS_ID]				  = ::LoadCursor(NULL, (LPCSTR)IDC_SIZENS);
		cursorMap[SIZEWE_ID]				  = ::LoadCursor(NULL, (LPCSTR)IDC_SIZEWE);
		cursorMap[SIZENWSE_ID]				  = ::LoadCursor(NULL, (LPCSTR)IDC_SIZENWSE);
		cursorMap[SIZENESW_ID]				  = ::LoadCursor(NULL, (LPCSTR)IDC_SIZENESW);
		cursorMap[CURSOR_IDX(COLOR_RED)]	  = ::LoadCursor(TApp::hInst(), (LPCSTR)RED_CUR);
		cursorMap[CURSOR_IDX(COLOR_GREEN)]	  = ::LoadCursor(TApp::hInst(), (LPCSTR)GREEN_CUR);
		cursorMap[CURSOR_IDX(COLOR_BLUE)]	  = ::LoadCursor(TApp::hInst(), (LPCSTR)BLUE_CUR);
		cursorMap[CURSOR_IDX(COLOR_YELLOW)]	  = ::LoadCursor(TApp::hInst(), (LPCSTR)YELLOW_CUR);
		cursorMap[CURSOR_IDX(COLOR_RED,   1)] = ::LoadCursor(TApp::hInst(), (LPCSTR)GENRED_CUR);
		cursorMap[CURSOR_IDX(COLOR_GREEN, 1)] = ::LoadCursor(TApp::hInst(), (LPCSTR)GENGREEN_CUR);
		cursorMap[CURSOR_IDX(COLOR_BLUE,  1)] = ::LoadCursor(TApp::hInst(), (LPCSTR)GENBLUE_CUR);
		cursorMap[CURSOR_IDX(COLOR_YELLOW,1)] = ::LoadCursor(TApp::hInst(), (LPCSTR)GENYELLOW_CUR);
		TRegisterClassU8(IPMSG_CAPTURE_CLASS, CS_DBLCLKS);
	}

	return	TWin::CreateU8(IPMSG_CAPTURE_CLASS, NULL, WS_POPUP|WS_DISABLED, 0);
}

BOOL TImageWin::EvCreate(LPARAM lParam)
{
	status = INIT;
	SetTimer(100, 10);
	if (!reEdit) {
		SetCursor();
	}

	return	TRUE;
}

BOOL TImageWin::EvNcDestroy()
{
	UnInit();
	return	FALSE;
}

BOOL TImageWin::EvPaint()
{
	PAINTSTRUCT	ps;
	HDC			hDc = ::BeginPaint(hWnd, &ps);

	if (reEdit) {
		::BitBlt(hDc, 0, 0, sCx, sCy, hSelfDc, 0, 0, BLACKNESS);
		::BitBlt(hDc, reRc.left, reRc.top, reRc.cx(), reRc.cy(), hSelfDc, 0, 0, SRCCOPY);
	}
	else {
		::BitBlt(hDc, 0, 0, sCx, sCy, hSelfDc, 0, 0, SRCCOPY);
	}
	::EndPaint(hWnd, &ps);

	if (status >= START && status <= DRAW_END) {
		DrawArea();
		DrawMarker();
	}

	return	TRUE;
}

BOOL TImageWin::EvTimer(WPARAM _timerID, TIMERPROC proc)
{
	KillTimer(_timerID);
	SetWindowLong(GWL_STYLE, (GetWindowLong(GWL_STYLE) & ~WS_DISABLED)|WS_VISIBLE);
	SetWindowPos(HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOREDRAW);
	InvalidateRect(NULL, TRUE);

	if (reEdit) {
		status = END;
		areaDlg.Create(&useClip, &withSave, TRUE);
	}

	return	TRUE;
}

BOOL TImageWin::EvChar(WCHAR code, LPARAM keyData)
{
	if (code == VK_RETURN) {
		areaDlg.PostMessage(WM_COMMAND, IDOK, 0);
	}
	else if (code == VK_ESCAPE) {
		areaDlg.PostMessage(WM_COMMAND, IDCANCEL, 0);
	}
	return	TRUE;
}

BOOL TImageWin::MakeDarkBmp(int level)
{
	::BitBlt(hDarkDc, 0, 0, sCx, sCy, hSelfDc, 0, 0, BLACKNESS);
	BLENDFUNCTION	bf = { AC_SRC_OVER, 0, (BYTE)level, 0 };
	::AlphaBlend(hDarkDc, 0, 0, sCx, sCy, hSelfDc, 0, 0, sCx, sCy, bf);
	return	TRUE;
}

BOOL TImageWin::ReEdit()
{
	if (!(hSelfBmp = reEdit->GetBitmap(reEdit->SelectedImageIndex()))) {
		return FALSE;
	}

	BITMAP	bmp  = {};
	BOOL	ret  = TRUE;

	if (::GetObject(hSelfBmp, sizeof(bmp), &bmp)) {
		hSelfBmpOld	= (HBITMAP)::SelectObject(hSelfDc, hSelfBmp);

		long	cx = abs(bmp.bmWidth);
		long	cy = abs(bmp.bmHeight);

		areaPts[0].x = (short)((sCx - cx) / 2);
		areaPts[0].y = (short)((sCy - cy) / 2);
		areaPts[1].x = (short)(areaPts[0].x + cx - 1);
		areaPts[1].y = (short)(areaPts[0].y + cy - 1);
		lastPts = areaPts[1];
		reRc = TRect(areaPts[0], cx, cy);
	}
	return	ret;
}

#include <math.h>

void DrawRoundRect(Graphics *g, Pen *pen, const TRect &rc, int r, Brush *brush=NULL)
{
	int		r2 = r * 2;
	INT		x  = INT(rc.x());
	INT		y  = INT(rc.y());
	INT		cx = INT(rc.cx());
	INT		cy = INT(rc.cy());

	if (r2 > cx || r2 > cy) {
		if (brush) {
			g->FillRectangle(brush, x, y, cx, cy);
		}
		g->DrawRectangle(pen, x, y, cx, cy);
		return;
	}

	GraphicsPath	p;
	p.AddArc(x + cx - r2, y,           r2, r2, (REAL)270, (REAL)90);
	p.AddArc(x + cx - r2, y + cy - r2, r2, r2, (REAL)0,   (REAL)90);
	p.AddArc(x,           y + cy - r2, r2, r2, (REAL)90,  (REAL)90);
	p.AddArc(x,           y,           r2, r2, (REAL)180, (REAL)90);
	p.CloseFigure();

	if (brush) {
		g->FillPath(brush, &p);
	}
	g->DrawPath(pen, &p);
}

TRect DrawMarkerMemo(HDC hDc, Graphics *g, Pen *pen, const ColPts& col, POINT *pt, TRect max_rc)
{
#define RC_MARGIN 2
 	HFONT	hFont = TSendDlg::GetEditFont();
 	HFONT	hOldFont = NULL;
 	if (hFont) hOldFont = (HFONT)::SelectObject(hDc, hFont);

	Wstr	wstr(col.memo.c_str());
	SIZE	size = {};
	::GetTextExtentPoint32W(hDc, wstr.s(), (int)wcslen(wstr.s()), &size);
	size.cx += RC_MARGIN * 2;
	size.cy += RC_MARGIN * 2;
	TRect	rc(pt->x - size.cx/2 + RC_MARGIN, pt->y + RC_MARGIN, size.cx, size.cy);

	::InflateRect(&max_rc, -RC_MARGIN-1, -RC_MARGIN-1);

	if (rc.x() < max_rc.x()) {
		rc.x() = max_rc.x();
	}
	else if ((rc.x() + size.cx) > max_rc.right) {
		rc.x() = max_rc.right - size.cx;
	}
	if (col.pts.front().y > col.pts.back().y) {
		rc.y() -= size.cy + RC_MARGIN * 2;
	}
	if (rc.y() < max_rc.y()) {
		rc.y() = max_rc.y();
	}
	else if ((rc.y() + size.cy) > max_rc.bottom) {
		rc.y() = max_rc.bottom - size.cy;
	}
	rc.set_cx(size.cx);
	rc.set_cy(size.cy);

	TRect	trc(rc.x()+RC_MARGIN+1, rc.y()+RC_MARGIN, rc.cx(), rc.cy());

	::SetBkMode(hDc, TRANSPARENT);
	::SetTextColor(hDc, col.color);

	::InflateRect(&rc, RC_MARGIN, RC_MARGIN);

	Color	bgClr;
	bgClr.SetFromCOLORREF((COLORREF)col.bgColor);
	SolidBrush	bgBrush(bgClr);

	DrawRoundRect(g, pen, rc, 4, &bgBrush);

	::DrawTextW(hDc, wstr.s(), -1, &trc, DT_LEFT);

	if (hOldFont) ::SelectObject(hDc, hOldFont);

	return	rc;
}

BOOL TImageWin::DrawMarker(HDC hDc)
{
	if (status >= DRAW_INIT && status <= DRAW_END && status != DRAW_INPUT) {
		HDC		hDcSv = hDc;
		HRGN	hRgn = NULL;
		POINT	offset = {0, 0};

		if (!hDc && !(hDc = ::GetDC(hWnd))) return FALSE;

		TRect rc(areaPts[0], lastPts);
		rc.Regular();

		if (!hDcSv) { // ウィンドウ座標同士のコピーなのでオフセット不要。クリッピングのみ
			hRgn = ::CreateRectRgn(rc.left, rc.top, rc.right+1, rc.bottom+1);
			::SelectClipRgn(hDc, hRgn);
		}
		else {	// ウィンドウ座標からビットマップにコピーの場合はオフセット必要
			offset.x = rc.left;
			offset.y = rc.top;
		}

		Graphics	g(hDc);
		g.SetSmoothingMode(SmoothingModeAntiAlias);

		for (auto &p: drawPts) {
			auto	vp = p.pts.begin();
			auto	ptx = [&](int x) { return x - offset.x; };
			auto	pty = [&](int y) { return y - offset.y; };
			int		x = ptx(vp->x);
			int		y = pty(vp->y);
			Color	col;
			col.SetFromCOLORREF((COLORREF)p.color);
			Pen		pen(col,  (float)cfg->MarkerThick);

			if (p.mode == MARKER_PEN) {
				GraphicsPath	path;
				POINTS	last = *vp;
				for (vp++; vp != p.pts.end(); vp++) {
					path.AddLine(ptx(last.x), pty(last.y), ptx(vp->x), pty(vp->y));
					last = *vp;
				}
				g.DrawPath(&pen, &path);
			}
			else if (p.mode == MARKER_LINE) {
				if (p.pts.size() > 1) {
					int		end_x = ptx(p.pts.back().x);
					int		end_y = pty(p.pts.back().y);
					g.DrawLine(&pen, x, y, end_x, end_y);
				}
			}
			else if (p.mode == MARKER_RECT) {
				if (p.pts.size() > 1) {
					int		end_x = ptx(p.pts.back().x);
					int		end_y = pty(p.pts.back().y);
					TRect	rrc(x, y, end_x-x, end_y-y);
					rrc.Regular();
					DrawRoundRect(&g, &pen, rrc, 4);
				}
			}
			else if (p.mode == MARKER_ARROW) {
				if (p.pts.size() > 1) {
					POINT	end_pt = {  ptx(p.pts.back().x), pty(p.pts.back().y) };
					if (p.memo.size()) {
						TRect	max_rc(ptx(rc.x()), pty(rc.y()), rc.cx(), rc.cy());
						TRect	mrc = DrawMarkerMemo(hDc, &g, &pen, p, &end_pt, max_rc);

						if (y > end_pt.y && mrc.bottom > end_pt.y) {
							double ratio = ((double)x - end_pt.x) / ((double)y - end_pt.y);
							end_pt.x = int(end_pt.x + ratio * (mrc.bottom - end_pt.y));
							end_pt.y = mrc.bottom;
						}
						else if (y < end_pt.y && mrc.top < end_pt.y) {
							double ratio = ((double)end_pt.x - x) / ((double)end_pt.y - y);
							end_pt.x = int(end_pt.x - ratio * (end_pt.y - mrc.top));
							end_pt.y = mrc.top;
						}
						p.pts.back().x = short(ptx(end_pt.x));
						p.pts.back().y = short(pty(end_pt.y));
					}
					AdjustableArrowCap cap(4, 4);
					pen.SetCustomStartCap(&cap);
					g.DrawLine(&pen, x, y, end_pt.x, end_pt.y);
				}
			}
		}
		if (!hDcSv) {
			::SelectClipRgn(hDc, NULL);
			::DeleteObject(hRgn);
			::ReleaseDC(hWnd, hDc);
		}
	}
	return	TRUE;
}

BOOL TImageWin::DrawArea(POINTS *pts)
{
	HDC		hDc;
	POINTS	cur = pts ? *pts : status == END ? areaPts[1] : lastPts;

	if (!(hDc = ::GetDC(hWnd))) return FALSE;

	HRGN	hRgn = ::CreateRectRgn(areaPts[0].x, areaPts[0].y, cur.x + 1, cur.y + 1);

	::ExtSelectClipRgn(hDc, hRgn, RGN_DIFF);
	::BitBlt(hDc, 0, 0, sCx, sCy, hDarkDc, 0, 0, SRCCOPY);
	::SelectClipRgn(hDc, NULL);
	::DeleteObject(hRgn);

	HPEN	hPen    = ::CreatePen(PS_DOT, 0, COLOR_RED);
	HPEN	hOldPen = (HPEN)::SelectObject(hDc, (HGDIOBJ)hPen);

	if (pts && !IS_SAME_PTS(cur, areaPts[0])) {
		TRect rc(areaPts[0], lastPts);
		rc.Regular();
		InflateRect(&rc, 1, 1);
		InvalidateRect(&rc, FALSE);
	}
	::MoveToEx(hDc, areaPts[0].x, areaPts[0].y, NULL);
	::LineTo(hDc, cur.x, areaPts[0].y);
	::LineTo(hDc, cur.x, cur.y);
	::LineTo(hDc, areaPts[0].x, cur.y);
	::LineTo(hDc, areaPts[0].x, areaPts[0].y);

	if (pts) lastPts = *pts;

	::SelectObject(hDc, hOldPen);
	::DeleteObject(hPen);

	::ReleaseDC(hWnd, hDc);

	return	TRUE;
}

TImageWin::ResizeMode TImageWin::GetResizeMode(POINTS pts)
{
	short	diff = 0;
	int	mode = RESIZE_NONE;
	if ((diff = areaPts[0].x - pts.x) >= 0 && diff < 10) mode |= RESIZE_X;
	if ((diff = pts.x - areaPts[1].x) >= 0 && diff < 10) mode |= RESIZE_CX;
	if ((diff = areaPts[0].y - pts.y) >= 0 && diff < 10) mode |= RESIZE_Y;
	if ((diff = pts.y - areaPts[1].y) >= 0 && diff < 10) mode |= RESIZE_CY;
	return	(ResizeMode)mode;
}

BOOL TImageWin::IsOverArea(POINTS pts)
{
	short	diff = 0;
	if ((diff = areaPts[0].x - pts.x) >= 0) return TRUE;
	if ((diff = pts.x - areaPts[1].x) >= 0) return TRUE;
	if ((diff = areaPts[0].y - pts.y) >= 0) return TRUE;
	if ((diff = pts.y - areaPts[1].y) >= 0) return TRUE;
	return	FALSE;
}

BOOL TImageWin::EvMouseMove(UINT fwKeys, POINTS pts)
{
	if (status == START) {
		DrawArea(&pts);
	}
	else if (status == DRAW_START) {
		if (!IS_SAME_PTS(drawPts.back().pts.back(), pts)) {
			if (areaDlg.GetMode() == MARKER_PEN) {
				drawPts.back().pts.push_back(pts);
			}
			else if (areaDlg.GetMode() == MARKER_RECT) {
				if (drawPts.back().pts.size() > 1) {
					TRect rc(drawPts.back().pts.front(), drawPts.back().pts.back());
					drawPts.back().pts.pop_back();
					rc.Regular();
					InflateRect(&rc, 2, 2);
					InvalidateRect(&rc, FALSE);
				}
				drawPts.back().pts.push_back(pts);
			}
			else {
				if (drawPts.back().pts.size() > 1) {
					TRect rc(drawPts.back().pts.front(), drawPts.back().pts.back());
					drawPts.back().pts.pop_back();
					rc.Regular();
					InflateRect(&rc, 10, 10);
					InvalidateRect(&rc, FALSE);
				}
				drawPts.back().pts.push_back(pts);
			}
		}
		DrawMarker();
	}
	else if (status == RESIZE_AREA) {
		if (rsMode & RESIZE_X) {
			areaPts[0].x = min(pts.x, areaPts[1].x);
		}
		if (rsMode & RESIZE_CX) {
			areaPts[1].x = max(pts.x, areaPts[0].x);
		}
		if (rsMode & RESIZE_Y) {
			areaPts[0].y = min(pts.y, areaPts[1].y);
		}
		if (rsMode & RESIZE_CY) {
			areaPts[1].y = max(pts.y, areaPts[0].y);
		}
		lastPts = areaPts[1];
		TRect rc(areaPts[0], lastPts);
		rc.Regular();
		auto inflate = reEdit ? 100 : 1;
		InflateRect(&rc, inflate, inflate);
		InvalidateRect(&rc, TRUE);
	}

	SetCursor(&pts);

	return	TRUE;
}


void TImageWin::NormalizeArea()
{
	if (areaPts[0].x > areaPts[1].x) {
		auto x = areaPts[0].x;
		areaPts[0].x = areaPts[1].x;
		areaPts[1].x = x;
	}
	if (areaPts[0].y > areaPts[1].y) {
		auto y = areaPts[0].y;
		areaPts[0].y = areaPts[1].y;
		areaPts[1].y = y;
	}
	lastPts = areaPts[1];
}

BOOL TImageWin::EventButton(UINT uMsg, int nHitTest, POINTS pts) // クライアント座標
{
	if (uMsg == WM_LBUTTONDOWN) {
		::SetCapture(hWnd);
		if (status == INIT) {
			status = START;
			areaPts[0] = lastPts = pts;
			return	TRUE;
		}
		if (status == START) {
			return	TRUE;
		}
		rsMode = GetResizeMode(pts);
		if (rsMode != RESIZE_NONE) {
			status = RESIZE_AREA;
			return	TRUE;
		}
		if (IsOverArea(pts)) {
			return	TRUE;
		}
		if (status == DRAW_INIT) {
			status = DRAW_START;
			drawPts.push_back(ColPts());
			drawPts.back().mode  = areaDlg.GetMode();
			drawPts.back().color = areaDlg.GetColor();
			drawPts.back().bgColor = areaDlg.GetBgColor();
			drawPts.back().pts.push_back(pts);
			areaDlg.Notify();
			DrawMarker();
			return	TRUE;
		}
	}

	if (uMsg == WM_LBUTTONUP) {
		::ReleaseCapture();
		if (status == START) {
			if (!areaDlg.hWnd) {
				areaPts[1] = pts;
				if ((areaPts[0].x == areaPts[1].x) || (areaPts[0].y == areaPts[1].y)) {
					status = INIT;
					InvalidateRect(NULL, FALSE);
				}
				else {
					NormalizeArea();
					status = END;
					areaDlg.Create(&useClip, &withSave);
					//MakeDarkBmp(100);
					InvalidateRect(NULL, FALSE);
				}
			}
			return	TRUE;
		}
		if (status == RESIZE_AREA) {
			rsMode = RESIZE_NONE;
			status = DRAW_INIT;
			return	TRUE;
		}
		if (status == DRAW_START) {
			status = DRAW_INIT;
			if (areaDlg.GetMode() == MARKER_ARROW && drawPts.back().pts.size() > 1) {
				drawPts.back().pts.pop_back();
				drawPts.back().pts.push_back(pts);

#define INPUTDLG_X	180
#define INPUTDLG_Y	20
				char	buf[MAX_BUF] = "";
				POINT	pt = {	pts.x - INPUTDLG_X / 2,
								pts.y > drawPts.back().pts.front().y ?
									pts.y + 2 : pts.y - 2 - INPUTDLG_Y };

			// 将来、非トップレベルウィンドウに変更する場合は、要クライアント座標変換
				::ClientToScreen(hWnd, &pt);
				if (pt.x + INPUTDLG_X > sX + sCx)	pt.x = sX + sCx - INPUTDLG_X;
				else if (pt.x < sX)					pt.x = sX;
				if (pt.y + INPUTDLG_Y > sY + sCy)	pt.y = sY + sCy - INPUTDLG_Y;
				else if (pt.y < sY)					pt.y = sY;

				status = DRAW_INPUT;
				areaDlg.EnableWindow(FALSE);
				UINT ret = inputDlg.Exec(pt, buf, sizeof(buf));
				areaDlg.EnableWindow(TRUE);
				status = DRAW_INIT;

				if (ret == IDOK) {
					InvalidateRect(NULL, FALSE);
					drawPts.back().memo = buf;
					DrawMarker();
				}
			}
			else {
				drawPts.back().pts.push_back(pts);
			}
			return	TRUE;
		}
	}
	return	TRUE;
}

void TImageWin::Notify(int result)
{
	if (result == IDRETRY) {
		if (reEdit) {
			status = END;
			SetMode(TRUE);
		}
		else {
			status = INIT;
			SetCursor();
			MakeDarkBmp(200);
		}
		InvalidateRect(NULL, FALSE);
		drawPts.clear();
	}
	else if (result == IDCANCEL || result == IDOK) {
		Show(SW_HIDE);
		parentWnd->Show();
		if (result == IDOK) {
			::SetFocus(parentWnd->GetDlgItem(SEND_EDIT));
			CutImage(useClip, withSave);
		}
		Destroy();
	}
}

void TImageWin::SetCursor(POINTS *pts)
{
	HCURSOR		h_cur = cursorMap[ARROW_ID];

	if (status == INIT) {
		h_cur = cursorMap[CROSS_ID];
	}
	else if (status >= DRAW_INIT) {
		auto	color = areaDlg.GetColor();
		auto	itr = cursorMap.find(CURSOR_IDX(color, areaDlg.GetMode() != MARKER_PEN));
		if (itr != cursorMap.end()) {
			h_cur = itr->second;
		}
	}

	auto rsmode_to_sizeid = [](auto rsm) {
		return	(rsm == RESIZE_X  || rsm == RESIZE_CX)   ? SIZEWE_ID :
				(rsm == RESIZE_Y  || rsm == RESIZE_CY)   ? SIZENS_ID :
				(rsm == RESIZE_XY || rsm == RESIZE_CXCY) ? SIZENWSE_ID : SIZENESW_ID;
	};
	if (status == RESIZE_AREA) {
		h_cur = cursorMap[rsmode_to_sizeid(rsMode)];
	}
	if (pts) {
		auto rsm = GetResizeMode(*pts);
		if (rsm != RESIZE_NONE) {
			h_cur = cursorMap[rsmode_to_sizeid(rsm)];
		}
	}

	::SetClassLong(hWnd, GCL_HCURSOR, LONG_RDC(h_cur));
}

void TImageWin::SetMode(BOOL is_draw)
{
	if (is_draw) {
		if (status < DRAW_INIT) {
			status = DRAW_INIT;
			drawPts.clear();
		}
	}
	else {
		status = END;
	}
	SetCursor();
}

void TImageWin::PopDrawHist()
{
	if (!drawPts.empty()) {
		drawPts.pop_back();
		InvalidateRect(NULL, TRUE);
	}
}

BOOL TImageWin::CutImage(BOOL use_clip, BOOL with_save)
{
	TRect	rc(areaPts[0], areaPts[1]);
	rc.Regular();

	rc.right  += 1; // include right/bottom line pixel
	rc.bottom += 1;
	int	off_x = reEdit ? (rc.x() - reRc.x()) : rc.left;
	int	off_y = reEdit ? (rc.y() - reRc.y()) : rc.top;

	HDC		hAreaDc		= ::CreateCompatibleDC(hSelfDc);
	HBITMAP	hAreaBmp	= ::CreateCompatibleBitmap(hSelfDc, rc.cx(), rc.cy());
	HBITMAP	hAreaBmpOld	= (HBITMAP)::SelectObject(hAreaDc, hAreaBmp);

	::BitBlt(hAreaDc, 0, 0, rc.cx(), rc.cy(), hSelfDc, off_x, off_y, SRCCOPY);
	DrawMarker(hAreaDc);

	if (use_clip && ::OpenClipboard(hWnd)) {
		::EmptyClipboard();
		::SetClipboardData(CF_BITMAP, hAreaBmp);
		::CloseClipboard();
	}

	parentWnd->InsertBitmapByHandle(hAreaBmp);

	if (with_save) {
		char	fname[MAX_PATH_U8];
		MakeImageTmpFileName(cfg->lastSaveDir, fname);
		OpenFileDlg	dlg(this->parent, OpenFileDlg::SAVE, NULL, OFN_OVERWRITEPROMPT);

		if (dlg.Exec(fname, sizeof(fname), NULL, "PNG file(*.png)\0*.png\0\0",
						cfg->lastSaveDir, "png")) {
			DWORD	size = 0;
			VBuf	*buf = BmpHandleToPngByte(hAreaBmp);
			if (buf) {
				HANDLE	hFile = CreateFileWithDirU8(fname, GENERIC_WRITE,
											 FILE_SHARE_READ|FILE_SHARE_WRITE,
											 NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
				if (hFile != INVALID_HANDLE_VALUE) {
					WriteFile(hFile, buf->Buf(), (DWORD)buf->Size(), &size, 0);
					CloseHandle(hFile);
					cfg->WriteRegistry(CFG_GENERAL);
					TOpenExplorerSelOneW(U8toWs(fname));
				}
				delete buf;
			}
		}
	}
	::SelectObject(hAreaDc, hAreaBmpOld);
	::DeleteDC(hAreaDc);
	::DeleteObject(hAreaBmp);

	return	TRUE;
}

