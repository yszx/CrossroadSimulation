#ifndef ATRNS_SIMU_H
#define ATRNS_SIMU_H

#include <ctime>	// ���ڼ�ʱ����ȫ���ģ��

// һЩ�Զ��峣����ͷ�ļ�
#define COLOR_NUM 5	// ��ɫ���࣬��ɫ������ view �Ĺ��캯����
#define LAYER_NUM 1	// ʮ��·�ڵ�ͼ����Ŀ

struct CrossRoad
{
	float rd_wid;
	int rd_lnm, rd_rnm;			// ���ҵ�·�ĵ�·��
	float rd_xlen, rd_ylen;		// x y �����ϵĳ���
	float rd_sidwk_wid;			// ���е����
	float rd_arc;				// Բ�ǰ뾶��Ĭ�ϵ������е����
};


#endif