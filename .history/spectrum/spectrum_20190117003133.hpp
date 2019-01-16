#pragma once

template<int length>
class Spectrum {
	private:
		class Intensity {
			public:
				operator double();

				double operator= (double);
				double operator= (const Intensity&);

				bool operator== (const Intensity&);
				bool operator< (const Intensity&);
		};
	public:
};