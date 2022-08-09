#pragma once
#include "mathplot.h"
#include "wx/string.h"

class parabola : public mpFX
{

public:
	parabola(wxString name, int flags, double eBarEntry, double CEntry, double DEntry, double stdBarEntry, double maxStd);
	~parabola();
	double GetY(double x);

	double eBar, C, D, stdBar, maxStd;
};
