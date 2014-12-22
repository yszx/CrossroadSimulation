#ifndef ATRNS_SIMU_H
#define ATRNS_SIMU_H

#include <ctime>	// ���ڼ�ʱ����ȫ���ģ��
#include <cmath>

// һЩ�Զ��峣����ͷ�ļ�
#define COLOR_NUM 5	// ��ɫ���࣬��ɫ������ view �Ĺ��캯����
#define LAYER_NUM 4	// ʮ��·�ڵ�ͼ����Ŀ
#define PI		  3.14159

struct CrossRoad
{
	float rd_wid;
	int rd_lnm, rd_rnm;			// ���ҵ�·�ĵ�·��
	float rd_xlen, rd_ylen;		// x y �����ϵĳ���
	float rd_sidwk_wid;			// ���е����
	float rd_arc;				// Բ�ǰ뾶��Ĭ�ϵ������е����
	float rd_cen_wid;			// �����߼��
};


#endif