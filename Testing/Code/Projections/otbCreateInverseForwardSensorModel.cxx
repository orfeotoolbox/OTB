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


/*!
 *
 * PURPOSE:
 *
 * Application to rproject an image region into gepgraphical coordinates
 * usinf un Interpolator+regionextractor and an Iterator.
 *
 */

// iostream is used for general output
#include <iostream>
#include <iterator>
#include <stdlib.h>

#include "otbMacro.h"

#include "otbImage.h"
#include "otbImageFileReader.h"

#include "otbInverseSensorModel.h"
#include "otbForwardSensorModel.h"

int otbCreateInverseForwardSensorModel(int argc, char* argv[])
{
  if (argc != 2)
    {
    std::cout << argv[0] << " <input filename>" << std::endl;
    return EXIT_FAILURE;
    }

  typedef otb::Image<unsigned int, 2>     ImageType;
  typedef otb::ImageFileReader<ImageType> ReaderType;
  typedef otb::InverseSensorModel<double> InverseModelType;
  typedef otb::ForwardSensorModel<double> ForwardModelType;

  //Allocate pointer
  InverseModelType::Pointer inverse_model = InverseModelType::New();
  ForwardModelType::Pointer forward_model = ForwardModelType::New();
  ReaderType::Pointer       reader = ReaderType::New();

  // Set parameters ...
  reader->SetFileName(argv[1]);

  // Read meta data (ossimKeywordlist)
  reader->GenerateOutputInformation();
  ImageType::Pointer inputImage = reader->GetOutput();

  otbGenericMsgDebugMacro(<< "Inverse model creation...");
  inverse_model->SetImageGeometry(inputImage->GetImageKeywordlist());
  if( inverse_model->IsValidSensorModel() == false )
    {
      std::cout<<"Invalid Model pointer m_Model == NULL!\n The ossim keywordlist is invalid!"<<std::endl;
      return EXIT_FAILURE;
    }

  otbGenericMsgDebugMacro(<< "Foreward model creation...");
  forward_model->SetImageGeometry(inputImage->GetImageKeywordlist());
  if( forward_model->IsValidSensorModel() == false )
    {
      std::cout<<"Invalid Model pointer m_Model == NULL!\n The ossim keywordlist is invalid!"<<std::endl;
      return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
