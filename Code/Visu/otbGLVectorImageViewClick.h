/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef otbGLVectorImageViewBaseAS_h
#define otbGLVectorImageViewBaseAS_h

#include <FL/gl.h>
#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Gl_Window.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_File_Chooser.H>

#include <vector>
#include <string>
#include "itkColorTable.h"

#include "otbImage.h"
#include "itkMacro.h"

#include "otbVectorImageView.h"

namespace otb
{

const int NUM_ClickModeTypes = 3;
typedef enum {CM_NOP, CM_SELECT, CM_BOX} ClickModeType;
typedef enum { COLOR_BLUE , COLOR_RED } ColorType;
typedef enum {IMG_VAL, IMG_INV, IMG_LOG, IMG_DX, IMG_DY, IMG_DZ,
  IMG_BLEND, IMG_MIP} ImageModeType;
typedef enum {IW_MIN, IW_MAX, IW_FLIP} IWModeType;

const char ClickModeTypeName[3][7] =
  {{'N', 'O', 'P', '\0', ' ', ' ', ' '},
  {'S', 'e', 'l', 'e', 'c', 't', '\0'},
  {'B', 'o', 'x', '\0', ' ', ' ', ' '}};
  
   /*! Structure clickPoint to store the x,y,z and intensity value of a
  * point in the image
*/
class ClickPoint 
  {
  public:
    ClickPoint()
    : x(0),y(0),z(0),value(0),color(COLOR_BLUE), w(0), h(0), isForBox(false){}
    ClickPoint( const ClickPoint & p )
    { x = p.x; y = p.y; z = p.z;value = p.value;color = p.color;
      w = p.w; h = p.h; isForBox=p.isForBox; }
    ClickPoint(float _x,float _y,float _z,double v,ColorType _color)
    : x(_x),y(_y),z(_z),value(v),color(_color),w(0),h(0),isForBox(false){}
    ClickPoint(float _x,float _y,float _z,double v,ColorType _color,
    	       float _w,float _h) :
	  x(_x),y(_y),z(_z),value(v),color(_color),w(_w),h(_h),isForBox(true){}

  public:
    float x, y, z, w, h;
    double value;
    ColorType color;
    bool isForBox;
  };


template <class TPixel> class ImageViewerAS;
/** \class GLVectorImageViewBase
* \brief Base class for image view
* Contain all algorithms and functionnalities for display an image in a window.
*/
template <class TPixel, class TPixelOverlay>
class ITK_EXPORT GLVectorImageViewClick :        public VectorImageView<TPixel>, 
                                                public Fl_Gl_Window
{
public:
    typedef GLVectorImageViewClick               Self;
    typedef VectorImageView<TPixel>             Superclass;
    typedef itk::SmartPointer<Self>             Pointer;
    typedef itk::SmartPointer<const Self>       ConstPointer;
    typedef otb::Image<TPixelOverlay,2>   		OverlayType;
    typedef typename OverlayType::Pointer    OverlayPointer;
	typedef itk::ColorTable<float>                ColorTableType;
    typedef typename ColorTableType::Pointer      ColorTablePointer;
   
    /** Method for creation through the object factory. */
    itkNewMacro(Self);

    /** Run-time type information (and related methods). */
    itkTypeMacro(GLVectorImageViewClick,VectorImageView);

  typedef ImageViewerAS<TPixel>                           ImageViewerType;
  typedef ImageViewerType *                             ImageViewerPointer;


  typedef typename Superclass::ImageType                ImageType;
  typedef typename Superclass::ImagePointer             ImagePointer;
  typedef typename Superclass::ImageConstPointer        ImageConstPointer;
  typedef typename Superclass::RegionType               RegionType;
  typedef typename Superclass::SizeType                 SizeType;
  typedef typename Superclass::IndexType                IndexType;
  typedef typename Superclass::PixelType                PixelType;

  /** Typedef Liste des canaux */
  typedef typename std::vector<int>                     ChannelsType;
  
  typedef enum { GRAY_LEVEL = 1, RGB_LEVEL = 3 }        ModeViewType;

  virtual void ClearSelectChannels(void);

  //Méthode d'accès sur le Viewer
  itkSetMacro(Viewer,ImageViewerPointer);
  itkGetConstMacro(Viewer,ImageViewerPointer);

  itkSetMacro(NbDim,int);
  itkSetMacro(ModeView,ModeViewType);

  itkSetMacro(GrayLevelChannel,int);
  itkSetMacro(RedChannel,int);
  itkSetMacro(GreenChannel,int);
  itkSetMacro(BlueChannel,int);

  virtual void SetRGBChannels(int pRedChannel,int pGreenChannel, int pBlueChannel)
  {
        SetRedChannel(pRedChannel);
        SetGreenChannel(pGreenChannel);
        SetBlueChannel(pBlueChannel);
  }

//  itkSetMacro(ChannelsWorks, ChannelsType);
  virtual void SetChannelsWorks(const ChannelsType & pChannelsWorks)
  {
        m_ChannelsWorks = pChannelsWorks;
  }

  virtual IndexType     GetCenterRegion( const RegionType & zone );
  static  RegionType    ShrinkRegion( const RegionType & zone, const float shrinkfactor );
  virtual RegionType    GetViewRegion( const IndexType & centerPointImage );
  virtual RegionType    GetViewRegion( const RegionType & region, const IndexType & centerPointImage );
  virtual RegionType    TranslateImageRegionToWindowRegion( const RegionType & imageRegion, const  RegionType & sousimageRegion, const int windowWidth, const int windowHeight);

  /** Update the view region, with the new zoom and new Window sizes */
  virtual void          MajViewRegion(void);
  virtual void          PrintInfos(void);
  virtual void          BuildWithImageRegion();
  virtual void          BuildWithWindowRegion(const int zoom);
  
  virtual IndexType  WindowCoord2ImageCoord( const IndexType & index )const;
  virtual SizeType   WindowSize2ImageSize( const SizeType & size )const;
  virtual RegionType WindowZone2ImageRegion( const RegionType & zone )const;

  virtual void Init(int x, int y, int w, int h, const char *l);

  virtual void FinalizeInitialisation(void);
  
  virtual void size(int w, int h);
  virtual void resize(int x, int y, int w, int h);

  /** Click Mode */
  
  /* list of points clicked and maximum no. of points to be stored*/
  std::list< ClickPoint > cClickedPoints;
  unsigned int maxClickPoints;

  ClickModeType cClickMode;
  float cClickSelect[3];
  float cClickSelectV;
  void (* cClickSelectCallBack)(float x,float y,float z,
                                           float v);
  void * cClickSelectArg;
  void (* cClickSelectArgCallBack)(float x, float y, float z, 
                                              float v, void *clickSelectArg);
    
  float cBoxMin[3];
  float cBoxMax[3];
  void (* cClickBoxCallBack)(float minX, float minY, float minZ, 
                                      float maxX, float maxY, float maxZ);
  void * cClickBoxArg;
  void (* cClickBoxArgCallBack)(float minX, float minY, float minZ,
                                         float maxX, float maxY, float maxZ,
                                         void * clickBoxArg);
  /*! Turn on/off the display of clicked points */
  void ViewClickedPoints( bool newViewClickedPoints );

  /*! Status of clicked points display - on/off */
  bool ViewClickedPoints();

  /*! Specify the clickMode - a user click in a window will cause 
    *  various events */
  void            clickMode(ClickModeType newClickMode);
  ClickModeType   clickMode(void);

  virtual void clickSelect(float x, float y, float z, ColorType color);
  virtual void clickSelectBox(float x, float y, float z, ColorType color,
  					int width, int height);
  
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
      /*const char * filename = fl_file_chooser("Please select a file name","*.*","");
      std::ofstream fpoints( filename );
      std::list< ClickPoint >::const_iterator point = cClickedPoints.begin();
      while( point != cClickedPoints.end() )
        {
        fpoints << point->x << "  " << point->y << "  " << point->z << std::endl;
        ++point;
        }
      fpoints.close();*/
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

/** End Click Mode */  
  
  virtual void update();
  virtual void draw();
//  virtual int  handle(int event);
  virtual void SetDoubleWindow( Fl_Double_Window * pflDoubleWindow)
  {
        m_flDoubleWindow = pflDoubleWindow;
  }

  itkSetMacro(CenterPointImage,IndexType);
  itkGetConstReferenceMacro(CenterPointImage,IndexType);
  itkSetMacro(DrawViewRectangle,RegionType);
  itkGetConstReferenceMacro(DrawViewRectangle,RegionType);

  itkSetMacro(Label,std::string);
//  itkGetConstReferenceMacro(Label,std::string);
  itkSetStringMacro(Label);
  itkGetStringMacro(Label);
  
  // Overlay
   bool cOverlay;

   /*! Activate overlays on image */   
   void ActivateOverlayFirst(bool b);
   void ActivateOverlaySecond(bool b);
  
   bool        cViewOverlayData;
  /*! Specify the 3D image to view as an overlay */
  void SetInputOverlay(	OverlayPointer newOverlayDataClassFirst, 
		   	OverlayPointer newOverlayDataClassSecond);
  
  /*! Return a pointer to the overlay data */
  const OverlayPointer & GetInputOverlay(void) const;

  /*! Return a pointer to the overlay data for the Red Class*/
  const OverlayPointer & GetInputOverlayClassFirst(void) const;

  /*! Return a pointer to the overlay data for the Blue Class*/
  const OverlayPointer & GetInputOverlayClassSecond(void) const;
  
  /*! Turn on/off the viewing of the overlay */
  void  ViewOverlayData(bool newViewOverlayData);
  
  /*! Status of the overlay - viewed /not viewed */
  bool  ViewOverlayData(void);
  
  /*! Specify the opacity of the overlay */
  void  OverlayOpacity(float newOverlayOpacity);
  
  /*! Get the opacity of the overlay */
  float OverlayOpacity(void);
  
  /*! Called when overlay is toggled or opacity is changed */
  void  ViewOverlayCallBack(void (* newOverlayCallBack)(void));
  
  ColorTablePointer GetColorTable(void);
  void SetColorTable(ColorTablePointer newColorTable);
  /*! Display Overlay in Color 'c'. You must ensure that the color-table specified
   * contains color 'c'. For example with the default useDiscrete() color table,
   * SetOverlayColorIndex( 0 ) will display the overlay in red. 
   * SetOverlayColorIndex( 1 ) purple etc.... */
  void SetOverlayColorIndex( unsigned int c)
    {
    cOverlayColorIndex = c;
    }
    
  /** Box Selection */
  bool cSelectRectangle;
  int coord[4];
  
  int cOffSetX;
  int cOffSetY;
  float cScale;
  
 // virtual void SetInput( const ImageType *image);

protected:

  //Méthodes utilisées pour l'overlay de l'image
  bool        cValidOverlayData;
  //Booleens permettant l'affichage des overlays de la classification
  bool	      cViewOverlayClassFirst;
  bool	      cViewOverlayClassSecond;
  
  float       cOverlayOpacity;
  // Overlay utilisé pour afficher le résultat de la classification
  // OverlayPointer cOverlayData;
  // Overlay utilisé pour afficher le résultat de la classe #C1
  OverlayPointer cOverlayDataClassFirst;
  // Overlay utilisé pour afficher le résultat de la classe #C2
  OverlayPointer cOverlayDataClassSecond;
  
  
  void     (* cViewOverlayCallBack)(void);
  
  ImageModeType cImageMode;
  //unsigned char * cWinOverlayData;
  unsigned char * cWinOverlayDataFirst;
  unsigned char * cWinOverlayDataSecond;
  unsigned int    cOverlayColorIndex;
  ColorTablePointer      cColorTable;
  
  //Méthode Get en écriture sur le Viewer
  itkGetMacro(Viewer,ImageViewerPointer);


  virtual void CalculeDataMinMax(const RegionType & region, double & pMin, double & pMax);
  virtual void SetWinImData(const RegionType & zone);
  
/*! FLTK required constructor - must use imData() to complete 
  definition */
  GLVectorImageViewClick();
  /*! Standard destructor */
  virtual ~GLVectorImageViewClick(void);

  /** Liste des canaux qui seront réellement traités [1...] */
  ChannelsType          m_ChannelsWorks;
  
  /** Nombre de dimensions de l'image : = 1 si GRAY_LEVEL, sinon 3 si RGB_LEVEL */
  int                   m_NbDim;
  int                   m_RedChannel;
  int                   m_GreenChannel;
  int                   m_BlueChannel;
  int                   m_GrayLevelChannel;

  bool                  m_ViewRectangle;
  RegionType            m_DrawViewRectangle;
  Fl_Double_Window *    m_flDoubleWindow;
  IndexType             m_CenterPointImage;

  ModeViewType          m_ModeView;


private:
    // Pointeur sur le Viewer
    ImageViewerPointer  m_Viewer;

    std::string         m_Label;   
};


} //namespace

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbGLVectorImageViewClick.txx"
#endif


#endif

