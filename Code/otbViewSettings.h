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
#ifndef otb_ViewSettings_h
#define otb_ViewSettings_h

#include "itkObject.h"
#include "itkSmartPointer.h"
#include "itkPoint.h"
#include "itkVector.h"
#include "itkSize.h"
#include "otbImageKeywordlist.h"

namespace otb
{

class ViewSettings 
  : public itk::Object
{
public:
  typedef ViewSettings                                          Self;
  typedef itk::Object                                           Superclass;
  typedef itk::SmartPointer<Self>                               Pointer;
  typedef itk::SmartPointer<const Self>                         ConstPointer;

  typedef itk::Point<double,2>                                  PointType;
  typedef itk::Vector<double,2>                                 SpacingType;
  typedef itk::Size<2>                                          SizeType;
  typedef otb::ImageKeywordlist                                 KeywordListType;

  itkNewMacro(Self);

  itkSetMacro(Origin,PointType);
  itkGetConstReferenceMacro(Origin,PointType);

  itkSetMacro(Spacing,SpacingType);
  itkGetConstReferenceMacro(Spacing,SpacingType);
  
  itkSetMacro(ViewportSize,SizeType);
  itkGetConstReferenceMacro(ViewportSize,SizeType);

  void SetRotationCenter(const PointType & center)
  {
    m_RotationCenter = center;
  }
  itkGetConstReferenceMacro(RotationCenter,PointType);

  void SetRotationAngle(const double & angle)
  {
    m_RotationAngle = angle;
  }

  itkGetConstReferenceMacro(RotationAngle,double);

  // Order of priority is Wkt, then keywordlist, then unknown
  void SetWkt(const std::string & wkt)
  {
    m_Wkt = wkt;
    m_GeometryChanged = true;
  }
  itkGetStringMacro(Wkt);

  void SetUseProjection(bool usep)
  {
    m_UseProjection = usep;
    m_GeometryChanged = true;
  }

  itkGetConstReferenceMacro(UseProjection,bool);
  itkBooleanMacro(UseProjection);

  itkSetMacro(GeometryChanged,bool);
  itkGetMacro(GeometryChanged,bool);
  itkGetConstReferenceMacro(GeometryChanged,bool);

  void SetKeywordList(const KeywordListType& kwl)
  {
    m_KeywordList = kwl;
    m_GeometryChanged = true;
  }

  itkGetConstReferenceMacro(KeywordList,KeywordListType);

  // Retrieve the current Viewport extent
  void GetViewportExtent(double & ulx, double & uly, double & lrx, double & lry) const;
  
  PointType GetViewportCenter() const;

  void ScreenToViewPortTransform(const double & xscreen, const double & yscreen, double & xvp, double & yvp) const;

  void ViewportToScreenTransform(const double & xvp, const double & yvp, double & xscreen, double & yscreen) const;

  void Zoom(const PointType & zoomCenter,double scale = 1.1);

  void Center(const PointType & center);

protected:
  ViewSettings();

  virtual ~ViewSettings();

private:
  // prevent implementation
  ViewSettings(const Self&);
  void operator=(const Self&);

  PointType   m_Origin;
  SpacingType m_Spacing;
  SizeType    m_ViewportSize;
  std::string m_Wkt;
  KeywordListType m_KeywordList;
  bool m_UseProjection;
  bool m_GeometryChanged;
  PointType   m_RotationCenter;
  double      m_RotationAngle;

}; // End class ViewSettings

} // End namespace otb

#endif
