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

#include "itkObject.h"
#include "itkImageRegion.h"
#include "itkAffineTransform.h"
#include "itkFixedArray.h"

// FLTK includes
#include <FL/gl.h>
#include "FL/Fl_Gl_Window.H"

namespace otb
{
/** \class Curve2D
*   \brief Base class Curves to be rendered in Curves2DWidget.
*   
*   \sa ImageViewerModel
*/

class Curve2D
  : public itk::Object
{
public:
  /** Standard class typedefs */
  typedef Curve2D                              Self;
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
  itkTypeMacro(Curve2D,Object);

  /// Render the curve according to display extent and axis characteristics
  virtual void  Render(const RegionType& extent,const AffineTransformType * space2ScreenTransform) = 0;

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


