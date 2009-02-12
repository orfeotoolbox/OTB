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
#ifndef __otbImageWidget_h
#define __otbImageWidget_h

// FLTK includes
#include <FL/gl.h>
#include "FL/Fl_Gl_Window.H"

// This is included for the default template
#include "otbImage.h"
#include "itkRGBPixel.h"

// This include is needed to get the OTB_GL_USE_ACCEL definition
#include "otbConfigure.h"

//#include "otbImageViewerController.h"

namespace otb
{
/** \class ImageWidget
*   \brief render an RGB bytes image buffer to the screen
*/

template <class TInputImage=otb::Image<itk::RGBPixel<unsigned char>,2 > > 
class ImageWidget
  : public Fl_Gl_Window, public itk::Object
{
public:
  /** Standard class typedefs */
  typedef ImageWidget                       Self;
  typedef itk::Object                       Superclass;
  typedef itk::SmartPointer<Self>           Pointer;
  typedef itk::SmartPointer<const Self>     ConstPointer;
  
  /** Method for creation through the object factory */
  itkNewMacro(Self);
  
  /** Runtime information */
  itkTypeMacro(ImageWidget,Object);
  /** Input image typedef */
  typedef TInputImage                         InputImageType;
  /** Image region typedef */
  typedef typename InputImageType::RegionType RegionType;
  /** Region size typedef */
  typedef typename RegionType::SizeType       SizeType;
  /** Controller typedef */
  //typedef otb::ImageViewerController        ControllerType;

  /** Reads the OpenGl buffer from an image pointer
   *  \param image The image pointer,
   *  \param region The region to read.
   *  Potential exception thrown if region is outside of the buffered
   *  region.
   * This method fills the m_OpenGl buffer according to the region
   *  size. Buffer in flipped over X axis if OTB_USE_GL_ACCEL is OFF.
   */  
  virtual void ReadBuffer(InputImageType * image, RegionType & region);

  /** Set/Get the Controller */
  //itkSetObjectMacro(Controller,ControllerType);
  //itkGetObjectMacro(Controller,ControllerType);
 
protected:
  /** Constructor */
  ImageWidget();
  /** Destructor */
  ~ImageWidget();
  /** Printself method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;
  /** Actually render the buffer to the screen. This method is 
    * used by FLTK routines and should not be called on its own.
    */
  virtual void draw(void);
  /** Handle resizing event. This method is used by FLTK routines and
    * should not be called on its own.
    */
  virtual void resize(int x, int y, int w, int h);
  /** Handle the event from the users.  This method is used by FLTK 
    * routines and should not be called on its own.
    */
  virtual int  handle(int event);

private:
  ImageWidget(const Self&);    // purposely not implemented
  void operator=(const Self&); // purposely not implemented

  /** OpenGl zoom factor */
  double m_IsotropicZoom;
  /** OpenGl buffer      */
  unsigned char * m_OpenGlBuffer;
  /** OpenGl buffer size */
  SizeType m_OpenGlBufferSize;
  /** Widget identifier */
  std::string m_Identifier;

}; // end class 
} // end namespace otb

#ifndef  OTB_MANUAL_INSTANTIATION
#include "otbImageWidget.txx"
#endif

#endif


