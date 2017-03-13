//
//  tests.cpp
//
//  by Jiahuan.Liu
//	jiahaun.liu@outlook.com
//
//  03/11/2017
//

#include <unistd.h>
#include <cstdio>
#include <iostream>
#include <cassert>
#include <algorithm>
#include <time.h>
#include <functional>

#include "tests.h"

#include "UnitTest.h"
#include "Marginality.h"
#include "ParalHull.h"
#include "ParalSort.h"

void testTimer()
{
	Timer t1;
	t1.start();
	assert(t1.stop() == 0);

	t1.start();
	usleep(1000);
	assert(t1.stop() == 1);

	t1.start();
	usleep(1000);
	t1.pause();
	usleep(5000);
	t1.resume();
	usleep(1000);
	assert(t1.stop() == 2);
}

void testUnitTest()
{
	auto null_task0 = [](Timer& t, int a){return a;};
	auto t0 = UnitTestFactory::create<int>(null_task0);
	t0.addCase(1, 1);
	t0.addCase(2, 2);
	t0.addCase(1, 0);
	__TRY__
	t0.run();
	__CATCH__(decltype(t0)::Exception& e)
	std::cout << e.what() << std::endl;
	__END__

	auto null_task1 = [](Timer& t, std::vector<int> v){usleep(1000); return v;};
	auto t1 = UnitTestFactory::create<std::vector<int>>(null_task1);
	t1.addCase({}, std::vector<int>({}));
	t1.addCase({1,2,3}, std::vector<int>({1,2,3}));
	t1.addCase({1,2,4}, std::vector<int>({1,2,3}));
	t1.addCase({}, std::vector<int>({}));
	__TRY__
	t1.run();
	__CATCH__(decltype(t1)::Exception& e)
	std::cout << e.what() << std::endl;
	__END__
}

void testSort()
{
	/*std::vector<int> test = {6,4,2,1,3,5,9,0};
	ParalSort::mergesort(test.begin(), test.end(), 4);
	for (int num : test)
		std::cout << num << " ";
	std::cout << std::endl;*/

	const int size = 5e6;
	const int ntests = 1;
	std::vector<double> vecs[ntests], gts[ntests];
	srand(1);

	using ret_type = std::vector<double>;
	auto paraSort = [](Timer& t, ret_type& v) {ParalSort::mergesort(v.begin(), v.end(), 2); return v;};
	auto seqSort = [](Timer& t, ret_type& v) {ParalSort::mergesort(v.begin(), v.end(), 1); return v;};
	auto paral = UnitTestFactory::create<ret_type>(paraSort);
	auto seq = UnitTestFactory::create<ret_type>(seqSort);

	Timer timer;
	int t1, t2;

	for (int t = 0; t < ntests; ++t)
	{
		for (int i = 0; i < size; ++i)
		{
			vecs[t].push_back(rand() / (double)RAND_MAX);
			gts[t].push_back(vecs[t][i]);
		}

		seq.addCase({}, gts[t]);

		std::vector<double> gt = gts[t];
		timer.start();
		std::sort(gt.begin(), gt.end());
		t1 = timer.stop();

		paral.addCase(gt, vecs[t]);

		std::vector<double> vec = vecs[t];
		timer.start();
		ParalSort::mergesort(vec.begin(), vec.end(), 1);
		t2 = timer.stop();
		
		std::cout << "sequential sort: " << ": " << t1 << " " << t2;
	}
	
	//seq.run();
	paral.run();

}

void testMarginalitySort()
{
	PointVec test0;
	test0.emplace_back(0, 0);
	test0.emplace_back(0.2, 0);
	test0.emplace_back(0, 0.2);
	test0.emplace_back(0.15, 0.1);
	test0.emplace_back(0.1, 0.15);
	test0.emplace_back(0.2, 0.2);
	test0.emplace_back(0.1, 0.05);
	test0.emplace_back(0.05, 0.1);

	auto refs0 = ParalHull::getRefs(test0);
	auto getRefFromRefItr = [](PointRefVec::iterator itr){return *itr;};
	auto getRefFromPtItr = [](PointVec::iterator itr){return &(*itr);};

	auto res_refs = Marginality::sort(refs0.begin(), refs0.end(), getRefFromRefItr);
	auto res = Marginality::sort(test0.begin(), test0.end(), getRefFromPtItr);
}

struct result_t
{
	int size;
	unsigned long t1;
	unsigned long t2;
	unsigned long t3;
	unsigned long t4;
};

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

	auto gt = ParalHull::sequential(timer, test1.begin(), test1.end(), getRefFromPtItr);
	std::cout << "------------------------------------\nresult hull:\n";
	for (auto& p : gt)
		printf("(%f,%f)", p[0], p[1]);
	std::cout << std::endl << size << " " << gt.size() << std::endl;

	std::cout << "------------------------------------\nmanualParalWithPresort:\n";
	timer.start();
	std::cout << "correctness: " << (ParalHull::manualParalWithPresort(timer, test1.begin(), test1.end(), getRefFromPtItr) == gt) << ". time: ";
	std::cout << timer.stop() << std::endl;

	std::cout << "------------------------------------\nmanualParal:\n";
	timer.start();
	std::cout << "correctness: " << (ParalHull::manualParal(timer, test1.begin(), test1.end(), getRefFromPtItr) == gt) << ". time: ";
	std::cout << timer.stop() << std::endl;

	std::cout << "------------------------------------\nsequential:\n";
	timer.start();
	std::cout << "correctness: " << (ParalHull::sequential(timer, test1.begin(), test1.end(), getRefFromPtItr) == gt) << ". time: ";
	std::cout << timer.stop() << std::endl;

	std::cout << "------------------------------------\nsequential reverse:\n";
	timer.start();
	std::cout << "correctness: " << (ParalHull::sequential(timer, testr.begin(), testr.end(), getRefFromPtItr) == gt) << ". time: ";
	std::cout << timer.stop() << std::endl;

	std::cout << "------------------------------------\nsequential with presort:\n";
	timer.start();
	std::cout << "correctness: " << (ParalHull::sequential(timer, test1.begin(), test1.end(), getRefFromPtItr, true).jaccard(gt)) << ". time: ";
	std::cout << timer.stop() << std::endl;

	std::cout << "------------------------------------\n";

	int cnt = 0;
	for (int i = 0; i < loop; ++i)
	{
		LOG_INFO << i;
		bool add = true;
		std::random_shuffle(test1.begin(), test1.end());
		//std::cout << ParalHull::sequential(timer, test1.begin(), test1.end(), getRefFromPtItr) << std::endl;
		if (ParalHull::sequential(timer, test1.begin(), test1.end(), getRefFromPtItr) != gt) 
		{
			LOG_WARN << "sequential WA";
			add = false;
		}
		if (ParalHull::manualParal(timer, test1.begin(), test1.end(), getRefFromPtItr) != gt)
		{
			LOG_WARN << "manualParal WA";
			add = false;
		}
		if (ParalHull::manualParalWithPresort(timer, test1.begin(), test1.end(), getRefFromPtItr) != gt)
		{
			LOG_WARN << "manualParalWithPresort WA";
			add = false;
		}
		cnt += add;
	}
	std::cout << seed << " " << size << " " << (loop ? (double)cnt / loop : 0) << std::endl;
	//std::cout << j << std::endl;
}

void testAlg()
{
	auto getRefFromRefItr = [](PointRefVec::iterator itr){return *itr;};
	auto getRefFromPtItr = [](PointVec::iterator itr){return &(*itr);};
	Timer timer;

	const double sizes[] = {2e5, 4e5, 6e5, 8e5, 1e6};
	const int loops[] = {50, 50, 50, 50, 50};
	// const double sizes[] = {2e3, 4e3, 6e3, 8e3, 1e4};
	// const int loops[] = {10, 10, 10, 10, 10};
	const int nexp = 5;
	const int nseed = 8;
	const char split[] = "------------------------------------\n";

	result_t total_result[nexp];
	int total_correct_cnt[nexp] = {0};

	for (int seed = 4; seed <= 4; ++seed)
	{
		PointVec::initRand(seed);

		result_t result[nexp];
		int size, loop;
		double conf;

		for (int i = 0; i < nexp; ++i)
		{
			size = (int)sizes[i], loop = loops[i];
			PointVec points(size);
			result[i].size = size;

			auto gt = ParalHull::sequential(timer, points.begin(), points.end(), getRefFromPtItr);
			int correct_cnt = 0;

			for (int j = 0; j < loop; ++j)
			{
				bool valid = true;

				std::random_shuffle(points.begin(), points.end());

				timer.start();
				auto r1 = ParalHull::manualParalWithPresort(timer, points.begin(), points.end(), getRefFromPtItr);
				auto t1 = timer.stop();

				timer.start();
				auto r2 = ParalHull::manualParal(timer, points.begin(), points.end(), getRefFromPtItr);
				auto t2 = timer.stop();

				timer.start();
				auto r3 = ParalHull::sequential(timer, points.begin(), points.end(), getRefFromPtItr);
				auto t3 = timer.stop();

				timer.start();
				auto r4 = ParalHull::sequential(timer, points.begin(), points.end(), getRefFromPtItr, true);
				auto t4 = timer.stop();

				if (r1 != gt) { LOG_WARN << seed << " " << j << " manualParalWithPresort WA: " << r1.jaccard(gt); valid = false; }
				if (r2 != gt) { LOG_WARN << seed << " " << j << " manualParal WA: " << r2.jaccard(gt); valid = false; }
				if (r3 != gt) { LOG_WARN << seed << " " << j << " sequential WA: " << r3.jaccard(gt); valid = false; }
				if (r4 != gt) { LOG_WARN << seed << " " << j << " sequentialWithPresort WA: " << r4.jaccard(gt); valid = false; }
				if (r1 == gt && r2 == gt && r3 == gt && r4 == gt)
				{
					result[i].t1 += t1;
					result[i].t2 += t2;
					result[i].t3 += t3;
					result[i].t4 += t4;
					correct_cnt += valid;
				}

			}

			LOG_INFO << seed << " " << size;

			if (correct_cnt == 0)
			{
				std::cout << split
					<< "result: bad result!" << std::endl
					<< "seed: " << seed << std::endl
					<< "size: " << size << std::endl;
				continue;
			}

			total_correct_cnt[i] += correct_cnt;

			std::cout << split
				<< "result:" << std::endl
				<< "seed: " << seed << std::endl
				<< "size: " << size << std::endl
				<< "confidence: " << (double)correct_cnt / loop << std::endl
				<< "manualParalWithPresort: " << result[i].t1 / correct_cnt << std::endl
				<< "manualParal: " << result[i].t2 / correct_cnt << std::endl
				<< "sequential: " << result[i].t3 / correct_cnt << std::endl
				<< "sequentialWithPresort: " << result[i].t4 / correct_cnt << std::endl;

		}

		for (int i = 0; i < nexp; ++i)
		{
			total_result[i].t1 += result[i].t1;
			total_result[i].t2 += result[i].t2;
			total_result[i].t3 += result[i].t3;
			total_result[i].t4 += result[i].t4;
		}

	}

	for (int i = 0; i < nexp; ++i)
	{
		int size = sizes[i];
		std::cout << split
			<< "total result:" << std::endl
			<< "size: " << size << std::endl
			<< "manualParalWithPresort: " << total_result[i].t1 / total_correct_cnt[i] << std::endl
			<< "manualParal: " << total_result[i].t2 / total_correct_cnt[i] << std::endl
			<< "sequential: " << total_result[i].t3 / total_correct_cnt[i] << std::endl
			<< "sequentialWithPresort: " << total_result[i].t4 / total_correct_cnt[i] << std::endl;
	}
}