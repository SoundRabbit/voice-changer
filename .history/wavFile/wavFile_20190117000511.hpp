#include <cstdint>
#include <string>
#include <vector>



#pragma once



class WavFile{
	public:
		std::vector<double> data;
		uint16_t quantifyingBitNum;
		uint32_t samplingRate;

		bool load(std::string fileName);
		bool save(std::string fileName);
};