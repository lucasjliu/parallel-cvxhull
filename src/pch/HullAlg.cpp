//
//  HullAlg.cpp
//
//	by Jiahuan.Liu
//	jiahaun.liu@outlook.com
//
//  03/04/2017
//

#include <stdio.h>
#include <algorithm>
#include <unordered_set>

#include "HullAlg.h"
#include "Hull.h"
#include "omp.h"

PointRefVec HullAlg::getRefs(const PointVec& vec)
{
	PointRefVec res;
	res.reserve(vec.size());
	for (auto& p : vec)
	{
		res.push_back(const_cast<PointRef>(&p));
	}
	return std::move(res);
}

////TODO combine them
PointVec HullAlg::getPts(const PointRefVec& vec)
{
	PointVec res;
	res.reserve(vec.size());
	for (const auto& ref : vec)
	{
		res.push_back(*ref);
	}
	return std::move(res);
}

void testAlg(int seed, int size)
{
	auto getRefFromRefItr = [](PointRefVec::iterator itr){return *itr;};
	auto getRefFromPtItr = [](PointVec::iterator itr){return &(*itr);};
	Timer timer;

	PointVec::initRand(seed);
	PointVec test1(size);
	PointVec triplet;
	triplet.emplace_back(1000,1000);
	triplet.emplace_back(1000.01,1000);
	triplet.emplace_back(1000,1000.01);
	test1.insert(test1.begin(), triplet.begin(), triplet.end());
	test1.insert(test1.end(), triplet.begin(), triplet.end());
	reverse(test1.begin(), test1.end());
	auto testr = test1;
	reverse(test1.begin(), test1.end());

	auto gt = HullAlg::sequential(timer, test1.begin(), test1.end(), getRefFromPtItr);
	for (int i = 0; i < 1000; ++i)
	{
		//std::cout << HullAlg::sequential(timer, test1.begin(), test1.end(), getRefFromPtItr) << std::endl;
		assert(HullAlg::sequential(timer, test1.begin(), test1.end(), getRefFromPtItr) == gt);
		assert(HullAlg::manualParal(timer, test1.begin(), test1.end(), getRefFromPtItr) == gt);
		assert(HullAlg::manualParalWithPresort(timer, test1.begin(), test1.end(), getRefFromPtItr) == gt);
		std::random_shuffle(test1.begin() + 3, test1.end() - 3);
	}
	//std::cout << j << std::endl;
}

void testAlg()
{
	//omp_set_num_threads(2);
	//omp_set_dynamic(true);

	auto getRefFromRefItr = [](PointRefVec::iterator itr){return *itr;};
	auto getRefFromPtItr = [](PointVec::iterator itr){return &(*itr);};
	Timer timer;

	// PointVec test0;
	// test0.emplace_back(0, 0);
	// test0.emplace_back(0.01, 0);
	// test0.emplace_back(0, 0.01);
	// test0.emplace_back(0.2, 0.2);
	// test0.emplace_back(0.05, 0.05);
	// test0.emplace_back(0.2, 0);
	// test0.emplace_back(0, 0.2);
	// test0.emplace_back(0.15, 0.1);
	// test0.emplace_back(0.1, 0.15);
	// test0.emplace_back(0.1, 0.05);
	// test0.emplace_back(0.05, 0.1);

	// auto gt = HullAlg::sequential(timer, test0.begin(), test0.end(), getRefFromPtItr);

	// for (int i = 0; i < 1000; ++i)
	// {//trivial test for correctness
	// 	assert(HullAlg::sequential(timer, test0.begin(), test0.end(), getRefFromPtItr).jaccard(gt) == 1);
	// 	std::random_shuffle(test0.begin()+3, test0.end());
	// }

	PointVec::initRand(4);
	PointVec test1(1e4);
	PointVec triplet;
	triplet.emplace_back(1000,1000);
	triplet.emplace_back(1000.01,1000);
	triplet.emplace_back(1000,1000.01);
	test1.insert(test1.begin(), triplet.begin(), triplet.end());
	test1.insert(test1.end(), triplet.begin(), triplet.end());
	reverse(test1.begin(), test1.end());
	auto testr = test1;
	reverse(test1.begin(), test1.end());

	// int res = HullAlg::sequential(timer, test1.begin(), test1.end(), getRefFromPtItr);
	// for (int i = 0; i < 1000; ++i)
	// {
	// 	//std::cout << HullAlg::sequential(timer, test1.begin(), test1.end(), getRefFromPtItr) << std::endl;
	// 	assert(HullAlg::sequential(timer, test1.begin(), test1.end(), getRefFromPtItr) == res);
	// 	std::random_shuffle(test1.begin() + 3, test1.end() - 3);
	// }
	

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

	//std::cout << test1.size() << std::endl;

	auto res = HullAlg::sequential(timer, test1.begin(), test1.end(), getRefFromPtItr);

	std::cout << "------------------------------------\n";
	timer.start();
	std::cout << "result: " << (HullAlg::manualParalWithPresort(timer, test1.begin(), test1.end(), getRefFromPtItr) == res) << " ";
	std::cout << timer.stop() << std::endl;

	std::cout << "------------------------------------\n";
	timer.start();
	std::cout << "result: " << (HullAlg::manualParal(timer, test1.begin(), test1.end(), getRefFromPtItr) == res) << " ";
	std::cout << timer.stop() << std::endl;

	/*std::cout << "------------------------------------\n";
	timer.start();
	std::cout << "result: " << HullAlg::manualParal(timer, testr.begin(), testr.end(), getRefFromPtItr) << " ";
	std::cout << timer.stop() << std::endl;*/

	std::cout << "------------------------------------\n";
	timer.start();
	std::cout << "result: " << (HullAlg::sequential(timer, test1.begin(), test1.end(), getRefFromPtItr) == res) << " ";
	std::cout << timer.stop() << std::endl;

	std::cout << "------------------------------------\n";
	timer.start();
	std::cout << "result: " << (HullAlg::sequential(timer, testr.begin(), testr.end(), getRefFromPtItr) == res) << " ";
	std::cout << timer.stop() << std::endl;

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