/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "otbWrapperOutputImageParameter.h"
#include "otbWrapperInputImageParameter.h"
#include "otbImageFileReader.h"
#include "otbWrapperTypes.h"
#include <vector>


int otbWrapperOutputImageParameterTest1(int itkNotUsed(argc), char* argv[])
{
  typedef otb::Wrapper::OutputImageParameter OutputImageParameterType;
  OutputImageParameterType::Pointer param = OutputImageParameterType::New();

  typedef otb::ImageFileReader< otb::Wrapper::FloatVectorImageType > ReaderType;

  ReaderType::Pointer reader = ReaderType::New();

  reader->SetFileName( argv[1] );
  reader->UpdateOutputInformation();

  param->SetValue(reader->GetOutput());
  param->SetFileName( argv[2] );

  param->SetKey(argv[3]);
  param->SetDescription(argv[4]);

  // an explicit writer initialization is needed
  param->InitializeWriters();
  param->Write();

  return EXIT_SUCCESS;
}


// template < typename ImageType >
// void Cross( int p , std::string inputfilename, std::string outputfilename)
// {
//   otb::Wrapper::InputImageParameter::Pointer paramIn (
//     otb::Wrapper::InputImageParameter::New() );
//   paramIn->SetFromFileName( inputfilename ); 
//   otb::Wrapper::OutputImageParameter::Pointer paramOut( 
//     otb::Wrapper::OutputImageParameter::New() ); 
//   paramOut->SetFileName( outputfilename );
//   paramOut->SetImage(paramIn->GetImage<ImageType>());
//   paramOut->InitializeWriters();
//   paramOut->SetPixelType(static_cast<otb::Wrapper::ImagePixelType>(p));
//   paramOut->Write();
// }


// int otbWrapperOutputImageParameterConversionTest(int , char* argv[])
// {
//   std::string filenamein = argv[1];
//   std::string filenameout = argv[2] ;
//   std::string extension = filenameout.substr( filenameout.find_last_of('.') );

//   filenameout = filenameout.substr( 0 , filenameout.find_last_of('.') );

//   for ( int i = otb::Wrapper::ImagePixelType_uint8 ; i < 11 ; i++ )
//     {
//       std::string type = 
//       otb::Wrapper::OutputImageParameter::ConvertPixelTypeToString(
//         static_cast<otb::Wrapper::ImagePixelType>(i) );
//       Cross< otb::Wrapper::UInt8ImageType > (i , filenamein , filenameout+"_UInt8_"+ type + extension ) ;
//       Cross< otb::Wrapper::Int16ImageType > ( i , filenamein , filenameout+"_Int16_"+ type + extension ) ;
//       Cross< otb::Wrapper::UInt16ImageType > ( i , filenamein , filenameout+"_UInt16_"+ type + extension ) ;
//       Cross< otb::Wrapper::Int32ImageType > ( i , filenamein , filenameout+"_Int21_"+ type + extension ) ;
//       Cross< otb::Wrapper::UInt32ImageType > ( i , filenamein , filenameout+"_UInt32_"+ type + extension ) ;
//       Cross< otb::Wrapper::FloatImageType > ( i , filenamein , filenameout+"_float_"+ type + extension ) ;
//       Cross< otb::Wrapper::DoubleImageType > ( i , filenamein , filenameout+"_double_"+ type + extension ) ;
//       Cross< otb::Wrapper::UInt8VectorImageType > ( i , filenamein , filenameout+"_UInt8Vect_"+ type + extension ) ;
//       Cross< otb::Wrapper::Int16VectorImageType > ( i , filenamein , filenameout+"_Int16Vect_"+ type + extension ) ;
//       Cross< otb::Wrapper::UInt16VectorImageType > ( i , filenamein , filenameout+"_UInt16Vect_"+ type + extension ) ;
//       Cross< otb::Wrapper::Int32VectorImageType > ( i , filenamein , filenameout+"_Int21Vect_"+ type + extension ) ;
//       Cross< otb::Wrapper::UInt32VectorImageType > ( i , filenamein , filenameout+"_UInt32Vect_"+ type + extension ) ;
//       Cross< otb::Wrapper::FloatVectorImageType > ( i , filenamein , filenameout+"_floatVect_"+ type + extension ) ;
//       Cross< otb::Wrapper::DoubleVectorImageType > ( i , filenamein , filenameout+"_doubleVect_"+ type + extension ) ;
//       Cross< otb::Wrapper::ComplexInt16ImageType > ( i , filenamein , filenameout+"_CInt16_"+ type + extension ) ;
//       Cross< otb::Wrapper::ComplexInt32ImageType > ( i , filenamein , filenameout+"_CInt32_"+ type + extension ) ;
//       Cross< otb::Wrapper::ComplexFloatImageType > ( i , filenamein , filenameout+"_Cfloat_"+ type + extension ) ;
//       Cross< otb::Wrapper::ComplexDoubleImageType > ( i , filenamein , filenameout+"_Cdouble_"+ type + extension ) ;
//       Cross< otb::Wrapper::ComplexInt16VectorImageType > ( i , filenamein , filenameout+"_CInt16Vect_"+ type + extension ) ;
//       Cross< otb::Wrapper::ComplexInt32VectorImageType > ( i , filenamein , filenameout+"_CInt32Vect_"+ type + extension ) ;
//       Cross< otb::Wrapper::ComplexFloatVectorImageType > ( i , filenamein , filenameout+"_CfloatVect_"+ type + extension ) ;
//       Cross< otb::Wrapper::ComplexDoubleVectorImageType > ( i , filenamein , filenameout+"_CdoubleVect_"+ type + extension ) ;
//     }
//   return 0;
// }
