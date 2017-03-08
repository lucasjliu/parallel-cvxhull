//
//  ParallelSort.cpp
//
//	@brief: marginality sort of vector of points with 2 dimensionality
//
//	by Jiahuan.Liu
//	jiahaun.liu@outlook.com
//
//  03/06/2017
//

#include <algorithm>
#include <time.h>
#include <functional>

#include "ParallelSort.h"

void testSort()
{
	const int size = 5e6;
	const int ntests = 1;
	std::vector<double> vecs[ntests], gts[ntests];
	srand(1);

	using ret_type = std::vector<double>;
	auto paraSort = [](Timer& t, ret_type& v) {ParallelSort::mergesort(v.begin(), v.end(), 2); return v;};
	auto seqSort = [](Timer& t, ret_type& v) {ParallelSort::mergesort(v.begin(), v.end(), 1); return v;};
	auto paral = UnitTestFactory::create<ret_type>(paraSort);
	auto seq = UnitTestFactory::create<ret_type>(seqSort);

	Timer timer;
	int t1, t2;

	for (int t = 0; t < ntests; ++t)
	{
		for (int i = 0; i < size; ++i)
		{
			vecs[t].push_back(rand() / RAND_MAX);
			gts[t].push_back(vecs[t][i]);
		}

		seq.addCase({}, gts[t]);

		auto gt = gts[t];
		timer.start();
		std::sort(gt.begin(), gt.end());
		t1 = timer.stop();

		paral.addCase(gts[t], vecs[t]);

		auto vec = vecs[t];
		timer.start();
		ParallelSort::mergesort(vec.begin(), vec.end(), 1);
		t2 = timer.stop();
		
		LOG_INFO << "case " << t << ": " << t1 << " " << t2;
	}
	
	//seq.run();
	paral.run();
}