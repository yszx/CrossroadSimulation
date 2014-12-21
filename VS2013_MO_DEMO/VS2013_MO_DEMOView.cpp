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
END_MESSAGE_MAP()

// CVS2013_MO_DEMOView construction/destruction

CVS2013_MO_DEMOView::CVS2013_MO_DEMOView()
	: CFormView(CVS2013_MO_DEMOView::IDD)
{
	// TODO: add construction code here
	m_color[0] = RGB(255, 0, 0);
	m_color[1] = RGB(0, 255, 0);
	m_color[2] = RGB(0, 0, 255);
	m_color[3] = RGB(255, 255, 0);
	m_color[4] = RGB(255, 0, 255);

	// ����
	for (int i = 0; i < 10; ++i){
		m_number.Add("1");
		m_name.Add("100");
	}
}

CVS2013_MO_DEMOView::~CVS2013_MO_DEMOView()
{
	if (m_trackPts) delete[] m_trackPts;
	if (m_curPtIndex) delete[] m_curPtIndex;
	if (m_curGeoEvent) delete[] m_curGeoEvent;
}

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


// ���� getTrackPoints ��initMap ����
void CVS2013_MO_DEMOView::OnOpetationGettrackpoints()
{
	getTrackPoints();
	initMap();			// ����̬�㣨���ݣ��ҽӵ� m_map �� TrackingLayer ����
}

// ��Ӧ SetTimer
void CVS2013_MO_DEMOView::OnTimer(UINT_PTR nIDEvent)
{
	for (unsigned int i = 0; i < m_trackPtsNum; ++i){ // ������ * ����̬��
		if (m_curPtIndex[i] < m_trackPts[i].GetCount()){
			CMoPoint pt(m_trackPts[i].Item(COleVariant(m_curPtIndex[i])));
			m_curGeoEvent[i].MoveTo(pt.GetX(), pt.GetY()); // �¼����ƶ���
			m_curPtIndex[i]++;
		}
	}
	CFormView::OnTimer(nIDEvent);
}

void CVS2013_MO_DEMOView::OnOpetationStart()
{
	SetTimer(1, 50, NULL);
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
		sym.SetColor(m_color[rand() % COLOR_NUM]);
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
	createNewLayer();
	//// ��ӵ����ݵ�����
	//CMoPoint pt[10];
	//for (int i = 0; i < 10; ++i){
	//	pt[i].SetX(10 * i);
	//	pt[i].SetY(10 * i);
	//	m_pointArray.Add(pt[i]);
	//}

	//// �������Ա� shape moPoint ò�Ʋ��ܱ���������Ч
	///*CMoTableDesc tbl;
	//tbl.CreateDispatch(TEXT("MapObjects2.TableDesc"));
	//tbl.SetFieldCount(1);
	//tbl.SetFieldName(0, TEXT("shape"));
	//tbl.SetFieldType(0, 21);*/
	//CMoTableDesc desc;
	//desc.CreateDispatch(TEXT("MapObjects2.TableDesc"));

	//// ����ͼ�㡢��¼�����������
	//CMoMapLayer lyr;
	//lyr.CreateDispatch(TEXT("MapObjects2.MapLayer"));
	//CMoRecordset rec(lyr.GetRecords());
	//rec.SetTableDesc(desc);
	////rec.Update();

	//for (int i = 0; i < m_pointArray.GetSize(); ++i){
	//	rec.AddNew();
	//	CMoFields flds =  rec.GetFields();	// ��ʱ flds Ϊ�գ��޷�д��
	//	SetValue(rec.GetFields(), TEXT("shape"), m_pointArray[i]);
	//	rec.Update();
	//}

	//// ͼ�㵽 m_map 
	//lyr.SetRecords(rec);
	//m_map.SetLayers(lyr);
	//m_map.Refresh();
	////// ����ͼ�㡢��¼�����������
	////CMoMapLayer lyr;
	////CMoRecordset rec; // (lyr.GetRecords());
	//////rec.SetAutoFlush(FALSE);			// ÿ���޸��Ƿ���������Զ�����

	////CMoFields flds;
	////CMoField fld;
	////fld.SetName(TEXT("shape")); // ��������
	////fld.SetType(moPoint);		// ��������

	////int rec_num = rec.GetCount();
	////rec.AddNew();
	////rec_num = rec.GetCount();
	////

	////for (int i = 0; i < m_pointArray.GetSize(); ++i){
	////	rec.AddNew();
	////	CMoFields flds =  rec.GetFields();	// ��ʱ flds Ϊ�գ��޷�д��
	////	SetValue(rec.GetFields(), TEXT("shape"), m_pointArray[i]);
	////	rec.Update();
	////}
	////m_map.SetLayers(lyr);
}

BOOL CVS2013_MO_DEMOView::createNewLayer()
{
	if (!pMoCon.CreateDispatch(TEXT("MapObjects2.DataConnection")))
		return FALSE;
	if (!desc.CreateDispatch("MapObjects2.TableDesc"))
		return FALSE;
	VARIANT vt;
	vt.vt = VT_BOOL;
	vt.boolVal = VARIANT_FALSE;
	pMoCon.SetDatabase((LPCTSTR)"F:\\�����\\MO\\����\\ArcInfo\\test"); //����ͼ�㱣��Ŀ¼
	if (!pMoCon.Connect())
		return FALSE;

	// ����ͼ����ֶθ��������ԣ���������������һ��ΪNumber��һ��ΪName����ʾ��ĺ��뼰���ơ�
	desc.SetFieldCount(2);
	desc.SetFieldName(0, "Number"); // ����
	desc.SetFieldType(0, moDouble); // ���
	desc.SetFieldLength(0, 10);		// ����
	desc.SetFieldPrecision(0, 10);	// λ������ֵ��
	desc.SetFieldScale(0, 5);		// С��λ��

	desc.SetFieldName(1, "Name");
	desc.SetFieldType(1, moDouble);
	desc.SetFieldLength(1, 10);
	desc.SetFieldPrecision(1, 10);	
	desc.SetFieldScale(1, 5);		

	// ������ͼ��,�������ơ����ͼ��ֶΡ�
	CMoGeoDataset geoDataset(pMoCon.AddGeoDataset("����_��", moShapeTypePoint,
		(LPDISPATCH)desc, vt, vt));
	if (!layer.CreateDispatch(TEXT("MapObjects2.MapLayer")))
		return FALSE;

	// ����ͼ��ļ�¼��
	//CMoPoint* pt = new CMoPoint[10];
	CMoPoint pt[10];
	for (int i = 0; i < 10; ++i){
		pt[i].CreateDispatch("MapObjects2.Point");
		//CMoPoint pt_tmp = m_map.ToMapPoint(i, i);
		pt[i].SetX(float(10 * (i + 1)));	// �������������
		pt[i].SetY(float(10 * (i + 1)));
		m_pointArray.Add(pt[i]);
	}

	layer.SetGeoDataset(geoDataset);
	CMoRecordset recs(layer.GetRecords());

	// ��������
	VARIANT test_idx;
	VariantInit(&test_idx);
	test_idx.vt = VT_I4;

	long edit_sta = recs.GetEditMode();
	for (int i = 0; i < m_pointArray.GetSize(); ++i){
		recs.AddNew();
		edit_sta = recs.GetEditMode();
		SetValue(recs.GetFields(), "Shape", pt[i]);
		SetValue(recs.GetFields(), "Number", 1.0);
		SetValue(recs.GetFields(), "Name", 2.0);

		recs.Update();
	}
	

	//����
	test_idx.lVal = 0;
	edit_sta = recs.GetEditMode();
	long rec_cnt = recs.GetCount();
	CMoField fld(recs.GetFields().Item(test_idx));
	CString rec_nm = fld.GetName();

	long test_tpe = layer.GetShapeType();


	//���õ����ɫ���������ͼ��
	layer.GetSymbol().SetColor(moRed);
	layer.GetSymbol().SetSize(10);
	CMoLayers layers(m_map.GetLayers());
	layers.Add(layer);

	m_map.Refresh();
	return TRUE;
}
