#include "tangentline.h"
tangentline::tangentline(wxString name, int flags, double AEntry, double BEntry, double CEntry, double riskFreeEntry, double levAmtEntry, double interestEntry, double mxShpStdEntry, double stdBarEntry, double maxStdEntry)
{
	m_name = name;
	m_showName = true;
	m_continuous = true;
	m_flags = flags;
	A = AEntry;
	B = BEntry;
	C = CEntry;
	riskFree = riskFreeEntry;
	levAmt = levAmtEntry;
	mxShpSTD = mxShpStdEntry;
	interest = interestEntry;
	m_drawOutsideMargins = false;
	stdBar = stdBarEntry;
	maxStd = maxStdEntry;
	SetContinuity(true);
}

tangentline::~tangentline()
{

}

double tangentline::GetY(double x)
{
	double y;

	if (x < 0)
	{
		y = -9999999999;
	}
	else
	{
		y = riskFree + x * sqrt(C * pow(riskFree, 2) - 2 * A * riskFree + B);
	}
	
	return y;
}