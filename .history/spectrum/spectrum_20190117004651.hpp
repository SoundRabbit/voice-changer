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
				Intensity(double&, double&, double&, double&);
				operator double();
				double operator= (double);
				double operator= (const Intensity&);
		};
		std::array<double, length> resultOfCdft;
		Spectrum() {};
	public:
		template<FowardAccessIterator>
		Spectrum()
};