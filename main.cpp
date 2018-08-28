extern "C"{
	#include "fftsg.c"
}
#include "overlapAdd.hpp"
#include "wavFile/wavFile.cpp"
#include <algorithm>
#include <iostream>
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
	wav.load(fn);

	// fft用のバッファ（fftsgが使う）
	const int n = 4096;
	int ip[n];
	double w[n*2];

	// 入力波に窓関数をかけて、再合成。
	overlapAdd<double>(
		wav.data.begin(), wav.data.end(),	// 処理範囲
		n,									// 窓幅
		WindowFunction::vorbis,				// 窓関数の種類
		[&](auto first, auto last){			// 窓関数がかかった波形への処理内容

			//スペクトルの取得。（aのスペクトルはaに上書きされて出力される。）
			vector<double> a(first, last);
			vector<double> s(a.size(), 0);
			ddct(n, -1, a.data(), ip, w);

			// a → s の過程でスペクトルの加工
			for(int i=0; i<a.size()/2; i++){
				s[i*2] = a[i];
			}

			// スペクトルの変更結果を反映
			a = s;
			ddct(n, 1, a.data(), ip, w);
			copy(a.begin(),a.end(),first);
		}
	);

	wav.save(fn+"-out.wav");
}