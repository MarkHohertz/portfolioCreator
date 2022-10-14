#pragma once
#include "wx/wx.h"
#include "wx/notebook.h" //allows you to create tabs
#include "wx/wxhtml.h"
#include "wx/menu.h"
#include "wx/msgdlg.h"
#include "wx/stattext.h"
#include "wx/sizer.h"
#include "sstream"
#include "wx/radiobut.h"
#include "wx/dataview.h"
#include "requester.h"
#include "cpr/cpr.h"
#include <string>
#include "stock.h"
#include "frontier.h"
#include "Eigen/Dense"
#include "mathplot.h"
#include "parabola.h"
#include "tangentline.h"
#include "wx/image.h"
#include "wx/bitmap.h"
#include "wx/dcclient.h"
#include "wx/statbmp.h"
#include "outputstats.h"



//these includes are for displaying the efficient frontier on first page
#include "wx/statbmp.h"
#include "wx/bitmap.h"
#include "wx/image.h"
#include "wx/wfstream.h"
#include "wx/file.h"
#include "wx/stream.h"
#include "wx/filefn.h"


class frame : public wxFrame {
public:

	frame(wxFrame* frame, const wxString& title, int x, int y, int w, int h);
	~frame();
	wxDECLARE_EVENT_TABLE();
	void OnSave(wxCommandEvent& event);
	void OnPrint(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void DeleteStock(wxCommandEvent& event);
	void AddStock(wxCommandEvent& event);
	void GenPort(wxCommandEvent& event);
	void BeginEdit(wxDataViewEvent& event);
	void TrackEdit(wxDataViewEvent& event);
	void PassTicker(wxDataViewEvent& event);
	void checkDuplicates();
	void toggleAnalytics(bool onIfTrue);
	void fillAllocationList(portType type);
	bool legitimatePrices();
	void setNewAnalyticsText();

private:

	wxString GetIntroText();
	wxString GetP2Text();
	wxString GetP4Text();
	wxString GetP4Descript();
	wxHtmlWindow* CreateHTMLCtrl(wxWindow* parent = NULL, wxString txt = "");
	
	int stockCount;
	int editRow;
	bool generate = false;
	double risk;
	double levAmt;
	double interest;
	double horizonLength;
	double portSize;
	stock* index = new stock("^GSPC");  //standalone stock to represent S&P 500 - required for beta calculations
	stock* russell = new stock("^RUT");
	requester myRequester;
	frontier* investPort = new frontier();
	parabola* frontierParab;
	tangentline* capitalAllocLine;
	mpScaleX* xaxis;
	mpScaleY* yaxis;
	outputstats* stats;

	std::vector<std::string> duplicates;
	std::string changeStock = "";

	//======================================
	//WXWIDGETS CONTROLS
	//======================================

	wxNotebook* m_notebook;

	//Page 1
	wxBoxSizer* topsizer;

	//Page 2
	wxBoxSizer* topsizer2;
	wxTextCtrl* riskCtrl;
	wxStaticBoxSizer* boxsz1;
	wxStaticBoxSizer* stockBox;
	wxDataViewRenderer* editor;
	wxDataViewCtrl* stockContainer;
	wxDataViewListStore* stockList;
	wxTextCtrl* horizonEntry;
	wxTextCtrl* sizeEntry;

	//Page 3
	mpWindow* graphWindow;
	wxPanel* thirdPanel;
	wxBoxSizer* panel3Sizer;
	wxBoxSizer* topsizer3;
	wxBoxSizer* analyticSizer;
	wxStaticBoxSizer* graphBoxSizer;
	wxStaticBoxSizer* allocBoxSizer;
	wxStaticBoxSizer* panel3TextSizer;
	wxDataViewCtrl* allocContainer;
	wxDataViewListStore* allocList;
	wxBoxSizer* graphSumSizer;
	wxBoxSizer* betaSizer;
	wxBoxSizer* sharpeSizer;
	wxBoxSizer* stdSizer;
	wxBoxSizer* panel3ToggleSizer;  //contains everything except the html control at the top of panel 3;
	wxStaticText* graphSummaryText;
	wxStaticText* betaText;
	wxStaticText* sharpeText;
	wxStaticText* stdText;
};
