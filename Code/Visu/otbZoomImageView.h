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
#ifndef otbZoomImageView_h
#define otbZoomImageView_h

#include <FL/gl.h>
#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Gl_Window.H>

#include "otbGLVectorImageViewBase.h"

namespace otb
{
  
/**
* \class ZoomImageView
* \brief Class for viewing an zoom image
* 
* See GLVectorImageViewBase.h for details...
  **/
template <class TPixel>
class ITK_EXPORT ZoomImageView :  public GLVectorImageViewBase<TPixel>
                                
{
public:
    typedef ZoomImageView                                       Self;
    typedef GLVectorImageViewBase<TPixel>                       Superclass;
    typedef itk::SmartPointer<Self>                             Pointer;
    typedef itk::SmartPointer<const Self>                       ConstPointer;

    /** Method for creation through the object factory. */
    itkNewMacro(Self);

    /** Run-time type information (and related methods). */
    itkTypeMacro(ZoomImageView,GLVectorImageViewBase);

  
  typedef typename Superclass::ImageType                ImageType;
  typedef typename Superclass::ImagePointer             ImagePointer;
  typedef typename Superclass::ImageConstPointer        ImageConstPointer;
  typedef typename Superclass::RegionType               RegionType;
  typedef typename Superclass::SizeType                 SizeType;
  typedef typename Superclass::IndexType                IndexType;
  typedef typename Superclass::PixelType                PixelType;

  virtual int  handle(int event);

  /** Show the image (display the window) */
  virtual void Show(void)
  {
        this->BuildWithWindowRegion(4);
        this->m_flDoubleWindow->show();
//        this->m_flDoubleWindow->size(           this->GetViewImageRegion().GetSize()[0], 
//  					        this->GetViewImageRegion().GetSize()[1]);
        ::itk::OStringStream lStream;
        lStream << this->GetLabel() << " - ("<<this->cWinZoom<<"x)";
        this->m_flDoubleWindow->label( lStream.str().data() );
        this->show();
        this->update();
  }

protected:
  
/*! Standard constructor */
  ZoomImageView();
  /*! Standard destructor */
  virtual ~ZoomImageView(void);
  
public:

};

} //namespace

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbZoomImageView.txx"
#endif

#endif

