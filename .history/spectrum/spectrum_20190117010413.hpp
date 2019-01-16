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
		std::array<double, length> resultOfCdft;

		// cdft用
		static int ip[length];
		static double w[length / 2];

		Spectrum() {};
	public:
		template<typename FowardAccessIterator>
		Spectrum(FowardAccessIterator begin, FowardAccessIterator end);
		Intensity operator[] (std::size_t);
		std::size_t size();
		
		template<typename FowardAccessIterator>
		void reverse(FowardAccessIterator begin, FowardAccessIterator end);
};

template<std::size_t length>
Spectrum<length>::Intensity::Intensity(double& _r1, double& _i1, double& _r2, double& _i2) : r1(_r1), i1(_i1), r2(_r2), i2(_i2) {
	intensity = std::sqrt(r1*r1+i1*i1);
}

template<std::size_t length>
Spectrum<length>::Intensity::operator double() {
	return intensity;
}

template<std::size_t length>
double Spectrum<length>::Intensity::operator= (const Intensity& intensity) {
	r1 *= (double)intensity / this.intensity;
	i1 *= (double)intensity / this.intensity;
	r2 *= (double)intensity / this.intensity;
	i2 *= (double)intensity / this.intensity;
	this.intensity = (double)intensity;
	return (double)intensity;
}

template<std::size_t length>
double Spectrum<length>::Intensity::operator= (double intensity) {
	r1 *= intensity / this.intensity;
	i1 *= intensity / this.intensity;
	r2 *= intensity / this.intensity;
	i2 *= intensity / this.intensity;
	this.intensity = intensity;
	return intensity;
}