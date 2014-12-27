#ifndef ATRNS_SIMU_H
#define ATRNS_SIMU_H

#include <cmath>
#include <ctime>	// ���ڼ�ʱ����ȫ���ģ��
#include <vector>
using namespace std;

// �ṹ��
struct Pos
{
	double x;
	double y;
};
struct CrossRoad
{
	float rd_wid;
	int rd_lnm, rd_rnm;			// ���ҵ�·�ĵ�·��
	float rd_xlen, rd_ylen;		// x y �����ϵĳ���
	float rd_sidwk_wid;			// ���е����
	float rd_arc;				// Բ�ǰ뾶��Ĭ�ϵ������е����
	float rd_cen_wid;			// �����߼��
};

// һЩ����
#define CLOCK_CYCLE			15	// ���̵�����
#define DIV_NUM				6	// ���˵�·�� 2/DIV_NUM
#define ROAD_NUM			20
#define WALK_NUM			8
#define CAR_COLOR_NUM		5	
#define LIGHT_COLOR_NUM		3
#define ANGLE_NUM			360
#define LAYER_NUM			4				// ʮ��·�ڵ�ͼ����Ŀ
#define PI					3.141592
#define DEF_ELAPSE_TIME		50				// ��ʱ�����(ms)-��
#define CLOCK_ELAPSE_TIME	250				// ʱ�Ӽ��(ms)
#define CAR_DEFAULT_SIZE	20
#define CAR_DEFAULT_DIS		8
#define CAR_TEST_VALUE		5
#define PER_TEST_VALUE		2.5

#define INFX				999999
#define INFY				999999

static int EventNum;						// evnt ���
static double CLOCK_PRO = 0;				// ��ǰ��λ���е�ʱ��
static int STATE;							// ȫ����λ��Ŀǰ�����˵���ʻ��
static double SIDEWALK[4];					// ���е���ʶ
static double LIGHT[4];						// ���̵Ƶ��ĸ���ʶ�㣨���ڵ����޷�����View�ࣩ
static vector<CMoLine>	MoTrackLine;		// �켣��
static vector<CMoLine>	MoTrafficLight;		// ���̵�
static CMoTrackingLayer MoTrackingLayer;	// �ҽ�ͼ��
static int TotalCarNum;						// ��������
static COLORREF CarColor[CAR_COLOR_NUM] =
{ RGB(255, 0, 0), RGB(0, 255, 0), RGB(0, 0, 255), RGB(255, 255, 0), RGB(255, 0, 255) };

static COLORREF LightColor[LIGHT_COLOR_NUM] =
{ RGB(255, 0, 0), RGB(0, 255, 0), RGB(255, 255, 0) };

// ��
class Car
{
public:
	Car();
	~Car();

private:
	// ��Ա����
	Pos startPos;		// ��ʼλ��
	CMoLine lne;		// ·��
	CMoPoints pts;		// �켣��ɢ�㼯
	int ptsInd;			// �㼯��������
	CMoPoint pt;		// evnt��Ӧ�ĵ�
	CMoSymbol sym;		// ����
	CMoGeoEvent evnt;	// �¼�
	int carInd;			// �������
	int shapNum;		// ��״���
	double stepDis;		// ÿ��������ʻ�ľ���
	int divNum;			// ��ʻ�Ĳ���

public:
	int evntNum;		// evnt ��ţ�Ψһ
	int symColorInd;	// ��ɫ Index
	int state;			// ��������λ�����ڴ�����λ�߼�
	BOOL InOneWay(int l1, int l2);		// �Ƿ���ͬһ�ܵ���
	Pos	curPos, nexPos;		// ��ǰλ�á���һλ��
	int lneNum;				// ·�߱��
	BOOL flagStop;			// ֹͣ��־������ֹͣ��
	BOOL flagLightStop;		// ֹͣ��־�����ֹͣ����λ������
	BOOL flagEnd;			// ĩβ��־
	// ����
	void CreateCar(int lNum = 1, double dis = 1);	
						// ��ʾ����һ������������������š�ÿ������ʻ����
	void ChangeColor();	// �ı䳵��ɫ��������
	void Move();		// �ƶ�
	void Stop();		// ֹͣ
	void Start();		// ����
	void Disappear();	// ��ʧ
	void LightState();	// �����λ
	void Bump();		// ��ײ���
	void Acc();			// ����
	void Dec();			// ����
};

class TrafficLight
{
public:
	int evntNum;		// evnt ��ţ�Ψһ
	Pos ptOri, ptEnd;	// ��ʶ��
	TrafficLight();
	~TrafficLight();
	void CreateLight(Pos pt1, Pos pt2);		// ��ʾ����
	void ChangeLightColor(short id = 0);	// ���õƵ���ɫ id��0-�� 1-�� 2-��

private:
	CMoLine lne;		// ���̵ƶ�Ӧ����
	CMoSymbol sym;		// ����
	CMoGeoEvent evnt;	// �¼�
};

class Person
{
public:
	int evntNum;		// evnt ��ţ�Ψһ
	BOOL flagStop;
	BOOL flagEnd;
	int ptsInd;
	int divNum;
	int state;			// ��λ
	int lneNum;			// ·�߱��
	double dis;			// �ٶ�
	Person();
	~Person();
	void Move();		// �ƶ�
	void Stop();
	void Disappear();	// ��ʧ
	void LightState();	// ��λ�ж�
	void CreatePerson(int lNum = 1);
	// �����������������double�͵�����
	double RandGenerate(int a = 0, int b = 1);
	void GenerateLineAndPts(CMoPoint& pt1, CMoPoint& pt2);

private:
	CMoGeoEvent evnt;
	Pos startPos;
	Pos curPos, nexPos;
	CMoPoints pts;
	CMoLine lne;
};

static vector<Car>	m_Car;		// ������
static vector<Person> m_Per;	// ������
static CrossRoad m_crsRd;		// ��·���ݽṹ��
static vector<TrafficLight>	m_Light;		// ������
#endif