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

#include "otbReflectanceToSurfaceReflectanceImageFilter.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbAtmosphericRadiativeTerms.h"

int otbReflectanceToSurfaceReflectanceImageFilter(int argc, char * argv[])
{
  const char * inputFileName  = argv[1];
  const char * outputFileName = argv[2];


  const unsigned int Dimension = 2;
  typedef double PixelType;
  typedef otb::VectorImage<PixelType,Dimension> InputImageType;
  typedef otb::VectorImage<PixelType,Dimension> OutputImageType;
  typedef otb::ImageFileReader<InputImageType>  ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;
  typedef otb::ReflectanceToSurfaceReflectanceImageFilter<InputImageType, OutputImageType> ReflectanceToSurfaceReflectanceImageFilterType;
  typedef otb::AtmosphericRadiativeTerms::DataVectorType DataVectorType;
  otb::AtmosphericRadiativeTerms::Pointer atmo = otb::AtmosphericRadiativeTerms::New();


  ReaderType::Pointer reader  = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();
  reader->SetFileName(inputFileName);
  writer->SetFileName(outputFileName);

  reader->GenerateOutputInformation();
  unsigned int nbChannel = reader->GetOutput()->GetNumberOfComponentsPerPixel();

  DataVectorType intrinsic;
  DataVectorType albedo;
  DataVectorType gaseous;
  DataVectorType downTrans;
  DataVectorType upTrans;
    
  std::cout<<nbChannel<<std::endl;
  for( unsigned int j=0; j<nbChannel; j++)
    {
      intrinsic.push_back(static_cast<double>(atof(argv[3+j])));
      albedo.push_back(static_cast<double>(atof(argv[3+j+nbChannel])));
      gaseous.push_back(static_cast<double>(atof(argv[3+j+2*nbChannel])));
      downTrans.push_back(static_cast<double>(atof(argv[3+j+3*nbChannel])));
      upTrans.push_back(static_cast<double>(atof(argv[3+j+4*nbChannel])));
    }

  atmo->SetIntrinsicAtmosphericReflectances(intrinsic);
  atmo->SetSphericalAlbedos(albedo);
  atmo->SetTotalGaseousTransmissions(gaseous);
  atmo->SetDownwardTransmittances(downTrans);
  atmo->SetUpwardTransmittances(upTrans);  
      
  // Instantiating object
  ReflectanceToSurfaceReflectanceImageFilterType::Pointer filter = ReflectanceToSurfaceReflectanceImageFilterType::New();
  filter->SetAtmosphericRadiativeTerms(atmo);
  filter->SetInput(reader->GetOutput());
  writer->SetInput(filter->GetOutput());

  writer->Update();


  return EXIT_SUCCESS;
}
