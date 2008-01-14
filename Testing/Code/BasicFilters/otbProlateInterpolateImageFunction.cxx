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
#include "itkExceptionObject.h"

#include "otbProlateInterpolateImageFunction.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include <fstream>

int otbProlateInterpolateImageFunction(int argc, char * argv[])
{  
  const char * infname = argv[1];
  const char * outfname = argv[2];

  typedef otb::Image<double,2>                             ImageType;
  typedef otb::ProlateInterpolateImageFunction<ImageType>  InterpolatorType;
  typedef InterpolatorType::ContinuousIndexType            ContinuousIndexType;
  typedef otb::ImageFileReader<ImageType>                  ReaderType;

   unsigned int i = 4;

   std::vector<ContinuousIndexType> indicesList;

  while(i<static_cast<unsigned int>(argc) && (i+1)<static_cast<unsigned int>(argc))
    {
      ContinuousIndexType idx;

      idx[0]=atof(argv[i]);
      idx[1]=atof(argv[i+1]);
      
      indicesList.push_back(idx);

      i+=2;
    }

  // Instantiating object
  InterpolatorType::Pointer prolate = InterpolatorType::New();

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);
  reader->Update();
  prolate->SetInputImage(reader->GetOutput());
  prolate->SetRadius(atoi(argv[3]));
  /************************
  typedef InterpolatorType::VectorType            VectType;
  VectType tutu=prolate->GetResampledProfil();
  for (unsigned int i=0; i<tutu.size(); i++)
    {
      std::cout << i+1 << " : " << tutu[i] << std::endl;
    }
  ************************/
  std::ofstream file;
  file.open(outfname);


  for(std::vector<ContinuousIndexType>::iterator it = indicesList.begin();it!=indicesList.end();++it)
    {
      file<<(*it)<<" -> "<<prolate->EvaluateAtContinuousIndex((*it))<<std::endl;
    }
  
  file.close();
  
  return EXIT_SUCCESS;
}
