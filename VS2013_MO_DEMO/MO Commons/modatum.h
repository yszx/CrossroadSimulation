#if !defined(AFX_MODATUM_H__5E54F939_4B48_416A_8833_82B60AB34D7A__INCLUDED_)
#define AFX_MODATUM_H__5E54F939_4B48_416A_8833_82B60AB34D7A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


// Dispatch interfaces referenced by this interface
class CMoSpheroid;

/////////////////////////////////////////////////////////////////////////////
// CMoDatum wrapper class

class CMoDatum : public COleDispatchDriver
{
public:
	CMoDatum() {}		// Calls COleDispatchDriver default constructor
	CMoDatum(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CMoDatum(const CMoDatum& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	long GetType();
	void SetType(long);
	CString GetName();
	void SetName(LPCTSTR);
	CMoSpheroid GetSpheroid();
	void SetSpheroid(LPDISPATCH);

// Operations
public:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODATUM_H__5E54F939_4B48_416A_8833_82B60AB34D7A__INCLUDED_)
