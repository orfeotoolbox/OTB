#ifndef otbGLVectorImageView_h
#define otbGLVectorImageView_h

#include <FL/gl.h>
#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Gl_Window.H>

#include "itkColorTable.h"
#include "itkVectorImage.h"

#include "otbGLVectorImageViewBase.h"

namespace otb
{
  
/**
* GLImageView : Derived from abstract class ImageView and Fl_Gl_Window
* See ImageView.h for details...
  **/
template <class TPixel, class OverlayPixelType>
class ITK_EXPORT GLVectorImageView :  public GLVectorImageViewBase<TPixel,OverlayPixelType>
                                
{
public:
    typedef GLVectorImageView                                   Self;
    typedef GLVectorImageViewBase<TPixel,OverlayPixelType>      Superclass;
    typedef itk::SmartPointer<Self>                             Pointer;
    typedef itk::SmartPointer<const Self>                       ConstPointer;

    /** Method for creation through the object factory. */
    itkNewMacro(Self);

    /** Run-time type information (and related methods). */
    itkTypeMacro(GLVectorImageView,GLVectorImageViewBase);

  
  typedef typename Superclass::ImageType                ImageType;
  typedef typename Superclass::ImagePointer             ImagePointer;
  typedef typename Superclass::ImageConstPointer        ImageConstPointer;
  typedef typename Superclass::RegionType               RegionType;
  typedef typename Superclass::SizeType                 SizeType;
  typedef typename Superclass::IndexType                IndexType;
  typedef typename Superclass::PixelType                PixelType;

  typedef typename Superclass::OverlayType              OverlayType;
  typedef typename Superclass::OverlayPointer           OverlayPointer;
  
  typedef typename Superclass::ColorTableType           ColorTableType;
  typedef typename Superclass::ColorTablePointer        ColorTablePointer;

  virtual void CalculeDataMinMax(double & pMin, double & pMax);

  virtual void SetWinImData(void);
 
protected:
  
/*! FLTK required constructor - must use imData() to complete 
  definition */
  GLVectorImageView();
  /*! Standard destructor */
  virtual ~GLVectorImageView(void);
  
public:

  virtual void Init(int x, int y, int w, int h, const char *l);

  /*! Specify the 3D image to view slice by slice */
//  virtual void SetInputImage(ImageType * newImData);
  virtual void TreatInputImage(void);
//  virtual ImageType * GetInput(void) ;
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
#include "otbGLVectorImageView.txx"
#endif


#endif

