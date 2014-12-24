#ifndef ATRNS_SIMU_H
#define ATRNS_SIMU_H

#include <ctime>	// ���ڼ�ʱ����ȫ���ģ��
#include <cmath>
#include <vector>
using namespace std;

// һЩ�Զ��峣����ͷ�ļ�
#define COLOR_NUM 5	// ��ɫ���࣬��ɫ������ view �Ĺ��캯����
#define LAYER_NUM 4	// ʮ��·�ڵ�ͼ����Ŀ
#define PI		  3.141592

static vector<CMoLine>		MoTrackLine;	// �켣��
static CMoTrackingLayer MoTrackingLayer;	// �ҽ�ͼ��
static int CarNum;							// ��������
static COLORREF	 m_color[COLOR_NUM];		// ��ɫ����

struct Pos
{
	float x;
	float y;
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
	Pos	curPos;			// ��ǰλ��
	CMoLine lne;		// ·��
	int lneNum;			// ·�߱��
	CMoGeoEvent evnt;	// �¼�
	int shapNum;		// ��״���
	CMoSymbol sym;		// ����
	CMoPoint pt;		// evnt��Ӧ�ĵ�

	// ����
	void Move();		// ����
	void Stop();		// ���������ʵ��ô�����ԣ�״̬����
	void Acc();			// ����
	void Dec();			// ����
};


#endif