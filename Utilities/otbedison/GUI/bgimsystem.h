/////////////////////////////////////////////////////////////////////////////
// Name:        bgimsystem.cpp
// Purpose:     Image processing system
// Author:      Bogdan Georgescu, Chris M. Christoudias
// Modified by:
// Created:     06/22/2000
// Copyright:   (c) Bogdan Georgescu, Chris M. Christoudias
// Version:     v0.1
/////////////////////////////////////////////////////////////////////////////

#include <wx/toolbar.h>
#include <wx/image.h>
#include <wx/bitmap.h>
#include <wx/splitter.h>
#include <wx/textfile.h>
#include <wx/textctrl.h>
#include <wx/spinctrl.h>
#include <wx/log.h>

// segmentation defaults
#define BG_SEGM_OP_SIZEX	200
#define BG_SEGM_OP_SIZEY	350
#define BG_SP_WIDTH			170
#define BG_SP_HEIGHT		110
#define BG_SP_HEIGHT_2		93
#define BG_SP_TOP_HEIGHT	70
#define	BG_RIGHT_CELL		112
#define BG_LEFT_CELL		8
#define	HOVER_MENU_X		30
#define	HOVER_MENU_Y		30
#define HOVER_MENU_BOUND	10
#define PLOT_MENU_HEIGHT	25

// end segmentation defaults

#define C_PARAMX 10
#define C_PARAMDX 30
#define C_PARAMY 20
#define C_PARAMDY 30
#define C_PARAMSX 90
#define C_PARAMSY 20
#define SEQ_PANEL_SIZE_X 200
#define SEQ_PANEL_SIZE_Y 30
#define MAX_SEQUENCE 500
#define MAX_POINT_SET 100
#define MAX_LINE_SET 100
#define MAX_CURVE_SET 10

#define MAX_CUSTOM_NODES 100

#define RANK_CONF_IMSIZEX	256
#define RANK_CONF_IMSIZEY	256
#define RANK_CONF_MARGINX	70
#define	RANK_CONF_MARGINY	40

//window type identifier constants
#define BG_EDGE_WINDOW 1
#define BG_SEGM_WINDOW 2

//max image size
#define	MAX_WIDTH	1600
#define	MAX_HEIGHT	1600

//min image size
#define	MIN_WIDTH	16
#define	MIN_HEIGHT	16

//determines zooming action
#define ZOOM_OUT	0
#define ZOOM_IN		1

// log everything to a text window (GUI only of course)
class bgLogTextCtrl : public wxLog
{
public:
    bgLogTextCtrl(wxTextCtrl *pTextCtrl);

private:
    // implement sink function
    virtual void DoLogString(const wxChar *szString, time_t t);

    // the control we use
    wxTextCtrl *m_pTextCtrl;
};

// Define a new application
class BgApp : public wxApp
{
public:
   bool OnInit();
};





class BgPointSet
{
public:
   int* x_;
   int* y_;
   int n_;
   wxPen pen_;
   int type_; // 0 circle
              // 1 point

   BgPointSet();
   ~BgPointSet();
   void CleanData();
   void SetPoints(int*, int* ,int);
};

class BgLineSet
{
public:
   int* xs_;
   int* xe_;
   int* ys_;
   int* ye_;
   double* lineParam_;
   int n_;
   wxPen pen_;

   BgLineSet();
   ~BgLineSet();
   void CleanData();
   void SetLines(int*, int*, int*, int*, double*, int);
   void SetLines(double*, double*, double*, int n);
};

class BgCurveSet
{
public:
   int type_; // standard curve types
   int* x_;
   int* y_;
   int xs_;
   int ys_;
   int n_;
   wxPen pen_;

   // dragging stuff
   int isDragging_;
   int ltodrag_;
   
   BgCurveSet();
   ~BgCurveSet();
   void CleanData();
   void SetParamCurve(int, double*, double*, int, int, int);
   void GetParamCurve(double*, double*, int&, int&);
   void DrawYourself(unsigned char*, int);
   void DrawLine(unsigned char* buf, int, int, int, int, int);
   void DrawPoint(unsigned char* buf,int , int, int);
   void DrawEllipticArc(unsigned char* buf, int, int, int, int, int, int, int);
   void SetCurve(BgCurveSet*);
   void StartDragging(int, int);
   void DragTo(int, int);
   void EndDragging(int, int);
};

class BgText
{
public:
	char	*text_;
	wxFont	*font_;
	int		x_, y_;
	int		id_;
	
	BgText(void);
	BgText(int,char*,wxFont,int,int);
	~BgText(void);
	void SetText(char*);
	void SetFont(wxFont);
	void SetId(int);
	void SetPlotLocation(int,int);
};

class BgTextObj
{
public:
	BgText *text_;
	BgTextObj	*next_;

	BgTextObj(void);
	BgTextObj(BgText*);
	~BgTextObj(void);
};

class BgTextList
{
public:
	BgTextList(void);
	~BgTextList(void);
	int AddText (BgText*);
	int RemoveText(int);
	BgText *GetText(void);
	int	GetTextCount(void);
	void ResetList(void);

private:
	BgTextObj *head_, *cur_;
	int	itemcount_;
	void DeleteText(void);
};

class BgBitmap
{
public:
	wxBitmap	*bitmap_;
	int			location_x_, location_y_;
	int			id_;

	BgBitmap(void);
	BgBitmap(wxBitmap*, int, int, int);
	~BgBitmap(void);
	void SetMap(wxBitmap*);
	void SetPlotLocation(int,int);
	void SetId(int);
};

class BgBitmapObj
{
public:
	BgBitmap	*bitmap_;
	BgBitmapObj	*next_;

	BgBitmapObj(void);
	BgBitmapObj(BgBitmap*);
	~BgBitmapObj(void);
};

class BgBitmapList
{
public:
	BgBitmapList(void);
	~BgBitmapList(void);
	int AddBitmap (BgBitmap*);
	void RemoveBitmap(BgBitmap*);
	BgBitmap *GetBitmap(void);
	void ResetList(void);
	int	GetBitmapCount(void);

private:
	BgBitmapObj *head_, *cur_;
	int	itemcount_;
	void DeleteBitmap(void);
};

class BgAxis
{
public:
	int		start_x_, start_y_;		//plotting origin of axis
	int		length_;				//axis length
	int		direction_;				//0 - horizontal, 1 - vertical
	int		ticknum_;				//number of ticks
	float	start_val_, stop_val_;	//starting and stopping location of axis
	BgText	*label_;				//axis label
	int		label_x_, label_y_;		//axis label plot location
	int		rotation_;				//sets rotation

	BgAxis(void);
	BgAxis(int, int, int, int, int, float, float);
	~BgAxis(void);

	void SetPlotOrigin(int, int);
	void SetLength(int);
	void SetDirection(int);
	void SetTicknum(int);
	void SetBounds(float,float);
	void Label(BgText*);
	void RemoveLabel(void);
	void SetLabelRotation(int);
	void PlotAxis(wxDC *dc);
};

class BgThread : public wxThread {
public:
	BgThread(wxThreadKind, void foo(void*), void*);
	~BgThread( void );

private:

	//determines the entry point of the thread (foo)
	void *Entry( void );

	//pointer to the function foo and its corresponding object
	void *Object_;
	void (*function_)(void*);
	
};

//Custom dialog box that allows for the addition of bitmaps
//and bold text (etc)
class BgDialog : public wxDialog {
public:
	BgDialog(wxWindow*, wxWindowID, const wxString&, const wxPoint&, const wxSize&, long, const wxString&);
	~BgDialog(void);

	//add/remove objects....
	void AddText(BgText *);
	void RemoveText(int);
	void AddBitmap(BgBitmap *);
	void RemoveBitmap(BgBitmap *);

	//paint/close dialog
	virtual void OnPaint(wxPaintEvent& );
	void OnExit(wxCommandEvent& );
	wxButton* okButton_;

	//declare an event table for this class
	DECLARE_EVENT_TABLE();

private:

	//text/bitmap lists...
	BgTextList		tlist_;
	BgBitmapList	blist_;

};

class BgHoverBar : public wxWindow
{
public:
	BgHoverBar(wxWindow*, wxWindowID, int, int, int, int, int, int, int);
	~BgHoverBar( void );

	wxBitmapButton* menuButton1_;
	wxBitmapButton* menuButton2_;
	wxStaticText* menuText1_;
	wxStaticText* menuText2_;
	wxMenu* view_menu;
	wxMenu* save_menu;

	void ShowMenu(wxCommandEvent& );
	void CheckViewItem(long);
	void Update( void );

	int	gradViewId_, confViewId_, weitViewId_, custViewId_;
	int	gradSaveId_, confSaveId_, weitSaveId_;

	//declare an event table for this class
	DECLARE_EVENT_TABLE();
};

class BgMenuPanel : public wxPanel
{
public:
	BgMenuPanel(wxWindow*, wxWindowID, int, int, int, int, int, int, int);
	~BgMenuPanel( void );

	wxBitmapButton* menuButton1_;
	wxBitmapButton* menuButton2_;
	wxStaticText* menuText1_;
	wxStaticText* menuText2_;
	wxMenu* view_menu;
	wxMenu* save_menu;
	wxWindow* scrollWindow_;

	void ShowMenu(wxCommandEvent& );
	void CheckViewItem(long);
	void Update( void );
	void EnableMenu(bool);
	void OnSize(wxSizeEvent& );
	void SetScrollWindow(wxWindow*);

	int	gradViewId_, confViewId_, weitViewId_, custViewId_;
	int	gradSaveId_, confSaveId_, weitSaveId_;

	//declare an event table for this class
	DECLARE_EVENT_TABLE();
};

class BgImCanvas : public wxScrolledWindow
{
public:

   BgImCanvas(wxWindow* child_frame, wxWindow *parent, const wxPoint& pos, const wxSize& size);
   ~BgImCanvas();
   virtual void OnDraw(wxDC& dc);
//   void OnEraseBackground(wxEraseEvent& );
   bool IsDirty() const { return m_dirty; }

   void OnEvent(wxMouseEvent&);
   void OnMouseRightDown(wxMouseEvent&);
   //void OnScroll(wxScrollWinEvent&);
   void AdjustScroll(int,int,int);
   
   wxBitmap *pbitmap;
   wxImage *pimage;
   bool hasbitmap;
   bool showbitmap_;

   wxMenu *localMenu_;
   int lmEventX_;
   int lmEventY_;
   int lmEventCurve_;
   int lmEventNode_;

   BgPointSet* pointSet_[MAX_POINT_SET];
   int nPointSets_;
   BgLineSet* lineSet_[MAX_LINE_SET];
   int nLineSets_;

   BgCurveSet* curveSet_[MAX_CURVE_SET];
   int nCurveSets_;

   int showtrack_;
   int trackval_[2];
   int hx_, hy_;

   // draging stuff
   int isDragging_;
   BgCurveSet dragCurve_;
   unsigned char* curveClick_;
   int ccx_;
   int ccy_;
   void FillCurveClick();
   int mouseModif_;
   int crossCursor_;
   int m_x_, m_y_;

   //keep track of certain the window events
   bool	has_focus;
   bool	leaving;

   //text list object
   BgTextList tlist_;

   //keep track of the number of un-plotted
   //text objects
   int	textObjectCount_;

   //x and y axis objects
   BgAxis	*xAxis, *yAxis;

   //list of bitmaps
   BgBitmapList blist_;

   //clear display
   bool	clear_display_;

   //do not update display
   bool	noUpdate_;

   //determines if zooming is requested
   bool	zoom_in, zoom_out, zoom_window;

   //zooming window parameters
   int	zoom_level, max_zoom_level, min_zoom_level;

   //zoom buffers
   unsigned char *buf;

   //zoom cursors
   wxCursor	*bgCURSOR_MAGNIFIER_PLUS;
   wxCursor *bgCURSOR_MAGNIFIER_MINUS;

   //maintain copy of zoomed image
   unsigned char *zImg;

   //define zoom box image
   wxImage	*zoombox;

   //defein refresh box image
   wxImage	*refresh_box;

   //maintain point map for reconstruction
   //of image boundaries
   bool		*point_map;
   wxColour	*point_colour;

   //define upper corner of zoom and refresh box
   int cx, cy;

   //popup menu window
   int	menuXl_, menuXu_, menuYl_, menuYu_;
   int	popup;
   wxWindow* menuWindow;

   void AddPointSet(BgPointSet*);
   void RemovePointSet(BgPointSet*);
   void AddLineSet(BgLineSet*);
   void RemoveLineSet(BgLineSet*);
   void AddCurveSet(BgCurveSet*);
   void RemoveCurveSet(BgCurveSet*);
   void AddText(BgText*);
   void RemoveText(int);
   void AddHorizontalAxis(int,int,int,int,float,float);
   void AddVerticalAxis	 (int,int,int,int,float,float);
   void ClearAxis();
   void LabelHorizontalAxis(BgText*);
   void LabelVerticalAxis(BgText*);
   void RemoveHorizontalAxisLabel(void);
   void RemoveVerticalAxisLabel(void);
   void RotateHorizontalAxisLabel(int);
   void RotateVerticalAxisLabel(int);
   void AddBitmap(BgBitmap*);
   void RemoveBitmap(BgBitmap*);
   void ClearDisplay(void);

   void AddTrackSet(int, int, int, int);
   void RemoveTrackSet();

   int	SetImage(wxString imname);
   void SetImage(wxImage& image);
   void SetSameImage(wxImage& image);
   void SetImageFromGray(unsigned char* data, int w, int h);
   void SetImage(unsigned char* data, int w, int h, bool color = false);
   void ShowBitmap(bool);
   void ClearData(int refresh=0);
   void AddMargin(int, int);

   int	Zoom(int);
   int	Zoom(int,int,int);
   int	ZoomIn(int, int);
   int	ZoomOut(int, int);
   void SetMaxZoomLevel(int);
   void SetMinZoomLevel(int);
   void DisplayZoomWindow(int,int);

   void OnCustomAddNode(wxCommandEvent&);
   void OnCustomDeleteNode(wxCommandEvent&);
   void OnCTypeEllipse(wxCommandEvent&);
   void OnCTypeVLine(wxCommandEvent&);
   void OnCTypeHLine(wxCommandEvent&);
   void OnCTypeLine(wxCommandEvent&);
   void OnCTypeBox(wxCommandEvent&);
   void OnCTypeCustom(wxCommandEvent&);

   void AddHoverWindow(wxWindow*, int);
   void SetHoverWindowLocation(int, int);

   //*************************

private:
   bool m_dirty;
   int	x_offset_, y_offset_;
   void MyDrawEllipticArc(wxDC& dc, int x, int y, int w, int h, int sa, int ea);
   wxWindow	*child_frame_;

   //used for zoom window
   void DefineZoomBox(int,int,int,int);
   void DefineRefreshBox(void);
   void CreatePointMap(int,int);
   void AddPoints(unsigned char*, int);

   DECLARE_EVENT_TABLE()
};


class BgParameterHistory {
public:
	BgParameterHistory( void );
	BgParameterHistory(void*, int);
	~BgParameterHistory();

	void				*params_;
	int					listSize_;
	BgParameterHistory	*next_;
};


//parameter history box tool
//IMPORTANT: WHEN ADDING A PARAMETER LIST, THIS OBJECT OBTAINS OWNERSHIP
//			 OF THAT LIST (I.E. IT WILL DELETE THE MEMORY FOR YOU!!!!).
class BgParameterHistoryBox : public wxComboBox {
public:
	BgParameterHistoryBox(wxWindow*, wxWindowID, const wxString&, const wxPoint&, const wxSize&, int, long, const wxValidator&, const wxString&);
	~BgParameterHistoryBox( void );

	//add/get parameter lists
	void AddParameterList(void*, int);
	void *GetParameterListData(int);
	int GetParameterListCount(int);

	//keep track of parameter history
	void SetCurrentList(void*, int);
	void* GetCurrentListData( void );
	int GetCurrentListCount( void );
	void UseParameterList(int);

private:

	//paramter list array...
	BgParameterHistory	*historyList_, currentList_;
	int					listCount_, maxCount_;

};


// Define the parameter dialog box

class BgParamDialog : public wxDialog
{
public:
   BgParamDialog(wxWindow* parent, wxWindowID id, const wxString& title,
      const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
      long style = wxDEFAULT_DIALOG_STYLE, const wxString& name = "dialogBox");
   ~BgParamDialog();
   void OnOk(wxCommandEvent& );
   void OnCancel(wxCommandEvent& );
   void SetValues(double, double, double, double, double, double, int, int, int, int, int);
   void GetValues(double&, double&, double&, double&, double&, double&, int&, int&, int&, int&, int&);

   // dialog parameters
   wxButton* okButton_;
   wxButton* cancelButton_;

   wxStaticText* txtNmxR_;
   wxStaticText* txtNmxC_;
   wxStaticText* txtHHR_;
   wxStaticText* txtHHC_;
   wxStaticText* txtHLR_;
   wxStaticText* txtHLC_;
   wxStaticText* txtMinPt_;
   wxStaticText* txtNmxType_;
   wxStaticText* txtHHType_;
   wxStaticText* txtHLType_;
   wxStaticText* txtKernelSize_;

   wxTextCtrl* valNmxR_;
   wxTextCtrl* valNmxC_;
   wxTextCtrl* valHHR_;
   wxTextCtrl* valHHC_;
   wxTextCtrl* valHLR_;
   wxTextCtrl* valHLC_;
   wxTextCtrl* valMinPt_;
   wxChoice* valNmxType_;
   wxChoice* valHHType_;
   wxChoice* valHLType_;
   wxTextCtrl* valKernelSize_;


DECLARE_EVENT_TABLE()
};

class BgSpeedSelect : public wxDialog
{
public:
   BgSpeedSelect(wxWindow* parent, wxWindowID id, const wxString& title,
      const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
      long style = wxDEFAULT_DIALOG_STYLE, const wxString& name = "dialogBox");
   ~BgSpeedSelect();
   void OnOk(wxCommandEvent& );
   void OnCancel(wxCommandEvent& );
   void SetSliderValue(float);
   void GetSliderValue(float&);

   // dialog parameters
   wxButton* okButton_;
   wxButton* cancelButton_;

   wxStaticText* txtQuality_;
   wxStaticText* txtSpeed_;

   wxSlider *sldSpeed_;
DECLARE_EVENT_TABLE()
};


// Define a new frame
class BgMdiFrame : public wxMDIParentFrame
{
public:
//   wxTextCtrl *textWindow;
//   wxLogWindow* logwind_;
    wxTextCtrl    *logtext_;
    wxLog *logTargetOld_;
    int logsize_;
   bgLogTextCtrl* bglogctrl_;
   char programDir_[1000];
   char helpDir_[1000];

   BgMdiFrame(wxWindow *parent, const wxWindowID id, const wxString& title,
      const wxPoint& pos, const wxSize& size, const long style);

   void InitToolBar(wxToolBar* toolBar);
   
   void	GetImageFileInfo(char**, char**);
   void ZoomControl(wxCommandEvent& );
   void OnSize(wxSizeEvent&);
   void OnAbout(wxCommandEvent&);
   void OnHelp(wxCommandEvent&);
   void OnNewEdgeWindow(wxCommandEvent&);
   void OnNewSegmWindow(wxCommandEvent&);
   void OnLoadImage(wxCommandEvent&);
   void OnLoadImageEdge(wxCommandEvent&);
   void OnLoadImageSegm(wxCommandEvent&);
   void OnSaveResult(wxCommandEvent& WXUNUSED(event));
   void OnQuit(wxCommandEvent&);
   void OnClose(wxCloseEvent&);
   void SetChildTitle(wxMDIChildFrame*, int, int, int);
   void UpdateZoomControl(wxMDIChildFrame*, int, int);

   DECLARE_EVENT_TABLE()
};

class BgMdiEdgeChild: public wxMDIChildFrame
{
public:
   //edge detection stuff
   double rankNmx_;
   double confNmx_;
   double rankH_;
   double confH_;
   double rankL_;
   double confL_;
   int nMin_;
   int nmxType_;
   int hystHighType_;
   int hystLowType_;
   int kernelSize_;

   double custHx_[MAX_CUSTOM_NODES];
   double custHy_[MAX_CUSTOM_NODES];
   int nCustH_;
   double custLx_[MAX_CUSTOM_NODES];
   double custLy_[MAX_CUSTOM_NODES];
   int nCustL_;

   BgEdgeDetect* cbgEdgeDetect_;
   BgImage* cbgImage_;
   BgEdgeList* cbgEdgeList_;
   BgPointSet cbgPointSet_;

   //window stuff
   wxSplitterWindow* imagePlotSplitter_;
   wxSplitterWindow* plotSplitter_;

   BgImCanvas* origEdgeImage_;
   BgImCanvas* plotNmxImage_;
   BgImCanvas* plotTotImage_;

   BgCurveSet highCurve_;
   BgCurveSet lowCurve_;
   BgCurveSet nmxCurve_;

   int hasEdge_;
   int hasImage_;

   wxMenuItem* miViewOrig_;
   wxMenuItem* miViewEdge_;

   // right panel stuff
   int bpsize_;
   int rightsize_;
   wxPanel* buttonPanel_;
   wxButton* edButton_;
   wxButton* cpButton_;
   wxCheckBox* viewOrigCheck_;
   wxCheckBox* viewEdgeCheck_;

   // more right panel stuff
   wxStaticText* txtNmxR_;
   wxStaticText* txtNmxC_;
   wxStaticText* txtHHR_;
   wxStaticText* txtHHC_;
   wxStaticText* txtHLR_;
   wxStaticText* txtHLC_;
   wxStaticText* txtMinPt_;
   wxStaticText* txtNmxType_;
   wxStaticText* txtHHType_;
   wxStaticText* txtHLType_;
   wxStaticText* txtKernelSize_;

   wxStaticText* valNmxR_;
   wxStaticText* valNmxC_;
   wxStaticText* valHHR_;
   wxStaticText* valHHC_;
   wxStaticText* valHLR_;
   wxStaticText* valHLC_;
   wxStaticText* valMinPt_;
   wxStaticText* valNmxType_;
   wxStaticText* valHHType_;
   wxStaticText* valHLType_;
   wxStaticText* valKernelSize_;

   int loopset_;
   //parent toolbar
   wxToolBar *toolbar;

   //keep track of current number of frames open
   bool window_open;

   //keep track of max/min zoom
   bool	maxZoom_, minZoom_;

   //keep track of filename and window number
   char *filename_;
   int	window_number_;

   //keep track of display image width, height, and zoom level
   int	width_, height_;

   BgMdiEdgeChild(wxMDIParentFrame *parent, const wxString& title, const wxPoint& pos, const wxSize& size, const long style);
   ~BgMdiEdgeChild();
   void OnQuit(wxCommandEvent& );
   void OnClose(wxCloseEvent& );
   void OnFocus(wxFocusEvent& );
   void NoZoom(void);
   void ZoomWindow(void);
   void ZoomIn(void);
   void ZoomOut(void);
   void UpdateZoomControl(void);
   void RunEnable(void);
   void SaveEnable(void);
   void UpdateToolBar(void);
   void ResetToolBar(void);
   void ReadImage(char*, char*);
   void OnLoadImage(wxCommandEvent& );
   void OnSaveEdgeMap(wxCommandEvent& );
   void OnEdgeDetect(wxCommandEvent& );
   void OnChangeParam(wxCommandEvent& );
   void OnViewOrig(wxCommandEvent& );
   void OnViewEdge(wxCommandEvent& );
   void OnCViewOrig(wxCommandEvent& );
   void OnCViewEdge(wxCommandEvent& );
   void OnSize(wxSizeEvent& );
   void SetTotalImage(void);
   void SetNmxImage(void);

   // param events
   void SetParametersNum(void);
   void SetParametersStr(void);
   void OnUpdateNum(wxCommandEvent& );

   DECLARE_EVENT_TABLE()
};

class BgMdiSegmentChild: public wxMDIChildFrame
{
public:
   BgImage* cbgImage_;
   BgImage* filtImage_;
   BgImage* segmImage_;
   BgImage* whiteImage_;

   //segmemtation parameters
   int		sigmaS, minRegion, kernelSize;
   float	sigmaR, aij, epsilon, *gradMap_, *confMap_, *weightMap_, *customMap_;
   bool		edgeParamsHaveChanged_;
   SpeedUpLevel	speedUpLevel_;
   float speedUpThreshold_;

   //point set used to draw boundaries
   BgPointSet* boundaries_;

   //window stuff
   BgImCanvas* displayImage_;

   //confidence map image
   BgImCanvas* plotWindow1_;

   //rank map image
   BgImCanvas* plotWindow2_;

   //ph diagram
   BgImCanvas* phDiagram_;

   int hasBoundaries_;
   int hasImage_;
   int hasFilter_;
   int hasSegment_;

   // left panel stuff
   wxPanel*	optionsPanel_;
   wxPanel* subPanel1_;
   wxPanel* subPanel2_;
   wxPanel* subPanel3_;
   BgMenuPanel*	winPanel1_;
   BgMenuPanel* winPanel2_;
   wxStaticBox* subPanelBox1_;
   wxStaticBox* subPanelBox2_;
   wxStaticBox* subPanelBox3_;
   wxStaticBox* subPanelBox4_;

   wxButton* runButton_;
   wxButton* loadButton_;

   wxRadioBox* viewImSegRadio_;
   wxRadioBox* operationRadio_;
   wxCheckBox* viewBoundariesCheck_;

   wxStaticText* textKernelSize_;
   wxTextCtrl* txtKernelSize_;

   wxStaticText* textSigmaR_;
   wxTextCtrl* txtSigmaR_;

   wxStaticText* textSigmaS_;
   wxTextCtrl* txtSigmaS_;

   wxStaticText* textA_;
   wxTextCtrl* txtA_;

   wxStaticText* textEpsilon_;
   wxTextCtrl* txtEpsilon_;

   wxStaticText* textMinRegion_;
   wxTextCtrl* txtMinRegion_;

   bool	isCurrentHistory_;
   bool checkTextBoxes_;
   wxStaticText* textParamBox_;
   BgParameterHistoryBox*	paramComboBox_;

   wxCheckBox* useWeightMap_;

   //used to split the window to display ph diagram
   wxSplitterWindow* imagePlotSplitter_;
   wxSplitterWindow* plotMapSplitter_;
   wxSplitterWindow* mapSplitter_;

   //store pointer to parent frame toolbar
   wxToolBar *toolbar;

   //keep track of current number of frames open
   bool window_open;

   //keep track of filename and window number
   char *filename_;
   int	window_number_;

   //keep track of display image width, height, and zoom level
   int	width_, height_;

   //keep track if max/min zoom occurred
   int	maxZoom_, minZoom_;

   BgMdiSegmentChild(wxMDIParentFrame *parent, const wxString& title, const wxPoint& pos, const wxSize& size, const long style);
   ~BgMdiSegmentChild();

   void OnQuit(wxCommandEvent& );
   void OnClose(wxCloseEvent& );
   void OnFocus(wxFocusEvent& );
   void ZoomWindow(void);
   void ZoomIn(void);
   void ZoomOut(void);
   void NoZoom(void);
   void UpdateZoomControl(void);
   void RunEnable(void);
   void SaveEnable(void);
   void UpdateToolBar(void);
   void ResetToolBar(void);
   void ReadImage(char*, char*);
   void OnLoadImage(wxCommandEvent& );
   void LoadCustomWeightMap(wxCommandEvent&);
   void OnSaveSegmentedImage(wxCommandEvent& );
   void OnSaveBoundaries(char*, int);
   void OnSegment(wxCommandEvent& );
   void Segment(void);
   void SetphDiagram(int, float*, float*);
   void OnViewImSeg(wxCommandEvent& );
   void OnChangeOperation(wxCommandEvent& );
   void OnChangeParameters(wxCommandEvent& );
   void	OnUpdateTextBoxes(wxCommandEvent& );
   void OnUpdateSpeedUpLevel(wxCommandEvent& );
   void OnViewBoundaries(wxCommandEvent& );
   void OnUseWeightMap(wxCommandEvent& );
   void OnUpdatePlotWindow1(wxCommandEvent& );
   void OnUpdatePlotWindow2(wxCommandEvent& );
   void OnSaveEdgeInformation(wxCommandEvent& );
   //void OnChangeView
   void OnSize(wxSizeEvent& );
   void ClearDisplay( void );
   void UpdateDisplay(bool);
   int  GetParameters(int&, float&, float&, float&, int&, int&, int);
   DECLARE_EVENT_TABLE()
};

// menu items ids
enum
{
   BG_QUIT = 100,
	  BG_MENU_WINDOW,
      BG_DIALOG_OK,
	  BG_CANVAS_VIEW1_GRADMAP,
	  BG_CANVAS_VIEW1_CONFMAP,
	  BG_CANVAS_VIEW1_WEITMAP,
	  BG_CANVAS_VIEW1_CUSTMAP,
	  BG_CANVAS_VIEW2_GRADMAP,
	  BG_CANVAS_VIEW2_CONFMAP,
	  BG_CANVAS_VIEW2_WEITMAP,
	  BG_CANVAS_VIEW2_CUSTMAP,
	  BG_CANVAS_VIEW_BUTTON,
	  BG_CANVAS_SAVE_GRADMAP,
	  BG_CANVAS_SAVE_CONFMAP,
	  BG_CANVAS_SAVE_WEITMAP,
	  BG_CANVAS_SAVE_BUTTON,
      BG_NEW_EDGE_WINDOW,
      BG_NEW_SEGM_WINDOW,
      BG_LOAD_IMAGE_EDGE,
      BG_LOAD_IMAGE_IMAGE,
      BG_EDGE_SAVE_MAP,
      BG_EDGE_DETECT,
      BG_EDGE_VIEW_ORIG,
      BG_EDGE_VIEW_EDGE,
      BG_EDGE_CVIEW_ORIG,
      BG_EDGE_CVIEW_EDGE,
      BG_EDGE_PARAM_HHR,
      BG_CHANGE_PARAM_EDGE,
      BG_REFRESH,
      BG_CHANGE_TITLE,
      BG_CHILD_EDGE_QUIT,
      BG_CHILD_SEGM_QUIT,
      BG_ABOUT,
      BG_HELP,
      BG_PARAMD_OK,
      BG_PARAMD_CANCEL,
      BG_SPEEDSEL_OK,
      BG_SPEEDSEL_CANCEL,
      BG_SPEEDSEL_SLD,
      BG_IMC_ADDNODE,
      BG_IMC_DELETENODE,
      BG_IMC_SELTYPE_ELLIPSE,
      BG_IMC_SELTYPE_VLINE,
      BG_IMC_SELTYPE_HLINE,
      BG_IMC_SELTYPE_LINE,
      BG_IMC_SELTYPE_BOX,
      BG_IMC_SELTYPE_CUSTOM,
	  BG_SEGM_SPEEDUP,
	  BG_SEGM_SPEEDUP_NONE,
	  BG_SEGM_SPEEDUP_MEDM,
	  BG_SEGM_SPEEDUP_HIGH,
      BG_SEGM_LOAD_IMAGE,
	  BG_SEGM_LOAD_MAP,
	  BG_SEGM_USE_MAP,
      BG_SEGM_SAVE_SEGMENTED,
      BG_SEGM_SAVE_EDGEMAP,
      BG_SEGM_SEGMENT,
      BG_SEGM_VIEW_IMSEG,
      BG_SEGM_VIEW_EDGES,
	  BG_SEGM_OPERATION,
	  BG_SEGM_USE_EDGE_MAP,
	  BG_SEGM_CHANGE_PARAMS,
	  BG_SEGM_TEXT_SIGMAS,
	  BG_SEGM_TEXT_SIGMAR,
	  BG_SEGM_TEXT_MINREG,
	  BG_SEGM_TEXT_GRADWIN,
	  BG_SEGM_TEXT_AIJ,
	  BG_SEGM_TEXT_EPSILON,
	  BG_LOAD_IMAGE,
	  BG_SAVE_RESULT,
	  BG_CROSS,
	  BG_ZOOM_IN,
	  BG_ZOOM_OUT,
	  BG_POINTER,
     BG_EVENT_UPDATE,
     BG_EVENT_UPDATE_ID
};
