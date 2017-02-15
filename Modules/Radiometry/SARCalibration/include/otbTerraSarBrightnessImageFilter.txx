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
#ifndef otbTerraSarBrightnessImageFilter_txx
#define otbTerraSarBrightnessImageFilter_txx

#include "otbTerraSarBrightnessImageFilter.h"
#include "otbTerraSarImageMetadataInterface.h"

namespace otb
{

template <class TInputImage, class TOutputImage>
void
TerraSarBrightnessImageFilter<TInputImage, TOutputImage>
::BeforeThreadedGenerateData()
{
  Superclass::BeforeThreadedGenerateData();

  // Load metada
  TerraSarImageMetadataInterface::Pointer lImageMetadata = otb::TerraSarImageMetadataInterface::New();
  lImageMetadata->SetMetaDataDictionary(this->GetInput()->GetMetaDataDictionary());
  bool mdIsAvailable = lImageMetadata->CanRead();

  // If the user doesn't set it AND the metadata is available, set calFactor using image metadata
  if (this->GetCalibrationFactor() == itk::NumericTraits<double>::Zero)
    {
    if (mdIsAvailable)
      {
      this->SetCalibrationFactor(lImageMetadata->GetCalibrationFactor());
      }
    else
      {
      itkExceptionMacro(<< "Invalid input image. Only TerraSar images are supproted");
      }
    }
}

}

#endif
