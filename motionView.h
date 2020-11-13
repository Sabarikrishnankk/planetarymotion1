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

// motionView.h : interface of the CmotionView class
//

#pragma once


class CmotionView : public CView
{
	CWinThread* newThread;
	CWinThread* secThread;
protected: // create from serialization only
	CmotionView() noexcept;
	DECLARE_DYNCREATE(CmotionView)

// Attributes
public:
	
	double winCenterX, winCenterY;
	CmotionDoc* GetDocument() const;

// Operations
public:
	int Orbit1;
	int Orbit2;
	int Sun;
	int i, j, dx, ecc;
	LPPOINT lpPoints;

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CmotionView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	afx_msg void OnButton4();
	static unsigned int StartThread(LPVOID param);
	static unsigned int NextThread(LPVOID parm);
};

#ifndef _DEBUG  // debug version in motionView.cpp
inline CmotionDoc* CmotionView::GetDocument() const
   { return reinterpret_cast<CmotionDoc*>(m_pDocument); }
#endif

