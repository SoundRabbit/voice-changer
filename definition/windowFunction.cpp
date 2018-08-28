#include "../declimination/windowFunction.hpp"
#include <iterator>


namespace WindowFunction{
	template<typename RandomAccessIterator, class Wfunction>
	void apply(RandomAccessIterator first, RandomAccessIterator last, Wfunction wf){
		using namespace std;

		const std::size_t n = std::distance(first, last) + 2;
		int count = 1;
		auto itr = first;
		while(itr != last){
			const double x = (double)count / n;
			*itr = (*itr) * wf(x);
			itr++;
			count++;
		}
		return;
	}	
}