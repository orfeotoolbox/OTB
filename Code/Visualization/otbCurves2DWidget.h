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
#ifndef __otbCurves2DWidget_h
#define __otbCurves2DWidget_h

#include "otbGlWidget.h"
#include "otbCurve2D.h"
#include "otbObjectList.h"

namespace otb
{
/** \class Curves2DWidget
 *
 *  \brief This widget renders a set of curves to the screen.
 *
 *  \ingroup Visualization
 */

class Curves2DWidget
  : public GlWidget
{
public:
  /** Standard class typedefs */
  typedef Curves2DWidget                Self;
  typedef GlWidget                      Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory */
  itkNewMacro(Self);

  /** Curve typedef */
  typedef Curve2D          CurveType;
  typedef Curve2D::Pointer CurvePointerType;
  /** Curve list typedef */
  typedef ObjectList<CurveType>  CurveListType;
  typedef CurveListType::Pointer CurveListPointerType;

  /** Color typedef */
  typedef CurveType::ColorType           ColorType;
  typedef CurveType::RegionType          RegionType;
  typedef RegionType::SizeType           SizeType;
  typedef CurveType::AffineTransformType AffineTransformType;
  typedef AffineTransformType::Pointer   AffineTransformPointerType;
  typedef CurveType::PointType           PointType;
  typedef CurveType::VectorType          VectorType;

  /// Set/Get the label of the x axis
  itkSetStringMacro(XAxisLabel);
  itkGetStringMacro(XAxisLabel);

  /// Set/Get the label of the y axis
  itkSetStringMacro(YAxisLabel);
  itkGetStringMacro(YAxisLabel);

  /// Set/Get the widget margins
  itkSetMacro(Margins, SizeType);
  itkGetConstReferenceMacro(Margins, SizeType);

  /// Set/Get the axis origin
  itkSetMacro(AxisOrigin, PointType);
  itkGetConstReferenceMacro(AxisOrigin, PointType);

  /// Set/Get the axis length
  itkSetMacro(AxisLength, VectorType);
  itkGetConstReferenceMacro(AxisLength, VectorType);

  /// Set/Get the grid origin
  itkSetMacro(GridOrigin, PointType);
  itkGetConstReferenceMacro(GridOrigin, PointType);

  /// Set/Get the grid spacing
  itkSetMacro(GridSpacing, VectorType);
  itkGetConstReferenceMacro(GridSpacing, VectorType);

  /// Set/Get the axis color
  itkSetMacro(AxisColor, ColorType);
  itkGetConstReferenceMacro(AxisColor, ColorType);

  /// Set/Get the grid color
  itkSetMacro(GridColor, ColorType);
  itkGetConstReferenceMacro(GridColor, ColorType);

  /// Should axis cross at zero or at their origin ?
  itkSetMacro(ZeroCrossingAxis, bool);
  itkGetMacro(ZeroCrossingAxis, bool);
  itkBooleanMacro(ZeroCrossingAxis);

  // Should axis auto scale to best fit the curves ?
  itkSetMacro(AutoScale, bool);
  itkGetMacro(AutoScale, bool);
  itkBooleanMacro(AutoScale);

  itkSetMacro(ScaleAxeYDefault, bool);
  itkGetMacro(ScaleAxeYDefault, bool);
  itkBooleanMacro(ScaleAxeYDefault);

  /** Add a new curve to the widget
   *  \return curve position in the list
   */
  unsigned int AddCurve(CurveType * curve);

  /** Remove the curve at the given location if any */
  void RemoveCurve(unsigned int idx);

  /** Remove all curves */
  void ClearAllCurves();

  /** Get the number of curves */
  unsigned int GetNumberOfCurves();

  /** TransformPoint */
  PointType TransformPoint(PointType spacePoint)
  {
    return m_SpaceToScreenTransform->TransformPoint(spacePoint);
  }

  /** Remove curve by ID instead of index */
  void RemoveCurveByItsID(unsigned int id);

  /** Get curve by ID instead of index */
  CurvePointerType GetCurveByItsId(unsigned int id);

  /** Set the title of the curve */
  itkSetStringMacro(Title);
  itkGetStringMacro(Title);

protected:
  /** Constructor */
  Curves2DWidget();
  /** Destructor */
  virtual ~Curves2DWidget();
  /** Printself method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** Actually render the buffer to the screen. This method is
    * used by FLTK routines and should not be called on its own.
    */
  virtual void draw(void);

  /** Scale axis and grid according to best curves fit */
  void AutoScale();

  /** Update the space to screen transform */
  void UpdateSpaceToScreenTransform();

  /** Render the axis */
  void RenderAxis();

  /** Render the grid */
  void RenderGrid();

  /** Render the curves */
  void RenderCurves();

private:
  Curves2DWidget(const Self&);    // purposely not implemented
  void operator =(const Self&); // purposely not implemented

  /** List of the curves */
  CurveListPointerType m_Curves;
  // X axis label
  std::string m_XAxisLabel;
  // Y axis label
  std::string m_YAxisLabel;
  // Margins around graph
  SizeType m_Margins;
  // The extent of the display where curves are drawn
  RegionType m_Extent;
  // Axis range start
  PointType m_AxisOrigin;
  // Axis range length
  VectorType m_AxisLength;
  // Grid start
  PointType m_GridOrigin;
  // Grid length
  VectorType m_GridSpacing;
  // If true, axis will cross in their zeros
  // Else they will be sticked left and lower
  bool m_ZeroCrossingAxis;
  // Automatic axis scaling
  bool m_AutoScale;
  // Colors
  ColorType m_AxisColor;
  ColorType m_GridColor;

  bool m_ScaleAxeYDefault;

  // Space to screen transform
  AffineTransformPointerType m_SpaceToScreenTransform;

  // Title
  std::string       m_Title;

}; // end class
} // end namespace otb

#endif
