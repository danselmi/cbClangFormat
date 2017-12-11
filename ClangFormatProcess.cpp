#include "ClangFormatProcess.h"
#include "cbClangFormat.h"
#include <wx/txtstrm.h>

ClangFormatProcess::ClangFormatProcess(cbClangFormat* parent, long id):
    wxProcess(parent, id)
{
    wxASSERT(parent);
    m_parent = parent;

    //turn redirection on
    Redirect();
}

ClangFormatProcess::~ClangFormatProcess(){}

bool ClangFormatProcess::ReadProcessOutput()
{
    if (IsInputAvailable())
    {
        wxTextInputStream ts(*GetInputStream());
        wxString line = ts.ReadLine();

        if(line.Length())
        {
            m_parent->OnProcessGeneratedOutputLine(line);
        }
        return true;
    }
    return false;
}

