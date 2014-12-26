/* 2014.10.17
*  说明：很多代码（注释）是测试代码，原稿不予删除。
* 1. CMoLine函数SetMeasuresAsLength 的作用是：
Calculates a measure value for every vertex on a Line object based
on the distance of the vertex from the start of the Line.

* 2. lineTrack m_trackPts 均为new的指针，均需使用 delete

* 3. //  [0:09//12/18/2014 guojianzhu]
	最基本的框架搭建完成，接下来的任务是：自定义符号（车子）、涉及多辆车子如何映射成几何对象（第一问）
	（第二问）画人行道，涉及：道路重新绘制、行人映射几何对象、行人车辆速度控制
	（第三问）红绿灯：红绿灯绘制、车辆加减速模拟
	（第四问）转向如何控制还不清楚

* 4. 研究自动化画十字路口 

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
#define COMMENT_LINE2		// 手动投放
//#define COMMENT_LINE3		// 随机投放


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
	// Map控件属性测试
	//m_map.SetAppearance(0); // 1-mo3D 0-moFlat
	m_map.SetBackColor(RGB(240,255,233));	// 参数实际上是RGB
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

// 此函数已调试过
void CVS2013_MO_DEMOView::OnFileOpen()
{
	// TODO: Add your command handler code here
	CString filter(TEXT("矢量地图文件 (*.shp)|*.shp|栅格地图文件(*.jpg)|*.jpg|"));
	CFileDialog dlg(TRUE, TEXT(".shp|*.jpg"), 0, OFN_ALLOWMULTISELECT |
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, filter);
	if (dlg.DoModal() == IDOK)
	{
		POSITION pos1 = dlg.GetStartPosition();//开始遍历所有多选的文件路径
		while (pos1)//添加多个图层
		{
			CString pathName = dlg.GetNextPathName(pos1);
			if ((!pathName.Right(3).Compare("shp")) || (!pathName.Right(3).Compare("SHP")))
				AddLayer(m_map, pathName);
			else  AddImageLayer(m_map, pathName);
		}
		m_map.Refresh();
	}	
	
	showRoad();	// 显示道路，函数里的参数，可以修改
}
BEGIN_EVENTSINK_MAP(CVS2013_MO_DEMOView, CFormView)
	ON_EVENT(CVS2013_MO_DEMOView, IDC_MAP1, DISPID_MOUSEDOWN, CVS2013_MO_DEMOView::MouseDownMap1, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CVS2013_MO_DEMOView, IDC_MAP1, 4, CVS2013_MO_DEMOView::AfterTrackingLayerDrawMap1, VTS_I4)
END_EVENTSINK_MAP()


// MouseDown 消息
//void CVS2013_MO_DEMOView::MouseDownMap1(short Button, short Shift, long X, long Y)
//{
//	// TODO: Add your message handler code here
//	//  [12/12/2014 guojianzhu]
//	//MessageBox("测试", 0, MB_OK | MB_ICONASTERISK);
//
//	//  [12/12/2014 guojianzhu]
//	// Map 控件方法 pan，来自 帮助文档
//	if (Shift == 0){
//		CMoRectangle rec = m_map.TrackRectangle();
//		if (rec)
//			m_map.SetExtent(rec);
//	}
//	else
//		m_map.Pan();	// MapControl控件对象的一个函数就把这个功能全封装了
//
//	// *MapDistance 方法Z
//	//float test = m_map.FromMapDistance(1000);
//
//	// SetCoorinateSystem 方法
//	VARIANT va = m_map.GetCoordinateSystem();
//	CMoRectangle bor = m_map.GetExtent();	// 只能追踪到指针地址，无法获取具体的值
//	//m_map.SetRotationAngle(30);
//
//	// 获取空间坐标
//	pt = m_map.ToMapPoint(float(X), float(Y));
//
//	// 激活 ** 事件
//	CMoRectangle rec(m_map.GetFullExtent());
//	VARIANT var;
//	VariantInit(&var);
//	va.vt = VT_DISPATCH;
//	va.pdispVal = rec.m_lpDispatch;
//	CMoTrackingLayer tLayer(m_map.GetTrackingLayer());
//	tLayer.Refresh(TRUE, var); // 应该就是这个激活了 AfterTrackingLayerDrawMap1 函数
//}
void CVS2013_MO_DEMOView::MouseDownMap1(short Button, short Shift, long X, long Y)
{

}

void CVS2013_MO_DEMOView::AfterTrackingLayerDrawMap1(long hDC)
{
	// TODO: Add your message handler code here
	// 设置字体
	CMoFont font;
	font.SetName("宋体");
	
	// 设置文本符号对象
	CMoTextSymbol m_Textsymbol;
	if (!m_Textsymbol.CreateDispatch("MapObjects2.TextSymbol")) return;
	m_Textsymbol.SetFont(font);

	// 注记
	m_map.DrawText("MO_GIS", pt, m_Textsymbol);
}

void CVS2013_MO_DEMOView::usingMoSymbol(CMoMapLayer m_MapLayer, COLORREF r, int sz, int style)
{
	CMoSymbol layerSymbol(m_MapLayer.GetSymbol());	// 创建一个新的符号对象
	// 1. 判定图层的几何类型
	//// 开始求解数据的几何类型
	long m_ShapeType = m_MapLayer.GetShapeType();

	// 0、1、2 依次为点线面
	if (m_ShapeType == 21 || m_ShapeType == 24)
		layerSymbol.SetSymbolType(0);
	else if (m_MapLayer.GetShapeType() == 22)
		layerSymbol.SetSymbolType(1);
	else
		layerSymbol.SetSymbolType(2);

	// 2.设置符号的属性
	//// 设置符号颜色
	layerSymbol.SetColor(r);

	//// 设置符号大小
	layerSymbol.SetSize(sz);

	// 3. 设置符号类型
	// 点线面符号设置
	layerSymbol.SetStyle(style);
}

void CVS2013_MO_DEMOView::showRoad()
{
	//  [12/17/2014 guojianzhu]
	// 地图符号设置测试
	// 为什么没有效果----已解决，m_map控件变量未刷新

	/*VARIENT 变量的初始化*/
	VARIANT index;
	VariantInit(&index);
	index.vt = VT_I4;		// 该结构的变量类型为整型

	CMoLayers lyrs = m_map.GetLayers();
	CMoMapLayer lyr;

	// 将道路图层放到最底
	for (int i = 0; i < lyrs.GetCount(); ++i){
		index.lVal = i;				// 第几个图层索引
		lyr = lyrs.Item(index);		// 第几个图层
		if (lyr.GetName() == "road_buf2")
			lyrs.MoveToBottom(i);
	}

	// 设置图层要素的属性
	for (int i = 0; i < lyrs.GetCount(); ++i){
		index.lVal = i;				// 第几个图层索引
		lyr = lyrs.Item(index);		// 第几个图层

		if (lyr.GetName() == "road_buf2")
			usingMoSymbol(lyr, RGB(110, 110, 110), 2, moSolidFill);	// 面
		else if (lyr.GetName() == "road"){
			usingMoSymbol(lyr, RGB(255, 255, 255), 2, moDashLine);	// 道路中心线
			lyr.SetVisible(FALSE); // 道路中心线设置为不可见
		}
		else if (lyr.GetName() == "road_center")
			usingMoSymbol(lyr, RGB(255, 255, 130), 4, moSolidLine);	// 道路分割线
	}
	m_map.Refresh();

	//CMoMapLayer lyr = m_map.GetLayers().Item(index);	// 获取第一图层
	//usingMoSymbol(lyr, RGB(110, 110, 110),2);			// 设置图层颜色
	//m_map.Refresh();


	//CString lyr_name = lyr.GetName();
	//CMoSymbol lyrSym(lyr.GetSymbol());								// 创建新的符号对象

	//if (lyr.GetShapeType() == 22){	// 图层为线状要素
	//	lyrSym.SetSymbolType(1);	// 符号对象设置为线状
	//	//unsigned long color_test = lyrSym.GetColor();	// 获取颜色
	//	//unsigned long color_test2 = RGB(255, 100, 10);
	//	lyrSym.SetColor(RGB(255, 0, 0));				// 设置颜色
	//	lyrSym.SetStyle(moDashLine);
	//	m_map.Refresh();
	//}
}

void CVS2013_MO_DEMOView::getTrackPoints()
{
	CMoMapLayer lyr(m_map.GetLayers().Item(COleVariant("road")));	// road 图层
	CMoRecordset rec(lyr.GetRecords());	// road 图层记录集
	int lineNum = rec.GetCount();		// 记录条数 <-> 线数目
	m_trackPtsNum = lineNum;			// 赋值给类的成员（后来补充）
	CMoLine* lineTrack = new CMoLine[lineNum];	// CMoLine 数组
	unsigned short lineIndex = 0;		// 索引

	// 获取 road 图层线数组
	rec.MoveFirst();
	while (lineIndex < lineNum){
		CMoField lineFld(rec.GetFields().Item(COleVariant("shape")));	
										// CMoField 记录集，用来得到线的数据
		CMoLine lineTmp(lineFld.GetValue().pdispVal);	
		lineTrack[lineIndex] = lineTmp;

		rec.MoveNext();
		++lineIndex;
	}

	// 等分线获取等分点
	m_trackPts = new CMoPoints[lineNum];
	for (int i = 0; i < lineNum;++i)
		m_trackPts[i].CreateDispatch(TEXT("MapObjects2.Points"));

	int divNum = 200;					 // 等分数目
	double* step = new double[lineNum] ; // 等分距离
	long nFirstPt = 0;					 // 一直为0
	for (int i = 0; i < lineNum; ++i){
		lineTrack[i].SetMeasuresAsLength();
		step[i] = lineTrack[i].GetLength() / divNum;
	}

	//// 测试代码
	//CMoField lineFld(rec.GetFields().Item(COleVariant("shape")));
	//rec.MoveFirst();
	//CMoLine lineTmp(lineFld.GetValue().pdispVal);
	//lineTmp.SetMeasuresAsLength(); // 此行代码未添加--导致bug
	//CMoPoints ptsTmp;

	//for (int i = 0; i < lineNum; ++i){
	//	ptsTmp = lineTmp.ReturnPointEvents(step[0] * i);
	//	m_test.Add(ptsTmp.Item(COleVariant(nFirstPt)));
	//}
	//// 测试代码

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

// 调用 getTrackPoints 、initMap 函数
void CVS2013_MO_DEMOView::OnOpetationGettrackpoints()
{
	getTrackPoints();
	initMap();			// 将动态点（数据）挂接到 m_map 的 TrackingLayer 上面
}


void CVS2013_MO_DEMOView::OnOpetationStop()
{
	KillTimer(1);
}

// 使用 CMoTrackLayer 挂接动态数据
void CVS2013_MO_DEMOView::initMap()
{
	//  [12/18/2014 guojianzhu]
	srand(unsigned(time(0)));

	/* 设置 TrackingLayer 的符号 */
	CMoTrackingLayer tLyr = m_map.GetTrackingLayer();	// 获取 trackinglayer
	tLyr.SetSymbolCount(m_trackPtsNum);					// 设置符号个数,个数与 
														//CMoGeoEvent 的 addEvent 第二个参数对应

	// CMoFont 字体设置
	CMoFont fnt;
	fnt.InitializeFont();	// 类似 CreateDispatch
	//fnt.SetName(TEXT("ESRI Transportation & Civic"));	// 设置字体库
	fnt.SetName(TEXT("MoCars2"));	// 设置字体库

	// 字体->symbol
	for (unsigned int i = 0; i < m_trackPtsNum; ++i){
		CMoSymbol sym(tLyr.GetSymbol(i));
		sym.SetColor(CarColor[rand() % CAR_COLOR_NUM]);
		sym.SetStyle(moTrueTypeMarker);			// TrueType类型
		sym.SetFont(fnt.GetFontDispatch());
		sym.SetSize(60);
		sym.SetCharacterIndex(1);			// 设置字体库中字的编号值（十进制）

		// 逆时针旋转
		if (i == 1) sym.SetRotation(270);
		if (i == 2) sym.SetRotation(180);
		else if (i == 3) sym.SetRotation(90);
	}
	fnt.ReleaseFont();

	// 使用 CMoTrackLayer 挂接动态数据
	CMoPoint* pt = new CMoPoint[m_trackPtsNum];
	m_curGeoEvent = new CMoGeoEvent[m_trackPtsNum];
	m_curPtIndex = new long[m_trackPtsNum];
	for (unsigned int i = 0; i < m_trackPtsNum; ++i){
		pt[i].CreateDispatch("MapObjects2.Point");
		pt[i].SetX(0);
		pt[i].SetY(0);

		m_curGeoEvent[i] = m_map.GetTrackingLayer().AddEvent(pt[i], i); // 挂接
		m_curPtIndex[i] = 0;
	}
}


// 自动画十字路口道路
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
	//m_dataCon.SetDatabase((LPCTSTR)"F:\\程序库\\MO\\数据\\ArcInfo\\test"); //设置图层保存目录
	//if (!m_dataCon.Connect())
	//	return FALSE;

	//// 设置图层的字段个数及属性，这里设置两个，一个为Number，一个为Name。表示点的号码及名称。
	//m_desc.SetFieldCount(2);
	//m_desc.SetFieldName(0, "Number"); // 名字
	//m_desc.SetFieldType(0, moDouble); // 类别
	//m_desc.SetFieldLength(0, 10);		// 长度
	//m_desc.SetFieldPrecision(0, 10);	// 位数（数值）
	//m_desc.SetFieldScale(0, 5);		// 小数位数

	//m_desc.SetFieldName(1, "Name");
	//m_desc.SetFieldType(1, moDouble);
	//m_desc.SetFieldLength(1, 10);
	//m_desc.SetFieldPrecision(1, 10);	
	//m_desc.SetFieldScale(1, 5);		

	//// 创建新图层,设置名称、类型及字段。
	//CMoGeoDataset geoDataset(m_dataCon.AddGeoDataset("测试_点", moShapeTypePoint,
	//	(LPDISPATCH)m_desc, vt, vt));
	//if (!m_layer.CreateDispatch(TEXT("MapObjects2.MapLayer")))
	//	return FALSE;

	//// 设置图层的记录集
	////CMoPoint* pt = new CMoPoint[10];
	//CMoPoint pt[10];
	//for (int i = 0; i < 10; ++i){
	//	pt[i].CreateDispatch("MapObjects2.Point");
	//	//CMoPoint pt_tmp = m_map.ToMapPoint(i, i);
	//	pt[i].SetX(float(10 * (i + 1)));	// 点数据随便设置
	//	pt[i].SetY(float(10 * (i + 1)));
	//	m_pointArray.Add(pt[i]);
	//}

	//m_layer.SetGeoDataset(geoDataset);
	//CMoRecordset recs(m_layer.GetRecords());

	//// 索引变量
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

	////测试
	//test_idx.lVal = 0;
	//edit_sta = recs.GetEditMode();
	//long rec_cnt = recs.GetCount();
	//CMoField fld(recs.GetFields().Item(test_idx));
	//CString rec_nm = fld.GetName();

	//long test_tpe = m_layer.GetShapeType();


	////设置点的颜色，并加入地图。
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
			CMoPoint pt(pts.Item(COleVariant(long(i))));	// pt 是新创建的对象
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

/////////////////////////////关键函数/////////////////////////////////////////////
CVS2013_MO_DEMOView::CVS2013_MO_DEMOView()
: CFormView(CVS2013_MO_DEMOView::IDD)
{
	// 初始化道路结构数据
	m_crsRd.rd_wid = 4;
	m_crsRd.rd_lnm = 3;
	m_crsRd.rd_rnm = 3;
	m_crsRd.rd_xlen = 200;
	m_crsRd.rd_ylen = 200;
	m_crsRd.rd_sidwk_wid = 5;
	m_crsRd.rd_arc = m_crsRd.rd_sidwk_wid;
	m_crsRd.rd_cen_wid = 1.5;

	// 开辟对象空间
	m_lyrNum = LAYER_NUM;
	m_dataCon = new CMoDataConnection[m_lyrNum];
	m_desc = new CMoTableDesc[m_lyrNum];
	m_layer = new CMoMapLayer[m_lyrNum];

	// 时钟
	ClockState = 0;
}

CVS2013_MO_DEMOView::~CVS2013_MO_DEMOView()
{
	// 释放在构造函数里创建的空间
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

	// x y 为圆心 r 为半径
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
	mo_line[0].GetParts().Add(mo_pts[0]);		// 第一条轨迹

	mo_pt[1].SetX(m_crsRd.rd_xlen);
	mo_pt[1].SetY(y + r);
	for (int i = 0; i < 2; ++i)
		mo_pts[1].Add(mo_pt[i]);
	mo_line[1].GetParts().Add(mo_pts[1]);		// 第二条轨迹

	mo_line[2].GetParts().Add(mo_pts[1]);
	mo_line[2].Offset(0, m_crsRd.rd_wid);		// 第三条轨迹

	mo_line[3].GetParts().Add(mo_pts[1]);
	mo_line[3].Offset(0, m_crsRd.rd_wid * 2);	// 第四条轨迹

	// x 不变
	y = m_crsRd.rd_ylen - y; // x轴对称
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
	mo_line[4].GetParts().Add(mo_pts[4]);	// 第五条轨迹

	// 存储到容器中
	for (int i = 0; i < 5; ++i)
		MoTrackLine.push_back(mo_line[i]);

	// 中心对称变换
	for (int i = 0; i < 5; ++i)
		MoTrackLine.push_back(centroSymmetric(mo_line[i], m_crsRd.rd_xlen / 2, m_crsRd.rd_ylen / 2));

	// 旋转平移变换
	for (int i = 0; i < 5; ++i)
		MoTrackLine.push_back(rotoffSymmetric(mo_line[i], m_crsRd.rd_xlen, 0, 90));
	for (int i = 0; i < 5; ++i)
		MoTrackLine.push_back(rotoffSymmetric(mo_line[i], 0, m_crsRd.rd_ylen, 270));

	return TRUE;
}

CMoLine CVS2013_MO_DEMOView::rotoffSymmetric(CMoLine& l, float offX, float offY, double ang)
{
	// 计算方便，对角度进行变换
	ang = ang*PI / 180.0;

	CMoLine mo_line;
	mo_line.CreateDispatch("MapObjects2.Line");

	CMoParts pats = l.GetParts();
	for (int j = 0; j < pats.GetCount(); ++j){
		CMoPoints pts(pats.Item(COleVariant(long(j))));
		CMoPoints ptsTmp;
		ptsTmp.CreateDispatch("MapObjects2.Points");
		for (int i = 0; i < pts.GetCount(); ++i){
			CMoPoint pt(pts.Item(COleVariant(long(i))));	// pt 是新创建的对象
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
	// 从底层创建相关 MO 对象
	for (unsigned int i = 0; i < m_lyrNum; ++i){
		if (!m_dataCon[i].CreateDispatch(TEXT("MapObjects2.DataConnection")))
			return FALSE;
		if (!m_desc[i].CreateDispatch("MapObjects2.TableDesc"))
			return FALSE;
		if (!m_layer[i].CreateDispatch("MapObjects2.MapLayer"))
			return FALSE;
	}

	// 连接数据
	VARIANT vt;
	vt.vt = VT_BOOL;
	vt.boolVal = VARIANT_FALSE;
	for (unsigned int i = 0; i < m_lyrNum; ++i){
		m_dataCon[i].SetDatabase((LPCTSTR)"F:\\程序库\\MO\\数据\\ArcInfo\\十字路口"); //设置图层保存目录
		if (!m_dataCon[i].Connect())
			return FALSE;
	}

	// 命名每一层的数据文件名
	CMoGeoDataset geoDataset[LAYER_NUM];
	geoDataset[0] = m_dataCon[0].AddGeoDataset("中心线_双黄线", moShapeTypeLine,
		(LPDISPATCH)m_desc[0], vt, vt);
	geoDataset[1] = m_dataCon[1].AddGeoDataset("道路", moShapeTypeLine,
		(LPDISPATCH)m_desc[1], vt, vt);
	geoDataset[2] = m_dataCon[2].AddGeoDataset("人行道", moPolygon,
		(LPDISPATCH)m_desc[2], vt, vt);
	geoDataset[3] = m_dataCon[3].AddGeoDataset("外围", moPolygon,
		(LPDISPATCH)m_desc[3], vt, vt);

	// 设置图层的属性及数据
	/// 第一层
	CMoLine   lnCntre[4];			// 4 条线
	CMoPoints pts[2], pts2[2];	// 2 个多点(第一层第二层的点)
	CMoPoint  pt[4];			// 4 个单点
	for (int i = 0; i < 4; ++i)
		lnCntre[i].CreateDispatch("MapObjects2.Line");
	for (int i = 0; i < 2; ++i){
		pts[i].CreateDispatch("MapObjects2.Points");
		pts2[i].CreateDispatch("MapObjects2.Points");
	}
	for (int i = 0; i < 4; ++i)
		pt[i].CreateDispatch("MapObjects2.Point");

	// 线的数据设定（通过 Parts Points 对象设置）
	float wid = m_crsRd.rd_cen_wid;	// 双黄线宽度
	pt[0].SetX(0);
	pt[0].SetY(m_crsRd.rd_ylen / 2);
	pt[1].SetX(m_crsRd.rd_xlen);
	pt[1].SetY(m_crsRd.rd_ylen / 2);
	pt[2].SetX(m_crsRd.rd_xlen / 2);
	pt[2].SetY(0);
	pt[3].SetX(m_crsRd.rd_xlen / 2);
	pt[3].SetY(m_crsRd.rd_ylen);


	pts[0].Add(pt[0]);
	pts[0].Add(pt[1]);	// 点串
	pts[1].Add(pt[2]);
	pts[1].Add(pt[3]);	// 点串

	lnCntre[0].GetParts().Add(pts[0]);	// 第一条线段
	lnCntre[1].GetParts().Add(pts[0]);	// 第二条线段
	lnCntre[2].GetParts().Add(pts[1]);	// 第三条线段
	lnCntre[3].GetParts().Add(pts[1]);	// 第四条线段
	lnCntre[0].Offset(0, wid / 2);
	lnCntre[1].Offset(0, -wid / 2);
	lnCntre[2].Offset(wid / 2, 0);
	lnCntre[3].Offset(-wid / 2, 0);

	/// 第二层
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
	pts2[0].Add(pt[1]);	// 点串
	pts2[1].Add(pt[2]);
	pts2[1].Add(pt[3]);	// 点串

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

	/// 第三层（人行道）
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


	/// 第四层
	CMoPoint ptBord[204];
	CMoPoints ptsBord[4];
	CMoPolygon poBord[4];
	for (int i = 0; i < 204; ++i)
		ptBord[i].CreateDispatch("MapObjects2.Point");
	for (int i = 0; i < 4; ++i)
		ptsBord[i].CreateDispatch("MapObjects2.Points");
	for (int i = 0; i < 4; ++i)
		poBord[i].CreateDispatch("MapObjects2.Polygon");

	// x y 为圆心
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

	/// 镜像对称
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


	// 往记录集写入数据
	for (unsigned int i = 0; i < m_lyrNum; ++i){
		m_layer[i].SetGeoDataset(geoDataset[i]);
	}
	CMoRecordset recs(m_layer[0].GetRecords());	// 第一图层的记录集
	for (int i = 0; i < 4; ++i){
		recs.AddNew();
		SetValue(recs.GetFields(), "Shape", lnCntre[i]);
		recs.Update();
	}
	recs = m_layer[1].GetRecords();	// 第二图层的记录集
	for (int i = 0; i < dashRdNum; ++i){
		recs.AddNew();
		SetValue(recs.GetFields(), "Shape", lnDash[i]);
		recs.Update();
	}
	recs = m_layer[2].GetRecords();	// 第三图层的记录集
	for (int i = 0; i < walkRdNum; ++i){
		recs.AddNew();
		SetValue(recs.GetFields(), "Shape", poWalk[i]);
		recs.Update();
	}
	recs = m_layer[3].GetRecords();	// 第四图层的记录集
	for (int i = 0; i < 4; ++i){
		recs.AddNew();
		SetValue(recs.GetFields(), "Shape", poBord[i]);
		recs.Update();
	}
	// 设置符号及刷新
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

	MoTrackingLayer = m_map.GetTrackingLayer();	// 获取 MoTrackingLayer，使用全局变量存储
	MoTrackingLayer.SetSymbolCount(carSymNum+lightSymNum);	

	// 设置车符号种类
	for (int i = 0; i < carSymNum;++i){
		CMoSymbol sym(MoTrackingLayer.GetSymbol(long(i)));
		
		CMoFont fnt;
		fnt.InitializeFont();			// 类似 CreateDispatch
		fnt.SetName(TEXT("MoCars2"));	// 设置字体库

		sym.SetColor(CarColor[i]);
		sym.SetStyle(moTrueTypeMarker);				// TrueType类型
		sym.SetFont(fnt.GetFontDispatch());
		sym.SetSize(CAR_DEFAULT_SIZE);
		sym.SetCharacterIndex(1);					// 设置字体库中字的编号值（十进制）
		fnt.ReleaseFont();
	}

	// 设置红绿灯种类
	for (int i = carSymNum; i < carSymNum + lightSymNum; ++i){
		CMoSymbol sym(MoTrackingLayer.GetSymbol(long(i)));
		sym.SetSymbolType(1);  // 符号默认为 1 （点状）
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

void CVS2013_MO_DEMOView::OnTestStarttest()	// 暂时调用所有测试函数
{
	createCrossRoad();
	initTrakingLayer();
	getTrackLine();

#ifndef COMMENT_LINE2
	// 创建车辆
	for (int i = 6; i <= 6; ++i){
		Car car;
		car.CreateCar(i, (rand() % 15 + 10) / 10.0);
		m_Car.push_back(car);
	}
#endif

#ifndef COMMENT_LINE
	// 创建灯
	TrafficLight light[8];
	Pos pt1, pt2;
	float x = m_crsRd.rd_xlen / 2 - m_crsRd.rd_lnm*m_crsRd.rd_wid - m_crsRd.rd_sidwk_wid;
	float y = m_crsRd.rd_ylen / 2;
	pt1.x = x;
	pt1.y = y;
	pt2.x = x;
	pt2.y = y - m_crsRd.rd_wid;

	light[0].CreateLight(pt1, pt2);	// 一

	pt1.y -= m_crsRd.rd_rnm*m_crsRd.rd_wid;
	light[1].CreateLight(pt1, pt2);	// 二

	LIGHT[0] = pt1.x;

	pt1.x = m_crsRd.rd_xlen - pt1.x;
	pt1.y = m_crsRd.rd_ylen - pt1.y;
	pt2.x = m_crsRd.rd_xlen - pt2.x;
	pt2.y = m_crsRd.rd_ylen - pt2.y;
	light[2].CreateLight(pt1, pt2);	// 三

	pt1.y -= m_crsRd.rd_rnm*m_crsRd.rd_wid;
	light[3].CreateLight(pt1, pt2);	// 四

	LIGHT[1] = pt1.x;

	x = m_crsRd.rd_xlen / 2;
	y = m_crsRd.rd_ylen / 2 - m_crsRd.rd_lnm*m_crsRd.rd_wid - m_crsRd.rd_sidwk_wid;
	pt1.x = x;
	pt1.y = y;
	pt2.x = x + m_crsRd.rd_wid;
	pt2.y = y;
	light[4].CreateLight(pt1, pt2);	// 五

	pt1.x += m_crsRd.rd_rnm*m_crsRd.rd_wid;
	light[5].CreateLight(pt1, pt2);	// 六

	LIGHT[2] = pt1.y;

	pt1.x = m_crsRd.rd_xlen - pt1.x;
	pt1.y = m_crsRd.rd_ylen - pt1.y;
	pt2.x = m_crsRd.rd_xlen - pt2.x;
	pt2.y = m_crsRd.rd_ylen - pt2.y;
	light[6].CreateLight(pt1, pt2);	// 七

	pt1.x += m_crsRd.rd_rnm*m_crsRd.rd_wid;
	light[7].CreateLight(pt1, pt2);	// 八

	LIGHT[3] = pt1.y;

	for (int i = 0; i < 8; ++i)
		m_Light.push_back(light[i]);
#endif

	SetTimer(2, CAR_ELAPSE_TIME, NULL);
	SetTimer(3, CLOCK_ELAPSE_TIME, NULL);
	SelecetState(3);	// 初始状态
}

void CVS2013_MO_DEMOView::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1){
		for (unsigned int i = 0; i < m_trackPtsNum; ++i){ // 挂载了 * 个动态点
			if (m_curPtIndex[i] < m_trackPts[i].GetCount()){
				CMoPoint pt(m_trackPts[i].Item(COleVariant(m_curPtIndex[i])));
				m_curGeoEvent[i].MoveTo(pt.GetX(), pt.GetY()); // 事件，移动点
				m_curPtIndex[i]++;
			}
		}
		CFormView::OnTimer(nIDEvent);
	}

	else if (nIDEvent == 2){
		// 车的三个状态，此处不能动
		for (unsigned i = 0; i < m_Car.size(); ++i){
			if (m_Car.at(i).flagEnd)
				m_Car.at(i).Disappear();
			else{
				m_Car.at(i).LightState();	// 先执行相位判断
				m_Car.at(i).Bump();			// 碰撞判断
				if (m_Car.at(i).flagStop)
					m_Car.at(i).Stop();
				else
					m_Car.at(i).Move();
			}
		}

		// 灯
		/*for (unsigned i = 0; i < m_Light.size(); ++i){
			m_Light.at(i).ChangeLightColor();
			}*/
		CFormView::OnTimer(nIDEvent);
	}

	// 模拟真实时间
	else if (nIDEvent == 3){

#ifndef COMMENT_LINE3
		// 模拟随机投放
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

		// 相位控制随时间变化，代码块在这
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
		

		// 清除 GeoEvent
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

/////////////////////////////相位控制/////////////////////////////////////////////
/*
*	相位状态映射到每个 Car 对象的 flagLightStop 变量，此变量+车与红绿灯的位置判断决定车是否停止
*	以及红绿灯的状态（红绿灯编号逆时针：12 56 43 87）
*/
void CVS2013_MO_DEMOView::SelecetState(int state)
{
	switch (state)	// state 为相位代号
	{
	case 1:
		// 车
		for (unsigned i = 0; i < m_Car.size(); ++i){
			//int tmp = m_Car.at(i).lneNum;
			//if ((tmp >= 11 && tmp <= 20))
			//	m_Car.at(i).flagLightStop = FALSE;
			//else
			//	m_Car.at(i).flagLightStop = TRUE;
			//if (m_Car.at(i).state != 1)
			//	m_Car.at(i).flagLightStop = TRUE;	// 相位逻辑 =_=
			if (m_Car.at(i).state == 1)
				m_Car.at(i).flagLightStop = FALSE;	// 相位逻辑 =_=
			else
				m_Car.at(i).flagLightStop = TRUE;	// 相位逻辑 =_=
		}

		// 灯（6 7绿 其余红）
		for (unsigned i = 0; i < m_Light.size(); ++i){
			if (i == 5 || i == 6) m_Light.at(i).ChangeLightColor(short(1));
			else m_Light.at(i).ChangeLightColor(short(0));
		}

		break;
	case 2:
		// 车（1 5 6 10）
		for (unsigned i = 0; i < m_Car.size(); ++i){
			/*int tmp = m_Car.at(i).lneNum;
			if (tmp == 1 || tmp == 5 || tmp == 6 || tmp == 10)
			m_Car.at(i).flagLightStop = FALSE;
			else
			m_Car.at(i).flagLightStop = TRUE;*/
			if (m_Car.at(i).state == 2)
				m_Car.at(i).flagLightStop = FALSE;	// 相位逻辑 =_=
			else
				m_Car.at(i).flagLightStop = TRUE;	// 相位逻辑 =_=
		}

		// 灯（1 4绿 其余红）
		for (unsigned i = 0; i < m_Light.size(); ++i){
			if (i == 0 || i == 3) m_Light.at(i).ChangeLightColor(short(1));
			else m_Light.at(i).ChangeLightColor(short(0));
		}

		break;
	case 3:
		// 车
		for (unsigned i = 0; i < m_Car.size(); ++i){
			//int tmp = m_Car.at(i).lneNum;
			//if ((tmp >= 1 && tmp <= 10))
			//	m_Car.at(i).flagLightStop = FALSE;
			//else
			//	m_Car.at(i).flagLightStop = TRUE;
			//if (m_Car.at(i).state != 3)
			//	m_Car.at(i).flagLightStop = TRUE;	// 相位逻辑 =_=
			if (m_Car.at(i).state == 3)
				m_Car.at(i).flagLightStop = FALSE;	// 相位逻辑 =_=
			else
				m_Car.at(i).flagLightStop = TRUE;	// 相位逻辑 =_=
		}

		// 灯(2 3绿 其余红)
		for (unsigned i = 0; i < m_Light.size(); ++i){
			if (i == 1 || i == 2) m_Light.at(i).ChangeLightColor(short(1));
			else m_Light.at(i).ChangeLightColor(short(0));
		}

		break;
	case 4:
		// 车（11 15 16 20）
		for (unsigned i = 0; i < m_Car.size(); ++i){
			//int tmp = m_Car.at(i).lneNum;
			//if (tmp == 11 || tmp == 15 || tmp == 16 || tmp == 20)
			//	m_Car.at(i).flagLightStop = FALSE;
			//else
			//	m_Car.at(i).flagLightStop = TRUE;
			//if (m_Car.at(i).state != 4)
			//	m_Car.at(i).flagLightStop = TRUE;	// 相位逻辑 =_=
			if (m_Car.at(i).state == 4)
				m_Car.at(i).flagLightStop = FALSE;	// 相位逻辑 =_=
			else
				m_Car.at(i).flagLightStop = TRUE;	// 相位逻辑 =_=
		}

		// 灯（5 8绿 其余红）
		for (unsigned i = 0; i < m_Light.size(); ++i){
			if (i == 4|| i == 7) m_Light.at(i).ChangeLightColor(short(1));
			else m_Light.at(i).ChangeLightColor(short(0));
		}
		break;
	default:
		break;
	}
}


/////////////////////////////汽车类实现/////////////////////////////////////////////
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

	// 初始化车编号
	carInd = TotalCarNum;
	
	// 挂接
	pt.CreateDispatch(TEXT("MapObjects2.Point"));
	pt.SetX(0);
	pt.SetY(0);
	evnt = MoTrackingLayer.AddEvent(pt, rand() % 5);

	// 根据线获取点集
	pts.CreateDispatch(TEXT("MapObjects2.Points"));
	stepDis = dis;
	divNum = int(lne.GetLength() / stepDis);

	lne.SetMeasuresAsLength();
	for (int i = 0; i < divNum; ++i)
		pts.Add(lne.ReturnPointEvents(stepDis*i).Item(COleVariant(long(0))));

	// 初始化起点位置
	ptsInd = 0;
	startPos.x = pts.Item(COleVariant(long(ptsInd))).GetX();
	startPos.y = pts.Item(COleVariant(long(ptsInd))).GetY();
	evnt.MoveTo(startPos.x, startPos.y);
	++TotalCarNum;

	// 相位 （lNum -> State）
	if ((lneNum >= 12 && lneNum <= 14) || (lneNum >= 17 && lneNum <= 19))
		state = 1;
	if ((lneNum >= 2 && lneNum <= 4) || (lneNum >= 7 && lneNum <= 9))
		state = 3;
	if (lneNum ==1 || lneNum == 5 || lneNum == 6 || lneNum == 10)
		state = 2;
	if (lneNum == 11 || lneNum == 15 || lneNum == 16 || lneNum == 20)
		state = 4;
}

void Car::Move()	// 判断车的状态+移动
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

	// 设置角度 ???
	/*double ang = 180.0 / PI*atan((nexPos.y - curPos.y) / (nexPos.x - curPos.x));
	MoTrackingLayer.GetSymbol(carInd).SetRotation(ang);*/
	evnt.MoveTo(nexPos.x, nexPos.y);
}

void Car::LightState()
{
	// 相位+车位置的判断
	if (flagLightStop){	// 相位
		switch ((lneNum - 1) / 5)	// 车位置的判断
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
	// 检验碰撞代码
	if (flagStop == FALSE){
		double judNum = CAR_DEFAULT_DIS*CAR_DEFAULT_DIS;
		for (int i = 0; i < carInd; ++i){
			if (InOneWay(m_Car.at(i).lneNum, lneNum)){	// 表示在同一跑道上
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
	evnt.MoveTo(INFX, INFY);	// 此种行为应该受到鄙视。。。但对内存释放机制不熟悉，所以==先将就咯
}

void Car::ChangeColor()
{
	evnt.SetSymbolIndex(rand() % CAR_COLOR_NUM);
}

/////////////////////////////红绿灯类/////////////////////////////////////////////
TrafficLight::TrafficLight()
{
}

TrafficLight::~TrafficLight()
{
}

void TrafficLight::CreateLight(Pos pt1, Pos pt2)
{
	// 这两行代码为补充，非设计（待重构）
	ptOri = pt1;
	ptEnd = pt2;
	// 设置定位点
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

	// 事件
	evnt = MoTrackingLayer.AddEvent(lne, rand() % LIGHT_COLOR_NUM + CAR_COLOR_NUM);
}

void TrafficLight::ChangeLightColor(short id)
{
//	evnt.SetSymbolIndex(rand() % LIGHT_COLOR_NUM + CAR_COLOR_NUM); // 随机
	evnt.SetSymbolIndex(id + CAR_COLOR_NUM);					   // id控制
}