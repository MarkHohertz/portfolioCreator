#pragma once
#include "wx/wx.h"
#include "frame.h"

class app : public wxApp
{
public:	
	app();
	~app();
	bool OnInit();
};