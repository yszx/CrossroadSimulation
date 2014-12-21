#ifndef ATRNS_SIMU_H
#define ATRNS_SIMU_H

#include <ctime>	// 用于计时和完全随机模拟

// 一些自定义常量和头文件
#define COLOR_NUM 5	// 颜色种类，颜色数组在 view 的构造函数里

struct CrossRoad
{
	float rd_wid;
	int rd_lnm, rd_rnm;			// 左右道路的道路数
	float rd_xlen, rd_ylen;		// x y 方向上的长度
	float rd_sidwk_wid;			// 人行道宽度
};


#endif