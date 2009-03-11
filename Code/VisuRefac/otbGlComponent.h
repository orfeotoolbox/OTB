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
#ifndef __otbGlComponent_h
#define __otbGlComponent_h

#include "itkObject.h"
#include "itkImageRegion.h"
#include "itkAffineTransform.h"
#include "itkFixedArray.h"

// FLTK includes
#include <FL/gl.h>
#include "FL/Fl_Gl_Window.H"

namespace otb
{
/** \class GlComponent
*   \brief Base class for openGl rendered components.
*/

class GlComponent
  : public itk::Object
{
public:
  /** Standard class typedefs */
  typedef GlComponent                          Self;
  typedef itk::Object                          Superclass;
  typedef itk::SmartPointer<Self>              Pointer;
  typedef itk::SmartPointer<const Self>        ConstPointer;
  typedef itk::ImageRegion<2>                  RegionType;
 
  // affine transform
  typedef itk::AffineTransform<double,2>       AffineTransformType;
  typedef AffineTransformType::InputPointType  PointType;
  typedef AffineTransformType::InputVectorType VectorType;
  typedef itk::FixedArray<double,4>             ColorType;
    
  /** Runtime information */
  itkTypeMacro(GlComponent,Object);

  /// Render the curve according to display extent and axis characteristics
  virtual void  Render(const RegionType& extent,const AffineTransformType * space2ScreenTransform) = 0;

  /** Set/Get the visible status */
  itkSetMacro(Visible,bool);
  itkGetMacro(Visible,bool);
  itkBooleanMacro(Visible);

protected:
  /** Constructor */
  GlComponent() : m_Visible(true)
  {}
  /** Destructor */
  virtual ~GlComponent(){}
  /** Printself method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os,indent);
  }

private:
  GlComponent(const Self&);     // purposely not implemented
  void operator=(const Self&); // purposely not implemented

  /// Is the component visible ?
  bool m_Visible;

}; // end class 
} // end namespace otb

#endif


