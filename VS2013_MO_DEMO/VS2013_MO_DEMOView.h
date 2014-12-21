
// VS2013_MO_DEMOView.h : interface of the CVS2013_MO_DEMOView class
//

#pragma once

#include "resource.h"
#include "MO Commons/map.h"
#include "ATRNS_SIMU.h"


class CVS2013_MO_DEMOView : public CFormView
{
	//  [12/17/2014 guojianzhu]
	// 1. 添加设置地图符号函数
	// 2. 添加几何对象集合，及相关函数
	// 3. 添加颜色等属性数组
	//  [12/20/2014 guojianzhu]
	// 4. 自动生成道路数据
public:
	CMoDataConnection pMoCon;
	CMoTableDesc desc;
	CMoMapLayer layer;
	CStringArray m_name;
	CStringArray m_number;

	BOOL createNewLayer();
	void getTrackPoints(); // 获取移动的点
	void initMap();		// 初始化参数（使用 CMoTrackLayer 挂接动态数据，在getTrackPoints后调用）
	void showRoad();	// 显示底层道路
	void usingMoSymbol(CMoMapLayer m_MapLayer, COLORREF r, int sz, int style);	// 设置地图符号

	CMoPoints*   m_trackPts;		// 动态数据的轨迹
	long*		 m_curPtIndex;		// 当前点的索引号
	CMoGeoEvent* m_curGeoEvent;		// 当前点事件

	COLORREF	 m_color[COLOR_NUM];		// 颜色数组

	unsigned int m_trackPtsNum; // 挂接的点的数目（即道路线的数目）

	CTypedPtrArray<CPtrArray, CMoPoint> m_pointArray;		// 点
	//CTypedPtrArray<CPtrArray, CMoLine> m_lineArray;			// 线
	//CTypedPtrArray<CPtrArray, CMoPolygon> m_polygonArray;	// 面

public:
	CMoPoint pt; 

protected: // create from serialization only
	CVS2013_MO_DEMOView();
	
	DECLARE_DYNCREATE(CVS2013_MO_DEMOView)

public:
	enum{ IDD = IDD_VS2013_MO_DEMO_FORM };

// Attributes
public:
	CVS2013_MO_DEMODoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct

// Implementation
public:
	virtual ~CVS2013_MO_DEMOView();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	CMap1 m_map;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnFileOpen();
	DECLARE_EVENTSINK_MAP()
	void MouseDownMap1(short Button, short Shift, long X, long Y);
	void AfterTrackingLayerDrawMap1(long hDC);
	afx_msg void OnOpetationGettrackpoints();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnOpetationStart();
	afx_msg void OnOpetationStop();
	afx_msg void OnOpetationAutodraw();
};

#ifndef _DEBUG  // debug version in VS2013_MO_DEMOView.cpp
inline CVS2013_MO_DEMODoc* CVS2013_MO_DEMOView::GetDocument() const
   { return reinterpret_cast<CVS2013_MO_DEMODoc*>(m_pDocument); }
#endif

