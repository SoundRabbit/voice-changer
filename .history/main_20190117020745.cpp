#include "overlapAdd/overlapAdd.hpp"
#include "spectrum/spectrum.hpp"
#include "wavFile/wavFile.cpp"
#include "windowFunction/windowFunction.hpp"
#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>

#define _USE_MATH_DEFINES
#include <cmath>

int main(){
	using namespace std;

	cout << "ファイル名 (please input file name) >" << flush;
	std::string fn;
	cin >> fn;

	// wavファイル読み込み（16bit・モノラル・リニアPCMのみ対応）
	WavFile wav;
	if(!wav.load(fn)){
		cout << "error at file input" << endl;
		return 0;
	}

	constexpr int windowWidth = 4096;

	int testCounter = 0;

	// 入力波に窓関数をかけて、再合成。
	overlapAdd<double>(
		wav.data.begin(), wav.data.end(),	// 処理範囲
		windowWidth,						// 窓幅
		WindowFunction::rect,				// 窓関数の種類
		[&](auto sampleBegin, auto sampleEnd){			// 窓関数がかかった波形への処理内容
			Spectrum<windowWidth> originalSpectrum(sampleBegin);
			originalSpectrum(sampleBegin);
		},
		[](double x)->double{
			return WindowFunction::sine(x)*WindowFunction::sine(x);
		}
	);

	wav.save(fn+"-out.wav");
}