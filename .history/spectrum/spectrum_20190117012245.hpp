extern "C"{
	#include "fftsg.c"
}
#include <cmath>
#include <array>



#pragma once



template<std::size_t length>
class Spectrum {
	private:
		class Intensity {
			private:
				double& r1;
				double& i1;
				double& r2;
				double& i2;
				double intensity;
				Intensity() {};
			public:
				Intensity(double& _r1, double& _i1, double& _r2, double& _i2);
				operator double();
				double operator= (double);
				double operator= (const Intensity&);
		};
		std::array<double, length * 2> resultOfCdft;

		// cdft用
		static int ip[length * 2] = {0};
		static double w[length];

		Spectrum() {};
	public:
		template<typename FowardAccessIterator>
		Spectrum(FowardAccessIterator begin, FowardAccessIterator end) {
			{
				auto itr = begin;
				for(int i=0; i<length; i++){
					resultOfCdft[i*2] = *itr;
					itr++;
				}
			}
			cdft(length*2, -1, resultOfCdft, ip, w);
		}
		
		template<typename FowardAccessIterator>
		void operator() (FowardAccessIterator begin, FowardAccessIterator end) {
			std::copy
		}

		double get(std::size_t);
		double set(std::size_t, double);
		std::size_t size();
};