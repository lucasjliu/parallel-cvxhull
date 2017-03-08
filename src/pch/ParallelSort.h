//
//  ParallelSort.h
//
//	@brief: marginality sort of vector of points with 2 dimensionality
//
//	by Jiahuan.Liu
//	jiahaun.liu@outlook.com
//
//  03/06/2017
//

#ifndef _PARALLEL_SORT_H
#define _PARALLEL_SORT_H

#include <vector>
#include <iterator>
#include <type_traits>
#include <algorithm>

#include "omp.h"
#include "Logger.h"
#include "UnitTest.h"

class ParallelSort
{
public:
	template <typename Itr>
	static void mergesort(Itr beg, Itr end, int thrNum);

	template <typename Itr>
	static void oddeven(Itr beg, Itr end, int thrNum);
};

class MergeSort
{
public:
	//std::less<> slightly slower than std::sort####
	template <typename Itr, 
		typename Comp=std::less<typename std::iterator_traits<Itr>::value_type> >
	static void sort(Itr beg, Itr end, int thrNum = 1, Comp comp = Comp());

private:
	template <typename Itr, typename Comp>
	static void _merge(Itr beg, Itr mid, Itr end, 
		typename std::vector<typename std::iterator_traits<Itr>::value_type>::iterator bufItr, 
		Comp comp);

	template <typename Itr, typename Comp>
	static void _parallel_recursive(Itr beg, Itr end, 
		typename std::vector<typename std::iterator_traits<Itr>::value_type>::iterator bufItr, 
		int thrNum, Comp comp);

	template <typename Itr, typename Comp>
	static void _parallel_iterative(Itr beg, Itr end, 
		typename std::vector<typename std::iterator_traits<Itr>::value_type>::iterator bufItr, 
		int thrNum, Comp comp) {}

	template <typename Itr, typename Comp>
	static void _sequential(Itr beg, Itr end, Comp comp);

};

template <typename Itr, typename Comp>
void MergeSort::sort(Itr beg, Itr end, int thrNum, Comp comp)
{
	if (thrNum <= 0) return;

	if (thrNum == 1)
	{
		_sequential(beg, end, comp);
		return;
	}

	const int size = end - beg;
	
	if (size <= 0) return;

	using val_t = typename std::iterator_traits<Itr>::value_type;
	std::vector<val_t> buf(size);

	_parallel_recursive(beg, end, buf.begin(), thrNum, comp);
}

template <typename Itr, typename Comp>
void MergeSort::_parallel_recursive(Itr beg, Itr end, 
	typename std::vector<typename std::iterator_traits<Itr>::value_type>::iterator bufItr, 
	int thrNum, Comp comp)
{
	LOG_INFO << omp_get_thread_num() << ": " << beg - beg << " " << end - beg << " " << thrNum << " " << omp_get_num_threads();

	if (thrNum <= 0) return;

	if (thrNum == 1)
	{
		_sequential(beg, end, comp);
		return;
	}

	const int size = end - beg;

	if (size <= 0) 
	{
		LOG_WARN << "array size exception.";
		return;
	}

	Itr mid = size / 2 + beg;

	//if thread numbers over 1, sort in parallel
	#pragma omp parallel sections num_threads(thrNum)
	{
		#pragma omp section
		_parallel_recursive(beg, mid, bufItr, thrNum / 2, comp);
		#pragma omp section
		_parallel_recursive(mid, end, bufItr + (mid - beg), thrNum - thrNum / 2, comp);
	}
	
	_merge(beg, mid, end, bufItr, comp);
}

template <typename Itr, typename Comp>
void MergeSort::_sequential(Itr beg, Itr end, Comp comp)
{
	std::sort(beg, end, comp);
}

template <typename Itr, typename Comp>
void MergeSort::_merge(Itr beg, Itr mid, Itr end, 
	typename std::vector<typename std::iterator_traits<Itr>::value_type>::iterator bufItr, 
	Comp comp)
{
	using BufItr = decltype(bufItr);

	const Itr beg0 = beg, end0 = mid, beg1 = mid;
	const BufItr bufBeg = bufItr;

	for (Itr itr = beg0; itr != end0; itr++, bufItr++)
	{//copy first half to buf, so that can directly write to original space
		(*bufItr) = (*itr);
	}

	const BufItr bufEnd = bufItr;
	
	//iterator for writing result, reading first half, reading second half
	Itr itr = beg0, itr0 = bufBeg, itr1 = beg1;

	while (itr0 != bufEnd)
	{//if itr1 == end1, remaining order is correct, no need to copy
		if (itr1 == beg1 || comp((*itr0), (*itr1)) )
		{
			(*itr++) = (*itr0++);
		}
		else
		{
			(*itr++) = (*itr1++);
		}
	}
}

template <typename Itr>
void ParallelSort::mergesort(Itr beg, Itr end, int thrNum)
{
	MergeSort::sort(beg, end, thrNum);
}

void testSort();

#endif //_PARALLEL_SORT_H