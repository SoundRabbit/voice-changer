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

			cout << "スペクトルの取得" << endl;

			// スペクトルの取得
			vector<double> resultOfRdft(n, 0);
			{
				int loopCounter = 0;
				for(auto itrSample = sampleBegin; itrSample != sampleEnd; ++itrSample){
					resultOfRdft[loopCounter*2] = *itrSample;
				}
			}
			cdft(n, -1, resultOfRdft.data(), ip, w);

			vector<double> originalSpectrum(n, 0);
			for(int k=0; k<originalSpectrum.size(); k+=2){
				originalSpectrum[k] = pow(resultOfRdft[k], 2) + pow(resultOfRdft[k+1], 2);
			}

			cout << "取得したスペクトルにLPFをかける" << endl;

			// 取得したスペクトルにLPFをかける
			vector<double> lpfSpectrum(originalSpectrum.begin(), originalSpectrum.end());
			rdft(n, -1, lpfSpectrum.data(), ip, w);
			for(int i=lpfSpectrum.size()/2; i<lpfSpectrum.size(); i++){
				lpfSpectrum[i] = 0;
			}
			rdft(lpfSpectrum.size(), 1, lpfSpectrum.data(), ip, w);

			cout << "取得したスペクトルにLPFをかける" << endl;

			// 1オクターブ上げる
			vector<double> pichUpedSpectrum(lpfSpectrum.size(), 0);
			for(int k=0; k<lpfSpectrum.size(); k+=2) {
				pichUpedSpectrum[k*2] = lpfSpectrum[k];
			}

			cout << "編集されたスペクトルを反映" << endl;

			// 編集されたスペクトルを反映
			for(int k=0; k<pichUpedSpectrum.size(); k+=2){
				resultOfRdft[k] *= pichUpedSpectrum[k] / originalSpectrum[k];
				resultOfRdft[k+1] *= pichUpedSpectrum[k] / originalSpectrum[k];
			}

			rdft(resultOfRdft.size(), 1, resultOfRdft.data(), ip, w);
			copy(resultOfRdft.begin(),resultOfRdft.end(),sampleBegin);
		},
		[](double x)->double{
			return WindowFunction::sine(x)*WindowFunction::sine(x);
		}
	);

	wav.save(fn+"-out.wav");
}