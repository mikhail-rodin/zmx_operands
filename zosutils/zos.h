#pragma once

#include <vector>
#include <string>
#include <array>
#include <cmath>

#define NOMINMAX // so that win32 doesn't pollute the global namespace with its min&max macros
#include <comutil.h>

/* Note on ZOSAPI headers.
MSVS Intellisense sometimes refuses to work with symbols imported from a TLB.
TLH, on the other hand, present no problem, as they're pretty much the usual C headers.
So right after cloning the repo one shall
1. Run generate_headers.sh (say, in MSYS) with an approriate glass catalogue
and Zemax-supplied TLBs as inputs. Now the /build/generated is populated.
2. Build in Release mode, thus generating TLH files.
3. Switch to Debug mode. 
*/
#if defined _DEBUG
	#include "..\build\x64\Release\ZOSAPI.tlh"
	#include "..\build\x64\Release\ZOSAPII.tlh"
#else
	#include "..\build\generated\zosapi.h"
#endif // DEBUG


namespace zmx {

	typedef double zmxfloat_t; // zemax float type

	template<typename T>
	struct Vec2
	{
		T x, y;
	};

	enum ErrCode {
		ERR_COM_INITIALIZE = -1,
		ERR_UNABLE_TO_CONNECT = -2,
		ERR_WRONG_ZOSAPI_MODE = -3,
		ERR_LICENCE = -4,
		ERR_UNSUPPORTED_FIELDTYPE = -100,
		ERR_UNSUPPORTED_APERTURE = -101,
		ERR_UNSUPPORTED_SURFACE = -1000, // - 0-based surface index = errcode
		ERR_CATAHASH = -2000, // - 0-based surface index = errcode
	};

	enum SurfType {
		SPH,
		CYL_X, CYL_Y,
		TOROIDAL,
		BICONIC,
		UNKNOWN,
	};

	class Glass
	{
		zmxfloat_t m_n, m_v;
	public:
		Glass(const _bstr_t name);
		inline const zmxfloat_t n() { return m_n; };
		inline const zmxfloat_t abbe_v() { return m_v; };
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
		const std::array<zmxfloat_t, 4>,
		const ZOSAPI_Interfaces::IOpticalSystemPtr); // pointer to an operand callback

	int RunOperand(p_operandfunc_t);

	SurfType ClassifySurf(ZOSAPI_Interfaces::ILDERowPtr pSurf);
} // namespace zmx

