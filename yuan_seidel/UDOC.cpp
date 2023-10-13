#define DEBUG
//#define ATTACH

#include "stdafx.h"
#include <stdlib.h>
#include <stdexcept>
#include <string>
#include <vector>

#define _USE_MATH_DEFINES
#include <cmath>

#include "..\zosutils\zos.h"
#include "..\yslib\yslib.h"
#include "..\zosutils\dbg.h"

//extern "C" {
//#include "../catahash.h"
//}

using namespace ZOSAPI;
using namespace ZOSAPI_Interfaces;

inline double DegToRad(double deg) { return deg * M_PI / 180.0; }

int Operand(
	std::vector<zmx::zmxfloat_t>& operand_results, 
	const std::array<zmx::zmxfloat_t, 4> args,
	const IOpticalSystemPtr pOS
)
{
	ILensDataEditorPtr pLDE = pOS->LDE;
	IMeritFunctionEditorPtr pMFE = pOS->MFE;
	ISystemDataPtr pSysData = pOS->SystemData;

	const int i_primary_wave = zmx::PrimaryWave(pSysData->Wavelengths);
	const int n_surfs = pLDE->NumberOfSurfaces;
	std::vector<YS::OSSurfaceXY> parax_surfs(n_surfs-1); // preallocating for all surfs except stop
	
	DBG("Entered, " << n_surfs << " surfaces, parax_surfs[" << parax_surfs.size() << "]");

	zmx::FirstOrder Y_first;
	pLDE->GetFirstOrderData(
		&Y_first.f_, &Y_first.FNo_parax, &Y_first.FNo_real, &Y_first.H_, &Y_first.V_parax);

	YS::Pupil X_pupil, Y_pupil;

	bool astigmatic_surfs_encountered = false;
	YS::f_parax_t n_ = 1.0, n = 1.0;
	for (unsigned int i_surf = 0, i_r_surf = 1; i_surf < n_surfs; ++i_surf)
	{
		ILDERowPtr pSurf = pLDE->GetSurfaceAt(i_surf);
		// coord break surfaces not supported (yet)
		if (pSurf->IsStop) {
			if (astigmatic_surfs_encountered)
			{
				DBG("Astigmatic surfs before stop.");
				// if double curvature surfs precede the stop, entrance pupil is atigmatic
				// and we have to trace it ourselves, at least, paraxially.
				return zmx::ERR_UNSUPPORTED_APERTURE;
				// TODO: backtracing EnP
			} else {
				// entrance pupil iz localized
				if (1 == i_surf) // if true, EnP = stop;
				{
					DBG("Stop in front");
					Y_pupil.s = - pSurf->Thickness;
					Y_pupil.D = 2.0 * pSurf->SemiDiameter;
				} else {
					{
						DBG("Stop inside");
						double D, sP;
						pLDE->GetPupil(nullptr, nullptr, &D, &sP, nullptr, nullptr, nullptr, nullptr);
						Y_pupil.D = D;
						Y_pupil.s = sP;
						//Y_pupil.D = Y_first.f_ / Y_first.FNo_real;
					}
				}
				X_pupil = Y_pupil;
			}
		} else if (!(pSurf->IsObject)) { // it's a reflecting/refracting surface
			YS::OSSurfaceXY parax_surf;
			parax_surf.cy = static_cast<YS::f_parax_t>(1.0 / pSurf->Radius);  // 1.0/inf = 0.0,	because IEEE 754
			if (SurfaceType_Toroidal == pSurf->type)
			{
				astigmatic_surfs_encountered = true; // first toroidal encountered sets the flag
				ISurfaceToroidalPtr pSurfData = pSurf->SurfaceData;
				const double rx = pSurfData->RadiusOfRotation;
				if (FP_ZERO == std::fpclassify(rx))
				// unlike Y radius, which uses IEEE754 INF for a flat surface, 
				// rx is 0 for flat
				{
					parax_surf.cx = 0;
				} else {
					parax_surf.cx = static_cast<YS::f_parax_t>(1.0 / rx);
				}
			}
			else
			{
				parax_surf.cx = 0;
			}
			parax_surf.d = static_cast<YS::f_parax_t>(pSurf->Thickness);
			const _bstr_t glassname = pSurf->Material;
			/*if (glassname.length() == 0)
			{
				n_ = 1; //air
			} else {
				const glasscat_record_t* pGlass = get_pGlass(glassname, glassname.length());
				n_ = static_cast<YS::f_parax_t>(pGlass->n);
			}*/
			try {
				zmx::Glass gls(glassname);
				n_ = gls.n();
			}
			catch (zmx::ErrCode code) {
				return code;
			}
			parax_surf.n = n;
			n = n_;
			DBG("i=" << i_surf << ", i_r=" << i_r_surf << ", cy =" << parax_surf.cy);
			parax_surfs[i_r_surf] = parax_surf;
			++i_r_surf; // refracting surface index
		}
	}
	
	if (FieldType_Angle != pSysData->Fields->GetFieldType())
		return zmx::ERR_UNSUPPORTED_FIELDTYPE;
	auto max_field = zmx::MaxField(pSysData->Fields);
	X_pupil.half_field = DegToRad(max_field.x);
	Y_pupil.half_field = DegToRad(max_field.y);
	DBG("X pupil: D = " << X_pupil.D << ", w = " << X_pupil.half_field << ", sP = " << X_pupil.s);
	DBG("Y pupil: D = " << Y_pupil.D << ", w = " << Y_pupil.half_field << ", sP = " << Y_pupil.s);

	parax_surfs[0].rt = YS::ParaxObject(X_pupil, Y_pupil);
	DBG("uY = " << parax_surfs[0].rt.YP.u << ", h = " << parax_surfs[0].rt.YP.h);
	YS::CylParaxTraceInplace(parax_surfs); 
	DBG("uY = " << parax_surfs[1].rt.YP.u << ", h = " << parax_surfs[1].rt.YP.h);
	// now the rnd table is filled with raytrace data for all 4 paraxial rays

	YS::D_vec_t YS_sum = {};
	// clamp 'incl' range to number of surfaces
	const int i_surf_first_incl = std::max(static_cast<int>(args[0]), 0);
	const int i_surf_last_incl = std::min(static_cast<int>(args[1]), n_surfs);
	// clamp 'excl' range so it's inside 'incl'
	const int i_surf_first_excl = std::max(static_cast<int>(args[2]), i_surf_first_incl);
	const int i_surf_last_excl = std::min(static_cast<int>(args[3]), i_surf_last_incl);
	DBG(Dbg::endl);

	if ((i_surf_first_incl < i_surf_first_excl) && (i_surf_last_excl < i_surf_last_incl))
	{
		//      0---|++++|-------------|+++++++|----n_surf
		// excluded range fully inside included => 2 included ranges
		YS_sum = YS::YuanSum(parax_surfs, i_surf_first_incl, i_surf_first_excl);
		DBG("surfaces [" << i_surf_first_incl << "; " << i_surf_first_excl);
		YS_sum += YS::YuanSum(parax_surfs, i_surf_last_excl, i_surf_last_incl);
		DBG("&& [" << i_surf_last_excl << "; " << i_surf_last_incl);
	} else if  (i_surf_first_incl == i_surf_first_excl) {
		//      0---||--------||+++++++++++++++|----n_surf
		YS_sum = YS::YuanSum(parax_surfs, i_surf_last_excl, i_surf_last_incl); 
		DBG("surfaces [" << i_surf_last_excl << "; " << i_surf_last_incl);
	} else {
		//      0---|+++++++++++++++|---------||----n_surf
		YS_sum = YS::YuanSum(parax_surfs, i_surf_first_incl, i_surf_first_excl); 
		DBG("surfaces [" << i_surf_first_incl << "; " << i_surf_first_excl);
	}

	DBG("D1=" << YS_sum[1] << ", D2=" << YS_sum[2]);

	for (short i_D = 0; i_D < YS_sum.size(); ++i_D) operand_results[i_D] = YS_sum[i_D];
	return 0;
}

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	return zmx::RunOperand(Operand);
}

int _tmain(int argc, _TCHAR* argv[])
{
	return zmx::RunOperand(Operand);
}