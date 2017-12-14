#ifndef CLANG_FORMAT_PROCESS_H
#define CLANG_FORMAT_PROCESS_H

#include <wx/process.h>
#include <wx/timer.h>

class cbClangFormat;
class ClangFormatProcess : public wxProcess
{
        //DECLARE_DYNAMIC_CLASS(ClangFormatProcess)
        cbClangFormat* parent_;
        wxTimer pollTimer_;
    public:
        ClangFormatProcess(cbClangFormat* parent, long id);
        virtual ~ClangFormatProcess();
        bool ReadProcessOutput();
        wxString getOutput(){return output_;}
    protected:
    private:
        wxString output_;
        virtual void OnTimer(wxTimerEvent& event);
        virtual void OnIdle(wxIdleEvent& event);
        DECLARE_EVENT_TABLE()
};

#endif

