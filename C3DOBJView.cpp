// ----------------------------------------------------------------------------
/*
The MIT License (MIT)

Copyright (c) 2000- Brad P. Taylor

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
// ----------------------------------------------------------------------------


#include "stdafx.h"
#include "DitherStudio.h"
#include "C3DOBJView.h"
#include "CSimpleHeightmap.h"
#include "SimpleEntitySystem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// C3DOBJView

#define STARTING_Z_VALUE (-314.15926f*2.0)

IMPLEMENT_DYNCREATE(C3DOBJView, CView)

C3DOBJView::C3DOBJView() :
	m_ButtonDownPos(),
	m_xAngle(45.0),
	m_xButtonDownAngle(45.0),
	m_yAngle(45.0),
	m_yButtonDownAngle(45.0),
	m_zDistance(STARTING_Z_VALUE), //-650.0f),
	m_zButtonDownDistance(STARTING_Z_VALUE), //-650.0f),
	m_fMinZDistance(0.5f),
	m_fMaxZDistance(1800.0f),
	m_LookAtPos(),
	m_ButtonDownLookAtPos(),
	m_VisibleRegion(900.0f, 900.0f, 900.0f),
	m_bFlipFlopScales(true),
	m_bShowUniqueColors(false),
	m_bRebuildRequested(false)
{
	ResetView();
}

void C3DOBJView::ResetView()
{
	m_LookAtPos = BPT::VECTOR3();
	m_xAngle = 45.0f;
	m_yAngle = 45.0f;
	m_xButtonDownAngle = 45.0f;
	m_yButtonDownAngle = 45.0f;
	m_zDistance = STARTING_Z_VALUE; // -650.0f;
	m_zButtonDownDistance = STARTING_Z_VALUE; // -650.0f;
}

C3DOBJView::~C3DOBJView()
{
}

#ifdef _DEBUG
void C3DOBJView::AssertValid() const
{
	CView::AssertValid();
}

void C3DOBJView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

C3DOBJView::doc_type* C3DOBJView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDitherStudioDoc)));
	return (C3DOBJView::doc_type*)m_pDocument;
}
#endif //_DEBUG



BEGIN_MESSAGE_MAP(C3DOBJView, CView)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_CANCELMODE()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_COMMAND(ID_RESET_3D_VIEW, &C3DOBJView::OnReset3dView)
	ON_UPDATE_COMMAND_UI(ID_RESET_3D_VIEW, &C3DOBJView::OnUpdateReset3dView)
	ON_COMMAND(ID_3D_FLIP_FLOP, &C3DOBJView::On3dFlipFlop)
	ON_UPDATE_COMMAND_UI(ID_3D_FLIP_FLOP, &C3DOBJView::OnUpdate3dFlipFlop)
	ON_COMMAND(ID_3D_SHOW_UNIQUE, &C3DOBJView::On3dShowUnique)
	ON_UPDATE_COMMAND_UI(ID_3D_SHOW_UNIQUE, &C3DOBJView::OnUpdate3dShowUnique)
END_MESSAGE_MAP()



// C3DOBJView message handlers


BOOL C3DOBJView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Add your specialized code here and/or call the base class

	BOOL bResult = CView::PreCreateWindow(cs);

	cs.style |= WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

	return bResult;
}



int C3DOBJView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_GLContext.Attach(m_hWnd)) {

		return -1;

	}

	return 0;
}


BOOL C3DOBJView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}


//void C3DOBJView::OnPaint()
//{
//	CPaintDC dc(this); // device context for painting
//					   // TODO: Add your message handler code here
//					   // Do not call CWnd::OnPaint() for painting messages
//}


void C3DOBJView::OnDraw(CDC* /*pDC*/)
{
	CDitherStudioDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	RenderScene();	// TODO: Add your specialized code here and/or call the base class
}


void C3DOBJView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: Add your message handler code here

	m_GLContext.Release();
}


void C3DOBJView::OnSize(UINT nType, int cx, int cy)
{
	if (m_GLContext.MakeCurrent()) {

		if (0 == cy) cy = 1;

		glViewport(0, 0, cx, cy);

#if 1

		float fAspect = (float)cx / (float)cy;
		m_projectionMat = BPT::glm::perspective(45.0f, fAspect, 0.125f, m_fMaxZDistance * 2.0f);

#else

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

#if 1
		float fAspect = (float)cx / (float)cy;
		BPT::glm::mat4 projMat = BPT::glm::perspective(45.0f, fAspect, 0.125f, m_fMaxZDistance * 2.0f);
		glLoadMatrixf(&projMat[0][0]);
#else
		GLfloat fAspect = (GLfloat)cx / (GLfloat)cy;
		gluPerspective(45.0f, fAspect, 0.5f, m_fMaxZDistance * 2.0f);
#endif

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//TRACE("OnSize %d,%d\n", cx, cy);

#endif

	}

	InvalidateRect(NULL, false);
}

bool
C3DOBJView::ChangeZDistance(const GLfloat zDistance)
{
	if (zDistance == m_zDistance) return false;

	m_zDistance = std::min(std::max(m_fMinZDistance, std::abs(zDistance)), m_fMaxZDistance) * (std::signbit(zDistance) ? -1.0f : 1.0f);

	return true;
}

void C3DOBJView::OnButtonDown(UINT nFlags, CPoint point)
{
	m_ButtonDownLookAtPos = m_LookAtPos;
	m_ButtonDownPos = point;
	m_zButtonDownDistance = m_zDistance;
	m_xButtonDownAngle = m_xAngle;
	m_yButtonDownAngle = m_yAngle;
	SetCapture();
	TrackMouse(nFlags, point);
}

void C3DOBJView::OnLButtonDown(UINT nFlags, CPoint point)
{
	OnButtonDown(nFlags, point);
}


void C3DOBJView::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (this == GetCapture()) {

		ReleaseCapture();

	}

	TrackMouse(nFlags, point);
}


void C3DOBJView::OnCancelMode()
{
	if (this == GetCapture()) {

		ReleaseCapture();

	}
}


void C3DOBJView::OnRButtonDown(UINT nFlags, CPoint point)
{
	OnButtonDown(nFlags, point);
}


void C3DOBJView::OnRButtonUp(UINT nFlags, CPoint point)
{
	if (this == GetCapture()) {

		ReleaseCapture();

	}

	TrackMouse(nFlags, point);
}


void C3DOBJView::OnMouseMove(UINT nFlags, CPoint point)
{
	TrackMouse(nFlags, point);
}


BOOL C3DOBJView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	GLfloat zCoeff = 1.0f;

	if (0 < zDelta) {

		zCoeff = 0.75f;

	}
	else if (0 > zDelta) {

		zCoeff = 1.25f;

	}

	if (ChangeZDistance(m_zDistance * zCoeff)) {

		RenderScene();

	}

	return TRUE;
}


void C3DOBJView::OnMButtonDown(UINT nFlags, CPoint point)
{
	OnButtonDown(nFlags, point);
}


void C3DOBJView::OnMButtonUp(UINT nFlags, CPoint point)
{
	if (this == GetCapture()) {

		ReleaseCapture();

	}

	TrackMouse(nFlags, point);
}


// ----------------------------------------------------------------------------

void C3DOBJView::TrackMouse(UINT nFlags, CPoint cursorPos)
{
	//TRACE("track mouse %d,%d\n", cursorPos.x, cursorPos.y);

	bool bHaveCapture = (this == GetCapture());

	bool bRender = false;

	if ((MK_RBUTTON & nFlags) && bHaveCapture) {

		CRect clientRect;

		GetClientRect(clientRect);

		GLfloat fCX = (GLfloat)clientRect.Width();
		GLfloat fCY = (GLfloat)clientRect.Height();

		GLfloat fScale = -m_zButtonDownDistance;

		GLfloat xDist = (((GLfloat)(cursorPos.x - m_ButtonDownPos.x)) * fScale) / fCX;
		GLfloat zDist = (((GLfloat)(cursorPos.y - m_ButtonDownPos.y)) * fScale) / fCY;

		GLfloat yAngle = (m_yAngle * 3.14159f) / 180.0f;

		GLfloat xDelta = (xDist * cosf(yAngle)) - (zDist * sinf(yAngle));
		GLfloat zDelta = (xDist * sinf(yAngle)) + (zDist * cosf(yAngle));

		m_LookAtPos = BPT::VECTOR3(
			m_ButtonDownLookAtPos.x() + xDelta,
			m_ButtonDownLookAtPos.y(),
			m_ButtonDownLookAtPos.z() + zDelta
		);

		bRender = true;

	}
	else if (((MK_LBUTTON | MK_MBUTTON) & nFlags) && bHaveCapture) {

		CRect clientRect;

		GetClientRect(clientRect);

		GLfloat fCX = (GLfloat)clientRect.Width();
		GLfloat fCY = (GLfloat)clientRect.Height();

		if ((MK_SHIFT | MK_MBUTTON) & nFlags) {

			const GLfloat fScale = 8.0f;

			GLfloat yDist = (((GLfloat)(cursorPos.y - m_ButtonDownPos.y)) * fScale) / fCY;

			if (ChangeZDistance(m_zButtonDownDistance + (yDist * m_zButtonDownDistance))) {

				bRender = true;

			}

		}
		else {

			const GLfloat fScale = 180.0f;

			m_yAngle = m_yButtonDownAngle + (((GLfloat)(cursorPos.x - m_ButtonDownPos.x)) * fScale) / fCX;
			m_xAngle = m_xButtonDownAngle + (((GLfloat)(cursorPos.y - m_ButtonDownPos.y)) * fScale) / fCY;

			bRender = true;

		}

	}

	if (bRender) {

		RenderScene();

	}

}

// ----------------------------------------------------------------------------

void C3DOBJView::RenderGridLines()
{

#if 1

	int lineCount = 66;

	float length = m_VisibleRegion.Length() * 2.0f;
	float half = length * 0.5f;
	float delta = length / (float)(lineCount - 1);

	float fz = -half;
	float yy = 0.0f;

	std::vector<BPT::vec3> lineVerts;
	std::vector<BPT::vec3> lineColors;

	lineVerts.reserve(lineCount * 2);
	lineColors.reserve(lineCount * 2);

	BPT::vec3 lineColor = BPT::vec3(0.0f, 0.25f, 1.0f);

	for (int zline = 0; zline < lineCount; zline++) {

		lineVerts.push_back(BPT::vec3(-half, yy, fz));
		lineVerts.push_back(BPT::vec3(+half, yy, fz));

		lineColors.push_back(lineColor);
		lineColors.push_back(lineColor);

		fz += delta;

	}

	float fx = -half;

	for (int xline = 0; xline < lineCount; xline++) {

		lineVerts.push_back(BPT::vec3(fx, yy, -half));
		lineVerts.push_back(BPT::vec3(fx, yy, +half));

		lineColors.push_back(lineColor);
		lineColors.push_back(lineColor);

		fx += delta;

	}

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, lineVerts.data()); // (const void*)&(*s_pCached->fibLattice.lattice.begin()));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, lineColors.data());
	glEnableVertexAttribArray(1);

	glDrawArrays(GL_LINES, 0, lineVerts.size());

#else


	// Render the grid
	// ------------------------------------------------------------------

	//glEnable(GL_LINE_SMOOTH);
	glBegin(GL_LINES);

	{
		glColor3f(0.0f, 0.25f, 1.0f);

		int lineCount = 66;

		float length = m_VisibleRegion.Length() * 2.0f;
		float half = length * 0.5f;
		float delta = length / (float)(lineCount - 1);

		float fz = -half;
		float yy = 0.0f;

		for (int zline = 0; zline < lineCount; zline++) {

			glVertex3f(-half, yy, fz);
			glVertex3f(+half, yy, fz);

			fz += delta;

		}

		float fx = -half;

		for (int xline = 0; xline < lineCount; xline++) {

			glVertex3f(fx, yy, -half);
			glVertex3f(fx, yy, +half);

			fx += delta;

		}

	}

	glEnd();

#endif

}

// ----------------------------------------------------------------------------

namespace BPT
{
	struct RootEntity : public SimpleEntity
	{
	};

	struct IRendererEntity : public SimpleEntity
	{
		virtual void Draw() = 0;
	};

	struct DisplayListEntity : public IRendererEntity
	{
		GLuint displayList;

		DisplayListEntity() : displayList(0)
		{
		}

		virtual void Draw()
		{

#if 1

			// TODO

#else

			glPushMatrix();

			mat4 m;
			GetConcatenatedMatrix(m); // TODO - preprocess

			glMultMatrixf(&m[0][0]);
			glTranslatef(position_.position.x, position_.position.y, position_.position.z);

			glCallList(displayList);

			glPopMatrix();

#endif

		}

		void Setup(GLuint dl)
		{
			displayList = dl;
		}
	};

	struct PointCloudEntity : public DisplayListEntity
	{
		float pointSize;

		PointCloudEntity() : pointSize(8.0f)
		{
		}

		virtual void Draw()
		{
			//glPointSize(pointSize);

			DisplayListEntity::Draw();
		}

		void Setup(GLuint dl, float ps)
		{
			DisplayListEntity::Setup(dl);
			pointSize = ps;
		}
	};

	struct MirrorCloudEntity : public DisplayListEntity
	{
		float pointSize;

		MirrorCloudEntity() : pointSize(8.0f)
		{
		}

		virtual void Draw()
		{

#if 1

			// TODO

#else

			mat4 m;
			GetConcatenatedMatrix(m); // TODO - preprocess

			glMultMatrixf(&m[0][0]);
			glTranslatef(position_.position.x, position_.position.y, position_.position.z);

			float n = 1.0f, xh = n, yh = n, zh = n;

			for (float z = -zh; z < zh; z += 1.0f)
			{
				float zScale = signbit(z) ? -1.0f : +1.0f;

				for (float x = -xh; x < xh; x += 1.0f)
				{
					float xScale = signbit(x) ? -1.0f : +1.0f;

					for (float y = -yh; y < yh; y += 1.0f)
					{
						float yScale = signbit(y) ? -1.0f : +1.0f;

						glPushMatrix();
						glScalef(xScale, yScale, zScale);
						//DisplayListEntity::Draw();
						glCallList(displayList);
						glPopMatrix();

					}
				}
			}

#endif

		}

		void Setup(GLuint dl, float ps)
		{
			DisplayListEntity::Setup(dl);
			pointSize = ps;
		}
	};

	// ---------------------------------------------------------------------------

	struct SimpleEntityContainer
	{
		SimpleEntity_Ref root;

		std::list< IRendererEntity * > renderables;

		SimpleEntityContainer()
		{
			root = EntityManager::Create<RootEntity>();
		}

		void Add(GLuint displayList,vec3 pos,float pointSize)
		{
			auto p = new MirrorCloudEntity;// PointCloudEntity;
			p->Setup(displayList, pointSize);
			p->position_.position = pos;

			auto r = SimpleEntity_Ref(p);
			root->Attach(r);

			renderables.push_back(p);
		}

		void EnterFrame()
		{
			root->EnterFrame(); // :-)
		}

		void Render()
		{
			for (auto i : renderables) i->Draw();
		}

	};

	SimpleEntityContainer hackContainer;

} // namespace BPT

// ----------------------------------------------------------------------------
// 
//
//	TGLRenderPointCloud()
//

template< typename V, typename C > void
TGLRenderPointCloud(V first, V last,C colorsIt ) //, BPT::VECTOR3 &o )
{
	//GLfloat scale = 256.0f;
	//glPushMatrix();
	//glScalef(scale, scale, scale);
	//glTranslatef(o.x(), o.y(), o.z());
	//glPointSize(4.0f);
	//glEnable(GL_POINT_SMOOTH);

	//glEnableClientState(GL_COLOR_ARRAY);
	//glEnableClientState(GL_VERTEX_ARRAY);

	glColorPointer(3, GL_FLOAT, 0, reinterpret_cast<GLvoid*>(&(*colorsIt)));
	glVertexPointer(3, GL_FLOAT, 0, reinterpret_cast<GLvoid*>(&(*first)));

	glDrawArrays(GL_POINTS, 0, std::distance(first, last));
//	glDrawArrays(GL_LINES, 0, std::distance(first, last)/2);
//	glDrawArrays(GL_QUADS, 0, std::distance(first, last));
//	glDrawArrays(GL_QUAD_STRIP, 0, std::distance(first, last));
	//glPopMatrix();
}

// ----------------------------------------------------------------------------

constexpr char kVS[] = 
R"(
precision highp float;
uniform mat4 mat;
attribute vec3 vPosition;
attribute vec4 vColor;
varying vec4 color;
void main()
{
    gl_Position = mat * vec4(vPosition,1.0);
	gl_PointSize = 4.0;
	color = vColor;
})";

constexpr char kFS[] = 
R"(
precision highp float;
varying vec4 color;
void main()
{
	//vec2 a = (gl_PointCoord.xy-0.5)*2.0;
	//float d = dot(a,a);
    //d = 1.0-fract(d);
    gl_FragColor = color;//vec4(vec3(d),1.0*step(0.5,d));//color; //vec4(d*(color.a),color.a); // - color*d; //vec4(vec3(,color.a);//vec4( color * ((a.x*a.y)) );//dot(gl_PointCoord,);
})";

// ----------------------------------------------------------------------------

namespace angle_shader_utils {

	// https://github.com/google/angle/blob/main/util/shader_utils.cpp

	GLuint CompileShader(GLenum type, const char* source)
	{
		GLuint shader = glCreateShader(type);

		const char* sourceArray[1] = { source };
		glShaderSource(shader, 1, sourceArray, nullptr);
		glCompileShader(shader);

		GLint compileResult;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compileResult);

		if (compileResult == 0)
		{
			GLint infoLogLength;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

			// Info log length includes the null terminator, so 1 means that the info log is an empty
			// string.
			if (infoLogLength > 1)
			{
				std::vector<GLchar> infoLog(infoLogLength);
				glGetShaderInfoLog(shader, static_cast<GLsizei>(infoLog.size()), nullptr, &infoLog[0]);
				std::cerr << "shader compilation failed: " << &infoLog[0];
			}
			else
			{
				std::cerr << "shader compilation failed. <Empty log message>";
			}

			std::cerr << std::endl;

			glDeleteShader(shader);
			shader = 0;
		}

		return shader;
	}

	GLuint CheckLinkStatusAndReturnProgram(GLuint program, bool outputErrorMessages)
	{
		if (glGetError() != GL_NO_ERROR)
			return 0;

		GLint linkStatus;
		glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
		if (linkStatus == 0)
		{
			if (outputErrorMessages)
			{
				GLint infoLogLength;
				glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

				// Info log length includes the null terminator, so 1 means that the info log is an
				// empty string.
				if (infoLogLength > 1)
				{
					std::vector<GLchar> infoLog(infoLogLength);
					glGetProgramInfoLog(program, static_cast<GLsizei>(infoLog.size()), nullptr,
						&infoLog[0]);

					std::cerr << "program link failed: " << &infoLog[0];
				}
				else
				{
					std::cerr << "program link failed. <Empty log message>";
				}
			}

			glDeleteProgram(program);
			return 0;
		}

		return program;
	}

	GLuint CompileProgram( const char* vsSource, const char* fsSource )
	{
		GLuint vs = CompileShader(GL_VERTEX_SHADER, vsSource);
		GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fsSource);

		if (vs == 0 || fs == 0)
		{
			glDeleteShader(fs);
			glDeleteShader(vs);
			return 0;
		}

		GLuint program = glCreateProgram();

		glAttachShader(program, vs);
		glDeleteShader(vs);

		glAttachShader(program, fs);
		glDeleteShader(fs);

		glLinkProgram(program);

		return CheckLinkStatusAndReturnProgram(program, true);
	}


} // namespace angle_shader_utils

// ----------------------------------------------------------------------------

void C3DOBJView::RenderPointClouds()
{

#if 1

	CDitherStudioDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if ((nullptr != pDoc->m_pLastFibSphereViz))
	{

#if 1

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, pDoc->m_pLastFibSphereViz->colors.data());
		glEnableVertexAttribArray(0);

		if (m_bShowUniqueColors)
		{
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, pDoc->m_pLastFibSphereViz->fibLattice.lattice.data());
		}
		else
		{
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, pDoc->m_pLastFibSphereViz->colors.data());
		}

		glEnableVertexAttribArray(1);

		GLint matLoc = glGetUniformLocation(shaderProgram, "mat");

		float n = 1.0f, xh = n, yh = n, zh = n;

		float scale = 1.0f;

		for (float z = -zh; z < zh; z += 1.0f)
		{
			float zScale = signbit(z) ? -1.0f : +1.0f;

			for (float x = -xh; x < xh; x += 1.0f)
			{
				float xScale = signbit(x) ? -1.0f : +1.0f;

				for (float y = -yh; y < yh; y += 1.0f)
				{
					float yScale = signbit(y) ? -1.0f : +1.0f;

					BPT::mat4 m = BPT::glm::scale(m_viewMat, BPT::vec3(xScale * scale, yScale * scale, zScale * scale));
					glUniformMatrix4fv(matLoc, 1, GL_FALSE, &m[0][0]);

					glDrawArrays(GL_POINTS, 0, pDoc->m_pLastFibSphereViz->fibLattice.lattice.size());
				}
			}
		}

#else

		GLuint vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, s_pCached->fibLattice.lattice.size() * sizeof(BPT::vec3), (const void *)&(*s_pCached->fibLattice.lattice.begin()), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

#endif

	}

#else

	CDitherStudioDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if ((nullptr != pDoc->m_pLastFibSphereViz))
	{
		// --------------------------------------------------------------------------

		static CDitherStudioDoc::fibsphere_viz_t* s_pCached = nullptr;
		static GLuint displayList = 0;
		
		if (m_bRebuildRequested || (s_pCached != pDoc->m_pLastFibSphereViz))
		{
			m_bRebuildRequested = false;

			s_pCached = pDoc->m_pLastFibSphereViz;

#if 1
			if (displayList) glDeleteLists(displayList, 1);
#endif

			// -------------------------------------------------------------------------

			glEnableClientState(GL_COLOR_ARRAY);
			glEnableClientState(GL_VERTEX_ARRAY);

			GLfloat scale = 256.0f;
			glPushMatrix();
			glScalef(scale, scale, scale);
			glEnable(GL_POINT_SMOOTH);

			// -------------------------------------------------------------------------

			displayList = glGenLists(1);
			glNewList(displayList, GL_COMPILE);

			CDitherStudioDoc::fibsphere_viz_t& viz = *pDoc->m_pLastFibSphereViz; // shorten the name 

			if (m_bShowUniqueColors)
			{
				TGLRenderPointCloud(viz.colors.begin(), viz.colors.end(), viz.fibLattice.lattice.begin()); // unique colors
			}
			else
			{
				TGLRenderPointCloud(viz.colors.begin(), viz.colors.end(), viz.colors.begin()); // , BPT::VECTOR3(x, y, z)); // color for both the vert and color
			}

			//
			//TGLRenderPointCloud(viz.fibLattice.lattice.begin(), viz.fibLattice.lattice.end(), viz.colors.begin()); // surface of a sphere

			glEndList();
			glPopMatrix();

#if 0
			for (int i = 0; i < 1; i++)
			{
				BPT::hackContainer.Add(displayList, BPT::vec3(-m_LookAtPos.x(), -m_LookAtPos.y(), -m_LookAtPos.z()), 8.0f);
			}
#endif

		}

		glPointSize(8.0f);// MINN(32.0f, exp(m_zDistance)));

		// --------------------------------------------------------------------------

#if 1

//#define PENDING_MOUSE_MOVEMENT_CULLS_OBJECTS
#if defined(PENDING_MOUSE_MOVEMENT_CULLS_OBJECTS)
		// TODO - find a better way to use this for now I am disabling it
		LARGE_INTEGER inTime, currTime, took, frequency;
		QueryPerformanceFrequency(&frequency);
		QueryPerformanceCounter(&inTime);
#endif

		bool bMouseMovePending = false;

		int counter = 0;

		MSG msg;

		// thanks to the flipflop 'logic' the following code isn't general enough to make an 3d array of objects :/
		// really need a scene graph or some kind, also think about how to handle things like this (simple transforms for instances)

		float n = 1.0f, xh = n, yh = n, zh = n;

		for (float z = -zh; z < zh; z += 1.0f)
		{
			float zScale = signbit(z) ? -1.0f : +1.0f;

			for (float x = -xh; x < xh; x += 1.0f)
			{
				float xScale = signbit(x) ? -1.0f : +1.0f;

				for (float y = -yh; y < yh; y += 1.0f)
				{
					float yScale = signbit(y) ? -1.0f : +1.0f;

#if defined(PENDING_MOUSE_MOVEMENT_CULLS_OBJECTS)
					// -----------------------------------------------------------------------
					// TODO - find a better way to use this for now I am disabling it
					if (bMouseMovePending && (0 == (++counter & 1))) {

						// should draw bounding volume instead of nothing...

						QueryPerformanceCounter(&currTime);

						took.QuadPart = currTime.QuadPart - inTime.QuadPart;
						took.QuadPart *= 1000000;
						took.QuadPart /= frequency.QuadPart;

						if ( 500000 > took.QuadPart ) continue;

					}

					// -----------------------------------------------------------------------
#endif

					glPushMatrix();

					// (really need to look a mirror mod, or some other periodic alignment for all the combinations)
					// I'm sure there is a much better way to do this as well

					if (m_bFlipFlopScales)
					{
						glScalef(xScale, yScale, zScale);
					}
					else 
					{
						glTranslatef(x, y, z);
					}

					glCallList(displayList);
					glPopMatrix();

					// abort rendering if there is a mouse message

					if ( !bMouseMovePending && (PeekMessage(&msg, this->GetSafeHwnd(), WM_MOUSEFIRST, WM_MOUSELAST, PM_NOREMOVE)) )
					{
						Invalidate(FALSE);
						bMouseMovePending = true;
					}
				}
			}

#if 0
			if (PeekMessage(&msg, this->GetSafeHwnd(), WM_PAINT, WM_PAINT, PM_NOREMOVE))
			{
				break;
			}
#endif

		}

#endif

	}

#endif

}

// ----------------------------------------------------------------------------

void C3DOBJView::RenderScene()
{

#if 1

	if (!m_GLContext.MakeCurrent()) return;

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);

	BPT::mat4 view = BPT::glm::translate(BPT::mat4(1.0f), BPT::vec3(0.0f, 0.0f, m_zDistance));
	view = BPT::glm::rotate(view, BPT::glm::radians(m_xAngle), BPT::vec3(1.0f, 0.0f, 0.0f));
	view = BPT::glm::rotate(view, BPT::glm::radians(m_yAngle), BPT::vec3(0.0f, 1.0f, 0.0f));
	view = BPT::glm::translate(view, BPT::vec3(m_LookAtPos.x(), m_LookAtPos.y(), m_LookAtPos.z()));
	m_viewMat = m_projectionMat * view;

	if (0 == shaderProgram)
	{
		shaderProgram = angle_shader_utils::CompileProgram(kVS, kFS);
	}

	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "mat"), 1, GL_FALSE, &m_viewMat[0][0]);

	glUseProgram(shaderProgram);

	RenderGridLines();

	RenderPointClouds();

	m_GLContext.SwapBuffers();

#else

	if (!m_GLContext.MakeCurrent()) return;

	//TRACE("RenderScene\n");

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// I think I read about a trick to avoid clearing every frame?
	// this led to an interesting looking bug that reminded me of
	// zbrush. imagine only applying an objects when the button is down
	// or in 'write' mode. simply moving the mouse/object orients the
	// next draw position for the brush.  or that is how I imagine it works

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	glClear(GL_DEPTH_BUFFER_BIT);

//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	glClear(GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);

	glFrontFace(GL_CCW);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

#if 1
	// is there a way to reverse z every other swap? (is that what needs to happen?)
#endif

	glTranslatef(0.0f, 0.0f, m_zDistance);
	glRotatef(m_xAngle, 1.0f, 0.0f, 0.0f);
	glRotatef(m_yAngle, 0.0f, 1.0f, 0.0f);
	glTranslatef(m_LookAtPos.x(), m_LookAtPos.y(), m_LookAtPos.z());

	RenderGridLines();

	RenderPointClouds();

#if 0
	BPT::hackContainer.EnterFrame();
	BPT::hackContainer.Render();
#endif

	m_GLContext.SwapBuffers();

#endif

}



void C3DOBJView::OnReset3dView()
{
	ResetView();
	RenderScene();
}


void C3DOBJView::OnUpdateReset3dView(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void C3DOBJView::On3dFlipFlop()
{
	m_bFlipFlopScales = !m_bFlipFlopScales;
	RenderScene();
}


void C3DOBJView::OnUpdate3dFlipFlop(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

// 

void C3DOBJView::RequestRebuild()
{
	m_bRebuildRequested = true;
}

void C3DOBJView::On3dShowUnique()
{
	m_bShowUniqueColors = !m_bShowUniqueColors;
	RequestRebuild();
	RenderScene();
}


void C3DOBJView::OnUpdate3dShowUnique(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_bShowUniqueColors ? 1 : 0);
	// TODO: Add your command update UI handler code here
}

