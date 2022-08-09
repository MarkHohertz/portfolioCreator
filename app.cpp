#include "app.h"

wxIMPLEMENT_APP(app);

app::app()
{
}
app::~app()
{
}
bool app::OnInit()
{
    if (!wxApp::OnInit())  //If app hasn't been initialized, return false
        return false;

    frame* appframe =
        new frame(NULL, "Portfolio Generator", 40, 40, 1200,900); //adds the frame, gives title, proportions/location

    appframe->Show(true);
    return true;
}