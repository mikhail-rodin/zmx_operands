#pragma once

#include <vector>
#include <array>

// Note - .tlh files will be generated from the .tlb files (above) once the project is compiled.
// Visual Studio will incorrectly continue to report IntelliSense error messages however until it is restarted.
#if defined _DEBUG
#	include "../build/generated/zosapi_dev.h"
#else
	#include "../build/generated/zosapi.h"
#endif // DEBUG

namespace zmx {

	typedef double zmxfloat_t; // zemax float type

	template<typename T>
	struct Vec2
	{
		T x, y;
	};

	enum {
		ERR_COM_INITIALIZE = -1,
		ERR_UNABLE_TO_CONNECT = -2,
		ERR_WRONG_ZOSAPI_MODE = -3,
		ERR_LICENCE = -4,
		ERR_UNSUPPORTED_FIELDTYPE = -100,
		ERR_UNSUPPORTED_APERTURE = -101,
		ERR_UNSUPPORTED_SURFACE = -1000, // - 0-based surface index = errcode
	};

	struct FirstOrder // matches zemax' own internal paraxial data tuple
	{
		zmxfloat_t f_ = 1;
		zmxfloat_t FNo_parax = 0; // = 1/( 2n'*sin( YM' ) ) where YM' is real marginal ray angle
		zmxfloat_t FNo_real = 0;  // = 1/( 2n'*tg( YMp' ) ) where YMp' is paraxial
		zmxfloat_t H_ = 1;        // paraxial image height
		zmxfloat_t V_parax = 1;   // paraxial magnification
		FirstOrder scaled(zmxfloat_t new_f_)
		{
			// NB: scaling =/= changing f' 
			const zmxfloat_t S = new_f_ / f_;
			FirstOrder out = *this;
			out.f_ = new_f_;
			out.H_ = H_ * new_f_ / f_;
			return out;
		}
	};

	struct FirstOrderXY
	{
		FirstOrder X, Y;  // Y is main section where image is squeezed (horizontal)
		inline double Anamorphosis() { return Y.f_ / X.f_; }
	};

	Vec2<zmxfloat_t> MaxField(ZOSAPI_Interfaces::IFieldsPtr fields);
	int PrimaryWave(ZOSAPI_Interfaces::IWavelengthsPtr waves);

	typedef int (*p_operandfunc_t) (
		std::vector<zmxfloat_t>&, 
		std::array<zmxfloat_t, 4>,
		ZOSAPI_Interfaces::IOpticalSystemPtr); // pointer to an operand callback
	int RunOperand(p_operandfunc_t);
} // namespace zmx