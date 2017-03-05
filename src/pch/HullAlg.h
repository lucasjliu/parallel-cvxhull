//
//  HullAlg.h
//
//	@brief: parallel convex hull algorithm
//
//  by Jiahuan.Liu
//	jiahaun.liu@outlook.com
//
//  03/04/2017
//

#include "Hull.h"

const int MIN_SIZE = NDim + 1;

class HullAlg
{
public:
	using Func = std::function<void(int tid)>;
private:
	Func null_func = [](int) {};
public:
	template <typename Itr, typename Deitr>
	static int sequential(Itr beg, Itr end, Deitr deitr);

	template <typename Itr, typename Deitr>
	static int manualParal(Itr beg, Itr end, Deitr deitr);

	template <typename Itr, typename Deitr>
	static int specuParal(Itr beg, Itr end, Deitr deitr);
//private:
	template <typename Itr, typename Deitr>
	static PointRefVec _sequential(Itr beg, Itr end, Deitr deitr, Hull& hull);

	template <typename Itr, typename Deitr>
	static std::vector<PointRefVec> _parallel(Itr beg, Itr end, Deitr deitr, std::vector<Hull>& hulls);
};