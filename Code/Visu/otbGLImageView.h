#ifndef otbGLImageView_h
#define otbGLImageView_h

#include <FL/gl.h>
#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Gl_Window.H>

#include "itkColorTable.h"
#include "itkImage.h"

#include "otbGLImageViewBase.h"

namespace otb
{
  
/**
* GLImageView : Derived from abstract class ImageView and Fl_Gl_Window
* See ImageView.h for details...
  **/
template <class TInputImage, class OverlayPixelType>
class ITK_EXPORT GLImageView :  public GLImageViewBase<TInputImage,OverlayPixelType>
                                
{
public:
    typedef GLImageView                                         Self;
    typedef GLImageViewBase<TInputImage,OverlayPixelType>       Superclass;
    typedef itk::SmartPointer<Self>             Pointer;
    typedef itk::SmartPointer<const Self>       ConstPointer;

    /** Method for creation through the object factory. */
    itkNewMacro(Self);

    /** Run-time type information (and related methods). */
    itkTypeMacro(GLImageView,GLImageViewBase);

  
  typedef typename Superclass::ImageType        ImageType;
  typedef typename ImageType::Pointer           ImagePointer;
  typedef typename Superclass::OverlayType      OverlayType;
  typedef typename OverlayType::Pointer         OverlayPointer;
  typedef typename ImageType::RegionType        RegionType;
  typedef typename ImageType::SizeType          SizeType;
  typedef typename ImageType::IndexType         IndexType;
  typedef typename ImageType::PixelType         PixelType;
  
  typedef typename Superclass::ColorTableType       ColorTableType;
  typedef typename Superclass::ColorTablePointer    ColorTablePointer;

  virtual void CalculeDataMaxMin(double & pMin, double & pMax);
 
protected:
  
/*! FLTK required constructor - must use imData() to complete 
  definition */
  GLImageView();
  /*! Standard destructor */
  virtual ~GLImageView(void);
  
public:

  virtual void Init(int x, int y, int w, int h, const char *l);

  /*! Specify the 3D image to view slice by slice */
  virtual void SetInputImage(ImageType * newImData);
  virtual ImageType * GetInput(void) ;
//  virtual const ImagePointer & GetInputImage(void) const;
  
  /*! Specify the 3D image to view as an overlay */
  void SetInputOverlay(OverlayType * newOverlayData);
  
  /*! Return a pointer to the overlay data */
  const OverlayPointer & GetInputOverlay(void) const;
  
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


  /*! Turn on/off the display of clicked points */
  void ViewClickedPoints( bool newViewClickedPoints );

  /*! Status of clicked points display - on/off */
  bool ViewClickedPoints();

  virtual void clickSelect(float x, float y, float z);
  
  virtual void size(int w, int h);
  virtual void resize(int x, int y, int w, int h);
  
  virtual void update();
  virtual void draw();
  
  virtual int  handle(int event);
  
};

} //namespace

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbGLImageView.txx"
#endif


#endif

