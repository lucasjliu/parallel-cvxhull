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
	auto getRefFromRefItr = [](PointRefVec::iterator itr){return *itr;};
	Timer timer;

	/*
	PointVec test0;
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
	std::cout << HullAlg::sequential(timer, refs0.begin(), refs0.end(), getRefFromRefItr) << " ";
	std::cout << timer.stop() << std::endl;


	reverse(refs0.begin(), refs0.end());
	timer.start();
	std::cout << HullAlg::sequential(timer, refs0.begin(), refs0.end(), getRefFromRefItr) << " ";
	std::cout << timer.stop() << std::endl;
	reverse(refs0.begin(), refs0.end());
	*/

	PointVec::initRand(1);
	PointVec test1(5e6);
	auto refs1 = HullAlg::getRefs(test1);

	std::cout << refs1.size() << std::endl;

	timer.start();
	std::cout << HullAlg::sequential(timer, refs1.begin(), refs1.end(), getRefFromRefItr) << " ";
	std::cout << timer.stop() << std::endl;

	reverse(refs1.begin(), refs1.end());
	timer.start();
	std::cout << HullAlg::sequential(timer, refs1.begin(), refs1.end(), getRefFromRefItr) << " ";
	std::cout << timer.stop() << std::endl;
	reverse(refs1.begin(), refs1.end());

	timer.start();
	std::cout << HullAlg::manualParal(timer, refs1.begin(), refs1.end(), getRefFromRefItr) << " ";
	std::cout << timer.stop() << std::endl;

	reverse(refs1.begin(), refs1.end());
	timer.start();
	std::cout << HullAlg::manualParal(timer, refs1.begin(), refs1.end(), getRefFromRefItr) << " ";
	std::cout << timer.stop() << std::endl;
	reverse(refs1.begin(), refs1.end());

	/*
	timer.start();
	std::cout << HullAlg::manualParal(timer, refs1.begin(), refs1.end(), getRefFromRefItr, 0) << " ";
	std::cout << timer.stop() << std::endl;

	reverse(refs1.begin(), refs1.end());
	timer.start();
	std::cout << HullAlg::manualParal(timer, refs1.begin(), refs1.end(), getRefFromRefItr, 0) << " ";
	std::cout << timer.stop() << std::endl;
	reverse(refs1.begin(), refs1.end());
	*/
}