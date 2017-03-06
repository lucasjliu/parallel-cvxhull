//
//  hull.h
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

#include <mpblocks/clarkson93.hpp>
#include <unordered_set>
#include <cstdint>

using namespace mpblocks;
using namespace mpblocks::clarkson93;

#define MAX_NUM 1000

typedef float Val_t;
#define NDim 2
typedef ExampleTraits2<Val_t, NDim>     Traits;
typedef Triangulation<Traits>           Triangulation_t;
typedef Triangulation_t::Point          Point;
typedef Triangulation_t::PointRef       PointRef;
typedef std::vector<PointRef>           PointRefVec;

class PointVec: public std::vector<Point>
{
	using val_t = Val_t ;
	using base_t = std::vector<Point>;

public:
	PointVec() {}

	PointVec(int num);

	void random();

	static void initRand(long seed = time(NULL));
};

class Hull
{
public:
	using hash_t = int64_t;
	using Simplex = Triangulation_t::Simplex;
	using PointHashSet = std::unordered_set<hash_t>;

public:
	Hull(int n = NDim + 1);

	bool insert(PointRef p);

	void insert(std::vector<PointRef>& pointRefs);

	void insert(PointVec& points);

	void clear();
	
	void printPeaks();

	std::vector<PointRef>& getPeaks();

private:
	hash_t _hash(Point& p);

	hash_t _hash(PointRef p);

	void _addPeak(PointRef p);

private:
	Triangulation_t     _hull;
	PointRefVec         _peaks;
	PointHashSet        _peakSet;
	PointRef            _init[NDim + 2];
	size_t              _size;
};

void testHull();

#endif