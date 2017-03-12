//
//  Hull.cpp
//
//  by Jiahuan.Liu
//	jiahaun.liu@outlook.com
//
//  03/03/2017
//

#include <iostream>
#include <stdio.h>
#include <time.h>
#include <math.h>

#include "Hull.h"

Hull::Hull(int n) :_initialized(false)
{
	_hull.m_antiOrigin = 0;
	_hull.m_sMgr.reserve((NDim + 1) * n);
}

bool Hull::insert(PointRef p)
{
	bool isPeak = true;

	if (!_initialized)
	{
		if (_origin.insert(p))
		{
			_hull.init(_origin.begin(), _origin.end(), 
				[](OriginSimplex::iterator itr){return *itr;});
			_initialized = true;
		}
	}
	else
	{
		isPeak = _hull.insert(p);
	}

	return isPeak;
}

void Hull::insert(PointRefVec& pointRefs)
{
	for (auto& p : pointRefs)
	{
		insert(p);
	}
}

void Hull::insert(PointVec& points)
{
	for (auto& p : points)
	{
		insert(&p);
	}
}

void Hull::clear()
{
	_hull.m_xv_walked.clear();
	_hull.m_xvh.clear();
	_hull.m_ridges.clear();
	_hull.clear();
}

PointRefVec Hull::getPeaks()
{
	PointRefVec peaks;
	PointHashSet peakSet;

	if (_initialized)
	{
		for(auto& S : _hull.m_sMgr)
		{
			if(!S.sets[clarkson93::simplex::HULL]) continue;
			for (int i = 0; i < NDim + 1; ++i) //should have better ending condition####
			{
				if (i == S.iPeak) continue;
				auto& hash = *(S.V[i]);
				if (peakSet.find(hash) == peakSet.end())
				{
					peaks.push_back(S.V[i]);
					peakSet.insert(hash);
				}
			}
		}
	}
	else
	{
		for (auto ref : _origin)
		{
			peaks.push_back(ref);
		}
	}
	
	return std::move(peaks);
}

bool OriginSimplex::insert(PointRef ref)
{
	bool done = false;

	if (_size < 2)
	{
		if (_size == 1 && (*ref == *front())) return false;
		push_back(ref);
		_size++;
	}
	else
	{
		val_t dx0 = (*ref)[0] - (*front())[0],
			dx1 = (*ref)[0] - (*back())[0],
			dy0 = (*ref)[1] - (*front())[1],
			dy1 = (*ref)[1] - (*back())[1];
		if ((!dx0 && !dx1) || (!dy0 && !dy1)) {} //duplicate
		else if (dx0 * dy1 == dx1 * dy0)
		{//in a line
			if ( !(dx0>0 ^ dx1>0) && !(dy0>0 ^ dy1>0) )
			{//out of facet
				std::abs(dx0) < std::abs(dx1) ? pop_front() : pop_back();
				push_back(ref);
			}
		}
		else
		{
			done = true;
			push_back(ref);
		}
	}
	
	return done;
}