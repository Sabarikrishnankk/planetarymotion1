// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface
// (the "Fluent UI") and is provided only as referential material to supplement the
// Microsoft Foundation Classes Reference and related electronic documentation
// included with the MFC C++ library software.
// License terms to copy, use or distribute the Fluent UI are available separately.
// To learn more about our Fluent UI licensing program, please visit
// https://go.microsoft.com/fwlink/?LinkId=238214.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// motionView.cpp : implementation of the CmotionView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "motion.h"
#endif

#include "motionDoc.h"
#include "motionView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CmotionView

IMPLEMENT_DYNCREATE(CmotionView, CView)

BEGIN_MESSAGE_MAP(CmotionView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CmotionView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_BUTTON2, &CmotionView::OnButton2)
	ON_COMMAND(ID_BUTTON3, &CmotionView::OnButton3)
	ON_COMMAND(ID_BUTTON4, &CmotionView::OnButton4)
END_MESSAGE_MAP()

// CmotionView construction/destruction

CmotionView::CmotionView() noexcept
{
	Orbit1 = 250;
	Orbit2 = 150;
	Sun = 50;
	i = 0;
	j = 65;
	newThread = NULL;
	secThread = NULL;
	dx = -20;
	ecc = 15;

	// TODO: add construction code here

}

CmotionView::~CmotionView()
{
}

BOOL CmotionView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CmotionView drawing

void CmotionView::OnDraw(CDC* pDC)
{
	CRect clientRect, orbitA, orbitB, sun;
	GetClientRect(clientRect);
	winCenterX = clientRect.CenterPoint().x;
	winCenterY = clientRect.CenterPoint().y;
	CPen redPen;
	redPen.CreatePen(PS_SOLID, 5, RGB(255, 0, 0));
	CPen* pOldPen = pDC->SelectObject(&redPen);
	pDC->Ellipse(winCenterX - Orbit1, winCenterY - Orbit1, winCenterX + Orbit1, winCenterY + Orbit1);
	pDC->SelectObject(pOldPen);
	CPen greenPen;
	greenPen.CreatePen(PS_SOLID, 5, RGB(0, 255, 0));
	CPen* pOldPen2 = pDC->SelectObject(&greenPen);
	pDC->BeginPath();
	pDC->Ellipse(winCenterX - Orbit1, winCenterY - Orbit1, winCenterX + Orbit1, winCenterY + Orbit1);
	pDC->Ellipse(winCenterX - Orbit2 + ecc, winCenterY + dx - Orbit2, winCenterX + Orbit2 - ecc, winCenterY + dx + Orbit2);
	pDC->EndPath();
	pDC->Ellipse(winCenterX - Orbit2 + ecc, winCenterY + dx - Orbit2, winCenterX + Orbit2 - ecc, winCenterY + dx + Orbit2);
	pDC->SelectObject(pOldPen2);
	CRgn elps;
	elps.CreateEllipticRgn(winCenterX - Sun, winCenterY - Sun, winCenterX + Sun, winCenterY + Sun);
	CBrush brush, bplA, bplB;
	brush.CreateSolidBrush(RGB(50, 0, 50));
	pDC->FillRgn(&elps, &brush);
	CRgn plaA, plaB;
	bplA.CreateSolidBrush(RGB(70, 0, 250));
	bplB.CreateSolidBrush(RGB(100, 0, 50));
	pDC->FlattenPath();
	int nNumPts = pDC->GetPath(NULL, NULL, 0);
	if (nNumPts == 0)
		return;

	LPPOINT lpPoints = NULL;
	LPBYTE lpTypes = NULL;
	try
	{
		lpPoints = new POINT[nNumPts];
		lpTypes = new BYTE[nNumPts];
	}
	catch (CException* pe)
	{
		delete[] lpPoints;
		lpPoints = NULL;
		delete[] lpTypes;
		lpTypes = NULL;
		pe->Delete();
	}
	if (lpPoints == NULL || lpTypes == NULL)
		return;

	// Now that we have the memory, really get the path data.
	pDC->FlattenPath();
	nNumPts = pDC->GetPath(lpPoints, lpTypes, nNumPts);
	if (nNumPts != -1)
	{
		plaA.CreateEllipticRgn(lpPoints[i].x - 15, lpPoints[i].y - 10, lpPoints[i].x + 10, lpPoints[i].y + 10);
		plaB.CreateEllipticRgn(lpPoints[j].x - 10, lpPoints[j].y - 10, lpPoints[j].x + 10, lpPoints[j].y + 10);
	}
	pDC->FillRgn(&plaA, &bplA);
	pDC->FillRgn(&plaB, &bplB);
	delete[] lpPoints;
	delete[] lpTypes;
	CmotionDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CmotionView printing


void CmotionView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CmotionView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CmotionView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CmotionView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CmotionView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CmotionView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CmotionView diagnostics

#ifdef _DEBUG
void CmotionView::AssertValid() const
{
	CView::AssertValid();
}

void CmotionView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CmotionDoc* CmotionView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CmotionDoc)));
	return (CmotionDoc*)m_pDocument;
}
#endif //_DEBUG


// CmotionView message handlers


void CmotionView::OnButton2()
{
	newThread = AfxBeginThread(CmotionView::StartThread, this);
	secThread = AfxBeginThread(CmotionView::NextThread, this);

	// TODO: Add your command handler code here
}


void CmotionView::OnButton3()
{
	newThread->SuspendThread();
	secThread->SuspendThread();
	// TODO: Add your command handler code here
}


void CmotionView::OnButton4()
{
	newThread->ResumeThread();
	secThread->ResumeThread();

	// TODO: Add your command handler code here
}


unsigned int CmotionView::StartThread(LPVOID param)
{

	CmotionView* pView = (CmotionView*) param;
	while (1)
	{
		pView->i++;
		if (pView->i >= 65)
		{
			pView->i = 0;
		}
		pView->Invalidate();
		Sleep(100);

	}

	// TODO: Add your implementation code here.
	return 0;
}


unsigned int CmotionView::NextThread(LPVOID parm)
{
	CmotionView* pView = (CmotionView*)parm;
	while (1)
	{
		pView->j++;
		if (pView->j >= 130)
		{
			pView->j = 65;
		}
		pView->Invalidate();
		Sleep(75);
	}
	// TODO: Add your implementation code here.
	return 0;
}
