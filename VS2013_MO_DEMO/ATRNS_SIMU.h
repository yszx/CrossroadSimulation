#ifndef ATRNS_SIMU_H
#define ATRNS_SIMU_H

#include <cmath>
#include <ctime>	// 用于计时和完全随机模拟
#include <vector>
using namespace std;

// 结构体
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

// 一些常量
#define CLOCK_CYCLE			15	// 红绿灯周期
#define DIV_NUM				6	// 行人的路程 2/DIV_NUM
#define ROAD_NUM			20
#define WALK_NUM			8
#define CAR_COLOR_NUM		5	
#define LIGHT_COLOR_NUM		3
#define ANGLE_NUM			360
#define LAYER_NUM			4				// 十字路口的图层数目
#define PI					3.141592
#define DEF_ELAPSE_TIME		50				// 定时器间隔(ms)-车
#define CLOCK_ELAPSE_TIME	250				// 时钟间隔(ms)
#define CAR_DEFAULT_SIZE	20
#define CAR_DEFAULT_DIS		8
#define CAR_TEST_VALUE		5
#define PER_TEST_VALUE		2.5

#define INFX				999999
#define INFY				999999

static int EventNum;						// evnt 编号
static double CLOCK_PRO = 0;				// 当前相位进行的时间
static int STATE;							// 全局相位（目前用于人的行驶）
static double SIDEWALK[4];					// 人行道标识
static double LIGHT[4];						// 红绿灯的四个标识点（由于灯类无法访问View类）
static vector<CMoLine>	MoTrackLine;		// 轨迹线
static vector<CMoLine>	MoTrafficLight;		// 红绿灯
static CMoTrackingLayer MoTrackingLayer;	// 挂接图层
static int TotalCarNum;						// 车辆个数
static COLORREF CarColor[CAR_COLOR_NUM] =
{ RGB(255, 0, 0), RGB(0, 255, 0), RGB(0, 0, 255), RGB(255, 255, 0), RGB(255, 0, 255) };

static COLORREF LightColor[LIGHT_COLOR_NUM] =
{ RGB(255, 0, 0), RGB(0, 255, 0), RGB(255, 255, 0) };

// 类
class Car
{
public:
	Car();
	~Car();

private:
	// 成员变量
	Pos startPos;		// 起始位置
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
	int evntNum;		// evnt 编号，唯一
	int symColorInd;	// 颜色 Index
	int state;			// 所处的相位，用于处理相位逻辑
	BOOL InOneWay(int l1, int l2);		// 是否在同一跑道上
	Pos	curPos, nexPos;		// 当前位置、下一位置
	int lneNum;				// 路线编号
	BOOL flagStop;			// 停止标志（立即停止）
	BOOL flagLightStop;		// 停止标志（红灯停止，相位决定）
	BOOL flagEnd;			// 末尾标志
	// 函数
	void CreateCar(int lNum = 1, double dis = 1);	
						// 显示创建一个车，参数：车道编号、每周期行驶距离
	void ChangeColor();	// 改变车颜色，测试用
	void Move();		// 移动
	void Stop();		// 停止
	void Start();		// 启动
	void Disappear();	// 消失
	void LightState();	// 针对相位
	void Bump();		// 碰撞检测
	void Acc();			// 加速
	void Dec();			// 减速
};

class TrafficLight
{
public:
	int evntNum;		// evnt 编号，唯一
	Pos ptOri, ptEnd;	// 标识点
	TrafficLight();
	~TrafficLight();
	void CreateLight(Pos pt1, Pos pt2);		// 显示创建
	void ChangeLightColor(short id = 0);	// 设置灯的颜色 id：0-红 1-绿 2-黄

private:
	CMoLine lne;		// 红绿灯对应的线
	CMoSymbol sym;		// 符号
	CMoGeoEvent evnt;	// 事件
};

class Person
{
public:
	int evntNum;		// evnt 编号，唯一
	BOOL flagStop;
	BOOL flagEnd;
	int ptsInd;
	int divNum;
	int state;			// 相位
	int lneNum;			// 路线编号
	double dis;			// 速度
	Person();
	~Person();
	void Move();		// 移动
	void Stop();
	void Disappear();	// 消失
	void LightState();	// 相位判断
	void CreatePerson(int lNum = 1);
	// 辅助函数（随机生成double型的数）
	double RandGenerate(int a = 0, int b = 1);
	void GenerateLineAndPts(CMoPoint& pt1, CMoPoint& pt2);

private:
	CMoGeoEvent evnt;
	Pos startPos;
	Pos curPos, nexPos;
	CMoPoints pts;
	CMoLine lne;
};

static vector<Car>	m_Car;		// 车容器
static vector<Person> m_Per;	// 人容器
static CrossRoad m_crsRd;		// 道路数据结构体
static vector<TrafficLight>	m_Light;		// 灯容器
#endif