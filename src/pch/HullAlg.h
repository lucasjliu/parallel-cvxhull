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
	using ret_type = PointVec;
public:
	//
	// @brief: public interface of algorithm
	// @param: beg, end: specify input points, should be RandomAccessItrator
	// 		   getRef: method to get PointRef from itr
	// @return: number of anti-origin points in resulting polygon
	//
	template <typename Itr, typename GetRef>
	static ret_type sequential(Timer& timer, Itr beg, Itr end, GetRef getRef, bool bSort = false);

	template <typename Itr, typename GetRef>
	static ret_type manualParal(Timer& timer, Itr beg, Itr end, GetRef getRef, int prevCnt, bool bSort = false);

	template <typename Itr, typename GetRef>
	static ret_type manualParal(Timer& timer, Itr beg, Itr end, GetRef getRef, bool bSort = false);

	template <typename Itr, typename GetRef>
	static ret_type manualParalWithPresort(Timer& timer, Itr beg, Itr end, GetRef getRef);

	template <typename Itr, typename GetRef>
	static ret_type specuParal(Timer& timer, Itr beg, Itr end, GetRef getRef);

	static PointRefVec getRefs(const PointVec& vec);
	static PointVec getPts(const PointRefVec& vec);
private:
	//
	// @brief: internal implementation of parallelization
	// @param: hull, hulls: for hull structures return
	// @return: Point vector of hull points
	//
	/*template <typename Itr, typename GetRef>
	static PointRefVec _sequential(Itr beg, Itr end, GetRef getRef, Hull& hull);

	template <typename Itr, typename GetRef>
	static std::vector<PointRefVec> _parallel(Itr beg, Itr end, GetRef getRef, std::vector<Hull>& hulls);*/

	template <typename Itr, typename GetRef>
	static PointVec _sequential(Itr beg, Itr end, GetRef getRef, Hull& hull, bool bSort);

	template <typename Itr, typename GetRef>
	static std::vector<PointVec> _parallel(Itr beg, Itr end, GetRef getRef, std::vector<Hull>& hulls, bool bSort);

	template <typename Vec>
	static int _count(const std::vector<Vec>& vecs);

	template <typename Vec>
	static Vec _flatten(const std::vector<Vec>& vecs, int count);

	template <typename Vec>
	static Vec _flatten(const std::vector<Vec>& vecs);
};

template <typename Itr, typename GetRef>
PointVec HullAlg::_sequential(Itr beg, Itr end, GetRef getRef, Hull& hull, bool bSort)
{
	////
	//LOG_INFO << "Seq: " << end - beg << " from " << beg - beg << " to " << end - beg;

	std::vector<Point> sortedList;

	if (bSort)
	{
		Timer t;
		t.start();
		sortedList = Marginality::sort(beg, end, getRef);
		//####TODO: getRef
		auto getSortedRef = [](std::vector<Point>::iterator itr){return &(*itr);};
		////
		//LOG_INFO << "sort: " << t.stop();
		for (Point& p: sortedList)
		{
			//LOG_INFO << "(" << p[0] << "," << p[1] << ")";
			hull.insert(&p);
		}
	}
	else
	{
		for (Itr itr = beg; itr != end; ++itr)
		{
			//LOG_INFO << "(" << (*getRef(itr))[0] << "," << (*getRef(itr))[1] << ")";
			hull.insert(getRef(itr));
		}
	}

	////
	auto peaks = hull.getPeaks();
	for (auto& p : peaks)
	{
		//LOG_INFO << "Hull: " << (*p)[0] << " " << (*p)[1];
	}

	return getPts(hull.getPeaks());
}

template <typename Itr, typename GetRef>
std::vector<PointVec> HullAlg::_parallel(Itr beg, Itr end, GetRef getRef, std::vector<Hull>& hulls, bool bSort)
{
	const int size = end - beg;
	int thrNum = THR_NUM;//####TODO
	int len = ceil(size / thrNum);
	
	while (len <= MIN_SIZE && thrNum > 1)
	{
		thrNum /= 2;
		len = ceil(size / thrNum);
	}

	hulls.reserve(thrNum);
	for (int i = 0; i < thrNum; ++i)
	{
		hulls.emplace_back(len);
	}

	std::vector<PointVec> results(thrNum);

	#pragma omp parallel shared(results, len, beg, end, getRef, hulls) num_threads(thrNum) //num_threads must be set
	{
		int tid = omp_get_thread_num();
		Itr first = beg + len * tid, 
			last = (tid == thrNum - 1 ? end : first + len);
			
		auto result = _sequential(first, last, getRef, hulls[tid], bSort);
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
	Vec res;
	res.reserve(count);
	for (auto& vec: vecs)
		for (auto& elem: vec)
			res.push_back(elem);
	return res;
}

template <typename Vec>
Vec HullAlg::_flatten(const std::vector<Vec>& vecs)
{
	return _flatten(vecs, _count(vecs));
}

template <typename Itr, typename GetRef>
HullAlg::ret_type HullAlg::sequential(Timer& timer, Itr beg, Itr end, GetRef getRef, bool bSort)
{
	int size = end - beg;
	Hull hull(size);
	return _sequential(beg, end, getRef, hull, bSort);
}

template <typename Itr, typename GetRef>
HullAlg::ret_type HullAlg::manualParal(Timer& timer, Itr beg, Itr end, GetRef getRef, int prevCnt, bool bSort)
{
	std::vector<Hull> hulls;
	auto results = _parallel(beg, end, getRef, hulls, bSort);
	int currCnt = _count(results);

	if (results.empty()) return {};
	else if (results.size() == 1) return std::move(results.front());

	//timer.pause();//####
	auto nextStep = _flatten(results, currCnt);
	//auto getRefFromRefItr = [](PointRefVec::iterator itr){return *itr;};
	auto getRefFromPtItr = [](PointVec::iterator itr){return &(*itr);};

	hulls.clear();
	//timer.resume();//####

	return (currCnt == prevCnt) ? 
		sequential(timer, nextStep.begin(), nextStep.end(), getRefFromPtItr, bSort) :
		manualParal(timer, nextStep.begin(), nextStep.end(), getRefFromPtItr, currCnt, bSort);
}

template <typename Itr, typename GetRef>
HullAlg::ret_type HullAlg::manualParal(Timer& timer, Itr beg, Itr end, GetRef getRef, bool bSort)
{
	std::vector<Hull> hulls;
	auto results = _parallel(beg, end, getRef, hulls, bSort);
	int currCnt = _count(results), prevCnt = end - beg;

	if (results.empty()) return {};
	else if (results.size() == 1) return std::move(results.front());

	//auto getRefFromRefItr = [](PointRefVec::iterator itr){return *itr;};
	auto getRefFromPtItr = [](PointVec::iterator itr){return &(*itr);};

	//timer.pause();//####
	while (currCnt != prevCnt)
	{
		prevCnt = currCnt;

		auto nextStep = _flatten(results, currCnt);
		//timer.resume();//####

		std::vector<Hull> hulls;
		results = _parallel(nextStep.begin(), nextStep.end(), getRefFromPtItr, hulls, bSort);
		currCnt = _count(results);

		if (results.empty()) return {};
		else if (results.size() == 1) return std::move(results.front());
		//timer.pause();//####
	}
	//timer.resume();//####
	auto nextStep = _flatten(results, currCnt);
	return sequential(timer, nextStep.begin(), nextStep.end(), getRefFromPtItr, bSort);
}

template <typename Itr, typename GetRef>
HullAlg::ret_type HullAlg::manualParalWithPresort(Timer& timer, Itr beg, Itr end, GetRef getRef)
{
	return manualParal(timer, beg, end, getRef, true);
}

void testAlg(int seed, int size, int loop = 1000);

#endif