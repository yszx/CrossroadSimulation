#ifndef ATRNS_SIMU_H
#define ATRNS_SIMU_H

#include <cmath>
#include <ctime>	// ���ڼ�ʱ����ȫ���ģ��
#include <vector>
using namespace std;

// һЩ�Զ��峣����ͷ�ļ�
#define CAR_COLOR_NUM	5	
#define LIGHT_COLOR_NUM 3
#define LAYER_NUM		4			// ʮ��·�ڵ�ͼ����Ŀ
#define PI				3.141592
#define ELAPSE_TIME		50			// ��ʱ�����

static vector<CMoLine>	MoTrackLine;		// �켣��
static vector<CMoLine>	MoTrafficLight;		// ���̵�
static CMoTrackingLayer MoTrackingLayer;	// �ҽ�ͼ��
static int TotalCarNum;						// ��������
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
	int rd_lnm, rd_rnm;			// ���ҵ�·�ĵ�·��
	float rd_xlen, rd_ylen;		// x y �����ϵĳ���
	float rd_sidwk_wid;			// ���е����
	float rd_arc;				// Բ�ǰ뾶��Ĭ�ϵ������е����
	float rd_cen_wid;			// �����߼��
};
class Car
{
public:
	Car();
	~Car();

private:
	// ��Ա����
	Pos startPos;		// ��ʼλ��
	Pos	curPos, nexPos;	// ��ǰλ�á���һλ��
	int lneNum;			// ·�߱��
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
	BOOL flag;			// ֹͣ��־
	// ����
	void CreateCar(int lNum = 1, double dis = 1);	
						// ��ʾ����һ������������������š�ÿ������ʻ����
	void ChangeColor();	// �ı䳵��ɫ��������
	void Move();		// ����
	void Stop();		// ֹͣ
	void Acc();			// ����
	void Dec();			// ����
};

class TrafficLight
{
public:
	TrafficLight();
	~TrafficLight();
	void CreateLight(Pos pt1, Pos pt2);		// ��ʾ����
	void ChangeLightColor();	// ���õƵ���ɫ

private:
	CMoLine lne;		// ���̵ƶ�Ӧ����
	CMoSymbol sym;		// ����
	CMoGeoEvent evnt;	// �¼�
};


#endif