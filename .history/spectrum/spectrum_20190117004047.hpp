#include <vector>



#pragma once



template<int length>
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
		std::vector<double> spectrum;
		Spectrum() {};
	public:
		template<
		Spectrum()
};