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

#include "otbStreamingStatisticsVectorImageFilter.h"
#include "otbImageFileReader.h"
#include "otbVectorImage.h"
#include <fstream>
#include "otbStreamingTraits.h"

int otbStreamingStatisticsVectorImageFilter(int argc, char * argv[])
{
  try
    {
      
      const char * infname = argv[1];
      const char * outfname = argv[2];
      
      const unsigned int Dimension = 2;
      typedef double PixelType;
      
      typedef otb::VectorImage<PixelType,Dimension> ImageType;
      typedef otb::ImageFileReader<ImageType> ReaderType;   
      typedef otb::StreamingStatisticsVectorImageFilter<ImageType> StreamingStatisticsVectorImageFilterType;
      
      // Instantiating object
      StreamingStatisticsVectorImageFilterType::Pointer filter = StreamingStatisticsVectorImageFilterType::New();
      
      
      ReaderType::Pointer reader = ReaderType::New();
      reader->SetFileName(infname);
      
      //filter->SetStreamingMode(otb::SET_NUMBER_OF_STREAM_DIVISIONS);
      filter->GetStreamer()->SetNumberOfStreamDivisions(200);
      filter->SetInput(reader->GetOutput());
      filter->Update();


      std::ofstream file;
      file.open(outfname);
      file<<"Minimum: "<<filter->GetMinimum()<<std::endl;
      file<<"Maximum: "<<filter->GetMaximum()<<std::endl;
      file<<std::fixed;
      file.precision(5);
      file<<"Sum: "<<filter->GetSum()<<std::endl;
      file<<"Mean: "<<filter->GetMean()<<std::endl;
      file<<"Covariance: "<<filter->GetCovariance()<<std::endl;
      file.close();
    }

  catch( itk::ExceptionObject & err ) 
    { 
    std::cout << "Exception itk::ExceptionObject thrown !" << std::endl; 
    std::cout << err << std::endl; 
    return EXIT_FAILURE;
    } 

  catch( ... ) 
    { 
    std::cout << "Unknown exception thrown !" << std::endl; 
    return EXIT_FAILURE;
    } 
  return EXIT_SUCCESS;
}
