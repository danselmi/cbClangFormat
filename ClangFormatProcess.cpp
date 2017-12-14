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

ClangFormatProcess::~ClangFormatProcess()
{
    pollTimer_.Stop();
}

bool ClangFormatProcess::ReadProcessOutput()
{
    bool hasInput = false;
    if (IsInputAvailable())
    {
        wxTextInputStream ts(*GetInputStream());
        wxString line = ts.ReadLine();

        if(line.Length())
            output_ += line;

        hasInput = true;
    }

    if ( IsErrorAvailable() )
    {
        wxTextInputStream ts(*GetErrorStream());
        wxString line = ts.ReadLine();

        if(line.Length())
            Manager::Get()->GetLogManager()->LogError(_("ClangFormat error: ") + line);

        hasInput = true;
    }
    return hasInput;
}

void ClangFormatProcess::OnTimer(wxTimerEvent&)
{
    while ( ReadProcessOutput() );
    //wxWakeUpIdle();
}

void ClangFormatProcess::OnIdle(wxIdleEvent&)
{
    //while ( ReadProcessOutput() );
}

