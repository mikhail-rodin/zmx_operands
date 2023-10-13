#include "zos.h"
extern "C" {
#include "..\build\generated\catahash.h"
}

using namespace zmx;

Vec2<zmxfloat_t> 
zmx::MaxField(ZOSAPI_Interfaces::IFieldsPtr fields)
{
	Vec2<zmxfloat_t> res = { 0,0 };
	for (short i_fld = 1; i_fld <= fields->NumberOfFields; ++i_fld)
	{
		ZOSAPI_Interfaces::IFieldPtr pFld = fields->GetField(i_fld);
		double xfld = abs(pFld->X);
		double yfld = abs(pFld->Y);
		if (xfld > res.x) res.x = xfld;
		if (yfld > res.y) res.y = yfld;
	}
	return res;
}

int
zmx::PrimaryWave(ZOSAPI_Interfaces::IWavelengthsPtr waves)
{
	for (short i_wave = 1; i_wave <= waves->NumberOfWavelengths; ++i_wave)
		if (waves->GetWavelength(i_wave)->IsPrimary) return i_wave;
}

int 
zmx::RunOperand(p_operandfunc_t OpFunc)
{
	if (S_OK != CoInitialize(nullptr)) return ERR_COM_INITIALIZE;
	ZOSAPI_Interfaces::IZOSAPI_ConnectionPtr pConnection(__uuidof(ZOSAPI::ZOSAPI_Connection));
	ZOSAPI_Interfaces::IZOSAPI_ApplicationPtr pApp = nullptr;
	try
	{
		pApp = pConnection->ConnectToApplication();
		// this will throw an exception if not launched from OpticStudio
	}	
	catch (std::exception& ex)
	{
		// TODO: handle
		return ERR_UNABLE_TO_CONNECT;
	}
	if (pApp == nullptr) return ERR_UNABLE_TO_CONNECT;
	if (pApp->Mode != ZOSAPI_Interfaces::ZOSAPI_Mode::ZOSAPI_Mode_Operand) return ERR_WRONG_ZOSAPI_MODE;
	if (!pApp->IsValidLicenseForAPI) return ERR_LICENCE;

	// Zemax expects a pointer to a fixed-size (usually 1001) heap-stored array of doubles
	// If size differs from OperandResults->Length, an exception will be thrown
	std::vector<zmxfloat_t> operand_results(pApp->OperandResults->Length);
	// So we preallocate a vector, which will be 99% empty

	std::array<zmxfloat_t, 4> args = {
		pApp->OperandArgument1,
		pApp->OperandArgument2,
		pApp->OperandArgument3,
		pApp->OperandArgument4
	};
	
	const int errcode = OpFunc(operand_results, args, pApp->PrimarySystem);
	operand_results[0] = (errcode<0) ? errcode : 42;
	// Note - OpticStudio will wait for the operand to complete until this application exits 
	if (pApp != nullptr)
	{
		pApp->OperandResults->WriteData(
			(long)operand_results.size(),
			&operand_results[0]);
	}
	return errcode;
}

zmx::Glass::Glass(const _bstr_t name)
{
	if (name.length() == 0)
	{
		m_n = 1.0; m_v = 89.3; //air
	}
	else {
		const glasscat_record_t* const pGlass = get_pGlass(name, name.length());
		if (0 == pGlass) throw ERR_CATAHASH;
		m_n = static_cast<zmxfloat_t>(pGlass->n);
		m_v = static_cast<zmxfloat_t>(pGlass->v);
	}
}

zmx::SurfType
zmx::ClassifySurf(ZOSAPI_Interfaces::ILDERowPtr pSurf)
{
	ZOSAPI_Interfaces::ISurfaceToroidalPtr pSurfData = pSurf->SurfaceData;
	const double r = pSurf->Radius;
	// zemax uses inf to represent flat Y radiuses
	// which relies on compiler- and architecture-dependent UB
	// NB! radiuses are inconsistent in ZOS2023: see rX below
	switch (pSurf->type)
	{
	case ZOSAPI_Interfaces::SurfaceType_Standard:
		return zmx::SPH;
		break;
	case ZOSAPI_Interfaces::SurfaceType_Toroidal:
	{//a local scope for the vars below
		const double rx = pSurfData->RadiusOfRotation;
		// rX, unlike rY, is flat at r=0!
		const bool Y_FLAT = (FP_INFINITE == std::fpclassify(r));
		const bool X_FLAT = (FP_ZERO == std::fpclassify(rx));
		if (X_FLAT && !Y_FLAT) {
			return zmx::CYL_Y;
		}
		else if (!X_FLAT && Y_FLAT) {
			return zmx::CYL_X;
		}
		else if (X_FLAT && Y_FLAT) {
			const bool Y_VAR = (ZOSAPI_Interfaces::SolveType_Variable == pSurf->RadiusCell->Solve);
			return Y_VAR ? zmx::CYL_Y : zmx::CYL_X;
		}
		else {
			return zmx::TOROIDAL;
		}
		break;
	}
	default:
		return zmx::UNKNOWN;
		break;
	}
}