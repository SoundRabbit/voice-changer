#define _USE_MATH_DEFINES
#include <cmath>



namespace WindowFunction{
	constexpr auto sine = [](double x)->double{return sin(M_PI * x);};
	constexpr auto vorbis = [](double x)->double{return sin(M_PI / 2 * pow(sin(M_PI * x), 2));};
	constexpr auto rect = [](double x)->double{return 1;};

	template<typename RandomAccessIterator, class Wfunction>
	void apply(RandomAccessIterator first, RandomAccessIterator last, Wfunction wf);
}