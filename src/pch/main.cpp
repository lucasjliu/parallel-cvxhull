//
//  main.cpp
//
//  by Jiahuan.Liu
//	jiahaun.liu@outlook.com
//
//  02/26/2017
//

#include "Hull.h"
#include "UnitTest.h"
#include "HullAlg.h"

int main(int argc, char** argv)
{
	if (argc == 3)
	{
		int seed = atoi(argv[1]);
		int size = atoi(argv[2]);
		testAlg(seed, size);
	}
	else
	{
		//testHull();
		//testTimer();
		//testUnitTest();
		testAlg();
		//testSort();
		//testMarginalitySort();
	}
	
}