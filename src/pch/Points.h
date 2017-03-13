//
//  Points.h
//
//	@brief: type and data struture of point and points container
//
//  by Jiahuan.Liu
//	jiahaun.liu@outlook.com
//
//  03/10/2017
//

#ifndef _POINTS_H
#define _POINTS_H

#include <mpblocks/clarkson93.hpp>
#include <unordered_set>
#include <functional>

using namespace mpblocks;
using namespace mpblocks::clarkson93;

#define MAX_NUM 1000

typedef double Val_t;
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

	bool operator== (const PointVec& pv) const;

	bool operator!= (const PointVec& pv) const;

	double jaccard (const PointVec& pv) const;

	static val_t rand();
};

typedef std::size_t hash_t;
typedef std::unordered_set<Point> PointHashSet;
typedef std::unordered_set<PointRef> PointRefHashSet;

namespace std
{
	template <> struct hash<Point>
	{
		hash_t operator()(const Point & p) const
		{
			return std::hash<Val_t>{}(p[0]) ^
				(std::hash<Val_t>{}(p[1]) << (sizeof(hash_t) / 2 * 8));
		}
	};
}

#endif