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
#ifndef __otbVerticalAsymptoteCurve_h
#define __otbVerticalAsymptoteCurve_h

#include "otbCurve2D.h"
#include "itkContinuousIndex.h"
#include "itkObjectFactory.h"

namespace otb
{
/** \class VerticalAsymptoteCurve
*   \brief Class for Vertical Asymptots rendering.
*
*   \sa ImageViewerModel
*  \ingroup Visualization
 */

class ITK_EXPORT VerticalAsymptoteCurve
  : public Curve2D
{
public:
  /** Standard class typedefs */
  typedef VerticalAsymptoteCurve        Self;
  typedef Curve2D                       Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  // Standard class macros
  itkNewMacro(Self);
  itkTypeMacro(VerticalAsymptoteCurve, Curve2D);

  /// Superclass typedefs
  typedef  Superclass::AffineTransformType AffineTransformType;
  typedef  Superclass::RegionType          RegionType;
  typedef  Superclass::PointType           PointType;
  typedef  Superclass::VectorType          VectorType;
  typedef  Superclass::ColorType           ColorType;
  typedef itk::ContinuousIndex<double, 2>  ContinuousIndexType;

  /// Render the curve according to display extent and axis characteristics
  virtual void  Render(const RegionType& extent, const AffineTransformType * space2ScreenTransform);

  /// Pre-computation
  virtual void BeforeRendering();

  /// Get the min for each axis from the data available
  virtual PointType GetMinimum();

  /// Get the max for each axis from the data available
  virtual PointType GetMaximum();

  /// Set/Get the Asymptote color
  itkSetMacro(VerticalAsymptoteColor, ColorType);
  itkGetMacro(VerticalAsymptoteColor, ColorType);

  /// Set the abcisse
  itkSetMacro(Abcisse, double);
  itkGetMacro(Abcisse, double);

protected:
  /** Constructor */
  VerticalAsymptoteCurve();
  /** Destructor */
  virtual ~VerticalAsymptoteCurve(){}
  /** Printself method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os, indent);
  }

private:
  VerticalAsymptoteCurve(const Self &);     // purposely not implemented
  void operator =(const Self&); // purposely not implemented

  // The histogram color
  ColorType m_VerticalAsymptoteColor;

  PointType m_Minimum;
  PointType m_Maximum;

  double m_Abcisse;
  double m_SpaceAbcisse;

}; // end class
} // end namespace otb

#endif
