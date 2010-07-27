/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Some parts of this code are derived from ITK. See ITKCopyright.txt
  for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbSarRadiometricCalibrationToImageFilter_txx
#define __otbSarRadiometricCalibrationToImageFilter_txx

#include "otbSarRadiometricCalibrationToImageFilter.h"

namespace otb
{

/**
 * Constructor
 */
template<class TInputImage, class TOutputImage>
SarRadiometricCalibrationToImageFilter<TInputImage, TOutputImage>
::SarRadiometricCalibrationToImageFilter()
{
	  typename FunctionType::Pointer function = FunctionType::New();
      this->SetFunction(function);
}

} // end namespace otb

#endif
