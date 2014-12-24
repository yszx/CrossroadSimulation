#ifndef ATRNS_SIMU_H
#define ATRNS_SIMU_H

#include <ctime>	// 用于计时和完全随机模拟
#include <cmath>
#include <vector>
using namespace std;

// 一些自定义常量和头文件
#define COLOR_NUM 5	// 颜色种类，颜色数组在 view 的构造函数里
#define LAYER_NUM 4	// 十字路口的图层数目
#define PI		  3.141592

static vector<CMoLine>		MoTrackLine;	// 轨迹线
static CMoTrackingLayer MoTrackingLayer;	// 挂接图层
static int CarNum;							// 车辆个数
static COLORREF	 m_color[COLOR_NUM];		// 颜色数组

struct Pos
{
	float x;
	float y;
};

struct CrossRoad
{
	float rd_wid;
	int rd_lnm, rd_rnm;			// 左右道路的道路数
	float rd_xlen, rd_ylen;		// x y 方向上的长度
	float rd_sidwk_wid;			// 人行道宽度
	float rd_arc;				// 圆角半径，默认等于人行道宽度
	float rd_cen_wid;			// 中心线间隔
};
class Car
{
public:
	Car();
	~Car();

private:
	// 成员变量
	Pos startPos;		// 起始位置
	Pos	curPos;			// 当前位置
	CMoLine lne;		// 路线
	int lneNum;			// 路线编号
	CMoGeoEvent evnt;	// 事件
	int shapNum;		// 形状编号
	CMoSymbol sym;		// 符号
	CMoPoint pt;		// evnt对应的点

	// 函数
	void Move();		// 启动
	void Stop();		// 这个函数能实现么，可以，状态传递
	void Acc();			// 加速
	void Dec();			// 减速
};


#endif