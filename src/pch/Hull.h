//
//  Hull.h
//
//	@brief: incremental convex hull data struture
//
//  by Jiahuan.Liu
//	jiahaun.liu@outlook.com
//
//  03/03/2017
//

#ifndef _HULL_H
#define _HULL_H

#include "Points.h"

class Hull
{
public:
	using Simplex = Triangulation_t::Simplex;

public:
	Hull(int n = NDim + 1);

	bool insert(PointRef p);

	void insert(std::vector<PointRef>& pointRefs);

	void insert(PointVec& points);

	void clear();
	
	void printPeaks();

	std::vector<PointRef> getPeaks();

private:
	Triangulation_t     _hull;
	PointRef            _origin[NDim + 2];
	size_t              _size;
};

#endif