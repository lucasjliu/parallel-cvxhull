//
//  main.cpp
//
//  by Jiahuan.Liu
//	jiahaun.liu@outlook.com
//
//  02/26/2017
//

#include "hull.h"
#include "TestBase.h"

int main()
{
    Hull hull;
	hull.addPoint(0,0);
	hull.addPoint(0.2,0);
	hull.addPoint(0,0.2);
	hull.addPoint(0.15,0.1);
	hull.addPoint(0.1,0.15);
    hull.addPoint(0.2,0.2);
	hull.printPeaks();
	//testTimer();
	//testUnitTest();
}