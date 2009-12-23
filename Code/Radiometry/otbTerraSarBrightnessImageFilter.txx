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
#ifndef __otbTerraSarBrightnessImageFilter_txx
#define __otbTerraSarBrightnessImageFilter_txx

#include "otbTerraSarBrightnessImageFilter.h"
//#include "otbImageMetadataInterfaceFactory.h"
//#include "otbImageMetadataInterfaceBase.h"
#include "otbTerraSarImageMetadataInterface.h"

namespace otb
{

template <class TInputImage, class TOutputImage>
void
TerraSarBrightnessImageFilter<TInputImage,TOutputImage>
::BeforeThreadedGenerateData()
{
  Superclass::BeforeThreadedGenerateData();

  // If the user doesn't set it AND the metadata is available, set calFactor using image metadata
 std::cout<<this->GetCalFactor()<<std::endl;
  if (this->GetCalFactor() == itk::NumericTraits<double>::min()) 
    {
      /** TODO : use a factory for RADAR image metadata interface */
      TerraSarImageMetadataInterface::Pointer lImageMetadata = otb::TerraSarImageMetadataInterface::New();
      if( !lImageMetadata->CanRead(this->GetInput()->GetMetaDataDictionary()) )
	{
	  itkExceptionMacro(<<"Invalid input image. Only TerraSar images are supproted");
	}
      this->SetCalFactor( lImageMetadata->GetCalibrationFactor(this->GetInput()->GetMetaDataDictionary()) );
    }
}

}

#endif
