
// OpenGL.h : главный файл заголовка для приложения OpenGL
//
#pragma once

#ifndef __AFXWIN_H__
	#error "включить stdafx.h до включения этого файла в PCH"
#endif

#include "resource.h"       // основные символы


// COpenGLApp:
// О реализации данного класса см. OpenGL.cpp
//

class COpenGLApp : public CWinApp
{
public:
	COpenGLApp();


// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern COpenGLApp theApp;
