#ifndef CLANG_FORMAT_PROCESS_H
#define CLANG_FORMAT_PROCESS_H

#include <wx/process.h>


class cbClangFormat;
class ClangFormatProcess : public wxProcess
{
        //DECLARE_DYNAMIC_CLASS(ClangFormatProcess)
        cbClangFormat* m_parent;
    public:
        ClangFormatProcess(cbClangFormat* parent, long id);
        virtual ~ClangFormatProcess();
        bool ReadProcessOutput();
    protected:
    private:
};

#endif

