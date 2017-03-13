//
//  main.cpp
//
//  by Jiahuan.Liu
//	jiahaun.liu@outlook.com
//
//  02/26/2017
//

#include <cstdlib>

#include "tests.h"

int main(int argc, char** argv)
{
	if (argc >= 3)
	{
		int seed = atoi(argv[1]);
		int size = atoi(argv[2]);
		int loop = 1000;
		if (argc == 4) loop = atoi(argv[3]);
		testAlg(seed, size, loop);
	}
	else
	{
		testAlg();
		// testTimer();
		// testUnitTest();
		// testSort();
		// testMarginalitySort();
	}
	
}