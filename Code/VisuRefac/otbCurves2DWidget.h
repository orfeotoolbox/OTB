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
//#include "otbCurve2D.h"
#include "otbObjectList.h"
#include "itkSize.h"
#include "itkContinuousIndex.h"

namespace otb
{
/** \class 2DCurveWidget
 *
 */

class Curves2DWidget
  : public GlWidget
{
public:
  /** Standard class typedefs */
  typedef Curves2DWidget                    Self;
  typedef GlWidget                          Superclass;
  typedef itk::SmartPointer<Self>           Pointer;
  typedef itk::SmartPointer<const Self>     ConstPointer;

  /** Method for creation through the object factory */
  itkNewMacro(Self);

//   typedef Curve2D                        CurveType;
//   typedef ObjectList<CurveType>          CurveListType;
//   typedef CurveListType::Pointer         CurveListPointerType;

  typedef Superclass::ColorType          ColorType;

  typedef itk::Size<2>                   SizeType;
  typedef itk::ContinuousIndex<double,2> ContinuousIndexType;

  itkSetStringMacro(XAxisLabel);
  itkGetStringMacro(XAxisLabel);
  
  itkSetStringMacro(YAxisLabel);
  itkGetStringMacro(YAxisLabel);

  itkSetMacro(Margins,SizeType);
  itkGetConstReferenceMacro(Margins,SizeType);

  itkSetMacro(AxisOrigin,ContinuousIndexType);
  itkGetConstReferenceMacro(AxisOrigin,ContinuousIndexType);

  itkSetMacro(AxisLength,ContinuousIndexType);
  itkGetConstReferenceMacro(AxisLength,ContinuousIndexType);
  
  itkSetMacro(GridOrigin,ContinuousIndexType);
  itkGetConstReferenceMacro(GridOrigin,ContinuousIndexType);

  itkSetMacro(GridSpacing,ContinuousIndexType);
  itkGetConstReferenceMacro(GridSpacing,ContinuousIndexType);

  itkSetMacro(AxisColor,ColorType);
  itkGetConstReferenceMacro(AxisColor,ColorType);

  itkSetMacro(GridColor,ColorType);
  itkGetConstReferenceMacro(GridColor,ColorType);
  
  itkSetMacro(ZeroCrossingAxis,bool);
  itkGetMacro(ZeroCrossingAxis,bool);
  itkBooleanMacro(ZeroCrossingAxis);
  
  ContinuousIndexType SpacePositionToScreenPosition(const ContinuousIndexType & pos) const;
  ContinuousIndexType ScreenPositionToSpacePosition(const ContinuousIndexType & pos) const;


protected:
  /** Constructor */
  Curves2DWidget();
  /** Destructor */
  ~Curves2DWidget();
  /** Printself method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** Actually render the buffer to the screen. This method is
    * used by FLTK routines and should not be called on its own.
    */
  virtual void draw(void);

  /// TODO: Document
//   AddCurve(CurveType * curve);

//   RemoveCurve(Curve * curve);

//   ClearAllCurves();
 
 // unsigned int GetNumberOfCurves();

  
  void RenderAxis();
  void RenderGrid();
  void RenderCurves();

private:
  Curves2DWidget(const Self&);    // purposely not implemented
  void operator=(const Self&); // purposely not implemented

  /** List of the curves */
//   CurveListPointerType  m_Curves;
  // X axis label
  std::string         m_XAxisLabel;
  // Y axis label
  std::string         m_YAxisLabel;
  // Margins around graph
  SizeType            m_Margins;
  // Axis range start
  ContinuousIndexType m_AxisOrigin;
  // Axis range length
  ContinuousIndexType m_AxisLength;
  // Grid start
  ContinuousIndexType m_GridOrigin;
  // Grid length
  ContinuousIndexType m_GridSpacing;
  // If true, axis will cross in their zeros
  // Else they will be sticked left and lower
  bool                m_ZeroCrossingAxis;

  // Colors
  ColorType           m_AxisColor;
  ColorType           m_GridColor;
  
}; // end class
} // end namespace otb

#endif


