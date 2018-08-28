#define _USE_MATH_DEFINES
#include <cmath>



namespace WindowFunction{
	auto sine = [](double x)->double{return sin(M_PI * x);};
	auto vorbis = [](double x)->double{return sin(M_PI / 2 * pow(sin(M_PI * x), 2));};

	template<typename RandomAccessIterator, class Wfunction>
	void apply(RandomAccessIterator first, RandomAccessIterator last, Wfunction wf);
}