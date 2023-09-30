#include "zos.h"

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