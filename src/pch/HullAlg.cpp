//
//  HullAlg.cpp
//
//	by Jiahuan.Liu
//	jiahaun.liu@outlook.com
//
//  03/04/2017
//

#include <stdio.h>

#include "HullAlg.h"
#include "omp.h"

PointRefVec HullAlg::getRefs(PointVec& vec)
{
	PointRefVec res;
	res.reserve(vec.size());
	for (auto& p : vec)
	{
		res.push_back(&p);
	}
	return std::move(res);
}

void testAlg()
{
	//omp_set_num_threads(2);
	//omp_set_dynamic(true);

	auto getRefFromRefItr = [](PointRefVec::iterator itr){return *itr;};
	auto getRefFromPtItr = [](PointVec::iterator itr){return &(*itr);};
	Timer timer;

	/*PointVec test0;
	test0.emplace_back(0, 0);
	test0.emplace_back(0.2, 0);
	test0.emplace_back(0, 0.2);
	test0.emplace_back(0.15, 0.1);
	test0.emplace_back(0.1, 0.15);
	test0.emplace_back(0.2, 0.2);
	test0.emplace_back(0.1, 0.05);
	test0.emplace_back(0.05, 0.1);
	auto refs0 = HullAlg::getRefs(test0);

	timer.start();
	std::cout << HullAlg::manualParal(timer, refs0.begin(), refs0.end(), getRefFromRefItr) << " ";
	std::cout << timer.stop() << std::endl;


	reverse(refs0.begin(), refs0.end());
	timer.start();
	std::cout << HullAlg::manualParal(timer, refs0.begin(), refs0.end(), getRefFromRefItr) << " ";
	std::cout << timer.stop() << std::endl;
	reverse(refs0.begin(), refs0.end());*/

	PointVec::initRand(1);
	PointVec test1(1e3), testr;
	/*auto refs1 = HullAlg::getRefs(test1);

	std::cout << refs1.size() << std::endl;

	std::cout << "------------------------------------\n";
	timer.start();
	auto test2 = test1;
	auto refs2 = HullAlg::getRefs(test2);
	std::cout << HullAlg::manualParalWithPresort(timer, refs1.begin(), refs1.end(), getRefFromRefItr) << " ";
	std::cout << timer.stop() << std::endl;

	std::cout << "------------------------------------\n";
	timer.start();
	std::cout << HullAlg::sequential(timer, refs1.begin(), refs1.end(), getRefFromRefItr) << " ";
	std::cout << timer.stop() << std::endl;

	std::cout << "------------------------------------\n";
	reverse(refs1.begin(), refs1.end());
	reverse(test1.begin(), test1.end());
	auto test3 = test1;
	reverse(test1.begin(), test1.end());
	auto refs3 = HullAlg::getRefs(test3);
	timer.start();
	std::cout << HullAlg::sequential(timer, refs3.begin(), refs3.end(), getRefFromRefItr) << " ";
	std::cout << timer.stop() << std::endl;
	reverse(refs1.begin(), refs1.end());*/

	std::cout << test1.size() << std::endl;

	std::cout << "------------------------------------\n";
	timer.start();
	std::cout << HullAlg::manualParalWithPresort(timer, test1.begin(), test1.end(), getRefFromPtItr) << " ";
	std::cout << timer.stop() << std::endl;

	std::cout << "------------------------------------\n";
	reverse(test1.begin(), test1.end());
	testr = test1;
	timer.start();
	std::cout << HullAlg::manualParalWithPresort(timer, testr.begin(), testr.end(), getRefFromPtItr) << " ";
	std::cout << timer.stop() << std::endl;
	reverse(test1.begin(), test1.end());

	std::cout << "------------------------------------\n";
	timer.start();
	std::cout << HullAlg::sequential(timer, test1.begin(), test1.end(), getRefFromPtItr) << " ";
	std::cout << timer.stop() << std::endl;

	std::cout << "------------------------------------\n";
	reverse(test1.begin(), test1.end());
	testr = test1;
	timer.start();
	std::cout << HullAlg::sequential(timer, testr.begin(), testr.end(), getRefFromPtItr) << " ";
	std::cout << timer.stop() << std::endl;
	reverse(test1.begin(), test1.end());

	/*
	std::cout << "------------------------------------\n";
	timer.start();
	std::cout << HullAlg::manualParal(timer, refs1.begin(), refs1.end(), getRefFromRefItr) << " ";
	std::cout << timer.stop() << std::endl;

	std::cout << "------------------------------------\n";
	reverse(refs1.begin(), refs1.end());
	timer.start();
	std::cout << HullAlg::manualParal(timer, refs1.begin(), refs1.end(), getRefFromRefItr) << " ";
	std::cout << timer.stop() << std::endl;
	reverse(refs1.begin(), refs1.end());
	*/
}