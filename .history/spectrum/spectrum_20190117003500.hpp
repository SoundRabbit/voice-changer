#pragma once

template<int length>
class Spectrum {
	private:
		class Intensity {
			private:
				double& r1;
				double& i1;
				double& r2;
				double$ i2;
			public:
				operator double();
				double operator= (double);
				double operator= (const Intensity&);
		};
	public:
};