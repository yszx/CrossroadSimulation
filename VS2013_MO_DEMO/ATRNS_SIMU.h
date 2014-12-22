#ifndef ATRNS_SIMU_H
#define ATRNS_SIMU_H

#include <ctime>	// 用于计时和完全随机模拟
#include <cmath>

// 一些自定义常量和头文件
#define COLOR_NUM 5	// 颜色种类，颜色数组在 view 的构造函数里
#define LAYER_NUM 4	// 十字路口的图层数目
#define PI		  3.14159

struct CrossRoad
{
	float rd_wid;
	int rd_lnm, rd_rnm;			// 左右道路的道路数
	float rd_xlen, rd_ylen;		// x y 方向上的长度
	float rd_sidwk_wid;			// 人行道宽度
	float rd_arc;				// 圆角半径，默认等于人行道宽度
	float rd_cen_wid;			// 中心线间隔
};


#endif