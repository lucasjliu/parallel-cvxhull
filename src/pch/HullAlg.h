//
//  HullAlg.h
//
//	@brief: parallel convex hull algorithm
//
//	by Jiahuan.Liu
//	jiahaun.liu@outlook.com
//
//  03/04/2017
//

#ifndef _HULLALG_H
#define _HULLALG_H

#include <algorithm>
#include <omp.h>
#include <math.h>
#include <assert.h>

#include "Hull.h"
#include "UnitTest.h"
#include "Marginality.h"

const int MIN_SIZE = NDim + 1;
const int THR_NUM = 2;

class HullAlg
{
public:
	//
	// @brief: public interface of algorithm
	// @param: beg, end: specify input points, should be RandomAccessItrator
	// 		   getRef: method to get PointRef from itr
	// @return: number of anti-origin points in resulting polygon
	//
	template <typename Itr, typename GetRef>
	static int sequential(Timer& timer, Itr beg, Itr end, GetRef getRef);

	template <typename Itr, typename GetRef>
	static int manualParal(Timer& timer, Itr beg, Itr end, GetRef getRef, int prevCnt);

	template <typename Itr, typename GetRef>
	static int manualParal(Timer& timer, Itr beg, Itr end, GetRef getRef);

	template <typename Itr, typename GetRef>
	static int manualParalWithPresort(Timer& timer, Itr beg, Itr end, GetRef getRef);

	template <typename Itr, typename GetRef>
	static int specuParal(Timer& timer, Itr beg, Itr end, GetRef getRef);

	static PointRefVec getRefs(PointVec& vec);
//private:
	//
	// @brief: internal implementation of parallelization
	// @param: hull, hulls: for hull structures return
	// @return: PointRef vector of anti-origin points
	//
	template <typename Itr, typename GetRef>
	static PointRefVec _sequential(Itr beg, Itr end, GetRef getRef, Hull& hull);

	template <typename Itr, typename GetRef>
	static std::vector<PointRefVec> _parallel(Itr beg, Itr end, GetRef getRef, std::vector<Hull>& hulls);

	template <typename Vec>
	static int _count(const std::vector<Vec>& vecs);

	template <typename Vec>
	static Vec _flatten(const std::vector<Vec>& vecs, int count);

	template <typename Vec>
	static Vec _flatten(const std::vector<Vec>& vecs);
};

template <typename Itr, typename GetRef>
PointRefVec HullAlg::_sequential(Itr beg, Itr end, GetRef getRef, Hull& hull)
{
	////
	LOG_INFO << "Seq: " << end - beg << " from " << beg - beg << " to " << end - beg;
	for (Itr itr = beg; itr != end; ++itr)
	{
		hull.insert(getRef(itr));
	}

	////
	auto peaks = hull.getPeaks();
	for (auto& p : peaks)
	{
		LOG_INFO << "Hull: " << (*p)[0] << " " << (*p)[1];
	}
	/*hull.printPeaks();*/

	return std::move(hull.getPeaks());
}

template <typename Itr, typename GetRef>
std::vector<PointRefVec> HullAlg::_parallel(Itr beg, Itr end, GetRef getRef, std::vector<Hull>& hulls)
{
	const int size = end - beg;
	int thrNum = THR_NUM;//####TODO
	int len = ceil(size / thrNum);
	
	while (len <= MIN_SIZE && thrNum > 1)
	{
		thrNum /= 2;
		len = ceil(size / thrNum);
	}

	////LOG_INFO << thrNum << " " << len;

	hulls.reserve(thrNum);
	for (int i = 0; i < thrNum; ++i)
	{
		hulls.emplace_back(len);
	}

	std::vector<PointRefVec> results(thrNum);

	omp_set_num_threads(thrNum);

	#pragma omp parallel shared(results, len, beg, end, getRef, hulls) num_threads(thrNum) //num_threads must be set
	{
		int tid = omp_get_thread_num();
		Itr first = beg + len * tid, 
			last = (tid == thrNum - 1 ? end : first + len);
		////LOG_INFO << tid << " " << last - first << " from " << first - beg << " to " << last - beg;
		PointRefVec result = _sequential(first, last, getRef, hulls[tid]);
		results[tid] = std::move(result);
	}

	return std::move(results);
}

template <typename Vec>
int HullAlg::_count(const std::vector<Vec>& vecs)
{
	int res = 0;
	for (auto& v : vecs)
	{
		res += v.size();
	}
	return res;
}

template <typename Vec>
Vec HullAlg::_flatten(const std::vector<Vec>& vecs, int count)
{
	Vec res(count);
	int cnt = 0;
	for (auto& vec: vecs)
		for (auto& elem: vec)
			res[cnt++] = elem;
	return res;
}

template <typename Vec>
Vec HullAlg::_flatten(const std::vector<Vec>& vecs)
{
	return _flatten(vecs, _count(vecs));
}

template <typename Itr, typename GetRef>
int HullAlg::sequential(Timer& timer, Itr beg, Itr end, GetRef getRef)
{
	int size = end - beg;
	//timer.pause();//####
	Hull hull(size);
	//timer.resume();//####
	return _sequential(beg, end, getRef, hull).size();
}

template <typename Itr, typename GetRef>
int HullAlg::manualParal(Timer& timer, Itr beg, Itr end, GetRef getRef, int prevCnt)
{
	std::vector<Hull> hulls;
	auto results = _parallel(beg, end, getRef, hulls);
	int currCnt = _count(results);

	if (results.size() <= 1) return currCnt;

	//timer.pause();//####
	PointRefVec nextStep = _flatten(results, currCnt);
	static std::function<PointRef(PointRefVec::iterator)> getRefFromRefItr = 
		[](PointRefVec::iterator itr){return *itr;};

	hulls.clear();
	//timer.resume();//####

	return (currCnt == prevCnt) ? 
		sequential(timer, nextStep.begin(), nextStep.end(), getRefFromRefItr) :
		manualParal(timer, nextStep.begin(), nextStep.end(), getRefFromRefItr, currCnt);
}

template <typename Itr, typename GetRef>
int HullAlg::manualParal(Timer& timer, Itr beg, Itr end, GetRef getRef)
{
	std::vector<Hull> hulls;
	auto results = _parallel(beg, end, getRef, hulls);
	int currCnt = _count(results), prevCnt = end - beg;

	if (results.size() <= 1) return currCnt;

	PointRefVec nextStep;
	auto getRefFromRefItr = [](PointRefVec::iterator itr){return *itr;};

	//timer.pause();//####
	while (currCnt != prevCnt)
	{
		prevCnt = currCnt;

		nextStep = _flatten(results, currCnt);
		//timer.resume();//####

		std::vector<Hull> hulls;
		results = _parallel(nextStep.begin(), nextStep.end(), getRefFromRefItr, hulls);
		currCnt = _count(results);

		if (results.size() <= 1) return currCnt;
		//timer.pause();//####
	}
	//timer.resume();//####
	nextStep = _flatten(results, currCnt);
	return sequential(timer, nextStep.begin(), nextStep.end(), getRefFromRefItr);
}

template <typename Itr, typename GetRef>
int HullAlg::manualParalWithPresort(Timer& timer, Itr beg, Itr end, GetRef getRef)
{
	timer.pause();
	Timer t;
	t.start();
	auto sortedList = Marginality::sort(beg, end, getRef);
	LOG_INFO << "sort: " << t.stop();
	timer.resume();
	return manualParal(timer, sortedList.begin(), sortedList.end(), getRef);//####getRef??
}

void testAlg();

#endif