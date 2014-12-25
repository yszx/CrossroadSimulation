
// VS2013_MO_DEMOView.h : interface of the CVS2013_MO_DEMOView class
//

#pragma once

#include "resource.h"
#include "MO Commons/map.h"
#include "ATRNS_SIMU.h"

class CVS2013_MO_DEMOView : public CFormView
{
	//  [12/17/2014 guojianzhu]
	// 1. ������õ�ͼ���ź���
	// 2. ��Ӽ��ζ��󼯺ϣ�����غ���
	// 3. �����ɫ����������
	//  [12/20/2014 guojianzhu]
	// 4. �Զ����ɵ�·����
public:
	//  [12/25/2014 guojianzhu]
	//friend class Car;
	vector<TrafficLight>	m_Light;		// ������
	vector<Car>				m_Car;			// ������
	//  [12/24/2014 guojianzhu]
	vector<CMoPoints>	m_TrackPoints;		// �켣��

	//  [12/21/2014 guojianzhu]
	CrossRoad m_crsRd;	// ��·���ݽṹ��

	unsigned int		m_lyrNum;	// ͼ�����
	CMoDataConnection*	m_dataCon;
	CMoTableDesc*		m_desc;
	CMoMapLayer*		m_layer;
	// ���Գ�Ա
	/*CStringArray m_name;
	CStringArray m_number;*/

	//  [12/24/2014 guojianzhu]
	/* ��������
	* ˵�������ĶԳƺ���
	* ���ܣ�����Ϊ���е��߶������ĵ����꣬����һ���߶���
	*/
	BOOL initTrakingLayer();	// ���� MoTrackingLayer ������
	CMoLine centroSymmetric(CMoLine& l, float offX, float offY);				// ���ĶԳ�
	CMoLine rotoffSymmetric(CMoLine& l, float offX, float offY, double ang);	// ��תƽ��
	void initMap2();	// ��ʼ��һЩ���� ����
	void startTest();	// ���в���

	BOOL getTrackLine();	// ���ʮ��·�ڵĸ�����
	BOOL createCrossRoad();	// ����ʮ��·�ڵ�·���ݣ���� m_crsRd
	BOOL createNewLayer();	// ����ͼ�㺯�������ڲ���
	void getTrackPoints();	// ��ȡ�ƶ��ĵ�
	void initMap();			// ��ʼ��������ʹ�� CMoTrackLayer �ҽӶ�̬���ݣ���getTrackPoints����ã�
	void showRoad();		// ��ʾ�ײ��·
	void usingMoSymbol(CMoMapLayer m_MapLayer, COLORREF r, int sz, int style);	// ���õ�ͼ����

	CMoPoints*   m_trackPts;		// ��̬���ݵĹ켣
	long*		 m_curPtIndex;		// ��ǰ���������
	CMoGeoEvent* m_curGeoEvent;		// ��ǰ���¼�

	unsigned int m_trackPtsNum; // �ҽӵĵ����Ŀ������·�ߵ���Ŀ��

	CTypedPtrArray<CPtrArray, CMoPoint> m_pointArray;		// ��
	//CTypedPtrArray<CPtrArray, CMoLine> m_lineArray;			// ��
	//CTypedPtrArray<CPtrArray, CMoPolygon> m_polygonArray;	// ��

	// ���캯��
protected: 
	CVS2013_MO_DEMOView();
	// ��������
public:
	virtual ~CVS2013_MO_DEMOView();
public:
	CMoPoint pt; 
	
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
	afx_msg void OnTestGettrackline();
	afx_msg void OnTestInitm();
	afx_msg void OnTestStarttest();
	afx_msg void OnTestStoptest();
};





#ifndef _DEBUG  // debug version in VS2013_MO_DEMOView.cpp
inline CVS2013_MO_DEMODoc* CVS2013_MO_DEMOView::GetDocument() const
   { return reinterpret_cast<CVS2013_MO_DEMODoc*>(m_pDocument); }
#endif

