#ifndef ATRNS_SIMU_H
#define ATRNS_SIMU_H

#include <cmath>
#include <ctime>	// 用于计时和完全随机模拟
#include <vector>
using namespace std;

// 一些自定义常量和头文件
#define CAR_COLOR_NUM	5	
#define LIGHT_COLOR_NUM 3
#define LAYER_NUM		4			// 十字路口的图层数目
#define PI				3.141592
#define ELAPSE_TIME		50			// 定时器间隔

static vector<CMoLine>	MoTrackLine;		// 轨迹线
static vector<CMoLine>	MoTrafficLight;		// 红绿灯
static CMoTrackingLayer MoTrackingLayer;	// 挂接图层
static int TotalCarNum;						// 车辆个数
static COLORREF CarColor[CAR_COLOR_NUM] =
{ RGB(255, 0, 0), RGB(0, 255, 0), RGB(0, 0, 255), RGB(255, 255, 0), RGB(255, 0, 255) };

static COLORREF LightColor[LIGHT_COLOR_NUM] =
{ RGB(255, 0, 0), RGB(0, 255, 0), RGB(255, 255, 0) };

struct Pos
{
	double x;
	double y;
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
	Pos	curPos, nexPos;	// 当前位置、下一位置
	int lneNum;			// 路线编号
	CMoLine lne;		// 路线
	CMoPoints pts;		// 轨迹离散点集
	int ptsInd;			// 点集的索引号
	CMoPoint pt;		// evnt对应的点
	CMoSymbol sym;		// 符号
	CMoGeoEvent evnt;	// 事件
	int carInd;			// 车辆编号
	int shapNum;		// 形状编号
	double stepDis;		// 每个周期行驶的距离
	int divNum;			// 行驶的步数

public:
	BOOL flag;			// 停止标志
	// 函数
	void CreateCar(int lNum = 1, double dis = 1);	
						// 显示创建一个车，参数：车道编号、每周期行驶距离
	void ChangeColor();	// 改变车颜色，测试用
	void Move();		// 启动
	void Stop();		// 停止
	void Acc();			// 加速
	void Dec();			// 减速
};

class TrafficLight
{
public:
	TrafficLight();
	~TrafficLight();
	void CreateLight(Pos pt1, Pos pt2);		// 显示创建
	void ChangeLightColor();	// 设置灯的颜色

private:
	CMoLine lne;		// 红绿灯对应的线
	CMoSymbol sym;		// 符号
	CMoGeoEvent evnt;	// 事件
};


#endif