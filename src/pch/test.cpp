#include <mpblocks/clarkson93.hpp>

using namespace mpblocks;
using namespace mpblocks::clarkson93;

typedef double Val_t;
//typedef Eigen::Matrix<Val_t,2,1> Vector;
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

    void addPoint(val_t x, val_t y)
    {
        std::cout << "add: " << x << " " << y << std::endl;
        m_ptStore.emplace_back(x,y);
        if( m_ptStore.size() < 3 )
            return;
        else if( m_ptStore.size() == 3)
            m_hull.init( &m_ptStore[0], &m_ptStore[3], [](Point* ptr){return ptr;} );
        else
            m_hull.insert(&m_ptStore.back());
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
	    srand(time(NULL));
    }

    bool autoStep()
    {
        random();
        return true;
    }

    void print()
    {
        std::cout << m_hull.m_sMgr.size() << std::endl;
        for( Simplex& S : m_hull.m_sMgr )
        {
            std::cout << 1 << std::endl;
            if( !S.sets[ clarkson93::simplex::HULL] )
                continue;

            Point peak = *(S.V[S.iPeak]);
            printf("%.2f %.2f\n", (peak)[0], (peak)[0]);
            /*std::vector<Point*> p;
            std::copy_if( S.V, S.V+3,
                std::back_inserter(p),
                [peak](Point* v){ return v != peak; } );

            Point p1 = *(p[0]);
            Point p2 = *(p[1]);*/

            //printf("%.2f %.2f\n%.2f %.2f\n\n", p1[0], p1[1], p2[0], p2[1]);
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
    hull.print();

    /*
    srand(time(NULL));
    std::cout << rand() / (double)RAND_MAX << std::endl
        << rand() / (double)RAND_MAX << std::endl
        << rand() / (double)RAND_MAX << std::endl
        << rand() / (double)RAND_MAX << std::endl
        << rand() / (double)RAND_MAX << std::endl
        << rand() / (double)RAND_MAX << std::endl
        << rand() / (double)RAND_MAX << std::endl;
    */
}