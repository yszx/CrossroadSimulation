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
	// 设定几个颜色值
	m_color[0] = RGB(255, 0, 0);
	m_color[1] = RGB(0, 255, 0);
	m_color[2] = RGB(0, 0, 255);
	m_color[3] = RGB(255, 255, 0);
	m_color[4] = RGB(255, 0, 255);

	// 初始化道路结构数据
	m_crsRd.rd_wid = 3.5;
	m_crsRd.rd_lnm = 3;
	m_crsRd.rd_rnm = 3;
	m_crsRd.rd_xlen = 200;
	m_crsRd.rd_ylen = 200;
	m_crsRd.rd_sidwk_wid = 5;
	m_crsRd.rd_arc = m_crsRd.rd_sidwk_wid;

	// 开辟对象空间
	m_lyrNum = LAYER_NUM;
	m_dataCon = new CMoDataConnection[m_lyrNum];
	m_desc = new CMoTableDesc[m_lyrNum];
	m_layer = new CMoMapLayer[m_lyrNum];

	//// 测试
	//for (int i = 0; i < 10; ++i){
	//	m_number.Add("1");
	//	m_name.Add("100");
	//}
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
	// Map控件属性测试
	//m_map.SetAppearance(0); // 1-mo3D 0-moFlat
	m_map.SetBackColor(RGB(240,255,233));	// 参数实际上是RGB
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


// 调用 getTrackPoints 、initMap 函数
void CVS2013_MO_DEMOView::OnOpetationGettrackpoints()
{
	getTrackPoints();
	initMap();			// 将动态点（数据）挂接到 m_map 的 TrackingLayer 上面
}

// 响应 SetTimer
void CVS2013_MO_DEMOView::OnTimer(UINT_PTR nIDEvent)
{
	for (unsigned int i = 0; i < m_trackPtsNum; ++i){ // 挂载了 * 个动态点
		if (m_curPtIndex[i] < m_trackPts[i].GetCount()){
			CMoPoint pt(m_trackPts[i].Item(COleVariant(m_curPtIndex[i])));
			m_curGeoEvent[i].MoveTo(pt.GetX(), pt.GetY()); // 事件，移动点
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
		sym.SetColor(m_color[rand() % COLOR_NUM]);
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

	// 每一层的命名
	CMoGeoDataset geoDataset(m_dataCon[0].AddGeoDataset("中心线", moShapeTypeLine,
		(LPDISPATCH)m_desc[0], vt, vt));

	// 设置图层的属性及数据
	/// 第一层
	CMoLine   ln[4];	// 4 条线
	CMoPoints pts[2];	// 2 个多点
	CMoPoint  pt[4];	// 4 个单点
	for (int i = 0; i < 4; ++i)
		ln[i].CreateDispatch("MapObjects2.Line");
	for (int i = 0; i < 2; ++i)
		pts[i].CreateDispatch("MapObjects2.Points");
	for (int i = 0; i < 4; ++i)
		pt[i].CreateDispatch("MapObjects2.Point");

	// 线的数据设定（通过 Parts Points 对象设置）
	int wid = 2;	// 双黄线宽度
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

	ln[0].GetParts().Add(pts[0]);	// 第一条线段
	ln[1].GetParts().Add(pts[0]);	// 第二条线段
	ln[2].GetParts().Add(pts[1]);	// 第三条线段
	ln[3].GetParts().Add(pts[1]);	// 第四条线段
	ln[0].Offset(0, wid / 2);
	ln[1].Offset(0, -wid / 2);
	ln[2].Offset(wid / 2, 0);
	ln[3].Offset(-wid / 2, 0);


	// 往记录集写入数据
	m_layer[0].SetGeoDataset(geoDataset);
	CMoRecordset recs(m_layer[0].GetRecords());	// 图层的记录集
	for (int i = 0; i < 4; ++i){
		recs.AddNew();
		SetValue(recs.GetFields(), "Shape", ln[i]);
		recs.Update();
	}

	// 设置符号及刷新
	m_layer[0].GetSymbol().SetSize(2);
	m_layer[0].GetSymbol().SetColor(RGB(255, 251, 134));
	CMoLayers layers(m_map.GetLayers());
	layers.Add(m_layer[0]);
	m_map.Refresh();

	return TRUE;
}
