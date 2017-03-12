//
//  Marginality.h
//
//	@brief: marginality sort of vector of points with 2 dimensionality
//
//	by Jiahuan.Liu
//	jiahaun.liu@outlook.com
//
//  03/05/2017
//

#ifndef _MARGINALITY_H
#define _MARGINALITY_H

#include <vector>
#include <type_traits>
#include <math.h>
#include <omp.h>

#include "Points.h" //NDim
#include "ParallelSort.h"

class Marginality
{
public:
	using val_t = double;
public:
	template <typename Itr, typename GetRef, 
		typename R = std::vector<typename std::iterator_traits<Itr>::value_type> >
	static R sort(Itr beg, Itr end, GetRef getRef);
private:
	static val_t entropy(val_t v1, val_t v2);
	static val_t entropy(val_t v1, val_t v2, val_t sum);
	static void accumulate(val_t& v1, val_t v2);
	static void initialize(val_t& v);
};

template <typename Itr, typename GetRef, 
	typename R>
R Marginality::sort(Itr beg, Itr end, GetRef getRef)
{
	const int size = end - beg;

	std::vector<int> vRanks[NDim];

	std::vector<Itr> idx2itr;
	idx2itr.reserve(size);
	for (Itr itr = beg; itr != end; ++itr)
	{
		idx2itr.push_back(itr);
	}

	#pragma omp parallel for schedule(dynamic, 1) shared(vRanks, getRef, idx2itr)
	for (int d = 0; d < NDim; ++d)
	{
		std::vector<std::pair<val_t, int>> pos;
		pos.reserve(size);
		
		for (int i = 0; i < size; ++i)
		{
			pos.emplace_back((*getRef(idx2itr[i]))[d], i);
		}

		vRanks[d] = std::vector<int>(size);
		std::vector<int>& ranks = vRanks[d];

		Timer t;
		t.start();
		ParallelSort::mergesort(pos.begin(), pos.end(), 2);//##TODO: thrNum
		//LOG_INFO << "sort: " << t.stop();

		//#pragma omp parallel for schedule(static) shared(ranks, pos, d)
		for (int i = 0; i < pos.size(); ++i)
		{
			auto currIdx = pos[i].second;
			ranks[currIdx] = i;
		}
	}

	std::vector<std::pair<val_t, int>> vals(size);

	//#pragma omp parallel for schedule(static) shared(vals)
	for (int i = 0; i < size; ++i)
	{
		initialize(vals[i].first);
		vals[i].second = i;
	}
	//#pragma omp parallel for schedule(static) shared(vRanks, vals) collapse(2)
	for (int d = 0; d < NDim; ++d)
	{
		for (int i = 0; i < size; ++i)
		{
			val_t rank = vRanks[d][i];
			accumulate(vals[i].first, entropy(rank, size - rank - 1, size));
		}
	}

	Timer t;
	t.start();
	ParallelSort::mergesort(vals.begin(), vals.end(), 2);//####TODO: thrNum
	//LOG_INFO << "sort: " << t.stop();
	
	R res;
	res.reserve(size); //####container operations on R object should be wrap as template specialization
	
	for (auto& val : vals)
	{//avoid random access requirement of R
		////
		//LOG_INFO << val.first << " " << (*getRef(idx2itr[val.second]))[0] << " " << (*getRef(idx2itr[val.second]))[1];
		res.push_back(*idx2itr[val.second]);
	}
	return res;
}

void testMarginalitySort();

#endif