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
#ifndef otb_GlROIActor_h
#define otb_GlROIActor_h

#include "otbGlActor.h"

#include "otbGenericRSTransform.h"
#include "otbFragmentShader.h"
#include "otbImageKeywordlist.h"
#include "itkImageRegion.h"
#include "itkVector.h"

namespace otb
{

class OTBIce_EXPORT GlROIActor 
  : public GlActor
{
public:
  typedef GlROIActor                                Self;
  typedef GlActor                                         Superclass;
  typedef itk::SmartPointer<Self>                         Pointer;
  typedef itk::SmartPointer<const Self>                   ConstPointer;
  typedef ImageKeywordlist                                ImageKeywordlistType;
  typedef itk::ImageRegion<2>                             ImageRegionType;
  typedef otb::GenericRSTransform<>                       RSTransformType;
  typedef RSTransformType::InputPointType                 PointType;
  typedef itk::Vector<double,3>                           ColorType;

  itkNewMacro(Self);

  // Retrieve the full extent of the actor
  void GetExtent(double & ulx, double & uly, double & lrx, double & lry) const ITK_OVERRIDE;

  // Update internal actor state with respect to ViewSettings
  void ProcessViewSettings() ITK_OVERRIDE;

  // Heavy load/unload operations of data
  void UpdateData() ITK_OVERRIDE;

  // Gl rendering of current state
  void Render() ITK_OVERRIDE;

  void SetUL( const PointType & );

  itkGetConstReferenceMacro(UL,PointType);

  void SetLR( const PointType & );

  itkGetConstReferenceMacro(LR,PointType);

  itkSetStringMacro(Wkt);
  itkGetStringMacro(Wkt);

  void SetKwl(const ImageKeywordlistType & kwl);
  itkGetConstReferenceMacro(Kwl,ImageKeywordlistType);
  
  itkSetMacro(Color,ColorType);
  itkGetConstReferenceMacro(Color,ColorType);
  
  itkSetMacro(Alpha,double);
  itkGetConstReferenceMacro(Alpha,double);

  itkSetMacro(Fill,bool);
  itkGetMacro(Fill,bool);
  itkBooleanMacro(Fill);

protected:
  GlROIActor();
  
  ~GlROIActor() ITK_OVERRIDE;
  
private:
  // prevent implementation
  GlROIActor(const Self&);
  void operator=(const Self&);

  void UpdateTransforms();
 
  PointType            m_UL;
  PointType            m_LR;

  std::string          m_Wkt;
  ImageKeywordlistType m_Kwl;

  ColorType            m_Color;
  double               m_Alpha;
  bool                 m_Fill;
  
  RSTransformType::Pointer m_ViewportToImageTransform;
  RSTransformType::Pointer m_ImageToViewportTransform;

  PointType            m_VpUL;
  PointType            m_VpUR;
  PointType            m_VpLL;
  PointType            m_VpLR;

}; // End class GlROIActor

} // End namespace otb

#endif
