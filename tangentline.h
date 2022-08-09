#pragma once
#include "mathplot.h"
#include "wx/string.h"

class tangentline : public mpFX
{

	public:
		tangentline(wxString name, int flags, double AEntry, double BEntry, double CEntry, double riskFreeEntry, double levAmtEntry, double interestEntry, double mxShpSTDEntry, double stdBarEntry, double maxStdEntry);
		~tangentline();
		double GetY(double x);

		double A, B, C, riskFree, levAmt, interest, mxShpSTD, stdBar, maxStd;
		
};

