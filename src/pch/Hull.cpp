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

Hull::Hull(int n) :_size(0)
{
	_hull.m_antiOrigin = 0;
	_hull.m_sMgr.reserve((NDim + 1) * n);
}

bool Hull::insert(PointRef p)
{
	if (_size < 3)
	{
		_origin[_size] = p;
	}

	_size++;
	
	bool isPeak = true;

	if (_size == 3)
	{
		_hull.init(_origin, _origin + 3, [](PointRef* pRef){return *pRef;});
	}
	else if (_size > 3)
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

#include <stdio.h>

void Hull::printPeaks()
{
	for( Simplex& S : _hull.m_sMgr )
	{
		if( !S.sets[ clarkson93::simplex::HULL] )
			continue;
		
		Point* peak = S.V[S.iPeak];
		std::vector<Point*> p;
		std::copy_if( S.V, S.V+3,
						std::back_inserter(p),
						[peak](Point* v){ return v != peak; } );
		
		Point     p1 = *(p[0]);
		Point     p2 = *(p[1]);
		
		printf("(%f, %f) -> (%f, %f)\n",
				p1[0], p1[1], p2[0], p2[1]);
		
		//ectx.move_to(p1);
		//ectx.line_to(p2);
	}
}

PointRefVec Hull::getPeaks()
{
	PointRefVec peaks;
	PointRefHashSet peakSet;

	for(auto& S : _hull.m_sMgr)
	{
		if(!S.sets[clarkson93::simplex::HULL]) continue;
		for (int i = 0; i < NDim + 1; ++i) //should have better ending condition####
		{
			if (i == S.iPeak) continue;
			auto hash = S.V[i];
			if (peakSet.find(hash) == peakSet.end())
			{
				peaks.push_back(S.V[i]);
				peakSet.insert(hash);
			}
		}
	}
	return std::move(peaks);
}