#pragma once

template<int length>
class Spectrum {
	private:
		class Intensity {
			private:
				double& R1, R2, I1, I2;
			public:
				operator double();
				double operator= (double);
				double operator= (const Intensity&);
		};
	public:
};