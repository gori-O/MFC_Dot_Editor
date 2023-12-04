
// MFC_DotEditorDlg.h: 헤더 파일
//

#pragma once

#define X_COUNT			30						// 가로
#define Y_COUNT			30						// 세로. 도트 갯수
#define RECT_INTERVAL	20						// 도트 간격

// CMFCDotEditorDlg 대화 상자
class CMFCDotEditorDlg : public CDialogEx
{
private:
	COLORREF m_dot_color[Y_COUNT][X_COUNT];				// 낙서장 전체의 도트 하나하나를 배열로 저장
	COLORREF m_left_color, m_right_color;				// 마우스 오른쪽, 왼쪽의 색상을 다르게 하기위함
	CPen m_grid_pen;									// 도트 그릴 때 사용할 cpen

	CRect m_left_rect, m_right_rect;					// ?
// 생성입니다.
public:
	CMFCDotEditorDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFC_DOTEDITOR_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedLeftColorBtn();
	afx_msg void OnBnClickedRightColorBtn();
	afx_msg void OnBnClickedClearBtn();
};
