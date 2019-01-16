#include "../declimination/overlapAdd.hpp"
#include "../windowFunction.hpp"
#include <iostream> //debug
#include <vector>



template<typename SampleType, typename RandomAccessIterator, class Wfunction1, class Next, class Wfunction2>
void overlapAdd(RandomAccessIterator first, RandomAccessIterator last,std::size_t width, Wfunction1 wf1, Next next, Wfunction2 wf2){
	using namespace std;

	const std::size_t step = width / 2;
	auto inputItr = first;

	vector<SampleType> result(distance(first, last), 0);
	auto outputItr = result.begin();

	int debugCounter = 0;

	while(width <= distance(inputItr ,last)){
		vector<SampleType> buf(inputItr, inputItr + width);

		WindowFunction::apply(buf.begin(), buf.end(), wf1);

		next(buf.begin(), buf.end());

		WindowFunction::apply(buf.begin(), buf.end(), wf2);


		for(auto& s : buf){
			(*outputItr) += s;
			outputItr++;
		}
		outputItr -= step;

		inputItr += step;
	}

	copy(result.begin(), result.end(), first);
}