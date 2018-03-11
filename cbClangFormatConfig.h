#ifndef CB_CLANG_FORMAT_CONF_PANEL_H
#define CB_CLANG_FORMAT_CONF_PANEL_H

#include <configurationpanel.h>
class wxFilePickerCtrl;
// ----------------------------------------------------------------------------
class cbClangFormatConfigPanel: public cbConfigurationPanel
// ----------------------------------------------------------------------------
{
public:

	/** Constructor. */
    cbClangFormatConfigPanel(wxString &clangFormatPath,  wxWindow* parent = NULL, wxWindowID id = -1);
    ~cbClangFormatConfigPanel(){}

	/** Returns the title displayed in the left column of the "Settings/Environment" dialog. */
	wxString GetTitle()          const {return wxT("cbClangFormat");}

	/** Returns string used to build active/inactive images path in the left column
	  * of the "Settings/Environment" dialog.
	  */
	wxString GetBitmapBaseName() const {return wxT("cbClangFormat");}

	/** Called automatically when user clicks on OK
	  */
	void OnApply();

	/** Called automatically when user clicks on Cancel
	  */
	void OnCancel(){}

private:
    wxString &clangFormatPath_;
    //wxString temporaryClangFormatPath_;
    wxFilePickerCtrl *filePickerCtrl_;

};


#endif

