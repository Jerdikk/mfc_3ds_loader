
// OpenGLView.cpp : ���������� ������ COpenGLView
//

#include "stdafx.h"
// SHARED_HANDLERS ����� ���������� � ������������ �������� ��������� ���������� ������� ATL, �������
// � ������; ��������� ��������� ������������ ��� ��������� � ������ �������.
#ifndef SHARED_HANDLERS
#include "OpenGL.h"
#endif

#include "OpenGLDoc.h"
#include "OpenGLView.h"
#include "3ds.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////////
//// ��� OpenGL
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
void drawSolidSphere(GLfloat radius, GLint slices, GLint stacks);
void drawSolidCube(GLfloat size);


//OpenGL State Management
////////
GLuint program1;  //Shader
GLint uColor;     //Shader color input
GLint uLightPosition;//Shader light position input
GLint mvIndex;    //Shader positioning input
GLint projIndex;     //Shader projection input
mat4 p, mv;       //Local projection and positioning variables


				  // Scene Related Functions and Variables
				  ////////

				  //Model Control Variables
GLfloat rotY = 0;    //rotate model around y axis
GLfloat rotX = 0;    //rotate model around x axis
//////////////////////////////////////////////////////////////////////////////
//// ��� OpenGL
//////////////////////////////////////////////////////////////////////////////


// COpenGLView

IMPLEMENT_DYNCREATE(COpenGLView, CView)

BEGIN_MESSAGE_MAP(COpenGLView, CView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_COMMAND(ID_FILE_OPEN, &COpenGLView::OnFileOpen)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////
//// ��� OpenGL
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
//// ��� OpenGL
//////////////////////////////////////////////////////////////////////////////


// ��������/����������� COpenGLView

COpenGLView::COpenGLView()
//////////////////////////////////////////////////////////////////////////////
//// ��� OpenGL
//////////////////////////////////////////////////////////////////////////////
// You will add the following line !!!
	: m_hRC(0), m_pDC(0), m_ErrorString(_ErrorStrings[0]) // Call constructors
//////////////////////////////////////////////////////////////////////////////
//// ��� OpenGL
//////////////////////////////////////////////////////////////////////////////

{
	// TODO: �������� ��� ��������

}

COpenGLView::~COpenGLView()
{
}

BOOL COpenGLView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �������� ����� Window ��� ����� ����������� ���������
	//  CREATESTRUCT cs

	//////////////////////////////////////////////////////////////////////////////
	//// ��� OpenGL
	//////////////////////////////////////////////////////////////////////////////

	// You will add stuff here !!!
	// An OpenGL window must be created with the following flags and must not
	// include CS_PARENTDC for the class style.
	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	//////////////////////////////////////////////////////////////////////////////
	//// ��� OpenGL
	//////////////////////////////////////////////////////////////////////////////


	return CView::PreCreateWindow(cs);
}

// ��������� COpenGLView

void COpenGLView::OnDraw(CDC* /*pDC*/)
{
	COpenGLDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �������� ����� ��� ��������� ��� ����������� ������
	//////////////////////////////////////////////////////////////////////////////
	//// ��� OpenGL
	//////////////////////////////////////////////////////////////////////////////

	Draw();

	//Swap buffers to show result
	if (FALSE == ::SwapBuffers(m_pDC->GetSafeHdc()))
	{
		SetError(7);
	}
	//////////////////////////////////////////////////////////////////////////////
	//// ��� OpenGL
	//////////////////////////////////////////////////////////////////////////////

}


// ����������� COpenGLView

#ifdef _DEBUG
void COpenGLView::AssertValid() const
{
	CView::AssertValid();
}

void COpenGLView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

COpenGLDoc* COpenGLView::GetDocument() const // �������� ������������ ������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COpenGLDoc)));
	return (COpenGLDoc*)m_pDocument;
}
#endif //_DEBUG


// ����������� ��������� COpenGLView


int COpenGLView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	//////////////////////////////////////////////////////////////////////////////
	//// ��� OpenGL
	//////////////////////////////////////////////////////////////////////////////

	GetRenderingContext();
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
	//// ��� OpenGL
	//////////////////////////////////////////////////////////////////////////////

	return 0;
}


void COpenGLView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: Add your message handler code here
	//////////////////////////////////////////////////////////////////////////////
	//// ��� OpenGL
	//////////////////////////////////////////////////////////////////////////////

	if (FALSE == ::wglDeleteContext(m_hRC))
	{
		SetError(6);
	}

	if (m_pDC)
	{
		delete m_pDC;
	}
	//////////////////////////////////////////////////////////////////////////////
	//// ��� OpenGL
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
//// ��� OpenGL
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

	//Get access to modern OpenGL functionality from this old style context.
	glewExperimental = GL_TRUE;
	if (GLEW_OK != glewInit())
	{
		AfxMessageBox(_T("GLEW could not be initialized!"));
		return FALSE;
	}

	//Get a new style pixel format
	if (!SetupPixelFormat())
	{
		return FALSE;
	}

	//Setup request for OpenGL 3.2 Core Profile
	int attribs[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB,   3,
		WGL_CONTEXT_MINOR_VERSION_ARB,   2,
		WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		//WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
		0, 0  //End
	};

	if (wglewIsSupported("WGL_ARB_create_context") == 1)
	{
		//If this driver supports new style rendering contexts, create one
		HGLRC oldContext = m_hRC;
		if (0 == (m_hRC = m_hRC = wglCreateContextAttribsARB(m_pDC->GetSafeHdc(), 0, attribs)))
		{
			SetError(4);
			return FALSE;
		}

		if (!wglMakeCurrent(NULL, NULL))
			wglDeleteContext(oldContext);
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
	static PIXELFORMATDESCRIPTOR pfd =
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
	return TRUE;
}

BOOL COpenGLView::SetupPixelFormat()
{
	//This is a modern pixel format attribute list.
	//It has an extensible structure. Just add in more argument pairs 
	//befroe the null to request more features.
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
	//Set up shader
	program1 = InitShader("vshader.glsl", "fshader.glsl");

	glUseProgram(program1);

	//Get locations of transformation matrices from shader
	mvIndex = glGetUniformLocation(program1, "mv");
	projIndex = glGetUniformLocation(program1, "p");

	//Get locations of lighting uniforms from shader
	uLightPosition = glGetUniformLocation(program1, "lightPosition");
	uColor = glGetUniformLocation(program1, "uColor");

	//Set default lighting and material properties in shader.
	glUniform4f(uLightPosition, 0.0f, 0.0f, 10.0f, 0.0f);
	glUniform3f(uColor, 1.0f, 1.0f, 1.0f);

	//Configure urgl object in uofrGraphics library
	urgl.connectShader(program1, "vPosition", "vNormal", NULL);

	glEnable(GL_DEPTH_TEST);
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
	mv = mat4();
	mv *= Translate(0.0f, 0.0f, -5.0f);
	mv *= RotateX(20.0f);

	//Allow variable controlled rotation around local x and y axes.
	mv *= RotateX(rotX);
	mv *= RotateY(rotY);
}


// Function: RenderStockScene
// Purpose:
//     Draw a stock scene that looks like a
//     black and white checkerboard
void RenderStockScene()
{
	const GLfloat delta = 0.5f;

	// define four vertices that make up a square.
	vec4 v1(0.0f, 0.0f, 0.0f, 1.0f);
	vec4 v2(0.0f, 0.0f, delta, 1.0f);
	vec4 v3(delta, 0.0f, delta, 1.0f);
	vec4 v4(delta, 0.0f, 0.0f, 1.0f);


	int color = 0;

	// define the two colors
	vec3 color1(0.9f, 0.9f, 0.9f);
	vec3 color2(0.05f, 0.05f, 0.05f);

	mat4 placementX = mv;
	mat4 placementZ;
	placementX *= Translate(-10.0f * delta, 0.0f, -10.0f * delta);
	for (int x = -10; x <= 10; x++)
	{
		placementZ = placementX;
		for (int z = -10; z <= 10; z++)
		{
			glUniform3fv(uColor, 1, (color++) % 2 ? color1 : color2);
			glUniformMatrix4fv(mvIndex, 1, GL_TRUE, placementZ);
			urgl.drawQuad(v1, v2, v3, v4);
			placementZ *= Translate(0.0f, 0.0f, delta);
		}
		placementX *= Translate(delta, 0.0f, 0.0f);

	}
}

// Function: RenderScene
// Purpose:
//     Your playground. Code additional scene details here.
void RenderScene()
{
	// draw a red sphere inside a light blue cube

	// Set the drawing color to red
	// Arguments are Red, Green, Blue
	glUniform3f(uColor, 1.0f, 0.0f, 0.0f);

	// Move the "drawing space" up by the sphere's radius
	// so the sphere is on top of the checkerboard
	// mv is a transformation matrix. It accumulates transformations through
	// right side matrix multiplication.
	mv *= Translate(0.0f, 0.5f, 0.0f);

	// Rotate drawing space by 90 degrees around X so the sphere's poles
	// are vertical
	mv *= RotateX(90.0f);

	//Send the transformation matrix to the shader
	glUniformMatrix4fv(mvIndex, 1, GL_TRUE, mv);

	// Draw a sphere.
	// Arguments are Radius, Slices, Stacks
	// Sphere is centered around current origin.
	urgl.drawSolidSphere(0.5f, 20, 20);


	// when we rotated the sphere earlier, we rotated drawing space
	// and created a new "frame"
	// to move the cube up or down we now have to refer to the z-axis
	mv *= Translate(0.0f, 0.0f, 0.5f);

	//Send the transformation matrix to the shader
	glUniformMatrix4fv(mvIndex, 1, GL_TRUE, mv);

	// set the drawing color to light blue
	glUniform3f(uColor, 0.5f, 0.5f, 1.0f);

	// Draw the cube.
	// Argument refers to length of side of cube.
	// Cube is centered around current origin.
	urgl.drawSolidCube(1.0f);
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

	// calculate a new projection matrix
	p = Perspective(50.0f, aspect_ratio, 0.5f, 20.0f);

	// send the projection to the shader
	glUniformMatrix4fv(projIndex, 1, GL_TRUE, p);

	return true;
}

void COpenGLView::OnFileOpen()
{
	// TODO: �������� ���� ��� ����������� ������
	int ttt = 1;
	// TODO: �������� ���� ��� ����������� ������
	int ttt = 1;
	CFileDialog fileDialog(TRUE, NULL, L"*.3ds");	//������ ������ ������ �����
	int result = fileDialog.DoModal();	//��������� ���������� ����
	if (result == IDOK)	//���� ���� ������
	{
		AfxMessageBox(fileDialog.GetPathName()); // �������� ������ ����


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
	��� ������ ���������� ������ ����� ���������� ��������, ��� �� �� ������ ������� ������ ������ ���������� ����������. ����� � ���������� ����������� ������, ��� ��� ��� ����� �������.

	int iBufferSize = 100000;
	CFileDialog fileDialog(TRUE,NULL,"*.*",OFN_ALLOWMULTISELECT |
			OFN_NOVALIDATE );	// ����� ���������� ������
	fileDialog.m_ofn.nMaxFile = iBufferSize;	// ������������� ������ ������
	char* cNewBuffer = new char[iBufferSize];	// �������� �����
	fileDialog.m_ofn.lpstrFile = cNewBuffer;	// ���� ��������� �� ����� �����
	fileDialog.m_ofn.lpstrFile[0] = NULL;	// ������ ���
	int result = fileDialog.DoModal();
	if (result==IDOK)
	{
		POSITION ps=fileDialog.GetStartPosition();	// �������� ��������� �������
		while (ps) AfxMessageBox(fileDialog.GetNextPathName(ps));// ���� ���� ��������� �����
	}
	delete []cNewBuffer;// ����������� ���������� ������
	����� ���������� ����� ��������� ��������� ���� �������, ��� � � ������, ������� ��������� ������. � ��� ������ �� ������ �������� ������� � ���������� ���� POSITION, ��� ���������� ���������� ��� ��������� � ������� GetNextPathName(), � ��� ������ ������ �� ���� �������, � ������ ��������� �������. ����� ������ ��� ������� ������ ������ 0.

	������ �� ��������� ������:

	CFileDialog fileDialog(TRUE,NULL,"*.mak;*.rc");
	��� ������, ����� ����� � ������� ����� ���������� ��������� ����������. ��� �� MFC ��������� �����������. � Windows ������� �������� ������ ��� ����, � ��� ����������� ����� �� Win32 ��������.

	����� ����. �� ����� ������ ������. ���������� ��� �������� ���������� �������� |*.mak|.

	CFileDialog fileDialog(TRUE,NULL,"*.mak",NULL,"File Mak (*.mak)|*.mak|Resouce Files (*.res)|*.res|");
	int result = fileDialog.DoModal();

	���������� ���������� ��� ���������� ������. ��� ������ ������ ����������� 2-� ����������. ��� ����������, ������� ����� ����������� � ����� �����.

	CFileDialog fileDialog(FALSE,"obj","*.obj");

	*/
}