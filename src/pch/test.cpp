//
//  test.cpp
//
//	@brief: test case for convex hull algorithm
//
//  by Jiahuan.Liu
//	jiahaun.liu@outlook.com
//
//  02/26/2017
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

#include <iostream>
#include <stdio.h>
#include <time.h>

struct Hull
{
    typedef Val_t val_t;
    
    std::vector<Point>  m_ptStore;
    Triangulation_t     m_hull;

    Hull()
    {
        m_ptStore.reserve(MAX_NUM);
        m_hull.m_antiOrigin = 0;
        m_hull.m_sMgr.reserve(MAX_NUM);
    }

    bool addPoint(val_t x, val_t y)
    {
        m_ptStore.emplace_back(x,y);
        if( m_ptStore.size() < 3 )
            return true;
        else if( m_ptStore.size() == 3)
		{
            m_hull.init( &m_ptStore[0], &m_ptStore[3], [](Point* ptr){return ptr;} );
			return true;
		}
        else
            return m_hull.insert(&m_ptStore.back());
		//printSimplexPeaks();
    }

    void clear()
    {
        m_hull.m_xv_walked.clear();
        m_hull.m_xvh.clear();
        m_hull.m_ridges.clear();

        m_hull.clear();
        m_ptStore.clear();
    }

    void random()
    {
        addPoint(  rand() / (val_t)RAND_MAX,
                   rand() / (val_t)RAND_MAX  );
    }

    void initRand(long seed) 
    {
	    srand((unsigned int)seed);
    }

    void initRand() 
    {
	    srand((unsigned int)time(NULL));
    }

    bool autoStep()
    {
        random();
        return true;
    }

    void printSimplexPeaks()
	{
		std::cout << m_hull.m_sMgr.size() << std::endl;
        for( Simplex& S : m_hull.m_sMgr )
		{
			if( !S.sets[ clarkson93::simplex::XV_HULL] || !S.V[S.iPeak] )
				continue;
			Point peak = *(S.V[S.iPeak]);
			printf("%.2f %.2f\n", (peak)[0], (peak)[1]);
        }
    }
	
	void print()
	{
		for( Simplex& S : m_hull.m_sMgr )
		{
			//if (!m_hull.peak(S)) continue;
			//Point peak = *m_hull.peak(S);
			if ( !S.V[S.iPeak] ) continue;
			Point peak = *(S.V[S.iPeak]);
			printf("%.2f %.2f\n", (peak)[0], (peak)[1]);
		}
	}
};

int main()
{
    Hull hull;
	hull.addPoint(0,0);
	hull.addPoint(0.2,0);
	hull.addPoint(0,0.2);
	hull.addPoint(0.15,0.1);
	hull.addPoint(0.1,0.15);
    hull.addPoint(0.2,0.2);
	//hull.print();
}