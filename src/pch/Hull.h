//
//  hull.h
//
//	@brief: convex hull algorithm class
//
//  by Jiahuan.Liu
//	jiahaun.liu@outlook.com
//
//  03/03/2017
//

#include <mpblocks/clarkson93.hpp>
#include <unordered_set>

using namespace mpblocks;
using namespace mpblocks::clarkson93;

#define MAX_NUM 100000

typedef double Val_t;
#define NDim 2
typedef ExampleTraits2<Val_t, NDim> Traits;

class Hull
{
    using val_t = Val_t ;
    using Triangulation_t = Triangulation<Traits>;
    using Simplex = Triangulation_t::Simplex;
    using Point = Triangulation_t::Point;
    using PointRef = Triangulation_t::PointRef;
    using PointHashSet = std::unordered_set<double>;
    
    std::vector<Point>      m_ptStore;
    Triangulation_t         m_hull;
    std::vector<PointRef>   m_peaks;

public:
    Hull();

    bool addPoint(val_t x, val_t y);

    void clear();

    void random();

    void initRand(long seed);

    void initRand();

    bool autoStep();
	
	void printPeaks();

    int getPeakNum();

private:
    //double 
};

void testHull();