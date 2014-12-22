// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


#include "stdafx.h"
#include "map.h"

// Dispatch interfaces referenced by this interface
#include "MoLayers.h"
#include "MoRectangle.h"
#include "motrackinglayer.h"
#include "MoPoint.h"
#include "moellipse.h"
#include "MoLine.h"
#include "mopolygon.h"
#include "mogeodatasets.h"
#include "MoDataConnection.h"
#include "mogeodataset.h"
#include "momaplayer.h"
#include "morecordset.h"
#include "MoFields.h"
#include "MoField.h"
#include "mopoints.h"
#include "MoSymbol.h"
#include "movaluemaprenderer.h"
#include "MoStrings.h"
#include "MoStatistics.h"
#include "MoClassBreaksRenderer.h"
#include "MoDotDensityRenderer.h"
#include "MoLabelRenderer.h"
#include "MoGeoEvent.h"
#include "MoImageLayer.h"
#include "motable.h"
#include "MoTextSymbol.h"
#include "MoTableDesc.h"
#include "MoAddressLocation.h"
#include "MoPlaceLocator.h"
#include "moparts.h"
#include "MoProjection.h"
#include "MoProjCoordSys.h"
#include "mogeocoordsys.h"
#include "MoUnit.h"
#include "modatum.h"
#include "MoSpheroid.h"
#include "MoPrimeMeridian.h"
#include "mogeotransformation.h"
#include "mostandardizer.h"
#include "MoGeocoder.h"
#include "MoZRenderer.h"
#include "MoGroupRenderer.h"
#include "MoChartRenderer.h"
#include "MoLabelPlacer.h"
#include "moeventrenderer.h"

/////////////////////////////////////////////////////////////////////////////
// CMap1

IMPLEMENT_DYNCREATE(CMap1, CWnd)

/////////////////////////////////////////////////////////////////////////////
// CMap1 properties

CMoLayers CMap1::GetLayers()
{
	LPDISPATCH pDispatch;
	GetProperty(0x4, VT_DISPATCH, (void*)&pDispatch);
	return CMoLayers(pDispatch);
}

void CMap1::SetLayers(LPDISPATCH propVal)
{
	SetProperty(0x4, VT_DISPATCH, propVal);
}

CMoRectangle CMap1::GetExtent()
{
	LPDISPATCH pDispatch;
	GetProperty(0x5, VT_DISPATCH, (void*)&pDispatch);
	return CMoRectangle(pDispatch);
}

void CMap1::SetExtent(LPDISPATCH propVal)
{
	SetProperty(0x5, VT_DISPATCH, propVal);
}

CMoRectangle CMap1::GetFullExtent()
{
	LPDISPATCH pDispatch;
	GetProperty(0x6, VT_DISPATCH, (void*)&pDispatch);
	return CMoRectangle(pDispatch);
}

void CMap1::SetFullExtent(LPDISPATCH propVal)
{
	SetProperty(0x6, VT_DISPATCH, propVal);
}

OLE_COLOR CMap1::GetBackColor()
{
	OLE_COLOR result;
	GetProperty(DISPID_BACKCOLOR, VT_I4, (void*)&result);
	return result;
}

void CMap1::SetBackColor(OLE_COLOR propVal)
{
	SetProperty(DISPID_BACKCOLOR, VT_I4, propVal);
}

short CMap1::GetBorderStyle()
{
	short result;
	GetProperty(DISPID_BORDERSTYLE, VT_I2, (void*)&result);
	return result;
}

void CMap1::SetBorderStyle(short propVal)
{
	SetProperty(DISPID_BORDERSTYLE, VT_I2, propVal);
}

BOOL CMap1::GetScrollBars()
{
	BOOL result;
	GetProperty(0x2, VT_BOOL, (void*)&result);
	return result;
}

void CMap1::SetScrollBars(BOOL propVal)
{
	SetProperty(0x2, VT_BOOL, propVal);
}

long CMap1::GetCancelAction()
{
	long result;
	GetProperty(0x8, VT_I4, (void*)&result);
	return result;
}

void CMap1::SetCancelAction(long propVal)
{
	SetProperty(0x8, VT_I4, propVal);
}

CMoTrackingLayer CMap1::GetTrackingLayer()
{
	LPDISPATCH pDispatch;
	GetProperty(0x9, VT_DISPATCH, (void*)&pDispatch);
	return CMoTrackingLayer(pDispatch);
}

void CMap1::SetTrackingLayer(LPDISPATCH propVal)
{
	SetProperty(0x9, VT_DISPATCH, propVal);
}

long CMap1::GetRefreshCount()
{
	long result;
	GetProperty(0xa, VT_I4, (void*)&result);
	return result;
}

void CMap1::SetRefreshCount(long propVal)
{
	SetProperty(0xa, VT_I4, propVal);
}

BOOL CMap1::GetEnabled()
{
	BOOL result;
	GetProperty(DISPID_ENABLED, VT_BOOL, (void*)&result);
	return result;
}

void CMap1::SetEnabled(BOOL propVal)
{
	SetProperty(DISPID_ENABLED, VT_BOOL, propVal);
}

OLE_HANDLE CMap1::GetHWnd()
{
	OLE_HANDLE result;
	GetProperty(DISPID_HWND, VT_I4, (void*)&result);
	return result;
}

void CMap1::SetHWnd(OLE_HANDLE propVal)
{
	SetProperty(DISPID_HWND, VT_I4, propVal);
}

short CMap1::GetAppearance()
{
	short result;
	GetProperty(DISPID_APPEARANCE, VT_I4, (void*)&result);
	return result;
}

void CMap1::SetAppearance(short propVal)
{
	SetProperty(DISPID_APPEARANCE, VT_I4, propVal);
}

double CMap1::GetMinWidth()
{
	double result;
	GetProperty(0x1, VT_R8, (void*)&result);
	return result;
}

void CMap1::SetMinWidth(double propVal)
{
	SetProperty(0x1, VT_R8, propVal);
}

long CMap1::GetMousePointer()
{
	long result;
	GetProperty(0xb, VT_I4, (void*)&result);
	return result;
}

void CMap1::SetMousePointer(long propVal)
{
	SetProperty(0xb, VT_I4, propVal);
}

BOOL CMap1::GetFullRedrawOnPan()
{
	BOOL result;
	GetProperty(0x3, VT_BOOL, (void*)&result);
	return result;
}

void CMap1::SetFullRedrawOnPan(BOOL propVal)
{
	SetProperty(0x3, VT_BOOL, propVal);
}

VARIANT CMap1::GetCoordinateSystem()
{
	VARIANT result;
	GetProperty(0xc, VT_VARIANT, (void*)&result);
	return result;
}

void CMap1::SetCoordinateSystem(const VARIANT& propVal)
{
	SetProperty(0xc, VT_VARIANT, &propVal);
}

long CMap1::GetMaxFileBuffer()
{
	long result;
	GetProperty(0xd, VT_I4, (void*)&result);
	return result;
}

void CMap1::SetMaxFileBuffer(long propVal)
{
	SetProperty(0xd, VT_I4, propVal);
}

long CMap1::GetWindowMode()
{
	long result;
	GetProperty(0xe, VT_I4, (void*)&result);
	return result;
}

void CMap1::SetWindowMode(long propVal)
{
	SetProperty(0xe, VT_I4, propVal);
}

double CMap1::GetRotationAngle()
{
	double result;
	GetProperty(0xf, VT_R8, (void*)&result);
	return result;
}

void CMap1::SetRotationAngle(double propVal)
{
	SetProperty(0xf, VT_R8, propVal);
}

LPDISPATCH CMap1::GetVisibleRegion()
{
	LPDISPATCH result;
	GetProperty(0x10, VT_DISPATCH, (void*)&result);
	return result;
}

void CMap1::SetVisibleRegion(LPDISPATCH propVal)
{
	SetProperty(0x10, VT_DISPATCH, propVal);
}

long CMap1::GetGeometryEnvironment()
{
	long result;
	GetProperty(0x2b, VT_I4, (void*)&result);
	return result;
}

void CMap1::SetGeometryEnvironment(long propVal)
{
	SetProperty(0x2b, VT_I4, propVal);
}

long CMap1::GetDisplayUnit()
{
	long result;
	GetProperty(0x2c, VT_I4, (void*)&result);
	return result;
}

void CMap1::SetDisplayUnit(long propVal)
{
	SetProperty(0x2c, VT_I4, propVal);
}

long CMap1::GetTrackingLayerDrawing()
{
	long result;
	GetProperty(0x2d, VT_I4, (void*)&result);
	return result;
}

void CMap1::SetTrackingLayerDrawing(long propVal)
{
	SetProperty(0x2d, VT_I4, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// CMap1 operations

void CMap1::AboutBox()
{
	InvokeHelper(0xfffffdd8, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CMap1::Pan()
{
	InvokeHelper(0x11, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

CMoPoint CMap1::ToMapPoint(float X, float Y)
{
	LPDISPATCH pDispatch;
	static BYTE parms[] =
		VTS_R4 VTS_R4;
	InvokeHelper(0x12, DISPATCH_METHOD, VT_DISPATCH, (void*)&pDispatch, parms,
		X, Y);
	return CMoPoint(pDispatch);
}

void CMap1::FlashShape(LPDISPATCH shape, short nTimes)
{
	static BYTE parms[] =
		VTS_DISPATCH VTS_I2;
	InvokeHelper(0x13, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 shape, nTimes);
}

void CMap1::Refresh()
{
	InvokeHelper(DISPID_REFRESH, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CMap1::DrawShape(LPDISPATCH shape, LPDISPATCH Symbol)
{
	static BYTE parms[] =
		VTS_DISPATCH VTS_DISPATCH;
	InvokeHelper(0x14, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 shape, Symbol);
}

void CMap1::DrawText(LPCTSTR text, LPDISPATCH shape, LPDISPATCH Symbol)
{
	static BYTE parms[] =
		VTS_BSTR VTS_DISPATCH VTS_DISPATCH;
	InvokeHelper(0x15, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 text, shape, Symbol);
}

CMoEllipse CMap1::TrackCircle()
{
	LPDISPATCH pDispatch;
	InvokeHelper(0x16, DISPATCH_METHOD, VT_DISPATCH, (void*)&pDispatch, NULL);
	return CMoEllipse(pDispatch);
}

CMoLine CMap1::TrackLine()
{
	LPDISPATCH pDispatch;
	InvokeHelper(0x17, DISPATCH_METHOD, VT_DISPATCH, (void*)&pDispatch, NULL);
	return CMoLine(pDispatch);
}

CMoRectangle CMap1::TrackRectangle()
{
	LPDISPATCH pDispatch;
	InvokeHelper(0x18, DISPATCH_METHOD, VT_DISPATCH, (void*)&pDispatch, NULL);
	return CMoRectangle(pDispatch);
}

void CMap1::FromMapPoint(LPDISPATCH Point, float* X, float* Y)
{
	static BYTE parms[] =
		VTS_DISPATCH VTS_PR4 VTS_PR4;
	InvokeHelper(0x19, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Point, X, Y);
}

double CMap1::ToMapDistance(float distance)
{
	double result;
	static BYTE parms[] =
		VTS_R4;
	InvokeHelper(0x1a, DISPATCH_METHOD, VT_R8, (void*)&result, parms,
		distance);
	return result;
}

float CMap1::FromMapDistance(double distance)
{
	float result;
	static BYTE parms[] =
		VTS_R8;
	InvokeHelper(0x1b, DISPATCH_METHOD, VT_R4, (void*)&result, parms,
		distance);
	return result;
}

CMoPolygon CMap1::TrackPolygon()
{
	LPDISPATCH pDispatch;
	InvokeHelper(0x1c, DISPATCH_METHOD, VT_DISPATCH, (void*)&pDispatch, NULL);
	return CMoPolygon(pDispatch);
}

void CMap1::OutputMap(long hDC)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1d, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 hDC);
}

void CMap1::PrintMap(LPCTSTR docName, LPCTSTR outputFile, BOOL landscapeOrientation)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BOOL;
	InvokeHelper(0x1e, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 docName, outputFile, landscapeOrientation);
}

void CMap1::ExportMap(long exportType, LPCTSTR outputFile, double scaleFactor)
{
	static BYTE parms[] =
		VTS_I4 VTS_BSTR VTS_R8;
	InvokeHelper(0x1f, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 exportType, outputFile, scaleFactor);
}

void CMap1::CopyMap(double scaleFactor)
{
	static BYTE parms[] =
		VTS_R8;
	InvokeHelper(0x20, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 scaleFactor);
}

void CMap1::CenterAt(double X, double Y)
{
	static BYTE parms[] =
		VTS_R8 VTS_R8;
	InvokeHelper(0x21, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 X, Y);
}

void CMap1::RefreshLayer(short index, const VARIANT& rect)
{
	static BYTE parms[] =
		VTS_I2 VTS_VARIANT;
	InvokeHelper(0x22, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 index, &rect);
}

void CMap1::ExportMap2(long exportType, LPCTSTR outputFile, double scaleFactor, const VARIANT& useSourceDepth)
{
	static BYTE parms[] =
		VTS_I4 VTS_BSTR VTS_R8 VTS_VARIANT;
	InvokeHelper(0x23, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 exportType, outputFile, scaleFactor, &useSourceDepth);
}

void CMap1::OutputMap2(long hDC, long X, long Y, long Width, long Height, const VARIANT& DrawFlags)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_VARIANT;
	InvokeHelper(0x24, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 hDC, X, Y, Width, Height, &DrawFlags);
}

void CMap1::RefreshRect(LPDISPATCH rect)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x25, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 rect);
}

void CMap1::EnableTIFFLZW(LPCTSTR licenseCode)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x26, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 licenseCode);
}

void CMap1::EnableGIF(LPCTSTR licenseCode)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x27, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 licenseCode);
}

void CMap1::EnableStreetMap(LPCTSTR licenseCode)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x28, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 licenseCode);
}

void CMap1::ExportMapToJpeg(LPCTSTR outputFile, long percentQuality, BOOL isProgressive, double scaleFactor, long scaleSymbology)
{
	static BYTE parms[] =
		VTS_BSTR VTS_I4 VTS_BOOL VTS_R8 VTS_I4;
	InvokeHelper(0x29, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 outputFile, percentQuality, isProgressive, scaleFactor, scaleSymbology);
}

void CMap1::ExportMap3(long formatType, const VARIANT& formatData, double scaleFactor, long scaleSymbology, long exportDepth, long palette)
{
	static BYTE parms[] =
		VTS_I4 VTS_VARIANT VTS_R8 VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x2a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 formatType, &formatData, scaleFactor, scaleSymbology, exportDepth, palette);
}

void CMap1::OutputMap3(long hDC, long X, long Y, long Width, long Height, const VARIANT& DrawFlags, double scaleFactor, long scaleSymbology, const VARIANT& outputExtent)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_VARIANT VTS_R8 VTS_I4 VTS_VARIANT;
	InvokeHelper(0x2e, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 hDC, X, Y, Width, Height, &DrawFlags, scaleFactor, scaleSymbology, &outputExtent);
}

void CMap1::ExportMapToJpeg2(LPCTSTR outputFile, long percentQuality, BOOL isProgressive, double scaleFactor, long scaleSymbology, const VARIANT& exportExtent)
{
	static BYTE parms[] =
		VTS_BSTR VTS_I4 VTS_BOOL VTS_R8 VTS_I4 VTS_VARIANT;
	InvokeHelper(0x2f, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 outputFile, percentQuality, isProgressive, scaleFactor, scaleSymbology, &exportExtent);
}

void CMap1::ExportMap4(long formatType, const VARIANT& formatData, double scaleFactor, long scaleSymbology, long exportDepth, long palette, const VARIANT& exportExtent)
{
	static BYTE parms[] =
		VTS_I4 VTS_VARIANT VTS_R8 VTS_I4 VTS_I4 VTS_I4 VTS_VARIANT;
	InvokeHelper(0x30, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 formatType, &formatData, scaleFactor, scaleSymbology, exportDepth, palette, &exportExtent);
}