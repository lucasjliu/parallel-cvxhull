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

#include <list>

#include "Points.h"

class OriginSimplex: public std::list<PointRef>
{
	using val_t 	= Val_t;
	using facet_t 	= std::pair<PointRef, PointRef>; //2D so faect is a line segment

public:
	OriginSimplex() :_size(0) {}
	bool insert(PointRef ref);

private:
	size_t		_size;
};

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

	std::vector<PointRef> getPeaks();

private:
	Triangulation_t		_hull;
	OriginSimplex		_origin;
	bool				_initialized;
	std::hash<Point>	_ptHash;
};

#endif