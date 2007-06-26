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

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "itkExceptionObject.h"
#include "itkBinaryMedianImageFilter.h"

#include <iostream>

#include "otbImage.h"

#include "otbImageFileReader.h"
#include "otbStreamingImageFileWriter.h"
#include "otbImageKeywordlist.h"
#include "otbMetaDataKey.h"

int otbOSSIMImageMetaDataReaderTest (int argc, char* argv[])
{
  try
  {
		
        // Verify the number of parameters in the command line
        const char * inputFilename  = argv[1];
		const char * outputAsciiFilename  = argv[2];
        //const char * outputFilename = argv[2];
       

        typedef unsigned char  	                                InputPixelType;
        typedef unsigned char  	                                OutputPixelType;
        const   unsigned int        	                        Dimension = 2;

        typedef otb::Image< InputPixelType,  Dimension >        InputImageType;
        typedef otb::Image< OutputPixelType, Dimension >        OutputImageType;

        typedef otb::ImageFileReader< InputImageType  >         ReaderType;
        typedef otb::StreamingImageFileWriter< OutputImageType> StreamingWriterType;

        ReaderType::Pointer reader = ReaderType::New();
        reader->SetFileName( inputFilename  );
		//reader->GenerateOutputInformation();
	
		typedef itk::BinaryMedianImageFilter<InputImageType,OutputImageType>  MedianFilterType;
		MedianFilterType::Pointer medianFilter = MedianFilterType::New();
		
		medianFilter->SetInput(reader->GetOutput());
//		medianFilter->GenerateOutputInformation();
		medianFilter->GetOutput()->UpdateOutputInformation();

  		otb::ImageKeywordlist otb_tmp;
		itk::ExposeMetaData< otb::ImageKeywordlist >(medianFilter->GetOutput()->GetMetaDataDictionary(),
	  											 otb::MetaDataKey::m_OSSIMKeywordlistKey,
												 otb_tmp);
		ossimKeywordlist ossim_kwl;
		otb_tmp.convertToOSSIMKeywordlist(ossim_kwl);
		
		otbMsgDebugMacro( <<"Dans EXE ");
		otbMsgDebugMacro( << " -> Ossim key word list copy : "<<ossim_kwl<<std::endl);
  		otbMsgDebugMacro( <<"FIN ");
		
		
		std::ofstream file;
		file.open(outputAsciiFilename);
		file << "--- OSSIM KEYWORDLIST ---" << std::endl;
		file << ossim_kwl;
		file.close();
// ../../../bin/thomas ~/ORFEO-TOOLBOX/otb/OTB-Data/LargeInput/TOULOUSE/QuickBird/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
/*        StreamingWriterType::Pointer writer = StreamingWriterType::New();
        writer->SetFileName( outputFilename );
        writer->SetNumberOfStreamDivisions( 10 );
        writer->SetInput( reader->GetOutput() );
        writer->Update(); 
  */      
  } 
  catch( itk::ExceptionObject & err ) 
  { 
    std::cerr << "Exception OTB attrappee dans exception ITK !" << std::endl; 
    std::cerr << err << std::endl; 
    return EXIT_FAILURE;
  } 
  catch( ... )
  {
    std::cerr << "Exception OTB non attrappee !" << std::endl; 
    return EXIT_FAILURE;
  }
  
  return EXIT_SUCCESS;
}
