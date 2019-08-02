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

#include "otbVectorImage.h"
#include "otbWrapperApplicationRegistry.h"
#include "otbWrapperTypes.h"
#include <string>

typedef otb::VectorImage<unsigned char> VectorImageType;
typedef VectorImageType::PixelType PixelType;

/* This function is creating and filling a vector image */
VectorImageType::Pointer create_vector_image( int pxl_s , int nb_comp , unsigned char value )
{
  VectorImageType::SizeType size;
  size.Fill(pxl_s);
  VectorImageType::IndexType index;
  index.Fill(0);
  VectorImageType::RegionType region;
  region.SetSize(size);
  region.SetIndex(index);

  VectorImageType::Pointer image = VectorImageType::New();

  image->SetLargestPossibleRegion( region );
  image->SetBufferedRegion( region );
  image->SetRequestedRegion( region );
  image->SetNumberOfComponentsPerPixel(nb_comp);
  image->Allocate();
  PixelType val(nb_comp);
  val.Fill(value);
  image->FillBuffer(val);
  return image;
}

int main(int , char * argv[] )
{
  int return_val = 0;
  auto img1 = create_vector_image(5,2,1);
  auto img2 = create_vector_image(5,1,2);
  VectorImageType::IndexType index;
  index.Fill(3); // Center of the images
  auto app = otb::Wrapper::ApplicationRegistry::CreateApplication("BandMathX");
  app->AddImageToParameterInputImageList("il", img1);
  app->UpdateParameters();
  app->AddImageToParameterInputImageList("il", img2);
  app->UpdateParameters();

  // Case one: only expression
  app->SetParameterString("exp", "im1b1+im2b1");
  app->UpdateParameters();
  app->SetParameterOutputImagePixelType("out", otb::Wrapper::ImagePixelType::ImagePixelType_uint8);
  app->Execute();
  auto output = app->GetParameterImageBase("out");
  output->Update();
  float im_val = 0;
  // We need to be carefull as we are taking the direct output of the underlying 
  // filter in the application
  auto output_int = dynamic_cast<otb::VectorImage<float> *>(output);
  if( output_int ) // this need to crash if not the case
  {
    im_val = output_int->GetPixel(index).GetElement(0);
    if ( im_val != 3 )
    {
      std::cout<<"Wrong value in test, was expecting 3, got "<<im_val<<std::endl;
      return_val++;
    }
  }
  else
  {
    std::cout<<"Not the right conversion, cannot retrieve the output"<<std::endl;
    return EXIT_FAILURE;
  }

  // Case two: expression and context
  app->SetParameterString("exp", "im1b1+val-im2b1");
  app->UpdateParameters();
  auto desc = app->GetParameterDescription("exp");
  if (desc.find("Following variables not allowed : val") == std::string::npos)
  {
    std::cout<<"Cannot find usual value in the parameter description."<<std::endl;
    std::cout<<"The test was looking for \"Following variables not allowed : val\""
    <<" in the parameter description and got \""<<desc<<"\" instead."<<std::endl;
    return_val++;
  }

  std::string context_path(argv[1]);
  app->SetParameterString("incontext",argv[1]);
  // val is set in the context to 1
  app->UpdateParameters();
  desc = app->GetParameterDescription("exp");
  if (desc.find("Valid expression") == std::string::npos )
  {
    std::cout<<"Cannot find usual value in the parameter description."<<std::endl;
    std::cout<<"The test was looking for \"Valid expression\""
    <<" in the parameter description and got \""<<desc<<"\" instead."<<std::endl;
    return_val++;
  }
  
  app->Execute();
  output = app->GetParameterImageBase("out");
  output->Update();
  // We need to be carefull as we are taking the direct output of the underlying 
  // filter in the application
  output_int = dynamic_cast<otb::VectorImage<float> *>(output);
  if( output_int ) // this need to crash if not the case
  {
    im_val = output_int->GetPixel(index).GetElement(0);
    if ( im_val != 0 )
    {
      std::cout<<"Wrong value in test, was expecting 0, got "<<im_val<<std::endl;
      return_val++;
    }
  }
  else
  {
    std::cout<<"Not the right conversion, cannot retrieve the output"<<std::endl;
    return EXIT_FAILURE;
  }

  // Case three: no expression and context
  app->SetParameterString("exp", "");
  app->UpdateParameters();
  auto exp = app->GetParameterString("exp");
  if (exp.find("im1b1 + 2*val + im2b1") == std::string::npos )
  {
    std::cout<<"The expression value is not set correctly."<<std::endl;
    std::cout<<"The test was looking for \"im1b1 + 2*val + im2b1\""
    <<" in the parameter value and got \""<<exp<<"\" instead."<<std::endl;
    return_val++;
  }
  app->Execute();
  output = app->GetParameterImageBase("out");
  output->Update();
  // We need to be carefull as we are taking the direct output of the underlying 
  // filter in the application
  output_int = dynamic_cast<otb::VectorImage<float> *>(output);
  if( output_int ) // this need to crash if not the case
  {
    im_val = output_int->GetPixel(index).GetElement(0);
    if (im_val != 5 )
    {
      std::cout<<"Wrong value in test, was expecting 5, got "<<im_val<<std::endl;
      return_val++;
    }
  }
  else
  {
    std::cout<<"Not the right conversion, cannot retrieve the output"<<std::endl;
    return EXIT_FAILURE;
  }
  return return_val;
}