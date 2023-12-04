
// MFC_DotEditorDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "MFC_DotEditor.h"
#include "MFC_DotEditorDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCDotEditorDlg 대화 상자



CMFCDotEditorDlg::CMFCDotEditorDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFC_DOTEDITOR_DIALOG, pParent), m_grid_pen(PS_SOLID, 1, RGB(220, 220, 220))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	for (int y = 0; y < Y_COUNT; y++) {						// 도트 전체를 흰색으로 초기화 한다.
		for (int x = 0; x < X_COUNT; x++) {
			m_dot_color[y][x] = RGB(255, 255, 255);
		}
	}

	m_left_color = RGB(0, 0, 255);							// 왼쪽클릭 색상 = blue 로 초기화
	m_right_color = RGB(255, 255, 255);						// 오른쪽 색상 = black 으로 초기화
}

void CMFCDotEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCDotEditorDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_LEFT_COLOR_BTN, &CMFCDotEditorDlg::OnBnClickedLeftColorBtn)
	ON_BN_CLICKED(IDC_RIGHT_COLOR_BTN, &CMFCDotEditorDlg::OnBnClickedRightColorBtn)
	ON_BN_CLICKED(IDC_CLEAR_BTN, &CMFCDotEditorDlg::OnBnClickedClearBtn)
END_MESSAGE_MAP()


// CMFCDotEditorDlg 메시지 처리기

BOOL CMFCDotEditorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	GetDlgItem(IDC_LEFT_COLOR_BTN)->GetWindowRect(&m_left_rect);		
	ScreenToClient(m_left_rect);
	m_left_rect.left -= 15;
	m_left_rect.right = m_left_rect.left + 10;

	GetDlgItem(IDC_RIGHT_COLOR_BTN)->GetWindowRect(&m_right_rect);
	ScreenToClient(m_right_rect);
	m_right_rect.left -= 15;
	m_right_rect.right = m_right_rect.left + 10;

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CMFCDotEditorDlg::OnPaint()
{
	CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.
	if (IsIconic())
	{
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPen* p_old_pen = dc.SelectObject(&m_grid_pen);				// 배경을 그림
		CBrush* p_old_brush = dc.GetCurrentBrush();					// 브러시 바뀌기전 현재브러시 저장

		dc.SelectStockObject(DC_BRUSH);										

		for (int y = 0; y < Y_COUNT; y++) {
			for (int x = 0; x < X_COUNT; x++) {
				if (dc.GetDCBrushColor() != m_dot_color[y][x]) {// 도트 색상이 달라지면=(배경을 제외하기 위한 조건절)
					dc.SetDCBrushColor(m_dot_color[y][x]);		// 해당 도트의 색으로 브러시설정한다.
				}
				dc.Rectangle(x * RECT_INTERVAL, y * RECT_INTERVAL,		// 설정된 크기로 도트들을 그린다.
					RECT_INTERVAL + 1 + x * RECT_INTERVAL,
					RECT_INTERVAL + 1 + y * 20);
			}
		}
		dc.SetDCBrushColor(m_left_color);							
		dc.Rectangle(m_left_rect);
		dc.SetDCBrushColor(m_right_color);
		dc.Rectangle(m_right_rect);
		dc.SelectObject(p_old_brush);					// 원래 브러시로 복구
		dc.SelectObject(p_old_pen);						// 원래 펜으로 복구  
		//CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMFCDotEditorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCDotEditorDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if (nFlags & (MK_LBUTTON | MK_RBUTTON)) {
		int x = point.x / RECT_INTERVAL;
		int y = point.y / RECT_INTERVAL;

		if (x < X_COUNT && y < Y_COUNT) {
			if (nFlags & MK_LBUTTON) {
				m_dot_color[y][x] = m_left_color;
			}
			else {
				m_dot_color[y][x] = m_right_color;
			}

			CClientDC dc(this);
			CPen* p_old_pen = dc.SelectObject(&m_grid_pen);
			CGdiObject* p_old_brush = dc.SelectStockObject(DC_BRUSH);
			dc.SetDCBrushColor(m_dot_color[y][x]);
			dc.Rectangle(x * RECT_INTERVAL, y * RECT_INTERVAL,
				RECT_INTERVAL + 1 + x * RECT_INTERVAL,
				RECT_INTERVAL + 1 + y * RECT_INTERVAL);
			dc.SelectObject(p_old_brush);
			dc.SelectObject(p_old_pen);
		}
	}

	CDialogEx::OnMouseMove(nFlags, point);
}


void CMFCDotEditorDlg::OnBnClickedLeftColorBtn()
{
	CColorDialog ins_dlg;
	ins_dlg.m_cc.Flags = ins_dlg.m_cc.Flags | CC_FULLOPEN | CC_RGBINIT;
	ins_dlg.m_cc.rgbResult = m_left_color;
	if (IDOK == ins_dlg.DoModal()) {
		m_left_color = ins_dlg.GetColor();
		InvalidateRect(m_left_rect, FALSE);
		//Invalidate();
	}
}


void CMFCDotEditorDlg::OnBnClickedRightColorBtn()
{
	CColorDialog ins_dlg;
	ins_dlg.m_cc.Flags = ins_dlg.m_cc.Flags | CC_FULLOPEN | CC_RGBINIT;
	ins_dlg.m_cc.rgbResult = m_right_color;
	if (IDOK == ins_dlg.DoModal()) {
		m_right_color = ins_dlg.GetColor();
		InvalidateRect(m_right_rect, FALSE);
		//Invalidate();
	}
}


void CMFCDotEditorDlg::OnBnClickedClearBtn()
{
	CClientDC dc(this);
	CPen *grid_pen;
	grid_pen = new CPen(PS_SOLID, 1, RGB(220, 220, 220));
	for (int y = 0; y < Y_COUNT; y++) {
		for (int x = 0; x < X_COUNT; x++) {
			dc.SetDCBrushColor(RGB(255, 255, 255));
			dc.SetDCPenColor(RGB(220, 220, 220));
			dc.SelectObject(grid_pen);
			dc.Rectangle(x * RECT_INTERVAL, y * RECT_INTERVAL,
				RECT_INTERVAL + 1 + x * RECT_INTERVAL,
				RECT_INTERVAL + 1 + y * 20);
		}
	}
}
