#include <iterator>

template<typename SampleType, typename RandomAccessIterator, class Wfunction1, class Next, class Wfunction2>
void overlapAdd(RandomAccessIterator first, RandomAccessIterator last,std::size_t width, Wfunction1 wf1, Next next, Wfunction2 wf2);



#include "overlapAdd.cpp"