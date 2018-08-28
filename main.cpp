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

			//スペクトルの取得。（aのスペクトルはaに上書きされて出力される。）
			vector<double> a(first, last);
			vector<double> s(a.size()*2, 0);
			ddct(n, -1, a.data(), ip, w);

			for(int i=0; i<a.size(); i++){
				s[(int)(i*2)] += a[i];
			}
			// スペクトルの変更結果を反映
			ddct(n*2, 1, s.data(), ip, w);

			// 波長を短くする
			for(int i=0; i<a.size(); i++){
				a[i] = (s[2*i] + s[2*i+1])/2.0;
			}

			copy(a.begin(),a.end(),first);
		}
	);

	wav.save(fn+"-out.wav");
}