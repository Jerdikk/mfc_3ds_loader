
// OpenGL.h : ������� ���� ��������� ��� ���������� OpenGL
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�������� stdafx.h �� ��������� ����� ����� � PCH"
#endif

#include "resource.h"       // �������� �������


// COpenGLApp:
// � ���������� ������� ������ ��. OpenGL.cpp
//

class COpenGLApp : public CWinApp
{
public:
	COpenGLApp();


// ���������������
public:
	virtual BOOL InitInstance();

// ����������
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern COpenGLApp theApp;
