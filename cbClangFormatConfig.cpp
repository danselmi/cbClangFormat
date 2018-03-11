#include <sdk.h> // Code::Blocks SDK
#include "cbClangFormatConfig.h"
#include <wx/listctrl.h>
#include <wx/filepicker.h>

namespace
{
const long ID_FILEPICKERCTRL = wxNewId();
}

cbClangFormatConfigPanel::cbClangFormatConfigPanel(wxString &clangFormatPath,  wxWindow* parent, wxWindowID id):
clangFormatPath_(clangFormatPath)
{
    Create( parent );

    const wxString message(_T("select clang-format executable"));

    #if defined(__WXMSW__)
    wxString fileSelectorWildcardStr = _T("*.exe");
    #else
    wxString fileSelectorWildcardStr = wxFileSelectorDefaultWildcardStr;
    #endif

    wxBoxSizer *BoxSizer = new wxBoxSizer(wxVERTICAL);
	filePickerCtrl_ = new wxFilePickerCtrl(this, ID_FILEPICKERCTRL, clangFormatPath_, message, fileSelectorWildcardStr, wxDefaultPosition, wxDefaultSize, wxFLP_FILE_MUST_EXIST|wxFLP_OPEN|wxFLP_USE_TEXTCTRL, wxDefaultValidator, _T("ID_FILEPICKERCTRL")),
	BoxSizer->Add(filePickerCtrl_, 0, wxALL|wxEXPAND, 5);
	BoxSizer->Add(0,0,1, wxALL|wxEXPAND|wxSHAPED, 5);
	SetSizer(BoxSizer);
	BoxSizer->Fit(this);
	BoxSizer->SetSizeHints(this);
}

void cbClangFormatConfigPanel::OnApply()
{
    clangFormatPath_ = filePickerCtrl_->GetPath();
}
