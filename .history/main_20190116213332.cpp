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

	// fft用のバッファ（fftsgが使う）
	const int n = 4096;
	int ip[n];
	double w[n*8];

	// 入力波に窓関数をかけて、再合成。
	overlapAdd<double>(
		wav.data.begin(), wav.data.end(),	// 処理範囲
		n,									// 窓幅
		WindowFunction::rect,				// 窓関数の種類
		[&](auto first, auto last){			// 窓関数がかかった波形への処理内容

			// スペクトルの取得
			vector<double> resultOfRdft(first, last);
			vector<double> originalFrequencySpectrum(resultOfRdft.size()/2 + 1, 0);
			rdft(n, -1, resultOfRdft.data(), ip, w);

			// c.f. fftsg.c line from 96 to 99
			originalFrequencySpectrum[0] = resultOfRdft[0];
			for(int i=1; i<originalFrequencySpectrum.size(); i++){
				originalFrequencySpectrum[i] = pow(resultOfRdft[i*2], 2) + pow(resultOfRdft[i*2+1], 2);
			}

			// 取得したスペクトルにLPFをかける
			vector<double> frequencySpectrum(originalFrequencySpectrum.begin(), originalFrequencySpectrum.end());
			rdft(frequencySpectrum.size(), -1, frequencySpectrum.data(), ip, w);
			frequencySpectrum[1] = 0;
			for(int i=frequencySpectrum.size()/2; i<frequencySpectrum.size(); i++){
				frequencySpectrum[i] = 0;
			}

			// 極値の取得
			vector<pair<int,double>> mms;
			for(int i=1; i<a.size()-1; i++){
				if((a[i+1]-a[i])*(a[i]-a[i-1]) <= 0){
					mms.push_back(make_pair(i, abs(a[i])));
				}
			}

			// フィルタの作成
			vector<double> filter(a.size(),0);

			// 極値以外を除去するフィルタの作成
			for(auto mm : mms){
				filter[mm.first] = 1;
			}

			// 周波数を2倍にする
			for(int i=0; i<a.size(); i++){
				s[(int)(i*2)] += a[i] * filter[i];
			}

			// スペクトルの変更結果を反映
			ddct(n*2, 1, s.data(), ip, w);

			// 波長を短くする
			for(int i=0; i<a.size(); i++){
				a[i] = (s[2*i] + s[2*i+1])/2.0;
			}

			copy(a.begin(),a.end(),first);
		},
		[](double x)->double{
			return WindowFunction::sine(x)*WindowFunction::sine(x);
		}
	);

	wav.save(fn+"-out.wav");
}