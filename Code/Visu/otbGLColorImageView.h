#ifndef otbGLColorImageView_h
#define otbGLColorImageView_h

#include <FL/gl.h>
#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Gl_Window.H>

#include "itkColorTable.h"
#include "itkRGBPixel.h"

#include "otbImageView.h"
#include "otbGLImageView.h"

#include <math.h>

namespace otb
{
  
/**
* GLImageView : Derived from abstract class SliceView and Fl_Gl_Window
* See SliceView.h for details...
  **/
template <class ImagePixelType, class OverlayPixelType>
class ITK_EXPORT GLColorImageView : public GLImageView<ImagePixelType, OverlayPixelType> 
{
  public:
    typedef GLColorImageView                                                    Self;
    typedef GLImageView<ImagePixelType, OverlayPixelType>                       Superclass;
    typedef itk::SmartPointer<Self>                                             Pointer;
    typedef itk::SmartPointer<const Self>                                       ConstPointer;

    /** Method for creation through the object factory. */
    itkNewMacro(Self);

    /** Run-time type information (and related methods). */
    itkTypeMacro(GLColorImageView,GLImageView);

    typedef itk::Image<itk::RGBPixel<ImagePixelType>, 3> ImageType;

    typedef typename ImageType::Pointer ImagePointer;
    typedef typename ImageType::ConstPointer ImageConstPointer;
    typedef typename Superclass::RegionType RegionType;
    typedef typename Superclass::SizeType SizeType;
    typedef typename Superclass::IndexType IndexType;
    
    /*! Specify the 3D image to view slice by slice */
    virtual void SetInputImage(ImageType * newImData);
    virtual void SetInput( const ImageType *image);
//    const ImageType * GetInput(void) const;
//    ImageType * GetInput(void);

    virtual void clickSelect(float newX, float newY, float newZ);
    virtual void size(int w, int h);
    
    virtual void resize(int x, int y, int w, int h);

    virtual void update();

    virtual void draw();
    
    virtual int handle(int event);

    virtual void Init(int x, int y, int w, int h, const char *l);



  protected:

    /*! FLTK required constructor - must use imData() to complete 
     *  definition */
    GLColorImageView();
    virtual ~GLColorImageView(void);


    float         cClickSelectR;
    float         cClickSelectG;
    float         cClickSelectB;

//    ImagePointer  cImData;
  };
  
} //namespace


#ifndef OTB_MANUAL_INSTANTIATION
#include "otbGLColorImageView.txx"
#endif

#endif

