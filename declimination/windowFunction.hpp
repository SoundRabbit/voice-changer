#define _USE_MATH_DEFINES
#include <cmath>



namespace WindowFunction{
	constexpr auto sine = [](double x)->double{return sin(M_PI * x);};
	constexpr auto vorbis = [](double x)->double{return sin(M_PI / 2 * pow(sin(M_PI * x), 2));};
	constexpr auto rect = [](double x)->double{return 1;};
	constexpr auto bartlett = [](double x)->double{return 1 - 2 * abs(x - 0.5);};
	constexpr auto blackman = [](double x)->double{return 0.42 - 0.5 * cos(2 * M_PI * x) + 0.08 * cos(4 * M_PI * x);};

	class Gauss{
	protected:
		double sigma;
	public:
		Gauss(double s):sigma(s){}
		double operator() (double x){return exp(pow(x,2)/pow(sigma,2));}
	};

	template<typename RandomAccessIterator, class Wfunction>
	void apply(RandomAccessIterator first, RandomAccessIterator last, Wfunction wf);
}