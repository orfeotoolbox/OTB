/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otb_ViewSettings_h
#define otb_ViewSettings_h

#include "itkObject.h"
#include "itkSmartPointer.h"
#include "itkPoint.h"
#include "itkVector.h"
#include "itkSize.h"

#include "otbImageKeywordlist.h"
#include "OTBIceExport.h"
#include <string>

namespace otb
{

class OTBIce_EXPORT ViewSettings 
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

  void Reset();

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
    // std::cout << "Wkt: '" << m_Wkt << "' -> '" << wkt << "'" << std::endl;    
    // std::cout << std::hex << this << "::SetWkt(" << (m_Wkt!=wkt) << ")" << std::endl;

    if(m_Wkt != wkt)
      {
      // std::cout << "geometry-changed: TRUE" << std::endl;

      m_Wkt = wkt;
      m_GeometryChanged = true;
      }
  }

  itkGetConstReferenceMacro( Wkt, std::string );

  void SetUseProjection(bool usep)
  {
    // std::cout << std::hex << this << "::SetUseProjection(" << usep << ")" << std::endl;

    if(m_UseProjection != usep)
      {
      // std::cout << "geometry-changed: TRUE " << std::endl;

      m_UseProjection = usep;
      m_GeometryChanged = true;
      }
  }

  itkGetConstMacro(UseProjection, bool);
  itkBooleanMacro(UseProjection);

  itkSetMacro(GeometryChanged,bool);
  itkGetConstMacro(GeometryChanged,bool);

  void SetKeywordList(const KeywordListType& kwl)
  {
    // std::cout << std::hex << this << "::SetKeywordList()" << std::endl;

    if(!(m_KeywordList==kwl))
      {
      // std::cout << "geometry-changed: TRUE" << std::endl;

      m_KeywordList = kwl;
      m_GeometryChanged = true;
      }
  }

  itkGetConstReferenceMacro(KeywordList,KeywordListType);

  // Retrieve the current Viewport extent
  void GetViewportExtent(double & ulx, double & uly, double & lrx, double & lry) const;
  
  PointType GetViewportCenter() const;

  void ScreenToViewPortTransform(const double & xscreen, const double & yscreen, double & xvp, double & yvp) const;

  void ViewportToScreenTransform(const double & xvp, const double & yvp, double & xscreen, double & yscreen) const;

  void Zoom(const PointType & zoomCenter,double scale = 1.1);

  void Center(const PointType & center);

  void SetPersepectiveAngle();

  void SetNorthUpAngle();

  void UpdateRotation(const PointType & newCenter, double newAngle);

  double GetScale( const PointType & origin, const PointType & extent, bool isSigned ) const;

protected:
  ViewSettings();

  ~ViewSettings() override;

private:
  // prevent implementation
  ViewSettings(const Self&);
  void operator=(const Self&);

  PointType m_Origin;
  SpacingType m_Spacing;
  SizeType m_ViewportSize;
  std::string m_Wkt;
  KeywordListType m_KeywordList;
  PointType m_RotationCenter;
  double m_RotationAngle;
  bool m_UseProjection : 1;
  bool m_GeometryChanged : 1;

}; // End class ViewSettings

} // End namespace otb

#endif
