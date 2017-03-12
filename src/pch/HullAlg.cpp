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

void testAlg(int seed, int size, int loop)
{
	auto getRefFromRefItr = [](PointRefVec::iterator itr){return *itr;};
	auto getRefFromPtItr = [](PointVec::iterator itr){return &(*itr);};
	Timer timer;

	PointVec::initRand(seed);
	PointVec test1(size);
	reverse(test1.begin(), test1.end());
	auto testr = test1;
	reverse(test1.begin(), test1.end());

	auto gt = HullAlg::sequential(timer, test1.begin(), test1.end(), getRefFromPtItr);
	std::cout << "------------------------------------\nhull:";
	for (auto& p : gt)
		printf("(%f,%f)", p[0], p[1]);
	std::cout << std::endl << size << " " << gt.size() << std::endl;

	std::cout << "------------------------------------\nmanualParalWithPresort: ";
	timer.start();
	std::cout << "correctness: " << (HullAlg::manualParalWithPresort(timer, test1.begin(), test1.end(), getRefFromPtItr) == gt) << ". time:";
	std::cout << timer.stop() << std::endl;

	std::cout << "------------------------------------\nmanualParal: ";
	timer.start();
	std::cout << "correctness: " << (HullAlg::manualParal(timer, test1.begin(), test1.end(), getRefFromPtItr) == gt) << ". time:";
	std::cout << timer.stop() << std::endl;

	std::cout << "------------------------------------\nsequential: ";
	timer.start();
	std::cout << "correctness: " << (HullAlg::sequential(timer, test1.begin(), test1.end(), getRefFromPtItr) == gt) << ". time:";
	std::cout << timer.stop() << std::endl;

	std::cout << "------------------------------------\nsequential reverse: ";
	timer.start();
	std::cout << "correctness: " << (HullAlg::sequential(timer, testr.begin(), testr.end(), getRefFromPtItr) == gt) << ". time:";
	std::cout << timer.stop() << std::endl;

	std::cout << "------------------------------------\nsequential with presort: ";
	timer.start();
	std::cout << "correctness: " << (HullAlg::sequential(timer, test1.begin(), test1.end(), getRefFromPtItr, true).jaccard(gt)) << ". time:";
	std::cout << timer.stop() << std::endl;

	int cnt = 0;
	for (int i = 0; i < loop; ++i)
	{
		LOG_INFO << i;
		bool add = true;
		std::random_shuffle(test1.begin(), test1.end());
		//std::cout << HullAlg::sequential(timer, test1.begin(), test1.end(), getRefFromPtItr) << std::endl;
		if (HullAlg::sequential(timer, test1.begin(), test1.end(), getRefFromPtItr) != gt) 
		{
			LOG_WARN << "sequential WA";
			add = false;
		}
		if (HullAlg::manualParal(timer, test1.begin(), test1.end(), getRefFromPtItr) != gt)
		{
			LOG_WARN << "manualParal WA";
			add = false;
		}
		if (HullAlg::manualParalWithPresort(timer, test1.begin(), test1.end(), getRefFromPtItr) != gt)
		{
			LOG_WARN << "manualParalWithPresort WA";
			add = false;
		}
		cnt += add;
	}
	std::cout << seed << " " << size << " " << (loop ? (double)cnt / loop : 0) << std::endl;
	//std::cout << j << std::endl;
}