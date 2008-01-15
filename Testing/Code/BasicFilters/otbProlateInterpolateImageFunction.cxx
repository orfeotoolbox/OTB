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
   std::cout<<"Prolate : Test : while varie jusqua : "<<static_cast<unsigned int>(argc)<<std::endl;
  while(i<static_cast<unsigned int>(argc) && (i+1)<static_cast<unsigned int>(argc))
    {  std::cout<<"Prolate : Test : while i vaut : "<<i<<" -> "<<atof(argv[i])<<" , "<<atof(argv[i+1])<<std::endl;
      ContinuousIndexType idx;
      std::cout<<"Prolate : Test : while"<<std::endl;
      idx[0]=atof(argv[i]);
      idx[1]=atof(argv[i+1]);
      
      indicesList.push_back(idx);

      i+=2;
    }
  std::cout<<"Prolate : Test : New()"<<std::endl;
  // Instantiating object
  InterpolatorType::Pointer prolate = InterpolatorType::New();
  std::cout<<"Prolate : Test : Reader::New()"<<std::endl;
  ReaderType::Pointer reader = ReaderType::New();
  std::cout<<"Prolate : Test : Reader::SetFileName : "<<infname<<std::endl;
  reader->SetFileName(infname);
  std::cout<<"Prolate : Test : Update()"<<std::endl;
  reader->Update();
  std::cout<<"Prolate : Test : prolate::Setinput()"<<std::endl;
  prolate->SetInputImage(reader->GetOutput());
  std::cout<<"Prolate : Test : prolate::SetRadius() : "<<atoi(argv[3])<<std::endl;
  prolate->SetRadius(atoi(argv[3]));
  std::cout<<"Prolate : Test : output name : "<<outfname<<std::endl;
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

 std::cout<<"Prolate : Test : for : "<<outfname<<std::endl;
 unsigned mmm = 1;
  for(std::vector<ContinuousIndexType>::iterator it = indicesList.begin();it!=indicesList.end();++it)
    {std::cout<<"Prolate : Test : for indice : "<<mmm<<std::endl;
      file<<(*it)<<" -> "<<prolate->EvaluateAtContinuousIndex((*it))<<std::endl;
      mmm++;
    }
   std::cout<<"Prolate : Test : close"<<std::endl;
  file.close();
  
  return EXIT_SUCCESS;
}
