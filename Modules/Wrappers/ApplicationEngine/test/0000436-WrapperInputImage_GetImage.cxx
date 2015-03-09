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

#include "otbWrapperInputImageParameter.h"


#include "otbLineSegmentDetector.h"

#include "otbVectorDataFileWriter.h"
#include "otbImageFileWriter.h"

// Code showing a problem calling twice GetImputImageParameter
// http://bugs.orfeo-toolbox.org/view.php?id=436

int main(int argc, char *argv[])
{
  if (argc != 3)
    {
    std::cout << "Usage : <inputImage>" << std::endl;

    return EXIT_FAILURE;
    }

  typedef otb::Wrapper::InputImageParameter InputImageParameterType;
  InputImageParameterType::Pointer param = InputImageParameterType::New();

  param->SetFromFileName(argv[2]);

  if( param->GetImage() != param->GetImage() )
    {
      return EXIT_FAILURE;
    }



  return EXIT_SUCCESS;
}
