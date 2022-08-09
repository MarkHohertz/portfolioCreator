#include "parabola.h"

parabola::parabola(wxString name, int flags, double eBarEntry, double CEntry, double DEntry, double stdBarEntry, double maxStdEntry)
{
	m_name = name;
	m_showName = true;
	m_continuous = true;
	m_flags = flags;
	eBar = eBarEntry;
	C = CEntry;
	D = DEntry;
	stdBar = stdBarEntry;
	maxStd = maxStdEntry;
	m_drawOutsideMargins = false;
	SetContinuity(true);
}

parabola::~parabola()
{

}

double parabola::GetY(double x)
{
	double y;
	if (x < stdBar)
	{
		y = -9999999999;
	}
	else
	{
		y = eBar + 1 / C * sqrt(D * C * (pow(x, 2) - pow(stdBar, 2)));
	}
	return y;
}