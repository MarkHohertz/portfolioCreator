#include "frame.h"



enum
{
    // IDs for controls
    ID_Save = wxID_HIGHEST,
    ID_Print,
    ID_About = wxID_ABOUT,
    ID_Low,
    ID_Med,
    ID_High,
    ID_RiskCtrl,
    ID_AddButton,
    ID_RemButton,
    ID_GenPort,
    ID_Container,
    ID_CostCtrl,
    //Defining maximum leverage
    Max_Leverage = 50
};

wxBEGIN_EVENT_TABLE(frame, wxFrame)
    EVT_MENU(ID_Save, frame::OnSave)
    EVT_MENU(ID_Print, frame::OnPrint)
    EVT_MENU(ID_About, frame::OnAbout)
    EVT_BUTTON(ID_RemButton, frame::DeleteStock)
    EVT_BUTTON(ID_AddButton, frame::AddStock)
    EVT_BUTTON(ID_GenPort, frame::GenPort)
    EVT_DATAVIEW_ITEM_ACTIVATED(ID_Container, frame::BeginEdit)
    EVT_DATAVIEW_ITEM_EDITING_STARTED(ID_Container, frame::TrackEdit)
    EVT_DATAVIEW_ITEM_VALUE_CHANGED(ID_Container, frame::PassTicker)
wxEND_EVENT_TABLE()

frame::frame(wxFrame* frame, const wxString& title, int x, int y, int w, int h) :
    wxFrame(frame, wxID_ANY, title, wxPoint(x, y), wxSize(w, h))           //places the sizers & controls into the GUI
{
    
    wxMenu* menuFile = new wxMenu;
    wxMenu* menuAbout = new wxMenu;
    
    menuFile->Append(ID_Save, _("&Save"));
    menuFile->Append(ID_Print, _("&Print"));
    menuAbout->Append(ID_About, _("&About this program"));

    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append(menuFile, _("&File"));
    menuBar->Append(menuAbout, _("&About"));
    
    //attach this menu bar to the frame
    SetMenuBar(menuBar);

    m_notebook = new wxNotebook(this, wxID_ANY); //notebook contains the panels created below
    

    //INTRODUCTION - first page
    //-----------------------------

   wxPanel* firstPanel = new wxPanel(m_notebook, wxID_ANY);
   topsizer = new wxBoxSizer( wxVERTICAL);
    // create text ctrl with minimal size 100x60
    topsizer->Add(
        CreateHTMLCtrl(firstPanel,GetIntroText()),
        1,            // make vertically stretchable
        wxEXPAND |    // make horizontally stretchable
        wxALL,        //   and make border all around
        0 );         // set border width to 0
    
    firstPanel->SetSizerAndFit(topsizer);  // you have to tell the window to use the sizer, otherwise wont work
    
     


   
    //PORTFOLIO INPUTS - second page
    //-----------------------------

    wxPanel* secondPanel = new wxPanel(m_notebook, wxID_ANY);

        
    topsizer2 = new wxBoxSizer(wxVERTICAL);
    topsizer2->Add(
        CreateHTMLCtrl(secondPanel, GetP2Text()),
        0,            
        wxEXPAND |    
        wxALL,        
        0);         
     boxsz1 = new wxStaticBoxSizer(wxHORIZONTAL, secondPanel, "Investor Preferences");
     
     stockBox = new wxStaticBoxSizer(wxVERTICAL, secondPanel, "Equity List");
      wxButton* addStock = new wxButton(stockBox->GetStaticBox(), ID_AddButton, "Add");
      wxButton* remStock = new wxButton(stockBox->GetStaticBox(), ID_RemButton, "Remove");
      wxButton* genPort = new wxButton(stockBox->GetStaticBox(), ID_GenPort, "Generate");
      wxBoxSizer* buttonsizer = new wxBoxSizer(wxHORIZONTAL);
     wxFlexGridSizer* investGrid = new wxFlexGridSizer(4, 3, 10, 10);
      riskCtrl = new wxTextCtrl(boxsz1->GetStaticBox(), ID_RiskCtrl);
     wxStaticText* riskT = new wxStaticText(boxsz1->GetStaticBox(), wxID_ANY, "Risk tolerance:");
     wxStaticText* riskPer = new wxStaticText(boxsz1->GetStaticBox(), wxID_ANY, "%");
     wxStaticText* portSize = new wxStaticText(boxsz1->GetStaticBox(), wxID_ANY, "Portfolio Size");
     sizeEntry = new wxTextCtrl(boxsz1->GetStaticBox(), ID_RiskCtrl);
     wxStaticText* portDescript = new wxStaticText(boxsz1->GetStaticBox(), wxID_ANY, "$ Initial Investment");
     wxStaticText* portHorizon = new wxStaticText(boxsz1->GetStaticBox(), wxID_ANY, "Investing Horizon");
     horizonEntry = new wxTextCtrl(boxsz1->GetStaticBox(), wxID_ANY);
     wxStaticText* horizonDescript = new wxStaticText(boxsz1->GetStaticBox(), wxID_ANY, "Years (1 Minimum)");

     stockContainer = new wxDataViewCtrl(stockBox->GetStaticBox(),ID_Container, wxDefaultPosition, wxSize(700,300));
     stockList = new wxDataViewListStore();

     stockList->AppendColumn("Number");
     stockList->AppendColumn("Ticker");
     stockList->AppendColumn("Current Price");
     stockList->AppendColumn("Industry");
     
     stockContainer->AppendTextColumn("Number", 0, wxDATAVIEW_CELL_INERT);
     stockContainer->AppendTextColumn("Ticker", 1, wxDATAVIEW_CELL_EDITABLE, 150);
     stockContainer->AppendTextColumn("Current Price", 2, wxDATAVIEW_CELL_INERT, 150);
     stockContainer->AppendTextColumn("Industry", 3, wxDATAVIEW_CELL_INERT);

     wxVector<wxVariant> firstRow;

     stockContainer->AssociateModel(stockList);

     stockCount = 0;


     investGrid->Insert(0, riskT);
     investGrid->Insert(1, riskCtrl);
     investGrid->Insert(2, riskPer);
     investGrid->Insert(9, portSize);
     investGrid->Insert(10, sizeEntry);
     investGrid->Insert(11, portDescript);
     investGrid->Insert(12, portHorizon);
     investGrid->Insert(13, horizonEntry);
     investGrid->Insert(14, horizonDescript);

     buttonsizer->Add(addStock, wxSizerFlags().Expand().Border(wxALL, 10));
     buttonsizer->Add(remStock, wxSizerFlags().Expand().Border(wxALL, 10));
     buttonsizer->Add(genPort, wxSizerFlags().Expand().Border(wxALL, 10));

     wxStaticText* caveat = new wxStaticText(stockBox->GetStaticBox(), wxID_ANY, "*For best results, please choose 3 or more stocks with positive returns.*");
     boxsz1->Add(investGrid, wxSizerFlags().Expand().Border(wxALL, 10));
     stockBox->Add(caveat, wxSizerFlags().Expand().Border(wxALL, 10));
     stockBox->Add(stockContainer, wxSizerFlags().Expand().Border(wxALL, 10));
     stockBox->Add(buttonsizer, wxSizerFlags().Expand().Border(wxALL, 10));

    topsizer2->Add(boxsz1,
        0,
        wxALIGN_TOP |
        wxALL,
        0);
    topsizer2->Add(stockBox,
        0,
        wxALIGN_TOP |
        wxALL,
        0);
   


    secondPanel->SetSizerAndFit(topsizer2);

    

    //TEMPORARY SPOT for the index data used for beta calcs, probably will need to put it in portfolio generation function
    

    //ASSET ALLOCATION - third page
    //-----------------------------

    thirdPanel = new wxPanel(m_notebook, wxID_ANY);
    panel3Sizer = new wxBoxSizer(wxVERTICAL);
    analyticSizer = new wxBoxSizer(wxHORIZONTAL);
    
    topsizer3 = new wxBoxSizer(wxVERTICAL);
    topsizer3->Add(
        CreateHTMLCtrl(thirdPanel, GetP4Text()),
        0,            
        wxEXPAND |    
        wxALL,       
        0);         
 
    graphBoxSizer = new wxStaticBoxSizer(wxHORIZONTAL, thirdPanel, "Efficient Frontier");
    allocBoxSizer = new wxStaticBoxSizer(wxHORIZONTAL, thirdPanel, "Allocation");
    graphWindow = new mpWindow(graphBoxSizer->GetStaticBox(), wxID_ANY, wxPoint(0,0), wxSize(500, 500), wxSUNKEN_BORDER);
    graphBoxSizer->Add(graphWindow, wxSizerFlags().Expand().FixedMinSize().Border(wxALL, 10));
    analyticSizer = new wxBoxSizer(wxHORIZONTAL);
    analyticSizer->Add(
        graphBoxSizer,
        0,            
        wxFIXED_MINSIZE |   
        wxALL,       
        10);         
    wxStaticBoxSizer* legend = new wxStaticBoxSizer(wxHORIZONTAL, graphBoxSizer->GetStaticBox(), "Porftolio Legend");
    wxStaticText* minVarLabel = new wxStaticText(graphBoxSizer->GetStaticBox(), wxID_ANY, "Minimum Variance");
    wxStaticText* maxShpLabel = new wxStaticText(graphBoxSizer->GetStaticBox(), wxID_ANY, "Maximum Sharpe Ratio");
    wxStaticText* riskPrefLabel = new wxStaticText(graphBoxSizer->GetStaticBox(), wxID_ANY, "Personal Risk Preference");
    wxFlexGridSizer* legendGrid = new wxFlexGridSizer(3, 2, 10, 10);
    
    wxImage minVarImg(wxT("./minVar.bmp"), wxBITMAP_TYPE_BMP);
    wxImage maxShpImg(wxT("./maxShp.bmp"), wxBITMAP_TYPE_BMP);
    wxImage riskPrefImg(wxT("./riskPref.bmp"), wxBITMAP_TYPE_BMP);
    wxBitmap minVarBmp(minVarImg.Scale(10, 10));
    wxBitmap maxShpBmp(maxShpImg.Scale(10, 10));
    wxBitmap riskPrefBmp(riskPrefImg.Scale(10, 10));
    wxStaticBitmap* minVarS = new wxStaticBitmap(graphBoxSizer->GetStaticBox(), wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize(minVarBmp.GetWidth(), minVarBmp.GetHeight()), 0);
    wxStaticBitmap* maxShpS = new wxStaticBitmap(graphBoxSizer->GetStaticBox(), wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize(maxShpBmp.GetWidth(), maxShpBmp.GetHeight()), 0);
    wxStaticBitmap* riskPrefS = new wxStaticBitmap(graphBoxSizer->GetStaticBox(), wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize(riskPrefBmp.GetWidth(), riskPrefBmp.GetWidth()), 0);
    minVarS->SetBitmap(minVarBmp);
    maxShpS->SetBitmap(maxShpBmp);
    riskPrefS->SetBitmap(riskPrefBmp);

    legendGrid->Insert(0, minVarS);
    legendGrid->Insert(1, minVarLabel);
    legendGrid->Insert(2, maxShpS);
    legendGrid->Insert(3, maxShpLabel);
    legendGrid->Insert(4, riskPrefS);
    legendGrid->Insert(5, riskPrefLabel);

    legend->Add(
        legendGrid,
        0,            
        wxFIXED_MINSIZE |
        wxALL,       
        10);         
    graphBoxSizer->Add(
        legend,
        0,           
        wxFIXED_MINSIZE |
        wxALL | wxALIGN_CENTER_VERTICAL,       
        10);         
    
   
    //add title to graph
    //add paragraphs of text underneath
    //add allocation table to the right of graph
    allocContainer = new wxDataViewCtrl(allocBoxSizer->GetStaticBox(), wxID_ANY, wxPoint(10,200), wxSize(500, 500));
    allocList = new wxDataViewListStore();
    allocList->AppendColumn("Number");
    allocList->AppendColumn("Ticker");
    allocList->AppendColumn("$ Value");
    allocList->AppendColumn("% Portfolio");
    allocList->AppendColumn("Current Price");
    allocList->AppendColumn("Industry");

    allocContainer->AppendTextColumn("Number", 0, wxDATAVIEW_CELL_INERT);
    allocContainer->AppendTextColumn("Ticker", 1, wxDATAVIEW_CELL_INERT);
    allocContainer->AppendTextColumn("$ Value", 2, wxDATAVIEW_CELL_INERT);
    allocContainer->AppendTextColumn("% Portfolio", 3, wxDATAVIEW_CELL_INERT);
    allocContainer->AppendTextColumn("Current Price", 4, wxDATAVIEW_CELL_INERT);
    allocContainer->AppendTextColumn("Industry", 5, wxDATAVIEW_CELL_INERT);
    allocBoxSizer->Add(allocContainer, wxSizerFlags().Expand().Bottom().Border(wxALL, 10));
    allocContainer->AssociateModel(allocList);
    analyticSizer->Add(
        allocBoxSizer,
        0,            
        wxFIXED_MINSIZE |
        wxALL,        
        10);         
    panel3ToggleSizer = new wxBoxSizer(wxVERTICAL);
    panel3ToggleSizer->Add(analyticSizer);
    panel3TextSizer = new wxStaticBoxSizer(wxVERTICAL, thirdPanel, "Analysis");
    graphSumSizer = new wxBoxSizer(wxHORIZONTAL);
    graphSumSizer->SetMinSize(analyticSizer->GetSize());
    betaSizer = new wxBoxSizer(wxHORIZONTAL);
    sharpeSizer = new wxBoxSizer(wxHORIZONTAL);
    stdSizer = new wxBoxSizer(wxHORIZONTAL);
    panel3Sizer->Add(topsizer3, wxSizerFlags().Expand().Border(wxALL, 10));
    wxStaticText* teststr = new wxStaticText(thirdPanel, wxID_ANY, "Writeup goes here.");
    panel3TextSizer->Add(teststr);
    /*
    graphSumSizer->Add(
        CreateHTMLCtrl(thirdPanel, GetGraphSummary()),
        0,            // make vertically stretchable
        wxEXPAND |    // make horizontally stretchable
        wxALL,        //   and make border all around
        0);         // set border width to 0
    betaSizer->Add(
        CreateHTMLCtrl(thirdPanel, GetBetaText()),
        0,            // make vertically stretchable
        wxEXPAND |    // make horizontally stretchable
        wxALL,        //   and make border all around
        0);         // set border width to 0
    sharpeSizer->Add(
        CreateHTMLCtrl(thirdPanel, GetSharpeText()),
        0,            // make vertically stretchable
        wxEXPAND |    // make horizontally stretchable
        wxALL,        //   and make border all around
        0);         // set border width to 0
    stdSizer->Add(
        CreateHTMLCtrl(thirdPanel, GetStdText()),
        0,            // make vertically stretchable
        wxEXPAND |    // make horizontally stretchable
        wxALL,        //   and make border all around
        0);         // set border width to 0

    panel3TextSizer->Add(graphSumSizer);
    panel3TextSizer->Add(betaSizer);
    panel3TextSizer->Add(sharpeSizer);
    panel3TextSizer->Add(stdSizer);
    */
    panel3ToggleSizer->Add(panel3TextSizer, 0, wxEXPAND | wxALL, 10);
    panel3Sizer->Add(panel3ToggleSizer, wxSizerFlags().Expand().Border(wxALL, 10));
    thirdPanel->SetSizerAndFit(panel3Sizer);
    panel3ToggleSizer->ShowItems(false);
    panel3ToggleSizer->Layout();
    m_notebook->AddPage(firstPanel, "Introduction");
    m_notebook->AddPage(secondPanel, "Portfolio Inputs");
    m_notebook->AddPage(thirdPanel, "Asset Allocation");
}

frame::~frame()
{
}


    
  
void frame::OnSave(wxCommandEvent& event)
{
    //waiting to fill this in for later, non-functional requirement
}

void frame::OnPrint(wxCommandEvent& event)
{
    //waiting to fill this in for later, non-functional requirement
}

void frame::OnAbout(wxCommandEvent& event)
{
    wxString msg;
    msg.Printf("This program was created by Mark Hohertz.\n");
    wxMessageBox(msg, "Portfolio Creator", wxOK | wxICON_INFORMATION, this);
}


void frame::DeleteStock(wxCommandEvent& event)  //removes selected stock from stockList on the second page
{ 
    if(stockContainer->GetSelection().IsOk() != NULL)
    { 
        wxVariant stockLabel;
        stockList->GetValueByRow(stockLabel, stockList->GetRow(stockContainer->GetSelection()), 1);
        
        if (stockList->GetRow(stockContainer->GetSelection()) != stockList->GetItemCount())
        {
            int temp = stockList->GetRow(stockContainer->GetSelection())+1; 
            stockList->DeleteItem(stockList->GetRow(stockContainer->GetSelection())); //deleting the GUI item
            stockCount -= 1;
            for (int i = temp; i <=stockCount; i++)  //relabel rows
            {
                std::stringstream ss;
                ss << i;
                if (i == 0)
                {
                    stockList->SetValueByRow(ss.str(), i, 0);
                }
                else 
                {
                    stockList->SetValueByRow(ss.str(), i - 1, 0);
                }
            }
            
        }
        else
        {
            stockList->DeleteItem(stockList->GetRow(stockContainer->GetSelection()));
            stockCount -= 1;
        }
    }
    toggleAnalytics(false);
   
}

void frame::AddStock(wxCommandEvent& event) //adding stock to stockList assuming data rules are followed
{
    checkDuplicates();
    bool isDefault = false;
    wxVariant tempTicker;
    for (auto i = 0; i < stockList->GetItemCount(); i++)
    {
        stockList->GetValueByRow(tempTicker, i, 1);
        if (std::string(tempTicker) == "Double-click to edit.")
        {
            wxString msg;
            msg.Printf(wxT("Please edit row %d before adding another row."), i + 1);
            wxMessageBox(msg, "Portfolio Creator", wxOK | wxICON_INFORMATION, this);
            isDefault = true;
        }
       
    }

    if (duplicates.empty() && isDefault != true)
    {
        stockCount += 1;
        std::stringstream ss;
        ss << stockCount;
        wxVector<wxVariant> defaultData;
        defaultData.push_back(ss.str());
        defaultData.push_back("Double-click to edit.");
        defaultData.push_back("N/A");
        defaultData.push_back("N/A");
        stockList->AppendItem(defaultData);
        toggleAnalytics(false);
    } 
    else if(duplicates.empty() == false && isDefault != true)
    {
        wxString msg;
        msg.Printf(wxT("Duplicate detected. Please type in a different stock on row %d."), editRow + 1);
        wxMessageBox(msg, "Portfolio Creator", wxOK | wxICON_INFORMATION, this);
    }
}

void frame::GenPort(wxCommandEvent& event)    //performs some data validation, triggers the logic in the frontier class, and generates analytics
{
    wxString msg;
    bool testPrices = legitimatePrices();
    if (duplicates.empty() == false)
    {
        std::stringstream ss;
        for (int i = 0; i < duplicates.size(); i++)
        {
            ss << "\n" << "     " << duplicates.at(i);
        }
        msg.Printf(wxT("The following were detected as duplicates: %s"), ss.str());
        wxMessageBox(msg, "Portfolio Creator", wxOK | wxICON_INFORMATION, this);
    }
    else if (!testPrices)
    {
        msg.Printf(wxT("Please ensure all tickers are legitimate before generating a portfolio."));
        wxMessageBox(msg, "Portfolio Creator", wxOK | wxICON_INFORMATION, this);
    }
    else if (stockCount < 2)
    {
        msg.Printf(wxT("Please include at least two or more stocks before creating a portfolio."));
        wxMessageBox(msg, "Portfolio Creator", wxOK | wxICON_INFORMATION, this);
    }
    else
    {
        std::string riskCheck, levAmtCheck, interestCheck, portSizeCheck;
        riskCheck = "";
        levAmtCheck = "";
        interestCheck = "";
        portSizeCheck = "";
        std::stringstream errorStream;

        //get risk tolerance

        try
        {
            risk = std::stod(std::string(riskCtrl->GetValue())) / 100;
            if (risk < 0 || risk > 1)
            {
                errorStream << "\n" << "   -Risk tolerance must be between 0 and 100.";
            }
        }
        catch (const std::exception& e)
        {
            risk = -1;
            riskCheck = "   -Risk tolerance % cannot have letters or symbols.";
            errorStream << "\n" << riskCheck;
        }

        //get portfolio size

        try
        {
            portSize = std::stod(std::string(sizeEntry->GetValue()));
            if (portSize < 0)
            {
                errorStream << "\n" << "   -Portfolio size must be greater than zero.";
            }
        }
        catch (const std::exception& e)
        {
            portSize = -1;
            portSizeCheck = "   -Portfolio size cannot have letters or symbols.";
            errorStream << "\n" << portSizeCheck;
        }
        if (errorStream.str() == "")
        {
            myRequester.updateStock("^GSPC");                       //need to get a "market" dataset in order to calculate betas. Market data required to add stocks to frontier
            wxVariant horizonVar2 = horizonEntry->GetValue();
            myRequester.pullData(horizonVar2.GetLong());
            index->setPriceData();
            index->calcReturnMap();
            index->calcDailyAVGRet();
            index->calcDailySTD();
            index->calcDailyAVGRet();
            myRequester.pullRiskFree();
            investPort->setRiskFree(myRequester.riskFree/100, interest);
            investPort->setRisk(risk);
            wxVariant tempTicker;
            wxVariant tempPrice;
            investPort->stockHolder.clear();
            for (auto i = 0; i < stockList->GetItemCount(); i++)
            {
                stockList->GetValueByRow(tempTicker, i, 1);
                stockList->GetValueByRow(tempPrice, i, 2);
                investPort->addStock(std::string(tempTicker), std::stod(std::string(tempPrice)), index->returns, index->dailyAVGRet, index->dailySTD);
            }
            wxString msg1;
            investPort->setCovarMatrix();
            investPort->calcIntermediateVars();
            investPort->calcMaxSTD();
            investPort->calcRiskPrefSTD();
            investPort->calcReturn(investPort->getSTD(minVar), minVar);
            investPort->calcReturn(investPort->getSTD(maxSTD), maxSTD);
            investPort->calcReturn(investPort->getSTD(riskPref), riskPref);
            investPort->calcMaxSharpeStdAndWeights();
            investPort->calcWeights(minVar);
            investPort->calcWeights(maxSTD);
            investPort->calcWeights(riskPref);
            stats = new outputstats(investPort->getWeights(riskPref),investPort->stockHolder,portSize);
           
            frontierParab = new parabola("Efficient Frontier", mpALIGN_RIGHT, investPort->getIntermediateVar("eBar"), investPort->getIntermediateVar("C"), investPort->getIntermediateVar("D"), investPort->getSTD(minVar), investPort->getSTD(maxSTD));
            capitalAllocLine = new tangentline("Capital Allocation Line", mpALIGN_LEFT, investPort->getIntermediateVar("A"), investPort->getIntermediateVar("B"), investPort->getIntermediateVar("C"), myRequester.riskFree/100, levAmt, interest / 100, investPort->getSTD(maxShp), investPort->getSTD(minVar), investPort->getSTD(maxSTD));
            mpScaleX* xaxis = new mpScaleX(wxT("Standard Deviation"), mpALIGN_BORDER_BOTTOM);
            mpScaleY* yaxis = new mpScaleY(wxT("Return"), mpALIGN_BORDER_LEFT);
            mpBitmapLayer* bmplayer1 = new mpBitmapLayer();
            mpBitmapLayer* bmplayer2 = new mpBitmapLayer();
            mpBitmapLayer* bmplayer3 = new mpBitmapLayer();
            graphWindow->DelAllLayers(false, true);
            graphWindow->SetMargins(50, 50, 50, 0);
            graphWindow->SetPosY(investPort->getReturn(minVar) + (investPort->getReturn(maxShp) - investPort->getReturn(minVar)) / 2);
            graphWindow->SetPosX(investPort->getSTD(maxSTD) / 2);
            graphWindow->SetScaleX(500 / investPort->getSTD(maxSTD));  //scale relative to mpWindow units
            graphWindow->SetScaleY(500 / investPort->getReturn(maxSTD));
            

            graphWindow->AddLayer(frontierParab);
            graphWindow->AddLayer(capitalAllocLine);
            graphWindow->AddLayer(xaxis);
            graphWindow->AddLayer(yaxis);
           
            wxImage        load1;
            load1.LoadFile(wxT("./minVar.bmp"), wxBITMAP_TYPE_BMP);
            bmplayer1->SetBitmap(load1, investPort->getSTD(minVar), investPort->getReturn(minVar));
            graphWindow->AddLayer(bmplayer1);
           

            wxImage        load2;
            load2.LoadFile(wxT("./maxShp.bmp"), wxBITMAP_TYPE_BMP);
            bmplayer2->SetBitmap(load2, investPort->getSTD(maxShp), investPort->getReturn(maxShp));
            graphWindow->AddLayer(bmplayer2);
           

            wxImage        load3;
            load3.LoadFile(wxT("./riskPref.bmp"), wxBITMAP_TYPE_BMP);
            bmplayer3->SetBitmap(load3, investPort->getSTD(riskPref), investPort->getReturn(riskPref));
            graphWindow->AddLayer(bmplayer3);

            toggleAnalytics(true);
            fillAllocationList(riskPref);
        }
        else
        {
            wxString msg;
            msg.Printf(wxT("The program was unable to generate a portfolio. Please address the following: \n %s"), errorStream.str());
            wxMessageBox(msg, "Portfolio Creator", wxOK | wxICON_INFORMATION, this);
        }
    }
}

void frame::BeginEdit(wxDataViewEvent& event)  //allows editing of tickers in stockList/stockContainer
{
    stockContainer->EditItem(stockContainer->GetSelection(), stockContainer->GetColumn(1));
}

void frame::TrackEdit(wxDataViewEvent& event)  //remembers last row of stockList to be edited
{
    editRow = stockList->GetRow(stockContainer->GetSelection());
    wxVariant temp;
    stockList->GetValueByRow(temp, editRow, 1);
    if (std::string(temp.GetString()) != "" && std::string(temp.GetString()) != "Double-click to edit.")
    {
        toggleAnalytics(false); 
    }
    else
    {
        
    }
}

void frame::PassTicker(wxDataViewEvent& event) //performs some data validation, feeds ticker to requestor, then feeds requested data to stockList on the 2nd page
{   
    checkDuplicates();
    wxVariant stockVar;
    std::string stock1;
    stockList->GetValueByRow(stockVar, editRow, 1);
    stock1 = stockVar.GetString();  
    wxString msg;

    if (duplicates.empty() == false)
    { 
        msg.Printf(wxT("Duplicate detected. Please type in a different stock on row %d."), editRow+1);
        wxMessageBox(msg, "Portfolio Creator", wxOK | wxICON_INFORMATION, this);
        wxVariant defaultStock = "Double-click to edit.";
        stockList->SetValueByRow(defaultStock, editRow, 1);
    }
    else
    {
        if (stock1 != "Double-click to edit.")    //makes sure value has changed from default and isn't in portfolio before adding
        {
            myRequester.updateStock(stock1);
            if (myRequester.givePrice() != "-1" && myRequester.giveIndustry() != "N/A")  //checks for valid response from requester
            {
                wxVariant curPrice = myRequester.givePrice();
                wxVariant industry = myRequester.giveIndustry();
                stockList->SetValueByRow(curPrice, editRow, 2);
                stockList->SetValueByRow(industry, editRow, 3);
                wxVariant horizonVar;
                horizonVar = horizonEntry->GetValue();
                myRequester.pullData(horizonVar.GetLong());
            }
            else
            {
                msg.Printf(wxT("Please type in a valid ticker"));
                wxMessageBox(msg, "Portfolio Creator", wxOK | wxICON_INFORMATION, this);
            }
        }
        else
        {

        }
    }
    
}
  
void frame::checkDuplicates()   //method to more wholistically search for duplicates, not just new duplicates added in the latest edited row
{
    wxVariant stockVar;
    std::string stock1;
    duplicates.clear();

    for (auto i = 0; i < stockList->GetItemCount(); i++)
    {
        wxVariant compare1;
        wxVariant compare2;
        stockList->GetValueByRow(compare1, i, 1);
        for (auto j = i+1; j < stockList->GetItemCount(); j++)
        {
            stockList->GetValueByRow(compare2, j, 1);
            if (std::string(compare1) == std::string(compare2))
            {
                duplicates.push_back(std::string(compare1));
            }
        }
    }
}

void frame::toggleAnalytics(bool onIfTrue)    //toggles analytics on the 3rd page based on whether a portfolio has been generated or not
{
    if (onIfTrue)
    {
        panel3ToggleSizer->ShowItems(true);
        panel3ToggleSizer->Layout();
    }
    else
    {
        panel3ToggleSizer->ShowItems(false);
        panel3ToggleSizer->Layout();
        allocList->DeleteAllItems();
    }
}

void frame::fillAllocationList(portType type)     //fills in allocList with stocks by working with an outputstats object
{        
    
    wxVector<wxVariant> tempItem;
    wxVariant tempTicker = "";
    wxVariant varcount = 0;
    wxVariant tempPrice;
    wxVariant tempIndustry;
    int count = 0;
    std::vector<std::string> tickerHolder = stats->GetTickers();
    Eigen::RowVectorXd tempValues;
    tempValues.resize(stats->g2lLabels.size());
    tempValues = stats->GetDollarAmts();
    Eigen::RowVectorXd tempWeights;
    tempWeights.resize(stats->g2lLabels.size());
    tempWeights = stats->getWeights();
    std::stringstream test;
    test << "Here are the Tickers: " << "\n";
    for (int i = 0; i < stats->g2lLabels.size(); i++)
    {
        test << tickerHolder[i] << "\n";
    }
    test << "Here are the values: " << "\n";
    for (int i = 0; i < stats->g2lLabels.size(); i++)
    {
        test << tempValues(i) << "\n";
    }
    test << "Portfolio size: " << std::stod(std::string(sizeEntry->GetValue())) << "\n";
    
    test << "Here are the weights: " << "\n";
    for (int i = 0; i < stats->g2lLabels.size(); i++)
    {
     
        test << tempWeights(i) << "\n";
       
    }
    test << "Trying a different way to access weights: " << investPort->getWeights(riskPref) << "\n";
    test << "Trying a different way to access weights: " << stats->getWeights() << "\n";
    wxString msg;
    msg.Printf(wxT("Test string: \n %s"), test.str());
    wxMessageBox(msg, "Portfolio Creator", wxOK | wxICON_INFORMATION, this);

    for (int i = 0; i < stats->g2lLabels.size(); i++)
    {
        
        std::stringstream count;
        count << (i + 1);
        std::stringstream amount;
        amount << (float)((int)(tempValues(0, i)*100))/100;
        std::stringstream weight;
        weight << (float)((int)(tempWeights(0, i)*10000))/100;
        tempItem.clear();
        for (int j = 0; j < stockCount; j++)
        {
            stockList->GetValueByRow(tempTicker, j, 1);
            if (tempTicker == tickerHolder[i])
            {
                stockList->GetValueByRow(tempPrice, j, 2);
                stockList->GetValueByRow(tempIndustry, j, 3);
            }
        }
        
        tempItem.push_back(count.str());
        tempItem.push_back(tickerHolder[i]);
        tempItem.push_back(amount.str());
        tempItem.push_back(weight.str());
        tempItem.push_back(tempPrice);
        tempItem.push_back(tempIndustry);
        
        allocList->AppendItem(tempItem);
        
    }
    
}

bool frame::legitimatePrices()         //data validation, looks to see if a legitimate price was brought in by the requestor. Otherwise signals an issue
{
    wxVariant tempPrice;
    bool legit = true;
    std::string inputString;
    double result;
    char* end;

    for (int i = 0; i < stockCount; i++)
    {
        stockList->GetValueByRow(tempPrice, i, 2);
        inputString = std::string(tempPrice);
        result = strtod(inputString.c_str(), &end);
        if (end == inputString.c_str() || *end != '\0')
        {
            legit = false;
        }
    }
    return legit;
}

wxString frame::GetIntroText()
{
    const char* text =
        "<html>"
        "<head>"
        "</head>"
        "<body>"
        "<h1>Portfolio Creator Introduction</h1>"
        // "<img src = \"Efficient-Frontier.gif\" alt = \"Simply Easy Learning\" width = \"400\" height = \"300\" >"   //using HTML to add photos is an option
        "<p><i>Created by Mark Hohertz</i></p>"
        "<p>This program helps you create a portfolio of equities. Portfolios are bundles of assets which can provide superior risk-adjusted returns over single-asset investing. However, not all portfolios are advantageous to the investor. Using Modern Portfolio Theory (MPT), as described by Nobel prize winning economist Harry Markowitz, one can determine a range of optimal portfolios, thereby avoiding those that do not adequately compensate the investor for assumed risks.</p>"
        "<p>MPT defines risk as standard deviation, a measure of spread around the mean of a dataset. By investing in a portfolio of stocks with varying correlations to the market, this risk can be decreased. However, not all risk can be diversified away with a portfolio. In fact, investors seeking higher returns should expect elevated levels of risk. Whatever the level of risk you chose, MPT maximizes the return you receive in exchange.</p>"
        "<p>To start planning your portfolio, click on the \"Portfolio Inputs\" tab at the top. To begin, fill out the \"Investor Preferences\" section based on your investment timeline, inital investment size, and risk tolerance. After that, click \"Add\" in the \"Equity List\" section to add a row, which you then type the ticker into. Once you are finished adding stocks, click the \"Generate\" button at the bottom right of the section to generate your portfolio on the \"Asset Allocation\" tab.</p>"
        //"<p>Some investors may feel strongly about investing only in companies that align with their own ethics. For this purpose, there is an optional \"Ideology\" tab where you can input an ideological focus. The tab will then generate examples of top companies which match your stance on issues like climate change, women's rights, and diversity. You can then add these companies into the \"Equity Selection\"; tab to create a portfolio.</p>"
              
        "</html>";

    return wxString::FromAscii(text);
}
wxString frame::GetP2Text()
{
    const char* text =
        "<html>"
        "<head>"
        "</head>"
        "<body>"
        "<h1>Portfolio Inputs</h1>"
        "<p><i>Please fill in the information below.</i></p>"
        "</body>"
        "</html>";
    return wxString::FromAscii(text);
}
wxString frame::GetP4Text()
{
    const char* text =
        "<html>"
        "<head>"
        "</head>"
        "<body>"
        "<h1>Portfolio Analytics</h1>"
        "<p><i>Please fill out the \"Portfolio Inputs\" tab and press the \"Generate\" button to see portfolio analytics.</i></p>"
        "</body>"
        "</html>";
    return wxString::FromAscii(text);
}
wxString frame::GetP4Descript()
{
    const char* text =
        "<html>"
        "<head>"
        "</head>"
        "<body>"
        "<p><i>Please fill out the \"Portfolio Inputs\" tab and press the \"Generate\" button to see portfolio analytics.</i></p>"
        "</body>"
        "</html>";
    return wxString::FromAscii(text);
}

wxString frame::GetGraphSummary()
{
    const char* text =
        "<html>"
        "<head>"
        "</head>"
        "<body>"
        "<p>The graph to the right displays all the efficient portfolios capable of being produced from the equities you selected. Though other combinations can be created, the ones on the line represent the only allocations that a logical investor should make. The dot on the line is where your portfolio sits on the efficient frontier of portfolios.  Portfolios get risker as you move left to right on the curve.</p>"
        "</body>"
        "</html>";
    return wxString::FromAscii(text);
}
wxString frame::GetBetaText()
{
    const char* text =
        "<html>"
        "<head>"
        "</head>"
        "<body>"
        "<p>Your portfolio’s beta is ___ when using the S&P 500 as a proxy of the market.  This gives your portfolio a Treynor ratio of ___ compared to ____ for the S&P.  Consider diversifying your portfolio with stocks that yield more or that have lower betas to improve this ratio.</p>"
        "</body>"
        "</html>";
    return wxString::FromAscii(text);
}
wxString frame::GetSharpeText()
{
    const char* text =
        "<html>"
        "<head>"
        "</head>"
        "<body>"
        "<p>Your portfolio’s Sharpe ratio is ___ compared to the S&P’s value of ____.  This means your portfolio’s risk/return tradeoff is better/worse than the S&P 500s.  To improve your portfolio’s Sharpe ratio, consider choosing stocks that yield more or that covary less with each other.</p>"
        "</body>"
        "</html>";
    return wxString::FromAscii(text);
}
wxString frame::GetStdText()
{
    const char* text =
        "<html>"
        "<head>"
        "</head>"
        "<body>"
        "<p>Your portfolio’s yearly returns have a standard deviation of ___ which is ___x that of the S&P 500 and ___x that of the Russell 2000. Assuming past returns hold, your portfolio would be worth $___ in ___ years.  Your largest holdings would be in ____ (ticker), ____ (ticker), and ____ (ticker). </p>"
        "</body>"
        "</html>";
    return wxString::FromAscii(text);
}

wxHtmlWindow* frame::CreateHTMLCtrl(wxWindow* parent, wxString txt)
{
    if (!parent)  
        parent = this;

    wxHtmlWindow* ctrl = new wxHtmlWindow(parent, wxID_ANY,
        wxDefaultPosition,
        FromDIP(wxSize(400, 100)));
    ctrl->SetPage(txt);
    return ctrl;
}
