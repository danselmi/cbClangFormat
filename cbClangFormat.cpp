#include <sdk.h> // Code::Blocks SDK
#include <configurationpanel.h>
#include <cbstyledtextctrl.h>
#include "cbClangFormat.h"
#include "ClangFormatProcess.h"

// Register the plugin with Code::Blocks.
// We are using an anonymous namespace so we don't litter the global one.
namespace
{
    PluginRegistrant<cbClangFormat> reg(_T("cbClangFormat"));

    const long ID_MENU_FORMAT_EDITOR_FILE      = wxNewId();
    const long ID_MENU_FORMAT_EDITOR_SELECTION = wxNewId();
    const long ID_FORMAT_EDITOR_FILE           = wxNewId();
    const long ID_FORMAT_EDITOR_SELECTION      = wxNewId();
    const long ID_FORMAT_PROJECT_FILE          = wxNewId();

    const long ID_PROCESS_PROJECT_FILE         = wxNewId();
    const long ID_PROCESS_EDITOR               = wxNewId();
}

// events handling
BEGIN_EVENT_TABLE(cbClangFormat, cbPlugin)
    EVT_MENU(ID_FORMAT_EDITOR_FILE,                cbClangFormat::OnFormatEditorFile)
    EVT_MENU(ID_FORMAT_EDITOR_SELECTION,           cbClangFormat::OnFormatEditorSelection)
    EVT_MENU(ID_MENU_FORMAT_EDITOR_FILE,           cbClangFormat::OnMenuFormatEditorFile)
    EVT_MENU(ID_MENU_FORMAT_EDITOR_SELECTION,      cbClangFormat::OnMenuFormatEditorSelection)
    EVT_MENU(ID_FORMAT_PROJECT_FILE,               cbClangFormat::OnFormatProjectFile)
    EVT_UPDATE_UI(ID_MENU_FORMAT_EDITOR_FILE,      cbClangFormat::OnUpdateMenuFormatEditorFile)
    EVT_UPDATE_UI(ID_MENU_FORMAT_EDITOR_SELECTION, cbClangFormat::OnUpdateMenuFormatEditorSelection)
    //EVT_UPDATE_UI(ID_FORMAT_PROJECT_FILE,          cbClangFormat::OnUpdateProjectFile)
    EVT_END_PROCESS(ID_PROCESS_EDITOR,             cbClangFormat::OnProcessEnd)
END_EVENT_TABLE()

// constructor
cbClangFormat::cbClangFormat()
{
    // Make sure our resources are available.
    // In the generated boilerplate code we have no resources but when
    // we add some, it will be nice that this code is in place already ;)
    if(!Manager::LoadResource(_T("cbClangFormat.zip")))
    {
        NotifyMissingFile(_T("cbClangFormat.zip"));
    }
}

// destructor
cbClangFormat::~cbClangFormat()
{
}

void cbClangFormat::OnAttach()
{
    // do whatever initialization you need for your plugin
    // NOTE: after this function, the inherited member variable
    // m_IsAttached will be TRUE...
    // You should check for it in other functions, because if it
    // is FALSE, it means that the application did *not* "load"
    // (see: does not need) this plugin...
}

void cbClangFormat::OnRelease(bool appShutDown)
{
    // do de-initialization for your plugin
    // if appShutDown is true, the plugin is unloaded because Code::Blocks is being shut down,
    // which means you must not use any of the SDK Managers
    // NOTE: after this function, the inherited member variable
    // m_IsAttached will be FALSE...
}

void cbClangFormat::BuildMenu(wxMenuBar* menuBar)
{
    Manager::Get()->GetLogManager()->DebugLog(_T("cbClangFormat plugin: Building menu"));
    int i = menuBar->FindMenu(_("&Edit"));
    if (i == wxNOT_FOUND)
    {
        Manager::Get()->GetLogManager()->DebugLog(_T("cbClangFormat plugin: edit menu not found"));
        return;
    }
    wxMenu *editMenu = menuBar->GetMenu(i);

    editMenu->AppendSeparator();
    editMenu->Append(ID_MENU_FORMAT_EDITOR_FILE, _("clang-format"));
    editMenu->Append(ID_MENU_FORMAT_EDITOR_SELECTION, _("clang-format selection"));
}

void cbClangFormat::BuildModuleMenu(const ModuleType type, wxMenu* menu, const FileTreeData* data)
{
    if ( type == mtProjectManager && data != 0 && data->GetKind() == FileTreeData::ftdkFile )
    {
        ProjectFile *prjFile = data->GetProjectFile();
        if( !prjFile )
            return;

        PrepareModuleMenu(menu, prjFile->file.GetFullPath());
    }
    else if ((type == mtEditorManager || type == mtEditorTab) &&
             Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor() )
    {
        PrepareModuleMenu(menu, Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor()->GetFilename());
    }
}

void cbClangFormat::OnProcessGeneratedOutputLine(const wxString &line)
{
    Manager::Get()->GetLogManager()->Log(_T("cbClangFormat::OnProcessGeneratedOutputLine: ") + line);
}

wxString cbClangFormat::GetClangFormatBinaryName()
{
    return wxString(_T("c:\\tools\\bin\\clang-format-r318667.exe"));
}

void cbClangFormat::PrepareModuleMenu(wxMenu* menu, const wxString &fullPath)
{
    EditorManager *edman = Manager::Get()->GetEditorManager();
    if ( edman && edman->IsOpen(fullPath) && edman->GetBuiltinEditor(fullPath) )
    {
        menu->Append(ID_FORMAT_EDITOR_FILE, _("clang-format"));
        if ( edman->GetBuiltinEditor(fullPath)->HasSelection() )
            menu->Append(ID_FORMAT_EDITOR_SELECTION, _("clang-format selection"));
    }
    else
        menu->Append(ID_FORMAT_PROJECT_FILE, _("clang-format"));
    fullPath_ = fullPath;
}

void cbClangFormat::OnMenuFormatEditorFile(wxCommandEvent& event)
{
    EditorManager *edman = Manager::Get()->GetEditorManager();
    if(!edman) return;

    cbEditor *ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    if(!ed) return;

    FormatEditorFile(ed);
}

void cbClangFormat::OnMenuFormatEditorSelection(wxCommandEvent& event)
{
    EditorManager *edman = Manager::Get()->GetEditorManager();
    if(!edman) return;

    cbEditor *ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    if(!ed) return;

    if(!ed->HasSelection()) return;

    FormatEditorFileSelection(ed);
}

void cbClangFormat::OnFormatEditorFile(wxCommandEvent& event)
{
    EditorManager *edman = Manager::Get()->GetEditorManager();
    if (!edman ) return;
    if( !edman->IsOpen(fullPath_) ) return;

    cbEditor *ed = edman->GetBuiltinEditor(fullPath_);
    if(!ed) return;

    FormatEditorFile(ed);
}

void cbClangFormat::OnFormatEditorSelection(wxCommandEvent& event)
{
    EditorManager *edman = Manager::Get()->GetEditorManager();
    if (!edman ) return;
    if( !edman->IsOpen(fullPath_) ) return;

    cbEditor *ed = edman->GetBuiltinEditor(fullPath_);
    if(!ed) return;


    if ( !ed->HasSelection() ) return;

    FormatEditorFileSelection(ed);
}

void cbClangFormat::FormatEditorFile(cbEditor *ed)
{
    wxString cmd( GetClangFormatBinaryName() +
                 _T(" -output-replacements-xml -assume-filename=") + ed->GetFilename() );

    StartClangFormat(cmd, ed);
}

void cbClangFormat::FormatEditorFileSelection(cbEditor *ed)
{
    cbStyledTextCtrl *stc = ed->GetControl();
    if(!stc) return;

    int pos = stc->GetCurrentPos();
    int start = stc->LineFromPosition(pos) + 1;
    pos = stc->GetAnchor();
    int stop = stc->LineFromPosition(pos) + 1;

    if ( start == stop ) return;

    if ( start > stop ) std::swap(start, stop);

    // -lines=5:10
    wxString cmd( GetClangFormatBinaryName() +
                 _T(" -lines=") + wxString::Format(_T("%d"), start) + _T(":") + wxString::Format(_T("%d "), stop) +
                 _T(" -output-replacements-xml -assume-filename=") + ed->GetFilename() );

    StartClangFormat(cmd, ed);
}

void cbClangFormat::StartClangFormat(const wxString &cmd, cbEditor *ed)
{
    ClangFormatProcess *pPrcs = new ClangFormatProcess(this, ID_PROCESS_EDITOR);
    Manager::Get()->GetLogManager()->Log(_("cbClangFormat plugin calling: ") + cmd);
    int pid = wxExecute(cmd, wxEXEC_ASYNC|wxEXEC_MAKE_GROUP_LEADER, pPrcs);
    if ( !pid )
    {
        delete pPrcs;
        pPrcs = NULL;
        Manager::Get()->GetLogManager()->DebugLog(_T("cbClangFormat plugin: error calling clang-format"));
        return;
    }

    wxOutputStream *ostrm = pPrcs->GetOutputStream();
    cbStyledTextCtrl *stc = ed->GetControl();
    if(!ostrm || !stc)
    {
        Manager::Get()->GetLogManager()->DebugLog(_T("cbClangFormat plugin: error getting ostream to process"));
        wxProcess::Kill(pid);
        return;
    }
    wxTextOutputStream sIn(*ostrm);
    wxString str = stc->GetTextRange(0, stc->GetLastPosition());
    sIn.WriteString(str);
    ostrm->Close();
    clangFormatProcesses_[pid] = pPrcs;
}

void cbClangFormat::OnFormatProjectFile(wxCommandEvent& event)
{
    if(fullPath_.IsEmpty()) return;

    wxString cmd( GetClangFormatBinaryName() + _(" -i ") + fullPath_ );
    Manager::Get()->GetLogManager()->Log(_("cbClangFormat plugin calling: ") + cmd);

    // EVT_END_PROCESS for processes with id ID_PROCESS_PROJECT_FILE are not processed so they delete themselves
    //PipedProcess
    ClangFormatProcess *pPrcs = new ClangFormatProcess(this, ID_PROCESS_PROJECT_FILE);
    if ( !wxExecute(cmd, wxEXEC_ASYNC|wxEXEC_MAKE_GROUP_LEADER, pPrcs) )
    {
        delete pPrcs;
        pPrcs = NULL;
        Manager::Get()->GetLogManager()->DebugLog(_("cbClangFormat plugin: error calling clang-format"));
    }
}

void cbClangFormat::OnUpdateMenuFormatEditorFile(wxUpdateUIEvent& event)
{
    event.Enable(Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor() != nullptr);
}

void cbClangFormat::OnUpdateMenuFormatEditorSelection(wxUpdateUIEvent& event)
{
    cbEditor *ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    event.Enable(ed && ed->HasSelection());
}

void cbClangFormat::OnProcessEnd(wxProcessEvent & event)
{
    Manager::Get()->GetLogManager()->Log(_T("cbClangFormat::OnProcessEnd"));
    int pid = event.GetPid();
    auto it = clangFormatProcesses_.find(pid);
    if ( it == clangFormatProcesses_.end() )
        return;
    clangFormatProcesses_.erase(pid);

    ClangFormatProcess *pPrcs = it->second;
    if(!pPrcs) return;


    while (pPrcs->ReadProcessOutput());
//
//    m_thrd = new CscopeParserThread(this, m_CscouptOutput);
//    m_thrd->Create();
//    m_thrd->Run();
//
//    Manager::Get()->GetLogManager()->Log(_T("parser Thread started"));
    delete pPrcs;
}

//void CscopePlugin::OnParserThreadEnded(wxCommandEvent &event)
//{
//    delete m_pProcess;
//    m_pProcess = NULL;
//
//    m_thrd = NULL;
//
//	CscopeResultTable *result = (CscopeResultTable*)event.GetClientData();
//	m_view->GetWindow()->SetMessage(m_EndMsg, 100);
//	m_view->GetWindow()->BuildTable( result );
//}
