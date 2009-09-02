/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMetaVesselTubeConverter.h,v $
  Language:  C++
  Date:      $Date: 2009-04-07 14:34:11 $
  Version:   $Revision: 1.3 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkMetaVesselTubeConverter_h
#define __itkMetaVesselTubeConverter_h


#include "metaVesselTube.h"
#include "itkSpatialObject.h"
#include "itkVesselTubeSpatialObject.h"

namespace itk 
{

template <unsigned int NDimensions = 3>
class ITK_EXPORT MetaVesselTubeConverter
{

public:

  MetaVesselTubeConverter();
  ~MetaVesselTubeConverter() {};

  typedef itk::VesselTubeSpatialObject<NDimensions> SpatialObjectType;


  typedef typename SpatialObjectType::TransformType TransformType;

  typedef typename SpatialObjectType::Pointer SpatialObjectPointer;

  //typedef typename itk::NDimensionalSpatialObject NDimSpatialObject;

  SpatialObjectPointer ReadMeta(const char* name);

  bool WriteMeta(SpatialObjectType* spatialObject,const char* name);

  SpatialObjectPointer MetaVesselTubeToVesselTubeSpatialObject(
                                                        MetaVesselTube * Tube);
  MetaVesselTube* VesselTubeSpatialObjectToMetaVesselTube(
                                            SpatialObjectType * spatialObject);

};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
  #include "itkMetaVesselTubeConverter.txx"
#endif


#endif
