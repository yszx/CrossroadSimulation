#if !defined(AFX_MOGEOEVENT_H__9E825CF7_53D9_4F1F_B8AF_23DB9A2B989E__INCLUDED_)
#define AFX_MOGEOEVENT_H__9E825CF7_53D9_4F1F_B8AF_23DB9A2B989E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.

/////////////////////////////////////////////////////////////////////////////
// CMoGeoEvent wrapper class

class CMoGeoEvent : public COleDispatchDriver
{
public:
	CMoGeoEvent() {}		// Calls COleDispatchDriver default constructor
	CMoGeoEvent(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CMoGeoEvent(const CMoGeoEvent& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	CString GetTag();
	void SetTag(LPCTSTR);
	long GetSymbolIndex();
	void SetSymbolIndex(long);
	double GetX();
	void SetX(double);
	double GetY();
	void SetY(double);
	long GetIndex();
	void SetIndex(long);
	LPDISPATCH GetShape();
	void SetShape(LPDISPATCH);

// Operations
public:
	void MoveTo(double X, double Y);
	void Move(double X, double Y);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MOGEOEVENT_H__9E825CF7_53D9_4F1F_B8AF_23DB9A2B989E__INCLUDED_)