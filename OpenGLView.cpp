
// OpenGLView.cpp : реализация класса COpenGLView
//

#include "stdafx.h"
// SHARED_HANDLERS можно определить в обработчиках фильтров просмотра реализации проекта ATL, эскизов
// и поиска; позволяет совместно использовать код документа в данным проекте.
#ifndef SHARED_HANDLERS
#include "OpenGL.h"
#endif

#include "OpenGLDoc.h"
#include "OpenGLView.h"
#include <GL/glew.h>
#include <GL/wglew.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/filesystem.h>
#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>
#include "3ds.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////////
//// для OpenGL
//////////////////////////////////////////////////////////////////////////////

// Window Event helpers
/////////


#define FILE_NAME  L"face.3ds"							// This is the 3D file we will load.

UINT g_Texture[MAX_TEXTURES] = { 0 };						// This holds the texture info, referenced by an ID

CLoad3DS g_Load3ds;										// This is 3DS class.  This should go in a good model class.
t3DModel g_3DModel;										// This holds the 3D Model info that we load in

int   g_ViewMode = GL_TRIANGLES;					// We want the default drawing mode to be normal
bool  g_bLighting = true;							// Turn lighting on initially
float g_RotateX = 0.0f;							// This is the current value at which the model is rotated
float g_RotationSpeed = 0.8f;							// This is the speed that our model rotates.  (-speed rotates left)




//Init
void InitializeOpenGL();

//Resize
bool ChangeSize(int w, int h);

//Draw
void Draw(void);
void PreRenderScene(void);
void RenderStockScene(void);
void RenderScene(void);
//void drawSolidSphere(GLfloat radius, GLint slices, GLint stacks);
//void drawSolidCube(GLfloat size);


//OpenGL State Management
////////
//GLuint program1;  //Shader
//GLint uColor;     //Shader color input
//GLint uLightPosition;//Shader light position input
//GLint mvIndex;    //Shader positioning input
//GLint projIndex;     //Shader projection input
//mat4 p, mv;       //Local projection and positioning variables


				  // Scene Related Functions and Variables
				  ////////

				  //Model Control Variables
//GLfloat rotY = 0;    //rotate model around y axis
//GLfloat rotX = 0;    //rotate model around x axis

// settings
unsigned int SCR_WIDTH;
unsigned int SCR_HEIGHT;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;
unsigned int texture1, texture2;

unsigned int shaderProgram;
unsigned int VBO, VAO;
Shader ourShader;
// world space positions of our cubes
glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};
//////////////////////////////////////////////////////////////////////////////
//// для OpenGL
//////////////////////////////////////////////////////////////////////////////


// COpenGLView

IMPLEMENT_DYNCREATE(COpenGLView, CView)

BEGIN_MESSAGE_MAP(COpenGLView, CView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_COMMAND(ID_FILE_OPEN, &COpenGLView::OnFileOpen)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////
//// для OpenGL
//////////////////////////////////////////////////////////////////////////////


// You will add stuff here!!!!
const char* const COpenGLView::_ErrorStrings[] = {
	{ "No Error" },                     // 0
	{ "Unable to get a DC" },           // 1
	{ "ChoosePixelFormat failed" },     // 2
	{ "SelectPixelFormat failed" },     // 3
	{ "wglCreateContext failed" },      // 4
	{ "wglMakeCurrent failed" },        // 5
	{ "wglDeleteContext failed" },      // 6
	{ "SwapBuffers failed" },           // 7
};

//////////////////////////////////////////////////////////////////////////////
//// для OpenGL
//////////////////////////////////////////////////////////////////////////////


// создание/уничтожение COpenGLView

COpenGLView::COpenGLView()
//////////////////////////////////////////////////////////////////////////////
//// для OpenGL
//////////////////////////////////////////////////////////////////////////////
// You will add the following line !!!
	: m_hRC(0), m_pDC(0), m_ErrorString(_ErrorStrings[0]) // Call constructors
//////////////////////////////////////////////////////////////////////////////
//// для OpenGL
//////////////////////////////////////////////////////////////////////////////

{
	// TODO: добавьте код создания

}

COpenGLView::~COpenGLView()
{
}

BOOL COpenGLView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: изменить класс Window или стили посредством изменения
	//  CREATESTRUCT cs

	//////////////////////////////////////////////////////////////////////////////
	//// для OpenGL
	//////////////////////////////////////////////////////////////////////////////

	// You will add stuff here !!!
	// An OpenGL window must be created with the following flags and must not
	// include CS_PARENTDC for the class style.
	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	//////////////////////////////////////////////////////////////////////////////
	//// для OpenGL
	//////////////////////////////////////////////////////////////////////////////


	return CView::PreCreateWindow(cs);
}

// рисование COpenGLView

void COpenGLView::OnDraw(CDC* /*pDC*/)
{
	COpenGLDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: добавьте здесь код отрисовки для собственных данных
	//////////////////////////////////////////////////////////////////////////////
	//// для OpenGL
	//////////////////////////////////////////////////////////////////////////////

	Draw();

	//Swap buffers to show result
	if (FALSE == ::SwapBuffers(m_pDC->GetSafeHdc()))
	{
		SetError(7);
	}
	//////////////////////////////////////////////////////////////////////////////
	//// для OpenGL
	//////////////////////////////////////////////////////////////////////////////

}


// диагностика COpenGLView

#ifdef _DEBUG
void COpenGLView::AssertValid() const
{
	CView::AssertValid();
}

void COpenGLView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

COpenGLDoc* COpenGLView::GetDocument() const // встроена неотлаженная версия
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COpenGLDoc)));
	return (COpenGLDoc*)m_pDocument;
}
#endif //_DEBUG


// обработчики сообщений COpenGLView


int COpenGLView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	//////////////////////////////////////////////////////////////////////////////
	//// для OpenGL
	//////////////////////////////////////////////////////////////////////////////

	//GetRenderingContext();
	if (!GetRenderingContext())
	{
		//Something went wrong with getting the rendering context.
		//Create a popup with the error message, then quit.
		AfxMessageBox(CString(m_ErrorString));
		return -1;
	}

	// We now have a rendering context, so we can set the initial drawing state.
	// Find the initialize OpenGL function provided in the Lab 1 notes and call it here
	InitializeOpenGL();
	//////////////////////////////////////////////////////////////////////////////
	//// для OpenGL
	//////////////////////////////////////////////////////////////////////////////

	return 0;
}


void COpenGLView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: Add your message handler code here
	//////////////////////////////////////////////////////////////////////////////
	//// для OpenGL
	//////////////////////////////////////////////////////////////////////////////
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	//glDeleteProgram(shaderProgram);

	if (FALSE == ::wglDeleteContext(m_hRC))
	{
		SetError(6);
	}

	if (m_pDC)
	{
		delete m_pDC;
	}
	//////////////////////////////////////////////////////////////////////////////
	//// для OpenGL
	//////////////////////////////////////////////////////////////////////////////

}


void COpenGLView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	// A resize event occured; cx and cy are the window's new width and height.
	// Find the OpenGL change size function given in the Lab 1 notes and call it here
	ChangeSize(cx, cy);
}

//////////////////////////////////////////////////////////////////////////////
//// для OpenGL
//////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// GL Rendering Context Creation Functions
//
// Since we are using Windows native windowing, we need to set up our own
// OpenGL rendering context. These functions do it to the main view area.
// It is possible to do it to a smaller sub view. If you are curious, you can
// find tutorials on how to do that on the net.
//

// Error reporting utility
void COpenGLView::SetError(int e)
{
	// if there was no previous error,
	// then save this one
	if (_ErrorStrings[0] == m_ErrorString)
	{
		m_ErrorString = _ErrorStrings[e];
	}
}

BOOL COpenGLView::GetRenderingContext()
{
	// Can we put this in the constructor?
	m_pDC = new CClientDC(this);

	if (NULL == m_pDC) // failure to get DC
	{
		SetError(1);
		return FALSE;
	}


	if (!GetOldStyleRenderingContext())
	{
		return TRUE;
	}


	//Get a new style pixel format
	if (!SetupPixelFormat())
	{
		return FALSE;
	}

	//Setup request for OpenGL 3.2 Core Profile
	/*int attribs[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB,   3,
		WGL_CONTEXT_MINOR_VERSION_ARB,   2,
		WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		//WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
		0, 0  //End
	};*/

	// Specify that we want to create an OpenGL 3.3 core profile context
	int gl33_attribs[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
		WGL_CONTEXT_MINOR_VERSION_ARB, 3,
		WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		0,
	};


	if (wglewIsSupported("WGL_ARB_create_context") == 1)
	{
		//If this driver supports new style rendering contexts, create one
		HGLRC oldContext = m_hRC;
		if (0 == (m_hRC = m_hRC = wglCreateContextAttribsARB(m_pDC->GetSafeHdc(), 0, gl33_attribs)))
		{
			SetError(4);
			return FALSE;
		}

	//	if (!wglMakeCurrent(NULL, NULL))
	//		wglDeleteContext(oldContext);
		if (FALSE == wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC))
		{
			SetError(5);
			return FALSE;
		}
	}
	else
	{
		//Otherwise use the old style rendering context we created earlier.
		AfxMessageBox(_T("GL 3.2 Context not possible. Using old style context. (GL 2.1 and before)"));
	}

	return TRUE;
}

BOOL COpenGLView::GetOldStyleRenderingContext()
{
	//A generic pixel format descriptor. This will be replaced with a more
	//specific and modern one later, so don't worry about it too much.
/*	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW |            // support window
		PFD_SUPPORT_OPENGL |            // support OpenGL
		PFD_DOUBLEBUFFER,               // double buffered
		PFD_TYPE_RGBA,                  // RGBA type
		32,                             // 32-bit color depth
		0, 0, 0, 0, 0, 0,               // color bits ignored
		0,                              // no alpha buffer
		0,                              // shift bit ignored
		0,                              // no accumulation buffer
		0, 0, 0, 0,                     // accum bits ignored
		24,                        // 24-bit z-buffer
		0,                              // no stencil buffer
		0,                              // no auxiliary buffer
		PFD_MAIN_PLANE,                 // main layer
		0,                              // reserved
		0, 0, 0                         // layer masks ignored
	};

	// Get the id number for the best match supported by the hardware device context
	// to what is described in pfd
	int pixelFormat = ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd);

	//If there's no match, report an error
	if (0 == pixelFormat)
	{
		SetError(2);
		return FALSE;
	}

	//If there is an acceptable match, set it as the current 
	if (FALSE == SetPixelFormat(m_pDC->GetSafeHdc(), pixelFormat, &pfd))
	{
		SetError(3);
		return FALSE;
	}

	//Create a context with this pixel format
	if (0 == (m_hRC = wglCreateContext(m_pDC->GetSafeHdc())))
	{
		SetError(4);
		return FALSE;
	}

	//Make it current. Now we're ready to get extended features.
	if (FALSE == wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC))
	{
		SetError(5);
		return FALSE;
	}
	return TRUE;*/

	WNDCLASSA window_class;// = {
	ZeroMemory(&window_class, sizeof(WNDCLASSA));
	window_class.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	window_class.lpfnWndProc = DefWindowProcA;
	window_class.hInstance = GetModuleHandle(0);
	window_class.lpszClassName = "Dummy_WGL_djuasiodwa";
	//};

	if (!RegisterClassA(&window_class)) {
		AfxMessageBox(L"Failed to register dummy OpenGL window.");
		return FALSE;
	}

	HWND dummy_window = CreateWindowExA(
		0,
		window_class.lpszClassName,
		"Dummy OpenGL Window",
		0,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		0,
		0,
		window_class.hInstance,
		0);

	if (!dummy_window) {
		AfxMessageBox(L"Failed to create dummy OpenGL window.");
		return FALSE;
	}

	HDC dummy_dc = ::GetDC(dummy_window);

	PIXELFORMATDESCRIPTOR pfd;// = {
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(pfd);
		pfd.nVersion = 1;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
			pfd.cColorBits = 32; 
			pfd.cAlphaBits = 8;
			pfd.iLayerType = PFD_MAIN_PLANE;
			pfd.cDepthBits = 24;
			pfd.cStencilBits = 8;
	//};

	int pixel_format = ChoosePixelFormat(dummy_dc, &pfd);
	if (!pixel_format) {
		AfxMessageBox(L"Failed to find a suitable pixel format.");
		return FALSE;
	}
	if (!SetPixelFormat(dummy_dc, pixel_format, &pfd)) {
		AfxMessageBox(L"Failed to set the pixel format.");
		return FALSE;
	}

	HGLRC dummy_context = wglCreateContext(dummy_dc);
	if (!dummy_context) {
		AfxMessageBox(L"Failed to create a dummy OpenGL rendering context.");
		return FALSE;
	}

	if (!wglMakeCurrent(dummy_dc, dummy_context)) {
		AfxMessageBox(L"Failed to activate dummy OpenGL rendering context.");
		return FALSE;
	}

	//wglCreateContextAttribsARB = (wglCreateContextAttribsARB_type*)wglGetProcAddress(
	//	"wglCreateContextAttribsARB");
	//wglChoosePixelFormatARB = (wglChoosePixelFormatARB_type*)wglGetProcAddress(
	//	"wglChoosePixelFormatARB");


	//Get access to modern OpenGL functionality from this old style context.
	glewExperimental = GL_TRUE;
	if (GLEW_OK != glewInit())
	{
		AfxMessageBox(_T("GLEW could not be initialized!"));
		return FALSE;
	}


	wglMakeCurrent(dummy_dc, 0);
	wglDeleteContext(dummy_context);
	::ReleaseDC(dummy_window, dummy_dc);
	::DestroyWindow(dummy_window);
	return TRUE;
}

BOOL COpenGLView::SetupPixelFormat()
{
	//This is a modern pixel format attribute list.
	//It has an extensible structure. Just add in more argument pairs 
	//befroe the null to request more features.
	/*
	const int attribList[] =
	{
		WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
		WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
		WGL_ACCELERATION_ARB,   WGL_FULL_ACCELERATION_ARB,
		WGL_DOUBLE_BUFFER_ARB,  GL_TRUE,
		WGL_PIXEL_TYPE_ARB,     WGL_TYPE_RGBA_ARB,
		WGL_COLOR_BITS_ARB,     32,
		WGL_DEPTH_BITS_ARB,     24,
		WGL_STENCIL_BITS_ARB,   8,
		0, 0  //End
	};


	unsigned int numFormats;
	int pixelFormat;
	PIXELFORMATDESCRIPTOR pfd;

	//Select a pixel format number
	wglChoosePixelFormatARB(m_pDC->GetSafeHdc(), attribList, NULL, 1, &pixelFormat, &numFormats);

	*/

	/*const int attribList[] =
	{
		WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
		WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
		WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
		WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
		WGL_COLOR_BITS_ARB, 32,
		WGL_DEPTH_BITS_ARB, 24,
		WGL_STENCIL_BITS_ARB, 8,
		0, // End
	};

	int pixelFormat;
	UINT numFormats;	
	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    //Flags
		PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
		32,                   // Colordepth of the framebuffer.
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		24,                   // Number of bits for the depthbuffer
		8,                    // Number of bits for the stencilbuffer
		0,                    // Number of Aux buffers in the framebuffer.
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	int ttt = wglChoosePixelFormatARB(m_pDC->GetSafeHdc(), attribList, NULL, 1, &pixelFormat, &numFormats);


	//Optional: Get the pixel format's description. We must provide a 
	//description to SetPixelFormat(), but its contents mean little.
	//According to MSDN: 
	//  The system's metafile component uses this structure to record the logical
	//  pixel format specification. The structure has no other effect upon the
	//  behavior of the SetPixelFormat function.
	//DescribePixelFormat(m_pDC->GetSafeHdc(), pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

	//Set it as the current 
	if (FALSE == SetPixelFormat(m_pDC->GetSafeHdc(), pixelFormat, &pfd))
	{
		SetError(3);
		return FALSE;
	}

	return TRUE;*/

	// Now we can choose a pixel format the modern way, using wglChoosePixelFormatARB.
	int pixel_format_attribs[] = {
		WGL_DRAW_TO_WINDOW_ARB,     GL_TRUE,
		WGL_SUPPORT_OPENGL_ARB,     GL_TRUE,
		WGL_DOUBLE_BUFFER_ARB,      GL_TRUE,
		WGL_ACCELERATION_ARB,       WGL_FULL_ACCELERATION_ARB,
		WGL_PIXEL_TYPE_ARB,         WGL_TYPE_RGBA_ARB,
		WGL_COLOR_BITS_ARB,         32,
		WGL_DEPTH_BITS_ARB,         24,
		WGL_STENCIL_BITS_ARB,       8,
		0
	};

	int pixel_format;
	UINT num_formats;
	wglChoosePixelFormatARB(m_pDC->GetSafeHdc(), pixel_format_attribs, 0, 1, &pixel_format, &num_formats);
	if (!num_formats) {
		AfxMessageBox(L"Failed to set the OpenGL 3.3 pixel format.");
	}

	PIXELFORMATDESCRIPTOR pfd;
	DescribePixelFormat(m_pDC->GetSafeHdc(), pixel_format, sizeof(pfd), &pfd);
	if (!SetPixelFormat(m_pDC->GetSafeHdc(), pixel_format, &pfd)) {
		AfxMessageBox(L"Failed to set the OpenGL 3.3 pixel format.");
		return FALSE;
	}
	return TRUE;
}


//Function: InitializeOpenGL
//Purpose:
//    Put OpenGL into a useful state for the intended drawing.
//    In this one we:
//        - choose a background color
//        - set up depth testing (Requires GL_DEPTH in Pixel Format)
//        - turn on the lights
//        - set up simplified material lighting properties
//        - set an initial camera position
void InitializeOpenGL()
{
	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile our shader zprogram
	// ------------------------------------
	ourShader.init("7.4.camera.vs", "7.4.camera.fs");

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};
	
	//unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	// load and create a texture 
	// -------------------------

	// texture 1
	// ---------
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char *data = stbi_load(FileSystem::getPath("resources/textures/container.jpg").c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	// texture 2
	// ---------
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	data = stbi_load(FileSystem::getPath("resources/textures/awesomeface.png").c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		// note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
	// -------------------------------------------------------------------------------------------
	ourShader.use();
	ourShader.setInt("texture1", 0);
	ourShader.setInt("texture2", 1);



	//Set up shader
	//program1 = InitShader("vshader.glsl", "fshader.glsl");

	//glUseProgram(program1);

	//Get locations of transformation matrices from shader
	//mvIndex = glGetUniformLocation(program1, "mv");
	//projIndex = glGetUniformLocation(program1, "p");

	//Get locations of lighting uniforms from shader
	//uLightPosition = glGetUniformLocation(program1, "lightPosition");
	//uColor = glGetUniformLocation(program1, "uColor");

	//Set default lighting and material properties in shader.
	//glUniform4f(uLightPosition, 0.0f, 0.0f, 10.0f, 0.0f);
	//glUniform3f(uColor, 1.0f, 1.0f, 1.0f);

	//Configure urgl object in uofrGraphics library
	//urgl.connectShader(program1, "vPosition", "vNormal", NULL);

	//glEnable(GL_DEPTH_TEST);
}
// Function: Draw
// Purpose:
//     Control drawing of the scene. To be called whenever the window
//     needs redrawing.
void Draw()
{
	// Clear the screen and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	PreRenderScene();
	RenderStockScene();
	RenderScene();
}

// Use this to perform view transforms or other tasks
// that will affect both stock scene and detail scene
void PreRenderScene()
{
	// select a default viewing transformation
	// of a 20 degree rotation about the X axis
	// then a -5 unit transformation along Z
//mv = mat4();
	//mv *= Translate(0.0f, 0.0f, -5.0f);
	//mv *= RotateX(20.0f);

	//Allow variable controlled rotation around local x and y axes.
	//mv *= RotateX(rotX);
	//mv *= RotateY(rotY);
}


// Function: RenderStockScene
// Purpose:
//     Draw a stock scene that looks like a
//     black and white checkerboard
void RenderStockScene()
{
	const GLfloat delta = 0.5f;

	// define four vertices that make up a square.
	//vec4 v1(0.0f, 0.0f, 0.0f, 1.0f);
	//vec4 v2(0.0f, 0.0f, delta, 1.0f);
	//vec4 v3(delta, 0.0f, delta, 1.0f);
	//vec4 v4(delta, 0.0f, 0.0f, 1.0f);


	int color = 0;

	// define the two colors
	//vec3 color1(0.9f, 0.9f, 0.9f);
	//vec3 color2(0.05f, 0.05f, 0.05f);

	//mat4 placementX = mv;
	//mat4 placementZ;
	//placementX *= Translate(-10.0f * delta, 0.0f, -10.0f * delta);
	//for (int x = -10; x <= 10; x++)
	//{
	//	placementZ = placementX;
	//	for (int z = -10; z <= 10; z++)
	//	{
	//		glUniform3fv(uColor, 1, (color++) % 2 ? color1 : color2);
	//		glUniformMatrix4fv(mvIndex, 1, GL_TRUE, placementZ);
	//		urgl.drawQuad(v1, v2, v3, v4);
	//		placementZ *= Translate(0.0f, 0.0f, delta);
	//	}
	//	placementX *= Translate(delta, 0.0f, 0.0f);

	//}
}

// Function: RenderScene
// Purpose:
//     Your playground. Code additional scene details here.
void RenderScene()
{
	// render
	   // ------
	CTime t = CTime::GetTickCount();	
	float currentFrame = t.GetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	// input
	// -----
	//processInput(window);

	// render
	// ------
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// bind textures on corresponding texture units
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);

	// activate shader
	ourShader.use();

	// pass projection matrix to shader (note that in this case it could change every frame)
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	ourShader.setMat4("projection", projection);

	// camera/view transformation
	glm::mat4 view = camera.GetViewMatrix();
	ourShader.setMat4("view", view);

	// render boxes
	glBindVertexArray(VAO);
	for (unsigned int i = 0; i < 10; i++)
	{
		// calculate the model matrix for each object and pass it to shader before drawing
		glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		model = glm::translate(model, cubePositions[i]);
		float angle = 20.0f * i;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		ourShader.setMat4("model", model);

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	// Set the drawing color to red
	// Arguments are Red, Green, Blue
	//glUniform3f(uColor, 1.0f, 0.0f, 0.0f);

	// Move the "drawing space" up by the sphere's radius
	// so the sphere is on top of the checkerboard
	// mv is a transformation matrix. It accumulates transformations through
	// right side matrix multiplication.
	//mv *= Translate(0.0f, 0.5f, 0.0f);

	// Rotate drawing space by 90 degrees around X so the sphere's poles
	// are vertical
	//mv *= RotateX(90.0f);

	//Send the transformation matrix to the shader
	//glUniformMatrix4fv(mvIndex, 1, GL_TRUE, mv);

	// Draw a sphere.
	// Arguments are Radius, Slices, Stacks
	// Sphere is centered around current origin.
	//urgl.drawSolidSphere(0.5f, 20, 20);


	// when we rotated the sphere earlier, we rotated drawing space
	// and created a new "frame"
	// to move the cube up or down we now have to refer to the z-axis
	//mv *= Translate(0.0f, 0.0f, 0.5f);

	//Send the transformation matrix to the shader
	//glUniformMatrix4fv(mvIndex, 1, GL_TRUE, mv);

	// set the drawing color to light blue
	//glUniform3f(uColor, 0.5f, 0.5f, 1.0f);

	// Draw the cube.
	// Argument refers to length of side of cube.
	// Cube is centered around current origin.
	//if (g_3DModel.numOfObjects > 0)
	//{
	//	urgl.drawSolidCube(1.0f);
	//}
	//else
	//{
		
	//}
		
}

// Function: ChangeSize
// Purpose:
//     Tell OpenGL how to deal with a new window size.
// Arguments:
//     int w, h: new width and height of the window, respectively.
bool ChangeSize(int w, int h)
{
	GLfloat aspect_ratio; // width/height ratio

						  //Make sure the window size is valid
	if (0 >= w || 0 >= h)
	{
		return false;
	}


	// tell OpenGL to render to whole window area
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	// compute the aspect ratio
	// this is used to prevent the picture from distorting when
	// the window is resized
	aspect_ratio = (GLdouble)w / (GLdouble)h;

	lastX = w / 2.0f;
	lastY = h / 2.0f;

	SCR_WIDTH=w;
	SCR_HEIGHT=h;
	// calculate a new projection matrix
	//p = Perspective(50.0f, aspect_ratio, 0.5f, 20.0f);

	// send the projection to the shader
	//glUniformMatrix4fv(projIndex, 1, GL_TRUE, p);

	return true;
}

void COpenGLView::OnFileOpen()
{
	// TODO: добавьте свой код обработчика команд
	
	// TODO: добавьте свой код обработчика команд
	int ttt = 1;
	CFileDialog fileDialog(TRUE, NULL, L"*.3ds");	//объект класса выбора файла
	int result = fileDialog.DoModal();	//запустить диалоговое окно
	if (result == IDOK)	//если файл выбран
	{
		AfxMessageBox(fileDialog.GetPathName()); // показать полный путь


	// First we need to actually load the .3DS file.  We just pass in an address to
	// our t3DModel structure and the file name string we want to load ("face.3ds").

		g_Load3ds.Import3DS(&g_3DModel, (const WCHAR *)fileDialog.GetPathName());			// Load our .3DS file into our model structure
		/*
		// Depending on how many textures we found, load each one (Assuming .BMP)
		// If you want to load other files than bitmaps, you will need to adjust CreateTexture().
		// Below, we go through all of the materials and check if they have a texture map to load.
		// Otherwise, the material just holds the color information and we don't need to load a texture.

		// Go through all the materials
		for(int i = 0; i < g_3DModel.numOfMaterials; i++)
		{
			// Check to see if there is a file name to load in this material
			if(strlen(g_3DModel.pMaterials[i].strFile) > 0)
			{
				// Use the name of the texture file to load the bitmap, with a texture ID (i).
				// We pass in our global texture array, the name of the texture, and an ID to reference it.
				CreateTexture(g_3DModel.pMaterials[i].strFile, g_Texture[i]);
			}

			// Set the texture ID for this material
			g_3DModel.pMaterials[i].texureId = i;
		}

			*/

	}

	/*
	При выборе нескольких файлов может возникнуть ситуация, что Вы не можете выбрать файлов больше некоторого количества. Выход в увеличении внутреннего буфера, вот так это можно сделать.

	int iBufferSize = 100000;
	CFileDialog fileDialog(TRUE,NULL,"*.*",OFN_ALLOWMULTISELECT |
			OFN_NOVALIDATE );	// выбор нескольких файлов
	fileDialog.m_ofn.nMaxFile = iBufferSize;	// устанавливаем размер буфера
	char* cNewBuffer = new char[iBufferSize];	// выделяем место
	fileDialog.m_ofn.lpstrFile = cNewBuffer;	// даем указатель на новое место
	fileDialog.m_ofn.lpstrFile[0] = NULL;	// ничего нет
	int result = fileDialog.DoModal();
	if (result==IDOK)
	{
		POSITION ps=fileDialog.GetStartPosition();	// получить начальную позицию
		while (ps) AfxMessageBox(fileDialog.GetNextPathName(ps));// пока есть выбранные файлы
	}
	delete []cNewBuffer;// освобождаем выделенную память
	После расширения можно указывать параметры окна диалога, вот я и указал, выбирай несколько файлов. А вот дальше мы должны получить позицию в переменную типа POSITION, эта переменная передается как указатель в функцию GetNextPathName(), а она выдает строку по этой позиции, и заодно уменьшает позицию. Когда файлов нет позиция станет равной 0.

	Фильтр на несколько файлов:

	CFileDialog fileDialog(TRUE,NULL,"*.mak;*.rc");
	Как видите, через точку с запятой можно передавать несколько расширений. Это не MFC придумало естественно. В Windows функция открытия файлов уже есть, и эта возможность взята из Win32 описания.

	Выбор типа. За типом указан фильтр. Расширения для открытия ограничены скобками |*.mak|.

	CFileDialog fileDialog(TRUE,NULL,"*.mak",NULL,"File Mak (*.mak)|*.mak|Resouce Files (*.res)|*.res|");
	int result = fileDialog.DoModal();

	Добавление расширения для сохранения файлов. Как видите теперь установлена 2-я переменная. Это расширение, которое будет добавляться в конец файла.

	CFileDialog fileDialog(FALSE,"obj","*.obj");

	*/
}


void COpenGLView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	if (firstMouse)
	{
		lastX = point.x;
		lastY = point.y;
		firstMouse = false;
	}

	float xoffset = point.x - lastX;
	float yoffset = lastY - point.y; // reversed since y-coordinates go from bottom to top

	lastX = point.x;
	lastY = point.y;

	camera.ProcessMouseMovement(xoffset, yoffset);
	CView::OnMouseMove(nFlags, point);
	Draw();

	//Swap buffers to show result
	if (FALSE == ::SwapBuffers(m_pDC->GetSafeHdc()))
	{
		SetError(7);
	}

}


void COpenGLView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного

	CView::OnLButtonDown(nFlags, point);
}


void COpenGLView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	if (nChar == 87)
		camera.ProcessKeyboard(FORWARD, 1);
	if (nChar == 83)
		camera.ProcessKeyboard(BACKWARD, 1);
	if (nChar == 65)
		camera.ProcessKeyboard(LEFT, 1);
	if (nChar == 68)
		camera.ProcessKeyboard(RIGHT, 1);
		
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
	Draw();

	//Swap buffers to show result
	if (FALSE == ::SwapBuffers(m_pDC->GetSafeHdc()))
	{
		SetError(7);
	}
}
