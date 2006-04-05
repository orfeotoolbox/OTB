/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - T.Feuvrier
  Language  :   C++
  Date      :   4 avril 2005
  Version   :   
  Role      :   Classe permettant de gérer l'affichage de la fenetre "Scroll"
  $Id$

=========================================================================*/
#ifndef otbScrollImageView_h
#define otbScrollImageView_h

#include <FL/gl.h>
#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Gl_Window.H>

#include "otbGLVectorImageViewBase.h"

namespace otb
{
  
/**
* GLImageView : Derived from abstract class ImageView and Fl_Gl_Window
* See ImageView.h for details...
  **/
template <class TPixel, class OverlayPixelType>
class ITK_EXPORT ScrollImageView :  public GLVectorImageViewBase<TPixel,OverlayPixelType>
                                
{
public:
    typedef ScrollImageView                                     Self;
    typedef GLVectorImageViewBase<TPixel,OverlayPixelType>      Superclass;
    typedef itk::SmartPointer<Self>                             Pointer;
    typedef itk::SmartPointer<const Self>                       ConstPointer;

    /** Method for creation through the object factory. */
    itkNewMacro(Self);

    /** Run-time type information (and related methods). */
    itkTypeMacro(ScrollImageView,GLVectorImageViewBase);

  
  typedef typename Superclass::ImageType                ImageType;
  typedef typename Superclass::ImagePointer             ImagePointer;
  typedef typename Superclass::ImageConstPointer        ImageConstPointer;
  typedef typename Superclass::RegionType               RegionType;
  typedef typename Superclass::SizeType                 SizeType;
  typedef typename Superclass::IndexType                IndexType;
  typedef typename Superclass::PixelType                PixelType;

  virtual int  handle(int event);

  virtual void Show(void)
  {
        this->BuildWithImageRegion();
        this->m_flDoubleWindow->show();
        this->m_flDoubleWindow->size(           this->GetViewImageRegion().GetSize()[0], 
  					        this->GetViewImageRegion().GetSize()[1]);
        this->show();
        this->update();
  }

protected:
  
/*! Standard constructor */
  ScrollImageView();
  /*! Standard destructor */
  virtual ~ScrollImageView(void);
  
};

} //namespace

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbScrollImageView.txx"
#endif

#endif

