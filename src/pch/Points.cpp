//
//  Points.cpp
//
//  by Jiahuan.Liu
//	jiahaun.liu@outlook.com
//
//  03/10/2017
//

#include <numeric>

#include "Points.h"

const int g_scale = 1e6;
const int g_range = g_scale / 2;

PointVec::PointVec(int num)
{
	base_t::reserve(num);
	while (num--)
	{
		random();
	}
}

PointVec::val_t PointVec::rand()
{
    return (val_t)(((long)::rand() % g_scale) - g_range) / 1e3;
}

void PointVec::random()
{
	base_t::emplace_back(rand(), rand());
}

void PointVec::initRand(long seed) 
{
	srand((unsigned int)seed);
}

bool PointVec::operator== (const PointVec& pv) const
{
	if (this->size() != pv.size()) return false;
	PointHashSet s(this->begin(), this->end());
	return std::accumulate(pv.begin(), pv.end(), true, [&s](bool res, const Point& p)
        {return res & (s.find(p) != s.end());});
}

bool PointVec::operator!= (const PointVec& pv) const
{
	return !((*this) == pv);
}

double PointVec::jaccard (const PointVec& pv) const
{
	PointHashSet s_or(this->begin(), this->end()),
		s_and = s_or;
    for (const Point& p : pv)
        if (s_or.find(p) != s_or.end())
            s_and.insert(p);
    s_or.insert(pv.begin(), pv.end());
	return (double)s_and.size() / s_or.size();
}