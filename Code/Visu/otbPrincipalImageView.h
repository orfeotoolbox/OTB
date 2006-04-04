/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - T.Feuvrier
  Language  :   C++
  Date      :   4 avril 2005
  Version   :   
  Role      :   Classe permettant de gérer l'affichage de la fenetre "Principal"
  $Id$

=========================================================================*/
#ifndef otbPrincipalImageView_h
#define otbPrincipalImageView_h

#include <FL/gl.h>
#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Gl_Window.H>

#include "otbGLVectorImageView.h"

namespace otb
{
  
/**
* GLImageView : Derived from abstract class ImageView and Fl_Gl_Window
* See ImageView.h for details...
  **/
template <class TPixel, class OverlayPixelType>
class ITK_EXPORT PrincipalImageView :  public GLVectorImageView<TPixel,OverlayPixelType>
                                
{
public:
    typedef PrincipalImageView                                  Self;
    typedef GLVectorImageView<TPixel,OverlayPixelType>          Superclass;
    typedef itk::SmartPointer<Self>                             Pointer;
    typedef itk::SmartPointer<const Self>                       ConstPointer;

    /** Method for creation through the object factory. */
    itkNewMacro(Self);

    /** Run-time type information (and related methods). */
    itkTypeMacro(PrincipalImageView,GLVectorImageView);

  
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

  virtual int  handle(int event);

  virtual void Show(void)
  {
        this->BuildWithImageRegion();
//std::cout << "Show: step 1"<<std::endl;
//        this->PrintInfos();
        this->m_flDoubleWindow->show();
//std::cout << "Show: step 2"<<std::endl;
//        this->PrintInfos();
        this->m_flDoubleWindow->size(           this->GetViewImageRegion().GetSize()[0], 
  					        this->GetViewImageRegion().GetSize()[1]);
//std::cout << "Show: step 3"<<std::endl;
//        this->PrintInfos();
//        this->update();
        this->show();
//std::cout << "Show: step 4"<<std::endl;
//        this->PrintInfos();
//        this->show();
        this->update();
//std::cout << "Show: step 5"<<std::endl;
//        this->PrintInfos();
  }

protected:
  
/*! FLTK required constructor - must use imData() to complete 
  definition */
  PrincipalImageView();
  /*! Standard destructor */
  virtual ~PrincipalImageView(void);
  

};

} //namespace

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbPrincipalImageView.txx"
#endif

#endif

