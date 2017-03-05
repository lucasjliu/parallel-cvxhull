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

using namespace mpblocks;
using namespace mpblocks::clarkson93;

typedef double Val_t;
typedef Eigen::Matrix<Val_t,2,1> Point;

typedef ExampleTraits2<Val_t,2>  Traits;
typedef typename Traits::Simplex  Simplex;
typedef Triangulation<Traits>     Triangulation_t;

#define MAX_NUM 100000

class Hull
{
    typedef Val_t val_t;
    
    std::vector<Point>  m_ptStore;
    Triangulation_t     m_hull;

public:
    Hull();

    bool addPoint(val_t x, val_t y);

    void clear();

    void random();

    void initRand(long seed);

    void initRand();

    bool autoStep();
	
	void printPeaks();
};