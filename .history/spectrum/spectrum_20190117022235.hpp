extern "C"{
	#include "fftsg.c"
}
#include <cmath>
#include <array>
#include <utility>
#include <iostream>



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
		static std::array<int, length * 2> ip;
		static std::array<double, length> w;

		Spectrum() {};
	public:
		template<typename FowardAccessIterator>
		Spectrum(FowardAccessIterator begin) {
			{
				auto itr = begin;
				for(int i=0; i<length; i++){
					resultOfCdft[i*2] = *itr;
					itr++;
				}
			}
			cdft(length*2, -1, resultOfCdft.data(), ip.data(), w.data());
		}
		
		template<typename FowardAccessIterator>
		void operator()(FowardAccessIterator begin) {
			std::array<double, length * 2> buf;
			std::copy(resultOfCdft.begin(), resultOfCdft.end(), buf.begin());
			cdft(length*2, 1, buf.data(), ip.data(), w.data());
			auto itr = begin;
			for(int i=0; i<length; i++) {
				*itr = buf[i*2];
				itr++;
			}
		}

		double get(std::size_t i) {
			return std::sqrt(std::pow(resultOfCdft[i*2], 2) + std::pow(resultOfCdft[i*2+1], 2));
		}
		double set(std::size_t i, double p) {
			double op = get(i);
			std::size_t k1 = i * 2;
			std::size_t k2 = (length - i - 1) * 2;
			resultOfCdft[k1] *= p / op;
			resultOfCdft[k1 + 1] *= p / op;
			resultOfCdft[k2] *= p / op;
			resultOfCdft[k2 + 1] *= p / op;
			return p;
		}

		std::size_t size() {
			return length / 2;
		}
};

std::array<int, length * 2> ip