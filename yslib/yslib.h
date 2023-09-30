#pragma once
#include <vector>
#include <array>
#include <algorithm>
#include <math.h>
/*
+-------------------------------------+
|  Yuan-Seidel aberration lib  v.0.0  |
+-------------------------------------+
Calculates parallel- and cross-cylinder anamorphic lens' third-order aberrations
based on Sheng Yuan's PhD thesis.

Units.
All angles, both output by subroutines and supplied in args, are in radians.
All distances in millimeters.

Sign convention.
A continental sign convention is used, specifically Soviet one as described by Slyusarev.
Positive ray slopes (angles) correspond to positive object-space heights, i.e.:
a right triangle cathetes are axis and positive-height object, then apex (pupil centre in this case)
has a positive angle (ray slope).

System variable names.
A mix of Kingslake's and Rusinov's symbols is used.

Symbols with a trailing underscore (i.e. n_ vs n) are image-space, 
others are either object-space or refraction-invariant.
Underscore in the middle of a var name doesn't imply image space though.

------------------------------------------------
| Symbol  |  Meaning                           |
------------------------------------------------
|   u     |  ray slope, marginal i.n.s.o.      | * if not specified otherwise **
|   U     |  ray slope, chief                  |
|   h     |  height on surf, marginal i.n.s.o. |
|   H     |  height on surf, chief             |
|  Linv   |  Lagrange invariant                |
|  Ainv   |  Abbe invariant                    |
|   n     |  index of refraction               |
|   c     |  curvature                         |
|   d     |  distance                          |
|   r     |  radius of curvature               |
+---------+------------------------------------+

** type of ray can also be inferred by the parent struct's name, encoded as follows:
{X or Y meridional section} & { [M]arginal, [P]rincipal or [C]hief } 

*/

namespace YS {

	template<typename T, size_t N>
	class SVec
	{
		T elts[N];
		inline void init(const T& val) { for (size_t i = 0; i < N; ++i) elts[i] = val; };
	public:
		SVec() { init(0); };
		SVec(const T& init_val) { init(init_val); }
		SVec(std::initializer_list<T> init_list);
		size_t size() const { return N; }
		inline const T& operator[] (const size_t i) const { return elts[i]; };
		inline       T& operator[] (const size_t i)       { return elts[i]; };
		SVec<T, N>&     operator+= (const SVec& rhs);
		SVec<T, N>      operator+ (const SVec& rhs) const;
		SVec<T, N>&     operator*= (const SVec& rhs);
		SVec<T, N>&     operator* (const SVec& rhs) const;
		operator double* () { return elts; }; // cast to C array operator
	};

	typedef float f_parax_t; // paraxial float
	typedef double f_t; // real ray float
	typedef SVec<f_parax_t, 17> D_vec_t; // Yuan-Seidel
	typedef SVec<f_parax_t, 6> S_vec_t; // Seidel

	enum SurfType { SPHERICAL, CYL_X, CYL_Y, TOROIDAL, UNKNOWN };

	struct RayParax
	{
		f_parax_t h = 0; // ray height
		f_parax_t u = 0; // ray slope, that is tg(ray angle with optical axis)
	};

	struct ParaxXYRaytrace
	{
		RayParax XM, YM, XP, YP; // incident (!) rays' coords
	};

	struct OSSurfaceXY
	{
		ParaxXYRaytrace rt;
		f_parax_t cx = 0; // curvature
		f_parax_t cy = 0;
		f_parax_t d = 0; // distance to next
		f_parax_t n = 1;
		f_parax_t abbe = 1;
		SurfType type = SPHERICAL;
		D_vec_t D = {}; // Yuan-Seidel coeffs ...
		// ... also used for desensitivitizing, so they're here 
		ParaxXYRaytrace parax_raytrace(f_parax_t n_);
	};

	struct Pupil
	{
		f_parax_t half_field = 1; // chief ray angle in radians
		// angle is in this struct since D and s are dependent on it due to pupil aberrations
		f_parax_t D = 1; // diameter
		f_parax_t s = 0; // distance to first/last optical surface (according to sign convention)
	};

	inline RayParax
	ParaxRefractTransfer(
			const RayParax ray,
			const f_parax_t c, const f_parax_t d, const f_parax_t n12
		) {
		RayParax ray_;
		// 1. refraction
		ray_.u = ray.u * n12 - ray.h * c * (1 - n12);
		// 2. transfer
		ray_.h = ray.h + ray_.u * d;
		return ray_;
	}

	ParaxXYRaytrace ParaxObject(Pupil X, Pupil Y);
	void CylParaxTraceInplace(std::vector<OSSurfaceXY>& rndt);
	void YuanCoeffsInplace(OSSurfaceXY& s, const OSSurfaceXY& s_);
	D_vec_t YuanSum(std::vector<OSSurfaceXY>& surfs, int first_surf, int last_surf);

	template<typename T, size_t N>
	inline SVec<T, N>::SVec(std::initializer_list<T> init_list)
	{
		for (const T& val : init_list) *elts++ = val;
	}
	template<typename T, size_t N>
	inline SVec<T, N>& SVec<T, N>::operator+=(const SVec& rhs)
	{
		for (size_t i = 0; i < N; ++i) elts[i] += rhs[i];
		return *this;
	}
	template<typename T, size_t N>
	inline SVec<T, N> SVec<T, N>::operator+(const SVec& rhs) const
	{
		return this += rhs;
	}
	template<typename T, size_t N>
	inline SVec<T, N>& SVec<T, N>::operator*=(const SVec& rhs)
	{
		for (size_t i = 0; i < N; ++i) elts[i] *= rhs[i];
		return *this;
	}
	template<typename T, size_t N>
	inline SVec<T, N>& SVec<T, N>::operator*(const SVec& rhs) const
	{
		return this *= rhs;
	}
}