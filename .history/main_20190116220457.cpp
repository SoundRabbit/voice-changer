extern "C"{
	#include "fftsg.c"
}
#include "overlapAdd.hpp"
#include "wavFile/wavFile.cpp"
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

	// cdft用のバッファ（fftsgが使う）
	constexpr int n = windowWidth * 2;
	int ip[n];
	double w[n*8];

	// 入力波に窓関数をかけて、再合成。
	overlapAdd<double>(
		wav.data.begin(), wav.data.end(),	// 処理範囲
		n,									// 窓幅
		WindowFunction::rect,				// 窓関数の種類
		[&](auto sampleBegin, auto sampleEnd){			// 窓関数がかかった波形への処理内容

			// スペクトルの取得
			vector<double> resultOfRdft(n, 0);
			{
				int loopCounter = 0;
				for(auto itrSample = sampleBegin; itrSample != sampleEnd; ++itrSample){
					resultOfRdft[loopCounter*2] = *itrSample;
				}
			}
			rdft(n, -1, resultOfRdft.data(), ip, w);

			// c.f. fftsg.c line from 96 to 99
			vector<double> originalFrequencySpectrum(n, 0);
			originalFrequencySpectrum[0] = resultOfRdft[0];
			for(int k=1; k<originalFrequencySpectrum.size(); k++){
				originalFrequencySpectrum[k] = pow(resultOfRdft[k*2], 2) + pow(resultOfRdft[k*2+1], 2);
			}

			// 取得したスペクトルにLPFをかける
			vector<double> frequencySpectrum(originalFrequencySpectrum.begin(), originalFrequencySpectrum.end());
			rdft(frequencySpectrum.size(), -1, frequencySpectrum.data(), ip, w);
			frequencySpectrum[1] = 0;
			for(int i=frequencySpectrum.size()/2; i<frequencySpectrum.size(); i++){
				frequencySpectrum[i] = 0;
			}
			rdft(frequencySpectrum.size(), 1, frequencySpectrum.data(), ip, w);

			// 1オクターブ上げる。

			// 編集されたスペクトルを反映
			resultOfRdft[0] *= frequencySpectrum[0] / originalFrequencySpectrum[0];
			for(int k=1; k<frequencySpectrum.size(); k++){
				resultOfRdft[k*2] *= frequencySpectrum[k] / originalFrequencySpectrum[k];
				resultOfRdft[k*2+1] *= frequencySpectrum[k] / originalFrequencySpectrum[k];
			}
			resultOfRdft[1] = 0;

			rdft(resultOfRdft.size(), 1, resultOfRdft.data(), ip, w);
			copy(resultOfRdft.begin(),resultOfRdft.end(),first);
		},
		[](double x)->double{
			return WindowFunction::sine(x)*WindowFunction::sine(x);
		}
	);

	wav.save(fn+"-out.wav");
}