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
      

    /**
    This code compute the shift between PAN and XS image from a bundle
    product in SENSOR geometry level using acquisition start time of
    XS and PAN images, as well as PAN sampling rate to get the shift
    in lines and FIRST_COL metadata to get shift in columns. Here are
    two examples of how this computation is done:

    Product 6543
                              XS                            PA
      START                   17:16:57.2374640              17:16:57.2373170
      Te                      0.294                         0.0735
      FIRST_COL               3577                          14304

      deltaMS-PA =  0.000147
      deltaMS-PA_nb_lin = deltaMS-PA / (TePA / 1000) = 2 lines
      deltacolMS-PA = (FIRST_COL _MS -1 )*4 + 1 - FIRST_COL_PA = 1 column

    Product 0445
                              XS                            PA
      START                   17:20:45.2371140              17:20:45.2369670
      Te                      0.294                         0.0735
      FIRST_COL               526                           2101

      deltaMS-PA =  0.000147
      deltaMS-PA_nb_lin = deltaMS-PA / (TePA / 1000) = 2 lines
      deltacolMS-PA = (FIRST_COL _MS -1 )*4 + 1 - FIRST_COL_PA = 0 column

    In order to get a transform from PAN physical space to XS physical
    space, we have to take into account two additional effects:
    - The shifts above must be multiplied by -1 to get the correct
      transform direction,
    - In SENSOR geometry level, sampling grids of XS and PAN product
      are phased, which means that a shift of 1.5 pixels must be added
      in both directions in order to get the upper-left corners of XS
      and PAN pixels to match.

    This leads to the following formula:
     */
    double lineShift_MS_P = -vcl_floor((timeDelta/(linePeriodPan/1000)+0.5))+1.5;
    double colShift_MS_P =  -((colStartXS-1)*4 - colStartPan + 1)+1.5;

     
    // Apply the scaling
    typedef itk::ScalableAffineTransform<double, 2>  TransformType;
    TransformType::Pointer transform = TransformType::New();
    transform->SetIdentity();

    // Apply the offset
    TransformType::OutputVectorType offset;
    offset[0] = colShift_MS_P;
    offset[1] = lineShift_MS_P;
    transform->Translate(offset);

    // Apply the scaling
    transform->Scale(0.25);

    return transform;
    }

} // End namespace otb

#endif


