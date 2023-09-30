#include "yslib.h"

using namespace YS;

ParaxXYRaytrace 
YS::ParaxObject(Pupil X, Pupil Y)
{
	// default object: Lange norm
	ParaxXYRaytrace obj;

	// marginal rays
	obj.XM.h = .5 * X.D; obj.XM.u = 0;
	obj.YM.h = .5 * Y.D; obj.YM.u = 0;

	// principal rays depend on entrance pupil position
	obj.XP.u = tan(X.half_field);
	obj.YP.u = tan(Y.half_field);

	obj.XP.h = -X.s * obj.XP.u;
	obj.YP.h = -X.s * obj.YP.u;

	return obj;
}

void
YS::CylParaxTraceInplace(std::vector<OSSurfaceXY> &surfs)
{
	// Since surfs have incident rays associated with them
	// for first surface they're constants - the Lange norm...
	for (int i_surf = 0; i_surf < surfs.size() - 1; ++i_surf)
	{
		// ...so we start from the second one: i_surf+1
		surfs[i_surf + 1].rt = surfs[i_surf].parax_raytrace(surfs[i_surf + 1].n);
	}
}

D_vec_t
YS::YuanSum(std::vector<OSSurfaceXY>& surfs, int first_surf, int last_surf)
{
	D_vec_t YS_sum(0);
	int n_surfs = surfs.size() - 1;
	for (
		int i_surf = first_surf; 
		i_surf <= (last_surf > n_surfs - 1 ? n_surfs - 1 : last_surf);
 		++i_surf)
	{
		YS::YuanCoeffsInplace(surfs[i_surf], surfs[i_surf + 1]);
		YS_sum += surfs[i_surf].D;
	}
	return YS_sum;
}

void
YS::YuanCoeffsInplace(OSSurfaceXY& s, const OSSurfaceXY& s_)
{
	// Abbe refraction invariant, which's one side of a Snellian eq
	// - it mean either pre- or post-refraction values can be used
	f_parax_t Ainv_XM = s.n * (s.rt.XM.u + s.rt.XM.h * s.cx);
	f_parax_t Ainv_YM = s.n * (s.rt.YM.u + s.rt.YM.h * s.cy);
	f_parax_t Ainv_XP = s.n * (s.rt.XP.u + s.rt.XP.h * s.cx);
	f_parax_t Ainv_YP = s.n * (s.rt.YP.u + s.rt.YP.h * s.cy);

	// Lagrange etendue conservation invariant
	f_parax_t Linv_X = Ainv_XP * s.rt.XM.h - Ainv_XM * s.rt.XP.h;
	f_parax_t Linv_Y = Ainv_YP * s.rt.YM.h - Ainv_YM * s.rt.YP.h;

	// refraction angle finite differences
	f_parax_t delta_uXM_n = s_.rt.XM.u / s_.n - s.rt.XM.u / s.n;
	f_parax_t delta_uYM_n = s_.rt.YM.u / s_.n - s.rt.YM.u / s.n;
	f_parax_t delta_cX_n = s_.cx / s_.n - s.cx / s.n; // P_x in Yuan's PhD
	f_parax_t delta_cY_n = s_.cy / s_.n - s.cy / s.n; // P_y per Yuan
	f_parax_t delta_1_n2 = 1.0/(s_.n * s_.n) - 1.0/(s.n * s.n);

	// Seidel spherical = -1/8*S1
	s.D[1] = -.125 * Ainv_XM * Ainv_XM * s.rt.XM.h * delta_uXM_n; // X S1
	s.D[2] = -.125 * Ainv_YM * Ainv_YM * s.rt.YM.h * delta_uYM_n; // Y S1
		
	// Seidel coma = -1/2*S2
	s.D[4] = -.5 * Ainv_XM * Ainv_XP * s.rt.XM.h * delta_uXM_n;
	s.D[7] = -.5 * Ainv_YM * Ainv_YP * s.rt.YM.h * delta_uYM_n;
		
	// Seidel astigmatic curvature = -1/4*(3*S3 + S4)
	s.D[8] = -.25 * (3 * Ainv_XP * Ainv_XP * s.rt.XM.h * delta_uXM_n
		+ Linv_X * Linv_X * delta_cX_n);
	s.D[9] = -.25 * (3 * Ainv_YP * Ainv_YP * s.rt.YM.h * delta_uYM_n
		+ Linv_Y * Linv_Y * delta_cY_n);

	// Seidel distortion = -1/2*S5
	s.D[13] = -.5 * (Ainv_XP * Ainv_XP * Ainv_XP * s.rt.XM.h * delta_1_n2
		+ Ainv_XP * s.rt.XP.h * (Linv_X - Ainv_XP * s.rt.XM.h) * delta_cX_n);
	s.D[14] = -.5 * (Ainv_YP * Ainv_YP * Ainv_YP * s.rt.YM.h * delta_1_n2
		+ Ainv_YP * s.rt.YP.h * (Linv_Y - Ainv_YP * s.rt.YM.h) * delta_cY_n);

	// Skew ray aberrations
	s.D[3] = -.25 * Ainv_YM * (
		s.rt.YM.h*(s_.rt.XM.u* s_.rt.XM.u- s.rt.XM.u* s.rt.XM.u)
		+ s.rt.XM.h* s.rt.XM.h*s.cx*(s_.rt.YM.u- s.rt.YM.u));
	s.D[11] = -.25 * Ainv_YM * ( // like D3, but delta_u^2 and h^2 are for principal ray
		s.rt.YM.h * (s_.rt.XP.u * s_.rt.XP.u - s.rt.XP.u * s.rt.XP.u)
		+ s.rt.XP.h * s.rt.XP.h * s.cx * (s_.rt.YM.u - s.rt.YM.u));
	s.D[10] = -.25 * Ainv_XM * ( // like D11, but swap x with y
		s.rt.XM.h * (s_.rt.YP.u * s_.rt.YP.u - s.rt.YP.u * s.rt.YP.u)
		+ s.rt.YP.h * s.rt.YP.h * s.cy * (s_.rt.XM.u - s.rt.XM.u));

	s.D[6] = -.5 * Ainv_YM * (
		s.rt.YM.h * (s_.rt.XM.u * s_.rt.XP.u - s.rt.XM.u * s.rt.XP.u)
		+ s.rt.XM.h * s.rt.XP.h * s.cx * (s_.rt.YM.u - s.rt.YM.u));
	s.D[5] = -.25 * ( 
		Ainv_YP * s.rt.YM.h * (s_.rt.XM.u * s_.rt.XM.u - s.rt.XM.u * s.rt.XM.u)
		+ Ainv_YM * s.rt.XM.h * s.rt.XM.h * s.cx * (s_.rt.YP.u - s.rt.YP.u));

	s.D[15] = -.5 * (
		Ainv_XP * s.rt.XM.h * (s_.rt.YP.u * s_.rt.YP.u - s.rt.YP.u * s.rt.YP.u)
		+ Ainv_XM * s.rt.YP.h * s.rt.YP.h * s.cy * (s_.rt.XP.u - s.rt.XP.u)
		);
	s.D[16] = -.5 * ( // like D15, but swap x with y
		Ainv_YP * s.rt.YM.h * (s_.rt.XP.u * s_.rt.XP.u - s.rt.XP.u * s.rt.XP.u)
		+ Ainv_YM * s.rt.XP.h * s.rt.XP.h * s.cx * (s_.rt.YP.u - s.rt.YP.u)
		);
	s.D[12] = -.5 * ( // like D16, but squared factors are split into bilinear (M*P)
		Ainv_YP * s.rt.YM.h * (s_.rt.XM.u * s_.rt.XP.u - s.rt.XM.u * s.rt.XP.u)
		+ Ainv_YM * s.rt.XM.h * s.rt.XP.h * s.cx * (s_.rt.YP.u - s.rt.YP.u)
		);
}

ParaxXYRaytrace 
YS::OSSurfaceXY::parax_raytrace(f_parax_t n_)
{
	ParaxXYRaytrace rt_;
	f_parax_t n12 = n / n_; // relative index, n12 := n/n_
	rt_.XM = ParaxRefractTransfer(
		rt.XM, cx, d, n12
	);
	rt_.YM = ParaxRefractTransfer(
		rt.YM, cy, d, n12
	);
	rt_.XP = ParaxRefractTransfer(
		rt.XP, cx, d, n12
	);
	rt_.YP = ParaxRefractTransfer(
		rt.YP, cy, d, n12
	);
	return rt_;
}
