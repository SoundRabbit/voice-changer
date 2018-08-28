#include <iterator>

template<typename RandomAccessIterator, class WindowFunction, class Next>
void overlapAdd(RandomAccessIterator first, RandomAccessIterator last,std::size_t width, WindowFunction wf, Next next);