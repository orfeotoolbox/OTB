/////////////////////////////////////////////////////////////////////////////
// Name:        bgimsystem.cpp
// Purpose:     Image processing system
// Author:      Bogdan Georgescu, Chris M. Christoudias
// Modified by:
// Created:     06/22/2000
// Copyright:   (c) Bogdan Georgescu, Chris M. Christoudias
// Version:     v0.1
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
   #pragma hdrstop
#endif

#ifndef WX_PRECOMP
   #include "wx/wx.h"
#endif

#include <wx/toolbar.h>
#include <wx/progdlg.h>

#if defined(__WXGTK__) || defined(__WXMOTIF__)
   #include "icons/mondrian.xpm"
   #include "icons/new.xpm"
   #include "icons/open.xpm"
   #include "icons/save.xpm"
   #include "icons/copy.xpm"
   #include "icons/cut.xpm"
   #include "icons/paste.xpm"
   #include "icons/print.xpm"
   #include "icons/help.xpm"
#endif

#include <wx/html/htmlwin.h>

#include "BgImagPGM.h"
#include "BgImagPNM.h"

// Edge detection include stuff
#include <math.h>
#include "BgImage.h"
#include "BgEdge.h"
#include "BgEdgeList.h"
#include "BgEdgeDetect.h"
#include "BgDefaults.h"

#include "msImageProcessor.h"

#include <stdio.h>
#include <string.h>
#include "bgimsystem.h"

IMPLEMENT_APP(BgApp)
// ---------------------------------------------------------------------------
// global variables
// ---------------------------------------------------------------------------

BgMdiFrame *g_frame = (BgMdiFrame *) NULL;
wxList g_children;

// For drawing lines in a canvas
static long g_xpos = -1;
static long g_ypos = -1;

static int	gs_nFrames	= 0;
static bool	on_exit		= false;
#define DEFAULT_LOG_SIZE 100

// ---------------------------------------------------------------------------
// event tables
// ---------------------------------------------------------------------------

BEGIN_EVENT_TABLE(BgMdiFrame, wxMDIParentFrame)
EVT_MENU(BG_ABOUT, BgMdiFrame::OnAbout)
EVT_MENU(BG_HELP, BgMdiFrame::OnHelp)
EVT_MENU(BG_QUIT, BgMdiFrame::OnQuit)
EVT_MENU(BG_NEW_EDGE_WINDOW, BgMdiFrame::OnNewEdgeWindow)
EVT_MENU(BG_NEW_SEGM_WINDOW, BgMdiFrame::OnNewSegmWindow)
EVT_MENU(BG_LOAD_IMAGE, BgMdiFrame::OnLoadImage)
EVT_MENU(BG_LOAD_IMAGE_EDGE, BgMdiFrame::OnLoadImageEdge)
EVT_MENU(BG_SEGM_LOAD_IMAGE, BgMdiFrame::OnLoadImageSegm)
EVT_MENU(BG_SAVE_RESULT, BgMdiFrame::OnSaveResult)
EVT_MENU(BG_CROSS, BgMdiFrame::ZoomControl)
EVT_MENU(BG_ZOOM_IN, BgMdiFrame::ZoomControl)
EVT_MENU(BG_ZOOM_OUT, BgMdiFrame::ZoomControl)
EVT_MENU(BG_POINTER, BgMdiFrame::ZoomControl)
EVT_CLOSE(BgMdiFrame::OnClose)

EVT_SIZE(BgMdiFrame::OnSize)
END_EVENT_TABLE()

// Note that BG_NEW_WINDOW and BG_ABOUT commands get passed
// to the parent window for processing, so no need to
// duplicate event handlers here.

BEGIN_EVENT_TABLE(BgMdiEdgeChild, wxMDIChildFrame)
EVT_MENU(BG_LOAD_IMAGE_EDGE, BgMdiEdgeChild::OnLoadImage)
EVT_MENU(BG_EDGE_DETECT, BgMdiEdgeChild::OnEdgeDetect)
EVT_MENU(BG_CHANGE_PARAM_EDGE, BgMdiEdgeChild::OnChangeParam)
EVT_MENU(BG_CHILD_EDGE_QUIT, BgMdiEdgeChild::OnQuit)
EVT_MENU(BG_EDGE_VIEW_ORIG, BgMdiEdgeChild::OnViewOrig)
EVT_MENU(BG_EDGE_VIEW_EDGE, BgMdiEdgeChild::OnViewEdge)
EVT_MENU(BG_EDGE_SAVE_MAP, BgMdiEdgeChild::OnSaveEdgeMap)

EVT_SET_FOCUS(BgMdiEdgeChild::OnFocus)
EVT_CLOSE(BgMdiEdgeChild::OnClose)
EVT_SIZE(BgMdiEdgeChild::OnSize)

EVT_BUTTON(BG_EDGE_DETECT, BgMdiEdgeChild::OnEdgeDetect)
EVT_BUTTON(BG_CHANGE_PARAM_EDGE, BgMdiEdgeChild::OnChangeParam)
EVT_CHECKBOX(BG_EDGE_CVIEW_ORIG, BgMdiEdgeChild::OnCViewOrig)
EVT_CHECKBOX(BG_EDGE_CVIEW_EDGE, BgMdiEdgeChild::OnCViewEdge)

EVT_COMMAND(BG_EVENT_UPDATE_ID, BG_EVENT_UPDATE, BgMdiEdgeChild::OnUpdateNum)

END_EVENT_TABLE()

BEGIN_EVENT_TABLE(BgMdiSegmentChild, wxMDIChildFrame)
EVT_MENU(BG_SEGM_LOAD_IMAGE, BgMdiSegmentChild::OnLoadImage)
EVT_MENU(BG_SEGM_SAVE_SEGMENTED, BgMdiSegmentChild::OnSaveSegmentedImage)
EVT_MENU(BG_SEGM_SAVE_EDGEMAP, BgMdiSegmentChild::OnSaveBoundaries)
EVT_MENU(BG_CHILD_SEGM_QUIT, BgMdiSegmentChild::OnQuit)
EVT_MENU(BG_SEGM_SEGMENT, BgMdiSegmentChild::OnSegment)
EVT_MENU(BG_SEGM_LOAD_MAP, BgMdiSegmentChild::LoadCustomWeightMap)
EVT_MENU(BG_SEGM_SPEEDUP_NONE, BgMdiSegmentChild::OnUpdateSpeedUpLevel)
EVT_MENU(BG_SEGM_SPEEDUP_MEDM, BgMdiSegmentChild::OnUpdateSpeedUpLevel)
EVT_MENU(BG_SEGM_SPEEDUP_HIGH, BgMdiSegmentChild::OnUpdateSpeedUpLevel)
EVT_MENU(BG_CANVAS_VIEW1_GRADMAP, BgMdiSegmentChild::OnUpdatePlotWindow1)
EVT_MENU(BG_CANVAS_VIEW1_CONFMAP, BgMdiSegmentChild::OnUpdatePlotWindow1)
EVT_MENU(BG_CANVAS_VIEW1_WEITMAP, BgMdiSegmentChild::OnUpdatePlotWindow1)
EVT_MENU(BG_CANVAS_VIEW1_CUSTMAP, BgMdiSegmentChild::OnUpdatePlotWindow1)
EVT_MENU(BG_CANVAS_VIEW2_GRADMAP, BgMdiSegmentChild::OnUpdatePlotWindow2)
EVT_MENU(BG_CANVAS_VIEW2_CONFMAP, BgMdiSegmentChild::OnUpdatePlotWindow2)
EVT_MENU(BG_CANVAS_VIEW2_WEITMAP, BgMdiSegmentChild::OnUpdatePlotWindow2)
EVT_MENU(BG_CANVAS_VIEW2_CUSTMAP, BgMdiSegmentChild::OnUpdatePlotWindow2)
EVT_MENU(BG_CANVAS_SAVE_GRADMAP, BgMdiSegmentChild::OnSaveEdgeInformation)
EVT_MENU(BG_CANVAS_SAVE_CONFMAP, BgMdiSegmentChild::OnSaveEdgeInformation)
EVT_MENU(BG_CANVAS_SAVE_WEITMAP, BgMdiSegmentChild::OnSaveEdgeInformation)

EVT_SET_FOCUS(BgMdiSegmentChild::OnFocus)
EVT_CLOSE(BgMdiSegmentChild::OnClose)
EVT_SIZE(BgMdiSegmentChild::OnSize)

EVT_BUTTON(BG_SEGM_LOAD_IMAGE, BgMdiSegmentChild::OnLoadImage)
EVT_BUTTON(BG_SEGM_SEGMENT, BgMdiSegmentChild::OnSegment)
EVT_BUTTON(BG_SEGM_LOAD_MAP, BgMdiSegmentChild::LoadCustomWeightMap)
EVT_RADIOBOX(BG_SEGM_VIEW_IMSEG, BgMdiSegmentChild::OnViewImSeg)
EVT_RADIOBOX(BG_SEGM_OPERATION, BgMdiSegmentChild::OnChangeOperation)
EVT_CHECKBOX(BG_SEGM_VIEW_EDGES, BgMdiSegmentChild::OnViewBoundaries)
EVT_CHECKBOX(BG_SEGM_USE_EDGE_MAP, BgMdiSegmentChild::OnUseWeightMap)
EVT_COMBOBOX(BG_SEGM_CHANGE_PARAMS, BgMdiSegmentChild::OnChangeParameters)

EVT_TEXT(BG_SEGM_TEXT_SIGMAS, BgMdiSegmentChild::OnUpdateTextBoxes)
EVT_TEXT(BG_SEGM_TEXT_SIGMAR, BgMdiSegmentChild::OnUpdateTextBoxes)
EVT_TEXT(BG_SEGM_TEXT_MINREG, BgMdiSegmentChild::OnUpdateTextBoxes)
EVT_TEXT(BG_SEGM_TEXT_GRADWIN, BgMdiSegmentChild::OnUpdateTextBoxes)
EVT_TEXT(BG_SEGM_TEXT_AIJ, BgMdiSegmentChild::OnUpdateTextBoxes)
EVT_TEXT(BG_SEGM_TEXT_EPSILON, BgMdiSegmentChild::OnUpdateTextBoxes)

END_EVENT_TABLE()

BEGIN_EVENT_TABLE(BgImCanvas, wxScrolledWindow)
EVT_RIGHT_DOWN(BgImCanvas::OnMouseRightDown)
EVT_MOUSE_EVENTS(BgImCanvas::OnEvent)

EVT_MENU(BG_IMC_ADDNODE, BgImCanvas::OnCustomAddNode)
EVT_MENU(BG_IMC_DELETENODE, BgImCanvas::OnCustomDeleteNode)
EVT_MENU(BG_IMC_SELTYPE_ELLIPSE, BgImCanvas::OnCTypeEllipse)
EVT_MENU(BG_IMC_SELTYPE_VLINE, BgImCanvas::OnCTypeVLine)
EVT_MENU(BG_IMC_SELTYPE_HLINE, BgImCanvas::OnCTypeHLine)
EVT_MENU(BG_IMC_SELTYPE_LINE, BgImCanvas::OnCTypeLine)
EVT_MENU(BG_IMC_SELTYPE_BOX, BgImCanvas::OnCTypeBox)
EVT_MENU(BG_IMC_SELTYPE_CUSTOM, BgImCanvas::OnCTypeCustom)

//EVT_SCROLLWIN(BgImCanvas::OnScroll)

END_EVENT_TABLE()

BEGIN_EVENT_TABLE(BgParamDialog, wxDialog)
EVT_BUTTON(BG_PARAMD_OK, BgParamDialog::OnOk)
EVT_BUTTON(BG_PARAMD_CANCEL, BgParamDialog::OnCancel)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(BgSpeedSelect, wxDialog)
EVT_BUTTON(BG_SPEEDSEL_OK, BgSpeedSelect::OnOk)
EVT_BUTTON(BG_SPEEDSEL_CANCEL, BgSpeedSelect::OnCancel)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(BgDialog, wxDialog)
EVT_BUTTON(BG_DIALOG_OK, BgDialog::OnExit)
EVT_PAINT(BgDialog::OnPaint)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(BgHoverBar, wxWindow)
EVT_BUTTON(BG_CANVAS_VIEW_BUTTON, BgHoverBar::ShowMenu)
EVT_BUTTON(BG_CANVAS_SAVE_BUTTON, BgHoverBar::ShowMenu)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(BgMenuPanel, wxPanel)
EVT_BUTTON(BG_CANVAS_VIEW_BUTTON, BgMenuPanel::ShowMenu)
EVT_BUTTON(BG_CANVAS_SAVE_BUTTON, BgMenuPanel::ShowMenu)
EVT_SIZE(BgMenuPanel::OnSize)
END_EVENT_TABLE()

// ===========================================================================
// implementation
// ===========================================================================

// ---------------------------------------------------------------------------
// Global Data used for Multi-Threaded Enviornment
// ---------------------------------------------------------------------------

bool	stop_flag;
int		percentDone;

// ---------------------------------------------------------------------------
// Log function
// ---------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// wxLogTextCtrl implementation
// ----------------------------------------------------------------------------

bgLogTextCtrl::bgLogTextCtrl(wxTextCtrl *pTextCtrl)
{
    m_pTextCtrl = pTextCtrl;
}

void bgLogTextCtrl::DoLogString(const wxChar *szString, time_t WXUNUSED(t))
{
    wxString msg;
    TimeStamp(&msg);
    msg << szString;

    m_pTextCtrl->AppendText(msg);
}

#define VAR_LOG_BUFFER_SIZE   (4096)

static wxChar varszBuf[VAR_LOG_BUFFER_SIZE];

FILE* glogfile;

void bgLog(const char* szFormat, ...)
{
   va_list argptr;
   va_start(argptr, szFormat);
   wxVsnprintf(varszBuf, WXSIZEOF(varszBuf), szFormat, argptr);
   va_end(argptr);
   ::wxLogMessage(varszBuf);
   bgLogFile(varszBuf);
}

void bgLogVar(const char* first, va_list alist)
{
//   va_start(alist, first);
   wxVsnprintf(varszBuf, WXSIZEOF(varszBuf), first, alist);
//   va_end(alist);
   ::wxLogMessage(varszBuf);
   bgLogFile(varszBuf);
}

void bgLogFile(const char* szFormat, ...)
{
   if (glogfile == 0)
      glogfile = fopen("filelog.txt", "w");
   va_list argptr;
   va_start(argptr, szFormat);
   vfprintf(glogfile, szFormat, argptr);
   va_end(argptr);
   fflush(glogfile);
}

inline int bgRound(double inline_x)
{
    return ((int) (inline_x+0.5));
}

// ---------------------------------------------------------------------------
// BgApp
// ---------------------------------------------------------------------------

// Initialise this in OnInit, not statically
bool BgApp::OnInit()
{
   // Create the main frame window
   
   g_frame = new BgMdiFrame((wxFrame *)NULL, -1, "Edge Detection and Image SegmentatiON System (EDISON)",
      wxPoint(10, 10), wxSize(1024, 768),
      wxDEFAULT_FRAME_STYLE | wxHSCROLL | wxVSCROLL);
#ifdef __WXMSW__
#if 0
   // Experimental: change the window menu
   wxMenu* windowMenu = new wxMenu;
   windowMenu->Append(5000, "My menu item!");
   frame->SetWindowMenu(windowMenu);
#endif
#endif
   
   // Give it an icon
#ifdef __WXMSW__
   g_frame->SetIcon(wxIcon("bg_icn"));
#else
   g_frame->SetIcon(wxIcon( mondrian_xpm ));
#endif

   // Make a menubar
   wxMenu *file_menu = new wxMenu;
   
   file_menu->Append(BG_NEW_EDGE_WINDOW, "New &edge window\tAlt-E", "Create a new edge detect window");
   file_menu->Append(BG_NEW_SEGM_WINDOW, "New &segment window\tAlt-S", "Create a new segmentation window");
   file_menu->Append(BG_LOAD_IMAGE_EDGE, "&Load edge image\tCtrl-L", "Load a new image to perform edge detection");
   file_menu->Append(BG_SEGM_LOAD_IMAGE, "&Load segment image\tShift-L", "Load a new image to perform segmentation");
   file_menu->Append(BG_QUIT, "E&xit\tAlt-X", "Quit the program");
   
   wxMenu *help_menu = new wxMenu;
   help_menu->Append(BG_ABOUT, "&About");
   help_menu->Append(BG_HELP, "&Help\tCtrl-H");
   
   wxMenuBar *menu_bar = new wxMenuBar;
   
   menu_bar->Append(file_menu, "&File");
   menu_bar->Append(help_menu, "&Help");

   // Associate the menu bar with the frame
   g_frame->SetMenuBar(menu_bar);
   
   g_frame->CreateStatusBar();
   
   g_frame->Show(TRUE);
   
   SetTopWindow(g_frame);

#if wxUSE_LIBPNG
  wxImage::AddHandler( new wxPNGHandler );
#endif

#if wxUSE_LIBJPEG
  wxImage::AddHandler( new wxJPEGHandler );
#endif

#if wxUSE_LIBTIFF
  wxImage::AddHandler( new wxTIFFHandler );
#endif

#if wxUSE_GIF
  wxImage::AddHandler( new wxGIFHandler );
#endif

#if wxUSE_PCX
  wxImage::AddHandler( new wxPCXHandler );
#endif

  wxImage::AddHandler( new bgPNMHandler );
  wxImage::AddHandler( new bgPGMHandler );
   return TRUE;
}

// ---------------------------------------------------------------------------
// BgMdiFrame
// ---------------------------------------------------------------------------

// Define my frame constructor
BgMdiFrame::BgMdiFrame(wxWindow *parent,
                       const wxWindowID id,
                       const wxString& title,
                       const wxPoint& pos,
                       const wxSize& size,
                       const long style)
                       : wxMDIParentFrame(parent, id, title, pos, size, style)
{
    logtext_ = new wxTextCtrl(this, -1, "Log window.\n",
                            wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxSUNKEN_BORDER | wxTE_READONLY);
    logtext_->SetBackgroundColour("wheat");
     bglogctrl_ = new bgLogTextCtrl(logtext_);
    logTargetOld_ = wxLog::SetActiveTarget(bglogctrl_);
    logsize_ = DEFAULT_LOG_SIZE;

   CreateToolBar(wxNO_BORDER | wxTB_FLAT | wxTB_HORIZONTAL);
   InitToolBar(GetToolBar());
   
   //get program location directory
   strcpy(programDir_, wxGetCwd());

   //get help directory location
   //(NOTE: This code must be altered to function properly in UNIX.)
   strcpy(helpDir_, programDir_);
   strcat(helpDir_, "\\doc\\help.html");

   // Accelerators
   wxAcceleratorEntry entries[13];
   entries[0].Set(wxACCEL_ALT, (int) 'E', BG_NEW_EDGE_WINDOW);
   entries[1].Set(wxACCEL_ALT, (int) 'S', BG_NEW_SEGM_WINDOW);
   entries[2].Set(wxACCEL_ALT, (int) 'X', BG_QUIT);
   entries[3].Set(wxACCEL_ALT, (int) 'C', BG_CHILD_EDGE_QUIT);
   entries[4].Set(wxACCEL_SHIFT, (int) 'C', BG_CHILD_SEGM_QUIT);
   entries[5].Set(wxACCEL_CTRL, (int) 'H', BG_HELP);
   entries[6].Set(wxACCEL_CTRL, (int) 'S', BG_EDGE_SAVE_MAP);
   entries[7].Set(wxACCEL_SHIFT, (int) 'S', BG_SEGM_SAVE_SEGMENTED);
   entries[8].Set(wxACCEL_CTRL, (int) 'L', BG_LOAD_IMAGE_EDGE);
   entries[9].Set(wxACCEL_SHIFT, (int) 'L', BG_SEGM_LOAD_IMAGE);
   entries[10].Set(wxACCEL_SHIFT, (int) 'M', BG_SEGM_LOAD_MAP);
   entries[11].Set(wxACCEL_CTRL, (int) 'R', BG_EDGE_DETECT);
   entries[12].Set(wxACCEL_SHIFT, (int) 'R', BG_SEGM_SEGMENT);
   wxAcceleratorTable accel(13, entries);
   SetAcceleratorTable(accel);
   
   // bgFileLog
   //glogfile = fopen("filelog.txt", "w");
   glogfile = 0;
}

void BgMdiFrame::OnClose(wxCloseEvent& event)
{
   if ( event.CanVeto() && (gs_nFrames > 0) )
   {
      wxString msg;
      if (gs_nFrames == 1)
         msg.Printf(_T("%d window still open, close anyhow?"), gs_nFrames);
      else
         msg.Printf(_T("%d windows still open, close anyhow?"), gs_nFrames);
      if ( wxMessageBox(msg, "Please confirm",
         wxICON_QUESTION | wxYES_NO) != wxYES )
      {
         event.Veto();
         
         return;
      }
   }

   //indicate that the system is exiting
   on_exit	= true;

   wxLog::SetActiveTarget(logTargetOld_);
   delete bglogctrl_;
   delete logtext_;

   // bgFileLog
   if (glogfile != 0)
      fclose(glogfile);

   event.Skip();
}

//sets the title of the active child frame
void BgMdiFrame::SetChildTitle(wxMDIChildFrame *activeChild, int zconst, int maxZoom, int minZoom)
{
	wxString	title;
	if(activeChild->GetId() == BG_EDGE_WINDOW)
	{
		BgMdiEdgeChild *edgeChild	= (BgMdiEdgeChild *) activeChild;
		if(maxZoom)
			title.Printf(_T("Edge Detection Frame %d - %s (%d x %d) x %d [Maximum Zoom]"), edgeChild->window_number_, edgeChild->filename_, edgeChild->width_, edgeChild->height_, zconst);
		else if(minZoom)
			title.Printf(_T("Edge Detection Frame %d - %s (%d x %d) [Original Image]"), edgeChild->window_number_, edgeChild->filename_, edgeChild->width_, edgeChild->height_);
		else
			title.Printf(_T("Edge Detection Frame %d - %s (%d x %d) x %d [Zoom]"), edgeChild->window_number_, edgeChild->filename_, edgeChild->width_, edgeChild->height_, zconst);
	}
	else if(activeChild->GetId() == BG_SEGM_WINDOW)
	{
		BgMdiSegmentChild *segmChild	= (BgMdiSegmentChild *) activeChild;
		if(maxZoom)
			title.Printf(_T("Segmentation Frame %d - %s (%d x %d) x %d [Maximum Zoom]"), segmChild->window_number_, segmChild->filename_, segmChild->width_, segmChild->height_, zconst);
		else if(minZoom)
			title.Printf(_T("Segmentation Frame %d - %s (%d x %d) [Original Image]"), segmChild->window_number_, segmChild->filename_, segmChild->width_, segmChild->height_);
		else
			title.Printf(_T("Segmentation Frame %d - %s (%d x %d) x %d [Zoom]"), segmChild->window_number_, segmChild->filename_, segmChild->width_, segmChild->height_, zconst);
	}
	activeChild->SetTitle(title);
	return;
}

//updates toolbar when maximum zoom occurs
void BgMdiFrame::UpdateZoomControl(wxMDIChildFrame *activeChild, int maxZoom, int minZoom)
{
	if(activeChild->GetId()	== BG_EDGE_WINDOW)
	{
		if(maxZoom)
			((BgMdiEdgeChild *) activeChild)->maxZoom_	= true;
		else
			((BgMdiEdgeChild *) activeChild)->maxZoom_	= false;
		if(minZoom)
			((BgMdiEdgeChild *) activeChild)->minZoom_	= true;
		else
			((BgMdiEdgeChild *) activeChild)->minZoom_	= false;
		((BgMdiEdgeChild *) activeChild)->UpdateZoomControl();
	}
	else if (activeChild->GetId() == BG_SEGM_WINDOW)
	{
		if(maxZoom)
			((BgMdiSegmentChild *) activeChild)->maxZoom_	= true;
		else
			((BgMdiSegmentChild *) activeChild)->maxZoom_	= false;
		if(minZoom)
			((BgMdiSegmentChild *) activeChild)->minZoom_	= true;
		else
			((BgMdiSegmentChild *) activeChild)->minZoom_	= false;
		((BgMdiSegmentChild *) activeChild)->UpdateZoomControl();
	}
}

void BgMdiFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
   Close();
}

void BgMdiFrame::OnAbout(wxCommandEvent& WXUNUSED(event) )
{
	const int BG_DIALOG_INDENT = 30, BG_DIALOG_TOP_MARGIN = 15;
	BgDialog aboutDialog(this, -1, "About EDISON", wxPoint(-1,-1), wxSize(450, 300), wxDEFAULT_DIALOG_STYLE | wxDIALOG_MODAL | wxBORDER | wxSYSTEM_MENU, "aboutDialog");
	wxFont	myFont(9, wxSWISS, wxNORMAL, wxBOLD, false);
	BgText	bgText(0, "Edge Detection and Image SegmentatiON System (EDISON) v1.1", myFont, BG_DIALOG_INDENT, BG_DIALOG_TOP_MARGIN);
	aboutDialog.AddText(&bgText);
	myFont.SetWeight(wxNORMAL);
	myFont.SetUnderlined(true);
	bgText.SetId(1);
	bgText.SetFont(myFont);
	bgText.SetText("Authors");
	bgText.SetPlotLocation(BG_DIALOG_INDENT, BG_DIALOG_TOP_MARGIN+15);
	aboutDialog.AddText(&bgText);
	myFont.SetUnderlined(false);
	bgText.SetId(2);
	bgText.SetFont(myFont);
	bgText.SetText(": Bogdan Georgescu, Chris Christoudias (freeware) 2002");
	bgText.SetPlotLocation(BG_DIALOG_INDENT+44, BG_DIALOG_TOP_MARGIN+15);
	aboutDialog.AddText(&bgText);
	bgText.SetId(3);
	bgText.SetText("Center for Advanced Information Processing (CAIP), Rutgers University");
	bgText.SetPlotLocation(BG_DIALOG_INDENT, BG_DIALOG_TOP_MARGIN+30);
	aboutDialog.AddText(&bgText);
	wxBitmap riul_logo("riul_logo", wxBITMAP_TYPE_RESOURCE), caip_logo("caip_logo", wxBITMAP_TYPE_RESOURCE), rutgers_logo("rutgers_logo", wxBITMAP_TYPE_RESOURCE);
	BgBitmap myBitmap(&riul_logo, 0, BG_DIALOG_INDENT+20, BG_DIALOG_TOP_MARGIN+60);
	aboutDialog.AddBitmap(&myBitmap);
	myBitmap.SetId(1);
	myBitmap.SetMap(&caip_logo);
	myBitmap.SetPlotLocation(BG_DIALOG_INDENT+85, BG_DIALOG_TOP_MARGIN+170);
	aboutDialog.AddBitmap(&myBitmap);
	myBitmap.SetId(2);
	myBitmap.SetMap(&rutgers_logo);
	myBitmap.SetPlotLocation(BG_DIALOG_INDENT+145, BG_DIALOG_TOP_MARGIN+165);
	aboutDialog.AddBitmap(&myBitmap);
	aboutDialog.Centre(wxBOTH);
	aboutDialog.ShowModal();
}

void BgMdiFrame::OnHelp(wxCommandEvent& WXUNUSED(event) )
{  
   wxDialog helpDialog(this, -1, "Help Window", wxPoint(10,10), wxSize(800,600), wxDEFAULT_DIALOG_STYLE | wxDIALOG_MODAL | wxBORDER | wxSYSTEM_MENU);
   wxHtmlWindow helpWind(&helpDialog, -1, wxPoint(10,10), wxSize(770,550));
   helpWind.LoadPage(helpDir_);
   helpDialog.ShowModal();
   bgLog("\n");
}

void BgMdiFrame::OnLoadImage(wxCommandEvent& WXUNUSED(event))
{

	wxMDIChildFrame *activeChild = GetActiveChild();
	wxCommandEvent zcev;
	if(activeChild)
	{
		if(activeChild->GetId() == BG_EDGE_WINDOW)
			((BgMdiEdgeChild*) activeChild)->OnLoadImage(zcev);
		else if(activeChild->GetId() == BG_SEGM_WINDOW)
			((BgMdiSegmentChild*) activeChild)->OnLoadImage(zcev);
	} else
	{
		//read an image
		char *pathname, *filename;
		GetImageFileInfo(&pathname, &filename);
		if(pathname)
		{

			//get current width and height of this window
			int width, height;
			GetSize(&width, &height);

			//half window width
			width = width/2;

			//create an edge window
			OnNewEdgeWindow(zcev);
			
			//load read image into edge window
			activeChild = GetActiveChild();
			((BgMdiEdgeChild*) activeChild)->ReadImage(pathname, filename);
			((BgMdiEdgeChild*) activeChild)->RunEnable();
			
			//set position and size of edge detection window
			activeChild->SetSize(0,-30,width,height);
			
			//create a segmentation window
			OnNewSegmWindow(zcev);
			
			//load read image into segment window
			activeChild = GetActiveChild();
			((BgMdiSegmentChild*) activeChild)->ReadImage(pathname, filename);
			((BgMdiSegmentChild*) activeChild)->RunEnable();

			//set position and size of segmnetation window
			activeChild->SetSize(width,-30,width,height);
			
			//de-allocate memory used by filename
			delete [] filename;
		}
	}
	
}

void BgMdiFrame::OnLoadImageEdge(wxCommandEvent& WXUNUSED(event))
{
   BgMdiEdgeChild* activeChild;
   activeChild = 0;
   activeChild = (BgMdiEdgeChild*) GetActiveChild();
   wxCommandEvent zcev;
   if (activeChild != 0)
   {
      activeChild->OnLoadImage(zcev);
   } else
   {
      OnNewEdgeWindow(zcev);
      activeChild = (BgMdiEdgeChild*) GetActiveChild();
      activeChild->OnLoadImage(zcev);
   }
}

void BgMdiFrame::OnLoadImageSegm(wxCommandEvent& WXUNUSED(event))
{
   BgMdiSegmentChild* activeChild;
   activeChild = 0;
   activeChild = (BgMdiSegmentChild*) GetActiveChild();
   wxCommandEvent zcev;
   if (activeChild != 0)
   {
      activeChild->OnLoadImage(zcev);
   } else
   {
      OnNewSegmWindow(zcev);
      activeChild = (BgMdiSegmentChild*) GetActiveChild();
      activeChild->OnLoadImage(zcev);
   }
}

void BgMdiFrame::OnSaveResult(wxCommandEvent& WXUNUSED(event))
{
	wxMDIChildFrame* activeChild = GetActiveChild();
	if(activeChild)
	{
		wxCommandEvent zcev;
		if (activeChild->GetId() == BG_EDGE_WINDOW)
			((BgMdiEdgeChild *) activeChild)->OnSaveEdgeMap(zcev);
		else if (activeChild->GetId() == BG_SEGM_WINDOW)
			((BgMdiSegmentChild *) activeChild)->OnSaveSegmentedImage(zcev);
	}
}

void BgMdiFrame::OnNewEdgeWindow(wxCommandEvent& WXUNUSED(event) )
{

   //indicate that another child frame will be created
   gs_nFrames++;

   // Make another frame, containing a edge processing window
      BgMdiEdgeChild *subframe = new BgMdiEdgeChild(g_frame, "Edge Detection Frame",
      wxPoint(-1, -1), wxSize(-1, -1),
      wxDEFAULT_FRAME_STYLE);

   wxString title;
   title.Printf(_T("Edge Detection Frame %d"), gs_nFrames);
   
   subframe->SetTitle(title);
   
   // Give it an icon
#ifdef __WXMSW__
   subframe->SetIcon(wxIcon("chrt_icn"));
#else
   subframe->SetIcon(wxIcon( mondrian_xpm ));
#endif
   
   // Make a menubar
   wxMenu *file_menu = new wxMenu;
   
   file_menu->Append(BG_NEW_EDGE_WINDOW, "New &edge window\tAlt-E", "Create a new edge detection window");
   file_menu->Append(BG_NEW_SEGM_WINDOW, "New &segment window\tAlt-S", "Create a new segmentation window");
   file_menu->Append(BG_LOAD_IMAGE_EDGE, "&Load image\tCtrl-L", "Load image to perform edge detection");
   file_menu->Append(BG_EDGE_SAVE_MAP, "&Save edge map\tCtrl-S");
   file_menu->Append(BG_CHILD_EDGE_QUIT, "&Close edge window\tAlt-C", "Close this window");
   file_menu->Append(BG_QUIT, "E&xit\tAlt-X");
   
   wxMenu *edge_menu = new wxMenu;
   
   edge_menu->Append(BG_EDGE_DETECT, "Edge Detect\tCtrl-R");
   edge_menu->Append(BG_CHANGE_PARAM_EDGE, "Change Parameters...");

   wxMenu *view_menu = new wxMenu;

   subframe->miViewOrig_ = new wxMenuItem(view_menu, BG_EDGE_VIEW_ORIG, "Original", "View original", TRUE);
   subframe->miViewEdge_ = new wxMenuItem(view_menu, BG_EDGE_VIEW_EDGE, "Edge map", "View edge map", TRUE);

   view_menu->Append(subframe->miViewOrig_);
   view_menu->Append(subframe->miViewEdge_);
   
   wxMenu *help_menu = new wxMenu;
   help_menu->Append(BG_ABOUT, "&About");
   help_menu->Append(BG_HELP, "&Help\tCtrl-H");
   
   wxMenuBar *menu_bar = new wxMenuBar;
   
   menu_bar->Append(file_menu, "&File");
   menu_bar->Append(edge_menu, "&Edge Detect");
   menu_bar->Append(view_menu, "&View");
   menu_bar->Append(help_menu, "&Help");

   //disable items on menu bar
   menu_bar->Enable(BG_EDGE_DETECT, false);
   menu_bar->Enable(BG_EDGE_SAVE_MAP, false);
   
   // Associate the menu bar with the frame
   subframe->SetMenuBar(menu_bar);
   
   subframe->CreateStatusBar();
   subframe->SetStatusText(title);
   
   int width, height;
   subframe->GetClientSize(&width, &height);
   
   subframe->Show(TRUE);
   subframe->miViewOrig_->Check(TRUE);
   subframe->miViewEdge_->Check(TRUE);
}

void BgMdiFrame::OnNewSegmWindow(wxCommandEvent& WXUNUSED(event) )
{

   //indicate that another child frame will be created
   gs_nFrames++;

   // Make another frame, containing a edge processing window
      BgMdiSegmentChild *subframe = new BgMdiSegmentChild(g_frame, "Segmentation Frame",
      wxPoint(-1, -1), wxSize(-1, -1),
      wxDEFAULT_FRAME_STYLE);

   wxString title;
   title.Printf(_T("Segmentation Frame %d"), gs_nFrames);
   
   subframe->SetTitle(title);
   
   // Give it an icon
#ifdef __WXMSW__
   subframe->SetIcon(wxIcon("chrt_icn"));
#else
   subframe->SetIcon(wxIcon( mondrian_xpm ));
#endif
   
   // Make a menubar
   wxMenu *file_menu = new wxMenu;
   file_menu->Append(BG_NEW_EDGE_WINDOW, "New &edge window\tAlt-E", "Create a new edge detection window");
   file_menu->Append(BG_NEW_SEGM_WINDOW, "New &segment window\tAlt-S", "Create a new segmentation window");
   file_menu->Append(BG_SEGM_LOAD_IMAGE, "&Load image\tShift-L", "Load image to perform segmentation");
   file_menu->Append(BG_SEGM_SAVE_SEGMENTED, "&Save result\tShift-S");
   file_menu->Append(BG_CHILD_SEGM_QUIT, "&Close window\tShift-C", "Close this window");
   file_menu->Append(BG_QUIT, "E&xit\tAlt-X");
   
   wxMenu *segm_menu = new wxMenu;
   segm_menu->Append(BG_SEGM_SEGMENT, "Segment Image\tShift-R");
   segm_menu->AppendSeparator();
   wxMenu *speedup_menu	= new wxMenu;
   speedup_menu->Append(BG_SEGM_SPEEDUP_NONE, "&None", "", true);
   speedup_menu->Append(BG_SEGM_SPEEDUP_MEDM, "&Medium", "", true);
   speedup_menu->Append(BG_SEGM_SPEEDUP_HIGH, "&High", "", true);
   wxMenuItem *segm_menu_item	= new wxMenuItem(segm_menu, BG_SEGM_SPEEDUP, "Speedup", "", false, speedup_menu);
   segm_menu->Append(segm_menu_item);

   wxMenu *weightmap_menu = new wxMenu;
   weightmap_menu->Append(BG_SEGM_LOAD_MAP,  "Load custom weight &map\tShift-M", "Load map images to perform segmentation");
   weightmap_menu->AppendSeparator();
   weightmap_menu->Append(BG_SEGM_USE_MAP, "&Use custom weight map", "", true);
   
   wxMenu *help_menu = new wxMenu;
   help_menu->Append(BG_ABOUT, "&About");
   help_menu->Append(BG_HELP, "&Help\tCtrl-H");

   wxMenuBar *menu_bar = new wxMenuBar;
   menu_bar->Append(file_menu, "&File");
   menu_bar->Append(segm_menu, "&Algorithm");
   menu_bar->Append(weightmap_menu, "&Weight Map");
   menu_bar->Append(help_menu, "&Help");

   //disable items on menu bar
   menu_bar->Enable(BG_SEGM_SEGMENT, false);
   menu_bar->Enable(BG_SEGM_SAVE_SEGMENTED, false);
   menu_bar->Enable(BG_SEGM_LOAD_MAP, false);
   menu_bar->Enable(BG_SEGM_USE_MAP, false);

   //check speedup to medium
   menu_bar->Check(BG_SEGM_SPEEDUP_MEDM, true);

   // Associate the menu bar with the frame
   subframe->SetMenuBar(menu_bar);
   subframe->CreateStatusBar();
   subframe->SetStatusText(title);
  
   subframe->Show(TRUE);
   subframe->Fit();
}

void BgMdiFrame::GetImageFileInfo(char **pathname, char **filename)
{

// get the file name
#if defined(__WXGTK__) || defined(__WXMOTIF__)
	wxFileDialog filedialog(this,"Choose an image file","","",
		"*",wxOPEN);
#else
	wxFileDialog filedialog(this,"Choose an image file","","",
		"Common image files|*.png;*.bmp;*.gif;*.tif;*.tiff;*.jpg;*.pnm;*.pgm;*.ppm|PNG files (*.png)|*.png|BMP files (*.bmp)|*.bmp|GIF files (*.gif)|*.gif|TIFF files (*.tif)|*.tif|JPEG files (*.jpg)|*.jpg|PNM files (*.pnm)|*.pnm|PGM/PPM files (*.pgm,*.ppm)|*.pgm;*.ppm",
		wxOPEN);
#endif

	//retrieve and check filename
	*filename = (char *) NULL;
	*pathname = (char *) NULL;
   	BgImCanvas *temp = new BgImCanvas(this, this, wxDefaultPosition, wxDefaultSize);
	if(filedialog.ShowModal()==wxID_OK)
	{
		char* temp_str	= (char *) filedialog.GetPath().c_str();
		*pathname	= new char [strlen(temp_str) + 1];
		strcpy(*pathname, temp_str);
		temp_str	= (char *) filedialog.GetFilename().c_str();
		*filename	= new char [strlen(temp_str) + 1];
		strcpy(*filename, temp_str);
		if (temp->SetImage(*pathname) == 0)
		{
			delete [] *pathname;
			delete [] *filename;
			*pathname	= (char *) NULL;
			*filename	= (char *) NULL;
		}
		else
			bgLog("Image %s loaded\n",filedialog.GetPath().c_str());
	}

	//de-allocate memory
	delete temp;

	//done.
	return;
}

//manages toolbar zoom controls
void BgMdiFrame::ZoomControl(wxCommandEvent& event)
{
	//set display
	wxToolBar	*toolbar = GetToolBar();
	switch (event.m_id)
	{
	case BG_CROSS:
		toolbar->ToggleTool(BG_CROSS, true);
		toolbar->ToggleTool(BG_ZOOM_IN, false);
		toolbar->ToggleTool(BG_ZOOM_OUT, false);
		toolbar->ToggleTool(BG_POINTER, false);
		break;
	case BG_ZOOM_IN:
		toolbar->ToggleTool(BG_CROSS, false);
		toolbar->ToggleTool(BG_ZOOM_IN, true);
		toolbar->ToggleTool(BG_ZOOM_OUT, false);
		toolbar->ToggleTool(BG_POINTER, false);
		break;
	case BG_ZOOM_OUT:
		toolbar->ToggleTool(BG_CROSS, false);
		toolbar->ToggleTool(BG_ZOOM_IN, false);
		toolbar->ToggleTool(BG_ZOOM_OUT, true);
		toolbar->ToggleTool(BG_POINTER, false);
		break;
		//BG_POINTER:
	default:
		toolbar->ToggleTool(BG_CROSS, false);
		toolbar->ToggleTool(BG_ZOOM_IN, false);
		toolbar->ToggleTool(BG_ZOOM_OUT, false);
		toolbar->ToggleTool(BG_POINTER, true);
		break;
	}

	//set zoom functionality
	wxMDIChildFrame	*activeChild	= GetActiveChild();
	if(activeChild->GetId()	== BG_EDGE_WINDOW)
	{
		switch (event.m_id)
		{
		case BG_CROSS:
			((BgMdiEdgeChild *) activeChild)->ZoomWindow();
			break;
		case BG_ZOOM_IN:
			((BgMdiEdgeChild *) activeChild)->ZoomIn();
			break;
		case BG_ZOOM_OUT:
			((BgMdiEdgeChild *) activeChild)->ZoomOut();
			break;
			//BG_POINTER:
		default:
			((BgMdiEdgeChild *) activeChild)->NoZoom();
			break;
		}
	}
	else if(activeChild->GetId() == BG_SEGM_WINDOW)
	{
		switch (event.m_id)
		{
		case BG_CROSS:
			((BgMdiSegmentChild *) activeChild)->ZoomWindow();
			break;
		case BG_ZOOM_IN:
			((BgMdiSegmentChild *) activeChild)->ZoomIn();
			break;
		case BG_ZOOM_OUT:
			((BgMdiSegmentChild *) activeChild)->ZoomOut();
			break;
			//BG_POINTER:
		default:
			((BgMdiSegmentChild *) activeChild)->NoZoom();
			break;
		}
	}

	return;
		
}


void BgMdiFrame::OnSize(wxSizeEvent& WXUNUSED(event))
{
   int w, h;
   GetClientSize(&w, &h);
   logtext_->SetSize(0, h-logsize_, w, logsize_);
   GetClientWindow()->SetSize(0, 0, w, h-logsize_);
}


void BgMdiFrame::InitToolBar(wxToolBar* toolBar)
{
   const int	BITMAP_COUNT = 9;
   wxBitmap* bitmaps[BITMAP_COUNT];
   
#ifdef __WXMSW__
   bitmaps[0] = new wxBitmap( "icon1", wxBITMAP_TYPE_RESOURCE);
   bitmaps[1] = new wxBitmap( "icon7", wxBITMAP_TYPE_RESOURCE);
   bitmaps[2] = new wxBitmap( "icon8", wxBITMAP_TYPE_RESOURCE);
   bitmaps[3] = new wxBitmap( "icon2", wxBITMAP_TYPE_RESOURCE);
   bitmaps[4] = new wxBitmap( "icon3", wxBITMAP_TYPE_RESOURCE);
   bitmaps[5] = new wxBitmap( "icon9", wxBITMAP_TYPE_RESOURCE);
   bitmaps[6] = new wxBitmap("icon10", wxBITMAP_TYPE_RESOURCE);
   bitmaps[7] = new wxBitmap("icon11", wxBITMAP_TYPE_RESOURCE);
   bitmaps[8] = new wxBitmap("icon12", wxBITMAP_TYPE_RESOURCE);
#else
   bitmaps[0] = new wxBitmap( bnew_xpm );
   bitmaps[1] = new wxBitmap( bnsg_xpm );
   bitmaps[2] = new wxBitmap( bhelp_xpm );
   bitmaps[3] = new wxBitmap( bopen_xpm );
   bitmaps[4] = new wxBitmap( bsave_xpm );
   bitmaps[5] = new wxBitmap( bcros_xpm );
   bitmaps[6] = new wxBitmap( bzin_xpm );
   bitmaps[7] = new wxBitmap( bzout_xpm );
   bitmaps[8] = new wxBitmap( bpoin_xpm );

#endif
   
#ifdef __WXMSW__
   int width = 24;
#else
   int width = 16;
#endif
   int currentX = 5;
   
   //add tools to tool bar
   toolBar->AddTool( BG_NEW_EDGE_WINDOW, *(bitmaps[0]), wxNullBitmap, FALSE, currentX, -1, (wxObject *) NULL, "New edge window");
   currentX += width + 5;
   toolBar->AddTool( BG_NEW_SEGM_WINDOW, *(bitmaps[1]), wxNullBitmap, FALSE, currentX, -1, (wxObject *) NULL, "New segment window");
   currentX += width + 5;
   toolBar->AddTool( BG_LOAD_IMAGE, *(bitmaps[3]), wxNullBitmap, FALSE, currentX, -1, (wxObject *) NULL, "Load image for processing");
   currentX += width + 5;
   toolBar->AddTool( BG_SAVE_RESULT, *(bitmaps[4]), wxNullBitmap, FALSE, currentX, -1, (wxObject *) NULL, "Save result");
   currentX += width + 5;
   toolBar->AddSeparator();
   toolBar->AddTool( BG_CROSS, *(bitmaps[5]), wxNullBitmap, TRUE, currentX, -1, (wxObject *) NULL, "Zoom Window");
   currentX += width + 5;
   toolBar->AddTool( BG_ZOOM_IN, *(bitmaps[6]), wxNullBitmap, TRUE, currentX, -1, (wxObject *) NULL, "Zoom In");
   currentX += width + 5;
   toolBar->AddTool( BG_ZOOM_OUT, *(bitmaps[7]), wxNullBitmap, TRUE, currentX, -1, (wxObject *) NULL, "Zoom Out");
   currentX += width + 5;
   toolBar->AddTool( BG_POINTER, *(bitmaps[8]), wxNullBitmap, TRUE, currentX, -1, (wxObject *) NULL, "Select");
   currentX += width + 5;
   toolBar->AddSeparator();
   toolBar->AddTool( BG_HELP, *bitmaps[2], wxNullBitmap, FALSE, currentX, -1, (wxObject *) NULL, "Help");
   
   //set bitmap size of controls buttons to 20x20
   wxSize tool_size(20,20);
   toolBar->SetToolBitmapSize(tool_size);

   toolBar->Realize();

   //disable certain tools
   toolBar->EnableTool(BG_SAVE_RESULT, false);
   toolBar->EnableTool(BG_CROSS, false);
   toolBar->EnableTool(BG_ZOOM_IN, false);
   toolBar->EnableTool(BG_ZOOM_OUT, false);
   toolBar->EnableTool(BG_POINTER, false);
   
   int i;
   for (i = 0; i < BITMAP_COUNT; i++)
      delete bitmaps[i];
}

// ---------------------------------------------------------------------------
// BgPointSet
// ---------------------------------------------------------------------------

BgPointSet::BgPointSet()
{
   x_ = y_ = 0;
   n_ = 0;
   type_ = 0;
   pen_.SetColour(*wxBLUE);
   //pen_.SetColour(*wxWHITE);
   pen_.SetWidth(1);
   pen_.SetStyle(wxSOLID);
}

BgPointSet::~BgPointSet()
{
   CleanData();
}

void BgPointSet::CleanData()
{
   if (n_ > 0)
   {
      delete [] x_;
      delete [] y_;
      x_ = y_ = 0;
      n_ = 0;
   }
}

void BgPointSet::SetPoints(int* x, int* y, int n)
{
   CleanData();
   n_ = n;
   x_ = new int[n_];
   y_ = new int[n_];
   for (int i=0; i<n; i++)
   {
      x_[i] = x[i];
      y_[i] = y[i];
   }
}

// ---------------------------------------------------------------------------
// BgCurveSet
// ---------------------------------------------------------------------------

BgCurveSet::BgCurveSet()
{
   x_ = y_ = 0;
   xs_ = ys_ = 0;
   n_ = 0;
   type_ = -1;
   pen_.SetColour(*wxBLUE);
   pen_.SetWidth(3);
   pen_.SetStyle(wxSOLID);
   isDragging_ = 0;
}

BgCurveSet::~BgCurveSet()
{
   CleanData();
}

void BgCurveSet::CleanData()
{
   if (n_ > 0)
   {
      delete [] x_;
      delete [] y_;
      x_ = y_ = 0;
      n_ = 0;
   }
   type_ = -1;
   xs_ = ys_ = 0;
}

void BgCurveSet::SetCurve(BgCurveSet* bgc)
{
   CleanData();
   type_ = bgc->type_;
   n_ = bgc->n_;
   x_ = new int[n_];
   y_ = new int[n_];
   xs_ = bgc->xs_;
   ys_ = bgc->ys_;
   for (int i=0; i<n_; i++)
   {
      x_[i] = bgc->x_[i];
      y_[i] = bgc->y_[i];
   }
}

void BgCurveSet::SetParamCurve(int type, double* x, double* y, int n, int xs, int ys)
{
   CleanData();
   type_ = type;
   n_ = n;
   x_ = new int[n_];
   y_ = new int[n_];
   xs_ = xs;
   ys_ = ys;
   for (int i=0; i<n; i++)
   {
      x_[i] = (int) (x[i]*xs);
      y_[i] = (int) (ys-y[i]*ys);
   }
}

void BgCurveSet::GetParamCurve(double* x, double* y, int& type, int& n)
{
   for (int i=0; i<n_; i++)
   {
      x[i] = ((double)x_[i])/xs_;
      y[i] = ((double)(ys_-y_[i]))/ys_;
   }
   type = type_;
   n = n_;
}

void BgCurveSet::DrawYourself(unsigned char* buf, int val)
{
   int j;
   switch (type_)
   {
   case -1:
      break;
   case FC_ELLIPSE:
      DrawEllipticArc(buf, val, -x_[0], y_[0], 2*x_[0], 2*(ys_-y_[0]), 0, 90);
      break;
   case FC_VERT_LINE:
      DrawLine(buf, val, x_[0], 0, x_[0], ys_);
      break;
   case FC_HORIZ_LINE:
      DrawLine(buf, val, 0, y_[0], xs_, y_[0]);
      break;
   case FC_LINE:
      DrawLine(buf, val, 0, y_[0], x_[0], ys_);
      break;
   case FC_SQUARE_BOX:
      DrawLine(buf, val, 0, y_[0], x_[0], y_[0]);
      DrawLine(buf, val, x_[0], y_[0], x_[0], ys_);
      break;
   case FC_CUSTOM:
      // lines
      for (j=0; j<(n_-1); j++)
         DrawLine(buf, val, x_[j], y_[j], x_[j+1], y_[j+1]);
      // control points
      for (j=0; j<n_; j++)
         DrawPoint(buf, val ,x_[j], y_[j]);
      break;
   }
   
}

void BgCurveSet::DrawPoint(unsigned char* buf, int val, int x, int y)
{
   int r, c;
   int dx, dy;
   for (dx=-2; dx<=2; dx++)
   {
      for (dy=-2; dy<=2; dy++)
      {
         c=x+dx;
         r=y+dy;
         if ((c>=0) && (c<xs_) && (r>=0) && (r<ys_) && ((abs(dx)+abs(dy))<4))
            buf[c+r*ys_] = val;
      }
   }
}

void BgCurveSet::DrawLine(unsigned char* buf, int val, int xs, int ys, int xe, int ye)
{
   int r, c;
   double dsx, dsy, dex, dey;
   if (abs(xs-xe)>abs(ys-ye))
   {
      // x scan
      if (xs > xe)
      {
         dsx = xe;
         dsy = ye;
         dex = xs;
         dey = ys;
      }
      else 
      {
         dex = xe;
         dey = ye;
         dsx = xs;
         dsy = ys;
      }

      for (c = (int) dsx; c<=(int)dex; c++)
      {
         if (c>=0 && c<xs_)
         {
            r = bgRound(dey-(dey-dsy)*(dex-c)/(dex-dsx));
            if (r>=0 && r<ys_)
            {
               buf[c+r*xs_] = val;
               // +/- 1
               if ((r+1)<ys_) buf[c+(r+1)*xs_] = val;
               if ((r-1)>=0) buf[c+(r-1)*xs_] = val;
               // +/- 2
               if ((r+2)<ys_) buf[c+(r+2)*xs_] = val;
               if ((r-2)>=0) buf[c+(r-2)*xs_] = val;
            }
         }
      }
   }
   else
   {
      // y scan
      if (ys > ye)
      {
         dsx = xe;
         dsy = ye;
         dex = xs;
         dey = ys;
      }
      else 
      {
         dex = xe;
         dey = ye;
         dsx = xs;
         dsy = ys;
      }
      
      // check bounds
      
      for (r = (int) dsy; r<=(int) dey; r++)
      {
         if (r>=0 && r<ys_)
         {
            c = bgRound(dex-(dex-dsx)*(dey-r)/(dey-dsy));
            if (c>=0 && c<xs_)
            {
               buf[c+r*xs_] = val;
               // +/- 1
               if ((c+1)<xs_) buf[c+1+r*xs_] = val;
               if ((c-1)>=0) buf[c-1+r*xs_] = val;
               // +/- 2
               if ((c+2)<xs_) buf[c+2+r*xs_] = val;
               if ((c-2)>=0) buf[c-2+r*xs_] = val;
            }
         }
      }
   }
}
void BgCurveSet::DrawEllipticArc(unsigned char* buf, int val, int x, int y, int w, int h, int sa, int ea)
{
   double xc, yc, rx, ry;
   rx = w/2;
   ry = h/2;
   xc = x+rx;
   yc = y+ry;
   int r, c;

//   if (rx > ry)
//   {
      // x scan
      for (c = (int) xc; c<=(int) (xc+rx); c++)
      {
         if (c>=0 && c<xs_)
         {
            r = bgRound(yc-ry*sqrt(1-(c-xc)*(c-xc)/(rx*rx)));
            if (r>=0 && r<ys_)
            {
               buf[c+r*xs_] = val;
               // +/- 1
               if ((r+1)<ys_) buf[c+(r+1)*xs_] = val;
               if ((r-1)>=0) buf[c+(r-1)*xs_] = val;
               // +/- 2
               if ((r+2)<ys_) buf[c+(r+2)*xs_] = val;
               if ((r-2)>=0) buf[c+(r-2)*xs_] = val;
            }
         }
      }
//   }
//   else
//   {
      // y scan
      for (r = (int)(yc-ry); r<=(int) yc; r++)
      {
         if (r>=0 && r<ys_)
         {
            c = bgRound(xc+rx*sqrt(1-(r-yc)*(r-yc)/(ry*ry)));
            if (c>=0 && c<xs_)
            {
               buf[c+r*xs_] = val;
               // +/- 1
               if ((c+1)<xs_) buf[c+1+r*xs_] = val;
               if ((c-1)>=0) buf[c-1+r*xs_] = val;
               // +/- 2
               if ((c+2)<xs_) buf[c+2+r*xs_] = val;
               if ((c-2)>=0) buf[c-2+r*xs_] = val;
            }
         }
      }
//   }
}

void BgCurveSet::StartDragging(int x, int y)
{
   isDragging_ = 1;
   int j;
   switch (type_)
   {
   case -1:
      break;
   case FC_ELLIPSE:
      if (abs(x)<3)
         ltodrag_ = 1;
      else if (abs(y-ys_)<3)
         ltodrag_ = 2;
      else
         ltodrag_ = 0;
      break;
   case FC_VERT_LINE:
      ltodrag_ = 0;
      break;
   case FC_HORIZ_LINE:
      ltodrag_ = 0;
      break;
   case FC_LINE:
      if (abs(x)<3)
         ltodrag_ = 1;
      else if (abs(y-ys_)<3)
         ltodrag_ = 2;
      else
         ltodrag_ = 0;
      break;
   case FC_SQUARE_BOX:
      if (abs((abs(x-x_[0])-abs(y-y_[0])))<3)
      {
         // drag corner
         ltodrag_ = 2;
      }
      else if (abs(x-x_[0])>abs(y-y_[0]))
      {
         // drag horizontal
         ltodrag_ = 0;
      }
      else
      {
         // drag vertical
         ltodrag_ = 1;
      }
      break;
   case FC_CUSTOM:
      // find point to drag
      int mind = abs(x-x_[0]) + abs(y-y_[0]);
      int cmind, mj;
      mj = 0;
      for (j=1; j<n_; j++)
      {
         cmind = abs(x-x_[j]) + abs(y-y_[j]);
         if (cmind < mind)
         {
            mind = cmind;
            mj = j;
         }
      }
      ltodrag_ = mj;
      break;
   }
}

void BgCurveSet::DragTo(int x, int y)
{
   if ((x<0) || (y<0) || (x>=xs_) || (y>=ys_))
      return;
   double k, ry;
   switch (type_)
   {
   case -1:
      break;
   case FC_ELLIPSE:
      // modify ellipse to drag
      if (ltodrag_ == 0)
      {
         k = ((double)x_[0])/(ys_-y_[0]);
         ry = sqrt(((double)x*x)/(k*k)+(y-ys_)*(y-ys_));
         x_[0] = bgRound(k*ry);
         y_[0] = bgRound(ys_-ry);
      }
      else if (ltodrag_ == 1)
      {
         y_[0] = y;
      }
      else
      {
         x_[0] = x;
      }
      break;
   case FC_VERT_LINE:
      x_[0] = x;
      break;
   case FC_HORIZ_LINE:
      y_[0] = y;
      break;
   case FC_LINE:
      // modify line to drag
      if (ltodrag_ == 0)
      {
         k = ((double) (ys_-y_[0])/x_[0]);
         y_[0] = ys_-bgRound((double (ys_-y)+k*x));
         x_[0] = bgRound((double (ys_-y)+k*x)/(k));
      }
      else if (ltodrag_ == 1)
      {
         y_[0] = y;
      }
      else
      {
         x_[0] = x;
      }
      break;
   case FC_SQUARE_BOX:
      if (ltodrag_ == 0)
         y_[0] = y;
      else if (ltodrag_ == 1)
         x_[0] = x;
      else
      {
         x_[0] = x;
         y_[0] = y;
      }
      break;
   case FC_CUSTOM:
      // modify line to drag
      if (ltodrag_ == 0)
      {
         y_[0] = y;
      } else if (ltodrag_ == (n_-1))
      {
         x_[n_-1] = x;
      } else
      {
         x_[ltodrag_] = x;
         y_[ltodrag_] = y;
      }
      break;
   }
}

void BgCurveSet::EndDragging(int x, int y)
{
   isDragging_ = 0;
}

// ---------------------------------------------------------------------------
// BgParameterHistory
// ---------------------------------------------------------------------------

BgParameterHistory::BgParameterHistory( void )
{
	params_		= (void *) NULL;
	listSize_	= 0;
	next_		= (BgParameterHistory *) NULL;
}

BgParameterHistory::BgParameterHistory(void *parameters, int itemCount)
{
	params_		= parameters;
	listSize_	= itemCount;
	next_		= (BgParameterHistory *) NULL;
}


BgParameterHistory::~BgParameterHistory( void )
{
	if(params_) delete [] params_;
}

// ---------------------------------------------------------------------------
// BgParameterHistoryBox
// ---------------------------------------------------------------------------

BgParameterHistoryBox::BgParameterHistoryBox(wxWindow* parent, wxWindowID id, const wxString& value, const wxPoint& pos, const wxSize& size, int n, long style, const wxValidator& validator, const wxString& name)
                     : wxComboBox(parent, id, value, pos, size, 0, (wxString*) NULL, style, validator, name)
{
	//initialize history list
	maxCount_		= n;
	listCount_		= 0;
	historyList_	= (BgParameterHistory *) NULL;

	//initialize combo box
	Append("Current");
	SetSelection(0);
}

BgParameterHistoryBox::~BgParameterHistoryBox( void )
{
	//delete history...
	BgParameterHistory	*temp;
	while(historyList_)
	{
		temp			= historyList_;
		historyList_	= historyList_->next_;
		delete temp;
	}
}

void BgParameterHistoryBox::AddParameterList(void *parameters, int itemCount)
{
	if(listCount_ < maxCount_)
	{
		BgParameterHistory	*newHistory	= new BgParameterHistory(parameters, itemCount);
		newHistory->next_	= historyList_;
		historyList_		= newHistory;
		listCount_++;
		char str[40];
		sprintf(str, "Parameter List -%d", listCount_);
		Append(str);
	}
	else
	{
		BgParameterHistory	*newHistory	= historyList_;
		while(newHistory->next_->next_)	newHistory	= newHistory->next_;
		if(newHistory->next_->params_) delete newHistory->next_->params_;
		newHistory->next_->params_		= parameters;
		newHistory->next_->listSize_	= itemCount;
		newHistory->next_->next_		= historyList_;
		historyList_					= newHistory->next_;
		newHistory->next_				= (BgParameterHistory *) NULL;
	}
}

void *BgParameterHistoryBox::GetParameterListData(int indexNumber)
{
	if(indexNumber < listCount_)
	{
		BgParameterHistory	*currentHistory	= historyList_;
		int count = 0;
		while(indexNumber != count)
		{
			currentHistory	= currentHistory->next_;
			count++;
		}
		return currentHistory->params_;
	}
	return (void *) NULL;
}

int BgParameterHistoryBox::GetParameterListCount(int indexNumber)
{
	if(indexNumber < listCount_)
	{
		BgParameterHistory	*currentHistory	= historyList_;
		int count = 0;
		while(indexNumber != count)
		{
			currentHistory	= currentHistory->next_;
			count++;
		}
		return currentHistory->listSize_;
	}
	return -1;
}

void BgParameterHistoryBox::UseParameterList(int indexNumber)
{
	if((indexNumber < listCount_) && (indexNumber != 0))
	{
		BgParameterHistory	*previousHistory	= historyList_;
		int count = 0;
		while(count != indexNumber - 1)
		{
			previousHistory	= previousHistory->next_;
			count++;
		}
		BgParameterHistory	*currentHistory		= previousHistory->next_;
		previousHistory->next_					= currentHistory->next_;
		currentHistory->next_					= historyList_;
		historyList_							= currentHistory;
	}
	return;
}

void BgParameterHistoryBox::SetCurrentList(void *parameters, int itemCount)
{
	currentList_.params_	= parameters;
	currentList_.listSize_	= itemCount;
}

void *BgParameterHistoryBox::GetCurrentListData( void )
{
	return currentList_.params_;
}

int BgParameterHistoryBox::GetCurrentListCount( void )
{
	return currentList_.listSize_;
}

// ---------------------------------------------------------------------------
// BgText
// ---------------------------------------------------------------------------

//default constructor
BgText::BgText(void)
{
	text_	= (char *)	NULL;
	font_	= (wxFont *)	NULL;
	id_		= 0;
	x_		= 0;
	y_		= 0;
}

//overloaded constructor
BgText::BgText(int id, char *text, wxFont font, int x, int y)
{
	text_		= new char [strlen(text) + 1];
	strcpy(text_, text);
	font_		= new wxFont;
	(*font_)	= font;
	id_			= id;
	x_			= x;
	y_			= y;
}

//destructor
BgText::~BgText(void)
{
	delete text_;
	delete font_;
}

//sets text string
//pre : text is a character string used to set the text
//      field of the text object
//post: the text field of the text object has been set to text
void BgText::SetText(char *text)
{
	if((text_)&&(strlen(text) > strlen(text_)))
	{
		delete [] text_;
		text_	= new char [strlen(text) + 1];
	}
	strcpy(text_, text);
	return;
}

//sets font of text
//pre : font specifies the new font of the text object
//post: the font of the text object has been changed to font
void BgText::SetFont(wxFont font)
{
	if(!font_)	font_	= new wxFont;
	(*font_)	= font;
	return;
}

//sets id of text
//pre : id is the new id of the text object
//post: the id of the text object has been set to id
void BgText::SetId(int id)
{
	id_	= id;
}

//sets plot location of text
//pre : (x,y) determine the new plot location of the text object
//post: the plot location of the text object has been set to (x,y)
void BgText::SetPlotLocation(int x, int y)
{
	x_	= x;
	y_	= y;
}

// ---------------------------------------------------------------------------
// BgTextObj
// ---------------------------------------------------------------------------

//constructor
BgTextObj::BgTextObj(BgText *text)
{
	text_	= new BgText(text->id_, text->text_, *(text->font_), text->x_, text->y_);
	next_	= NULL;
}

//destructor
BgTextObj::~BgTextObj(void)
{
	delete text_;
}

// ---------------------------------------------------------------------------
// BgTextList
// ---------------------------------------------------------------------------

// *** public methods *** //

//constructor
BgTextList::BgTextList(void)
{
	head_		= cur_	= (BgTextObj *) NULL;
	itemcount_	= 0;
}

//destructor
BgTextList::~BgTextList(void)
{
	cur_ = head_;
	while(cur_)
		DeleteText();
}

//adds text object to list
//pre : text is a text object to be added to the list
//post: text has been added to the list
int BgTextList::AddText(BgText *text)
{
	//search for existsing text object
	int id	= text->id_;
	cur_	= head_;
	while((cur_)&&(cur_->text_->id_ != id))
		cur_ = cur_->next_;

	//if it exists change its contents
	if(cur_)
	{
		BgText *tmp_text	= cur_->text_;
		tmp_text->SetText(text->text_);
		tmp_text->SetFont(*(text->font_));
		tmp_text->SetPlotLocation(text->x_, text->y_);
	}
	//otherwise add it to existing list
	else
	{
		BgTextObj	*temp;
		if((temp = new BgTextObj(text)) == NULL)
			return 1;
		
		temp->next_	= head_;
		if(head_ == NULL)
			cur_	= temp;
		head_		= temp;
		itemcount_++;
	}

	return 0;
}		

//removes text object from list
//pre : textId is the id of the text to be removed from the
//      list
//post: text object having id textId has been removed from the
//      list if it exists (1 is returned upon error)
int BgTextList::RemoveText(int textId)
{
	cur_ = head_;
	while((cur_)&&(cur_->text_->id_ != textId))
		cur_	= cur_->next_;

	if(cur_)
	{
		DeleteText();
		return 0;
	}
	else
		return 1;
}

//returns text object from text list
//post: the text object pointed to by cur_ is returned
BgText	*BgTextList::GetText(void)
{
	if(cur_)
	{
		BgText	*text;
		text	= cur_->text_;
		cur_	= cur_->next_;
		return text;
	}
	else
		return (BgText *) NULL;
}

//resets cur_ pointer to head of the list
void BgTextList::ResetList(void)
{
	cur_	= head_;
}

//returns the number of text objects contained
//within the list
//post: count of text objects contained by list is returned
int	BgTextList::GetTextCount(void)
{
	return itemcount_;
}

// *** private methods ***

//deletes a text object node pointed to
//by cur_
//post: text object has been deleted from the text list
void BgTextList::DeleteText(void)
{
	if(cur_ == head_)
	{
		head_	= head_->next_;
		delete cur_;
		cur_		= head_;
	}
	else
	{
		BgTextObj	*temp = cur_;
		cur_	= cur_->next_;
		if(!cur_)	cur_	= head_;
		delete temp;
	}
}

// ---------------------------------------------------------------------------
// BgBitmap
// ---------------------------------------------------------------------------

//default constructor
BgBitmap::BgBitmap(void)
{
	bitmap_		= (wxBitmap *) NULL;
	location_x_	= location_y_	= 0;
	id_			= 0;
}

//overloaded constructor
BgBitmap::BgBitmap(wxBitmap *bitmap, int id, int location_x, int location_y)
{
	bitmap_		= new wxBitmap;
	(*bitmap_)	= (*bitmap);
	id_			= id;
	location_x_	= location_x;
	location_y_	= location_y;
}

//destructor
BgBitmap::~BgBitmap(void)
{
	if(bitmap_)	delete bitmap_;
}

//set bitmap content
//pre : bitmap is the new bitmap content
//post: bitmap content has been changed to that of bitmap
void BgBitmap::SetMap(wxBitmap *bitmap)
{
	if(!bitmap_)	bitmap_	= new wxBitmap;
	(*bitmap_)	= (*bitmap);
}

//set plot location
//pre : (location_x, location_y) define new plot location of bitmap
//post: plot location of bitmap has been set to (location_x, location_y)
void BgBitmap::SetPlotLocation(int location_x, int location_y)
{
	location_x_	= location_x;
	location_y_	= location_y;
}

//set bitmap id
//pre : id is the new bitmap id
//post: the bitmap id has been set to id
void BgBitmap::SetId(int id)
{
	id_	= id;
}

// ---------------------------------------------------------------------------
// BgBitmapObj
// ---------------------------------------------------------------------------

//default constructor
BgBitmapObj::BgBitmapObj(void)
{
	bitmap_	= (BgBitmap *) NULL;
	next_	= (BgBitmapObj *) NULL;
}

//overloaded constructor
BgBitmapObj::BgBitmapObj(BgBitmap* bitmap)
{
	bitmap_	= new BgBitmap(bitmap->bitmap_, bitmap->id_,
						   bitmap->location_x_, bitmap->location_y_);
	next_	= (BgBitmapObj *) NULL;
}

//destructor
BgBitmapObj::~BgBitmapObj(void)
{
	if(bitmap_)	delete bitmap_;
}

// ---------------------------------------------------------------------------
// BgBitmap
// ---------------------------------------------------------------------------

//constructor
BgBitmapList::BgBitmapList(void)
{
	head_		= cur_	= (BgBitmapObj *) NULL;
	itemcount_	= 0;
}

//destuctor
BgBitmapList::~BgBitmapList(void)
{
	cur_ = head_;
	while(cur_)
		DeleteBitmap();
}

//add a bitmap object to the list
//pre : bitmap is a bitmap object to be added to the list
//post: bitmap has been added to the list
int BgBitmapList::AddBitmap(BgBitmap *bitmap)
{
	BgBitmapObj	*temp;
	if((temp = new BgBitmapObj(bitmap)) == NULL)
		return 1;

	temp->next_	= head_;
	if(head_ == NULL)
		cur_	= temp;
	head_		= temp;
	itemcount_++;

	return 0;
}		

//remove bitmap from list
//pre : bitmap is to be removed from list
//post: bitmap has been removed from the list
void BgBitmapList::RemoveBitmap(BgBitmap *bitmap)
{
	int	id	= bitmap->id_;
	cur_	= head_;
	while((cur_)&&(cur_->bitmap_->id_ != id))
		cur_	= cur_->next_;
	DeleteBitmap();
}

//get bitmap from the list pointed to by cur_
//post: bitmap pointed to by cur_ has been returned
//      and cur_ has been incremented to the next list
//      item
BgBitmap *BgBitmapList::GetBitmap(void)
{
	BgBitmap	*temp	= (BgBitmap *)	NULL;
	if(cur_)
	{
		temp	= cur_->bitmap_;
		cur_	= cur_->next_;
	}

	return temp;
}

//reset bitmap list cur_ pointer
//post: cur_ has been set to the head of the list
void BgBitmapList::ResetList(void)
{
	cur_	= head_;
}

//get the number of bitmaps stored by list
//post: bitmap count has been returned
int BgBitmapList::GetBitmapCount(void)
{
	return itemcount_;
}

//delete bitmap object from list
//post: bitmap object pointed to by cur has been deleted
void BgBitmapList::DeleteBitmap(void)
{
	if(cur_)
	{
		BgBitmapObj	*temp	= cur_;
		cur_				= cur_->next_;
		delete temp;
		itemcount_--;
	}
}

// ---------------------------------------------------------------------------
// BgAxis
// ---------------------------------------------------------------------------

//default constructor
BgAxis::BgAxis(void)
{
	start_x_	= 0; start_y_	= 0;
	length_		= 0;
	ticknum_	= 0;
	direction_	= 0;
	start_val_	= 0; stop_val_	= 0;
	label_		= (BgText *) NULL;
	rotation_	= 0;
}

//overloaded constructor
BgAxis::BgAxis(int start_x, int start_y, int length, int ticknum, int direction, float start_val, float stop_val)
{
	start_x_	= start_x;
	start_y_	= start_y;
	length_		= length;
	ticknum_	= ticknum;
	direction_	= direction;
	start_val_	= start_val;
	stop_val_	= stop_val;
	label_		= (BgText *) NULL;
	rotation_	= 0;
}

//destuctor
BgAxis::~BgAxis(void)
{
	if(label_)	delete label_;
}

//set plotting origin
//pre : (start_x, start_y) specify the new axis plotting origin
//post: axis plotting origin has been set to (start_x, start_y)
void BgAxis::SetPlotOrigin(int start_x, int start_y)
{
	start_x_	= start_x;
	start_y_	= start_y;
}

//set axis length
//pre : length specifies the new length of the axis
//post: the axis lengh has been set to length
void BgAxis::SetLength(int length)
{
	length_		= length;
}

//set axis tick number
//pre : ticknum is the new tick number of the axis
//post: the axis tick number has been set to ticknum
void BgAxis::SetTicknum(int ticknum)
{
	ticknum_	= ticknum;
}

//set axis boundaries
//pre : (start_val, stop_val) define the new axis boundaries
//post: the axis boundaries have been set to (start_val, stop_val)
void BgAxis::SetBounds(float start_val, float stop_val)
{
	start_val_	= start_val;
	stop_val_	= stop_val;
}

//add axis label
//pre : label specifies the axis label
//post: the axis has been labeled using label
void BgAxis::Label(BgText *label)
{
	if(label_)	delete label_;
	char *text	= label->text_;
	int label_length	= 3*strlen(text);
	//horizontal axis
	if(direction_ == 0)
	{
		label_x_	= start_x_ + length_/2 - label_length;
		label_y_	= start_y_ + 30;
	}
	//vertical axis
	else
	{
		label_x_	= start_x_ - 60;
		label_y_	= start_y_ - length_/2 + label_length;
	}
	label_	= new BgText(label->id_, text, *(label->font_), label_x_, label_y_);
}

//removes axis label
//post: the axis label has been removed
void BgAxis::RemoveLabel(void)
{
	if(label_)	delete label_;
	label_	= (BgText *) NULL;
}

//sets label rotation
//pre : rotation specifies clockwise label rotation (rotation = 0
//      leaves the label parallel to the axis)
//post: label rotation has been set to rotation
void BgAxis::SetLabelRotation(int rotation)
{
	rotation_	= rotation;
}

//draw axis object
//pre : dc is a dc object used to draw onto a window
//post: the axis has been drawn onto the window using
//      using the dc object
void BgAxis::PlotAxis(wxDC *dc)
{
	//set tick length
	int	ticklength = 8;

	//calculate axis ending locations
	int	end_x	= start_x_ + length_;
	int	end_y	= start_y_ - length_;

	//define shift in x and y direction
	int	shift_x	= 5;
	int	shift_y	= 5;

	//create font
	wxFont plotFont(7, wxSWISS, wxNORMAL, wxNORMAL, false, "", wxFONTENCODING_DEFAULT);

	//plot axis line
	//horizontal axis
	if(direction_ == 0)
	{
		//draw axis line
		dc->DrawLine(start_x_, start_y_, end_x, start_y_);
		dc->DrawLine(start_x_, start_y_+1, end_x, start_y_+1);

		//draw tick marks
		int	increment	= length_/ticknum_;
		if(length_%ticknum_ != 0)	increment++;
		int	x_location	= start_x_;
		while(x_location < end_x)
		{
			dc->DrawLine(x_location, start_y_, x_location, start_y_+ticklength);
			dc->DrawLine(x_location+1, start_y_, x_location+1, start_y_+ticklength);
			x_location	= x_location + increment;
		}
		dc->DrawLine(end_x-1, start_y_, end_x-1, start_y_+ticklength);
		dc->DrawLine(end_x, start_y_, end_x, start_y_+ticklength);

		//draw floating point axis markers
		x_location	= start_x_ - shift_x;
		float	marker_x			= start_val_;
		float	marker_increment	= (stop_val_ - start_val_)/ticknum_;
		char	marker_str[8];
		int i	= 0, fixed_y	= start_y_ + 10;
		char	align[6];
		if(stop_val_ < 10)
			strcpy(align, "%4.2f");
		else
		{
			strcpy(align, "%4.0f");
			x_location	= x_location - shift_x;
			shift_x		= 2*shift_x;
		}
		dc->SetFont(plotFont);
		for(i = 0; i < ticknum_; i++)
		{
			sprintf(marker_str, align, marker_x);
			dc->DrawText(marker_str, x_location, fixed_y);
			marker_x	+= marker_increment;
			x_location	+= increment;
		}
		sprintf(marker_str, align, marker_x);
		dc->DrawText(marker_str, end_x - shift_x, fixed_y);

		//add label
		if(label_)
		{
			dc->SetFont(*(label_->font_));
			dc->DrawRotatedText(label_->text_, label_x_, label_y_, 0+rotation_);
		}
	}
	//vertical axis
	else
	{
		//draw axis line
		dc->DrawLine(start_x_, start_y_, start_x_, end_y);
		dc->DrawLine(start_x_-1, start_y_, start_x_-1, end_y);

		//draw tick marks
		int	increment	= length_/ticknum_;
		if(length_%ticknum_ != 0)	increment++;
		int	y_location	= end_y;
		while(y_location < start_y_)
		{
			dc->DrawLine(start_x_, y_location, start_x_-ticklength, y_location);
			dc->DrawLine(start_x_, y_location+1, start_x_-ticklength, y_location+1);
			y_location	= y_location + increment;
		}
		dc->DrawLine(start_x_, start_y_, start_x_-ticklength, start_y_);
		dc->DrawLine(start_x_, start_y_+1, start_x_-ticklength, start_y_+1);

		//draw floating point axis markers
		y_location	= start_y_;
		float	marker_y			= start_val_;
		float	marker_increment	= (stop_val_ - start_val_)/ticknum_;
		char	marker_str[8];
		int i	= 0, fixed_x = start_x_ - 30;
		char	align[6];
		if(stop_val_ < 10)
			strcpy(align, "%4.2f");
		else
			strcpy(align, "%4.0f");
		dc->SetFont(plotFont);
		sprintf(marker_str, align, marker_y);
		dc->DrawText(marker_str, fixed_x, y_location - shift_y);
		marker_y	+= marker_increment;
		y_location	-= increment;
		for(i = 1; i < ticknum_; i++)
		{
			sprintf(marker_str, align, marker_y);
			dc->DrawText(marker_str, fixed_x, y_location);
			marker_y	+= marker_increment;
			y_location	-= increment;
		}
		sprintf(marker_str, align, marker_y);
		dc->DrawText(marker_str, fixed_x, end_y - shift_y);

		//add label
		if(label_)
		{
			dc->SetFont(*(label_->font_));
			dc->DrawRotatedText(label_->text_, label_x_, label_y_, 90+rotation_);
		}
	}
}

// ---------------------------------------------------------------------------
// BgThread
// ---------------------------------------------------------------------------

BgThread::BgThread(wxThreadKind kind, void foo(void*), void *Object) : wxThread(kind)
{
	function_	= foo;
	Object_		= Object;
}

BgThread::~BgThread( void )
{
	function_	= NULL;
	Object_		= NULL;
}

void *BgThread::Entry( void )
{
	function_(Object_);
	return (void *) NULL;
}

// ---------------------------------------------------------------------------
// BgDialog
// ---------------------------------------------------------------------------

BgDialog::BgDialog(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
		: wxDialog(parent, id, title, pos, size, style, name)
{
	
	okButton_	= new wxButton(this, BG_DIALOG_OK, "OK", wxPoint(size.GetWidth()/2-40, size.GetHeight()-60));
}

BgDialog::~BgDialog( void )
{
	delete okButton_;
}

void BgDialog::AddText(BgText *text)
{
	tlist_.AddText(text);
}

void BgDialog::AddBitmap(BgBitmap *bitmap)
{
	blist_.AddBitmap(bitmap);
}

void BgDialog::RemoveText(int id)
{
	tlist_.RemoveText(id);
}

void BgDialog::RemoveBitmap(BgBitmap *bitmap)
{
	blist_.RemoveBitmap(bitmap);
}

void BgDialog::OnPaint(wxPaintEvent& WXUNUSED(event))
{

	wxPaintDC	dc(this);

	//draw new text to the image from text object list
	BgText	*bgText;
	tlist_.ResetList();
	while(bgText = tlist_.GetText())
	{
		dc.SetFont(*(bgText->font_));
		dc.DrawText(bgText->text_, bgText->x_, bgText->y_);
	}

	//draw bitmaps
	blist_.ResetList();
	BgBitmap	*bitmap;
	while(bitmap = blist_.GetBitmap())
		dc.DrawBitmap(*(bitmap->bitmap_), bitmap->location_x_, bitmap->location_y_, true);

}

void BgDialog::OnExit(wxCommandEvent& WXUNUSED(event))
{
	EndModal(wxID_OK);
}

// ---------------------------------------------------------------------------
// BgHoverBar
// ---------------------------------------------------------------------------

BgHoverBar::BgHoverBar(wxWindow* parent, wxWindowID id, int gradViewId, int confViewId, int weitViewId, int custViewId, int gradSaveId, int confSaveId, int weitSaveId)
		  : wxWindow(parent, id, wxDefaultPosition, wxDefaultSize, wxRAISED_BORDER)
{
	//set the size of the window
	SetSize(120,30);

	//place a button using a bitmap
	menuButton1_	= new wxBitmapButton(this, BG_CANVAS_VIEW_BUTTON, wxBitmap("down_arrow", wxBITMAP_TYPE_RESOURCE), wxPoint(33,2), wxSize(18,20));
	menuButton2_	= new wxBitmapButton(this, BG_CANVAS_SAVE_BUTTON, wxBitmap("down_arrow", wxBITMAP_TYPE_RESOURCE), wxPoint(92,2), wxSize(18,20));
	menuText1_		= new wxStaticText(this, -1, "View", wxPoint(5,5), wxSize(25,20));
	menuText2_		= new wxStaticText(this, -1, "Save", wxPoint(62,5), wxSize(25,20));

	//get view menu identification constants
	gradViewId_	= gradViewId;
	confViewId_	= confViewId;
	weitViewId_	= weitViewId;
	custViewId_	= custViewId;
	gradSaveId_	= gradSaveId;
	confSaveId_	= confSaveId;
	weitSaveId_	= weitSaveId;

	//create view menu
	view_menu = new wxMenu;
	view_menu->Append(gradViewId_, "&Gradient Map", "", true);
	view_menu->Append(confViewId_, "&Confidence Map", "", true);
	view_menu->Append(weitViewId_, "&Weight Map", "", true);
	view_menu->Append(custViewId_, "C&ustom Weight Map", "", true);

	//create save menu
	save_menu = new wxMenu;
	save_menu->Append(gradSaveId_, "&Gradient Map");
	save_menu->Append(confSaveId_, "&Confidence Map");
	save_menu->Append(weitSaveId_, "&Weight Map");

}

BgHoverBar::~BgHoverBar( void )
{
	//de-allocate menus...
	delete view_menu;
	delete save_menu;
}

void BgHoverBar::ShowMenu(wxCommandEvent& event)
{
	int buttonId	= event.GetId();
	if(buttonId	== BG_CANVAS_VIEW_BUTTON)
	{
		menuText1_->SetForegroundColour(wxColour(0,0,100));
		menuText2_->SetForegroundColour(wxColour(0,0,0));
		menuText1_->Refresh();
		menuText2_->Refresh();
		PopupMenu(view_menu, 0, 26);
	}
	else
	{
		menuText1_->SetForegroundColour(wxColour(0,0,0));
		menuText2_->SetForegroundColour(wxColour(0,0,100));
		menuText1_->Refresh();
		menuText2_->Refresh();
		PopupMenu(save_menu, 0, 26);
	}
	Update();
}

void BgHoverBar::CheckViewItem(long viewItemId)
{
	view_menu->Check(gradViewId_, false);
	view_menu->Check(confViewId_, false);
	view_menu->Check(weitViewId_, false);
	view_menu->Check(custViewId_, false);
	view_menu->Check(viewItemId, true);
}

void BgHoverBar::Update( void )
{
	menuText1_->SetForegroundColour(wxColour(0,0,0));
	menuText2_->SetForegroundColour(wxColour(0,0,0));
	menuText1_->Refresh();
	menuText2_->Refresh();
}

// ---------------------------------------------------------------------------
// BgMenuPanel
// ---------------------------------------------------------------------------

BgMenuPanel::BgMenuPanel(wxWindow* parent, wxWindowID id, int gradViewId, int confViewId, int weitViewId, int custViewId, int gradSaveId, int confSaveId, int weitSaveId)
		   : wxPanel(parent, id, wxDefaultPosition, wxDefaultSize)
{
	//keep pointer to scroll window
	scrollWindow_	= (wxWindow *) NULL;

	//set the position and size of the window
	SetSize(0,0,120,30);

	//place a button using a bitmap
	menuButton1_	= new wxBitmapButton(this, BG_CANVAS_VIEW_BUTTON, wxBitmap("down_arrow", wxBITMAP_TYPE_RESOURCE), wxPoint(33,2), wxSize(18,20));
	menuButton2_	= new wxBitmapButton(this, BG_CANVAS_SAVE_BUTTON, wxBitmap("down_arrow", wxBITMAP_TYPE_RESOURCE), wxPoint(92,2), wxSize(18,20));
	menuText1_		= new wxStaticText(this, -1, "View", wxPoint(5,5), wxSize(25,20));
	menuText2_		= new wxStaticText(this, -1, "Save", wxPoint(62,5), wxSize(25,20));

	//get view menu identification constants
	gradViewId_	= gradViewId;
	confViewId_	= confViewId;
	weitViewId_	= weitViewId;
	custViewId_	= custViewId;
	gradSaveId_	= gradSaveId;
	confSaveId_	= confSaveId;
	weitSaveId_	= weitSaveId;

	//create view menu
	view_menu = new wxMenu;
	view_menu->Append(gradViewId_, "&Gradient Map", "", true);
	view_menu->Append(confViewId_, "&Confidence Map", "", true);
	view_menu->Append(weitViewId_, "&Weight Map", "", true);
	view_menu->Append(custViewId_, "C&ustom Weight Map", "", true);

	//create save menu
	save_menu = new wxMenu;
	save_menu->Append(gradSaveId_, "&Gradient Map");
	save_menu->Append(confSaveId_, "&Confidence Map");
	save_menu->Append(weitSaveId_, "&Weight Map");

}

BgMenuPanel::~BgMenuPanel( void )
{
	//de-allocate menus...
	delete view_menu;
	delete save_menu;
}

void BgMenuPanel::ShowMenu(wxCommandEvent& event)
{
	int buttonId	= event.GetId();
	if(buttonId	== BG_CANVAS_VIEW_BUTTON)
	{
		menuText1_->SetForegroundColour(wxColour(0,0,200));
		menuText2_->SetForegroundColour(wxColour(0,0,0));
		menuText1_->Refresh();
		menuText2_->Refresh();
		PopupMenu(view_menu, 0, 26);
	}
	else
	{
		menuText1_->SetForegroundColour(wxColour(0,0,0));
		menuText2_->SetForegroundColour(wxColour(0,0,200));
		menuText1_->Refresh();
		menuText2_->Refresh();
		PopupMenu(save_menu, 0, 26);
	}
	Update();
}

void BgMenuPanel::CheckViewItem(long viewItemId)
{
	view_menu->Check(gradViewId_, false);
	view_menu->Check(confViewId_, false);
	view_menu->Check(weitViewId_, false);
	view_menu->Check(custViewId_, false);
	view_menu->Check(viewItemId, true);
}

void BgMenuPanel::Update( void )
{
	menuText1_->SetForegroundColour(wxColour(0,0,0));
	menuText2_->SetForegroundColour(wxColour(0,0,0));
	menuText1_->Refresh();
	menuText2_->Refresh();
}

void BgMenuPanel::EnableMenu(bool enable)
{
	menuText1_->Enable(enable);
	menuText2_->Enable(enable);
	menuButton1_->Enable(enable);
	menuButton2_->Enable(enable);
}

//adjusts size of scroll window
void BgMenuPanel::OnSize(wxSizeEvent& WXUNUSED(event))
{
	if(scrollWindow_)
	{
		int w, h;
		GetClientSize(&w, &h);
		scrollWindow_->SetSize(0,PLOT_MENU_HEIGHT,w,h-PLOT_MENU_HEIGHT);
	}
}

void BgMenuPanel::SetScrollWindow(wxWindow *scrollwindow)
{
	scrollWindow_	= scrollwindow;
}

// ---------------------------------------------------------------------------
// BgLineSet
// ---------------------------------------------------------------------------

BgLineSet::BgLineSet()
{
   xs_ = ys_ = xe_ = ye_ = 0;
   lineParam_ = 0;
   n_ = 0;
   pen_.SetColour(*wxRED);
   //pen_.SetColour(*wxWHITE);
   pen_.SetWidth(2);
   pen_.SetStyle(wxSOLID);
}

BgLineSet::~BgLineSet()
{
   CleanData();
}

void BgLineSet::CleanData()
{
   if (n_ > 0)
   {
      delete [] xs_;
      delete [] xe_;
      delete [] ys_;
      delete [] ye_;
      delete [] lineParam_;
      xs_ = ys_ = xe_ = ye_ = 0;
      lineParam_ = 0;
      n_ = 0;
   }
}

void BgLineSet::SetLines(int* xs, int* xe, int* ys, int* ye, double* lineParam, int n)
{
   CleanData();
   n_ = n;
   xs_ = new int[n_];
   xe_ = new int[n_];
   ys_ = new int[n_];
   ye_ = new int[n_];
   lineParam_ = new double[n_*3];
   int i;
   for (i=0; i<n; i++)
   {
      xs_[i] = xs[i];
      ys_[i] = ys[i];
      xe_[i] = xe[i];
      ye_[i] = ye[i];
   }
   for (i=0; i<(3*n); i++)
      lineParam_[i] = lineParam[i];
}

void BgLineSet::SetLines(double* startp, double* endp, double* lineParam, int n)
{
   CleanData();
   n_ = n;
   xs_ = new int[n_];
   xe_ = new int[n_];
   ys_ = new int[n_];
   ye_ = new int[n_];
   lineParam_ = new double[n_*3];
   int i;
   for (i=0; i<n; i++)
   {
      xs_[i] = (int) startp[2*i];
      ys_[i] = (int) startp[2*i+1];
      xe_[i] = (int) endp[2*i];
      ye_[i] = (int) endp[2*i+1];
   }
   for (i=0; i<(3*n); i++)
      lineParam_[i] = lineParam[i];
}

// ---------------------------------------------------------------------------
// BgImCanvas
// ---------------------------------------------------------------------------

// Define a constructor for my canvas
BgImCanvas::BgImCanvas(wxWindow *child_frame, wxWindow *parent, const wxPoint& pos, const wxSize& size)
: wxScrolledWindow(parent, -1, pos, size,
                   wxSIMPLE_BORDER|wxVSCROLL|wxHSCROLL)
{
   SetBackgroundColour(wxColour("WHITE"));
   pbitmap=(wxBitmap*) NULL;
   pimage=(wxImage*) NULL;
   hasbitmap=FALSE;
   showbitmap_ = TRUE;
   m_dirty = FALSE;
   nLineSets_ = nPointSets_ = 0;

   //set pointer to child frame (e.g. used to update window title)
   child_frame_	= child_frame;

   // clickable curve stuff
   nCurveSets_ = 0;
   ccx_ = RANK_CONF_IMSIZEX;
   ccy_ = RANK_CONF_IMSIZEY;
   curveClick_ = new unsigned char[ccx_*ccy_];
   isDragging_ = 0;
   FillCurveClick();
   mouseModif_ = 0;
   crossCursor_ = 0;
   showtrack_ = 0;

   //initialize window events
   has_focus		= false;
   leaving			= false;

   //initialize plotting canvas
   x_offset_		= 0;
   y_offset_		= 0;
   textObjectCount_	= 0;
   xAxis			= (BgAxis *) NULL;
   yAxis			= (BgAxis *) NULL;
   clear_display_	= false;

   //initializing zooming parameters
   zoom_in			= false;
   zoom_out			= false;
   zoom_window		= false;
   zoom_level		= 1;

   //initialize empty zoom box and refresh box
   zoombox			= (wxImage  *) NULL;
   refresh_box		= (wxImage  *) NULL;

   //initialize point map
   point_map		= NULL;
   point_colour		= (wxColour *) NULL;

   //initialize zoomed image buffer
   zImg				= NULL;

   //initialize zoom window corner
   cx				= 0;
   cy				= 0;

   //initialize mouse pointer location
   m_x_	= 0;
   m_y_	= 0;

   //initialize zoom buffers
   buf	= (unsigned char *) NULL;

   //initialize hover window such that
   //it does not popup
   popup	= 0;

   //initialize hover window
   menuWindow	= (wxWindow *) NULL;

   //pay attention to updates
   noUpdate_	= false;

   //construct zoom cursors
   //??????????????????????????????
   wxString str;
   str.Printf(wxT("icon10"));
   bgCURSOR_MAGNIFIER_PLUS	= new wxCursor(str, wxBITMAP_TYPE_CUR_RESOURCE);
   //bgCURSOR_MAGNIFIER_PLUS	= new wxCursor("icon10", wxBITMAP_TYPE_RESOURCE, 0, 0);
   str.Printf(wxT("icon11"));
   bgCURSOR_MAGNIFIER_MINUS	= new wxCursor(str, wxBITMAP_TYPE_CUR_RESOURCE);
   //bgCURSOR_MAGNIFIER_MINUS	= new wxCursor("icon11", wxBITMAP_TYPE_RESOURCE, 0, 0);

   //build local menu
   localMenu_ = new wxMenu();
   localMenu_->Append(BG_IMC_ADDNODE,"Add node");
   localMenu_->Append(BG_IMC_DELETENODE,"Delete node");
   localMenu_->AppendSeparator();
   localMenu_->Append(BG_IMC_SELTYPE_ELLIPSE,"Arc");
   localMenu_->Append(BG_IMC_SELTYPE_VLINE,"Vertical line");
   localMenu_->Append(BG_IMC_SELTYPE_HLINE,"Horizontal line");
   localMenu_->Append(BG_IMC_SELTYPE_LINE,"Line");
   localMenu_->Append(BG_IMC_SELTYPE_BOX,"Box");
   localMenu_->Append(BG_IMC_SELTYPE_CUSTOM,"Custom");
}

BgImCanvas::~BgImCanvas()
{
   ClearData();
   if(xAxis)	delete xAxis;
   if(yAxis)	delete yAxis;
   delete [] curveClick_;
   delete localMenu_;
   delete bgCURSOR_MAGNIFIER_PLUS;
   delete bgCURSOR_MAGNIFIER_MINUS;
}

void BgImCanvas::OnCustomAddNode(wxCommandEvent& WXUNUSED(event))
{
   if ((lmEventCurve_ < 0) || (lmEventCurve_ >= nCurveSets_) ||
      (curveSet_[lmEventCurve_]->type_!=FC_CUSTOM))
   {
      return;
   }
   int n, type;
   n = curveSet_[lmEventCurve_]->n_;
   double *tx, *ty;
   tx = new double[n+1];
   ty = new double[n+1];
   curveSet_[lmEventCurve_]->GetParamCurve(tx, ty, type, n);
   double x, y;
   x = ((double) lmEventX_)/ccx_;
   y = (ccy_ - ((double) lmEventY_))/ccy_;

   // determine closest line
   double cx, cy, cr, ax, ay, dx, dy;
   double mny = 10;
   int ci = -1;
   int i;
   for (i=0; i<(n-1); i++)
   {
      cx = tx[i+1]-tx[i];
      cy = ty[i+1]-ty[i];
      cr = sqrt(cx*cx+cy*cy);
      if (cr <= 0)
         continue;
      ax = x-tx[i];
      ay = y-ty[i];
      dx = (cx*ax+cy*ay)/cr;
      dy = fabs((-cy*ax+cx*ay)/cr);
      if ((dx>=0) && (dx<=cr))
      {
         if (dy<mny)
         {
            mny = dy;
            ci = i;
         }
      }
   }
   if (ci >= 0)
   {
      // modify curve
      for (i=n; i>ci; i--)
      {
         tx[i] = tx[i-1];
         ty[i] = ty[i-1];
      }
      tx[ci+1] = x;
      ty[ci+1] = y;
      curveSet_[lmEventCurve_]->SetParamCurve(type, tx, ty, n+1, ccx_, ccy_);
   }
      
   delete [] ty;
   delete [] tx;
   mouseModif_ = 1;
   AddPendingEvent(wxCommandEvent(BG_EVENT_UPDATE, BG_EVENT_UPDATE_ID));

   FillCurveClick();
   return;
}
void BgImCanvas::OnCustomDeleteNode(wxCommandEvent& WXUNUSED(event))
{
   if ((lmEventCurve_ < 0) || (lmEventCurve_ >= nCurveSets_) || 
      (lmEventNode_ < 0) || (lmEventNode_ >= (curveSet_[lmEventCurve_]->n_-1)))
   {
      return;
   }
   if (lmEventNode_ == 0)
   {
      bgLog("Cannot delete first node.\n");
      return;
   }
   if (lmEventNode_ == (curveSet_[lmEventCurve_]->n_-1))
   {
      bgLog("Cannot delete last node.\n");
      return;
   }
   // delete the node
   int i;
   int n = curveSet_[lmEventCurve_]->n_;
   int *tx, *ty;
   tx = curveSet_[lmEventCurve_]->x_;
   ty = curveSet_[lmEventCurve_]->y_;
   for (i=lmEventNode_; i<(n-1); i++)
   {
      tx[i] = tx[i+1];
      ty[i] = ty[i+1];
   }
   curveSet_[lmEventCurve_]->n_ = n-1;
   mouseModif_ = 1;
   AddPendingEvent(wxCommandEvent(BG_EVENT_UPDATE, BG_EVENT_UPDATE_ID));
   FillCurveClick();
   return;
}
void BgImCanvas::OnCTypeEllipse(wxCommandEvent& WXUNUSED(event))
{
   if (lmEventCurve_ < nCurveSets_)
   {
      if (curveSet_[lmEventCurve_]->type_ == FC_CUSTOM)
      {
         curveSet_[lmEventCurve_]->x_[0] = curveSet_[lmEventCurve_]->x_[curveSet_[lmEventCurve_]->n_-1];
         curveSet_[lmEventCurve_]->n_ = 1;
      }
      curveSet_[lmEventCurve_]->type_ = FC_ELLIPSE;
      FillCurveClick();
      mouseModif_ = 1;
      AddPendingEvent(wxCommandEvent(BG_EVENT_UPDATE, BG_EVENT_UPDATE_ID));
   }
}
void BgImCanvas::OnCTypeVLine(wxCommandEvent& WXUNUSED(event))
{
   if (lmEventCurve_ < nCurveSets_)
   {
      if (curveSet_[lmEventCurve_]->type_ == FC_CUSTOM)
      {
         curveSet_[lmEventCurve_]->x_[0] = curveSet_[lmEventCurve_]->x_[curveSet_[lmEventCurve_]->n_-1];
         curveSet_[lmEventCurve_]->n_ = 1;
      }
      curveSet_[lmEventCurve_]->type_ = FC_VERT_LINE;
      FillCurveClick();
      mouseModif_ = 1;
      AddPendingEvent(wxCommandEvent(BG_EVENT_UPDATE, BG_EVENT_UPDATE_ID));
   }
}
void BgImCanvas::OnCTypeHLine(wxCommandEvent& WXUNUSED(event))
{
   if (lmEventCurve_ < nCurveSets_)
   {
      if (curveSet_[lmEventCurve_]->type_ == FC_CUSTOM)
      {
         curveSet_[lmEventCurve_]->x_[0] = curveSet_[lmEventCurve_]->x_[curveSet_[lmEventCurve_]->n_-1];
         curveSet_[lmEventCurve_]->n_ = 1;
      }
      curveSet_[lmEventCurve_]->type_ = FC_HORIZ_LINE;
      mouseModif_ = 1;
      AddPendingEvent(wxCommandEvent(BG_EVENT_UPDATE, BG_EVENT_UPDATE_ID));
      FillCurveClick();
   }
}
void BgImCanvas::OnCTypeLine(wxCommandEvent& WXUNUSED(event))
{
   if (lmEventCurve_ < nCurveSets_)
   {
      if (curveSet_[lmEventCurve_]->type_ == FC_CUSTOM)
      {
         curveSet_[lmEventCurve_]->x_[0] = curveSet_[lmEventCurve_]->x_[curveSet_[lmEventCurve_]->n_-1];
         curveSet_[lmEventCurve_]->n_ = 1;
      }
      curveSet_[lmEventCurve_]->type_ = FC_LINE;
      mouseModif_ = 1;
      AddPendingEvent(wxCommandEvent(BG_EVENT_UPDATE, BG_EVENT_UPDATE_ID));
      FillCurveClick();
   }
}
void BgImCanvas::OnCTypeBox(wxCommandEvent& WXUNUSED(event))
{
   if (lmEventCurve_ < nCurveSets_)
   {
      if (curveSet_[lmEventCurve_]->type_ == FC_CUSTOM)
      {
         curveSet_[lmEventCurve_]->x_[0] = curveSet_[lmEventCurve_]->x_[curveSet_[lmEventCurve_]->n_-1];
         curveSet_[lmEventCurve_]->n_ = 1;
      }
      curveSet_[lmEventCurve_]->type_ = FC_SQUARE_BOX;
      mouseModif_ = 1;
      AddPendingEvent(wxCommandEvent(BG_EVENT_UPDATE, BG_EVENT_UPDATE_ID));
      FillCurveClick();
   }
}
void BgImCanvas::OnCTypeCustom(wxCommandEvent& WXUNUSED(event))
{
   if (lmEventCurve_ < nCurveSets_)
   {
      if (curveSet_[lmEventCurve_]->type_ != FC_CUSTOM)
      {
         double x[3];
         double y[3];
         int n,type;
         curveSet_[lmEventCurve_]->GetParamCurve(x,y,type,n);
         n = 3;
         type = FC_CUSTOM;
         x[2] = x[0];
         x[0] = 0;
         y[2] = 0;
         x[2] = (x[2]>1) ? 1:x[2];
         x[2] = (x[2]<0) ? 0:x[2];
         y[0] = (y[0]>1) ? 1:y[0];
         y[0] = (y[0]<0) ? 0:y[0];
         x[1] = (x[0]+x[2])*2.0/3.0;
         y[1] = (y[0]+y[2])*2.0/3.0;
         curveSet_[lmEventCurve_]->SetParamCurve(type, x, y, n, ccx_, ccy_);
      }
      curveSet_[lmEventCurve_]->type_ = FC_CUSTOM;
      mouseModif_ = 1;
      AddPendingEvent(wxCommandEvent(BG_EVENT_UPDATE, BG_EVENT_UPDATE_ID));
      FillCurveClick();
   }
}

void BgImCanvas::AddPointSet(BgPointSet* ps)
{
   pointSet_[nPointSets_++] = ps;

   //update point map if the point set consists
   //of data points (not circles)
   if((ps->type_	== 1)&&(hasbitmap))
   {
	   int nt, *tx, *ty;
	   nt = ps->n_;
	   tx = ps->x_;
	   ty = ps->y_;
	   int i, dp, width = pimage->GetWidth();
	   for(i=0; i<nt; i++)
	   {
		   dp				= ty[i]*width+tx[i];
		   point_map[dp]	= true;
	   }
   }

   //define new pen colour is yet defined, define it
   if(point_colour)	delete point_colour;
   point_colour	= new wxColour(ps->pen_.GetColour());

   //add point data to zoomed image (if it has been defined,
   //i.e. an image has been loaded into the image canvas)
   if(zImg)	AddPoints(zImg, ((pimage->GetWidth())*zoom_level));

   Refresh();
}

void BgImCanvas::RemovePointSet(BgPointSet* ps)
{
   int i = 0;
   while (i<nPointSets_ && pointSet_[i]!=ps)
      i++;
   if (i<nPointSets_)
   {
      i++;
      while(i<nPointSets_)
      {
         pointSet_[i-1] = pointSet_[i];
         i++;
      }
      nPointSets_--;
   }

   //adjust point map accordingly
   if((point_map)&&(ps->type_	== 1))	//point
   {
	   int width = pimage->GetWidth();
	   int nt, *tx, *ty;
	   nt	= ps->n_;
	   tx	= ps->x_;
	   ty	= ps->y_;
	   for(i=0; i<nt; i++)
		   point_map[ty[i]*width+tx[i]] = false;
   }

   //delete pen colour if no more point sets exist
   if(nPointSets_ == 0)
   {
	   delete point_colour;
	   point_colour	= (wxColour *) NULL;
   }

   //remove this point set from the zoomed image...

   /*********************************************************/

   if(zImg)
   {
	   delete [] zImg;
	   int width, height;
	   width	= (pimage->GetWidth())*zoom_level;
	   height	= (pimage->GetHeight())*zoom_level;
	   zImg		= new unsigned char [3*width*height];
	   if(zoom_level > 1)
		   bgZoomIn(&zImg, pimage->GetData(), pimage->GetWidth(), pimage->GetHeight(), zoom_level, false);
	   else if(zoom_level == 1)
		   memcpy(zImg, pimage->GetData(), 3*width*height*sizeof(unsigned char));

	   //add any other point sets that may exist
	   AddPoints(zImg, width);
   }

   /*********************************************************/

   Refresh(false);

}

void BgImCanvas::AddLineSet(BgLineSet* ls)
{
   lineSet_[nLineSets_++] = ls;
   Refresh();
}

void BgImCanvas::AddTrackSet(int x, int y, int hx, int hy)
{
   trackval_[0] = x;
   trackval_[1] = y;
   hx_ = hx;
   hy_ = hy;
   showtrack_ = 1;
   Refresh();

}

void BgImCanvas::RemoveTrackSet()
{
   showtrack_ = 0;
   Refresh();
}
   

void BgImCanvas::RemoveLineSet(BgLineSet* ls)
{
   int i = 0;
   while (i<nLineSets_ && lineSet_[i]!=ls)
      i++;
   if (i<nLineSets_)
   {
      i++;
      while(i<nLineSets_)
      {
         lineSet_[i-1] = lineSet_[i];
         i++;
      }
      nLineSets_--;
   }
   Refresh();
}

void BgImCanvas::AddCurveSet(BgCurveSet* cs)
{
   curveSet_[nCurveSets_++] = cs;
   Refresh();
}

void BgImCanvas::RemoveCurveSet(BgCurveSet* cs)
{
   int i = 0;
   while (i<nCurveSets_ && curveSet_[i]!=cs)
      i++;
   if (i<nCurveSets_)
   {
      i++;
      while(i<nCurveSets_)
      {
         curveSet_[i-1] = curveSet_[i];
         i++;
      }
      nCurveSets_--;
   }
   Refresh();
}

//adds text object to be dran to the image
//pre : bgText is the text object to be plotted
//post: bgText has been added to the image
void BgImCanvas::AddText(BgText *bgText)
{
	//add text to text list
	if(tlist_.AddText(bgText))
	{
		bgLog("BgImCanvas::AddText Error: Out of memory.\n");
		return;
	}

	//obtain new text object count
	textObjectCount_	= tlist_.GetTextCount();
}

//clears all text object from text list
void BgImCanvas::RemoveText(int text_id)
{
	tlist_.RemoveText(text_id);
	return;
}

//adds horizontal axis
void BgImCanvas::AddHorizontalAxis(int start_x, int start_y, int length, int ticknum, float start_val, float stop_val)
{
	if(xAxis)	delete xAxis;
	xAxis	= new BgAxis(start_x, start_y, length, ticknum, 0, start_val, stop_val);
}

//adds horizontal axis
void BgImCanvas::AddVerticalAxis(int start_x, int start_y, int length, int ticknum, float start_val, float stop_val)
{
	if(yAxis)	delete yAxis;
	yAxis	= new BgAxis(start_x, start_y, length, ticknum, 1, start_val, stop_val);
}

//clears axis
void BgImCanvas::ClearAxis( void )
{
	if(xAxis)	delete xAxis;
	if(yAxis)	delete yAxis;
	xAxis	= yAxis	= (BgAxis *) NULL;
}

//labels horizontal axis
void BgImCanvas::LabelHorizontalAxis(BgText *bgText)
{
	if(xAxis)	xAxis->Label(bgText);
}

//labels vertical axis
void BgImCanvas::LabelVerticalAxis(BgText *bgText)
{
	if(yAxis)	yAxis->Label(bgText);
}

//removes label on horizontal axis
void BgImCanvas::RemoveHorizontalAxisLabel(void)
{
	if(xAxis)	xAxis->RemoveLabel();
}

//removes lable on vertical axis
void BgImCanvas::RemoveVerticalAxisLabel(void)
{
	if(yAxis)	yAxis->RemoveLabel();
}

//rotates horizontal axis label clockwise from default position
void BgImCanvas::RotateHorizontalAxisLabel(int rotation)
{
	if(xAxis)	xAxis->SetLabelRotation(rotation);
}

//rotates vertical axis label clockwise from default position
void BgImCanvas::RotateVerticalAxisLabel(int rotation)
{
	if(yAxis)	yAxis->SetLabelRotation(rotation);
}

//plot bitmap at specified location
void BgImCanvas::AddBitmap(BgBitmap *bitmap)
{
	blist_.AddBitmap(bitmap);
}

//remove bitmap from plot
void BgImCanvas::RemoveBitmap(BgBitmap *bitmap)
{
	blist_.RemoveBitmap(bitmap);
}

//clears display
void BgImCanvas::ClearDisplay(void)
{
	clear_display_	= true;
	Refresh();
}

void BgImCanvas::ClearData(int refresh)
{
   if(hasbitmap==TRUE) {
      delete pbitmap;
      delete pimage;
   }
   if(zoombox)			delete zoombox;
   if(refresh_box)		delete refresh_box;
   if(zImg)				delete zImg;
   if(point_map)		delete [] point_map;
   if(buf)				delete [] buf;
   hasbitmap=FALSE;
   pbitmap=(wxBitmap*) NULL;
   pimage=(wxImage*) NULL;
   zoombox=(wxImage*) NULL;
   refresh_box=(wxImage*) NULL;
   zImg=NULL;
   point_map=NULL;
   showtrack_ = 0;
   if (refresh > 0)
      Refresh();
}

int BgImCanvas::SetImage(wxString imname)
{
   ClearData();
   pimage=new wxImage();
   if (!pimage->LoadFile(imname))
   {
      delete pimage;
      wxLogError("Can't load image "+imname);
      return 0;
   }
   else
   {
      SetScrollbars(1, 1, (pimage->GetWidth())/1, (pimage->GetHeight())/1);
#if wxCHECK_VERSION(2, 3, 0)
      pbitmap = new wxBitmap(pimage);
#else
      pbitmap = new wxBitmap(pimage->ConvertToBitmap());
#endif
      hasbitmap=TRUE;
      m_dirty=TRUE;

	  //create point map
	  CreatePointMap(pimage->GetWidth(), pimage->GetHeight());

	  //take account for zoom (takes care of refresh)
	  Zoom(zoom_level);
   }
   return 1;
}

void BgImCanvas::SetImage(wxImage& image)
{
   ClearData();
   pimage=new wxImage();
   *pimage = image;
   SetScrollbars(1, 1, (pimage->GetWidth())/1, (pimage->GetHeight())/1);
   
#if wxCHECK_VERSION(2, 3, 0)
      pbitmap = new wxBitmap(pimage);
#else
      pbitmap = new wxBitmap(pimage->ConvertToBitmap());
#endif
   hasbitmap=TRUE;
   m_dirty=TRUE;

   //create point map
   CreatePointMap(image.GetWidth(), image.GetHeight());

   //take account for zoom (takes care of refresh)
   Zoom(zoom_level);
   
}

void BgImCanvas::SetSameImage(wxImage& image)
{
   *pimage = image;
#if wxCHECK_VERSION(2, 3, 0)
   *pbitmap = wxBitmap(image);
#else
   *pbitmap = image.ConvertToBitmap();
#endif

   //create point map
   CreatePointMap(image.GetWidth(), image.GetHeight());

   //take account for zoom (takes care of refresh)
   Zoom(zoom_level);
}

void BgImCanvas::SetImageFromGray(unsigned char* data, int w, int h)
{
   ClearData();
   
   pimage = new wxImage(w, h);

   int i;
   unsigned char* itTData;
   itTData = pimage->GetData();

   for (i=0; i<w*h; i++)
   {
      *(itTData++)=data[i];
      *(itTData++)=data[i];
      *(itTData++)=data[i];
   }
   SetScrollbars(1, 1, (pimage->GetWidth())/1, (pimage->GetHeight())/1);

#if wxCHECK_VERSION(2, 3, 0)
   pbitmap = new wxBitmap(pimage);
#else
   pbitmap = new wxBitmap(pimage->ConvertToBitmap());
#endif
   hasbitmap=TRUE;
   m_dirty=TRUE;

   //create point map
   CreatePointMap(w, h);

   //take account for zoom (takes care of refresh)
   Zoom(zoom_level);
}

void BgImCanvas::SetImage(unsigned char* data, int w, int h, bool colorim)
{
   ClearData();
   
   pimage = new wxImage(w, h);

   int i;
   unsigned char* itTData;
   itTData = pimage->GetData();

   if (colorim == false)
   {
      for (i=0; i<w*h; i++)
      {
         *(itTData++)=data[i];
         *(itTData++)=data[i];
         *(itTData++)=data[i];
      }
   } else
   {
      for (i=0; i<(3*w*h); i++)
      {
         *(itTData++)=data[i];
      }
   }
   SetScrollbars(1, 1, (pimage->GetWidth())/1, (pimage->GetHeight())/1);

#if wxCHECK_VERSION(2, 3, 0)
   pbitmap = new wxBitmap(pimage);
#else
   pbitmap = new wxBitmap(pimage->ConvertToBitmap());
#endif
   hasbitmap=TRUE;
   m_dirty=TRUE;


   //create point map
   CreatePointMap(w, h);

   //take account for zoom (takes care of refresh)
   Zoom(zoom_level);
}

/*
Still needs work!!!!!!
*/

void BgImCanvas::ShowBitmap(bool showbitmap)
{
	showbitmap_	= showbitmap;
	if(hasbitmap)
	{
		//compute height and width of original image
		int width	= pimage->GetWidth();
		int height	= pimage->GetHeight();

		//paint zoom image
		if(!showbitmap_)
			memset(zImg, 255, 3*zoom_level*zoom_level*width*height*sizeof(unsigned char));
		else
			bgZoomIn(&zImg, pimage->GetData(), width, height, zoom_level, false);

		//plot points from point set onto zoom image
		AddPoints(zImg, width*zoom_level);

		//re-draw image onto canvas
		Refresh();
	}
	return;
}


// Define the repainting behaviour
void BgImCanvas::OnDraw(wxDC& dc)
{
	//clear the display
	if(clear_display_)
	{
		clear_display_	= false;
		dc.Clear();
		return;
	}

	//do not update the display (good for keeping
	//a display clear in light of onSize() events
	//for example)
	if(noUpdate_) return;

	//catch leaving event
	if((zoom_window)&&(leaving))
	{
		//if a refresh window remains, plot it and delete it
		if(refresh_box)
		{
#if wxCHECK_VERSION(2, 3, 0)
         dc.DrawBitmap(wxBitmap(refresh_box), cx, cy);
#else
			dc.DrawBitmap(refresh_box->ConvertToBitmap(), cx, cy);
#endif
			delete refresh_box;
			refresh_box	= (wxImage*) NULL;
		}
		
		//retrieve leaving event
		leaving	= false;

		//exit
		return;
	}


	if (hasbitmap==false)
	{
		dc.Clear();
		return;
	}
	//re-plot image bitmap
	if(hasbitmap==true && showbitmap_==true)
	{
		if((!zoom_window)||(!has_focus))
			dc.DrawBitmap(*pbitmap,0+x_offset_,0+y_offset_);
	}
	else if(!zoom_window)
	{
		dc.Clear();
	}
	if (showtrack_ == 1)
	{
		wxPen tPen=dc.GetPen();
		dc.SetBrush(*wxTRANSPARENT_BRUSH);
		dc.SetPen(*wxRED_PEN);
		//+x_offset_+y_offset_
		dc.DrawEllipse(trackval_[0]-2, trackval_[1]-2, 5,5);
		dc.DrawEllipse(trackval_[0]-hx_, trackval_[1]-hy_, 2*hx_+1, 2*hy_+1);
		dc.SetPen(tPen);
		dc.SetBrush(wxNullBrush);
	}
	if ((nPointSets_ > 0)&&((!zoom_window)||(!has_focus)))
	{
		int i,j;
		int* tx;
		int* ty;
		int x, xc;
		int y, yc;
		int nt;
		wxPen tPen=dc.GetPen();
		dc.SetBrush(*wxTRANSPARENT_BRUSH);
		for (i=0; i<nPointSets_; i++)
		{
			dc.SetPen(pointSet_[i]->pen_);
			nt = pointSet_[i]->n_;
			tx = pointSet_[i]->x_;
			ty = pointSet_[i]->y_;
			if(pointSet_[i]->type_ == 0) // circle
			{
				for (j=0; j<nt; j++)
				{
					dc.DrawEllipse(tx[j]-2+x_offset_, ty[j]-2+y_offset_, 5,5);
				}
			}
			else if(pointSet_[i]->type_ == 1) // point
			{
				for (j=0; j<nt; j++)
				{
					xc	= zoom_level*(tx[j] + x_offset_);
					yc	= zoom_level*(ty[j] + y_offset_);
					for(y=0; y<zoom_level; y++)
					{
						for(x=0; x<zoom_level; x++)
						{
							dc.DrawPoint(xc+x, yc+y);
						}
					}
				}
			}
		}
		dc.SetPen(tPen);
		dc.SetBrush(wxNullBrush);
	}
	
	if (nLineSets_ > 0)
	{
		int i,j;
		int* tsx;
		int* tsy;
		int* tex;
		int* tey;
		int nt;
		wxPen tPen=dc.GetPen();
		dc.SetBrush(*wxTRANSPARENT_BRUSH);
		for (i=0; i<nLineSets_; i++)
		{
			dc.SetPen(lineSet_[i]->pen_);
			nt = lineSet_[i]->n_;
			tsx = lineSet_[i]->xs_;
			tsy = lineSet_[i]->ys_;
			tex = lineSet_[i]->xe_;
			tey = lineSet_[i]->ye_;
			for (j=0; j<nt; j++)
			{
				dc.DrawLine(tsx[j]+x_offset_, tsy[j]+y_offset_, tex[j]+x_offset_, tey[j]+y_offset_);
			}
		}
		dc.SetPen(tPen);
		dc.SetBrush(wxNullBrush);
	}
	int txs,tys,txe,tye;
	
	if (nCurveSets_ > 0)
	{
		int i,j;
		int* tx;
		int* ty;
		int xs, ys;
		int nt;
		wxPen tPen=dc.GetPen();
		dc.SetBrush(*wxTRANSPARENT_BRUSH);
		for (i=0; i<nCurveSets_; i++)
		{
			dc.SetPen(curveSet_[i]->pen_);
			tx = curveSet_[i]->x_;
			ty = curveSet_[i]->y_;
			nt = curveSet_[i]->n_;
			xs = curveSet_[i]->xs_;
			ys = curveSet_[i]->ys_;
			
			switch (curveSet_[i]->type_)
			{
			case -1:
				break;
			case FC_ELLIPSE:
				MyDrawEllipticArc(dc,-tx[0]+x_offset_, ty[0]+y_offset_, 2*tx[0], 2*(ys-ty[0]), 0, 90);
				break;
			case FC_VERT_LINE:
				//            if ((tx[0]>=0) && (tx[0]<xs))
				dc.DrawLine(tx[0]+x_offset_, 0+y_offset_, tx[0]+x_offset_, ys+y_offset_);
				break;
			case FC_HORIZ_LINE:
				if ((ty[0]>=0) && (ty[0]<ys))
					dc.DrawLine(0+x_offset_, ty[0]+y_offset_, xs+x_offset_, ty[0]+y_offset_);
				break;
			case FC_LINE:
				// determine clipping
				tye = ys;
				txe = tx[0];
				txe = (txe<=0)?1:txe;
				txs = 0;
				tys = ty[0];
				tys = (tys>ys)?ys:tys;
				if (txe>xs)
				{
					tye = ccy_-((ccy_-tys)*(txe-xs))/txe;
					txe = xs;
				}
				if (tys<0)
				{
					txs= (txe*(-tys))/(tye-tys);
					tys= 0;
				}
				
				dc.DrawLine(0+x_offset_, ty[0]+y_offset_, tx[0]+x_offset_, ys+y_offset_);
				break;
			case FC_SQUARE_BOX:
				txs = (tx[0]<0)?0:tx[0];
				txs = (txs>=xs)?xs-1:txs;
				tys = (ty[0]<0)?0:ty[0];
				tys = (tys>=ys)?ys-1:tys;
				dc.DrawLine(0+x_offset_, tys+y_offset_, txs+x_offset_, tys+y_offset_);
				dc.DrawLine(txs+x_offset_, tys+y_offset_, txs+x_offset_, ys+y_offset_);
				break;
			case FC_CUSTOM:
				for (j=0; j<(nt-1); j++)
					dc.DrawLine(tx[j]+x_offset_, ty[j]+y_offset_, tx[j+1]+x_offset_, ty[j+1]+y_offset_);
				break;
			}
			//         wxPen pPen=dc.GetPen();
			//         pPen.SetWidth(17);
			//         pPen.SetColour(64,64,64);
			//         dc.SetPen(pPen);
			switch (curveSet_[i]->type_)
			{
			case -1:
				break;
			case FC_ELLIPSE:
			case FC_LINE:
				//            dc.DrawPoint(0,ty[0]);
				//            dc.DrawPoint(tx[0],ys);
				dc.DrawRectangle(0-2+x_offset_,ty[0]-2+y_offset_,5,5);
				dc.DrawRectangle(tx[0]-2+x_offset_,ys-2+y_offset_,5,5);
				break;
			case FC_SQUARE_BOX:
				//            dc.DrawPoint(0,ty[0]);
				//            dc.DrawPoint(tx[0],ys);
				//            dc.DrawPoint(tx[0],ty[0]);
				dc.DrawRectangle(0-2+x_offset_,ty[0]-2+y_offset_,5,5);
				dc.DrawRectangle(tx[0]-2+x_offset_,ys-2+y_offset_,5,5);
				dc.DrawRectangle(tx[0]-2+x_offset_,ty[0]-2+y_offset_,5,5);
				break;
			case FC_CUSTOM:
				for (j=0; j<nt; j++)
					dc.DrawRectangle(tx[j]-2+x_offset_,ty[j]-2+y_offset_,5,5);
				//               dc.DrawPoint(tx[j],ty[j]);
				break;
			}
		}
		dc.SetPen(tPen);
		dc.SetBrush(wxNullBrush);
	}
	
	if (isDragging_ == 1)
	{ 
		int j;
		int* tx;
		int* ty;
		int xs, ys;
		int nt;
		wxPen tPen=dc.GetPen();
		dc.SetBrush(*wxTRANSPARENT_BRUSH);
		dc.SetPen(dragCurve_.pen_);
		tx = dragCurve_.x_;
		ty = dragCurve_.y_;
		nt = dragCurve_.n_;
		xs = dragCurve_.xs_;
		ys = dragCurve_.ys_;
		
		switch (dragCurve_.type_)
		{
		case -1:
			break;
		case FC_ELLIPSE:
            MyDrawEllipticArc(dc,-tx[0]+x_offset_, ty[0]+y_offset_, 2*tx[0], 2*(ys-ty[0]), 0, 90);
            break;
		case FC_VERT_LINE:
			//            if ((tx[0]>=0) && (tx[0]<xs))
			dc.DrawLine(tx[0]+x_offset_, 0+y_offset_, tx[0]+x_offset_, ys+y_offset_);
            break;
		case FC_HORIZ_LINE:
            if ((ty[0]>=0) && (ty[0]<ys))
				dc.DrawLine(0+x_offset_, ty[0]+y_offset_, xs+x_offset_, ty[0]+y_offset_);
            break;
		case FC_LINE:
            // determine clipping
            tye = ys;
            txe = tx[0];
            txe = (txe<=0)?1:txe;
            txs = 0;
            tys = ty[0];
            tys = (tys>ys)?ys:tys;
            if (txe>xs)
            {
				tye = ccy_-((ccy_-tys)*(txe-xs))/txe;
				txe = xs;
            }
            if (tys<0)
            {
				txs= (txe*(-tys))/(tye-tys);
				tys= 0;
            }
			
            dc.DrawLine(0+x_offset_, ty[0]+y_offset_, tx[0]+x_offset_, ys+y_offset_);
            break;
		case FC_SQUARE_BOX:
            txs = (tx[0]<0)?0:tx[0];
            txs = (txs>=xs)?xs-1:txs;
            tys = (ty[0]<0)?0:ty[0];
            tys = (tys>=ys)?ys-1:tys;
            dc.DrawLine(0+x_offset_, tys+y_offset_, txs+x_offset_, tys+y_offset_);
            dc.DrawLine(txs+x_offset_, tys+y_offset_, txs+x_offset_, ys+y_offset_);
            break;
		case FC_CUSTOM:
            for (j=0; j<(nt-1); j++)
				dc.DrawLine(tx[j]+x_offset_, ty[j]+y_offset_, tx[j+1]+x_offset_, ty[j+1]+y_offset_);
            break;
		}
		
		dc.SetPen(tPen);
		dc.SetBrush(wxNullBrush);
		
	}
	
	//draw horizontal and vertical axis
	if(xAxis)	xAxis->PlotAxis(&dc);
	if(yAxis)	yAxis->PlotAxis(&dc);
	
	//draw new text to the image from text object list
	BgText	*bgText;
	tlist_.ResetList();
	while(bgText = tlist_.GetText())
	{
		dc.SetFont(*(bgText->font_));
		dc.DrawText(bgText->text_, bgText->x_, bgText->y_);
	}
	
	//draw bitmaps
	blist_.ResetList();
	BgBitmap	*bitmap;
	while(bitmap = blist_.GetBitmap())
		dc.DrawBitmap(*(bitmap->bitmap_), bitmap->location_x_, bitmap->location_y_, true);

	//draw zoom window box
	if((zoom_window)&&(has_focus))
	{
		//draw refresh window
#if wxCHECK_VERSION(2, 3, 0)
      if(refresh_box)   dc.DrawBitmap(wxBitmap(refresh_box), cx, cy);
#else
		if(refresh_box)	dc.DrawBitmap(refresh_box->ConvertToBitmap(), cx, cy);
#endif

		//compute height and width of zoom box and window
		int zWidth, zHeight;
		int dWidth, dHeight;
		zWidth	= zoombox->GetWidth();
		zHeight	= zoombox->GetHeight();
		GetSize(&dWidth, &dHeight);

		//compute upper corner of window
		cx = m_x_ - zWidth/2;
		cy = m_y_ - zHeight/2;

		//check bounds....
		if(cx < 0)	cx = 0;
		if(cy < 0)	cy = 0;
		if(cx >= dWidth - zWidth)	cx	= dWidth  - zWidth - 1;
		if(cy >= dHeight - zHeight)	cy	= dHeight - zHeight - 1;

		//place in image coordinate frame
		cx	= cx + GetScrollPos(wxHORIZONTAL);
		cy	= cy + GetScrollPos(wxVERTICAL);

		//check bounds		
		int width	= pimage->GetWidth()*zoom_level, height	= pimage->GetHeight()*zoom_level;
		if(cx >= width - zWidth)	cx	= width - zWidth - 1;
		if(cy >= height - zHeight)	cy	= height - zHeight - 1;

		//draw zoom window
#if wxCHECK_VERSION(2, 3, 0)
      dc.DrawBitmap(wxBitmap(zoombox), cx, cy);
#else
		dc.DrawBitmap(zoombox->ConvertToBitmap(), cx, cy);
#endif

		//create refresh box for next iteration
		DefineRefreshBox();
	}

}

//adds an image margin to the image
//pre : (x_margin, y_margin) define the margins in the
//      x and y direction respectively
//post: an image margin has been added
void BgImCanvas::AddMargin(int x_margin, int y_margin)
{
	//shift the image
	x_offset_	= x_margin;
	y_offset_	= y_margin;

	//redraw the image
	Refresh();

}

//overloaded zoom function that does not use current mouse position
int BgImCanvas::Zoom(int zconst)
{
	return Zoom(zconst, 0, 0);
}

//allows one to zoom into/out of the image dispalyed by the image canvas
//pre : zconst is the zoom constant
//      (m_x, m_y) is the current position of the mouse
//post: the image has been zoomed in by a factor of zconst
//      returns 1 when zoom not possible due to image constraints,
//		zoom at maximum or minimum image dimensions, or possible error
int BgImCanvas::Zoom(int zconst, int m_x, int m_y)
{
	//must have bitmap to zoom
	if(hasbitmap)
	{
		//determine zooming action
		int	action;
		if(zconst < zoom_level)
			action	= ZOOM_OUT;
		else if(zconst > zoom_level)
			action	= ZOOM_IN;
		
		//check bounds
		int	width	= pimage->GetWidth(), height	= pimage->GetHeight();
		if(zconst < 0)
		{
			//take absolute value of zoom constant
			int	pos_zc	= -zconst;

			//compute new width
			if(width%pos_zc)
				width	= width/pos_zc+1;
			else
				width	/= pos_zc;

			//compute new height
			if(height%pos_zc)
				height	= height/pos_zc+1;
			else
				height	/= -zconst;

			//make sure width and height are above minimum
			//before continuing
			if((width < MIN_WIDTH)||(height < MIN_HEIGHT))
				return 1;
		}
		else if (zconst > 0)
		{
			width	*= zconst;
			height	*= zconst;
			if((width > MAX_WIDTH)||(height > MAX_HEIGHT))
				return 1;
		}
		else
		{
			//zconst equals zero (ambiguous so exit)
			return 1;
		}
		
		//set zoom_level
		zoom_level	= zconst;

		if(zoom_level == 1)
		{
			delete	pbitmap;
#if wxCHECK_VERSION(2, 3, 0)
         pbitmap = new wxBitmap(pimage);
#else
			pbitmap	= new wxBitmap(pimage->ConvertToBitmap());
#endif
			if(zImg)	delete zImg;
			zImg	= new unsigned char [3*height*width];
			memcpy(zImg, pimage->GetData(), 3*height*width);
		}
		else if(zoom_level > 1)
		{
			//zoom into image using zoom level
			wxImage	tempIm(width, height);
			if(zImg)	delete [] zImg;
			zImg	= new unsigned char [3*width*height];
			bgZoomIn(&zImg, pimage->GetData(), pimage->GetWidth(), pimage->GetHeight(), zoom_level, false);
			memcpy(tempIm.GetData(), zImg, 3*width*height*sizeof(unsigned char));
			
			//reset the bitmap using zoomed image
			delete pbitmap;
#if wxCHECK_VERSION(2, 3, 0)
         pbitmap = new wxBitmap(tempIm);
#else
			pbitmap = new wxBitmap(tempIm.ConvertToBitmap());
#endif
		}
		else if(zoom_level < -1)
		{
			//zoom out of image using zoom level
			wxImage	tempIm(width, height);
			if(zImg)	delete [] zImg;
			zImg	= new unsigned char [3*width*height];
			bgZoomOut(&zImg, pimage->GetData(), pimage->GetWidth(), pimage->GetHeight(), (-zoom_level), false);
			memcpy(tempIm.GetData(), zImg, 3*width*height*sizeof(unsigned char));
			
			//reset the bitmap using zoomed image
			delete pbitmap;
#if wxCHECK_VERSION(2, 3, 0)
         pbitmap = new wxBitmap(tempIm);
#else
			pbitmap = new wxBitmap(tempIm.ConvertToBitmap());
#endif
		}

		//if a bitmap is not to be shown, clear (make white) the
		//zoomed image
		if(!showbitmap_)
			memset(zImg, 255, 3*width*height*sizeof(unsigned char));

		//add point data to zoomed image
		AddPoints(zImg, width);

		//re-display image
		Refresh();

		//set scroll bars according to current mouse position and zooming action
		AdjustScroll(m_x, m_y, action);

		//return 1 when maximum or minimum image dimension has occured
		if((width == MAX_WIDTH)||(width == MIN_WIDTH)||(height == MAX_HEIGHT)||(height == MIN_HEIGHT))
			return 1;

		//return 1 when close to maximum or minimum image dimension...
		/*********************************************************************/

		if(zconst > 0)
		{
			width	+= pimage->GetWidth();
			height	+= pimage->GetHeight();
			if((width > MAX_WIDTH)||(height > MAX_HEIGHT))
				return 1;
		}
		else if(zconst < 0)
		{
			
			//obtain width and height of original image
			width	= pimage->GetWidth();
			height	= pimage->GetHeight();
			
			//take absolute value of zoom constant
			int	pos_zc	= -zconst;
			
			//compute new width
			if(width%pos_zc)
				width	= width/pos_zc+1;
			else
				width	/= pos_zc;
			
			//compute new height
			if(height%pos_zc)
				height	= height/pos_zc+1;
			else
				height	/= -zconst;
			width	= (pimage->GetWidth());

			//check image bounds...
			if((width < MIN_WIDTH)||(height < MIN_HEIGHT))
				return 1;
		}

		/*********************************************************************/
	}

	//done.
	return 0;

}

//allows one to zoom into the image stored by the image canvas
//pre : (m_x, m_y) is the current mouse position
//post: the image has been zoomed in by a factor of zoom_level+1
//      1 is returned when max zoom level is reached
int BgImCanvas::ZoomIn(int m_x, int m_y)
{
	//must have bitmap to zoom into
	if(hasbitmap)
	{
		//make sure zoom level is not at maximum before proceeding -
		//max_zoom_level has value 0 when unspecified
		if((max_zoom_level)&&(zoom_level == max_zoom_level))
		{
			wxWindow *parent	= child_frame_->GetParent();
			((BgMdiFrame *) parent)->UpdateZoomControl((wxMDIChildFrame *) child_frame_, true, false);
			return 1;
		}

		//zoom image
		int zconst = zoom_level + 1;
		if(zconst == -1) zconst = 1;
		int	maxZoom	= Zoom(zconst, m_x, m_y);

		//take into account maximum zoom level
		maxZoom = (maxZoom || ((max_zoom_level)&&(zoom_level == max_zoom_level)));

		//set title of child frame
		wxWindow *parent	= child_frame_->GetParent();
		((BgMdiFrame *) parent)->SetChildTitle((wxMDIChildFrame *) child_frame_, zoom_level, maxZoom, false);

		//update zoom control of child frame
		((BgMdiFrame *) parent)->UpdateZoomControl((wxMDIChildFrame *) child_frame_, maxZoom, false);

		//indicate if max zoom has occured
		return maxZoom;
	}

	//done.
	return 0;
}

//allows one to zoom out of the image stored by the image canvas
//pre : (m_x, m_y) is the current mouse position
//post: the image has been zoomed out by a factor of zoom_level-1
//      returns 1 if minimum zoom has occured
int BgImCanvas::ZoomOut(int m_x, int m_y)
{
	//must have bitmap to zoom into
	if(hasbitmap)
	{
		//make sure zoom level is not at minimum before proceeding -
		//min_zoom_level has value 0 when unspecified
		if((min_zoom_level)&&(zoom_level == min_zoom_level))
		{
			wxWindow *parent	= child_frame_->GetParent();
			((BgMdiFrame *) parent)->UpdateZoomControl((wxMDIChildFrame *) child_frame_, false, true);
			return 1;
		}

		//zoom image
		int zconst = zoom_level - 1;
		if(zconst == 0)	zconst = -2;
		int	minZoom	= Zoom(zconst, m_x, m_y);

		//take into account mininimum zoom level;
		minZoom = (int)(minZoom ||((min_zoom_level)&&(zoom_level == min_zoom_level)));

		//set title of child frame
		wxWindow *parent	= child_frame_->GetParent();
		((BgMdiFrame *) parent)->SetChildTitle((wxMDIChildFrame *) child_frame_, zoom_level, false, minZoom);

		//update zoom control of child frame
		((BgMdiFrame *) parent)->UpdateZoomControl((wxMDIChildFrame *) child_frame_, false, minZoom);

		//indicate if min zoom has occured
		return minZoom;
	}

	//done.
	return 0;
}

//sets max zoom level
void BgImCanvas::SetMaxZoomLevel(int mzl)
{
	max_zoom_level	= mzl;
}

//sets min zoom level
void BgImCanvas::SetMinZoomLevel(int mzl)
{
	min_zoom_level	= mzl;
}


#define WIN_PERC 16

void BgImCanvas::DefineZoomBox(int l_x, int h_x, int l_y, int h_y)
{

	//obtain image height and width
	int	iWidth = pimage->GetWidth(), iHeight = pimage->GetHeight();

	//calculate box height and width
	static int bWidth = h_x-l_x+1, bHeight = h_y-l_y+1;

	//allocate/deallocate memory
	if((bWidth != h_x-l_x+1)||(!buf))
	{
		bWidth	= h_x-l_x+1;
		bHeight	= h_y-l_y+1;
		if(buf)	delete buf;
		buf	= new unsigned char [3*bWidth*bHeight];
	}

	//get point pen colour
	unsigned char r, g, b;
	if(point_colour)
	{
		r	= point_colour->Red();
		g	= point_colour->Green();
		b	= point_colour->Blue();
	}

	//crop image data and store it into buf (if the bitmap is being displayed...)
	unsigned char	*imData		= pimage->GetData();
	int bx, by, ix, iy, idp, bdp;
	if(showbitmap_)
	{
		for(by=0; by<bHeight; by++)
		{
			for(bx=0; bx<bWidth; bx++)
			{
				ix	= bx + l_x;
				iy	= by + l_y;
				idp	= 3*(iy*iWidth + ix);
				bdp	= 3*(by*bWidth + bx);
				buf[bdp  ]	= imData[idp  ];
				buf[bdp+1]	= imData[idp+1];
				buf[bdp+2]	= imData[idp+2];
				
				//account for point sets using point map
				if(point_map[idp/3])
				{
					buf[bdp]	= r;
					buf[bdp+1]	= g;
					buf[bdp+2]	= b;
				}
			}
		}
	}
	//create blank (white) image and plot point set onto it
	//prior to zoom
	else
	{
		memset(buf, 255, 3*bWidth*bHeight*sizeof(unsigned char));
		for(by=0; by<bHeight; by++)
		{
			for(bx=0; bx<bWidth; bx++)
			{
				ix	= bx + l_x;
				iy	= by + l_y;
				idp	= 3*(iy*iWidth + ix);
				bdp	= 3*(by*bWidth + bx);

				//account for point sets using point map
				if(point_map[idp/3])
				{
					buf[bdp]	= r;
					buf[bdp+1]	= g;
					buf[bdp+2]	= b;
				}
			}
		}
	}

	//zoom the data in buf...

	//set zoom window size and allocate memory
	static int	zWidth	= bWidth*zoom_level*2;
	static int	zHeight	= bHeight*zoom_level*2;
	if((zWidth != bWidth*zoom_level*2)||(!zoombox))
	{
		zWidth	= bWidth*zoom_level*2;
		zHeight	= bHeight*zoom_level*2;
		if(zoombox)	delete zoombox;
		zoombox	= new wxImage(zWidth, zHeight);

	}

	//create a zoom box image using buf
	unsigned char	*zbData	= zoombox->GetData();
	bgZoomIn(&zbData, buf, bWidth, bHeight, zoom_level*2, false);

	//done.
	return;
}

void BgImCanvas::DefineRefreshBox(void)
{

	//obtain image height and width
	int	iWidth = pimage->GetWidth()*zoom_level, iHeight = pimage->GetHeight()*zoom_level;

	//calculate box height and width
	static int bWidth = zoombox->GetWidth(), bHeight = zoombox->GetHeight();

	//allocate/de-allocate memory for refresh box
	if((bWidth != zoombox->GetWidth())||(!refresh_box))
	{
		bWidth	= zoombox->GetWidth();
		bHeight	= zoombox->GetHeight();
		if(refresh_box)	delete [] refresh_box;
		refresh_box	= new wxImage(bWidth, bHeight);
	}
	unsigned char *rBuf	= refresh_box->GetData();

	//get point pen colour
	unsigned char r, g, b;
	if(point_colour)
	{
		r	= point_colour->Red();
		g	= point_colour->Green();
		b	= point_colour->Blue();
	}

	//define refresh box...

	//crop image data and store it into rBuf...
	unsigned char *imData	= pimage->GetData();
	int bx, by, ix, iy, bdp, idp;
	for(by=0; by<bHeight; by++)
	{
		for(bx=0; bx<bWidth; bx++)
		{
			ix	= bx + cx;
			iy	= by + cy;
			idp	= 3*(iy*iWidth + ix);
			bdp	= 3*(by*bWidth + bx);
			rBuf[bdp  ]	= zImg[idp  ];
			rBuf[bdp+1]	= zImg[idp+1];
			rBuf[bdp+2]	= zImg[idp+2];
		}
	}

	//done.
	return;
}

//creates point map used by zoom and refresh windows
void BgImCanvas::CreatePointMap(int w, int h)
{

	//initialze point map
	if(point_map)	delete [] point_map;
	point_map	= new bool [w*h];
	memset(point_map, 0, w*h*sizeof(bool));

	//if point sets already exist then add the,
	if (nPointSets_ > 0)
	{
		//adjust point pen colour to be that of the last
		//point set added
		if(point_colour)	delete point_colour;
		point_colour	= new wxColour(pointSet_[nPointSets_-1]->pen_.GetColour());

		int nt, *tx, *ty, i, j;
		for (i=0; i<nPointSets_; i++)
		{
			nt = pointSet_[i]->n_;
			tx = pointSet_[i]->x_;
			ty = pointSet_[i]->y_;
			if(pointSet_[i]->type_ == 1) // point
			{
				for(j=0; j<nt; j++)
					point_map[ty[j]*w+tx[j]] = true;
			}
		}
	}
}

//adds points from point sets onto specified image
void BgImCanvas::AddPoints(unsigned char *im, int width)
{
	//if point sets have been defined add point data to image
	if(nPointSets_	> 0)
	{
		unsigned char r, g, b;
		wxColour	pen_colour;
		int	nt, *tx, *ty, i, j, x, y, dp, offset;
		for(i=0; i < nPointSets_; i++)
		{
			if(pointSet_[i]->type_ == 1) //point
			{
				//get pen colour
				pen_colour	= pointSet_[i]->pen_.GetColour();
				r	= pen_colour.Red();
				g	= pen_colour.Green();
				b	= pen_colour.Blue();

				//place points
				nt	= pointSet_[i]->n_;
				tx	= pointSet_[i]->x_;
				ty	= pointSet_[i]->y_;
				for(j=0; j<nt; j++)
				{
					dp = 3*(ty[j]*width+tx[j])*zoom_level;
					for(y=0; y<zoom_level; y++)
					{
						for(x=0; x<zoom_level; x++)
						{
							offset	= 3*(y*width+x);
							im[dp+offset  ]	= r;
							im[dp+offset+1]	= g;
							im[dp+offset+2]	= b;
						}
					}
				}
			}
		}
	}
}

//displays zoom window at cursor position
void BgImCanvas::DisplayZoomWindow(int m_x, int m_y)
{

	//set mouse position...
	m_x_	= m_x;
	m_y_	= m_y;

	//compute width and height of window....

	/***********************************************************/

	//obtain image height and width
	int w = pimage->GetWidth(), h = pimage->GetHeight();

	//take percentage of height and width to compute
	//box half-width and half-height
	int	hw, hh;
	int	wp	= WIN_PERC;
	if(w%wp)
		hw	= w/wp + 1;
	else
		hw	= w/wp;

	if(h%wp)
		hh	= h/wp + 1;
	else
		hh	= h/wp;

	/***********************************************************/

	//normalize mouse coordinates to image coordinate frame

	/***********************************************************/

	if(zoom_level > 0)
	{
		m_x	= (m_x + GetScrollPos(wxHORIZONTAL))/zoom_level;
		m_y	= (m_y + GetScrollPos(wxVERTICAL))/zoom_level;
	}
	else if(zoom_level < 0)
	{
		m_x	= (m_x + GetScrollPos(wxHORIZONTAL))*(-zoom_level);
		m_y	= (m_y + GetScrollPos(wxVERTICAL))*(-zoom_level);
	}

	/***********************************************************/

	//make sure x_m and y_m are with bounds before proceeding...

	/***********************************************************/

	if((m_x < 0)||(m_x >= w)||(m_y < 0)||(m_y >= h))
		return;

	/***********************************************************/

	//compute zoom window bounds...

	/***********************************************************/

	//define window bounds based on current mouse position
	int low_x, hi_x, low_y, hi_y;
	if((low_x	= m_x - hw) <  0) low_x	= 0;
	if((hi_x	= m_x + hw) >= w) hi_x	= w-1;
	if((low_y	= m_y - hh) <  0) low_y	= 0;
	if((hi_y	= m_y + hh) >= h) hi_y	= h-1;

	//make window larger according to current mouse position	
	if(m_x			<= hw) hi_x		+= hw-m_x;
	if((w-m_x-1)	<= hw) low_x	-= hw-(w-m_x-1);
	if(m_y			<= hh) hi_y		+= hh-m_y;
	if((h-m_y-1)	<= hh) low_y	-= hh-(h-m_y-1);

	/***********************************************************/

	//define zoom box...

	/***********************************************************/

	DefineZoomBox(low_x, hi_x, low_y, hi_y);

	/***********************************************************/

	//display zoomed image over current mouse position...

	/***********************************************************/

	Refresh(false);

	/***********************************************************/

	//done.
	return;

}

/*
void BgImCanvas::OnEraseBackground(wxEraseEvent& WXUNUSED(event))
{
}
*/
void BgImCanvas::FillCurveClick()
{
   int i;
   for (i=0; i<ccx_*ccy_; i++)
      curveClick_[i] = 0;
   
   for (i=0; i<nCurveSets_; i++)
      curveSet_[i]->DrawYourself(curveClick_, i+1);

   Refresh();
//   write_pgm_image("curveclick.pgm", curveClick_, 256, 256, "", 4);
}

/*
void BgImCanvas::OnPaint(wxPaintEvent& WXUNUSED(event))
{
   wxClientDC dc(this);
   PrepareDC(dc);
   if(hasbitmap==TRUE)
   {
      dc.DrawBitmap(*pbitmap,0,0);
   }
   else 
   {
      dc.Clear();
   }
}
*/

void BgImCanvas::MyDrawEllipticArc(wxDC& dc, int x, int y, int w, int h, int sa, int ea)
{
   double xc, yc, rx, ry;
   rx = w/2;
   ry = h/2;
   xc = x+rx;
   yc = y+ry;
   int r, c;
   int low_r = y_offset_;
   int hig_r = ccy_+y_offset_;
   int low_c = x_offset_;
   int hig_c = ccx_+x_offset_;

//   if (rx > ry)
//   {
      // x scan
      for (c = (int) xc; c<=(int) (xc+rx); c++)
      {
         if (c>=low_c && c<hig_c)
         {
            r = bgRound(yc-ry*sqrt(1-(c-xc)*(c-xc)/(rx*rx)));
            if (r>=low_r && r<hig_r)
            {
               dc.DrawPoint(c,r);
               // +/- 1
               if ((r+1)<hig_r) dc.DrawPoint(c,r+1);
               if ((r-1)>=low_r) dc.DrawPoint(c,r-1);;
            }
         }
      }
//   }
//   else
//   {
      // y scan
      for (r = (int)(yc-ry); r<=(int) yc; r++)
      {
         if (r>=low_r && r<hig_r)
         {
            c = bgRound(xc+rx*sqrt(1-(r-yc)*(r-yc)/(ry*ry)));
            if (c>=low_c && c<hig_c)
            {
               dc.DrawPoint(c,r);
               // +/- 1
               if ((c+1)<hig_c) dc.DrawPoint(c+1,r);
               if ((c-1)>=low_c) dc.DrawPoint(c-1,r);;
            }
         }
      }
//   }
}

void BgImCanvas::OnMouseRightDown(wxMouseEvent& event)
{
   wxClientDC dc(this);
   PrepareDC(dc);
   wxPoint pt(event.GetLogicalPosition(dc));
   static int x;
   static int y;
   x = pt.x-x_offset_;
   y = pt.y-y_offset_;
   if (x>=0 && x<ccx_ && y>=0 && y<ccy_)
   {
      if (curveClick_[x+y*ccx_]>0)
      {
         lmEventCurve_ = curveClick_[x+y*ccx_] - 1;
         if (curveSet_[lmEventCurve_]->type_ == FC_CUSTOM)
         {
            // determine if close to a node
            int *tempx, *tempy, tempn;
            double mindist, crtdist;
            int i, minnode;
            tempx = curveSet_[lmEventCurve_]->x_;
            tempy = curveSet_[lmEventCurve_]->y_;
            tempn = curveSet_[lmEventCurve_]->n_;
            mindist = sqrt((x-tempx[0])*(x-tempx[0])+(y-tempy[0])*(y-tempy[0]));
            minnode = 0;
            for (i=1; i<tempn; i++)
            {
               crtdist = sqrt((x-tempx[i])*(x-tempx[i])+(y-tempy[i])*(y-tempy[i]));
               if (crtdist < mindist)
               {
                  mindist = crtdist;
                  minnode = i;
               }
            }
            if (mindist <= 3)
            {
               // delete node option
               lmEventNode_ = minnode;
               localMenu_->Enable(BG_IMC_ADDNODE,FALSE);
               localMenu_->Enable(BG_IMC_DELETENODE,TRUE);
            } else
            {
               // add node option
               localMenu_->Enable(BG_IMC_ADDNODE,TRUE);
               localMenu_->Enable(BG_IMC_DELETENODE,FALSE);
            }
         }
         else
         {
            localMenu_->Enable(BG_IMC_ADDNODE,FALSE);
            localMenu_->Enable(BG_IMC_DELETENODE,FALSE);
         }
         lmEventX_ = x;//+x_offset_;
         lmEventY_ = y;//+y_offset_;
         PopupMenu(localMenu_, x+x_offset_, y+y_offset_);
      }
   }
}

/*
//updates hover menu during scroll event
void BgImCanvas::OnScroll(wxScrollWinEvent& event)
{

}
*/

//attemtps to adjust scroll position of window according
//to current mouse position
void BgImCanvas::AdjustScroll(int x, int y, int action)
{
	if(zoom_level > 1)
	{

		//get window width and height
		int	winWidth, winHeight;
		GetSize(&winWidth, &winHeight);

		//get window vertical and horzontal scroll position
		//and offset current mouse position to obtain mouse
		//position relative to the image
		x	= x + GetScrollPos(wxHORIZONTAL);
		y	= y + GetScrollPos(wxVERTICAL);

		//re-location mouse position relative to new image width
		//and height
		if(action == ZOOM_IN)
		{
			x	= zoom_level*(x/(zoom_level-1));
			y	= zoom_level*(y/(zoom_level-1));
		}
		else if(action == ZOOM_OUT)
		{
			x	= zoom_level*(x/(zoom_level+1));
			y	= zoom_level*(y/(zoom_level+1));
		}

		//resize scroll
		int	width	= (pimage->GetHeight())*zoom_level;
		int	height	= (pimage->GetWidth())*zoom_level;
		SetScrollbars(1, 1, width, height);

		//set scroll position according to x and y
		int offset_x	= x/2;
		int offset_y	= y/2;
		SetScrollPos(wxHORIZONTAL, offset_x, true);
		SetScrollPos(wxVERTICAL, offset_y, true);

		//refresh window contents
		Scroll(offset_x, offset_y);
	}
}

void BgImCanvas::OnEvent(wxMouseEvent& event)
{
/*
   wxClientDC dc(this);
   PrepareDC(dc);
   
   wxPoint pt(event.GetLogicalPosition(dc));
   
   if (g_xpos > -1 && g_ypos > -1 && event.Dragging())
   {
      dc.SetPen(*wxBLACK_PEN);
      dc.DrawLine(g_xpos, g_ypos, pt.x, pt.y);
      
      m_dirty = TRUE;
   }
   
   g_xpos = pt.x;
   g_ypos = pt.y;
   */


   //store mouse pointer location...
   m_x_	= event.m_x;
   m_y_	= event.m_y;

   //keep track of menu window (hide/show)...
   static bool hideWindow = true;
   if((m_x_ >= menuXl_)&&(m_y_ >= menuYl_)&&(m_x_ <= menuXu_)&&(m_y_ <= menuYu_))
	   hideWindow	= false;
   else if(!event.Leaving())
	   hideWindow	= true;

   wxClientDC dc(this);
   PrepareDC(dc);
   wxPoint pt(event.GetLogicalPosition(dc));
   static int x;
   static int y;
   static int curvedrag;
   x = pt.x-x_offset_;
   y = pt.y-y_offset_;
//   x = event.GetX();
//   y = event.GetY();

   if (isDragging_ == 0 && event.LeftDown())
   {
      // check if over any line
      if (x>=0 && x<ccx_ && y>=0 && y<ccy_)
      {
         if (curveClick_[x+y*ccx_]>0)
         {
            isDragging_ = 1;
            curvedrag = curveClick_[x+y*ccx_]-1;
            dragCurve_.SetCurve(curveSet_[curveClick_[x+y*ccx_]-1]);
            dragCurve_.pen_.SetColour(wxColour(128,128,128));
            dragCurve_.StartDragging(x, y);
            Refresh();
         }
      }
   }
   else if (isDragging_ == 1 && event.Dragging())
   {
      // modify curve to drag
      dragCurve_.DragTo(x, y);
      Refresh(false);
   }
   else if (isDragging_ == 1)// && event.LeftUp())
   {
      isDragging_ = 0;
      dragCurve_.EndDragging(x, y);
      curveSet_[curvedrag]->SetCurve(&dragCurve_);
      FillCurveClick();
      mouseModif_ = 1;
      AddPendingEvent(wxCommandEvent(BG_EVENT_UPDATE, BG_EVENT_UPDATE_ID));
      Refresh();
   }
   else if (isDragging_ == 0 && event.Moving())
   {
      // check if over any line
      if (x>=0 && x<ccx_ && y>=0 && y<ccy_)
      {
         if (curveClick_[x+y*ccx_]>0 && crossCursor_ == 0)
         {
            crossCursor_ = 1;
            SetCursor(*wxCROSS_CURSOR);
         }
         else if (crossCursor_ == 1)
         {
            SetCursor(*wxSTANDARD_CURSOR);
            crossCursor_ = 0;
         }
      }
   }

   //if zoom is enabled, set the cursor to the zoom icon
   //otherwise leave it as default
   if(event.Entering())
   {
	   //when parent is not the active child frame, set the cursor
	   //to wxCURSOR_ARROW
	   bool	current_child	= false;
	   wxMDIParentFrame *parent	= (wxMDIParentFrame *)(child_frame_->GetParent());
	   wxMDIChildFrame	*activeChild	= parent->GetActiveChild();
	   if(activeChild == (wxMDIChildFrame *) child_frame_)
		   current_child	= true;

	   if((zoom_out || zoom_in)&&(current_child))
		   SetCursor(wxCURSOR_MAGNIFIER);
	   else if((zoom_window)&&(current_child))
		   SetCursor(wxCURSOR_CROSS);
	   else
		   SetCursor(wxCURSOR_ARROW);

	   //indiciate that the window has acquired focus
	   has_focus	= true;

	   //show the menu window
	   if((menuWindow)&&(popup))
		   menuWindow->Show(TRUE);
   }

   //indicate that the window has lost focus
   if(event.Leaving())
   {
	   //indicate that the window has lost focus
	   has_focus	= false;

	   //indicate that the mouse pointer is leaving
	   //this window
	   leaving		= true;

	   //refresh canvas upon using a zoom window
	   if(zoom_window)
		Refresh(false);

	   //hide the menu window
	   if((hideWindow)&&(popup)&&(menuWindow))
		   menuWindow->Show(FALSE);

   }

   //check if the left mouse button has been clicked and zoom is activated
   //if so zoom the image
   if(event.LeftDown()&&zoom_in)
	   ZoomIn(m_x_, m_y_);

   if((event.LeftDown())&&zoom_out)
	   ZoomOut(m_x_, m_y_);

   if(zoom_window)
	   DisplayZoomWindow(m_x_, m_y_);

}

void BgImCanvas::AddHoverWindow(wxWindow* hoverWindow, int pp)
{
	popup		= pp;
	menuWindow	= hoverWindow;
	if(popup)	menuWindow->Show(FALSE);
	else		menuWindow->Show(TRUE);
	int width, height;
	menuWindow->GetSize(&width, &height);
	menuXl_	= HOVER_MENU_X - HOVER_MENU_BOUND;
	menuYl_	= HOVER_MENU_Y - HOVER_MENU_BOUND;
	menuXu_	= menuXl_ + width + HOVER_MENU_BOUND;
	menuYu_	= menuXu_ + width + HOVER_MENU_BOUND;
	menuWindow->SetSize(HOVER_MENU_X, HOVER_MENU_Y, width, height);
}

void BgImCanvas::SetHoverWindowLocation(int x, int y)
{
	int width, height;
	menuWindow->GetSize(&width, &height);
	if((x < 0)||(x >= width)||(y < 0)||(y >= height))	return;
	menuWindow->SetSize(x, y, width, height);
}

// ---------------------------------------------------------------------------
// BgMdiEdgeChild
// ---------------------------------------------------------------------------

BgMdiEdgeChild::BgMdiEdgeChild(wxMDIParentFrame *parent, const wxString& title,
                           const wxPoint& pos, const wxSize& size,
                           const long style)
                           : wxMDIChildFrame(parent, BG_EDGE_WINDOW, title, pos, size, style)
{

   //set window number
   window_number_ = gs_nFrames;

   //assume image is not yet loaded into segmentation window
   filename_	= NULL;

   bpsize_ = 160;

   imagePlotSplitter_ = new wxSplitterWindow(this, -1, wxPoint(bpsize_, 0), wxDefaultSize, wxSP_NOBORDER | wxSP_3DSASH);
   plotSplitter_ = new wxSplitterWindow(imagePlotSplitter_, -1, wxDefaultPosition, wxDefaultSize, wxSP_NOBORDER | wxSP_3DSASH);
   origEdgeImage_ = new BgImCanvas(this, imagePlotSplitter_, wxDefaultPosition, wxDefaultSize);
   origEdgeImage_->SetScrollbars(20, 20, 50, 50);
   plotNmxImage_ = new BgImCanvas(this, plotSplitter_, wxDefaultPosition, wxDefaultSize);

   plotNmxImage_->SetScrollbars(20, 20, 50, 50);
   plotTotImage_ = new BgImCanvas(this, plotSplitter_, wxDefaultPosition, wxDefaultSize);

   plotTotImage_->SetScrollbars(20, 20, 50, 50);
   
   g_children.Append(this);
   //imagePlotSplitter_->SetClientSize(GetClientSize());
   imagePlotSplitter_->SplitVertically(origEdgeImage_, plotSplitter_,256);
   plotSplitter_->SplitHorizontally(plotNmxImage_, plotTotImage_,256);

   // panel stuff
   buttonPanel_ = new wxPanel(this, -1, wxPoint(0, 0));
   edButton_ = new wxButton(buttonPanel_, BG_EDGE_DETECT, "Edge Detect", wxPoint(40,10));
   cpButton_ = new wxButton(buttonPanel_, BG_CHANGE_PARAM_EDGE, "Change...", wxPoint(40,155));

   wxStaticBox* viewSB = new wxStaticBox(buttonPanel_, -1, "View", wxPoint(35, 45), wxSize(85, 65)); 
   viewOrigCheck_ = new wxCheckBox(buttonPanel_, BG_EDGE_CVIEW_ORIG, "Image", wxPoint(45,65));//, wxPoint(10,85));
   viewEdgeCheck_ = new wxCheckBox(buttonPanel_, BG_EDGE_CVIEW_EDGE, "Edges", wxPoint(45,85));//, wxPoint(10,105));

   int deltal = 45;
   // put the parameters
   wxStaticText* stParam = new wxStaticText(buttonPanel_, -1, "PARAMETERS:", wxPoint(C_PARAMX-5, 105 + C_PARAMY));

   txtKernelSize_ = new wxStaticText(buttonPanel_, -1, "Grad Win.", wxPoint(C_PARAMX-5,deltal+125+C_PARAMY+0*C_PARAMDY));
   valKernelSize_ = new wxStaticText(buttonPanel_, -1, "NA     ", wxPoint(C_PARAMX+C_PARAMDX+35,deltal+125+C_PARAMY+0*C_PARAMDY), wxSize(C_PARAMSX-20,C_PARAMSY));

   txtMinPt_ = new wxStaticText(buttonPanel_, -1, "Min. length", wxPoint(C_PARAMX-5,deltal+125+C_PARAMY+1*C_PARAMDY));
   valMinPt_ = new wxStaticText(buttonPanel_, -1, "NA     ", wxPoint(C_PARAMX+C_PARAMDX+35,deltal+125+C_PARAMY+1*C_PARAMDY), wxSize(C_PARAMSX-20,C_PARAMSY));

   wxStaticBox* nmxSB = new wxStaticBox(buttonPanel_, -1, "Nonmaxima supp.", wxPoint(5, deltal+205+0), wxSize(140,4*C_PARAMDY-5));
   txtNmxType_ = new wxStaticText(buttonPanel_, -1, "Type", wxPoint(C_PARAMX, deltal+205+C_PARAMY+0*C_PARAMDY));
   valNmxType_ = new wxStaticText(buttonPanel_, -1, "NA    ", wxPoint(C_PARAMX+C_PARAMDX,deltal+205+C_PARAMY+0*C_PARAMDY), wxSize(C_PARAMSX,C_PARAMSY));
   txtNmxR_ = new wxStaticText(buttonPanel_, -1, "Rank", wxPoint(C_PARAMX,deltal+205+C_PARAMY+1*C_PARAMDY));
   valNmxR_ = new wxStaticText(buttonPanel_, -1, "NA     ", wxPoint(C_PARAMX+C_PARAMDX,deltal+205+C_PARAMY+1*C_PARAMDY), wxSize(C_PARAMSX,C_PARAMSY));
   txtNmxC_ = new wxStaticText(buttonPanel_, -1, "Conf", wxPoint(C_PARAMX,deltal+205+C_PARAMY+2*C_PARAMDY));
   valNmxC_ = new wxStaticText(buttonPanel_, -1, "NA     ", wxPoint(C_PARAMX+C_PARAMDX,deltal+205+C_PARAMY+2*C_PARAMDY), wxSize(C_PARAMSX,C_PARAMSY));

   wxStaticBox* hhSB = new wxStaticBox(buttonPanel_, -1, "Hyst. High Tr.", wxPoint(5, deltal+325+0), wxSize(140,4*C_PARAMDY-5));
   txtHHType_ = new wxStaticText(buttonPanel_, -1, "Type", wxPoint(C_PARAMX,deltal+325+C_PARAMY+0*C_PARAMDY));
   txtHHR_ = new wxStaticText(buttonPanel_, -1, "Rank", wxPoint(C_PARAMX,deltal+325+C_PARAMY+1*C_PARAMDY));
   txtHHC_ = new wxStaticText(buttonPanel_, -1, "Conf", wxPoint(C_PARAMX,deltal+325+C_PARAMY+2*C_PARAMDY));
   valHHType_ = new wxStaticText(buttonPanel_, -1, "NA    ", wxPoint(C_PARAMX+C_PARAMDX,deltal+325+C_PARAMY+0*C_PARAMDY), wxSize(C_PARAMSX,C_PARAMSY));
   valHHR_ = new wxStaticText(buttonPanel_, -1, "NA     ", wxPoint(C_PARAMX+C_PARAMDX,deltal+325+C_PARAMY+1*C_PARAMDY), wxSize(C_PARAMSX,C_PARAMSY));
   valHHC_ = new wxStaticText(buttonPanel_, -1, "NA     ", wxPoint(C_PARAMX+C_PARAMDX,deltal+325+C_PARAMY+2*C_PARAMDY), wxSize(C_PARAMSX,C_PARAMSY));

   wxStaticBox* hlSB = new wxStaticBox(buttonPanel_, -1, "Hyst. Low Tr.", wxPoint(5, deltal+445+0), wxSize(140, 4*C_PARAMDY-5));
   txtHLType_ = new wxStaticText(buttonPanel_, -1, "Type", wxPoint(C_PARAMX,deltal+445+C_PARAMY+0*C_PARAMDY));
   txtHLR_ = new wxStaticText(buttonPanel_, -1, "Rank", wxPoint(C_PARAMX,deltal+445+C_PARAMY+1*C_PARAMDY));
   txtHLC_ = new wxStaticText(buttonPanel_, -1, "Conf", wxPoint(C_PARAMX,deltal+445+C_PARAMY+2*C_PARAMDY));
   valHLType_ = new wxStaticText(buttonPanel_, -1, "NA    ", wxPoint(C_PARAMX+C_PARAMDX,deltal+445+C_PARAMY+0*C_PARAMDY), wxSize(C_PARAMSX,C_PARAMSY));
   valHLR_ = new wxStaticText(buttonPanel_, -1, "NA     ", wxPoint(C_PARAMX+C_PARAMDX,deltal+445+C_PARAMY+1*C_PARAMDY), wxSize(C_PARAMSX,C_PARAMSY));
   valHLC_ = new wxStaticText(buttonPanel_, -1, "NA     ", wxPoint(C_PARAMX+C_PARAMDX,deltal+445+C_PARAMY+2*C_PARAMDY), wxSize(C_PARAMSX,C_PARAMSY));
   

   //set lower bound zoom limit on display image
   //to the size of the original image
   origEdgeImage_->SetMinZoomLevel(1);

   // set default parameters for edge detection
   rankNmx_ = RANK_NMX;
   confNmx_ = CONF_NMX;
   rankH_ = RANK_H;
   confH_ = CONF_H;
   rankL_ = RANK_L;
   confL_ = CONF_L;
   nMin_ = NMIN;
   nmxType_ = FC_ELLIPSE;
   hystHighType_ = FC_SQUARE_BOX;
   hystLowType_ = FC_ELLIPSE;
   kernelSize_ = KERNEL_SIZE;

   //set params
   SetParametersNum();
   SetParametersStr();

   // also default custom parameters
   nCustH_ = 3;
   nCustL_ = 3;
   custHx_[0] = 0;
   custHx_[1] = 0.7;
   custHx_[2] = 1;
   custHy_[0] = 1;
   custHy_[1] = 0.7;
   custHy_[2] = 0;

   custLx_[0] = 0;
   custLx_[1] = 0.5;
   custLx_[2] = 0.7;
   custLy_[0] = 0.7;
   custLy_[1] = 0.5;
   custLy_[2] = 0;

   // set the values in the boxes

//   cbgEdgeDetect_ = new BgEdgeDetect(kernelSize_);
   cbgEdgeDetect_ = 0;
   cbgImage_ = new BgImage();
   cbgEdgeList_ = 0;
   hasEdge_ = 0;
   hasImage_ = 0;

   int w, h;
   GetClientSize(&w, &h);
   buttonPanel_->SetSize(0, 0, bpsize_, h);
   imagePlotSplitter_->SetSize(bpsize_, 0, w-bpsize_, h);
   imagePlotSplitter_->SetSashPosition(bpsize_+(w-bpsize_)/2,TRUE);
   plotSplitter_->SetSashPosition(h/2,TRUE);
   rightsize_ = (w-bpsize_)/2;
   viewOrigCheck_->SetValue(TRUE);
   viewEdgeCheck_->SetValue(TRUE);

   plotTotImage_->AddCurveSet(&nmxCurve_);
   plotNmxImage_->AddCurveSet(&highCurve_);
   plotNmxImage_->AddCurveSet(&lowCurve_);
   lowCurve_.pen_.SetColour(255,0,0);

   //add margin to plots
   plotNmxImage_->AddMargin(RANK_CONF_MARGINX,RANK_CONF_MARGINY);
   plotTotImage_->AddMargin(RANK_CONF_MARGINX,RANK_CONF_MARGINY);

   //add title to each individual plot
   BgText bgText(1, "      Diagram after Non-Maxima Supression", *wxSWISS_FONT, RANK_CONF_MARGINX - 10, RANK_CONF_MARGINY/2-10);

   plotNmxImage_->AddText(&bgText);
   bgText.SetText("      Diagram before Non-Maxima Supression");
   plotTotImage_->AddText(&bgText);

   //add x and y axis
   plotNmxImage_->AddHorizontalAxis(RANK_CONF_MARGINX-2,RANK_CONF_IMSIZEY+RANK_CONF_MARGINY,RANK_CONF_IMSIZEX+2,10,0.0,1.0);
   plotNmxImage_->AddVerticalAxis  (RANK_CONF_MARGINX-1,RANK_CONF_IMSIZEY+RANK_CONF_MARGINY,RANK_CONF_IMSIZEX,10,0.0,1.0);
   plotTotImage_->AddHorizontalAxis(RANK_CONF_MARGINX-2,RANK_CONF_IMSIZEY+RANK_CONF_MARGINY,RANK_CONF_IMSIZEX+2,10,0.0,1.0);
   plotTotImage_->AddVerticalAxis  (RANK_CONF_MARGINX-1,RANK_CONF_IMSIZEY+RANK_CONF_MARGINY,RANK_CONF_IMSIZEX,10,0.0,1.0);

   //label x and y axis
   bgText.SetText("Rank (   )");
   plotNmxImage_->LabelHorizontalAxis(&bgText);
   plotTotImage_->LabelHorizontalAxis(&bgText);
   bgText.SetText("Confidence (  )");
   plotNmxImage_->LabelVerticalAxis(&bgText);
   plotTotImage_->LabelVerticalAxis(&bgText);

   //place greek symbols
   wxBitmap	ro("ro", wxBITMAP_TYPE_RESOURCE), eta("eta", wxBITMAP_TYPE_RESOURCE);
   BgBitmap	ro_bmp(&ro, 1, RANK_CONF_MARGINX - 10, RANK_CONF_MARGINY/2 - 3);
   BgBitmap eta_bmp(&eta, 2, RANK_CONF_MARGINX, RANK_CONF_MARGINY/2 - 3);
   plotNmxImage_->AddBitmap(&ro_bmp);
   plotNmxImage_->AddBitmap(&eta_bmp);
   plotTotImage_->AddBitmap(&ro_bmp);
   plotTotImage_->AddBitmap(&eta_bmp);
   ro_bmp.SetPlotLocation(RANK_CONF_MARGINX+137,RANK_CONF_IMSIZEY+RANK_CONF_MARGINY+35);
   ro_bmp.SetId(3);
   plotNmxImage_->AddBitmap(&ro_bmp);
   plotTotImage_->AddBitmap(&ro_bmp);
   wxBitmap rotated_eta("rotated_eta", wxBITMAP_TYPE_RESOURCE);
   BgBitmap rotated_eta_bmp(&rotated_eta, 4, RANK_CONF_MARGINX-55,RANK_CONF_IMSIZEY+RANK_CONF_MARGINY-162);
   plotNmxImage_->AddBitmap(&rotated_eta_bmp);
   plotTotImage_->AddBitmap(&rotated_eta_bmp);

   //indicate that the edge window is now open
   window_open	= true;

   //disable edge detection button
   edButton_->Enable(false);

   //get parent tool bar and update it
   toolbar	= parent->GetToolBar();
   UpdateToolBar();

   //set max/min zoom
   maxZoom_	= 0;
   minZoom_	= 1;

}

/*
BgMdiEdgeChild::BgMdiEdgeChild(wxMDIParentFrame *parent, const wxString& title,
                           const wxPoint& pos, const wxSize& size,
                           const long style)
                           : wxMDIChildFrame(parent, BG_EDGE_WINDOW, title, pos, size, style)
{

   //set window number
   window_number_ = gs_nFrames;

   //assume image is not yet loaded into segmentation window
   filename_	= NULL;

   bpsize_ = 100;
   imagePlotSplitter_ = new wxSplitterWindow(this, -1,wxDefaultPosition, wxDefaultSize, wxSP_NOBORDER | wxSP_3DSASH);
   plotSplitter_ = new wxSplitterWindow(imagePlotSplitter_, -1, wxDefaultPosition, wxDefaultSize, wxSP_NOBORDER | wxSP_3DSASH);
   origEdgeImage_ = new BgImCanvas(this, imagePlotSplitter_, wxDefaultPosition, wxDefaultSize);
   origEdgeImage_->SetScrollbars(20, 20, 50, 50);
   plotNmxImage_ = new BgImCanvas(this, plotSplitter_, wxDefaultPosition, wxDefaultSize);

   plotNmxImage_->SetScrollbars(20, 20, 50, 50);
   plotTotImage_ = new BgImCanvas(this, plotSplitter_, wxDefaultPosition, wxDefaultSize);

   plotTotImage_->SetScrollbars(20, 20, 50, 50);
   
   g_children.Append(this);
   //imagePlotSplitter_->SetClientSize(GetClientSize());
   imagePlotSplitter_->SplitVertically(origEdgeImage_, plotSplitter_,256);
   plotSplitter_->SplitHorizontally(plotNmxImage_, plotTotImage_,256);
   // panel stuff
   buttonPanel_ = new wxPanel(this, -1, wxDefaultPosition, wxDefaultSize);
   edButton_ = new wxButton(buttonPanel_, BG_EDGE_DETECT, "Edge Detect", wxPoint(10,10));
   cpButton_ = new wxButton(buttonPanel_, BG_CHANGE_PARAM_EDGE, "Parameters...", wxPoint(10,45));
   viewOrigCheck_ = new wxCheckBox(buttonPanel_, BG_EDGE_CVIEW_ORIG, "View Image", wxPoint(10,85));
   viewEdgeCheck_ = new wxCheckBox(buttonPanel_, BG_EDGE_CVIEW_EDGE, "View Edges", wxPoint(10,105));

   //set lower bound zoom limit on display image
   //to the size of the original image
   origEdgeImage_->SetMinZoomLevel(1);

   // set default parameters for edge detection
   rankNmx_ = RANK_NMX;
   confNmx_ = CONF_NMX;
   rankH_ = RANK_H;
   confH_ = CONF_H;
   rankL_ = RANK_L;
   confL_ = CONF_L;
   nMin_ = NMIN;
   nmxType_ = FC_ELLIPSE;
   hystHighType_ = FC_SQUARE_BOX;
   hystLowType_ = FC_ELLIPSE;
   kernelSize_ = KERNEL_SIZE;

   // also default custom parameters
   nCustH_ = 3;
   nCustL_ = 3;
   custHx_[0] = 0;
   custHx_[1] = 0.7;
   custHx_[2] = 1;
   custHy_[0] = 1;
   custHy_[1] = 0.7;
   custHy_[2] = 0;

   custLx_[0] = 0;
   custLx_[1] = 0.5;
   custLx_[2] = 0.7;
   custLy_[0] = 0.7;
   custLy_[1] = 0.5;
   custLy_[2] = 0;



//   cbgEdgeDetect_ = new BgEdgeDetect(kernelSize_);
   cbgEdgeDetect_ = 0;
   cbgImage_ = new BgImage();
   cbgEdgeList_ = 0;
   hasEdge_ = 0;
   hasImage_ = 0;

   int w, h;
   GetClientSize(&w, &h);
   buttonPanel_->SetSize(w-bpsize_, 0, bpsize_, h);
   imagePlotSplitter_->SetSize(0, 0, w-bpsize_, h);
   imagePlotSplitter_->SetSashPosition((w-bpsize_)/2,TRUE);
   plotSplitter_->SetSashPosition(h/2,TRUE);
   rightsize_ = (w-bpsize_)/2;
   viewOrigCheck_->SetValue(TRUE);
   viewEdgeCheck_->SetValue(TRUE);

   plotTotImage_->AddCurveSet(&nmxCurve_);
   plotNmxImage_->AddCurveSet(&highCurve_);
   plotNmxImage_->AddCurveSet(&lowCurve_);
   lowCurve_.pen_.SetColour(255,0,0);

   //add margin to plots
   plotNmxImage_->AddMargin(RANK_CONF_MARGINX,RANK_CONF_MARGINY);
   plotTotImage_->AddMargin(RANK_CONF_MARGINX,RANK_CONF_MARGINY);

   //add title to each individual plot
   BgText bgText(1, "      Diagram after Non-Maxima Supression", *wxSWISS_FONT, RANK_CONF_MARGINX - 10, RANK_CONF_MARGINY/2-10);

   plotNmxImage_->AddText(&bgText);
   bgText.SetText("      Diagram before Non-Maxima Supression");
   plotTotImage_->AddText(&bgText);

   //add x and y axis
   plotNmxImage_->AddHorizontalAxis(RANK_CONF_MARGINX-2,RANK_CONF_IMSIZEY+RANK_CONF_MARGINY,RANK_CONF_IMSIZEX+2,10,0.0,1.0);
   plotNmxImage_->AddVerticalAxis  (RANK_CONF_MARGINX-1,RANK_CONF_IMSIZEY+RANK_CONF_MARGINY,RANK_CONF_IMSIZEX,10,0.0,1.0);
   plotTotImage_->AddHorizontalAxis(RANK_CONF_MARGINX-2,RANK_CONF_IMSIZEY+RANK_CONF_MARGINY,RANK_CONF_IMSIZEX+2,10,0.0,1.0);
   plotTotImage_->AddVerticalAxis  (RANK_CONF_MARGINX-1,RANK_CONF_IMSIZEY+RANK_CONF_MARGINY,RANK_CONF_IMSIZEX,10,0.0,1.0);

   //label x and y axis
   bgText.SetText("Rank (   )");
   plotNmxImage_->LabelHorizontalAxis(&bgText);
   plotTotImage_->LabelHorizontalAxis(&bgText);
   bgText.SetText("Confidence (  )");
   plotNmxImage_->LabelVerticalAxis(&bgText);
   plotTotImage_->LabelVerticalAxis(&bgText);

   //place greek symbols
   wxBitmap	ro("ro", wxBITMAP_TYPE_RESOURCE), eta("eta", wxBITMAP_TYPE_RESOURCE);
   BgBitmap	ro_bmp(&ro, 1, RANK_CONF_MARGINX - 10, RANK_CONF_MARGINY/2 - 3);
   BgBitmap eta_bmp(&eta, 2, RANK_CONF_MARGINX, RANK_CONF_MARGINY/2 - 3);
   plotNmxImage_->AddBitmap(&ro_bmp);
   plotNmxImage_->AddBitmap(&eta_bmp);
   plotTotImage_->AddBitmap(&ro_bmp);
   plotTotImage_->AddBitmap(&eta_bmp);
   ro_bmp.SetPlotLocation(RANK_CONF_MARGINX+137,RANK_CONF_IMSIZEY+RANK_CONF_MARGINY+35);
   ro_bmp.SetId(3);
   plotNmxImage_->AddBitmap(&ro_bmp);
   plotTotImage_->AddBitmap(&ro_bmp);
   wxBitmap rotated_eta("rotated_eta", wxBITMAP_TYPE_RESOURCE);
   BgBitmap rotated_eta_bmp(&rotated_eta, 4, RANK_CONF_MARGINX-55,RANK_CONF_IMSIZEY+RANK_CONF_MARGINY-162);
   plotNmxImage_->AddBitmap(&rotated_eta_bmp);
   plotTotImage_->AddBitmap(&rotated_eta_bmp);

   //indicate that the edge window is now open
   window_open	= true;

   //disable edge detection button
   edButton_->Enable(false);

   //get parent tool bar and update it
   toolbar	= parent->GetToolBar();
   UpdateToolBar();

   //set max/min zoom
   maxZoom_	= 0;
   minZoom_	= 1;

}
*/

BgMdiEdgeChild::~BgMdiEdgeChild()
{
   if(filename_)	delete [] filename_;

   if (hasEdge_ == 1)
     origEdgeImage_->RemovePointSet(&cbgPointSet_);

   if (cbgEdgeList_ != 0)
      delete cbgEdgeList_;
   delete cbgImage_;
   if (cbgEdgeDetect_ != 0)
      delete cbgEdgeDetect_;

   delete viewEdgeCheck_;
   delete viewOrigCheck_;
   delete cpButton_;
   delete edButton_;
   delete buttonPanel_;



   delete origEdgeImage_;
   delete plotNmxImage_;
   delete plotTotImage_;

   delete plotSplitter_;
   delete imagePlotSplitter_;
   g_children.DeleteObject(this);
}

void BgMdiEdgeChild::OnUpdateNum(wxCommandEvent& WXUNUSED(event))
{
   double tx[MAX_CUSTOM_NODES];
   double ty[MAX_CUSTOM_NODES];
   int ttype,i;
   int npoints,modif=0;
   if (plotNmxImage_->mouseModif_ == 1)
   {
      modif = 1;
      // get new hyst params
      highCurve_.GetParamCurve(tx, ty, ttype,npoints);
      if ((ttype!=FC_CUSTOM) && (ttype!=-1))
      {
         hystHighType_ = ttype;
         rankH_ = tx[0];
         confH_ = ty[0];
      } else
      {
         hystHighType_ = ttype;
         for (i=0; i<npoints; i++)
         {
            custHx_[i] = tx[i];
            custHy_[i] = ty[i];
         }
         nCustH_ = npoints;
      }
      lowCurve_.GetParamCurve(tx, ty, ttype,npoints);
      if ((ttype!=FC_CUSTOM) && (ttype!=-1))
      {
         hystLowType_ = ttype;
         rankL_ = tx[0];
         confL_ = ty[0];
      } else
      {
         hystLowType_ = ttype;
         for (i=0; i<npoints; i++)
         {
            custLx_[i] = tx[i];
            custLy_[i] = ty[i];
         }
         nCustL_ = npoints;
      }
      plotNmxImage_->mouseModif_ = 0;
   }
   if (plotTotImage_->mouseModif_ == 1)
   {
      modif = 1;
      // get new nmx params
      nmxCurve_.GetParamCurve(tx, ty, ttype,npoints);
      if ((ttype!=FC_CUSTOM) && (ttype!=-1))
      {
         nmxType_ = ttype;
         rankNmx_ = tx[0];
         confNmx_ = ty[0];
      }
      plotTotImage_->mouseModif_ = 0;
   }
   if (modif == 1)
   {
      SetParametersNum();
      SetParametersStr();
   }
}

void BgMdiEdgeChild::SetParametersNum()
{
   wxString ts;
   ts = wxString::Format("%.3g", rankH_);
   valHHR_->SetLabel(ts);
   ts = wxString::Format("%.3g", confH_);
   valHHC_->SetLabel(ts);
   ts = wxString::Format("%.3g", rankL_);
   valHLR_->SetLabel(ts);
   ts = wxString::Format("%.3g", confL_);
   valHLC_->SetLabel(ts);
   ts = wxString::Format("%.3g", rankNmx_);
   valNmxR_->SetLabel(ts);
   ts = wxString::Format("%.3g", confNmx_);
   valNmxC_->SetLabel(ts);
}
void BgMdiEdgeChild::SetParametersStr()
{
   switch(hystHighType_)
   {
   case FC_ELLIPSE:
      valHHType_->SetLabel("arc");
      break;
   case FC_VERT_LINE:
      valHHType_->SetLabel("vertical line");		
      break;
   case FC_HORIZ_LINE:
      valHHType_->SetLabel("horizontal line");		
      break;
   case FC_SQUARE_BOX:
      valHHType_->SetLabel("box");		
      break;
   case FC_LINE:
      valHHType_->SetLabel("line");  		
      break;
  	case FC_CUSTOM:
      valHHType_->SetLabel("custom");
      break;
   }

   switch(hystLowType_)
   {
   case FC_ELLIPSE:
      valHLType_->SetLabel("arc");
      break;
   case FC_VERT_LINE:
      valHLType_->SetLabel("vertical line");		
      break;
   case FC_HORIZ_LINE:
      valHLType_->SetLabel("horizontal line");		
      break;
   case FC_SQUARE_BOX:
      valHLType_->SetLabel("box");		
      break;
   case FC_LINE:
      valHLType_->SetLabel("line");  		
      break;
  	case FC_CUSTOM:
      valHLType_->SetLabel("custom");
      break;
   }

   switch(nmxType_)
   {
   case FC_ELLIPSE:
      valNmxType_->SetLabel("arc");
      break;
   case FC_VERT_LINE:
      valNmxType_->SetLabel("vertical line");		
      break;
   case FC_HORIZ_LINE:
      valNmxType_->SetLabel("horizontal line");		
      break;
   case FC_SQUARE_BOX:
      valNmxType_->SetLabel("box");		
      break;
   case FC_LINE:
      valNmxType_->SetLabel("line");  		
      break;
  	case FC_CUSTOM:
      valNmxType_->SetLabel("custom");
      break;
   }   
   wxString ts;
   ts = wxString::Format("%d", nMin_);
   valMinPt_->SetLabel(ts);
   ts = wxString::Format("%d", kernelSize_);
   valKernelSize_->SetLabel(ts);


}

void BgMdiEdgeChild::OnViewEdge(wxCommandEvent&  WXUNUSED(event))
{
   viewEdgeCheck_->SetValue(miViewEdge_->IsChecked());

   if (hasEdge_ == false)
      return;

   if (miViewEdge_->IsChecked() == TRUE)
   {
      // show edges
      origEdgeImage_->AddPointSet(&cbgPointSet_);

   }
   else
   {
      // hide edges
      origEdgeImage_->RemovePointSet(&cbgPointSet_);
   }

}

void BgMdiEdgeChild::OnViewOrig(wxCommandEvent&  WXUNUSED(event))
{
   viewOrigCheck_->SetValue(miViewOrig_->IsChecked());

   if (hasImage_ == false)
      return;

   if (miViewOrig_->IsChecked() == TRUE)
   {
      // show edges
      //canvas->AddPointSet(edgesSeq_[crtImage_]);
      origEdgeImage_->ShowBitmap(true);
   }
   else
   {
      // hide edges
      //canvas->RemovePointSet(edgesSeq_[crtImage_]);
      origEdgeImage_->ShowBitmap(false);
   }
   origEdgeImage_->Refresh();

}

void BgMdiEdgeChild::OnCViewEdge(wxCommandEvent&  WXUNUSED(event))
{
   miViewEdge_->Check(viewEdgeCheck_->GetValue());
   if (hasEdge_ == false)
      return;

   if (viewEdgeCheck_->GetValue() == TRUE)
   {
      // show edges
      origEdgeImage_->AddPointSet(&cbgPointSet_);

   }
   else
   {
      // hide edges
      origEdgeImage_->RemovePointSet(&cbgPointSet_);
   }
}

void BgMdiEdgeChild::OnCViewOrig(wxCommandEvent&  WXUNUSED(event))
{
   miViewOrig_->Check(viewOrigCheck_->GetValue());

   if (hasImage_ == false)
      return;

   if (viewOrigCheck_->GetValue() == TRUE)
   {
      // show edges
      //canvas->AddPointSet(edgesSeq_[crtImage_]);
      origEdgeImage_->ShowBitmap(true);
   }
   else
   {
      // hide edges
      //canvas->RemovePointSet(edgesSeq_[crtImage_]);
      origEdgeImage_->ShowBitmap(false);
   }

}


void BgMdiEdgeChild::OnSize(wxSizeEvent& WXUNUSED(event))
{
   int w, h;
   GetClientSize(&w, &h);
   buttonPanel_->SetSize(0, 0, bpsize_, h);
   imagePlotSplitter_->SetSize(bpsize_, 0, w-bpsize_, h);
   plotSplitter_->SetSashPosition(h/2,TRUE);
   h = w-bpsize_-rightsize_;
   h = (h<0)? 0:h;
   imagePlotSplitter_->SetSashPosition(h,TRUE);
}

void BgMdiEdgeChild::OnQuit(wxCommandEvent& WXUNUSED(event))
{
   Close(TRUE);
}

void BgMdiEdgeChild::OnClose(wxCloseEvent& event)
{
   gs_nFrames--;
   //indicate that the window is closed (used by OnFocus)
   window_open	= false;
   //reset toolbar
   if(gs_nFrames == 0) ResetToolBar();
   event.Skip();
}

void BgMdiEdgeChild::OnFocus(wxFocusEvent& WXUNUSED(event))
{
	//update toolbar
	if(!on_exit) UpdateToolBar();
	return;
}

void BgMdiEdgeChild::ZoomWindow(void)
{
	//display zoom window
	origEdgeImage_->zoom_window	= true;
	origEdgeImage_->zoom_in		= false;
	origEdgeImage_->zoom_out		= false;
}

void BgMdiEdgeChild::ZoomIn(void)
{
	//zoom into display image
	origEdgeImage_->zoom_window	= false;
	origEdgeImage_->zoom_in		= true;
	origEdgeImage_->zoom_out		= false;
	return;
}

void BgMdiEdgeChild::ZoomOut(void)
{
	//zoom out of display image
	origEdgeImage_->zoom_window	= false;
	origEdgeImage_->zoom_in		= false;
	origEdgeImage_->zoom_out		= true;
	return;
}

void BgMdiEdgeChild::NoZoom(void)
{
	//do not zoom display image
	origEdgeImage_->zoom_window	= false;
	origEdgeImage_->zoom_in		= false;
	origEdgeImage_->zoom_out	= false;
	return;
}

void BgMdiEdgeChild::UpdateZoomControl(void)
{
	//determine whether to enable zoom in control based on maximum zoom
	if(maxZoom_ || minZoom_)
		UpdateToolBar();
	else
	{
//		toolbar->Realize();
		toolbar->EnableTool(BG_ZOOM_IN, true);
		toolbar->EnableTool(BG_ZOOM_OUT, true);
	}
}

void BgMdiEdgeChild::RunEnable(void)
{
	edButton_->Enable(true);
	wxMenuBar *menubar = GetMenuBar();
	menubar->Enable(BG_EDGE_DETECT, true);
}

void BgMdiEdgeChild::SaveEnable(void)
{
//	toolbar->Realize();
	toolbar->EnableTool(BG_SAVE_RESULT, true);
}

void BgMdiEdgeChild::UpdateToolBar(void)
{
	//update toolbar
	if(window_open)
	{
		//determine whether to enable save based on whether segmentation
		//has occurred
		bool save_enable;
		if(hasEdge_)
			save_enable	= true;
		else
			save_enable	= false;

		//determine whether to enable zoom controls based on whether image
		//has been loaded
		bool load_enable;
		if(hasImage_)
			load_enable	= true;
		else
			load_enable	= false;

		//determine whether to enable zoom in control based on maximum zoom
		bool max_zoom;
		if(maxZoom_)
			max_zoom	= true;
		else
			max_zoom	= false;

		//determine whether to enable zoom out control based on minimum zoom
		bool min_zoom;
		if(minZoom_)
			min_zoom	= true;
		else
			min_zoom	= false;

		//adjust toolbar
		toolbar->SetToolShortHelp(BG_LOAD_IMAGE, "Load image to perform edge detection");
		toolbar->SetToolShortHelp(BG_SAVE_RESULT, "Save edge map");
		toolbar->EnableTool(BG_SAVE_RESULT, save_enable);
		toolbar->EnableTool(BG_CROSS, load_enable);
		toolbar->EnableTool(BG_ZOOM_IN, ((load_enable)&&(!max_zoom)));
		toolbar->EnableTool(BG_ZOOM_OUT, ((load_enable)&&(!min_zoom)));
		toolbar->EnableTool(BG_POINTER, true);

		//set to no zoom
		toolbar->ToggleTool(BG_CROSS, false);
		toolbar->ToggleTool(BG_ZOOM_IN, false);
		toolbar->ToggleTool(BG_ZOOM_OUT, false);
		toolbar->ToggleTool(BG_POINTER, true);
		origEdgeImage_->SetCursor(wxCURSOR_ARROW);
		NoZoom();

//		toolbar->Realize();
	}
	return;
}

void BgMdiEdgeChild::ResetToolBar(void)
{
	//update toolbar
	toolbar->SetToolShortHelp(BG_LOAD_IMAGE, "Load image to process");
	toolbar->SetToolShortHelp(BG_SAVE_RESULT, "Save result");
	toolbar->EnableTool(BG_SAVE_RESULT, false);
	toolbar->EnableTool(BG_CROSS, false);
	toolbar->EnableTool(BG_ZOOM_IN, false);
	toolbar->EnableTool(BG_ZOOM_OUT, false);
	toolbar->EnableTool(BG_POINTER, false);
	toolbar->ToggleTool(BG_CROSS, false);
	toolbar->ToggleTool(BG_ZOOM_IN, false);
	toolbar->ToggleTool(BG_ZOOM_OUT, false);
	toolbar->ToggleTool(BG_POINTER, false);
//	toolbar->Realize();
	return;
}

void BgMdiEdgeChild::ReadImage(char *pathname, char *filename)
{
	plotTotImage_->ClearData(1);
	plotNmxImage_->ClearData(1);
	
	if (origEdgeImage_->SetImage(pathname) == 0)
		return;
	bgLog("Image %s loaded\n",pathname);

	//obtain and store image filename
	if(filename_)	delete [] filename_;
	filename_	= new char [strlen(filename) + 1];
	strcpy(filename_, filename);
	
	miViewOrig_->Check(TRUE);
	viewOrigCheck_->SetValue(TRUE);
	origEdgeImage_->showbitmap_ = true;
	
	if (hasEdge_ == 1)
        origEdgeImage_->RemovePointSet(&cbgPointSet_);
	// set cbgImage
	cbgImage_->SetImageFromRGB(origEdgeImage_->pimage->GetData(), origEdgeImage_->pimage->GetWidth(), origEdgeImage_->pimage->GetHeight());
	if (cbgEdgeDetect_ != 0)
		delete cbgEdgeDetect_;
	cbgEdgeDetect_ = new BgEdgeDetect(kernelSize_);
	hasImage_ = 1;
	hasEdge_ = 0;
	
	//get image dimension
	width_			= origEdgeImage_->pimage->GetWidth();
	height_			= origEdgeImage_->pimage->GetHeight();
	
	//reset the zoom level of the original edge image
	origEdgeImage_->Zoom(1);
	
	//reset max/min zoom flags
	maxZoom_	= 0;
	minZoom_	= 1;
	
	//update interface...
	
	/***********************************************/
	
	//enable run
	RunEnable();
	
	//update the tool bar
	UpdateToolBar();
	
	//set window title
	wxString statusname;
	statusname.Printf(_T("Edge Detection Frame %d - %s (%d x %d) [Original Image]"), window_number_, filename_, width_, height_);
	SetTitle(statusname);
	
	/***********************************************/
}

void BgMdiEdgeChild::OnLoadImage(wxCommandEvent& WXUNUSED(event))
{
   // get the file name
//   wxFileDialog filedialog(this,"Choose an image file","","",
//      "All files (*.*)|*.*|PNG files (*.png)|*.png|BMP files (*.bmp)|*.bmp|GIF files (*.gif)|*.gif|TIFF files (*.tif)|*.tif|JPEG files (*.jpg)|*.jpg|PNM files (*.pnm)|*.pnm",
//      wxOPEN);
#if defined(__WXGTK__) || defined(__WXMOTIF__)
  wxFileDialog filedialog(this,"Choose an image file","","",
			  "*",wxOPEN);
#else
   wxFileDialog filedialog(this,"Choose an image file","","",
      "Common image files|*.png;*.bmp;*.gif;*.tif;*.tiff;*.jpg;*.pnm;*.pgm;*.ppm|PNG files (*.png)|*.png|BMP files (*.bmp)|*.bmp|GIF files (*.gif)|*.gif|TIFF files (*.tif)|*.tif|JPEG files (*.jpg)|*.jpg|PNM files (*.pnm)|*.pnm|PGM/PPM files (*.pgm,*.ppm)|*.pgm;*.ppm",
      wxOPEN);
#endif
   if(filedialog.ShowModal()==wxID_OK)
   {
      plotTotImage_->ClearData(1);
      plotNmxImage_->ClearData(1);

      if (hasEdge_ == 1)
        origEdgeImage_->RemovePointSet(&cbgPointSet_);

      if (origEdgeImage_->SetImage(filedialog.GetPath().c_str()) == 0)
         return;
      bgLog("Image %s loaded\n",filedialog.GetPath().c_str());

	  //obtain and store image filename
	  if(filename_)	delete [] filename_;
	  filename_	= new char [strlen(filedialog.GetFilename().c_str()) + 1];
	  strcpy(filename_, filedialog.GetFilename().c_str());

      miViewOrig_->Check(TRUE);
      viewOrigCheck_->SetValue(TRUE);
      origEdgeImage_->showbitmap_ = true;

      // set cbgImage
      cbgImage_->SetImageFromRGB(origEdgeImage_->pimage->GetData(), origEdgeImage_->pimage->GetWidth(), origEdgeImage_->pimage->GetHeight());
      if (cbgEdgeDetect_ != 0)
         delete cbgEdgeDetect_;
      cbgEdgeDetect_ = new BgEdgeDetect(kernelSize_);
      hasImage_ = 1;
      hasEdge_ = 0;

	  //get image dimension
	  width_			= origEdgeImage_->pimage->GetWidth();
	  height_			= origEdgeImage_->pimage->GetHeight();

	  //reset the zoom level of the original edge image
	  origEdgeImage_->Zoom(1);

	  //reset max/min zoom flags
	  maxZoom_	= 0;
	  minZoom_	= 1;

	  //update interface...

	  /***********************************************/
	  //enable run
	  RunEnable();

	  //update the tool bar
	  UpdateToolBar();

	  //set window title
      wxString statusname;
      statusname.Printf(_T("Edge Detection Frame %d - %s (%d x %d) [Original Image]"), window_number_, filename_, width_, height_);
      SetTitle(statusname);

	  /***********************************************/
   }
}

void BgMdiEdgeChild::OnSaveEdgeMap(wxCommandEvent& WXUNUSED(event))
{
   if (hasEdge_ == 0)
   {
      bgLog("No edge map, run edge detection first!\n");
      return;
   }
   // get the file name
   wxFileDialog filedialog(this,"Choose an image file","","",
      "PGM files (*.pgm)|*.pgm",
      wxSAVE);

   if(filedialog.ShowModal()==wxID_OK)
   {
      BgImage tempImage(cbgImage_->x_, cbgImage_->y_);
      cbgEdgeList_->SetBinImage(&tempImage);
      write_pgm_image(filedialog.GetPath().c_str(), tempImage.im_, tempImage.y_, tempImage.x_, "", 255);
      bgLog("Edge map saved in: %s\n",filedialog.GetPath().c_str());

      char tch[100];
      sprintf(tch,"%s.txt",filedialog.GetPath().c_str());
      cbgEdgeList_->SaveEdgeList(tch);
   }
}

void BgMdiEdgeChild::OnEdgeDetect(wxCommandEvent& WXUNUSED(event))
{
   // determine if we have image
   if (cbgImage_->hasIm_ == false)
   {
      //no image loaded
      bgLog("No image loaded!\n");
      return;
   }
   if (hasEdge_ == 1)
      origEdgeImage_->RemovePointSet(&cbgPointSet_);
   
   if (cbgEdgeList_ != 0)
      delete cbgEdgeList_;
   cbgEdgeList_ = new BgEdgeList();

   // test if modif params
   double tx[MAX_CUSTOM_NODES];
   double ty[MAX_CUSTOM_NODES];
   int ttype,i;
   int npoints;

   if (plotTotImage_->mouseModif_ == 1)
   {
      // get new nmx params
      nmxCurve_.GetParamCurve(tx, ty, ttype,npoints);
      if ((ttype!=FC_CUSTOM) && (ttype!=-1))
      {
         nmxType_ = ttype;
         rankNmx_ = tx[0];
         confNmx_ = ty[0];
      }
      plotTotImage_->mouseModif_ = 0;
   }
   if (plotNmxImage_->mouseModif_ == 1)
   {
      // get new hyst params
      highCurve_.GetParamCurve(tx, ty, ttype,npoints);
      if ((ttype!=FC_CUSTOM) && (ttype!=-1))
      {
         hystHighType_ = ttype;
         rankH_ = tx[0];
         confH_ = ty[0];
      } else
      {
         hystHighType_ = ttype;
         for (i=0; i<npoints; i++)
         {
            custHx_[i] = tx[i];
            custHy_[i] = ty[i];
         }
         nCustH_ = npoints;
      }
      lowCurve_.GetParamCurve(tx, ty, ttype,npoints);
      if ((ttype!=FC_CUSTOM) && (ttype!=-1))
      {
         hystLowType_ = ttype;
         rankL_ = tx[0];
         confL_ = ty[0];
      } else
      {
         hystLowType_ = ttype;
         for (i=0; i<npoints; i++)
         {
            custLx_[i] = tx[i];
            custLy_[i] = ty[i];
         }
         nCustL_ = npoints;
      }
      plotNmxImage_->mouseModif_ = 0;
   }

   if (hystHighType_ == FC_CUSTOM)
      cbgEdgeDetect_->SetCustomHigh(custHx_, custHy_, nCustH_);
   if (hystLowType_ == FC_CUSTOM)
      cbgEdgeDetect_->SetCustomLow(custLx_, custLy_, nCustL_);

   // determine if we have permanent data
   if (cbgEdgeDetect_->havePerm_ == true) {
      // compute only nmx and hyst
      cbgEdgeDetect_->DoRecompute(cbgEdgeList_, rankNmx_, confNmx_, rankH_, confH_, rankL_, confL_,
         nMin_, nmxType_, hystHighType_, hystLowType_);
      // set only nmx image
      SetNmxImage();
   }
   else
   {
      // compute all steps
      cbgEdgeDetect_->DoEdgeDetect(cbgImage_, cbgEdgeList_, rankNmx_, confNmx_, rankH_, confH_, rankL_, confL_,
         nMin_, nmxType_, hystHighType_, hystLowType_);
      // set total and nmx image
      SetTotalImage();
      SetNmxImage();
   }

   // get binary edge image
   BgImage tempImage(cbgImage_->x_, cbgImage_->y_);
   cbgEdgeList_->SetBinImage(&tempImage);

   int* edgex;
   int* edgey;
   int nEdgep;
   edgex = new int[(cbgImage_->x_) * (cbgImage_->y_)];
   edgey = new int[(cbgImage_->x_) * (cbgImage_->y_)];
   cbgPointSet_.type_ = 1;

   cbgEdgeList_->GetAllEdgePoints(edgex, edgey, &nEdgep);
   cbgPointSet_.SetPoints(edgex, edgey, nEdgep);
   hasEdge_ = 1;
   delete [] edgey;
   delete [] edgex;

   // update image canvas
   if (miViewEdge_->IsChecked())
      origEdgeImage_->AddPointSet(&cbgPointSet_);

   //active save tool
   SaveEnable();

   //update menu bar
   wxMenuBar *menubar = GetMenuBar();
   menubar->Enable(BG_EDGE_SAVE_MAP, true);

}

void BgMdiEdgeChild::SetTotalImage(void)
{
   unsigned char* buf;
   int xsz = RANK_CONF_IMSIZEX;
   int ysz = RANK_CONF_IMSIZEY;
   int imsz = xsz*ysz;
   buf = new unsigned char[imsz];
   int i;
   for (i=0; i<imsz; i++)
      buf[i] = 255;

   int l, c;
   int xo = cbgEdgeDetect_->x_;
   int yo = cbgEdgeDetect_->y_;
   float* rank;
   float* conf;
   rank = cbgEdgeDetect_->permRank_;
   conf = cbgEdgeDetect_->permConf_;
   for (i=0; i<xo*yo; i++)
   {
      if (rank[i]>0 && conf[i]>0)
      {
         c = (int) (rank[i]*((double) xsz));
         c = (c>=xsz) ? xsz-1 : c;
         l = (int) (conf[i]*((double) ysz));
         l = (l>=ysz) ? ysz-1 : l;
         l = ysz-1-l;
         buf[c+l*xsz]=80;
      }
   }

   nmxCurve_.SetParamCurve(nmxType_, &rankNmx_, &confNmx_, 1, xsz, ysz);
   plotTotImage_->SetImageFromGray(buf, xsz, ysz);
   plotTotImage_->FillCurveClick();

   delete [] buf;

}

void BgMdiEdgeChild::SetNmxImage(void)
{
   unsigned char* buf;
   int xsz = RANK_CONF_IMSIZEX;
   int ysz = RANK_CONF_IMSIZEY;
   int imsz = xsz*ysz;
   buf = new unsigned char[imsz];
   int i;
   for (i=0; i<imsz; i++)
      buf[i] = 255;

   int l, c;
   int xo = cbgEdgeDetect_->x_;
   int yo = cbgEdgeDetect_->y_;
   float* rank;
   float* conf;
   rank = cbgEdgeDetect_->permNmxRank_;
   conf = cbgEdgeDetect_->permNmxConf_;
   for (i=0; i<xo*yo; i++)
   {
      if (rank[i]>0 && conf[i]>0)
      {
         c = (int) (rank[i]*((double) xsz));
         c = (c>=xsz) ? xsz-1 : c;
         l = (int) (conf[i]*((double) ysz));
         l = (l>=ysz) ? ysz-1 : l;
         l = ysz-1-l;
         buf[c+l*xsz]=80;
      }
   }
   
   if (hystHighType_ != FC_CUSTOM)
      highCurve_.SetParamCurve(hystHighType_, &rankH_, &confH_, 1, xsz, ysz);
   else 
   {
      highCurve_.SetParamCurve(hystHighType_, custHx_, custHy_, nCustH_, xsz, ysz);
   }
   if (hystLowType_ != FC_CUSTOM)
      lowCurve_.SetParamCurve(hystLowType_, &rankL_, &confL_, 1, xsz, ysz);
   else 
   {
      lowCurve_.SetParamCurve(hystLowType_, custLx_, custLy_, nCustL_, xsz, ysz);
   }
   plotNmxImage_->SetImageFromGray(buf, xsz, ysz);
   plotNmxImage_->FillCurveClick();

   delete [] buf;

}

void BgMdiEdgeChild::OnChangeParam(wxCommandEvent& WXUNUSED(event))
{
   // show the parameters window and change it
   double tx[20];
   double ty[20];
   int ttype,i;
   int npoints;
   if (plotTotImage_->mouseModif_ == 1)
   {
      // get new nmx params
      nmxCurve_.GetParamCurve(tx, ty, ttype,npoints);
      if ((ttype!=FC_CUSTOM) && (ttype!=-1))
      {
         nmxType_ = ttype;
         rankNmx_ = tx[0];
         confNmx_ = ty[0];
      }
      plotTotImage_->mouseModif_ = 0;
   }
   if (plotNmxImage_->mouseModif_ == 1)
   {
      // get new hyst params
      highCurve_.GetParamCurve(tx, ty, ttype,npoints);
      if ((ttype!=FC_CUSTOM) && (ttype!=-1))
      {
         hystHighType_ = ttype;
         rankH_ = tx[0];
         confH_ = ty[0];
      } else
      {
         hystHighType_ = ttype;
         for (i=0; i<npoints; i++)
         {
            custHx_[i] = tx[i];
            custHy_[i] = ty[i];
         }
         rankH_ = tx[npoints-1];
         confH_ = ty[0];
         nCustH_ = npoints;
      }
      lowCurve_.GetParamCurve(tx, ty, ttype,npoints);
      if ((ttype!=FC_CUSTOM) && (ttype!=-1))
      {
         hystLowType_ = ttype;
         rankL_ = tx[0];
         confL_ = ty[0];
      } else
      {
         hystLowType_ = ttype;
         for (i=0; i<npoints; i++)
         {
            custLx_[i] = tx[i];
            custLy_[i] = ty[i];
         }
         rankL_ = tx[npoints-1];
         confL_ = ty[0];
         nCustL_ = npoints;
      }
      plotNmxImage_->mouseModif_ = 0;
   }
   BgParamDialog paramDialog(this, -1, "Change Parameters", wxDefaultPosition, wxSize(250,510),
      wxDEFAULT_DIALOG_STYLE | wxDIALOG_MODAL);
   paramDialog.SetValues(rankNmx_, confNmx_, rankH_, confH_, rankL_, confL_, nMin_,
      nmxType_, hystHighType_, hystLowType_, kernelSize_);
   if (paramDialog.ShowModal()==wxID_OK)
   {
      // do change param stuff
      int tempKernelSize;
      tempKernelSize = kernelSize_;
      paramDialog.GetValues(rankNmx_, confNmx_, rankH_, confH_, rankL_, confL_, nMin_,
         nmxType_, hystHighType_, hystLowType_, tempKernelSize);
      if (tempKernelSize != kernelSize_)
      {
         kernelSize_ = tempKernelSize;
         if (cbgEdgeDetect_ != 0) {
            delete cbgEdgeDetect_;
            cbgEdgeDetect_ = new BgEdgeDetect(kernelSize_);
         }
      }

      // change image param
      int xsz = RANK_CONF_IMSIZEX;
      int ysz = RANK_CONF_IMSIZEY;
      
      if (hystHighType_ != FC_CUSTOM)
         highCurve_.SetParamCurve(hystHighType_, &rankH_, &confH_, 1, xsz, ysz);
      else 
      {
         highCurve_.SetParamCurve(hystHighType_, custHx_, custHy_, nCustH_, xsz, ysz);
      }
      if (hystLowType_ != FC_CUSTOM)
         lowCurve_.SetParamCurve(hystLowType_, &rankL_, &confL_, 1, xsz, ysz);
      else 
      {
         lowCurve_.SetParamCurve(hystLowType_, custLx_, custLy_, nCustL_, xsz, ysz);
      }

      nmxCurve_.SetParamCurve(nmxType_, &rankNmx_, &confNmx_, 1, xsz, ysz);

      plotNmxImage_->FillCurveClick();
      plotTotImage_->FillCurveClick();

      SetParametersStr();
      SetParametersNum();
   }
}


// ---------------------------------------------------------------------------
// BgParamDialog
// ---------------------------------------------------------------------------

BgParamDialog::BgParamDialog(wxWindow* parent, wxWindowID id, const wxString& title,
      const wxPoint& pos, const wxSize& size,
      long style, const wxString& name)
: wxDialog(parent, id, title, pos, size, style, name)
{
   okButton_ = new wxButton(this, BG_PARAMD_OK, "Ok", wxPoint(20+C_PARAMX+10,450));
   cancelButton_ = new wxButton(this, BG_PARAMD_CANCEL, "Cancel", wxPoint(20+C_PARAMX+10+C_PARAMDX+60,450));
   
   /*
   txtNmxR_ = new wxStaticText(this, -1, "Nmx. rank: ", wxPoint(C_PARAMX,C_PARAMY+0*C_PARAMDY));
   txtNmxC_ = new wxStaticText(this, -1, "Nmx. conf: ", wxPoint(C_PARAMX,C_PARAMY+1*C_PARAMDY));
   txtHHR_ = new wxStaticText(this, -1, "Hyst. high rank: ", wxPoint(C_PARAMX,C_PARAMY+2*C_PARAMDY));
   txtHHC_ = new wxStaticText(this, -1, "Hyst. high conf: ", wxPoint(C_PARAMX,C_PARAMY+3*C_PARAMDY));
   txtHLR_ = new wxStaticText(this, -1, "Hyst. low rank: ", wxPoint(C_PARAMX,C_PARAMY+4*C_PARAMDY));
   txtHLC_ = new wxStaticText(this, -1, "Hyst. low conf: ", wxPoint(C_PARAMX,C_PARAMY+5*C_PARAMDY));
   txtMinPt_ = new wxStaticText(this, -1, "Min. points: ", wxPoint(C_PARAMX,C_PARAMY+6*C_PARAMDY));
   txtNmxType_ = new wxStaticText(this, -1, "Nmx. type: ", wxPoint(C_PARAMX,C_PARAMY+7*C_PARAMDY));
   txtHHType_ = new wxStaticText(this, -1, "Hyst. high type: ", wxPoint(C_PARAMX,C_PARAMY+8*C_PARAMDY));
   txtHLType_ = new wxStaticText(this, -1, "Hyst. low: ", wxPoint(C_PARAMX,C_PARAMY+9*C_PARAMDY));
   txtKernelSize_ = new wxStaticText(this, -1, "Kernel radius: ", wxPoint(C_PARAMX,C_PARAMY+10*C_PARAMDY));

   valNmxR_ = new wxTextCtrl(this, -1, "NA     ", wxPoint(C_PARAMX+C_PARAMDX,C_PARAMY+0*C_PARAMDY));
   valNmxC_ = new wxTextCtrl(this, -1, "NA     ", wxPoint(C_PARAMX+C_PARAMDX,C_PARAMY+1*C_PARAMDY));
   valHHR_ = new wxTextCtrl(this, -1, "NA     ", wxPoint(C_PARAMX+C_PARAMDX,C_PARAMY+2*C_PARAMDY));
   valHHC_ = new wxTextCtrl(this, -1, "NA     ", wxPoint(C_PARAMX+C_PARAMDX,C_PARAMY+3*C_PARAMDY));
   valHLR_ = new wxTextCtrl(this, -1, "NA     ", wxPoint(C_PARAMX+C_PARAMDX,C_PARAMY+4*C_PARAMDY));
   valHLC_ = new wxTextCtrl(this, -1, "NA     ", wxPoint(C_PARAMX+C_PARAMDX,C_PARAMY+5*C_PARAMDY));
   valMinPt_ = new wxTextCtrl(this, -1, "NA     ", wxPoint(C_PARAMX+C_PARAMDX,C_PARAMY+6*C_PARAMDY));
   valNmxType_ = new wxChoice(this, -1, wxPoint(C_PARAMX+C_PARAMDX,C_PARAMY+7*C_PARAMDY), wxDefaultSize);
   valHHType_ = new wxChoice(this, -1, wxPoint(C_PARAMX+C_PARAMDX,C_PARAMY+8*C_PARAMDY), wxDefaultSize);
   valHLType_ = new wxChoice(this, -1, wxPoint(C_PARAMX+C_PARAMDX,C_PARAMY+9*C_PARAMDY), wxDefaultSize);
   valKernelSize_ = new wxTextCtrl(this, -1, "NA     ", wxPoint(C_PARAMX+C_PARAMDX,C_PARAMY+10*C_PARAMDY));
   */

   txtKernelSize_ = new wxStaticText(this, -1, "Grad Win.", wxPoint(20+C_PARAMX-5,0+C_PARAMY+0*C_PARAMDY));
   valKernelSize_ = new wxTextCtrl(this, -1, "NA     ", wxPoint(20+C_PARAMX+C_PARAMDX+35,0+C_PARAMY+0*C_PARAMDY), wxSize(C_PARAMSX-20,C_PARAMSY));

   txtMinPt_ = new wxStaticText(this, -1, "Min. length", wxPoint(20+C_PARAMX-5,0+C_PARAMY+1*C_PARAMDY));
   valMinPt_ = new wxTextCtrl(this, -1, "NA     ", wxPoint(20+C_PARAMX+C_PARAMDX+35,0+C_PARAMY+1*C_PARAMDY), wxSize(C_PARAMSX-20,C_PARAMSY));

   wxStaticBox* nmxSB = new wxStaticBox(this, -1, "Nonmaxima supp.", wxPoint(20+5, 80+0), wxSize(140,4*C_PARAMDY-5));
   txtNmxType_ = new wxStaticText(this, -1, "Type", wxPoint(20+C_PARAMX, 80+C_PARAMY+0*C_PARAMDY));
   valNmxType_ = new wxChoice(this, -1, wxPoint(20+C_PARAMX+C_PARAMDX,80+C_PARAMY+0*C_PARAMDY), wxSize(C_PARAMSX,C_PARAMSY));
   txtNmxR_ = new wxStaticText(this, -1, "Rank", wxPoint(20+C_PARAMX,80+C_PARAMY+1*C_PARAMDY));
   valNmxR_ = new wxTextCtrl(this, -1, "NA     ", wxPoint(20+C_PARAMX+C_PARAMDX,80+C_PARAMY+1*C_PARAMDY), wxSize(C_PARAMSX,C_PARAMSY));
   txtNmxC_ = new wxStaticText(this, -1, "Conf", wxPoint(20+C_PARAMX,80+C_PARAMY+2*C_PARAMDY));
   valNmxC_ = new wxTextCtrl(this, -1, "NA     ", wxPoint(20+C_PARAMX+C_PARAMDX,80+C_PARAMY+2*C_PARAMDY), wxSize(C_PARAMSX,C_PARAMSY));

   wxStaticBox* hhSB = new wxStaticBox(this, -1, "Hyst. High Tr.", wxPoint(20+5, 200+0), wxSize(140,4*C_PARAMDY-5));
   txtHHType_ = new wxStaticText(this, -1, "Type", wxPoint(20+C_PARAMX,200+C_PARAMY+0*C_PARAMDY));
   txtHHR_ = new wxStaticText(this, -1, "Rank", wxPoint(20+C_PARAMX,200+C_PARAMY+1*C_PARAMDY));
   txtHHC_ = new wxStaticText(this, -1, "Conf", wxPoint(20+C_PARAMX,200+C_PARAMY+2*C_PARAMDY));
   valHHType_ = new wxChoice(this, -1, wxPoint(20+C_PARAMX+C_PARAMDX,200+C_PARAMY+0*C_PARAMDY), wxSize(C_PARAMSX,C_PARAMSY));
   valHHR_ = new wxTextCtrl(this, -1, "NA     ", wxPoint(20+C_PARAMX+C_PARAMDX,200+C_PARAMY+1*C_PARAMDY), wxSize(C_PARAMSX,C_PARAMSY));
   valHHC_ = new wxTextCtrl(this, -1, "NA     ", wxPoint(20+C_PARAMX+C_PARAMDX,200+C_PARAMY+2*C_PARAMDY), wxSize(C_PARAMSX,C_PARAMSY));

   wxStaticBox* hlSB = new wxStaticBox(this, -1, "Hyst. Low Tr.", wxPoint(20+5, 320+0), wxSize(140, 4*C_PARAMDY-5));
   txtHLType_ = new wxStaticText(this, -1, "Type", wxPoint(20+C_PARAMX,320+C_PARAMY+0*C_PARAMDY));
   txtHLR_ = new wxStaticText(this, -1, "Rank", wxPoint(20+C_PARAMX,320+C_PARAMY+1*C_PARAMDY));
   txtHLC_ = new wxStaticText(this, -1, "Conf", wxPoint(20+C_PARAMX,320+C_PARAMY+2*C_PARAMDY));
   valHLType_ = new wxChoice(this, -1, wxPoint(20+C_PARAMX+C_PARAMDX,320+C_PARAMY+0*C_PARAMDY), wxSize(C_PARAMSX,C_PARAMSY));
   valHLR_ = new wxTextCtrl(this, -1, "NA     ", wxPoint(20+C_PARAMX+C_PARAMDX,320+C_PARAMY+1*C_PARAMDY), wxSize(C_PARAMSX,C_PARAMSY));
   valHLC_ = new wxTextCtrl(this, -1, "NA     ", wxPoint(20+C_PARAMX+C_PARAMDX,320+C_PARAMY+2*C_PARAMDY), wxSize(C_PARAMSX,C_PARAMSY));



   // put choices
   valNmxType_->Append("Arc");
   valNmxType_->Append("Vertical Line");
   valNmxType_->Append("Horizontal Line");
   valNmxType_->Append("Line");
   valNmxType_->Append("Box");

   valHHType_->Append("Arc");
   valHHType_->Append("Vertical Line");
   valHHType_->Append("Horizontal Line");
   valHHType_->Append("Line");
   valHHType_->Append("Box");
   valHHType_->Append("Custom");

   valHLType_->Append("Arc");
   valHLType_->Append("Vertical Line");
   valHLType_->Append("Horizontal Line");
   valHLType_->Append("Line");
   valHLType_->Append("Box");
   valHLType_->Append("Custom");

}

BgParamDialog::~BgParamDialog()
{
   delete txtNmxR_;
   delete txtNmxC_;
   delete txtHHR_;
   delete txtHHC_;
   delete txtHLR_;
   delete txtHLC_;
   delete txtMinPt_;
   delete txtNmxType_;
   delete txtHHType_;
   delete txtHLType_;
   delete txtKernelSize_;

   delete valNmxR_;
   delete valNmxC_;
   delete valHHR_;
   delete valHHC_;
   delete valHLR_;
   delete valHLC_;
   delete valMinPt_;
   delete valNmxType_;
   delete valHHType_;
   delete valHLType_;
   delete valKernelSize_;

   delete okButton_;
   delete cancelButton_;
}

void BgParamDialog::OnOk(wxCommandEvent& WXUNUSED(event))
{
   EndModal(wxID_OK);
}

void BgParamDialog::OnCancel(wxCommandEvent& WXUNUSED(event))
{
   EndModal(wxID_CANCEL);
}

void BgParamDialog::SetValues(double nmxR, double nmxC, double hhR, double hhC, double hlR, double hlC,
                              int nMin, int nmxT, int hhT, int hlT, int ks)
{
   wxString ts;
   ts = wxString::Format("%.3g", nmxR);
   valNmxR_->SetValue(ts);
   ts = wxString::Format("%.3g", nmxC);
   valNmxC_->SetValue(ts);
   ts = wxString::Format("%.3g", hhR);
   valHHR_->SetValue(ts);
   ts = wxString::Format("%.3g", hhC);
   valHHC_->SetValue(ts);
   ts = wxString::Format("%.3g", hlR);
   valHLR_->SetValue(ts);
   ts = wxString::Format("%.3g", hlC);
   valHLC_->SetValue(ts);
   ts = wxString::Format("%d", nMin);
   valMinPt_->SetValue(ts);
   valNmxType_->SetSelection(nmxT);
   valHHType_->SetSelection(hhT);
   valHLType_->SetSelection(hlT);
   ts = wxString::Format("%d", ks);
   valKernelSize_->SetValue(ts);

}

void BgParamDialog::GetValues(double& nmxR, double& nmxC, double& hhR, double& hhC, double& hlR, double& hlC,
                              int& nMin, int& nmxT, int& hhT, int& hlT, int& ks)
{
   double td;
   long tl;
   int ti;

   td = -1;
   tl = -1;
   ti = -1;
   if ((valNmxR_->GetValue().ToDouble(&td) == TRUE) && (td>=0))
      nmxR = td;
   else
      bgLog("nmx. rank value out of range.\n");
   
   if ((valNmxC_->GetValue().ToDouble(&td) == TRUE) && (td>=0))
      nmxC = td;
   else
      bgLog("nmx. conf. value out of range.\n");

   if ((valHHR_->GetValue().ToDouble(&td) == TRUE) && (td>=0))
      hhR = td;
   else
      bgLog("hyst. high rank value out of range.\n");

   if ((valHHC_->GetValue().ToDouble(&td) == TRUE) && (td>=0))
      hhC = td;
   else
      bgLog("hyst. high conf. value out of range.\n");

   if ((valHLR_->GetValue().ToDouble(&td) == TRUE) && (td>=0))
      hlR = td;
   else
      bgLog("hyst. low rank value out of range.\n");

   if ((valHLC_->GetValue().ToDouble(&td) == TRUE) && (td>=0))
      hlC = td;
   else
      bgLog("hyst. low conf. value out of range.\n");

   if ((valMinPt_->GetValue().ToLong(&tl) == TRUE) && (tl>=0))
      nMin = (int) tl;
   else
      bgLog("min. edge points value out of range.\n");

   if ((ti=valNmxType_->GetSelection()) != -1)
      nmxT = ti;

   if ((ti=valHHType_->GetSelection()) != -1)
      hhT = ti;

   if ((ti=valHLType_->GetSelection()) != -1)
      hlT = ti;

   if ((valKernelSize_->GetValue().ToLong(&tl) == TRUE) && (tl>0) && ((2*tl+1)<=MAX_FILTS))
      ks = (int) tl;
   else
      bgLog("kernel radius value out of range.\n");


}

// ---------------------------------------------------------------------------
// BgSpeedSelect
// ---------------------------------------------------------------------------

BgSpeedSelect::BgSpeedSelect(wxWindow* parent, wxWindowID id, const wxString& title,
      const wxPoint& pos, const wxSize& size,
      long style, const wxString& name)
: wxDialog(parent, id, title, pos, size, style, name)
{
   okButton_ = new wxButton(this, BG_SPEEDSEL_OK, "Ok", wxPoint(30,80));
   cancelButton_ = new wxButton(this, BG_SPEEDSEL_CANCEL, "Cancel", wxPoint(120,80));
   
   txtQuality_ = new wxStaticText(this, -1, "Speed", wxPoint(160, 10));
   txtSpeed_ = new wxStaticText(this, -1, "Quality", wxPoint(40, 10));

   sldSpeed_ = new wxSlider(this, BG_SPEEDSEL_SLD, 0, 0, 100, wxPoint(18,40), wxSize(155,-1),
                             wxSL_AUTOTICKS | wxSL_LABELS);
   sldSpeed_->SetTickFreq(20, 0);
}

BgSpeedSelect::~BgSpeedSelect()
{
   delete sldSpeed_;

   delete okButton_;
   delete cancelButton_;
}

void BgSpeedSelect::OnOk(wxCommandEvent& WXUNUSED(event))
{
   EndModal(wxID_OK);
}

void BgSpeedSelect::OnCancel(wxCommandEvent& WXUNUSED(event))
{
   EndModal(wxID_CANCEL);
}

void BgSpeedSelect::SetSliderValue(float sliderV)
{
   sldSpeed_->SetValue((int) (sliderV*100));
}

void BgSpeedSelect::GetSliderValue(float& sliderV)
{
   sliderV = (float) (sldSpeed_->GetValue() / 100.0);
}

// ---------------------------------------------------------------------------
// BgMdiSegmentChild
// ---------------------------------------------------------------------------

BgMdiSegmentChild::BgMdiSegmentChild(wxMDIParentFrame *parent, const wxString& title,
                           const wxPoint& pos, const wxSize& size,
                           const long style)
                           : wxMDIChildFrame(parent, BG_SEGM_WINDOW, title, pos, size, style)
{

   //set window number
   window_number_ = gs_nFrames;

   //assume image is not yet loaded into segmentation window
   filename_	= NULL;

   //split window to display the segmented image on the left hand side,
   //and the ph diagram on the right hand side
   imagePlotSplitter_	= new wxSplitterWindow(this, -1, wxDefaultPosition, wxDefaultSize, wxSP_NOBORDER | wxSP_3DSASH);
   plotMapSplitter_		= new wxSplitterWindow(imagePlotSplitter_, -1, wxDefaultPosition, wxDefaultSize, wxSP_NOBORDER | wxSP_3DSASH);
   mapSplitter_			= new wxSplitterWindow(plotMapSplitter_, -1, wxDefaultPosition, wxDefaultSize, wxSP_NOBORDER | wxSP_3DSASH);

   //create option panels
   winPanel1_ = new BgMenuPanel(mapSplitter_, -1, BG_CANVAS_VIEW1_GRADMAP, BG_CANVAS_VIEW1_CONFMAP, BG_CANVAS_VIEW1_WEITMAP, BG_CANVAS_VIEW1_CUSTMAP,
								                  BG_CANVAS_SAVE_GRADMAP, BG_CANVAS_SAVE_CONFMAP, BG_CANVAS_SAVE_WEITMAP);
   winPanel2_ = new BgMenuPanel(mapSplitter_, -1, BG_CANVAS_VIEW2_GRADMAP, BG_CANVAS_VIEW2_CONFMAP, BG_CANVAS_VIEW2_WEITMAP, BG_CANVAS_VIEW2_CUSTMAP,
								                  BG_CANVAS_SAVE_GRADMAP, BG_CANVAS_SAVE_CONFMAP, BG_CANVAS_SAVE_WEITMAP);
   winPanel1_->view_menu->Enable(BG_CANVAS_VIEW1_CUSTMAP, false);
   winPanel2_->view_menu->Enable(BG_CANVAS_VIEW2_CUSTMAP, false);
   winPanel1_->CheckViewItem(BG_CANVAS_VIEW1_CONFMAP);
   winPanel2_->CheckViewItem(BG_CANVAS_VIEW2_GRADMAP);
   winPanel1_->EnableMenu(false);
   winPanel2_->EnableMenu(false);

   //define ph diagram and segmented display image
   phDiagram_ = new BgImCanvas(this, plotMapSplitter_, wxDefaultPosition, wxDefaultSize);
   phDiagram_->SetScrollbars(20, 20, 50, 50);
   plotWindow1_ = new BgImCanvas(this, winPanel1_, wxPoint(0,PLOT_MENU_HEIGHT), wxDefaultSize);
   plotWindow1_->SetScrollbars(20, 20, 50, 50);
   plotWindow2_ = new BgImCanvas(this, winPanel2_, wxPoint(0,PLOT_MENU_HEIGHT), wxDefaultSize);
   plotWindow2_->SetScrollbars(20, 20, 50, 50);
   displayImage_ = new BgImCanvas(this, imagePlotSplitter_, wxDefaultPosition, wxDefaultSize);
   displayImage_->SetScrollbars(20, 20, 50, 50);

   //set the scroll window of each panel
   winPanel1_->SetScrollWindow((wxWindow *) plotWindow1_);
   winPanel2_->SetScrollWindow((wxWindow *) plotWindow2_);

   //set lower bound zoom limit on display image
   //to the size of the original image
   displayImage_->SetMinZoomLevel(1);

   //place each image object into their corresponding positions in the split
   //window
   imagePlotSplitter_->SplitVertically(displayImage_, plotMapSplitter_,256);
   plotMapSplitter_->SplitHorizontally(phDiagram_, mapSplitter_, 256);
   mapSplitter_->SplitVertically(winPanel1_, winPanel2_, 256);

   //inititalize segmentation parameters
   sigmaS		= 7;
   sigmaR		= float(6.5);
   aij			= float(0.3);
   epsilon		= float(0.3);
   minRegion	= 20;
   kernelSize	= 2;
	
   //set text size
   wxSize txtSize(50, 20);

   //allocate memory for display images
   cbgImage_			= new BgImage();
   filtImage_			= new BgImage();
   segmImage_			= new BgImage();
   whiteImage_			= new BgImage();

   //allocate memory to store boundary pixel locations
   boundaries_			= new BgPointSet();

   hasImage_		= 0;
   hasFilter_		= 0;
   hasSegment_		= 0;
   hasBoundaries_	= 0;

   //shut off text box monitoring
   checkTextBoxes_	= false;

   optionsPanel_	= new wxPanel(this, -1, wxPoint(0, 0), wxSize(BG_SEGM_OP_SIZEX, BG_SEGM_OP_SIZEY));

   wxBoxSizer	*toplayout = new wxBoxSizer(wxVERTICAL);

   loadButton_ = new wxButton(optionsPanel_, BG_SEGM_LOAD_IMAGE, "Load Image");
   toplayout->Add(loadButton_, 0, wxCENTER | wxBOTTOM | wxTOP, 5);
   
   wxString operations[] = {"Segment", "Filter Only", "Fusion Only"};
   operationRadio_ = new wxRadioBox(optionsPanel_, BG_SEGM_OPERATION, "Operation", wxDefaultPosition, wxSize(BG_SP_WIDTH, -1), 3, operations, 3 ,wxRA_SPECIFY_ROWS);
   toplayout->Add(operationRadio_, 0, wxCENTER | wxBOTTOM, 5);

   runButton_ = new wxButton(optionsPanel_, BG_SEGM_SEGMENT, "Run");
   toplayout->Add(runButton_, 0, wxCENTER | wxBOTTOM, 5);

   wxString choices[] = {"Original", "Filtered", "Segmented", "No Image"};
   viewImSegRadio_ = new wxRadioBox(optionsPanel_, BG_SEGM_VIEW_IMSEG, "View Image", wxDefaultPosition, wxSize(BG_SP_WIDTH, -1), 4, choices, 4, wxRA_SPECIFY_ROWS);
   toplayout->Add(viewImSegRadio_, 0, wxCENTER | wxBOTTOM, 5);

   viewBoundariesCheck_ = new wxCheckBox(optionsPanel_, BG_SEGM_VIEW_EDGES, "Overlay Boundaries");
   toplayout->Add(viewBoundariesCheck_, 0, wxALIGN_LEFT | wxLEFT | wxBOTTOM, 10);

   //add bandwidth parameters...
   subPanel1_		= new wxPanel(optionsPanel_, -1, wxDefaultPosition, wxSize(BG_SP_WIDTH, BG_SP_HEIGHT));
   subPanelBox1_	= new wxStaticBox(subPanel1_, -1, "Bandwidth", wxPoint(0, 0), wxSize(BG_SP_WIDTH, BG_SP_TOP_HEIGHT), wxSIMPLE_BORDER, "staticBox");
   subPanelBox2_	= new wxStaticBox(subPanel1_, -1, "", wxPoint(0, BG_SP_TOP_HEIGHT-8), wxSize(BG_SP_WIDTH, BG_SP_HEIGHT-BG_SP_TOP_HEIGHT), wxSIMPLE_BORDER, "staticBox");
   textSigmaS_		= new wxStaticText(subPanel1_, -1, "Spatial [2h+1]", wxPoint(BG_LEFT_CELL, 18));
   txtSigmaS_		= new wxTextCtrl(subPanel1_, BG_SEGM_TEXT_SIGMAS, "7", wxPoint(BG_RIGHT_CELL, 15), txtSize);
   textSigmaR_		= new wxStaticText(subPanel1_, -1, "Color [2h]", wxPoint(BG_LEFT_CELL, 43));
   txtSigmaR_		= new wxTextCtrl(subPanel1_, BG_SEGM_TEXT_SIGMAR, "6.5", wxPoint(BG_RIGHT_CELL, 40), txtSize);
   textMinRegion_	= new wxStaticText(subPanel1_, -1, "Minimum Region", wxPoint(BG_LEFT_CELL, 78));
   txtMinRegion_	= new wxTextCtrl(subPanel1_, BG_SEGM_TEXT_MINREG, "20", wxPoint(BG_RIGHT_CELL, 75), txtSize);
   toplayout->Add(subPanel1_, 0, wxCENTER | wxBOTTOM | wxTOP, 5);

   useWeightMap_ = new wxCheckBox(optionsPanel_, BG_SEGM_USE_EDGE_MAP, "Use Weight Map");
   toplayout->Add(useWeightMap_, 0, wxALIGN_LEFT | wxLEFT | wxBOTTOM, 10);

   //add weight map parameters...
   subPanel2_		= new wxPanel(optionsPanel_, -1, wxDefaultPosition, wxSize(BG_SP_WIDTH, BG_SP_HEIGHT_2));
   subPanelBox3_	= new wxStaticBox(subPanel2_, -1, "", wxPoint(0, 0), wxSize(BG_SP_WIDTH, BG_SP_HEIGHT_2), wxSIMPLE_BORDER, "staticBox");
   textKernelSize_	= new wxStaticText(subPanel2_, -1, "Grad. Window (2n+1)", wxPoint(BG_LEFT_CELL, 18));
   txtKernelSize_	= new wxTextCtrl(subPanel2_, BG_SEGM_TEXT_GRADWIN, "2", wxPoint(BG_RIGHT_CELL, 15), txtSize);
   textA_			= new wxStaticText(subPanel2_, -1, "Mixture Parameter", wxPoint(BG_LEFT_CELL, 43));
   txtA_			= new wxTextCtrl(subPanel2_, BG_SEGM_TEXT_AIJ, "0.3", wxPoint(BG_RIGHT_CELL, 40), txtSize);
   textEpsilon_		= new wxStaticText(subPanel2_, -1, "Threshold", wxPoint(BG_LEFT_CELL, 68));
   txtEpsilon_		= new wxTextCtrl(subPanel2_, BG_SEGM_TEXT_EPSILON, "0.3", wxPoint(BG_RIGHT_CELL, 65), txtSize);
   toplayout->Add(subPanel2_, 0, wxALIGN_CENTER_HORIZONTAL | wxALIGN_TOP, 5);

   //add parameter history control
   subPanel3_		= new wxPanel(optionsPanel_, -1, wxDefaultPosition, wxSize(BG_SP_WIDTH, 43));
   subPanelBox4_	= new wxStaticBox(subPanel3_, -1, "", wxPoint(0, 0), wxSize(BG_SP_WIDTH, 43), wxSIMPLE_BORDER, "staticBox");
   textParamBox_	= new wxStaticText(subPanel3_, -1, "History", wxPoint(BG_LEFT_CELL, 18));
   paramComboBox_	= new BgParameterHistoryBox(subPanel3_, BG_SEGM_CHANGE_PARAMS, "", wxPoint(BG_RIGHT_CELL-50, 15), wxSize(100,20), 5, 0, wxDefaultValidator, "comboBox");
   toplayout->Add(subPanel3_, 0, wxALIGN_CENTER | wxCENTER | wxBOTTOM, 10);
    
   optionsPanel_->SetAutoLayout(TRUE);
   toplayout->Fit(optionsPanel_);
   optionsPanel_->SetSizer(toplayout);

   g_children.Append(this);

   //set the size and sash position of the splitters
   int w, h;
   GetClientSize(&w, &h);
   imagePlotSplitter_->SetSize(BG_SEGM_OP_SIZEX, 0, w-BG_SEGM_OP_SIZEX, h);
   imagePlotSplitter_->SetSashPosition((w-BG_SEGM_OP_SIZEX)/2, TRUE);
   plotMapSplitter_->SetSashPosition(h/2, TRUE);
   mapSplitter_->SetSashPosition((w-BG_SEGM_OP_SIZEX)/4, TRUE);

   //initalize edge maps...
   customMap_	= (float *) NULL;
   confMap_		= (float *) NULL;
   gradMap_		= (float *) NULL;
   weightMap_	= (float *) NULL;

   //indicate that the edge parameters have not changed
   edgeParamsHaveChanged_	= false;

   //add margin to plots
   phDiagram_->AddMargin(RANK_CONF_MARGINX,RANK_CONF_MARGINY);
   plotWindow1_->AddMargin(RANK_CONF_MARGINX,RANK_CONF_MARGINY);
   plotWindow2_->AddMargin(RANK_CONF_MARGINX,RANK_CONF_MARGINY);

   //add title to each individual plot
   BgText bgText(1,"      Diagram of Region Boundary Data Points", *wxSWISS_FONT, RANK_CONF_MARGINX - 10, RANK_CONF_MARGINY/2-10);
   phDiagram_->AddText(&bgText);
   bgText.SetText("Confidence Map");
   plotWindow1_->AddText(&bgText);
   bgText.SetText("Gradient Map");
   plotWindow2_->AddText(&bgText);

   //add x and y axis
   phDiagram_->AddHorizontalAxis(RANK_CONF_MARGINX-2,RANK_CONF_IMSIZEY+RANK_CONF_MARGINY,RANK_CONF_IMSIZEX+2,10,0.0,1.0);
   phDiagram_->AddVerticalAxis  (RANK_CONF_MARGINX-1,RANK_CONF_IMSIZEY+RANK_CONF_MARGINY,RANK_CONF_IMSIZEX,10,0.0,1.0);

   //label x and y axis
   bgText.SetText("Rank (   )");
   phDiagram_->LabelHorizontalAxis(&bgText);
   bgText.SetText("Confidence (  )");
   phDiagram_->LabelVerticalAxis(&bgText);

   //place greek symbols
   wxBitmap	ro("ro", wxBITMAP_TYPE_RESOURCE), eta("eta", wxBITMAP_TYPE_RESOURCE);
   BgBitmap	ro_bmp(&ro, 1, RANK_CONF_MARGINX - 10, RANK_CONF_MARGINY/2 - 3);
   BgBitmap eta_bmp(&eta, 2, RANK_CONF_MARGINX, RANK_CONF_MARGINY/2 - 3);
   phDiagram_->AddBitmap(&ro_bmp);
   phDiagram_->AddBitmap(&eta_bmp);
   ro_bmp.SetPlotLocation(RANK_CONF_MARGINX+137,RANK_CONF_IMSIZEY+RANK_CONF_MARGINY+35);
   ro_bmp.SetId(3);
   phDiagram_->AddBitmap(&ro_bmp);
   wxBitmap rotated_eta("rotated_eta", wxBITMAP_TYPE_RESOURCE);
   BgBitmap rotated_eta_bmp(&rotated_eta, 4, RANK_CONF_MARGINX-55,RANK_CONF_IMSIZEY+RANK_CONF_MARGINY-162);
   phDiagram_->AddBitmap(&rotated_eta_bmp);

   //declare that segmentation window is open
   window_open	= true;

   //get parent tool bar and update it
   toolbar	= parent->GetToolBar();
   UpdateToolBar();

   //disable confidence map text boxes
   txtA_->Enable(false);
   txtEpsilon_->Enable(false);
   txtKernelSize_->Enable(false);
   textA_->Enable(false);
   textEpsilon_->Enable(false);
   textKernelSize_->Enable(false);

   //disable run button
   runButton_->Enable(false);

   //disable radio boxes
   viewImSegRadio_->Enable(false);
   operationRadio_->Enable(false);

   //disable view boundaries check box
   viewBoundariesCheck_->Enable(false);

   //intialize parameter combo box
   float *myParameters = new float [6];
   myParameters[0]	= sigmaS;
   myParameters[1]	= sigmaR;
   myParameters[2]	= aij;
   myParameters[3]	= epsilon;
   myParameters[4]	= minRegion;
   myParameters[5]	= kernelSize;
   paramComboBox_->SetCurrentList((void *) myParameters, 6);
   isCurrentHistory_	= true;

   //turn on text box monitoring
   checkTextBoxes_	= true;

   //initialize speedup level to medium
   speedUpLevel_	= MED_SPEEDUP;
   speedUpThreshold_ = (float) 0.1;

   //initialize max/min zoom
   maxZoom_	= 0;
   minZoom_	= 1;

}


BgMdiSegmentChild::~BgMdiSegmentChild()
{
   if (hasBoundaries_ == 1)
     displayImage_->RemovePointSet(boundaries_);

	if(filename_)	delete [] filename_;

   delete optionsPanel_;
   delete displayImage_;
   
   delete boundaries_;
   delete whiteImage_;
   delete segmImage_;
   delete filtImage_;
   delete cbgImage_;

   if (customMap_)	delete [] customMap_;
   if (confMap_)	delete [] confMap_;
   if (gradMap_)	delete [] gradMap_;
   if (weightMap_)	delete [] weightMap_;

   g_children.DeleteObject(this);
}

void BgMdiSegmentChild::OnViewImSeg(wxCommandEvent&  WXUNUSED(event))
{
	//if an image has not been loaded then exit this method...
	if (hasImage_ == 0)
		return;
	
	switch(viewImSegRadio_->GetSelection())
	{
		
	//view original image
	case 0:
		boundaries_->pen_.SetColour(*wxWHITE);
		displayImage_->SetImage(cbgImage_->im_, cbgImage_->x_, cbgImage_->y_, cbgImage_->colorIm_);
		break;
		
	//view filtered image
	case 1:
		if (hasFilter_ == 0)
		{
			viewImSegRadio_->SetSelection(0);
			bgLog("Filter the image first!\n");
			return;
		} else
		{
			boundaries_->pen_.SetColour(*wxWHITE);
			displayImage_->SetImage(filtImage_->im_, filtImage_->x_, filtImage_->y_, filtImage_->colorIm_);
		}
		break;
		
	//view segmented image
	case 2:
		if (hasSegment_ == 0)
		{
			viewImSegRadio_->SetSelection(0);
			bgLog("Segment the image first!\n");
			return;
		} else
		{
			boundaries_->pen_.SetColour(*wxWHITE);
			displayImage_->SetImage(segmImage_->im_, segmImage_->x_, segmImage_->y_, segmImage_->colorIm_);
		}
		break;
		
	//view boundaries
	default:
		if (hasBoundaries_ == 0)
		{
			viewImSegRadio_->SetSelection(0);
			bgLog("Process the image first!\n");
			return;
		} else
		{
			boundaries_->pen_.SetColour(*wxBLACK);
			displayImage_->SetImage(whiteImage_->im_, whiteImage_->x_, whiteImage_->y_, whiteImage_->colorIm_);
		}
	}
}

//specifies image operation(i.e. segment, filter or fuse regions)
void BgMdiSegmentChild::OnChangeOperation(wxCommandEvent& WXUNUSED(event))
{

	wxMenuBar	*menubar	= GetMenuBar();
	switch(operationRadio_->GetSelection())
	{
	//segment the image
	case 0:
		menubar->SetLabel(BG_SEGM_SEGMENT, "Segment Image\tShift-R");
		break;
	//filter the image
	case 1:
		menubar->SetLabel(BG_SEGM_SEGMENT, "Filter Image\tShift-R");
		break;
	//fuse regions
	default:
		menubar->SetLabel(BG_SEGM_SEGMENT, "Fuse Regions\tShift-R");
		break;
	}

	//done.
	return;

}

//changes the paramters depending on the parameter list
void BgMdiSegmentChild::OnChangeParameters(wxCommandEvent& WXUNUSED(event))
{
	float	*myParameters;

	//get current selection index...
	int	selIndex	= paramComboBox_->GetSelection();

	//aquire current parameters and store them in "current parameter" slot...
	if(isCurrentHistory_)
	{
		int		csigmaS, cminRegion, ckernelSize;
		float	csigmaR, caij, cepsilon;
		GetParameters(csigmaS, csigmaR, caij, cepsilon, cminRegion, ckernelSize, 0);
		myParameters	= (float *) paramComboBox_->GetCurrentListData();
		myParameters[0]	= csigmaS;
		myParameters[1]	= csigmaR;
		myParameters[2]	= caij;
		myParameters[3]	= cepsilon;
		myParameters[4]	= cminRegion;
		myParameters[5]	= ckernelSize;
	}

	//check to see if the "current parameter" slot has been selected,
	//if so set the current history flag and use the parameters from
	//current history slot
	if(selIndex == 0)
	{
		isCurrentHistory_		= true;
		myParameters			= (float *) paramComboBox_->GetCurrentListData();
	}
	//otherwise indicate that the current slot has not just been selected
	//and get parameters from current parameter list
	else
	{
		isCurrentHistory_		= false;
		myParameters			= (float *) paramComboBox_->GetParameterListData(selIndex-1);
	}

	//set the text boxes...
	char str[10];
	checkTextBoxes_	= false;
	sprintf(str, "%d", bgRoundSign(myParameters[0]));
	txtSigmaS_->SetValue(str);
	sprintf(str, "%3.1f", myParameters[1]);
	txtSigmaR_->SetValue(str);
	sprintf(str, "%3.1f", myParameters[2]);
	txtA_->SetValue(str);
	sprintf(str, "%3.1f", myParameters[3]);
	txtEpsilon_->SetValue(str);
	sprintf(str, "%d", bgRoundSign(myParameters[4]));
	txtMinRegion_->SetValue(str);
	sprintf(str, "%d", bgRoundSign(myParameters[5]));
	txtKernelSize_->SetValue(str);
	checkTextBoxes_	= true;

}

void BgMdiSegmentChild::OnUpdateTextBoxes(wxCommandEvent& event)
{
	//update parameter history....
	if(checkTextBoxes_)
	{
		paramComboBox_->SetSelection(0);
		isCurrentHistory_	= true;
	}

	//check if a edge parameter has been changed
	int	id	= event.GetId();
	if((id == BG_SEGM_TEXT_GRADWIN)||(id == BG_SEGM_TEXT_AIJ)||(id == BG_SEGM_TEXT_EPSILON))
		edgeParamsHaveChanged_	= true;

}

void BgMdiSegmentChild::OnUpdateSpeedUpLevel(wxCommandEvent& event)
{
	long menuItemId			= event.GetId();
	wxMenuBar*	myMenuBar	= GetMenuBar();
	switch(menuItemId)
	{
	case BG_SEGM_SPEEDUP_MEDM:
		speedUpLevel_	= MED_SPEEDUP;
		myMenuBar->Check(BG_SEGM_SPEEDUP_NONE, false);
		myMenuBar->Check(BG_SEGM_SPEEDUP_HIGH, false);
		break;
	case BG_SEGM_SPEEDUP_HIGH:
		speedUpLevel_	= HIGH_SPEEDUP;
		myMenuBar->Check(BG_SEGM_SPEEDUP_NONE, false);
		myMenuBar->Check(BG_SEGM_SPEEDUP_MEDM, false);
      {
         BgSpeedSelect speedSelect(this, -1, "Select speed/quality", wxDefaultPosition, wxSize(220,150),
            wxDEFAULT_DIALOG_STYLE | wxDIALOG_MODAL);
         speedSelect.SetSliderValue(speedUpThreshold_);
         if (speedSelect.ShowModal()==wxID_OK)
         {
            speedSelect.GetSliderValue(speedUpThreshold_);
         }
      }

		break;
	default:
		speedUpLevel_	= NO_SPEEDUP;
		myMenuBar->Check(BG_SEGM_SPEEDUP_MEDM, false);
		myMenuBar->Check(BG_SEGM_SPEEDUP_HIGH, false);
	}
}

void BgMdiSegmentChild::OnViewBoundaries(wxCommandEvent&  WXUNUSED(event))
{
		
	if (hasBoundaries_ == false)
		return;

	bool	isChecked;
	isChecked = viewBoundariesCheck_->GetValue();
	
	if (isChecked)
	{
		// show edges
		displayImage_->AddPointSet(boundaries_);
		
	}
	else
	{
		// hide edges
		displayImage_->RemovePointSet(boundaries_);
	}

}


//activates/de-activates the text boxes used for
//synergistic segmentation based on the use confidence
//map checkbox
void BgMdiSegmentChild::OnUseWeightMap(wxCommandEvent& WXUNUSED(event))
{

	//depending on use confidence map checkbox, activate/de-activate
	//text boxes
	if(useWeightMap_->GetValue())
	{
		//enable text boxes
		txtA_->Enable(true);
		txtEpsilon_->Enable(true);
		txtKernelSize_->Enable(true);
		textA_->Enable(true);
		textEpsilon_->Enable(true);
		textKernelSize_->Enable(true);
	}
	else
	{
		//disable text boxes
		txtA_->Enable(false);
		txtEpsilon_->Enable(false);
		txtKernelSize_->Enable(false);
		textA_->Enable(false);
		textEpsilon_->Enable(false);
		textKernelSize_->Enable(false);
	}

	//done.
	return;
}

void BgMdiSegmentChild::OnUpdatePlotWindow1(wxCommandEvent& event)
{
	int menuItemId	= event.GetId();

	//update checkmarks
	winPanel1_->CheckViewItem(menuItemId);

	//change plot...
	int i;
	int data_length	= width_*height_;
	unsigned char *buffer = new unsigned char[data_length];
	BgText bgText(1,"ConfidenceMap", *wxSWISS_FONT, 0, 0);
	switch(menuItemId)
	{
	case BG_CANVAS_VIEW1_GRADMAP:
		bgText.SetText("Gradient Map");
		bgText.SetPlotLocation(RANK_CONF_MARGINX+width_/2-40,RANK_CONF_MARGINY/2-10);
		plotWindow1_->AddText(&bgText);
		for(i = 0; i < data_length; i++)
			buffer[i] = (unsigned char)(255*gradMap_[i] + 0.5);
		plotWindow1_->SetImageFromGray(buffer, width_, height_);
		break;
	case BG_CANVAS_VIEW1_CONFMAP:
		bgText.SetText("Confidence Map");
		bgText.SetPlotLocation(RANK_CONF_MARGINX+width_/2-45,RANK_CONF_MARGINY/2-10);
		plotWindow1_->AddText(&bgText);
		for(i = 0; i < data_length; i++)
			buffer[i] = (unsigned char)(255*confMap_[i] + 0.5);
		plotWindow1_->SetImageFromGray(buffer, width_, height_);
		break;
	case BG_CANVAS_VIEW1_WEITMAP:
		bgText.SetText("Weight Map");
		bgText.SetPlotLocation(RANK_CONF_MARGINX+width_/2-35,RANK_CONF_MARGINY/2-10);
		plotWindow1_->AddText(&bgText);
		for(i = 0; i < data_length; i++)
			buffer[i] = (unsigned char)(255*weightMap_[i] + 0.5);
		plotWindow1_->SetImageFromGray(buffer, width_, height_);
		break;
	case BG_CANVAS_VIEW1_CUSTMAP:
		bgText.SetText("Custom Weight Map");
		bgText.SetPlotLocation(RANK_CONF_MARGINX+width_/2-60,RANK_CONF_MARGINY/2-10);
		plotWindow1_->AddText(&bgText);
		for(i = 0; i < data_length; i++)
			buffer[i] = (unsigned char)(255*customMap_[i] + 0.5);
		plotWindow1_->SetImageFromGray(buffer, width_, height_);
		break;
	}
	
	//de-allocate memory used by conf/gradient map buffer
	delete [] buffer;
}

void BgMdiSegmentChild::OnUpdatePlotWindow2(wxCommandEvent& event)
{
	int menuItemId	= event.GetId();

	//update checkmarks
	winPanel2_->CheckViewItem(menuItemId);

	//change plot...
	int i;
	int data_length	= width_*height_;
	unsigned char *buffer = new unsigned char[data_length];
	BgText bgText(1,"ConfidenceMap", *wxSWISS_FONT, 0, 0);
	switch(menuItemId)
	{
	case BG_CANVAS_VIEW2_GRADMAP:
		bgText.SetText("Gradient Map");
		bgText.SetPlotLocation(RANK_CONF_MARGINX+width_/2-40,RANK_CONF_MARGINY/2-10);
		plotWindow2_->AddText(&bgText);
		for(i = 0; i < data_length; i++)
			buffer[i] = (unsigned char)(255*gradMap_[i] + 0.5);
		plotWindow2_->SetImageFromGray(buffer, width_, height_);
		break;
	case BG_CANVAS_VIEW2_CONFMAP:
		bgText.SetText("Confidence Map");
		bgText.SetPlotLocation(RANK_CONF_MARGINX+width_/2-45,RANK_CONF_MARGINY/2-10);
		plotWindow2_->AddText(&bgText);
		for(i = 0; i < data_length; i++)
			buffer[i] = (unsigned char)(255*confMap_[i] + 0.5);
		plotWindow2_->SetImageFromGray(buffer, width_, height_);
		break;
	case BG_CANVAS_VIEW2_WEITMAP:
		bgText.SetText("Weight Map");
		bgText.SetPlotLocation(RANK_CONF_MARGINX+width_/2-35,RANK_CONF_MARGINY/2-10);
		plotWindow2_->AddText(&bgText);
		for(i = 0; i < data_length; i++)
			buffer[i] = (unsigned char)(255*weightMap_[i] + 0.5);
		plotWindow2_->SetImageFromGray(buffer, width_, height_);
		break;
	case BG_CANVAS_VIEW2_CUSTMAP:
		bgText.SetText("Custom Weight Map");
		bgText.SetPlotLocation(RANK_CONF_MARGINX+width_/2-60,RANK_CONF_MARGINY/2-10);
		plotWindow2_->AddText(&bgText);
		for(i = 0; i < data_length; i++)
			buffer[i] = (unsigned char)(255*customMap_[i] + 0.5);
		plotWindow2_->SetImageFromGray(buffer, width_, height_);
		break;
	}
		
	//de-allocate memory used by conf/gradient map buffer
	delete [] buffer;
}


void BgMdiSegmentChild::OnSaveEdgeInformation(wxCommandEvent& event)
{

#if defined(__WXGTK__) || defined(__WXMOTIF__)
	wxFileDialog filedialog(this,"Save Edge Information","","",
		"*",wxSAVE);
#else
	wxFileDialog filedialog(this,"Save Edge Information","","",
		"Matlab ASCII data files (*.dat)|*.dat|PGM Files (*.pgm)|*.pgm",
		wxSAVE);
#endif

   if(filedialog.ShowModal()==wxID_OK)
   {

	   //get the image type
	   int dtype = filedialog.GetFilterIndex();

	   //obtain pointer to data
	   float	*myData;
	   switch(event.GetId())
	   {
	   case BG_CANVAS_SAVE_GRADMAP:
		   myData	= gradMap_;
		   break;
	   case BG_CANVAS_SAVE_CONFMAP:
		   myData	= confMap_;
		   break;
	   case BG_CANVAS_SAVE_WEITMAP:
		   myData	= weightMap_;
		   break;
	   }
	   
	   //get the filename and path
	   char* path		= (char*) filedialog.GetPath().c_str();
	   char* filename	= (char*) filedialog.GetFilename().c_str();
	   
	   //PGM Image
	   if(dtype)
	   {
		   unsigned char *buf	= new unsigned char [height_*width_];
		   int i;
		   for(i = 0; i < height_*width_; i++)
			   buf[i]	= myData[i]*255 + 0.5;
		   write_pgm_image(path, buf, height_, width_, "", 255);
		   delete [] buf;
	   }
	   else
	   //Matlab Data File
	   {
		   write_MATLAB_ASCII(path, myData, height_, width_);
	   }
   }

}

void BgMdiSegmentChild::OnSize(wxSizeEvent& WXUNUSED(event))
{
   int w, h;
   GetClientSize(&w, &h);
   optionsPanel_->SetSize(0, 0, BG_SEGM_OP_SIZEX, h);
   imagePlotSplitter_->SetSize(BG_SEGM_OP_SIZEX, 0, w-BG_SEGM_OP_SIZEX, h);
   imagePlotSplitter_->SetSashPosition((w-BG_SEGM_OP_SIZEX)/2, TRUE);
   plotMapSplitter_->SetSashPosition(h/2, TRUE);
   mapSplitter_->SetSashPosition((w-BG_SEGM_OP_SIZEX)/4, TRUE);
}

//clears display of ph diagram and rank and confidence maps
void BgMdiSegmentChild::ClearDisplay( void )
{
	phDiagram_->ClearDisplay();
	plotWindow1_->ClearDisplay();
	plotWindow2_->ClearDisplay();
}

//turns plotting capability on/off
void BgMdiSegmentChild::UpdateDisplay(bool update)
{
	if(update)
	{
		phDiagram_->noUpdate_	= false;
		plotWindow1_->noUpdate_	= false;
		plotWindow2_->noUpdate_	= false;
	}
	else
	{
		phDiagram_->noUpdate_	= true;
		plotWindow1_->noUpdate_	= true;
		plotWindow2_->noUpdate_	= true;
	}
}

void BgMdiSegmentChild::OnQuit(wxCommandEvent& WXUNUSED(event))
{
   Close(TRUE);
}

void BgMdiSegmentChild::OnClose(wxCloseEvent& event)
{
   //decrement global counter indicating number of frames open
   gs_nFrames--;
   //indicate that the window is now close (used by OnFocus)
   window_open = false;
   //reset the toolbar
   if(gs_nFrames == 0) ResetToolBar();
   event.Skip();
}

void BgMdiSegmentChild::OnFocus(wxFocusEvent& WXUNUSED(event))
{
	//update toolbar
	if(!on_exit) UpdateToolBar();
	return;
}

void BgMdiSegmentChild::ZoomWindow(void)
{
	//display zoom window
	displayImage_->zoom_window	= true;
	displayImage_->zoom_in		= false;
	displayImage_->zoom_out		= false;
}

void BgMdiSegmentChild::ZoomIn(void)
{
	//zoom into display image
	displayImage_->zoom_window	= false;
	displayImage_->zoom_in		= true;
	displayImage_->zoom_out		= false;
	return;
}

void BgMdiSegmentChild::ZoomOut(void)
{
	//zoom out of display image
	displayImage_->zoom_window	= false;
	displayImage_->zoom_in		= false;
	displayImage_->zoom_out		= true;
	return;
}

void BgMdiSegmentChild::NoZoom(void)
{
	//do not zoom display image
	displayImage_->zoom_window	= false;
	displayImage_->zoom_in		= false;
	displayImage_->zoom_out		= false;
	return;
}

void BgMdiSegmentChild::UpdateZoomControl(void)
{
	//determine whether to enable zoom in control based on maximum zoom
	if(maxZoom_ || minZoom_)
		UpdateToolBar();
	else
	{
		toolbar->EnableTool(BG_ZOOM_IN, true);
		toolbar->EnableTool(BG_ZOOM_OUT, true);
//		toolbar->Realize();
	}
}

void BgMdiSegmentChild::RunEnable(void)
{
	wxMenuBar *menubar = GetMenuBar();
	menubar->Enable(BG_SEGM_LOAD_MAP, true);
	menubar->Enable(BG_SEGM_SEGMENT, true);
	runButton_->Enable(true);
	viewImSegRadio_->Enable(true);
	viewImSegRadio_->Enable(1, false);
	viewImSegRadio_->Enable(2, false);
	viewImSegRadio_->Enable(3, false);
}

void BgMdiSegmentChild::SaveEnable(void)
{
	toolbar->EnableTool(BG_SAVE_RESULT, true);
}

void BgMdiSegmentChild::UpdateToolBar(void)
{
	//update toolbar (except during close)
	if(window_open)
	{
		//determine whether to enable save based on whether segmentation
		//has occurred
		bool save_enable;
		if(hasSegment_)
			save_enable	= true;
		else
			save_enable	= false;

		//determine whether to enable zoom controls based on whether image
		//has been loaded
		bool load_enable;
		if(hasImage_)
			load_enable	= true;
		else
			load_enable	= false;

		//determine whether to enable zoom in control based on maximum zoom
		bool max_zoom;
		if(maxZoom_)
			max_zoom	= true;
		else
			max_zoom	= false;

		//determine whether to enable zoom out control based on minimum zoom
		bool min_zoom;
		if(minZoom_)
			min_zoom	= true;
		else
			min_zoom	= false;

		//adjust toolbar
		toolbar->SetToolShortHelp(BG_LOAD_IMAGE, "Load image to perform image segmentation");
		toolbar->SetToolShortHelp(BG_SAVE_RESULT, "Save segmented image");
		toolbar->EnableTool(BG_SAVE_RESULT, save_enable);
		toolbar->EnableTool(BG_CROSS, load_enable);
		toolbar->EnableTool(BG_ZOOM_IN, ((load_enable)&&(!max_zoom)));
		toolbar->EnableTool(BG_ZOOM_OUT, ((load_enable)&&(!min_zoom)));
		toolbar->EnableTool(BG_POINTER, true);

		//set to no zoom
		toolbar->ToggleTool(BG_CROSS, false);
		toolbar->ToggleTool(BG_ZOOM_IN, false);
		toolbar->ToggleTool(BG_ZOOM_OUT, false);
		toolbar->ToggleTool(BG_POINTER, true);
		displayImage_->SetCursor(wxCURSOR_ARROW);
		NoZoom();
	}
	return;
}

void BgMdiSegmentChild::ResetToolBar(void)
{
	//update toolbar
	toolbar->SetToolShortHelp(BG_LOAD_IMAGE, "Load image to process");
	toolbar->SetToolShortHelp(BG_SAVE_RESULT, "Save result");
	toolbar->EnableTool(BG_SAVE_RESULT, false);
	toolbar->EnableTool(BG_CROSS, false);
	toolbar->EnableTool(BG_ZOOM_IN, false);
	toolbar->EnableTool(BG_ZOOM_OUT, false);
	toolbar->EnableTool(BG_POINTER, false);
	toolbar->ToggleTool(BG_CROSS, false);
	toolbar->ToggleTool(BG_ZOOM_IN, false);
	toolbar->ToggleTool(BG_ZOOM_OUT, false);
	toolbar->ToggleTool(BG_POINTER, false);
	return;
}

void BgMdiSegmentChild::ReadImage(char *pathname, char *filename)
{
	//take away point set if boundaries have been calculated
	if(hasBoundaries_)
	{
		hasBoundaries_ = false;
		displayImage_->RemovePointSet(boundaries_);
		boundaries_->CleanData();
	}

	if(displayImage_->SetImage(pathname) == 0)
		return;

	//obtain and store image filename
	if(filename_)	delete [] filename_;
	filename_	= new char [strlen(filename) + 1];
	strcpy(filename_, filename);
	
	displayImage_->showbitmap_ = true;
	
	//set value of view boundaries control
	viewBoundariesCheck_->SetValue(0);
	
	//set image view option to original
	viewImSegRadio_->SetSelection(0);

	//get image dimension
	width_			= displayImage_->pimage->GetWidth();
	height_			= displayImage_->pimage->GetHeight();
	
	//reset zoom level of display image
	displayImage_->Zoom(1);
	
	//reset max/min zoom flags
	maxZoom_	= 0;
	minZoom_	= 1;
	
	//set cbgImage
	cbgImage_->SetImageFromRGB(displayImage_->pimage->GetData(), width_, height_, true);
	hasImage_		= 1;
	hasSegment_		= 0;
	hasBoundaries_	= 0;
	
	//adjust titles
	BgText title(1, "Confidence Map", *wxSWISS_FONT, RANK_CONF_MARGINX+width_/2-45,RANK_CONF_MARGINY/2-10);
	plotWindow1_->AddText(&title);
	title.SetText("Gradient Map");
	title.SetPlotLocation(RANK_CONF_MARGINX+width_/2-40,RANK_CONF_MARGINY/2-10);
	plotWindow2_->AddText(&title);
	
	//add axis
	plotWindow1_->ClearAxis();
	plotWindow2_->ClearAxis();
	plotWindow1_->AddHorizontalAxis(RANK_CONF_MARGINX, RANK_CONF_MARGINY+height_, width_, 5, 0, width_-1);
	plotWindow1_->AddVerticalAxis(RANK_CONF_MARGINX, RANK_CONF_MARGINY+height_, height_, 5, 0, height_-1);
	plotWindow2_->AddHorizontalAxis(RANK_CONF_MARGINX, RANK_CONF_MARGINY+height_, width_, 5, 0, width_-1);
	plotWindow2_->AddVerticalAxis(RANK_CONF_MARGINX, RANK_CONF_MARGINY+height_, height_, 5, 0, height_-1);
	
	//label axis
	BgText label(1, "x", *wxSWISS_FONT, 0, 0);
	plotWindow1_->LabelHorizontalAxis(&label);
	plotWindow2_->LabelHorizontalAxis(&label);
	label.SetText("y");
	plotWindow1_->LabelVerticalAxis(&label);
	plotWindow2_->LabelVerticalAxis(&label);
	plotWindow1_->RotateVerticalAxisLabel(-90);
	plotWindow2_->RotateVerticalAxisLabel(-90);
	
	//set white image
	unsigned char *img = new unsigned char [(width_)*(height_)*3];
	memset(img, 255, (width_)*(height_)*3); 
	whiteImage_->SetImageFromRGB(img, width_, height_, true);
	delete [] img;
	
	//clear the plot window displays
	ClearDisplay();

	//do no update the plot windows
	UpdateDisplay(false);
	
	//delete edge maps if they exists...
	if(customMap_)	delete [] customMap_;
	if(gradMap_  )	delete [] gradMap_;
	if(confMap_  )	delete [] confMap_;
	if(weightMap_)	delete [] weightMap_;
	
	//set edge maps to NULL...
	customMap_	= (float *) NULL;
	gradMap_	= (float *) NULL;
	confMap_	= (float *) NULL;
	weightMap_	= (float *) NULL;
	
	//update interface...
	
	/***********************************************/
	
	//enable run
	RunEnable();
	
	//enable operations radio box
	operationRadio_->Enable(true);
	
	//update the tool bar
	UpdateToolBar();
	
	//set window title
	wxString statusname;
	statusname.Printf(_T("Segmentation Frame %d - %s (%d x %d) [Original Image]"), window_number_, filename_, width_, height_);
	SetTitle(statusname);
	
	//update the menubar
	wxMenuBar	*menubar	= GetMenuBar();
	menubar->Enable(BG_SEGM_SAVE_SEGMENTED, false);
	menubar->Enable(BG_SEGM_USE_MAP, false);
	
	//update window menus
	winPanel1_->view_menu->Enable(BG_CANVAS_VIEW1_CUSTMAP, false);
	winPanel2_->view_menu->Enable(BG_CANVAS_VIEW2_CUSTMAP, false);
	winPanel1_->EnableMenu(false);
	winPanel2_->EnableMenu(false);
	
	/***********************************************/

}

void BgMdiSegmentChild::OnLoadImage(wxCommandEvent& WXUNUSED(event))
{
// get the file name
//   wxFileDialog filedialog(this,"Choose an image file","","",
//      "All files (*.*)|*.*|PNG files (*.png)|*.png|BMP files (*.bmp)|*.bmp|GIF files (*.gif)|*.gif|TIFF files (*.tif)|*.tif|JPEG files (*.jpg)|*.jpg|PNM files (*.pnm)|*.pnm",
//      wxOPEN);
#if defined(__WXGTK__) || defined(__WXMOTIF__)
  wxFileDialog filedialog(this,"Choose an image file","","",
			  "*",wxOPEN);
#else
   wxFileDialog filedialog(this,"Choose an image file","","",
      "Common image files|*.png;*.bmp;*.gif;*.tif;*.tiff;*.jpg;*.pnm;*.pgm;*.ppm|PNG files (*.png)|*.png|BMP files (*.bmp)|*.bmp|GIF files (*.gif)|*.gif|TIFF files (*.tif)|*.tif|JPEG files (*.jpg)|*.jpg|PNM files (*.pnm)|*.pnm|PGM/PPM files (*.pgm,*.ppm)|*.pgm;*.ppm",
      wxOPEN);
#endif

   if(filedialog.ShowModal()==wxID_OK)
   {
	  //take away point set if boundaries have been calculated
	  if(hasBoundaries_)
	  {
		  hasBoundaries_ = false;
		  displayImage_->RemovePointSet(boundaries_);
		  boundaries_->CleanData();
	  }

      if (displayImage_->SetImage(filedialog.GetPath().c_str()) == 0)
         return;
      bgLog("Image %s loaded\n",filedialog.GetPath().c_str());

	  //obtain and store image filename
	  if(filename_)	delete [] filename_;
	  filename_	= new char [strlen(filedialog.GetFilename().c_str()) + 1];
	  strcpy(filename_, filedialog.GetFilename().c_str());

      displayImage_->showbitmap_ = true;

	  //set value of view boundaries control
	  viewBoundariesCheck_->SetValue(0);

	  //set image view option to original
	  viewImSegRadio_->SetSelection(0);

	  //set operation option to segment
	  operationRadio_->SetSelection(0);

	  //get image dimension
	  width_			= displayImage_->pimage->GetWidth();
	  height_			= displayImage_->pimage->GetHeight();

	  //reset zoom level of display image
	  displayImage_->Zoom(1);

	  //reset max/min zoom flags
	  maxZoom_	= 0;
	  minZoom_	= 1;

      //set cbgImage
      cbgImage_->SetImageFromRGB(displayImage_->pimage->GetData(), width_, height_, true);
      hasImage_			= 1;
	  hasFilter_		= 0;
      hasSegment_		= 0;
      hasBoundaries_	= 0;

	  //adjust titles
	  BgText title(1, "Confidence Map", *wxSWISS_FONT, RANK_CONF_MARGINX+width_/2-45,RANK_CONF_MARGINY/2-10);
	  plotWindow1_->AddText(&title);
	  title.SetText("Gradient Map");
	  title.SetPlotLocation(RANK_CONF_MARGINX+width_/2-40,RANK_CONF_MARGINY/2-10);
	  plotWindow2_->AddText(&title);

	  //add axis
	  plotWindow1_->ClearAxis();
	  plotWindow2_->ClearAxis();
	  plotWindow1_->AddHorizontalAxis(RANK_CONF_MARGINX, RANK_CONF_MARGINY+height_, width_, 5, 0, width_-1);
	  plotWindow1_->AddVerticalAxis(RANK_CONF_MARGINX, RANK_CONF_MARGINY+height_, height_, 5, 0, height_-1);
	  plotWindow2_->AddHorizontalAxis(RANK_CONF_MARGINX, RANK_CONF_MARGINY+height_, width_, 5, 0, width_-1);
	  plotWindow2_->AddVerticalAxis(RANK_CONF_MARGINX, RANK_CONF_MARGINY+height_, height_, 5, 0, height_-1);

	  //label axis
	  BgText label(1, "x", *wxSWISS_FONT, 0, 0);
	  plotWindow1_->LabelHorizontalAxis(&label);
	  plotWindow2_->LabelHorizontalAxis(&label);
	  label.SetText("y");
	  plotWindow1_->LabelVerticalAxis(&label);
	  plotWindow2_->LabelVerticalAxis(&label);
	  plotWindow1_->RotateVerticalAxisLabel(-90);
	  plotWindow2_->RotateVerticalAxisLabel(-90);

	  //set white image
	  unsigned char *img = new unsigned char [(width_)*(height_)*3];
	  memset(img, 255, (width_)*(height_)*3); 
	  whiteImage_->SetImageFromRGB(img, width_, height_, true);
	  delete [] img;

	  //clear the plot window displays
	  ClearDisplay();
	  
	  //do not update plot windows
	  UpdateDisplay(false);

	  //delete edge maps if they exists...
	  if(customMap_)	delete [] customMap_;
	  if(gradMap_  )	delete [] gradMap_;
	  if(confMap_  )	delete [] confMap_;
	  if(weightMap_)	delete [] weightMap_;
	  
	  //set edge maps to NULL...
	  customMap_	= (float *) NULL;
	  gradMap_		= (float *) NULL;
	  confMap_		= (float *) NULL;
	  weightMap_	= (float *) NULL;

	  //update interface...

	  /***********************************************/

	  //enable run
	  RunEnable();

	  //enable operations radio box
	  operationRadio_->Enable(true);

	  //update the tool bar
	  UpdateToolBar();

	  //set window title
      wxString statusname;
      statusname.Printf(_T("Segmentation Frame %d - %s (%d x %d) [Original Image]"), window_number_, filename_, width_, height_);
      SetTitle(statusname);

	  //update the menubar
	  wxMenuBar	*menubar	= GetMenuBar();
	  menubar->Enable(BG_SEGM_SAVE_SEGMENTED, false);
	  menubar->Enable(BG_SEGM_USE_MAP, false);

	  //update window menus
	  winPanel1_->view_menu->Enable(BG_CANVAS_VIEW1_CUSTMAP, false);
	  winPanel2_->view_menu->Enable(BG_CANVAS_VIEW2_CUSTMAP, false);
	  winPanel1_->EnableMenu(false);
	  winPanel2_->EnableMenu(false);

	  /***********************************************/

   }
}


void BgMdiSegmentChild::LoadCustomWeightMap(wxCommandEvent& WXUNUSED(event))
{
	
	//make sure an image was read...
	if(hasImage_ == 0)
	{
		bgLog("image has not been loaded, load image.\n");
		return;
	}
	
	//compute data length
    int	xsz			= cbgImage_->x_, ysz = cbgImage_->y_;
	int	data_length	= (xsz)*(ysz);

// get the file name
#if defined(__WXGTK__) || defined(__WXMOTIF__)
  wxFileDialog filedialog(this,"Upload Custom Weight Map","","",
			  "*",wxOPEN);
#else
   wxFileDialog filedialog(this,"Upload Custom Weight Map","","",
      "Matlab ASCII data files (*.dat)|*.dat|All Files (*.*)|*.*",
      wxOPEN);
#endif
   if(filedialog.ShowModal()==wxID_OK)
   {

	   //de-allocate memory for customMap (if it exists)
	   if(customMap_)	delete [] customMap_;
	   
	   //allocate memory for customMap
	   customMap_				= new float [data_length];		
	   
	   //get data file name
	   const char	*filename		= filedialog.GetPath().c_str();
	   
	   //attempt to read in data
	   FILE	*fp = fopen(filename, "rb");
	   int i;
	   for(i = 0; i < data_length; i++)
	   {
		   if(!fscanf(fp, "%f", &customMap_[i]))
		   {
			   bgLog("Load Map Error: Data length does not match the size of the image.");
			   delete [] customMap_;
			   customMap_	= NULL;
			   return;
		   }
	   }
	   fclose(fp);

	   //inform user of successful upload
	   bgLog("Weight map '%s' successfully uploaded.\n", filename); 

	   //enable use custom weight map menu items
	   wxMenuBar* myMenuBar	= GetMenuBar();
	   myMenuBar->Enable(BG_SEGM_USE_MAP, true);
	   winPanel1_->view_menu->Enable(BG_CANVAS_VIEW1_CUSTMAP, true);
	   winPanel2_->view_menu->Enable(BG_CANVAS_VIEW2_CUSTMAP, true);
	   
   }
}

void BgMdiSegmentChild::OnSaveSegmentedImage(wxCommandEvent& WXUNUSED(event))
{
   if ((hasSegment_ == 0)&&(hasFilter_ == 0))
   {
      bgLog("No result to save. Run segmenter.\n");
      return;
   }

#if defined(__WXGTK__) || defined(__WXMOTIF__)
  wxFileDialog filedialog(this,"Choose an image file","","",
			  "*",wxSAVE);
#else
   wxFileDialog filedialog(this,"Choose an image file","","",
      "PNM files (*.pnm)|*.pnm|PNG files (*.png)|*.png|PCX files (*.pcx)|*.pcx|PGM files (*.pgm)|*.pgm|JPEG files (*.jpg) (not recommended)|*.jpg",
      wxSAVE);
#endif
   if(filedialog.ShowModal()==wxID_OK)
   {

	   //get the image type
	   int imtype;
	   switch (filedialog.GetFilterIndex())
	   {
	   case 0:
		   imtype = wxBITMAP_TYPE_PNM;
		   break;
	   case 1:
		   imtype = wxBITMAP_TYPE_PNG;
		   break;
	   case 2:
		   imtype = wxBITMAP_TYPE_PCX;
		   break;
	   case 3:
		   imtype = wxBITMAP_TYPE_ANY;
		   break;
	   case 4:
		   imtype = wxBITMAP_TYPE_JPEG;
		   break;
	   default:
		   return;
	   }
	   
	   //if the filtered image is available then save it...
	   if(hasFilter_ == 1)
	   {

		   //get path and add extension
		   char *path	= new char [strlen(filedialog.GetPath()) + 1];
		   strcpy(path, filedialog.GetPath());
		   BgAddExtension(&path, "_filt");
		   
		   //convert the image to wxImage format...
		   wxImage tmpIm(filtImage_->x_, filtImage_->y_);
		   unsigned char* tmpImData;
		   tmpImData = tmpIm.GetData();
		   filtImage_->GetImageColor(tmpImData);
		   
		   //save the image...
		   tmpIm.SaveFile(path, imtype);
		   bgLog("Filtered image saved to:\t'%s'.\n", path);

		   //de-allocate memory
		   delete path;

	   }
	   
	   //if the segmented image is available then save it...
	   if(hasSegment_ == 1)
	   {
		   
		   //get path and add extension
		   char *path	= new char [strlen(filedialog.GetPath()) + 1];
		   strcpy(path, filedialog.GetPath());
		   BgAddExtension(&path, "_segm");
		   
		   //convert the image to wxImage format...
		   wxImage tmpIm(segmImage_->x_, segmImage_->y_);
		   unsigned char* tmpImData;
		   tmpImData = tmpIm.GetData();
		   segmImage_->GetImageColor(tmpImData);
		   
		   //save the image...
		   tmpIm.SaveFile(path, imtype);
		   bgLog("Segmented image saved to:\t'%s'.\n", path);

		   //de-allocate memory
		   delete [] path;
		   
	   }
	   
	   //save boundaries
	   OnSaveBoundaries((char*)filedialog.GetPath().c_str(), imtype);
	   
   }
}

//saves edge map
void BgMdiSegmentChild::OnSaveBoundaries(char *filename, int imtype)
{

	//make sure boundaries exist before proceeding...
	if(hasBoundaries_ == 0)
		return;

	//create edge map
	int width = cbgImage_->x_, height = cbgImage_->y_;
	wxImage tmpImg(width, height);
	unsigned char *buf = tmpImg.GetData();
	memset(buf, 255, width*height*3*sizeof(unsigned char));
	int n = boundaries_->n_;
	int i, dp, x, y;
	for(i = 0; i < n; i++)
	{
		x			= boundaries_->x_[i];
		y			= boundaries_->y_[i];
		dp			= y*width+x;
		buf[3*dp]	= buf[3*dp+1]	= buf[3*dp+2]	= 0;
	}

	//create new filename (add _em extension)
	char *new_filename	= new char [strlen(filename) + 1];
	strcpy(new_filename, filename);
	BgAddExtension(&new_filename, "_bndy");

	//save edgemap using specified filename and image format
	tmpImg.SaveFile(new_filename, imtype);
	bgLog("Boundaries saved to:\t'%s'.\n", new_filename);

	//de-allocate memory
	delete	[] new_filename;

	//done.
	return;

}

//construct a ph diagram using the boundary pixels
//aquired from image segmentation (also outputs results
//to 'xyrc.txt'
void BgMdiSegmentChild::SetphDiagram(int width, float *confMap, float *rankMap)
{
   unsigned char* buf;
   int xsz	= RANK_CONF_IMSIZEX;
   int ysz	= RANK_CONF_IMSIZEY;
   int imsz = xsz*ysz;
   buf = new unsigned char[imsz];
   int i;
   for (i=0; i<imsz; i++)
      buf[i] = 255;

   int l, c, dpoint;
   int n = boundaries_->n_;
   int *xpos = boundaries_->x_, *ypos = boundaries_->y_;
   for (i=0; i<n; i++)
   {
	  dpoint = (ypos[i])*width+xpos[i];
      if ((rankMap[dpoint]>0) && (confMap[dpoint]>0))
      {
         c = (int) (rankMap[dpoint]*((double) xsz));
         c = (c>=xsz) ? xsz-1 : c;
         l = (int) (confMap[dpoint]*((double) ysz));
         l = (l>=ysz) ? ysz-1 : l;
         l = ysz-1-l;
         buf[c+l*xsz]=80;

      }
   }

   phDiagram_->SetImageFromGray(buf, xsz, ysz);

   //adjust scroll bar...
   phDiagram_->SetScrollbars(1, 1, xsz+2*RANK_CONF_MARGINX, ysz+2*RANK_CONF_MARGINY+40);
   delete [] buf;
}

void mySegment(void *Object)
{
	BgMdiSegmentChild *segmObj	= (BgMdiSegmentChild *) Object;
	segmObj->Segment();
}

void BgMdiSegmentChild::OnSegment(wxCommandEvent& WXUNUSED(event))
{

	//get parameters from GUI
	int error = GetParameters(sigmaS, sigmaR, aij, epsilon, minRegion, kernelSize, 1);
	if(error)
		return;

	//create the working thread
	BgThread *workingThread	= new BgThread(wxTHREAD_DETACHED, mySegment, this);
	stop_flag	= false;
	workingThread->Create();
	workingThread->Run();

	//determine progress bar title and message
	int	operation	= operationRadio_->GetSelection();
	char myMessage[80], myTitle[80];
	switch (operation)
	{
	//filter
	case 1:
		strcpy(myTitle,		"Image Filtering Progress");
		strcpy(myMessage,	"Filtering Image...");
		break;
	//fuse regions
	case 2:
		strcpy(myTitle,		"Region Fusing Progress");
		strcpy(myMessage,	"Fusing Image Regions...");
		break;
	//segment
	default:
		strcpy(myTitle,		"Image Segmentation Progress");
		strcpy(myMessage,	"Segmenting Image...");
		break;
	}

	//call progress dialog
	bool	done;
	percentDone	= 0;
	wxProgressDialog progressDialog(myTitle, myMessage, 100, this, wxPD_AUTO_HIDE | wxPD_APP_MODAL | wxPD_CAN_ABORT);
	while((done = progressDialog.Update(percentDone))&&(percentDone != 100)) wxYield();
	if(!done)	stop_flag	= true;	//if the algorithm is not done but the user aborted the operation
									//then tell the thread to stop running

	//if its succesfully completed update the parameter history box
	if(done)
	{
		int	selIndex	= paramComboBox_->GetSelection();
		if(selIndex)
		{
			paramComboBox_->UseParameterList(selIndex-1);
		}
		else
		{
			float *myParameters = new float [6];
			myParameters[0]	= sigmaS;
			myParameters[1]	= sigmaR;
			myParameters[2]	= aij;
			myParameters[3]	= epsilon;
			myParameters[4]	= minRegion;
			myParameters[5]	= kernelSize;
			paramComboBox_->AddParameterList((void *) myParameters, 6);
		}

		//set the selection to the current parameter slot
		paramComboBox_->SetSelection(0);
		isCurrentHistory_	= true;
	}
	return;

}

void BgMdiSegmentChild::Segment( void )
{
	// parameters
	//   sigma_s
	//   sigma_r
	//   a
	//   epsilon
	//   minRegion
	//   kernel radius
	//   filter
	//   speedup level
	
	if (hasImage_ == 0)
	{
		bgLog("No image loaded!\n");
		return;
	}

	//if image segmentaiton is not synergistic clear
	//display and disable it, otherwise enable it
	if(!useWeightMap_->GetValue())
	{
		ClearDisplay();
		UpdateDisplay(false);
	}
	else
	{
		UpdateDisplay(true);
	}
	
	//display parameters to the user
	if(useWeightMap_->GetValue())
	{
		bgLog("Input parameters:\n");
		bgLog("\tSpatial Bandwidth\t\t= %4d\n\tColor Bandwidth\t\t= %4.1f\n", sigmaS, sigmaR);
		bgLog("\tMinimum Region\t\t= %4d\n", minRegion);
		bgLog("\tGradient Window Radius\t= %4d\n", kernelSize);
		bgLog("\tMixing Parameter\t\t= %4.1f\n", aij);
		bgLog("\tThreshold\t\t\t= %4.1f\n", epsilon);
	}
	else
	{
		bgLog("Input parameters:\n");
		bgLog("\tSpatial Bandwidth\t= %4d\n\tColor Bandwidth\t= %4.1f\n", sigmaS, sigmaR);
		bgLog("\tMinimum Region\t= %4d\n", minRegion);
	}
	
	//obtain image dimensions
	int	width, height;
	width = cbgImage_->x_;
	height = cbgImage_->y_;

	//obtain image type (color or grayscale)
	imageType	gtype;
	if(cbgImage_->colorIm_)
		gtype = COLOR;
	else
		gtype = GRAYSCALE;

	//if gradient and confidence maps are not defined, 
	//and synergistic segmentation is requested, then compute them;
	//also compute them if the parameters have changed
	if(useWeightMap_->GetValue())
	{
		
		//if the weight map has already been defined
		//then find out if it needs to be recomputed
		if((weightMap_)&&(edgeParamsHaveChanged_))
		{
			delete [] confMap_;
			delete [] gradMap_;
			delete [] weightMap_;
			weightMap_	= (float *) NULL;
			//indicate that the change has been recognized...
			edgeParamsHaveChanged_	= false;
		}
		
		//if the weight map has not been computed or discarded
		//then recompute it...
		if(!weightMap_)
		{
			
			//allocate memory for gradient and confidence maps
			confMap_	= new float[width*height];
			gradMap_	= new float[width*height];
			
			//compute gradient and confidence maps
			BgEdgeDetect	edgeDetector(kernelSize);
			edgeDetector.ComputeEdgeInfo(cbgImage_, confMap_, gradMap_);
			
			//compute weight map...
			
			/******************************************************************/
			
			//allocate memory for weight map
			weightMap_ = new float[width*height];
			
			//compute weight map using gradient and confidence maps
			int i;
			for (i=0; i<width*height; i++)
			{
				if (gradMap_[i] > 0.02)
					weightMap_[i] = aij*gradMap_[i] + (1 - aij)*confMap_[i];
				else
					weightMap_[i] = 0;
			}
			
			/******************************************************************/
		}
		
	}
	
	//determine operation (filtering or segmentation)
	int	operation	= operationRadio_->GetSelection();

	//create instance of image processor class
	msImageProcessor *iProc = new msImageProcessor();

	//define an input image using the image under consideration
	//(if filtering or segmentation has taken place, then use this
	// result upon performing fusing...)
	if((operation == 2)&&(hasFilter_))
		iProc->DefineImage(filtImage_->im_, gtype, height, width);
	else
		iProc->DefineImage(cbgImage_->im_, gtype, height, width);

	//determine if a custom weight map is to be used
	wxMenuBar *menubar = GetMenuBar();
	bool useCustomMap	= menubar->IsChecked(BG_SEGM_USE_MAP);

	//set the weight map (if one was specified and a custom map is not being utilized)
	if((useWeightMap_->GetValue())&&(weightMap_)&&(!useCustomMap))	iProc->SetWeightMap(weightMap_, epsilon);

	//set the custom map (if one was provided)
	if((useWeightMap_->GetValue())&&(customMap_)&&(useCustomMap))	iProc->SetWeightMap(customMap_, epsilon);
	
	//check for errors in image definition or in the setting
	//of the confidence map...
	if (iProc->ErrorStatus)
	{
		bgLog("%s\n", iProc->ErrorMessage);
		return;
	}

	//perform image segmentation or filtering....
	timer_start(); //start the timer
   iProc->SetSpeedThreshold(speedUpThreshold_);
	switch(operation)
	{
	//filter
	case 1:

		iProc->Filter(sigmaS, sigmaR, speedUpLevel_);
		if (iProc->ErrorStatus == EL_ERROR)
		{
			bgLog("%s\n", iProc->ErrorMessage);
			return;
		} else if (iProc->ErrorStatus == EL_HALT)
		{
			break;
		}

		//obtain the filtered image....
		filtImage_->Resize(width, height, cbgImage_->colorIm_);
		iProc->GetResults(filtImage_->im_);
		if (iProc->ErrorStatus == EL_ERROR)
		{
			bgLog("%s\n", iProc->ErrorMessage);
			return;
		}

		//indicate that only the filtered image has been computed...
		hasFilter_	= 1;
		hasSegment_	= 0;
		
		break;

	//fuse
	case 2:

		iProc->FuseRegions(sigmaR, minRegion);
		if (iProc->ErrorStatus == EL_ERROR)
		{
			bgLog("%s\n", iProc->ErrorMessage);
			return;
		} else if (iProc->ErrorStatus == EL_HALT)
		{
			break;
		}

		//obtain the segmented image...
		segmImage_->Resize(width, height, cbgImage_->colorIm_);
		iProc->GetResults(segmImage_->im_);
		if (iProc->ErrorStatus == EL_ERROR)
		{
			bgLog("%s\n", iProc->ErrorMessage);
			return;
		}

		//indicate that the segmented image has been computed...
		hasSegment_	= 1;

		break;

	//segment
	default:

		//filter the image...
		iProc->Filter(sigmaS, sigmaR, speedUpLevel_);
		if (iProc->ErrorStatus == EL_ERROR)
		{
			bgLog("%s\n", iProc->ErrorMessage);
			return;
		} else if (iProc->ErrorStatus == EL_HALT)
		{
			break;
		}

		//filter the image....
		int dim;
		if(cbgImage_->colorIm_)
			dim = 3;
		else
			dim = 1;
		unsigned char *tempImage = new unsigned char [dim*height*width];
		iProc->GetResults(tempImage);
		if (iProc->ErrorStatus == EL_ERROR)
		{
			bgLog("%s\n", iProc->ErrorMessage);
			delete [] tempImage;
			return;
		}
		
		//fuse regions...
		iProc->FuseRegions(sigmaR, minRegion);
		if (iProc->ErrorStatus == EL_ERROR)
		{
			bgLog("%s\n", iProc->ErrorMessage);
			delete [] tempImage;
			return;
		} else if (iProc->ErrorStatus == EL_HALT)
		{
			delete [] tempImage;
			break;
		}

		//obtain the segmented and filtered image...
		filtImage_->Resize(width, height, cbgImage_->colorIm_);
		memcpy(filtImage_->im_, tempImage, dim*height*width*sizeof(unsigned char));
		delete [] tempImage;
		segmImage_->Resize(width, height, cbgImage_->colorIm_);
		iProc->GetResults(segmImage_->im_);
		if (iProc->ErrorStatus)
		{
			bgLog("%s\n", iProc->ErrorMessage);
			return;
		}

		//indicate that both the filtered and segmented image have been computed...
		hasFilter_	= 1;
		hasSegment_	= 1;

	}

	//check and see if the algorithm has been halted, if so
	//then de-allocate the image processing object and
	//indicate to the working thread that it is safe
	//to exit at this point and exit
	if(iProc->ErrorStatus == EL_HALT)
	{
		delete iProc;
		stop_flag	= false;
		bgLog("Operation Canceled.\n");
		return;
	}

	//set has boundaries to true
	hasBoundaries_	= true;
	
	//clean boundary data if any exists
	boundaries_->CleanData();
	
	//get boundary indeces from region list object
	//of the image processor object...
	RegionList	*regionList			= iProc->GetBoundaries();
	int			*regionIndeces		= regionList->GetRegionIndeces(0);
	int			numRegions			= regionList->GetNumRegions();
	int			boundaryPointCount	= 0;
	
	//calculate the number of boundary points stored by region list
	//class
	int i;
	for(i = 0; i < numRegions; i++)
		boundaryPointCount += regionList->GetRegionCount(i);
	
	//create a point set using calculated boundary point count...
	boundaries_->x_	= new int [boundaryPointCount];
	boundaries_->y_	= new int [boundaryPointCount];
	boundaries_->n_	= boundaryPointCount;
	for(i = 0; i < boundaryPointCount; i++)
	{
		boundaries_->x_[i]	= regionIndeces[i]%width;
		boundaries_->y_[i]	= regionIndeces[i]/width;
	}
	
	//set pen width, and style
	boundaries_->pen_.SetWidth(1);
	boundaries_->pen_.SetStyle(wxSOLID);
	
	//set point type to point (= 1)
	boundaries_->type_	= 1;
	
	//construct the phDiagram
	if(useWeightMap_->GetValue())	SetphDiagram(width, confMap_, gradMap_);
	
	//display the confidence and gradient maps respectively
	if(useWeightMap_->GetValue())
	{
		wxCommandEvent myevent;
		myevent.m_id = BG_CANVAS_VIEW1_CONFMAP;
		OnUpdatePlotWindow1(myevent);
		myevent.m_id = BG_CANVAS_VIEW2_GRADMAP;
		OnUpdatePlotWindow2(myevent);
	}

	//delete the image processing object
	delete iProc;

	//done.

	//stop the timer
	timer_stop();

	//update the GUI...

	/**********************************************************************************/

	//update the menu bar
	menubar->Enable(BG_SEGM_SAVE_SEGMENTED, true);

	//update tool bar
	SaveEnable();

	//update segmentation window....
	switch(operation)
	{

	//the image has been filtered only....
	case 1:
		viewImSegRadio_->Enable(1, true);
		viewImSegRadio_->Enable(2, false);
		viewImSegRadio_->SetSelection(1);
		break;

	//the image has been segmented only...
	case 2:
		viewImSegRadio_->Enable(2, true);
		viewImSegRadio_->SetSelection(2);
		break;

	//image has been filtered and segmented...
	default:
		viewImSegRadio_->Enable(1, true);
		viewImSegRadio_->Enable(2, true);
		viewImSegRadio_->SetSelection(2);
		break;

	}
	viewImSegRadio_->Enable(3, true);
	viewBoundariesCheck_->Enable(true);
	wxCommandEvent zcev;
	OnViewImSeg(zcev);

	//update the menu bars...
	winPanel1_->EnableMenu(useWeightMap_->GetValue());
	winPanel2_->EnableMenu(useWeightMap_->GetValue());

	/**********************************************************************************/

	//set progress to 100% (need this for case of filtering)
	percentDone = 100;
	
	//indicate that it is safe for the thread to stop executing
	stop_flag	= false; 

	//done.
	return;

}

int BgMdiSegmentChild::GetParameters(int &sigmaS, float &sigmaR, float &aij, float &epsilon, int &minRegion, int &kernelSize, int dataEntryCheck)
{

   double	tempd;
   char		str[10];

   //do not perform data entry check
   if(!dataEntryCheck)
   {
	   //sigmaS
	   txtSigmaS_->GetValue().ToDouble(&tempd);
	   sigmaS			= bgRoundSign(tempd);

	   //sigmaR
	   txtSigmaR_->GetValue().ToDouble(&tempd);
	   sigmaR			= (float) tempd;

	   //minRegion
	   txtMinRegion_->GetValue().ToDouble(&tempd);
	   minRegion		= bgRoundSign(tempd);

	   //kernel size
	   txtKernelSize_->GetValue().ToDouble(&tempd);
	   kernelSize		= bgRoundSign(tempd);

	   //aij
	   txtA_->GetValue().ToDouble(&tempd);
	   aij				= (float) tempd;

	   //epsilon
	   txtEpsilon_->GetValue().ToDouble(&tempd);
	   epsilon			= (float) tempd;

	   //done.
	   return 0;
   }
   
   //perform data entry check
   if ((txtSigmaS_->GetValue().ToDouble(&tempd) == TRUE) && (tempd > 0))
   {
	   sigmaS = (int)(tempd + 0.5);
	   sprintf(str, "%d", sigmaS);
	   txtSigmaS_->SetValue(str);
   }
   else
   {
	   bgLog("The value of the spatial bandwidth cannot be zero or negative.\n");
	   return 1;
   }
   if ((txtSigmaR_->GetValue().ToDouble(&tempd) == TRUE) && (tempd > 0))
	   sigmaR = tempd;
   else
   {
	   bgLog("The value of the range bandwidth cannot be zero or negative.\n");
	   return 1;
   }
   if ((txtMinRegion_->GetValue().ToDouble(&tempd) == TRUE) && (tempd >= 0))
   {
	   minRegion = (int)(tempd + 0.5);
	   sprintf(str, "%d", minRegion);
	   txtMinRegion_->SetValue(str);
   }
   else
   {
	   bgLog("The value of minimum region cannot be negative.\n");
	   return 1;
   }
   if ((txtKernelSize_->GetValue().ToDouble(&tempd) == TRUE) && (tempd > 0))
   {
	   kernelSize = (int)(tempd + 0.5);
	   sprintf(str, "%d", kernelSize);
	   txtKernelSize_->SetValue(str);
   }
   else
   {
	   bgLog("The gradient window radius cannot be zero or negative.\n");
	   return 1;
   }
   if ((txtA_->GetValue().ToDouble(&tempd) == TRUE) && (tempd >= 0) && (tempd <= 1))
   {
	   aij = tempd;
	   if(aij < 0.01) txtA_->SetValue("0.0");
   }
   else
   {
	   bgLog("The value of the mixture parameter cannot be negative or greater than one.\n");
	   return 1;
   }
   if ((txtEpsilon_->GetValue().ToDouble(&tempd) == TRUE) && (tempd >= 0) && (tempd <= 1))
   {
	   epsilon = tempd;
	   if(epsilon < 0.01) txtEpsilon_->SetValue("0.0");
   }
   else
   {
	   bgLog("The threshold value cannot be negative or greater than one.\n");
	   return 1;
   }
   return 0;
}