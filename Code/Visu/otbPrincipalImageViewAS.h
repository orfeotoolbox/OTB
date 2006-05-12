/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - R.Garrigues
  Language  :   C++
  Date      :   27 avril 2006
  Version   :   
  Role      :   Classe permettant de gérer l'affichage de la fenetre "Principal"
  $Id$

=========================================================================*/

#ifndef otbPrincipalImageViewAS_h
#define otbPrincipalImageViewAS_h

#include <FL/gl.h>
#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Gl_Window.H>

#include "otbGLVectorImageViewClick.h"

namespace otb
{
  
/**
* \class PrincipalImageViewAS
* \brief Class for viewing an image in entier resolution
* 
* See GLVectorImageViewClick.h for details...
  **/
template <class TPixel, class TPixelOverlay>
class ITK_EXPORT PrincipalImageViewAS :  public
GLVectorImageViewClick<TPixel, TPixelOverlay>
                                
{
public:
    
    float _x,_y,_z;   
    typedef PrincipalImageViewAS                                  Self;
    typedef GLVectorImageViewClick<TPixel, TPixelOverlay>                       Superclass;
    typedef itk::SmartPointer<Self>                             Pointer;
    typedef itk::SmartPointer<const Self>                       ConstPointer;

    /** Method for creation through the object factory. */
    itkNewMacro(Self);

    /** Run-time type information (and related methods). */
    itkTypeMacro(PrincipalImageViewAS,GLVectorImageViewClick);

  
  typedef typename Superclass::ImageType                ImageType;
  typedef typename Superclass::ImagePointer             ImagePointer;
  typedef typename Superclass::ImageConstPointer        ImageConstPointer;
  typedef typename Superclass::RegionType               RegionType;
  typedef typename Superclass::SizeType                 SizeType;
  typedef typename Superclass::IndexType                IndexType;
  typedef typename Superclass::PixelType                PixelType;

  virtual int  handle(int event);

  /** Show the image (display the window) */
  virtual void Show(void);
  
  /** Erase the last clicked point */
  virtual void UndoPt();
  
  /** Erase all clicked points */
  virtual void ResetPts();
  
  void SetClickable(bool b);
  
  /** Selected point correct */
  bool cMem;
  
  /** Box color */
  ColorType cRectColor;
  
  /** To know if click or box */
  int nbDrag;

  /** Image clickable */
  bool cClickable;  
  
protected:
  
  /*! Standard constructor */
  PrincipalImageViewAS();
  /*! Standard destructor */
  virtual ~PrincipalImageViewAS(void);
  

};

} //namespace

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbPrincipalImageViewAS.txx"
#endif

#endif

