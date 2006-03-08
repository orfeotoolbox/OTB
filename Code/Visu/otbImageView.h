#ifndef _otbImageView_h
#define _otbImageView_h

/*
* This is a cross-platform image viewer for 3D medical images.
* See the class definition for details...
*/

#include <itkProcessObject.h>
#include <itkImage.h>
#include <fltkUtils.h>
#include <list>
#include <FL/fl_file_chooser.H>
#include <fstream>

//using namespace itk;

namespace otb
{
  
/*! Clicking in a window will cause different events
*  NOP = nothing
*  SELECT = report pixel info
*/
const int NUM_ClickModeTypes = 3;
typedef enum {CM_NOP, CM_SELECT, CM_BOX} ClickModeType;
const char ClickModeTypeName[3][7] =
  {{'N', 'O', 'P', '\0', ' ', ' ', ' '},
  {'S', 'e', 'l', 'e', 'c', 't', '\0'},
  {'B', 'o', 'x', '\0', ' ', ' ', ' '}};

  /*! Handling of values outside intensity window range - values above 
  *    and below can be handled separately
  *  IW_MIN = set values outside range to min value
  *  IW_MAX = set values outside range to max value
  *  IW_FLIP = rescale values to be within range by flipping
*/
const int NUM_ImageModeTypes = 8;
typedef enum {IMG_VAL, IMG_INV, IMG_LOG, IMG_DX, IMG_DY, IMG_DZ,
  IMG_BLEND, IMG_MIP} ImageModeType;
const char ImageModeTypeName[8][8] =
  {{'V', 'a', 'l', 'u', 'e', '\0', ' ', ' '},
  {'I', 'n', 'v', 'e', 'r', 's', 'e', '\0'},
  {'L', 'o', 'g', '\0', ' ', ' ', ' ', ' '},
  {'D', 'e', 'r', 'i', 'v', '-', 'X', '\0'},
  {'D', 'e', 'r', 'i', 'v', '-', 'Y', '\0'},
  {'D', 'e', 'r', 'i', 'v', '-', 'Z', '\0'},
  {'B', 'l', 'e', 'n', 'd', '\0', ' ', ' '},
  {'M', 'I', 'P', '\0', ' ', ' ', ' ', ' '}};

const int NUM_IWModeTypes = 3;
typedef enum {IW_MIN, IW_MAX, IW_FLIP} IWModeType;
const char IWModeTypeName[3][5] =
  {{'M', 'i', 'n', '\0', ' '},
  {'M', 'a', 'x', '\0', ' '},
  {'F', 'l', 'i', 'p', '\0'}};

  /*! Structure clickPoint to store the x,y,z and intensity value of a
  * point in the image
*/
class ClickPoint 
  {
  public:
    ClickPoint()
    : x(0),y(0),z(0),value(0){}
    ClickPoint( const ClickPoint & p )
    { x = p.x; y = p.y; z = p.z; value = p.value; }
    ClickPoint(float _x,float _y,float _z,double v)
    : x(_x),y(_y),z(_z),value(v){}

  public:
    float x, y, z;
    double value;
  };


/*! Multifunction slice-by-slice viewer
*  Allows for viewing the coronal, axial, and sagittal slices of
*   a 3D volume. Operates independent of any GUI/via system in this
*   base class (except fltkUtils.h - which could also be abstracted).
*   To learn all of the available viewing options when viewing an image,
*   press 'h' in the window.
*  This is a virtual base class, which provides most of the data 
*   members and methods slice viewers would need.
*  To get a concrete slice-viewer, inherit this class along with a 
*   windowing class (example: Fl_window or Fl_Gl_Window).
*
*  /author Stephen R. Aylward
*
*  /date 11/22/99
*/
template <class imType>
class ITK_EXPORT ImageView : public itk::ProcessObject
{
  public:
    typedef ImageView                           Self;
    typedef itk::ProcessObject                  Superclass;
    typedef itk::SmartPointer<Self>             Pointer;
    typedef itk::SmartPointer<const Self>       ConstPointer;

    /** Method for creation through the object factory. */
    itkNewMacro(Self);

    /** Run-time type information (and related methods). */
    itkTypeMacro(ImageView,itk::ProcessObject);

    // some typedefs
    typedef itk::Image<imType,3>                ImageType;
    typedef typename ImageType::Pointer         ImagePointer;
    typedef typename ImageType::ConstPointer    ImageConstPointer;

   
  protected:
    void   (* cSliceNumCallBack)(void);
    void    * cSliceNumArg;
    void   (* cSliceNumArgCallBack)(void * sliceNumArg);
    
    bool                     cValidImData;
    bool                     cViewImData;
    bool                     cViewClickedPoints;
//    ImagePointer             cImData;

    unsigned long            cDimSize[3];
    float                    cOrigin[3];
    float                    cSpacing[3];
    void                    (* cViewImDataCallBack)(void);
    void                     * cViewImDataArg;
    void                    (* cViewImDataArgCallBack)(void *viewImDataArg);
    
    ClickModeType cClickMode;
    float         cClickSelect[3];
    float         cClickSelectV;
    void          (* cClickSelectCallBack)(float x,float y,float z,
                                           float v);
    void           * cClickSelectArg;
    void          (* cClickSelectArgCallBack)(float x, float y, float z, 
                                              float v, void *clickSelectArg);
    
    float       cBoxMin[3];
    float       cBoxMax[3];
    void        (* cClickBoxCallBack)(float minX, float minY, float minZ, 
                                      float maxX, float maxY, float maxZ);
    void         * cClickBoxArg;
    void        (* cClickBoxArgCallBack)(float minX, float minY, float minZ,
                                         float maxX, float maxY, float maxZ,
                                         void * clickBoxArg);
    
    float       cIWMin;
    float       cIWMax;
    IWModeType  cIWModeMin;
    IWModeType  cIWModeMax;
    void        (* cIWCallBack)(void);
    void         * cIWArg;
    void        (* cIWArgCallBack)(void * iwArg);
    
    ImageModeType cImageMode;
    
    bool        cFlipX[3];
    bool        cFlipY[3];
    bool        cFlipZ[3];
    bool        cTranspose[3];
    
    float               cWinZoom;
    unsigned int        cWinOrder[3];
    unsigned int        cWinOrientation;
    void                (* cWinOrientationCallBack)(void);
    void                 * cWinOrientationArg;
    void                (* cWinOrientationArgCallBack)(void * 
                                                       winOrientationArg);
    
    int         cWinCenter[3];
    void        (* cWinCenterCallBack)(void);
    void        * cWinCenterArg;
    void        (* cWinCenterArgCallBack)(void * winCenterArg);
    
    bool        cViewAxisLabel;
    char        cAxisLabelX[3][80];
    char        cAxisLabelY[3][80];
    
    bool        cViewOverlayData;
    bool        cViewCrosshairs;
    bool        cViewValue;
    bool        cViewValuePhysicalUnits;
    char *      cPhysicalUnitsName;
    bool        cViewDetails;
    
    int   cWinMinX;
    int   cWinMaxX;
    unsigned int   cWinSizeX;
    int   cWinMinY;
    int   cWinMaxY;
    unsigned int   cWinSizeY;
    int   cWinDataSizeX;
    int   cWinDataSizeY;
    unsigned int   inDataSizeX;
    unsigned int   inDataSizeY;
    unsigned char  *cWinImData;
    unsigned short *cWinZBuffer;
    
    double cDataMax, cDataMin;
    
    /* list of points clicked and maximum no. of points to be stored*/
    std::list< ClickPoint > cClickedPoints;
    unsigned int maxClickPoints;
    int cX, cY, cW, cH;
    int fastMovVal; //fast moving pace
    int fastMovThresh; //how many single step moves before fast moving
    
    /*! ImageView Constructor */
//    ImageView(int x, int y, int w, int h, const char *l=0);
    ImageView();
    /*! Standard destructor */
    virtual ~ImageView(void);

  public:

    virtual void Init(int x, int y, int w, int h, const char *l=0);
    
    /*! Specify the 3D image to view in slices - pure virtual function*/
//    virtual void  SetInputImage( ImageType * newImData ) = 0;
    
    /*! Return a pointer to the image data */
    
//    const ImageConstPointer & GetInputImage(void) const;
    const ImageType * GetInput(void) const;
    
    /*! Dammit, give me a pointer to the image that's not const! */
//    ImagePointer GetInputImage(void);
    ImageType * GetInput(void);
    
    /*! Return a pointer to the pixels being displayed */
    unsigned char *         winImData(void);
    
    /*! Return the window x-dim */
    
    unsigned int    winDataSizeX(void);
    
    /*! Return the window y-dim */
    
    unsigned int    winDataSizeY(void);
    /*! Turn on/off the viewing of the image */
    void          viewImData(bool newViewImData);
    /*! Status of the image - viewed / not viewed */
    bool            viewImData(void);
    /*! Called when viewing of the image is toggled */
    void            viewImDataCallBack(
                          void (* newViewImDataCallBack)(void));
    void            viewImDataCallBack(
                          void (* newViewImDataArgCallBack)(void *),
                          void * viewImDataArg);
    
    /*! Return the minimum value for the image */
    double dataMin(void) const;

    /*! Return the maximum value for the image */
    double dataMax(void) const;

    /*! Flip the image about the x-axis */
    virtual void    flipX(bool newFlipX);
    /*! Is the image flipped? */
    bool            flipX(void);
    /*! Flip the image about the y-axis */
    virtual void    flipY(bool newFlipY);
    /*! Is the image flipped? */
    bool            flipY(void);
    /*! Flip the image about the z-axis */
    virtual void    flipZ(bool newFlipZ);
    /*! Is the image flipped? */
    bool            flipZ(void);
    
    /*! Transpose the image in the xy-plane */
    virtual void    Transpose(bool newTranspose);
    /*! Is the image Transpose? */
    bool            Transpose(void);
    
    /*! Specify a zoom factor */
    void    winZoom(float newWinZoom);
    /*! How much is the window zoomed */
    float   winZoom(void);
    /*! Specify the coordinates of the center of the region of interest 
    *  to view */
    void    winCenter(int newWinCenterX, int newWinCenterY, int newWinCenterZ);
    /*! Default centering, center at the middle of the image */
    void    winCenter(void);
    /*! Get the coordinates of the center of the region of interest 
    *  being viewed */
    unsigned int    winCenterX(void);
    /*! Get the coordinates of the center of the region of interest 
    *  being viewed */
    unsigned int    winCenterY(void);
    /*! Get the coordinates of the center of the region of interest 
    *  being viewed */
    unsigned int    winCenterZ(void);
    /*! Shift the region of interest being viewed */
    void    winShift(int newWinShiftUp, int newWinShiftRight);
    /*! Called when center is changed */
    void    winCenterCallBack(void (* newWinCenterCallBack)(void));
    void    winCenterCallBack(void (* newWinCenterArgCallBack)(void *), 
                              void * newWinCetnerArg);
    
    /*! Return the total number of slices */
    unsigned int    numSlices(void);
    /*! Specify the slice to view */
    void            sliceNum(unsigned int newSliceNum);
    /*! What slice is being viewed */
    unsigned int    sliceNum(void);
    /*! Called when new slice is viewed */
    void            sliceNumCallBack(
                              void (* newSliceNumCallBack)(void));
    void            sliceNumCallBack(
                              void (* newSliceNumCallBack)(void *),
                              void * newSliceNumArg);
    
    unsigned int orientation(void);
    void         orientation(unsigned int newOrientation);
    void         orientationCallBack(
                              void (*newOrientationCallBack)(void));
    void         orientationCallBack(
                              void (*newOrientationArgCallBack)(void *),
                              void * newOrientationArg);
    
      /*! Specify the clickMode - a user click in a window will cause 
    *  various events */
    void            clickMode(ClickModeType newClickMode);
    ClickModeType   clickMode(void);
    
    virtual void clickSelect(float newX, float newY, float newZ);
    float        clickSelectX(void);
    float        clickSelectY(void);
    float        clickSelectZ(void);
    void         clickSelectCallBack(
                              void (*newClickSelectCallBack)(float, float,
                              float, float));
    void         clickSelectCallBack(
                              void (*newClickSelectArgCallBack)(float, float,
                              float, float,
                              void *),
                              void * newClickSelectArg);
    
      /*! Returns the last ith clicked point - ie. to get the last point
      * i = 0, to get the 2nd last point i = 1, and so on
    */
    bool         getClickedPoint(unsigned int i, ClickPoint & point);
    int          numClickedPointsStored(){ return cClickedPoints.size();}
    
    void         maxClickedPointsStored(unsigned int i);
    unsigned int maxClickedPointsStored();
    
    void  clearClickedPointsStored(){cClickedPoints.clear();}
    
    void  deleteLastClickedPointsStored(){cClickedPoints.pop_front();}  
    
    void  saveClickedPointsStored() {
      const char * filename = fl_file_chooser("Please select a file name","*.*","");
      std::ofstream fpoints( filename );
      std::list< ClickPoint >::const_iterator point = cClickedPoints.begin();
      while( point != cClickedPoints.end() )
        {
        fpoints << point->x << "  " << point->y << "  " << point->z << std::endl;
        ++point;
        }
      fpoints.close();
      };

    void            boxMin(float minX, float minY, float minZ);
    virtual void    boxMax(float maxX, float maxY, float maxZ);
    void            clickBoxCallBack(
                              void (*newClickBoxCallBack)(float, float, float,
                              float, float, float));
    void            clickBoxCallBack(
                              void (*newClickBoxArgCallBack)(float, float, float,
                              float, float, float,
                              void *),
                              void * newClickBoxArg);
    
    void        iwMin(float newIWMin);
    float       iwMin(void);
    void        iwMax(float newIWMax);
    float       iwMax(void);
    void        iwModeMin(IWModeType newIWModeMin);
    IWModeType  iwModeMin(void);
    void        iwModeMax(IWModeType newIWModeMin);
    IWModeType  iwModeMax(void);
    void        iwCallBack(void (*newIWCallBack)(void));
    void        iwCallBack(void (*newIWArgCallBack)(void *),
                           void * newIWArg);
    
    void          imageMode(ImageModeType newImageMode);
    ImageModeType imageMode(void);
    
    virtual void size(int w, int h);
    virtual void resize(int x, int y, int w, int h);
    
    virtual int  handle(int event);
    

//    virtual void update() = 0;
//    virtual void draw() = 0;
    virtual void update(){};
    virtual void draw(){};
    
    unsigned int WinMinX(){return cWinMinX;}
    unsigned int WinMinY(){return cWinMinY;}
    unsigned int WinSizeX(){return cWinSizeX;}
    unsigned int WinSizeY(){return cWinSizeY;}

  void viewDetails(bool detail){cViewDetails = detail;}
  bool viewDetails(){return cViewDetails;}

  void viewValue(bool value){cViewValue = value;}
  bool viewValue(){return cViewValue;}

  void viewCrosshairs(bool crosshairs){cViewCrosshairs = crosshairs;}
  bool viewCrosshairs(){return cViewCrosshairs;}

  void SetFastMoveValue(int moval) { fastMovVal = moval;} //fast moving pace
  int GetFastMoveValue() {return fastMovVal;}
  //how many single step moves before fast moving
  void SetFastMoveThresh(int thresh) { fastMovThresh = thresh;} 
  int GetFastMoveThresh() {return fastMovThresh;}

};

} //namespace

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageView.txx"
#endif


#endif

