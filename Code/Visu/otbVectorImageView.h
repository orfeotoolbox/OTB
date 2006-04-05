/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - T.Feuvrier
  Language  :   C++
  Date      :   4 avril 2005
  Version   :   
  Role      :   Classe de base, utilisee pour l'affichage d'une image dans une fenetre.
  $Id$

=========================================================================*/
#ifndef _otbVectorImageView_h
#define _otbVectorImageView_h

/*
* This is a cross-platform image viewer for 3D medical images.
* See the class definition for details...
*/

#include <itkProcessObject.h>
#include <itkVectorImage.h>
//#include <fltkUtils.h>
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
    : x(0),y(0),/*z(0),*/value(0){}
    ClickPoint( const ClickPoint & p )
    { x = p.x; y = p.y; /*z = p.z; */value = p.value; }
    ClickPoint(float _x,float _y,/*float _z,*/double v)
    : x(_x),y(_y),/*z(_z),*/value(v){}

  public:
    float x, y/*, z*/;
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
template <class TPixel>
class ITK_EXPORT VectorImageView : public itk::ProcessObject
{
  public:
    typedef VectorImageView                     Self;
    typedef itk::ProcessObject                  Superclass;
    typedef itk::SmartPointer<Self>             Pointer;
    typedef itk::SmartPointer<const Self>       ConstPointer;

    /** Method for creation through the object factory. */
    itkNewMacro(Self);

    /** Run-time type information (and related methods). */
    itkTypeMacro(VectorImageView,itk::ProcessObject);

    // some typedefs
    typedef itk::VectorImage<TPixel,2>          ImageType;
    typedef typename ImageType::Pointer         ImagePointer;
    typedef typename ImageType::ConstPointer    ImageConstPointer;
    typedef typename ImageType::RegionType      RegionType;
    typedef typename ImageType::SizeType        SizeType;
    typedef typename ImageType::IndexType       IndexType;
    typedef typename ImageType::PixelType       PixelType;

    itkSetMacro(ViewImageRegion, RegionType);
    itkGetConstReferenceMacro(ViewImageRegion, RegionType);

  protected:
    void   (* cSliceNumCallBack)(void);
    void    * cSliceNumArg;
    void   (* cSliceNumArgCallBack)(void * sliceNumArg);
    
    bool                     cValidImData;
    bool                     cViewImData;
    bool                     cViewClickedPoints;
    RegionType               m_ImageRegion;
    RegionType               m_ViewImageRegion;
    
    unsigned long            cDimSize[2];
    void                    (* cViewImDataCallBack)(void);
    void                     * cViewImDataArg;
    void                    (* cViewImDataArgCallBack)(void *viewImDataArg);
    
    float       cIWMin;
    float       cIWMax;

    void        (* cIWCallBack)(void);
    void         * cIWArg;
    void        (* cIWArgCallBack)(void * iwArg);

    ImageModeType cImageMode;
    
    float               cWinZoom;
    unsigned int   cWinSizeX;
    unsigned int   cWinSizeY;
    int   cWinDataSizeX;
    int   cWinDataSizeY;
    unsigned char  *cWinImData;
    
    double cDataMax, cDataMin;
    
    int cX, cY, cW, cH;
    
    /*! VectorImageView Constructor */
    VectorImageView();
    /*! Standard destructor */
    virtual ~VectorImageView(void);

  public:

    virtual void Init(int x, int y, int w, int h, const char *l=0);
    
    /*! Return a pointer to the image data */
    
  virtual void SetInput( const ImageType *image);

//    const ImageConstPointer & GetInputImage(void) const;
    const ImageType * GetInput(void) const;
    
    /*! Dammit, give me a pointer to the image that's not const! */
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

    
    /*! Specify a zoom factor */
    void    winZoom(float newWinZoom);
    /*! How much is the window zoomed */
    float   winZoom(void);
    

    virtual void        iwMin(float newIWMin);
    virtual float       iwMin(void);
    virtual void        iwMax(float newIWMax);
    virtual float       iwMax(void);
    void        iwCallBack(void (*newIWCallBack)(void));
    void        iwCallBack(void (*newIWArgCallBack)(void *),
                           void * newIWArg);
    

    void          imageMode(ImageModeType newImageMode);
    ImageModeType imageMode(void);
    
    virtual void size(int w, int h);
    virtual int sizeX(void){return cW;}
    virtual int sizeY(void){return cH;}
    virtual void resize(int x, int y, int w, int h);
    
    virtual void update(){};
    virtual void draw(){};
    
    virtual unsigned int WinSizeX(){return cWinSizeX;}
    virtual unsigned int WinSizeY(){return cWinSizeY;}

};

} //namespace

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbVectorImageView.txx"
#endif


#endif

