//
//  ParalHull.cpp
//
//	by Jiahuan.Liu
//	jiahaun.liu@outlook.com
//
//  03/04/2017
//

#include <stdio.h>
#include <algorithm>
#include <unordered_set>

#include "ParalHull.h"
#include "Hull.h"
#include "omp.h"

PointRefVec ParalHull::getRefs(const PointVec& vec)
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
PointVec ParalHull::getPts(const PointRefVec& vec)
{
	PointVec res;
	res.reserve(vec.size());
	for (const auto& ref : vec)
	{
		res.push_back(*ref);
	}
	return std::move(res);
}