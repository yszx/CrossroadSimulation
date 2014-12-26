/* 2014.10.17
*  ˵�����ܶ���루ע�ͣ��ǲ��Դ��룬ԭ�岻��ɾ����
* 1. CMoLine����SetMeasuresAsLength �������ǣ�
Calculates a measure value for every vertex on a Line object based
on the distance of the vertex from the start of the Line.

* 2. lineTrack m_trackPts ��Ϊnew��ָ�룬����ʹ�� delete

* 3. //  [0:09//12/18/2014 guojianzhu]
	������Ŀ�ܴ��ɣ��������������ǣ��Զ�����ţ����ӣ����漰�����������ӳ��ɼ��ζ��󣨵�һ�ʣ�
	���ڶ��ʣ������е����漰����·���»��ơ�����ӳ�伸�ζ������˳����ٶȿ���
	�������ʣ����̵ƣ����̵ƻ��ơ������Ӽ���ģ��
	�������ʣ�ת����ο��ƻ������

* 4. �о��Զ�����ʮ��·�� 

*/
#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "VS2013_MO_DEMO.h"
#endif

#include "VS2013_MO_DEMODoc.h"
#include "VS2013_MO_DEMOView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//#define COMMENT_LINE
#define COMMENT_LINE2		// �ֶ�Ͷ��
//#define COMMENT_LINE3		// ���Ͷ��


// CVS2013_MO_DEMOView

IMPLEMENT_DYNCREATE(CVS2013_MO_DEMOView, CFormView)

BEGIN_MESSAGE_MAP(CVS2013_MO_DEMOView, CFormView)
	ON_WM_SIZE()
	ON_COMMAND(ID_FILE_OPEN, &CVS2013_MO_DEMOView::OnFileOpen)
	ON_COMMAND(ID_OPETATION_GETTRACKPOINTS, &CVS2013_MO_DEMOView::OnOpetationGettrackpoints)
	ON_WM_TIMER()
	ON_COMMAND(ID_OPETATION_START, &CVS2013_MO_DEMOView::OnOpetationStart)
	ON_COMMAND(ID_OPETATION_STOP, &CVS2013_MO_DEMOView::OnOpetationStop)
	ON_COMMAND(ID_OPETATION_AUTODRAW, &CVS2013_MO_DEMOView::OnOpetationAutodraw)
	ON_COMMAND(ID_TEST_GETTRACKLINE, &CVS2013_MO_DEMOView::OnTestGettrackline)
	ON_COMMAND(ID_TEST_INITM, &CVS2013_MO_DEMOView::OnTestInitm)
	ON_COMMAND(ID_TEST_STARTTEST, &CVS2013_MO_DEMOView::OnTestStarttest)
	ON_COMMAND(ID_TEST_STOPTEST, &CVS2013_MO_DEMOView::OnTestStoptest)
END_MESSAGE_MAP()

// CVS2013_MO_DEMOView construction/destruction

void CVS2013_MO_DEMOView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MAP1, m_map);
}

BOOL CVS2013_MO_DEMOView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	return CFormView::PreCreateWindow(cs);
}

void CVS2013_MO_DEMOView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	//  [12/12/2014 guojianzhu]
	// Map�ؼ����Բ���
	//m_map.SetAppearance(0); // 1-mo3D 0-moFlat
	m_map.SetBackColor(RGB(240,255,233));	// ����ʵ������RGB
	srand(unsigned(time(0)));
}


// CVS2013_MO_DEMOView diagnostics

#ifdef _DEBUG
void CVS2013_MO_DEMOView::AssertValid() const
{
	CFormView::AssertValid();
}

void CVS2013_MO_DEMOView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CVS2013_MO_DEMODoc* CVS2013_MO_DEMOView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CVS2013_MO_DEMODoc)));
	return (CVS2013_MO_DEMODoc*)m_pDocument;
}
#endif //_DEBUG


// CVS2013_MO_DEMOView message handlers


void CVS2013_MO_DEMOView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if (m_map.m_hWnd)
		m_map.SetWindowPos(0, 0, 0, cx, cy, SWP_NOZORDER);
}

// �˺����ѵ��Թ�
void CVS2013_MO_DEMOView::OnFileOpen()
{
	// TODO: Add your command handler code here
	CString filter(TEXT("ʸ����ͼ�ļ� (*.shp)|*.shp|դ���ͼ�ļ�(*.jpg)|*.jpg|"));
	CFileDialog dlg(TRUE, TEXT(".shp|*.jpg"), 0, OFN_ALLOWMULTISELECT |
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, filter);
	if (dlg.DoModal() == IDOK)
	{
		POSITION pos1 = dlg.GetStartPosition();//��ʼ�������ж�ѡ���ļ�·��
		while (pos1)//��Ӷ��ͼ��
		{
			CString pathName = dlg.GetNextPathName(pos1);
			if ((!pathName.Right(3).Compare("shp")) || (!pathName.Right(3).Compare("SHP")))
				AddLayer(m_map, pathName);
			else  AddImageLayer(m_map, pathName);
		}
		m_map.Refresh();
	}	
	
	showRoad();	// ��ʾ��·��������Ĳ����������޸�
}
BEGIN_EVENTSINK_MAP(CVS2013_MO_DEMOView, CFormView)
	ON_EVENT(CVS2013_MO_DEMOView, IDC_MAP1, DISPID_MOUSEDOWN, CVS2013_MO_DEMOView::MouseDownMap1, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CVS2013_MO_DEMOView, IDC_MAP1, 4, CVS2013_MO_DEMOView::AfterTrackingLayerDrawMap1, VTS_I4)
END_EVENTSINK_MAP()


// MouseDown ��Ϣ
//void CVS2013_MO_DEMOView::MouseDownMap1(short Button, short Shift, long X, long Y)
//{
//	// TODO: Add your message handler code here
//	//  [12/12/2014 guojianzhu]
//	//MessageBox("����", 0, MB_OK | MB_ICONASTERISK);
//
//	//  [12/12/2014 guojianzhu]
//	// Map �ؼ����� pan������ �����ĵ�
//	if (Shift == 0){
//		CMoRectangle rec = m_map.TrackRectangle();
//		if (rec)
//			m_map.SetExtent(rec);
//	}
//	else
//		m_map.Pan();	// MapControl�ؼ������һ�������Ͱ��������ȫ��װ��
//
//	// *MapDistance ����Z
//	//float test = m_map.FromMapDistance(1000);
//
//	// SetCoorinateSystem ����
//	VARIANT va = m_map.GetCoordinateSystem();
//	CMoRectangle bor = m_map.GetExtent();	// ֻ��׷�ٵ�ָ���ַ���޷���ȡ�����ֵ
//	//m_map.SetRotationAngle(30);
//
//	// ��ȡ�ռ�����
//	pt = m_map.ToMapPoint(float(X), float(Y));
//
//	// ���� ** �¼�
//	CMoRectangle rec(m_map.GetFullExtent());
//	VARIANT var;
//	VariantInit(&var);
//	va.vt = VT_DISPATCH;
//	va.pdispVal = rec.m_lpDispatch;
//	CMoTrackingLayer tLayer(m_map.GetTrackingLayer());
//	tLayer.Refresh(TRUE, var); // Ӧ�þ������������ AfterTrackingLayerDrawMap1 ����
//}
void CVS2013_MO_DEMOView::MouseDownMap1(short Button, short Shift, long X, long Y)
{

}

void CVS2013_MO_DEMOView::AfterTrackingLayerDrawMap1(long hDC)
{
	// TODO: Add your message handler code here
	// ��������
	CMoFont font;
	font.SetName("����");
	
	// �����ı����Ŷ���
	CMoTextSymbol m_Textsymbol;
	if (!m_Textsymbol.CreateDispatch("MapObjects2.TextSymbol")) return;
	m_Textsymbol.SetFont(font);

	// ע��
	m_map.DrawText("MO_GIS", pt, m_Textsymbol);
}

void CVS2013_MO_DEMOView::usingMoSymbol(CMoMapLayer m_MapLayer, COLORREF r, int sz, int style)
{
	CMoSymbol layerSymbol(m_MapLayer.GetSymbol());	// ����һ���µķ��Ŷ���
	// 1. �ж�ͼ��ļ�������
	//// ��ʼ������ݵļ�������
	long m_ShapeType = m_MapLayer.GetShapeType();

	// 0��1��2 ����Ϊ������
	if (m_ShapeType == 21 || m_ShapeType == 24)
		layerSymbol.SetSymbolType(0);
	else if (m_MapLayer.GetShapeType() == 22)
		layerSymbol.SetSymbolType(1);
	else
		layerSymbol.SetSymbolType(2);

	// 2.���÷��ŵ�����
	//// ���÷�����ɫ
	layerSymbol.SetColor(r);

	//// ���÷��Ŵ�С
	layerSymbol.SetSize(sz);

	// 3. ���÷�������
	// �������������
	layerSymbol.SetStyle(style);
}

void CVS2013_MO_DEMOView::showRoad()
{
	//  [12/17/2014 guojianzhu]
	// ��ͼ�������ò���
	// Ϊʲôû��Ч��----�ѽ����m_map�ؼ�����δˢ��

	/*VARIENT �����ĳ�ʼ��*/
	VARIANT index;
	VariantInit(&index);
	index.vt = VT_I4;		// �ýṹ�ı�������Ϊ����

	CMoLayers lyrs = m_map.GetLayers();
	CMoMapLayer lyr;

	// ����·ͼ��ŵ����
	for (int i = 0; i < lyrs.GetCount(); ++i){
		index.lVal = i;				// �ڼ���ͼ������
		lyr = lyrs.Item(index);		// �ڼ���ͼ��
		if (lyr.GetName() == "road_buf2")
			lyrs.MoveToBottom(i);
	}

	// ����ͼ��Ҫ�ص�����
	for (int i = 0; i < lyrs.GetCount(); ++i){
		index.lVal = i;				// �ڼ���ͼ������
		lyr = lyrs.Item(index);		// �ڼ���ͼ��

		if (lyr.GetName() == "road_buf2")
			usingMoSymbol(lyr, RGB(110, 110, 110), 2, moSolidFill);	// ��
		else if (lyr.GetName() == "road"){
			usingMoSymbol(lyr, RGB(255, 255, 255), 2, moDashLine);	// ��·������
			lyr.SetVisible(FALSE); // ��·����������Ϊ���ɼ�
		}
		else if (lyr.GetName() == "road_center")
			usingMoSymbol(lyr, RGB(255, 255, 130), 4, moSolidLine);	// ��·�ָ���
	}
	m_map.Refresh();

	//CMoMapLayer lyr = m_map.GetLayers().Item(index);	// ��ȡ��һͼ��
	//usingMoSymbol(lyr, RGB(110, 110, 110),2);			// ����ͼ����ɫ
	//m_map.Refresh();


	//CString lyr_name = lyr.GetName();
	//CMoSymbol lyrSym(lyr.GetSymbol());								// �����µķ��Ŷ���

	//if (lyr.GetShapeType() == 22){	// ͼ��Ϊ��״Ҫ��
	//	lyrSym.SetSymbolType(1);	// ���Ŷ�������Ϊ��״
	//	//unsigned long color_test = lyrSym.GetColor();	// ��ȡ��ɫ
	//	//unsigned long color_test2 = RGB(255, 100, 10);
	//	lyrSym.SetColor(RGB(255, 0, 0));				// ������ɫ
	//	lyrSym.SetStyle(moDashLine);
	//	m_map.Refresh();
	//}
}

void CVS2013_MO_DEMOView::getTrackPoints()
{
	CMoMapLayer lyr(m_map.GetLayers().Item(COleVariant("road")));	// road ͼ��
	CMoRecordset rec(lyr.GetRecords());	// road ͼ���¼��
	int lineNum = rec.GetCount();		// ��¼���� <-> ����Ŀ
	m_trackPtsNum = lineNum;			// ��ֵ����ĳ�Ա���������䣩
	CMoLine* lineTrack = new CMoLine[lineNum];	// CMoLine ����
	unsigned short lineIndex = 0;		// ����

	// ��ȡ road ͼ��������
	rec.MoveFirst();
	while (lineIndex < lineNum){
		CMoField lineFld(rec.GetFields().Item(COleVariant("shape")));	
										// CMoField ��¼���������õ��ߵ�����
		CMoLine lineTmp(lineFld.GetValue().pdispVal);	
		lineTrack[lineIndex] = lineTmp;

		rec.MoveNext();
		++lineIndex;
	}

	// �ȷ��߻�ȡ�ȷֵ�
	m_trackPts = new CMoPoints[lineNum];
	for (int i = 0; i < lineNum;++i)
		m_trackPts[i].CreateDispatch(TEXT("MapObjects2.Points"));

	int divNum = 200;					 // �ȷ���Ŀ
	double* step = new double[lineNum] ; // �ȷ־���
	long nFirstPt = 0;					 // һֱΪ0
	for (int i = 0; i < lineNum; ++i){
		lineTrack[i].SetMeasuresAsLength();
		step[i] = lineTrack[i].GetLength() / divNum;
	}

	//// ���Դ���
	//CMoField lineFld(rec.GetFields().Item(COleVariant("shape")));
	//rec.MoveFirst();
	//CMoLine lineTmp(lineFld.GetValue().pdispVal);
	//lineTmp.SetMeasuresAsLength(); // ���д���δ���--����bug
	//CMoPoints ptsTmp;

	//for (int i = 0; i < lineNum; ++i){
	//	ptsTmp = lineTmp.ReturnPointEvents(step[0] * i);
	//	m_test.Add(ptsTmp.Item(COleVariant(nFirstPt)));
	//}
	//// ���Դ���

	for (int i = 0; i < lineNum; ++i){
		for (int j = 0; j < divNum; ++j){
			CMoPoints ptsTmp;
			ptsTmp = lineTrack[i].ReturnPointEvents(step[i] * j);
			m_trackPts[i].Add(ptsTmp.Item(COleVariant(nFirstPt)));
		}
	}

	delete[] lineTrack;
}
void CVS2013_MO_DEMOView::OnOpetationStart()
{
	SetTimer(1, 50, NULL);
}

// ���� getTrackPoints ��initMap ����
void CVS2013_MO_DEMOView::OnOpetationGettrackpoints()
{
	getTrackPoints();
	initMap();			// ����̬�㣨���ݣ��ҽӵ� m_map �� TrackingLayer ����
}


void CVS2013_MO_DEMOView::OnOpetationStop()
{
	KillTimer(1);
}

// ʹ�� CMoTrackLayer �ҽӶ�̬����
void CVS2013_MO_DEMOView::initMap()
{
	//  [12/18/2014 guojianzhu]
	srand(unsigned(time(0)));

	/* ���� TrackingLayer �ķ��� */
	CMoTrackingLayer tLyr = m_map.GetTrackingLayer();	// ��ȡ trackinglayer
	tLyr.SetSymbolCount(m_trackPtsNum);					// ���÷��Ÿ���,������ 
														//CMoGeoEvent �� addEvent �ڶ���������Ӧ

	// CMoFont ��������
	CMoFont fnt;
	fnt.InitializeFont();	// ���� CreateDispatch
	//fnt.SetName(TEXT("ESRI Transportation & Civic"));	// ���������
	fnt.SetName(TEXT("MoCars2"));	// ���������

	// ����->symbol
	for (unsigned int i = 0; i < m_trackPtsNum; ++i){
		CMoSymbol sym(tLyr.GetSymbol(i));
		sym.SetColor(CarColor[rand() % CAR_COLOR_NUM]);
		sym.SetStyle(moTrueTypeMarker);			// TrueType����
		sym.SetFont(fnt.GetFontDispatch());
		sym.SetSize(60);
		sym.SetCharacterIndex(1);			// ������������ֵı��ֵ��ʮ���ƣ�

		// ��ʱ����ת
		if (i == 1) sym.SetRotation(270);
		if (i == 2) sym.SetRotation(180);
		else if (i == 3) sym.SetRotation(90);
	}
	fnt.ReleaseFont();

	// ʹ�� CMoTrackLayer �ҽӶ�̬����
	CMoPoint* pt = new CMoPoint[m_trackPtsNum];
	m_curGeoEvent = new CMoGeoEvent[m_trackPtsNum];
	m_curPtIndex = new long[m_trackPtsNum];
	for (unsigned int i = 0; i < m_trackPtsNum; ++i){
		pt[i].CreateDispatch("MapObjects2.Point");
		pt[i].SetX(0);
		pt[i].SetY(0);

		m_curGeoEvent[i] = m_map.GetTrackingLayer().AddEvent(pt[i], i); // �ҽ�
		m_curPtIndex[i] = 0;
	}
}


// �Զ���ʮ��·�ڵ�·
void CVS2013_MO_DEMOView::OnOpetationAutodraw()
{
	//createNewLayer();
	createCrossRoad();
}

BOOL CVS2013_MO_DEMOView::createNewLayer()
{
	//if (!m_dataCon.CreateDispatch(TEXT("MapObjects2.DataConnection")))
	//	return FALSE;
	//if (!m_desc.CreateDispatch("MapObjects2.TableDesc"))
	//	return FALSE;
	//VARIANT vt;
	//vt.vt = VT_BOOL;
	//vt.boolVal = VARIANT_FALSE;
	//m_dataCon.SetDatabase((LPCTSTR)"F:\\�����\\MO\\����\\ArcInfo\\test"); //����ͼ�㱣��Ŀ¼
	//if (!m_dataCon.Connect())
	//	return FALSE;

	//// ����ͼ����ֶθ��������ԣ���������������һ��ΪNumber��һ��ΪName����ʾ��ĺ��뼰���ơ�
	//m_desc.SetFieldCount(2);
	//m_desc.SetFieldName(0, "Number"); // ����
	//m_desc.SetFieldType(0, moDouble); // ���
	//m_desc.SetFieldLength(0, 10);		// ����
	//m_desc.SetFieldPrecision(0, 10);	// λ������ֵ��
	//m_desc.SetFieldScale(0, 5);		// С��λ��

	//m_desc.SetFieldName(1, "Name");
	//m_desc.SetFieldType(1, moDouble);
	//m_desc.SetFieldLength(1, 10);
	//m_desc.SetFieldPrecision(1, 10);	
	//m_desc.SetFieldScale(1, 5);		

	//// ������ͼ��,�������ơ����ͼ��ֶΡ�
	//CMoGeoDataset geoDataset(m_dataCon.AddGeoDataset("����_��", moShapeTypePoint,
	//	(LPDISPATCH)m_desc, vt, vt));
	//if (!m_layer.CreateDispatch(TEXT("MapObjects2.MapLayer")))
	//	return FALSE;

	//// ����ͼ��ļ�¼��
	////CMoPoint* pt = new CMoPoint[10];
	//CMoPoint pt[10];
	//for (int i = 0; i < 10; ++i){
	//	pt[i].CreateDispatch("MapObjects2.Point");
	//	//CMoPoint pt_tmp = m_map.ToMapPoint(i, i);
	//	pt[i].SetX(float(10 * (i + 1)));	// �������������
	//	pt[i].SetY(float(10 * (i + 1)));
	//	m_pointArray.Add(pt[i]);
	//}

	//m_layer.SetGeoDataset(geoDataset);
	//CMoRecordset recs(m_layer.GetRecords());

	//// ��������
	//VARIANT test_idx;
	//VariantInit(&test_idx);
	//test_idx.vt = VT_I4;

	//long edit_sta = recs.GetEditMode();
	//for (int i = 0; i < m_pointArray.GetSize(); ++i){
	//	recs.AddNew();
	//	edit_sta = recs.GetEditMode();
	//	SetValue(recs.GetFields(), "Shape", pt[i]);
	//	SetValue(recs.GetFields(), "Number", 1.0);
	//	SetValue(recs.GetFields(), "Name", 2.0);

	//	recs.Update();
	//}
	//

	////����
	//test_idx.lVal = 0;
	//edit_sta = recs.GetEditMode();
	//long rec_cnt = recs.GetCount();
	//CMoField fld(recs.GetFields().Item(test_idx));
	//CString rec_nm = fld.GetName();

	//long test_tpe = m_layer.GetShapeType();


	////���õ����ɫ���������ͼ��
	//m_layer.GetSymbol().SetColor(moRed);
	//m_layer.GetSymbol().SetSize(10);
	//CMoLayers layers(m_map.GetLayers());
	//layers.Add(m_layer);

	//m_map.Refresh();
	return TRUE;
}

void CVS2013_MO_DEMOView::OnTestInitm()
{
	initMap2();
}

CMoLine CVS2013_MO_DEMOView::centroSymmetric(CMoLine& l, float offX, float offY)
{
	CMoLine mo_line;
	mo_line.CreateDispatch("MapObjects2.Line");

	CMoParts pats = l.GetParts();
	for (int j = 0; j < pats.GetCount(); ++j){
		CMoPoints pts(pats.Item(COleVariant(long(j))));
		CMoPoints ptsTmp;
		ptsTmp.CreateDispatch("MapObjects2.Points");
		for (int i = 0; i < pts.GetCount(); ++i){
			CMoPoint pt(pts.Item(COleVariant(long(i))));	// pt ���´����Ķ���
			pt.SetX(-pt.GetX() + 2 * offX);
			pt.SetY(-pt.GetY() + 2 * offY);
			ptsTmp.Add(pt);
		}
		mo_line.GetParts().Add(ptsTmp);
	}

	return mo_line;
}
void CVS2013_MO_DEMOView::OnTestGettrackline()
{
	getTrackLine();
}

/////////////////////////////�ؼ�����/////////////////////////////////////////////
CVS2013_MO_DEMOView::CVS2013_MO_DEMOView()
: CFormView(CVS2013_MO_DEMOView::IDD)
{
	// ��ʼ����·�ṹ����
	m_crsRd.rd_wid = 4;
	m_crsRd.rd_lnm = 3;
	m_crsRd.rd_rnm = 3;
	m_crsRd.rd_xlen = 200;
	m_crsRd.rd_ylen = 200;
	m_crsRd.rd_sidwk_wid = 5;
	m_crsRd.rd_arc = m_crsRd.rd_sidwk_wid;
	m_crsRd.rd_cen_wid = 1.5;

	// ���ٶ���ռ�
	m_lyrNum = LAYER_NUM;
	m_dataCon = new CMoDataConnection[m_lyrNum];
	m_desc = new CMoTableDesc[m_lyrNum];
	m_layer = new CMoMapLayer[m_lyrNum];

	// ʱ��
	ClockState = 0;
}

CVS2013_MO_DEMOView::~CVS2013_MO_DEMOView()
{
	// �ͷ��ڹ��캯���ﴴ���Ŀռ�
	if (m_trackPts) delete[] m_trackPts;
	if (m_curPtIndex) delete[] m_curPtIndex;
	if (m_curGeoEvent) delete[] m_curGeoEvent;
	if (m_dataCon) delete[] m_dataCon;
	if (m_desc) delete[] m_desc;
	if (m_layer) delete[] m_layer;

	MoTrackingLayer.ClearEvents();
	m_Car.clear();						// 
}

BOOL CVS2013_MO_DEMOView::getTrackLine()
{
	CMoLine		mo_line[5];
	CMoPoints	mo_pts[5];
	CMoPoint	mo_pt[102];
	for (int i = 0; i < 5; ++i)
		mo_line[i].CreateDispatch("MapObjects2.Line");
	for (int i = 0; i < 5; ++i)
		mo_pts[i].CreateDispatch("MapObjects2.Points");
	for (int i = 0; i < 102; ++i)
		mo_pt[i].CreateDispatch("MapObjects2.Point");

	// x y ΪԲ�� r Ϊ�뾶
	float x = m_crsRd.rd_xlen / 2 - m_crsRd.rd_lnm*m_crsRd.rd_wid - m_crsRd.rd_sidwk_wid;
	float y = m_crsRd.rd_ylen / 2 - m_crsRd.rd_lnm*m_crsRd.rd_wid - m_crsRd.rd_sidwk_wid;
	float r = m_crsRd.rd_sidwk_wid + m_crsRd.rd_wid / 2;

	mo_pt[0].SetX(0);
	mo_pt[0].SetY(y + r);
	for (int i = 0; i < 100; ++i){
		mo_pt[i + 1].SetX(x + r*sin(i / 100.0*PI / 2.0));
		mo_pt[i + 1].SetY(y + r*cos(i / 100.0*PI / 2.0));
	}
	mo_pt[101].SetX(x + r);
	mo_pt[101].SetY(0);

	for (int i = 0; i < 102; ++i)
		mo_pts[0].Add(mo_pt[i]);
	mo_line[0].GetParts().Add(mo_pts[0]);		// ��һ���켣

	mo_pt[1].SetX(m_crsRd.rd_xlen);
	mo_pt[1].SetY(y + r);
	for (int i = 0; i < 2; ++i)
		mo_pts[1].Add(mo_pt[i]);
	mo_line[1].GetParts().Add(mo_pts[1]);		// �ڶ����켣

	mo_line[2].GetParts().Add(mo_pts[1]);
	mo_line[2].Offset(0, m_crsRd.rd_wid);		// �������켣

	mo_line[3].GetParts().Add(mo_pts[1]);
	mo_line[3].Offset(0, m_crsRd.rd_wid * 2);	// �������켣

	// x ����
	y = m_crsRd.rd_ylen - y; // x��Գ�
	r += m_crsRd.rd_lnm*m_crsRd.rd_wid;

	mo_pt[0].SetX(0);
	mo_pt[0].SetY(y-r);
	for (int i = 0; i < 100; ++i){
		mo_pt[i + 1].SetX(x + r*sin(i / 100.0*PI / 2.0));
		mo_pt[i + 1].SetY(y - r*cos(i / 100.0*PI / 2.0));
	}
	mo_pt[101].SetX(x + r);
	mo_pt[101].SetY(m_crsRd.rd_ylen);
	for (int i = 0; i < 102; ++i)
		mo_pts[4].Add(mo_pt[i]);
	mo_line[4].GetParts().Add(mo_pts[4]);	// �������켣

	// �洢��������
	for (int i = 0; i < 5; ++i)
		MoTrackLine.push_back(mo_line[i]);

	// ���ĶԳƱ任
	for (int i = 0; i < 5; ++i)
		MoTrackLine.push_back(centroSymmetric(mo_line[i], m_crsRd.rd_xlen / 2, m_crsRd.rd_ylen / 2));

	// ��תƽ�Ʊ任
	for (int i = 0; i < 5; ++i)
		MoTrackLine.push_back(rotoffSymmetric(mo_line[i], m_crsRd.rd_xlen, 0, 90));
	for (int i = 0; i < 5; ++i)
		MoTrackLine.push_back(rotoffSymmetric(mo_line[i], 0, m_crsRd.rd_ylen, 270));

	return TRUE;
}

CMoLine CVS2013_MO_DEMOView::rotoffSymmetric(CMoLine& l, float offX, float offY, double ang)
{
	// ���㷽�㣬�ԽǶȽ��б任
	ang = ang*PI / 180.0;

	CMoLine mo_line;
	mo_line.CreateDispatch("MapObjects2.Line");

	CMoParts pats = l.GetParts();
	for (int j = 0; j < pats.GetCount(); ++j){
		CMoPoints pts(pats.Item(COleVariant(long(j))));
		CMoPoints ptsTmp;
		ptsTmp.CreateDispatch("MapObjects2.Points");
		for (int i = 0; i < pts.GetCount(); ++i){
			CMoPoint pt(pts.Item(COleVariant(long(i))));	// pt ���´����Ķ���
			double x = pt.GetX();
			double y = pt.GetY();
			pt.SetX(x*cos(ang) - y*sin(ang) + offX);
			pt.SetY(x*sin(ang) + y*cos(ang) + offY);
			ptsTmp.Add(pt);
		}
		mo_line.GetParts().Add(ptsTmp);
	}

	return mo_line;
}

BOOL CVS2013_MO_DEMOView::createCrossRoad()
{
	// �ӵײ㴴����� MO ����
	for (unsigned int i = 0; i < m_lyrNum; ++i){
		if (!m_dataCon[i].CreateDispatch(TEXT("MapObjects2.DataConnection")))
			return FALSE;
		if (!m_desc[i].CreateDispatch("MapObjects2.TableDesc"))
			return FALSE;
		if (!m_layer[i].CreateDispatch("MapObjects2.MapLayer"))
			return FALSE;
	}

	// ��������
	VARIANT vt;
	vt.vt = VT_BOOL;
	vt.boolVal = VARIANT_FALSE;
	for (unsigned int i = 0; i < m_lyrNum; ++i){
		m_dataCon[i].SetDatabase((LPCTSTR)"F:\\�����\\MO\\����\\ArcInfo\\ʮ��·��"); //����ͼ�㱣��Ŀ¼
		if (!m_dataCon[i].Connect())
			return FALSE;
	}

	// ����ÿһ��������ļ���
	CMoGeoDataset geoDataset[LAYER_NUM];
	geoDataset[0] = m_dataCon[0].AddGeoDataset("������_˫����", moShapeTypeLine,
		(LPDISPATCH)m_desc[0], vt, vt);
	geoDataset[1] = m_dataCon[1].AddGeoDataset("��·", moShapeTypeLine,
		(LPDISPATCH)m_desc[1], vt, vt);
	geoDataset[2] = m_dataCon[2].AddGeoDataset("���е�", moPolygon,
		(LPDISPATCH)m_desc[2], vt, vt);
	geoDataset[3] = m_dataCon[3].AddGeoDataset("��Χ", moPolygon,
		(LPDISPATCH)m_desc[3], vt, vt);

	// ����ͼ������Լ�����
	/// ��һ��
	CMoLine   lnCntre[4];			// 4 ����
	CMoPoints pts[2], pts2[2];	// 2 �����(��һ��ڶ���ĵ�)
	CMoPoint  pt[4];			// 4 ������
	for (int i = 0; i < 4; ++i)
		lnCntre[i].CreateDispatch("MapObjects2.Line");
	for (int i = 0; i < 2; ++i){
		pts[i].CreateDispatch("MapObjects2.Points");
		pts2[i].CreateDispatch("MapObjects2.Points");
	}
	for (int i = 0; i < 4; ++i)
		pt[i].CreateDispatch("MapObjects2.Point");

	// �ߵ������趨��ͨ�� Parts Points �������ã�
	float wid = m_crsRd.rd_cen_wid;	// ˫���߿��
	pt[0].SetX(0);
	pt[0].SetY(m_crsRd.rd_ylen / 2);
	pt[1].SetX(m_crsRd.rd_xlen);
	pt[1].SetY(m_crsRd.rd_ylen / 2);
	pt[2].SetX(m_crsRd.rd_xlen / 2);
	pt[2].SetY(0);
	pt[3].SetX(m_crsRd.rd_xlen / 2);
	pt[3].SetY(m_crsRd.rd_ylen);


	pts[0].Add(pt[0]);
	pts[0].Add(pt[1]);	// �㴮
	pts[1].Add(pt[2]);
	pts[1].Add(pt[3]);	// �㴮

	lnCntre[0].GetParts().Add(pts[0]);	// ��һ���߶�
	lnCntre[1].GetParts().Add(pts[0]);	// �ڶ����߶�
	lnCntre[2].GetParts().Add(pts[1]);	// �������߶�
	lnCntre[3].GetParts().Add(pts[1]);	// �������߶�
	lnCntre[0].Offset(0, wid / 2);
	lnCntre[1].Offset(0, -wid / 2);
	lnCntre[2].Offset(wid / 2, 0);
	lnCntre[3].Offset(-wid / 2, 0);

	/// �ڶ���
	int dashRdNum = 4 * (m_crsRd.rd_lnm + m_crsRd.rd_rnm - 2);
	CMoLine   *lnDash = new CMoLine[dashRdNum];
	for (int i = 0; i < dashRdNum; ++i)
		lnDash[i].CreateDispatch("MapObjects2.Line");

	pt[0].SetX(0);
	pt[0].SetY(m_crsRd.rd_ylen / 2);
	pt[1].SetX(m_crsRd.rd_xlen / 2 - m_crsRd.rd_lnm*m_crsRd.rd_wid - m_crsRd.rd_sidwk_wid);
	pt[1].SetY(m_crsRd.rd_ylen / 2);
	pt[2].SetX(m_crsRd.rd_xlen / 2);
	pt[2].SetY(0);
	pt[3].SetX(m_crsRd.rd_xlen / 2);
	pt[3].SetY(m_crsRd.rd_ylen / 2 - m_crsRd.rd_rnm*m_crsRd.rd_wid - m_crsRd.rd_sidwk_wid);


	pts2[0].Add(pt[0]);
	pts2[0].Add(pt[1]);	// �㴮
	pts2[1].Add(pt[2]);
	pts2[1].Add(pt[3]);	// �㴮

	for (int i = 0; i < dashRdNum; ++i){
		if ((dashRdNum / 4) % 2 == 1) continue;
		if (i < dashRdNum / 2)
			lnDash[i].GetParts().Add(pts2[0]);
		else
			lnDash[i].GetParts().Add(pts2[1]);
	}
	for (int i = 0; i < dashRdNum / 2; ++i){
		if (i < dashRdNum / 8)
			lnDash[i].Offset(0, (i + 1)*m_crsRd.rd_wid);
		else if (i < dashRdNum / 4)
			lnDash[i].Offset(0, -(i + 1 - dashRdNum / 8)*m_crsRd.rd_wid);
		else if (i < 3 * dashRdNum / 8)
			lnDash[i].Offset(m_crsRd.rd_xlen / 2 + m_crsRd.rd_wid*m_crsRd.rd_rnm + m_crsRd.rd_sidwk_wid,
			(i + 1 - dashRdNum / 4)*m_crsRd.rd_wid);
		else
			lnDash[i].Offset(m_crsRd.rd_xlen / 2 + m_crsRd.rd_wid*m_crsRd.rd_rnm + m_crsRd.rd_sidwk_wid,
			-(i + 1 - 3 * dashRdNum / 8)*m_crsRd.rd_wid);
	}

	for (int i = dashRdNum / 2; i < dashRdNum; ++i){
		if (i < 5 * dashRdNum / 8)
			lnDash[i].Offset((i + 1 - dashRdNum / 2)*m_crsRd.rd_wid, 0);
		else if (i < 6 * dashRdNum / 8)
			lnDash[i].Offset(-(i + 1 - 5 * dashRdNum / 8)*m_crsRd.rd_wid, 0);
		else if (i < 7 * dashRdNum / 8)
			lnDash[i].Offset((i + 1 - 6 * dashRdNum / 8)*m_crsRd.rd_wid,
			m_crsRd.rd_ylen / 2 + m_crsRd.rd_wid*m_crsRd.rd_rnm + m_crsRd.rd_sidwk_wid);
		else
			lnDash[i].Offset(-(i + 1 - 7 * dashRdNum / 8)*m_crsRd.rd_wid,
			m_crsRd.rd_ylen / 2 + m_crsRd.rd_wid*m_crsRd.rd_rnm + m_crsRd.rd_sidwk_wid);
	}

	/// �����㣨���е���
	int walkRdNum = 4;
	CMoPoint ptWalk[4];
	CMoPoints ptsWalk[2];
	CMoPolygon poWalk[4];
	for (int i = 0; i < 4; ++i)
		ptWalk[i].CreateDispatch("MapObjects2.Point");
	for (int i = 0; i < 2; ++i)
		ptsWalk[i].CreateDispatch("MapObjects2.Points");
	for (int i = 0; i < 4; ++i)
		poWalk[i].CreateDispatch("MapObjects2.Polygon");
	ptWalk[0].SetX(m_crsRd.rd_xlen / 2 - m_crsRd.rd_lnm*m_crsRd.rd_wid - m_crsRd.rd_sidwk_wid);
	ptWalk[0].SetY(m_crsRd.rd_ylen / 2 - m_crsRd.rd_lnm*m_crsRd.rd_wid);
	ptWalk[1].SetX(m_crsRd.rd_xlen / 2 - m_crsRd.rd_lnm*m_crsRd.rd_wid);
	ptWalk[1].SetY(m_crsRd.rd_ylen / 2 - m_crsRd.rd_lnm*m_crsRd.rd_wid);
	ptWalk[2].SetX(m_crsRd.rd_xlen / 2 - m_crsRd.rd_lnm*m_crsRd.rd_wid);
	ptWalk[2].SetY(m_crsRd.rd_ylen / 2 + m_crsRd.rd_lnm*m_crsRd.rd_wid);
	ptWalk[3].SetX(m_crsRd.rd_xlen / 2 - m_crsRd.rd_lnm*m_crsRd.rd_wid - m_crsRd.rd_sidwk_wid);
	ptWalk[3].SetY(m_crsRd.rd_ylen / 2 + m_crsRd.rd_lnm*m_crsRd.rd_wid);

	for (int i = 0; i < 4; ++i)
		ptsWalk[0].Add(ptWalk[i]);

	poWalk[0].GetParts().Add(ptsWalk[0]);
	poWalk[1].GetParts().Add(ptsWalk[0]);
	poWalk[1].Offset(m_crsRd.rd_sidwk_wid + (m_crsRd.rd_lnm + m_crsRd.rd_rnm)*m_crsRd.rd_wid, 0);

	ptWalk[0].SetX(m_crsRd.rd_xlen / 2 - m_crsRd.rd_lnm*m_crsRd.rd_wid);
	ptWalk[0].SetY(m_crsRd.rd_ylen / 2 - m_crsRd.rd_lnm*m_crsRd.rd_wid - m_crsRd.rd_sidwk_wid);
	ptWalk[1].SetX(m_crsRd.rd_xlen / 2 - m_crsRd.rd_lnm*m_crsRd.rd_wid + (m_crsRd.rd_lnm + m_crsRd.rd_rnm)*m_crsRd.rd_wid);
	ptWalk[1].SetY(m_crsRd.rd_ylen / 2 - m_crsRd.rd_lnm*m_crsRd.rd_wid - m_crsRd.rd_sidwk_wid);
	ptWalk[2].SetX(m_crsRd.rd_xlen / 2 - m_crsRd.rd_lnm*m_crsRd.rd_wid + (m_crsRd.rd_lnm + m_crsRd.rd_rnm)*m_crsRd.rd_wid);
	ptWalk[2].SetY(m_crsRd.rd_ylen / 2 - m_crsRd.rd_lnm*m_crsRd.rd_wid - m_crsRd.rd_sidwk_wid + m_crsRd.rd_sidwk_wid);
	ptWalk[3].SetX(m_crsRd.rd_xlen / 2 - m_crsRd.rd_lnm*m_crsRd.rd_wid);
	ptWalk[3].SetY(m_crsRd.rd_ylen / 2 - m_crsRd.rd_lnm*m_crsRd.rd_wid - m_crsRd.rd_sidwk_wid + m_crsRd.rd_sidwk_wid);
	for (int i = 0; i < 4; ++i)
		ptsWalk[1].Add(ptWalk[i]);
	poWalk[2].GetParts().Add(ptsWalk[1]);
	poWalk[3].GetParts().Add(ptsWalk[1]);
	poWalk[3].Offset(0, m_crsRd.rd_sidwk_wid + (m_crsRd.rd_lnm + m_crsRd.rd_rnm)*m_crsRd.rd_wid);


	/// ���Ĳ�
	CMoPoint ptBord[204];
	CMoPoints ptsBord[4];
	CMoPolygon poBord[4];
	for (int i = 0; i < 204; ++i)
		ptBord[i].CreateDispatch("MapObjects2.Point");
	for (int i = 0; i < 4; ++i)
		ptsBord[i].CreateDispatch("MapObjects2.Points");
	for (int i = 0; i < 4; ++i)
		poBord[i].CreateDispatch("MapObjects2.Polygon");

	// x y ΪԲ��
	float x = m_crsRd.rd_xlen / 2 - m_crsRd.rd_lnm*m_crsRd.rd_wid - m_crsRd.rd_sidwk_wid * 2;
	float y = m_crsRd.rd_ylen / 2 - m_crsRd.rd_lnm*m_crsRd.rd_wid - m_crsRd.rd_sidwk_wid * 2;
	ptBord[0].SetX(0);
	ptBord[0].SetY(y + 2 * m_crsRd.rd_sidwk_wid);
	ptBord[1].SetX(0);
	ptBord[1].SetY(y + m_crsRd.rd_sidwk_wid);
	for (int i = 0; i < 100; ++i){
		ptBord[i + 2].SetX(x + m_crsRd.rd_sidwk_wid*sin(i / 100.0*PI / 2.0));
		ptBord[i + 2].SetY(y + m_crsRd.rd_sidwk_wid*cos(i / 100.0*PI / 2.0));
	}
	ptBord[102].SetX(x + m_crsRd.rd_sidwk_wid);
	ptBord[102].SetY(0);
	ptBord[103].SetX(x + 2 * m_crsRd.rd_sidwk_wid);
	ptBord[103].SetY(0);

	x += m_crsRd.rd_sidwk_wid;
	y += m_crsRd.rd_sidwk_wid;
	for (int i = 0; i < 100; ++i){
		ptBord[i + 104].SetX(x + m_crsRd.rd_sidwk_wid*cos(i / 100.0*PI / 2.0));
		ptBord[i + 104].SetY(y + m_crsRd.rd_sidwk_wid*sin(i / 100.0*PI / 2.0));
	}

	for (int i = 0; i < 204; ++i)
		ptsBord[0].Add(ptBord[i]);
	poBord[0].GetParts().Add(ptsBord[0]);

	/// ����Գ�
	for (int i = 0; i < 204; ++i)
		ptBord[i].SetX(m_crsRd.rd_xlen - ptBord[i].GetX());
	for (int i = 0; i < 204; ++i)
		ptsBord[1].Add(ptBord[i]);
	poBord[1].GetParts().Add(ptsBord[1]);

	for (int i = 0; i < 204; ++i)
		ptBord[i].SetY(m_crsRd.rd_ylen - ptBord[i].GetY());
	for (int i = 0; i < 204; ++i)
		ptsBord[2].Add(ptBord[i]);
	poBord[2].GetParts().Add(ptsBord[2]);

	for (int i = 0; i < 204; ++i)
		ptBord[i].SetX(m_crsRd.rd_xlen - ptBord[i].GetX());
	for (int i = 0; i < 204; ++i)
		ptsBord[3].Add(ptBord[i]);
	poBord[3].GetParts().Add(ptsBord[3]);


	// ����¼��д������
	for (unsigned int i = 0; i < m_lyrNum; ++i){
		m_layer[i].SetGeoDataset(geoDataset[i]);
	}
	CMoRecordset recs(m_layer[0].GetRecords());	// ��һͼ��ļ�¼��
	for (int i = 0; i < 4; ++i){
		recs.AddNew();
		SetValue(recs.GetFields(), "Shape", lnCntre[i]);
		recs.Update();
	}
	recs = m_layer[1].GetRecords();	// �ڶ�ͼ��ļ�¼��
	for (int i = 0; i < dashRdNum; ++i){
		recs.AddNew();
		SetValue(recs.GetFields(), "Shape", lnDash[i]);
		recs.Update();
	}
	recs = m_layer[2].GetRecords();	// ����ͼ��ļ�¼��
	for (int i = 0; i < walkRdNum; ++i){
		recs.AddNew();
		SetValue(recs.GetFields(), "Shape", poWalk[i]);
		recs.Update();
	}
	recs = m_layer[3].GetRecords();	// ����ͼ��ļ�¼��
	for (int i = 0; i < 4; ++i){
		recs.AddNew();
		SetValue(recs.GetFields(), "Shape", poBord[i]);
		recs.Update();
	}
	// ���÷��ż�ˢ��
	m_layer[0].GetSymbol().SetSize(2);
	m_layer[0].GetSymbol().SetColor(RGB(255, 251, 134));
	m_layer[1].GetSymbol().SetSize(1);
	m_layer[1].GetSymbol().SetColor(RGB(255, 255, 255));
	m_layer[1].GetSymbol().SetStyle(moDashLine);
	m_layer[2].GetSymbol().SetColor(RGB(255, 255, 255));
	m_layer[2].GetSymbol().SetStyle(moDownwardDiagonalFill);
	//m_layer[3].GetSymbol().SetColor(RGB(0, 255, 0));
	//m_layer[3].GetSymbol().SetStyle(moDiagonalCrossFill);
	m_layer[3].GetSymbol().SetStyle(moGrayFill);

	CMoLayers layers(m_map.GetLayers());
	for (unsigned int i = 0; i < m_lyrNum; ++i)
		layers.Add(m_layer[i]);
	m_map.SetBackColor(RGB(0, 0, 0));
	m_map.Refresh();

	return TRUE;
}

BOOL CVS2013_MO_DEMOView::initTrakingLayer()
{
	int carSymNum = CAR_COLOR_NUM;
	int lightSymNum = 3;

	MoTrackingLayer = m_map.GetTrackingLayer();	// ��ȡ MoTrackingLayer��ʹ��ȫ�ֱ����洢
	MoTrackingLayer.SetSymbolCount(carSymNum+lightSymNum);	

	// ���ó���������
	for (int i = 0; i < carSymNum;++i){
		CMoSymbol sym(MoTrackingLayer.GetSymbol(long(i)));
		
		CMoFont fnt;
		fnt.InitializeFont();			// ���� CreateDispatch
		fnt.SetName(TEXT("MoCars2"));	// ���������

		sym.SetColor(CarColor[i]);
		sym.SetStyle(moTrueTypeMarker);				// TrueType����
		sym.SetFont(fnt.GetFontDispatch());
		sym.SetSize(CAR_DEFAULT_SIZE);
		sym.SetCharacterIndex(1);					// ������������ֵı��ֵ��ʮ���ƣ�
		fnt.ReleaseFont();
	}

	// ���ú��̵�����
	for (int i = carSymNum; i < carSymNum + lightSymNum; ++i){
		CMoSymbol sym(MoTrackingLayer.GetSymbol(long(i)));
		sym.SetSymbolType(1);  // ����Ĭ��Ϊ 1 ����״��
		sym.SetSize(3);	 
		sym.SetColor(LightColor[i - carSymNum]);
	}

	return TRUE;
}

void CVS2013_MO_DEMOView::initMap2()
{
	initTrakingLayer();
	getTrackLine();
}

void CVS2013_MO_DEMOView::OnTestStarttest()	// ��ʱ�������в��Ժ���
{
	createCrossRoad();
	initTrakingLayer();
	getTrackLine();

#ifndef COMMENT_LINE2
	// ��������
	for (int i = 6; i <= 6; ++i){
		Car car;
		car.CreateCar(i, (rand() % 15 + 10) / 10.0);
		m_Car.push_back(car);
	}
#endif

#ifndef COMMENT_LINE
	// ������
	TrafficLight light[8];
	Pos pt1, pt2;
	float x = m_crsRd.rd_xlen / 2 - m_crsRd.rd_lnm*m_crsRd.rd_wid - m_crsRd.rd_sidwk_wid;
	float y = m_crsRd.rd_ylen / 2;
	pt1.x = x;
	pt1.y = y;
	pt2.x = x;
	pt2.y = y - m_crsRd.rd_wid;

	light[0].CreateLight(pt1, pt2);	// һ

	pt1.y -= m_crsRd.rd_rnm*m_crsRd.rd_wid;
	light[1].CreateLight(pt1, pt2);	// ��

	LIGHT[0] = pt1.x;

	pt1.x = m_crsRd.rd_xlen - pt1.x;
	pt1.y = m_crsRd.rd_ylen - pt1.y;
	pt2.x = m_crsRd.rd_xlen - pt2.x;
	pt2.y = m_crsRd.rd_ylen - pt2.y;
	light[2].CreateLight(pt1, pt2);	// ��

	pt1.y -= m_crsRd.rd_rnm*m_crsRd.rd_wid;
	light[3].CreateLight(pt1, pt2);	// ��

	LIGHT[1] = pt1.x;

	x = m_crsRd.rd_xlen / 2;
	y = m_crsRd.rd_ylen / 2 - m_crsRd.rd_lnm*m_crsRd.rd_wid - m_crsRd.rd_sidwk_wid;
	pt1.x = x;
	pt1.y = y;
	pt2.x = x + m_crsRd.rd_wid;
	pt2.y = y;
	light[4].CreateLight(pt1, pt2);	// ��

	pt1.x += m_crsRd.rd_rnm*m_crsRd.rd_wid;
	light[5].CreateLight(pt1, pt2);	// ��

	LIGHT[2] = pt1.y;

	pt1.x = m_crsRd.rd_xlen - pt1.x;
	pt1.y = m_crsRd.rd_ylen - pt1.y;
	pt2.x = m_crsRd.rd_xlen - pt2.x;
	pt2.y = m_crsRd.rd_ylen - pt2.y;
	light[6].CreateLight(pt1, pt2);	// ��

	pt1.x += m_crsRd.rd_rnm*m_crsRd.rd_wid;
	light[7].CreateLight(pt1, pt2);	// ��

	LIGHT[3] = pt1.y;

	for (int i = 0; i < 8; ++i)
		m_Light.push_back(light[i]);
#endif

	SetTimer(2, CAR_ELAPSE_TIME, NULL);
	SetTimer(3, CLOCK_ELAPSE_TIME, NULL);
	SelecetState(3);	// ��ʼ״̬
}

void CVS2013_MO_DEMOView::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1){
		for (unsigned int i = 0; i < m_trackPtsNum; ++i){ // ������ * ����̬��
			if (m_curPtIndex[i] < m_trackPts[i].GetCount()){
				CMoPoint pt(m_trackPts[i].Item(COleVariant(m_curPtIndex[i])));
				m_curGeoEvent[i].MoveTo(pt.GetX(), pt.GetY()); // �¼����ƶ���
				m_curPtIndex[i]++;
			}
		}
		CFormView::OnTimer(nIDEvent);
	}

	else if (nIDEvent == 2){
		// ��������״̬���˴����ܶ�
		for (unsigned i = 0; i < m_Car.size(); ++i){
			if (m_Car.at(i).flagEnd)
				m_Car.at(i).Disappear();
			else{
				m_Car.at(i).LightState();	// ��ִ����λ�ж�
				m_Car.at(i).Bump();			// ��ײ�ж�
				if (m_Car.at(i).flagStop)
					m_Car.at(i).Stop();
				else
					m_Car.at(i).Move();
			}
		}

		// ��
		/*for (unsigned i = 0; i < m_Light.size(); ++i){
			m_Light.at(i).ChangeLightColor();
			}*/
		CFormView::OnTimer(nIDEvent);
	}

	// ģ����ʵʱ��
	else if (nIDEvent == 3){

#ifndef COMMENT_LINE3
		// ģ�����Ͷ��
		Car car;
		
		int rdNum = rand() % 20 + 1;
		int tmp = rdNum % 10;
		if (tmp == 2 || tmp == 4 || tmp == 7 || tmp == 9){

		}
		else{
			car.CreateCar(rdNum, 1);
			m_Car.push_back(car);
		}
#endif

		// ��λ������ʱ��仯�����������
		ClockState += CLOCK_ELAPSE_TIME / 1000.0;
		switch ((int(ClockState) / 10)%4)
		{
		case 0:
			SelecetState(3);
			break;
		case 1:
			SelecetState(2);
			break;
		case 2:
			SelecetState(1);
			break;
		case 3:
			SelecetState(4);
			break;
		default:
			break;
		}
		

		// ��� GeoEvent
		if (MoTrackingLayer.GetEventCount() > 1000)
			MoTrackingLayer.ClearEvents();

		CFormView::OnTimer(nIDEvent);
	}
}

void CVS2013_MO_DEMOView::OnTestStoptest()
{
	KillTimer(2);
	KillTimer(3);
}

/////////////////////////////��λ����/////////////////////////////////////////////
/*
*	��λ״̬ӳ�䵽ÿ�� Car ����� flagLightStop �������˱���+������̵Ƶ�λ���жϾ������Ƿ�ֹͣ
*	�Լ����̵Ƶ�״̬�����̵Ʊ����ʱ�룺12 56 43 87��
*/
void CVS2013_MO_DEMOView::SelecetState(int state)
{
	switch (state)	// state Ϊ��λ����
	{
	case 1:
		// ��
		for (unsigned i = 0; i < m_Car.size(); ++i){
			//int tmp = m_Car.at(i).lneNum;
			//if ((tmp >= 11 && tmp <= 20))
			//	m_Car.at(i).flagLightStop = FALSE;
			//else
			//	m_Car.at(i).flagLightStop = TRUE;
			//if (m_Car.at(i).state != 1)
			//	m_Car.at(i).flagLightStop = TRUE;	// ��λ�߼� =_=
			if (m_Car.at(i).state == 1)
				m_Car.at(i).flagLightStop = FALSE;	// ��λ�߼� =_=
			else
				m_Car.at(i).flagLightStop = TRUE;	// ��λ�߼� =_=
		}

		// �ƣ�6 7�� ����죩
		for (unsigned i = 0; i < m_Light.size(); ++i){
			if (i == 5 || i == 6) m_Light.at(i).ChangeLightColor(short(1));
			else m_Light.at(i).ChangeLightColor(short(0));
		}

		break;
	case 2:
		// ����1 5 6 10��
		for (unsigned i = 0; i < m_Car.size(); ++i){
			/*int tmp = m_Car.at(i).lneNum;
			if (tmp == 1 || tmp == 5 || tmp == 6 || tmp == 10)
			m_Car.at(i).flagLightStop = FALSE;
			else
			m_Car.at(i).flagLightStop = TRUE;*/
			if (m_Car.at(i).state == 2)
				m_Car.at(i).flagLightStop = FALSE;	// ��λ�߼� =_=
			else
				m_Car.at(i).flagLightStop = TRUE;	// ��λ�߼� =_=
		}

		// �ƣ�1 4�� ����죩
		for (unsigned i = 0; i < m_Light.size(); ++i){
			if (i == 0 || i == 3) m_Light.at(i).ChangeLightColor(short(1));
			else m_Light.at(i).ChangeLightColor(short(0));
		}

		break;
	case 3:
		// ��
		for (unsigned i = 0; i < m_Car.size(); ++i){
			//int tmp = m_Car.at(i).lneNum;
			//if ((tmp >= 1 && tmp <= 10))
			//	m_Car.at(i).flagLightStop = FALSE;
			//else
			//	m_Car.at(i).flagLightStop = TRUE;
			//if (m_Car.at(i).state != 3)
			//	m_Car.at(i).flagLightStop = TRUE;	// ��λ�߼� =_=
			if (m_Car.at(i).state == 3)
				m_Car.at(i).flagLightStop = FALSE;	// ��λ�߼� =_=
			else
				m_Car.at(i).flagLightStop = TRUE;	// ��λ�߼� =_=
		}

		// ��(2 3�� �����)
		for (unsigned i = 0; i < m_Light.size(); ++i){
			if (i == 1 || i == 2) m_Light.at(i).ChangeLightColor(short(1));
			else m_Light.at(i).ChangeLightColor(short(0));
		}

		break;
	case 4:
		// ����11 15 16 20��
		for (unsigned i = 0; i < m_Car.size(); ++i){
			//int tmp = m_Car.at(i).lneNum;
			//if (tmp == 11 || tmp == 15 || tmp == 16 || tmp == 20)
			//	m_Car.at(i).flagLightStop = FALSE;
			//else
			//	m_Car.at(i).flagLightStop = TRUE;
			//if (m_Car.at(i).state != 4)
			//	m_Car.at(i).flagLightStop = TRUE;	// ��λ�߼� =_=
			if (m_Car.at(i).state == 4)
				m_Car.at(i).flagLightStop = FALSE;	// ��λ�߼� =_=
			else
				m_Car.at(i).flagLightStop = TRUE;	// ��λ�߼� =_=
		}

		// �ƣ�5 8�� ����죩
		for (unsigned i = 0; i < m_Light.size(); ++i){
			if (i == 4|| i == 7) m_Light.at(i).ChangeLightColor(short(1));
			else m_Light.at(i).ChangeLightColor(short(0));
		}
		break;
	default:
		break;
	}
}


/////////////////////////////������ʵ��/////////////////////////////////////////////
Car::Car()
{
	flagStop = FALSE;
	flagEnd = FALSE;
	flagLightStop = FALSE;
}

Car::~Car()
{

}

void Car::CreateCar(int lNum, double dis)
{
	lneNum = lNum;
	lne = MoTrackLine.at(lneNum - 1);

	// ��ʼ�������
	carInd = TotalCarNum;
	
	// �ҽ�
	pt.CreateDispatch(TEXT("MapObjects2.Point"));
	pt.SetX(0);
	pt.SetY(0);
	evnt = MoTrackingLayer.AddEvent(pt, rand() % 5);

	// �����߻�ȡ�㼯
	pts.CreateDispatch(TEXT("MapObjects2.Points"));
	stepDis = dis;
	divNum = int(lne.GetLength() / stepDis);

	lne.SetMeasuresAsLength();
	for (int i = 0; i < divNum; ++i)
		pts.Add(lne.ReturnPointEvents(stepDis*i).Item(COleVariant(long(0))));

	// ��ʼ�����λ��
	ptsInd = 0;
	startPos.x = pts.Item(COleVariant(long(ptsInd))).GetX();
	startPos.y = pts.Item(COleVariant(long(ptsInd))).GetY();
	evnt.MoveTo(startPos.x, startPos.y);
	++TotalCarNum;

	// ��λ ��lNum -> State��
	if ((lneNum >= 12 && lneNum <= 14) || (lneNum >= 17 && lneNum <= 19))
		state = 1;
	if ((lneNum >= 2 && lneNum <= 4) || (lneNum >= 7 && lneNum <= 9))
		state = 3;
	if (lneNum ==1 || lneNum == 5 || lneNum == 6 || lneNum == 10)
		state = 2;
	if (lneNum == 11 || lneNum == 15 || lneNum == 16 || lneNum == 20)
		state = 4;
}

void Car::Move()	// �жϳ���״̬+�ƶ�
{
	curPos = nexPos;
	if (ptsInd < divNum){
		nexPos.x = pts.Item(COleVariant(long(ptsInd))).GetX();
		nexPos.y = pts.Item(COleVariant(long(ptsInd))).GetY();
		++ptsInd;
	}
	else{
		curPos.x = INFX;
		curPos.y = INFY;
		nexPos.x = INFX;
		nexPos.y = INFY;
		flagEnd = TRUE;
	}

	// ���ýǶ� ???
	/*double ang = 180.0 / PI*atan((nexPos.y - curPos.y) / (nexPos.x - curPos.x));
	MoTrackingLayer.GetSymbol(carInd).SetRotation(ang);*/
	evnt.MoveTo(nexPos.x, nexPos.y);
}

void Car::LightState()
{
	// ��λ+��λ�õ��ж�
	if (flagLightStop){	// ��λ
		switch ((lneNum - 1) / 5)	// ��λ�õ��ж�
		{
		case 0:
			if (curPos.x < LIGHT[0] && curPos.x > LIGHT[0] - TEST_VALUE)
				flagStop = TRUE;
			else
				flagStop = FALSE;
			break;
		case 1:
			if (curPos.x > LIGHT[1] && curPos.x < LIGHT[1] + TEST_VALUE)
				flagStop = TRUE;
			else
				flagStop = FALSE;
			break;
		case 2:
			if (curPos.y < LIGHT[2] && curPos.y > LIGHT[2] - TEST_VALUE)
				flagStop = TRUE;
			else
				flagStop = FALSE;
			break;
		case 3:
			if (curPos.y > LIGHT[3] && curPos.y < LIGHT[3] + TEST_VALUE)
				flagStop = TRUE;
			else
				flagStop = FALSE;
			break;
		default:
			break;
		}
	}
	else
		flagStop = FALSE;
}

BOOL Car::InOneWay(int l1, int l2)
{
	if (l1 == l2)	
		return TRUE;
	if (abs(l1 - l2) > 1)
		return FALSE;
	if (l1 % 5 == 3 || l2 % 5 == 3)
		return FALSE;
	return TRUE;
}

void Car::Bump()
{
	// ������ײ����
	if (flagStop == FALSE){
		double judNum = CAR_DEFAULT_DIS*CAR_DEFAULT_DIS;
		for (int i = 0; i < carInd; ++i){
			if (InOneWay(m_Car.at(i).lneNum, lneNum)){	// ��ʾ��ͬһ�ܵ���
				Pos tmp = m_Car.at(i).curPos;
				double res = (curPos.x - tmp.x)*(curPos.x - tmp.x) +
					(curPos.y - tmp.y)*(curPos.y - tmp.y);
				if (res <= judNum){
					flagStop = TRUE;
					break;
				}
			}
		}
	}
}

void Car::Stop()
{
	evnt.MoveTo(nexPos.x, nexPos.y);
}

void Car::Start()
{
	flagStop = FALSE;
}

void Car::Disappear()
{
	//MoTrackingLayer.RemoveEvent(carInd);
	evnt.MoveTo(INFX, INFY);	// ������ΪӦ���ܵ����ӡ����������ڴ��ͷŻ��Ʋ���Ϥ������==�Ƚ��Ϳ�
}

void Car::ChangeColor()
{
	evnt.SetSymbolIndex(rand() % CAR_COLOR_NUM);
}

/////////////////////////////���̵���/////////////////////////////////////////////
TrafficLight::TrafficLight()
{
}

TrafficLight::~TrafficLight()
{
}

void TrafficLight::CreateLight(Pos pt1, Pos pt2)
{
	// �����д���Ϊ���䣬����ƣ����ع���
	ptOri = pt1;
	ptEnd = pt2;
	// ���ö�λ��
	lne.CreateDispatch("MapObjects2.Line");
	CMoPoint pt;
	pt.CreateDispatch("MapObjects2.Point");
	CMoPoints pts;
	pts.CreateDispatch("MapObjects2.Points");

	pt.SetX(pt1.x);
	pt.SetY(pt1.y);
	pts.Add(pt);
	pt.SetX(pt2.x);
	pt.SetY(pt2.y);
	pts.Add(pt);

	lne.GetParts().Add(pts);

	// �¼�
	evnt = MoTrackingLayer.AddEvent(lne, rand() % LIGHT_COLOR_NUM + CAR_COLOR_NUM);
}

void TrafficLight::ChangeLightColor(short id)
{
//	evnt.SetSymbolIndex(rand() % LIGHT_COLOR_NUM + CAR_COLOR_NUM); // ���
	evnt.SetSymbolIndex(id + CAR_COLOR_NUM);					   // id����
}