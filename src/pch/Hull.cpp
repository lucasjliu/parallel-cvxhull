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

Hull::Hull()
{
    m_ptStore.reserve(MAX_NUM);
    m_hull.m_antiOrigin = 0;
    m_hull.m_sMgr.reserve(MAX_NUM);
}

bool Hull::addPoint(val_t x, val_t y)
{
    m_ptStore.emplace_back(x,y);
    bool isPeak = true;
    if (m_ptStore.size() == 3)
        m_hull.init( &m_ptStore[0], &m_ptStore[3], [](Point* ptr){return ptr;} );
    else if (m_ptStore.size() > 3)
        isPeak = m_hull.insert(&m_ptStore.back());
    //if (isPeak)
        //m_peaks.insert(&m_ptStore.back());
    return isPeak;
}

void Hull::clear()
{
    m_hull.m_xv_walked.clear();
    m_hull.m_xvh.clear();
    m_hull.m_ridges.clear();

    m_hull.clear();
    m_ptStore.clear();
}

void Hull::random()
{
    addPoint(  rand() / (val_t)RAND_MAX,
                rand() / (val_t)RAND_MAX  );
}

void Hull::initRand(long seed) 
{
    srand((unsigned int)seed);
}

void Hull::initRand() 
{
    srand((unsigned int)time(NULL));
}

bool Hull::autoStep()
{
    random();
    return true;
}

void Hull::printPeaks()
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

int Hull::getPeakNum()
{
    return m_peaks.size();
}

void testHull()
{
    Hull hull;
    assert(hull.addPoint(0,0));
	assert(hull.addPoint(0.2,0));
	assert(hull.addPoint(0,0.2));
	assert(!hull.addPoint(0.05,0.1));
	assert(hull.addPoint(0.1,0.15));
    assert(hull.addPoint(0.2,0.2));
	hull.printPeaks();
    std::cout << hull.getPeakNum() << std::endl;
    //assert(hull.getPeakNum() == 4);
}