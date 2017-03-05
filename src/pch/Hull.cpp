//
//  hull.cpp
//
//  by Jiahuan.Liu
//	jiahaun.liu@outlook.com
//
//  03/03/2017
//

#include <iostream>
#include <stdio.h>
#include <time.h>

#include "hull.h"

PointVec::PointVec(int num)
{
    base_t::reserve(num);
    while (num--)
    {
        random();
    }
}

void PointVec::random()
{
    base_t::emplace_back(rand() / (val_t)RAND_MAX,
                         rand() / (val_t)RAND_MAX);
}

void PointVec::initRand(long seed) 
{
    srand((unsigned int)seed);
}

Hull::Hull() :_size(0)
{
    _hull.m_antiOrigin = 0;
    _hull.m_sMgr.reserve(MAX_NUM);
}

bool Hull::insert(PointRef p)
{
    if (_size < 3)
    {
        _init[_size] = p;
        _addPeak(p);
    }

    _size++;
    bool isPeak = true;

    if (_size == 3)
    {
        _hull.init(_init, _init + 3, [](PointRef* pRef){return *pRef;});
    }
    else if (_size > 3)
    {
        isPeak = _hull.insert(p);
    }

    return isPeak;
}

void Hull::insert(std::vector<PointRef>& pointRefs)
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

Hull::hash_t Hull::_hash(PointRef p)
{
    return reinterpret_cast<hash_t&>((*p)[0]) |
        (reinterpret_cast<hash_t&>((*p)[1]) << 32);
}

Hull::hash_t Hull::_hash(Point& p)
{
    return _hash(&p);
}

void Hull::_addPeak(PointRef p)
{
    double hash = _hash(p);
    if (_peakSet.find(hash) == _peakSet.end())
    {
        _peaks.push_back(p);
        _peakSet.insert(hash);
    }
}

void Hull::clear()
{
    _hull.m_xv_walked.clear();
    _hull.m_xvh.clear();
    _hull.m_ridges.clear();
    _hull.clear();
}

void Hull::printPeaks()
{
    for(auto& S : _hull.m_sMgr)
    {
        if ( !S.V[S.iPeak] ) continue;
        Point peak = *(S.V[S.iPeak]);
        printf("%.2f %.2f\n", (peak)[0], (peak)[1]);
    }
}

std::vector<Hull::PointRef>& Hull::getPeaks()
{
    for(auto& S : _hull.m_sMgr)
    {
        PointRef p = S.V[S.iPeak];
        if (p) _addPeak(p);
    }
    return _peaks;
}

void testHull()
{
    PointVec points;
    points.emplace_back(0,0);
    points.emplace_back(0.2,0);
    points.emplace_back(0,0.2);
    points.emplace_back(0.05,0.1);
    points.emplace_back(0.1,0.15);
    points.emplace_back(0.2,0.2);

    Hull hull;
    assert(hull.insert(&points[0]));
	assert(hull.insert(&points[1]));
	assert(hull.insert(&points[2]));
	assert(!hull.insert(&points[3]));
	assert(hull.insert(&points[4]));
    assert(hull.insert(&points[5]));
	hull.printPeaks();
    assert(hull.getPeaks().size() == 5);
}