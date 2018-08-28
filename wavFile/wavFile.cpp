#define _USE_MATH_DEFINES

#include "wavFile.hpp"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>

bool WavFile::load(std::string fileName){
	using namespace std;

	fstream file(fileName, ios::binary | ios::in);
	if(!file.good()){
		return false;
	}

	file.seekg(16);

	uint32_t fmtSize;
	file.read((char*)(&fmtSize), 4);

	uint16_t type;
	file.read((char*)(&type), 2);
	if(type != 1){
		cout << "format error" << endl;
		return false;
	}

	uint16_t channel;
	file.read((char*)(&channel), 2);
	if(channel == 2){
		cout << "shoud load monaural sound" << endl;
		return false;
	}

	file.read((char*)(&samplingRate), 4);

	file.ignore(6);

	file.read((char*)(&quantifyingBitNum), 2);

	file.seekg(20 + fmtSize + 8);
	while(!file.eof()){
		int16_t buf;
		file.read((char*)(&buf), 2);
		data.push_back(buf);
	}

	return true;
}

bool WavFile::save(std::string fileName){
	using namespace std;

	fstream file(fileName, ios::binary | ios::out);

	file.write("RIFF",4);
	file.write("\x00",4);
	file.write("WAVE",4);
	file.write("fmt ",4);

	uint32_t fmtSize = 16;
	uint16_t format = 1;
	uint16_t channel = 1;
	uint32_t dataSpeed = channel * quantifyingBitNum * samplingRate;
	uint16_t blockSize = channel * quantifyingBitNum;
	uint32_t wavDataSize = data.size() * quantifyingBitNum / 8;

	file.write((char*)(&fmtSize), 4);
	file.write((char*)(&format), 2);
	file.write((char*)(&channel), 2);
	file.write((char*)(&samplingRate), 4);
	file.write((char*)(&dataSpeed), 4);
	file.write((char*)(&blockSize), 2);
	file.write((char*)(&quantifyingBitNum), 2);
	file.write("data", 4);
	file.write((char*)(&wavDataSize), 4);

	auto mm = minmax_element(data.begin(),data.end());
	double mx = max(*(mm.first), *(mm.second));
	
	for(const auto dbuf : data){
		int16_t buf = (int16_t)(dbuf * 30000 / mx);
		file.write((char*)(&buf), 2);
	}

	uint32_t fileSize = (uint32_t)file.tellg() - 3;
	file.seekg(4);
	file.write((char*)(&fileSize), 4);
}