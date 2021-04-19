
// OpenGLView.h : ��������� ������ COpenGLView
//

#pragma once

// You should have already added this line.
#include "Angel.h"
#include "uofrGraphics.h"

class COpenGLView : public CView
{
protected: // ������� ������ �� ������������
	COpenGLView();
	DECLARE_DYNCREATE(COpenGLView)

// ��������
public:
	COpenGLDoc* GetDocument() const;

// ��������
public:

// ���������������
public:
	virtual void OnDraw(CDC* pDC);  // �������������� ��� ��������� ����� �������������
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// ����������
public:
	virtual ~COpenGLView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

//////////////////////////////////////////////////////////////////////////////
//// ��� OpenGL
//////////////////////////////////////////////////////////////////////////////
protected:
	// You will add the following stuff!!!

	virtual BOOL GetOldStyleRenderingContext(void);
	virtual BOOL SetupPixelFormat(void);


private:
	//OpenGL Setup
	BOOL GetRenderingContext();
	//Rendering Context and Device Context Pointers
	HGLRC     m_hRC;
	CDC*      m_pDC;

	//Error Handling
	void SetError(int e);
	static const char* const _ErrorStrings[];
	const char* m_ErrorString;

//////////////////////////////////////////////////////////////////////////////
//// ��� OpenGL
//////////////////////////////////////////////////////////////////////////////



// ��������� ������� ����� ���������
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnFileOpen();
};

#ifndef _DEBUG  // ���������� ������ � OpenGLView.cpp
inline COpenGLDoc* COpenGLView::GetDocument() const
   { return reinterpret_cast<COpenGLDoc*>(m_pDocument); }
#endif

