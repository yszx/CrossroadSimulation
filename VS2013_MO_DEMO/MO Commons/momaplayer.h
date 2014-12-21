#if !defined(AFX_MOMAPLAYER_H__247737DF_E0D7_4638_AA92_2D68268EE871__INCLUDED_)
#define AFX_MOMAPLAYER_H__247737DF_E0D7_4638_AA92_2D68268EE871__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


// Dispatch interfaces referenced by this interface
class CMoRectangle;
class CMoRecordset;
class CMoSymbol;
class CMoGeoDataset;
class CMoStrings;

/////////////////////////////////////////////////////////////////////////////
// CMoMapLayer wrapper class

class CMoMapLayer : public COleDispatchDriver
{
public:
	CMoMapLayer() {}		// Calls COleDispatchDriver default constructor
	CMoMapLayer(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CMoMapLayer(const CMoMapLayer& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	CString GetName();
	void SetName(LPCTSTR);
	CMoRectangle GetExtent();
	void SetExtent(LPDISPATCH);
	BOOL GetVisible();
	void SetVisible(BOOL);
	CMoRecordset GetRecords();
	void SetRecords(LPDISPATCH);
	CMoSymbol GetSymbol();
	void SetSymbol(LPDISPATCH);
	CMoGeoDataset GetGeoDataset();
	void SetGeoDataset(LPDISPATCH);
	long GetLayerType();
	void SetLayerType(long);
	CMoRectangle GetAreaOfInterest();
	void SetAreaOfInterest(LPDISPATCH);
	LPDISPATCH GetRenderer();
	void SetRenderer(LPDISPATCH);
	CString GetTag();
	void SetTag(LPCTSTR);
	long GetShapeType();
	void SetShapeType(long);
	BOOL GetValid();
	void SetValid(BOOL);
	BOOL GetIndexed();
	void SetIndexed(BOOL);
	VARIANT GetCoordinateSystem();
	void SetCoordinateSystem(const VARIANT&);
	VARIANT GetGeographicTransformation();
	void SetGeographicTransformation(const VARIANT&);
	double GetDensificationTolerance();
	void SetDensificationTolerance(double);
	VARIANT GetFilterShape();
	void SetFilterShape(const VARIANT&);
	long GetFilterOperator();
	void SetFilterOperator(long);
	CString GetFilterExpression();
	void SetFilterExpression(LPCTSTR);
	CMoStrings GetFilterFields();
	void SetFilterFields(LPDISPATCH);
	long GetFilterOrder();
	void SetFilterOrder(long);
	CMoStrings GetFilterTables();
	void SetFilterTables(LPDISPATCH);

// Operations
public:
	CMoRecordset SearchExpression(LPCTSTR expression);
	BOOL AddRelate(LPCTSTR toField, LPDISPATCH Table, LPCTSTR fromField, const VARIANT& CheckFields);
	void RemoveRelates();
	CMoRecordset SearchByDistance(LPDISPATCH shape, double distance, LPCTSTR expression);
	CMoRecordset SearchShape(LPDISPATCH shape, long searchMethod, LPCTSTR expression);
	BOOL BuildIndex(BOOL force);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MOMAPLAYER_H__247737DF_E0D7_4638_AA92_2D68268EE871__INCLUDED_)
