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
#ifndef __otbCurve2D_h
#define __otbCurve2D_h

#include "otbGlComponent.h"

// FLTK includes
#include <FL/gl.h>
#include "FL/Fl_Gl_Window.H"

namespace otb
{
/** \class Curve2D
*   \brief Base class Curves to be rendered in Curves2DWidget.
*   
*   \sa Curves2DWidget
*/

class Curve2D
  : public  GlComponent
{
public:
  /** Standard class typedefs */
  typedef Curve2D                              Self;
  typedef GlComponent                          Superclass;
  typedef itk::SmartPointer<Self>              Pointer;
  typedef itk::SmartPointer<const Self>        ConstPointer;
  typedef itk::ImageRegion<2>                  RegionType;
 
  // affine transform
  typedef Superclass::AffineTransformType      AffineTransformType;
  typedef Superclass::PointType                PointType;
  typedef Superclass::VectorType               VectorType;
  typedef Superclass::ColorType                ColorType;
    
  /** Runtime information */
  itkTypeMacro(Curve2D,GlComponent);

  /// This method is provided to do some computation before rendering
  virtual void BeforeRendering() {};
  
  /// Get the min for each axis from the data available 
  virtual PointType GetMinimum() = 0;

  /// Get the max for each axis from the data available
  virtual PointType GetMaximum() = 0;

  /// Get/Set the label
  itkGetStringMacro(Label);
  itkSetStringMacro(Label);

  /// Get/set the label color
  itkSetMacro(LabelColor,ColorType);
  itkGetConstReferenceMacro(LabelColor,ColorType);

protected:
  /** Constructor */
  Curve2D() : m_Label("Curve"), m_LabelColor()
  {
    // Default color
    m_LabelColor.Fill(1);
  }
  /** Destructor */
  virtual ~Curve2D(){}
  /** Printself method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os,indent);
  }

private:
  Curve2D(const Self&);     // purposely not implemented
  void operator=(const Self&); // purposely not implemented

  /// The curve label
  std::string m_Label;

  /// The curve label color
  ColorType   m_LabelColor;

}; // end class 
} // end namespace otb

#endif


