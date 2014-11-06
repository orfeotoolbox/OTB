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
#ifndef __otbPleiadesPToXSAffineTransformCalculator__cxx
#define __otbPleiadesPToXSAffineTransformCalculator__cxx

#include "otbPleiadesPToXSAffineTransformCalculator.h"
#include "otbPleiadesImageMetadataInterface.h"
#include "otbDateTimeAdapter.h"
#include "otbImageKeywordlist.h"
#include "itkMetaDataObject.h"

namespace otb {

bool
PleiadesPToXSAffineTransformCalculator
::CanCompute(const itk::ImageBase<2> * panchromaticImage, const itk::ImageBase<2> * xsImage)
{
  bool isPanPHR = false;
  bool isXSPHR = false;
      
  otb::PleiadesImageMetadataInterface::Pointer phrIMI =
    otb::PleiadesImageMetadataInterface::New();
  phrIMI->SetMetaDataDictionary(panchromaticImage->GetMetaDataDictionary());
  isPanPHR = phrIMI->CanRead();
      
  phrIMI->SetMetaDataDictionary(xsImage->GetMetaDataDictionary());
  isXSPHR = phrIMI->CanRead();
      
  if (isPanPHR && isXSPHR)
    {
    ImageKeywordlist kwlPan;
    ImageKeywordlist kwlXS;
        
    itk::ExposeMetaData<ImageKeywordlist>(
      panchromaticImage->GetMetaDataDictionary(),
      MetaDataKey::OSSIMKeywordlistKey,
      kwlPan);
        
    itk::ExposeMetaData<ImageKeywordlist>(
      xsImage->GetMetaDataDictionary(),
      MetaDataKey::OSSIMKeywordlistKey,
      kwlXS);
    
    // Get geometric processing
    std::string panProcessing = kwlPan.GetMetadataByKey("support_data.processing_level");
    std::string xsProcessing = kwlXS.GetMetadataByKey("support_data.processing_level");
        
    if (panProcessing.compare("SENSOR") == 0 &&
        xsProcessing.compare("SENSOR") == 0)
      {
      
      std::string pid = kwlPan.GetMetadataByKey("image_id");
      std::string xsid = kwlXS.GetMetadataByKey("image_id");
      
      pid  = pid.substr(0,pid.size()-4);
      xsid = xsid.substr(0,xsid.size()-4);

      if(pid == xsid)
        {
        return true;
        }
      }
    }
  return false;
}

PleiadesPToXSAffineTransformCalculator
::TransformType::Pointer
PleiadesPToXSAffineTransformCalculator
::Compute(const itk::ImageBase<2> * panchromaticImage, const itk::ImageBase<2> * xsImage)
{
  if(!CanCompute(panchromaticImage,xsImage))
    {
    itkGenericExceptionMacro("Can not compute affine transform between images, they do not correspond to Pleiades sensor bundle.");
    }

    ImageKeywordlist kwlPan;
    ImageKeywordlist kwlXS;
    
    itk::ExposeMetaData<ImageKeywordlist>(
      panchromaticImage->GetMetaDataDictionary(),
      MetaDataKey::OSSIMKeywordlistKey,
      kwlPan);
        
    itk::ExposeMetaData<ImageKeywordlist>(
      xsImage->GetMetaDataDictionary(),
      MetaDataKey::OSSIMKeywordlistKey,
      kwlXS);

    // Compute time delta
    std::string strStartTimePan = kwlPan.GetMetadataByKey("support_data.time_range_start");
    std::string strStartTimeXS = kwlXS.GetMetadataByKey("support_data.time_range_start");
      
    DateTimeAdapter::Pointer startTimePan = DateTimeAdapter::New();
    DateTimeAdapter::Pointer startTimeXS = DateTimeAdapter::New();
      
    startTimePan->SetFromIso8601(strStartTimePan);
    startTimeXS->SetFromIso8601(strStartTimeXS);
      
    double timeDelta = startTimeXS->GetDeltaInSeconds(startTimePan);
      
    // Retrieve line period in Pan
    std::string tmpStr = kwlPan.GetMetadataByKey("support_data.line_period");
    double linePeriodPan = atof(tmpStr.c_str());
      
    // Retrieve column start
    tmpStr = kwlPan.GetMetadataByKey("support_data.swath_first_col");
    int colStartPan = atoi(tmpStr.c_str());
    tmpStr = kwlXS.GetMetadataByKey("support_data.swath_first_col");
    int colStartXS = atoi(tmpStr.c_str());
      
    // Compute shift between MS and P (in Pan pixels)
    // in order to keep the top left corners unchanged, apply an
    // additional offset of (3/2) panchro pixels
    std::cout<<timeDelta/(linePeriodPan/1000)<<std::endl;
    double lineShift_MS_P = (timeDelta/(linePeriodPan/1000))-1.5;
    double colShift_MS_P =  ((colStartXS)*4 - colStartPan)-1.5;
     
    // Apply the scaling
    typedef itk::ScalableAffineTransform<double, 2>  TransformType;
    TransformType::Pointer transform = TransformType::New();
    transform->SetIdentity();

    // Apply the offset
    TransformType::OutputVectorType offset;
    offset[0] = static_cast<double>(colShift_MS_P);

    // Y axis inverted
    offset[1] = static_cast<double>(-lineShift_MS_P);
    transform->Translate(offset);

    transform->Scale(0.25);

    // Invert the transform to get the P to XS transform    
    return transform;
    }

} // End namespace otb

#endif


