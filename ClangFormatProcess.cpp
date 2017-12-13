#include "ClangFormatProcess.h"
#include "cbClangFormat.h"
#include <wx/txtstrm.h>
#include <sdk.h> // Code::Blocks SDK

BEGIN_EVENT_TABLE(ClangFormatProcess, wxProcess)
    EVT_TIMER(wxID_ANY, ClangFormatProcess::OnTimer)
    EVT_IDLE(ClangFormatProcess::OnIdle)
END_EVENT_TABLE()

ClangFormatProcess::ClangFormatProcess(cbClangFormat* parent, long id):
    wxProcess(parent, id),
    pollTimer_(this)
{
    wxASSERT(parent);
    parent_ = parent;

    //turn redirection on
    Redirect();
    pollTimer_.Start(50);
}

ClangFormatProcess::~ClangFormatProcess(){}

bool ClangFormatProcess::ReadProcessOutput()
{
    Manager::Get()->GetLogManager()->Log(_("ClangFormatProcess::ReadProcessOutput()"));
    bool hasInput = false;
    if (IsInputAvailable())
    {

        wxTextInputStream ts(*GetInputStream());
        wxString line = ts.ReadLine();

        if(line.Length())
        {
            parent_->OnProcessGeneratedOutputLine(line);
        }
        hasInput = true;
    }

    if ( IsErrorAvailable() )
    {
        Manager::Get()->GetLogManager()->Log(_("IsErrorAvailable"));
        wxTextInputStream ts(*GetErrorStream());
        wxString line = ts.ReadLine();

        if(line.Length())
        {
            parent_->OnProcessGeneratedOutputLine(line);
        }
        hasInput = true;
    }
    return hasInput;
}

void ClangFormatProcess::OnTimer(wxTimerEvent&)
{
    wxWakeUpIdle();
}

void ClangFormatProcess::OnIdle(wxIdleEvent&)
{
    while ( ReadProcessOutput() )
        ;
}

