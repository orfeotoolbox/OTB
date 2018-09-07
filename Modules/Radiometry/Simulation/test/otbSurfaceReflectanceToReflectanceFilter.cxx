/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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


#include "otbMacro.h"
#include "otbSpectralResponse.h"

#include "otbSurfaceReflectanceToReflectanceFilter.h"

int otbSurfaceReflectanceToReflectanceFilterTest(int argc, char * argv[])
{
  if ( argc != 7 )
    {
    std::cout << argv[0] << std::endl << "\t" << "<RSR_filename>" << "\t" << "<intrinsic>"  << "\t" << "<albedeo>"  << "\t" << "<gaseous>"  << "\t" << "<downTrans>"  << "\t" << "<upTrans>"   << std::endl;
    return EXIT_FAILURE;
    }

  const unsigned int                            Dimension = 2;
  typedef double                                PixelType;
  typedef otb::VectorImage<PixelType, Dimension> ImageType;

//  typedef itk::ImageRegionIterator< ImageType > IteratorType;
  //typedef ResponseType::PairType    PairType;
  //typedef otb::ObjectList< PairType > PairListType;
  //   PairListType::Pointer pairList = PairListType::New();

  typedef otb::SurfaceReflectanceToReflectanceFilter< ImageType, ImageType>  SurfaceReflectanceToReflectanceFilterType;
  typedef SurfaceReflectanceToReflectanceFilterType::Pointer  SurfaceReflectanceToReflectanceFilterPointerType;

  typedef otb::SpectralResponse< PixelType, PixelType>  ResponseType;
  typedef ResponseType::Pointer  ResponsePointerType;
  //
  ResponsePointerType  myResponse=ResponseType::New();
  //Load file into vector
  const std::string file(argv[1]);
  myResponse->Load(file, 100.0);

  std::cout << "Input SpectResponse " << myResponse << std::endl;
  //rsr to image
  ImageType::IndexType start;
  start[0] =  0;
  start[1] =  0;

  ImageType::SizeType  size;
  size[0] = 1;
  size[1] = 1;

  std::cout << "Image size: " << size << std::endl;

  ImageType::PointType origin;
  origin[0] = 0;
  origin[1] = 0;
  //origin[1] = -90;

  ImageType::SpacingType spacing;
  spacing[0] = 1;
  spacing[1] = 1;
  //spacing[1] = -resolution;

  ImageType::RegionType region;
  region.SetSize( size );
  region.SetIndex( start );

  ImageType::Pointer image = ImageType::New();
  image->SetRegions( region );
  image->SetNumberOfComponentsPerPixel( (myResponse)->Size() );
  image->Allocate();

  ImageType::PixelType pixel;
  pixel.SetSize((myResponse)->Size());

  for ( unsigned int i = 0; i<(myResponse)->Size(); ++i )
    {
    pixel[i] = myResponse->GetResponse()[i].second;
    }
  image->SetPixel(start, pixel);

  //Instantiation
  SurfaceReflectanceToReflectanceFilterPointerType  myFilter=SurfaceReflectanceToReflectanceFilterType::New();

  //   myFilter->SetInput(image);
  //
  //   typedef otb::AtmosphericRadiativeTerms::DataVectorType DataVectorType;
  //   otb::AtmosphericRadiativeTerms::Pointer atmo = otb::AtmosphericRadiativeTerms::New();
  //
  //   DataVectorType intrinsic;
  //   DataVectorType albedo;
  //   DataVectorType gaseous;
  //   DataVectorType downTrans;
  //   DataVectorType upTrans;
  //
  //
  //   intrinsic.push_back(static_cast<double>(atof(argv[2])));
  //   albedo.push_back(static_cast<double>(atof(argv[3])));
  //   gaseous.push_back(static_cast<double>(atof(argv[4])));
  //   downTrans.push_back(static_cast<double>(atof(argv[5])));
  //   upTrans.push_back(static_cast<double>(atof(argv[6])));
  //
  //
  //   atmo->SetIntrinsicAtmosphericReflectances(intrinsic);
  //   atmo->SetSphericalAlbedos(albedo);
  //   atmo->SetTotalGaseousTransmissions(gaseous);
  //   atmo->SetDownwardTransmittances(downTrans);
  //   atmo->SetUpwardTransmittances(upTrans);
  //
  //   myFilter->SetAtmosphericRadiativeTerms(atmo);
  //   myFilter->Update();
  //
  //   i = 0;
  //   IteratorType iterator2( myFilter->GetOutput(), myFilter->GetOutput()->GetRequestedRegion() );
  //   for ( iterator2.GoToBegin(); !iterator2.IsAtEnd(); ++iterator2 )
  //   {
  //     myResponse->GetResponse()[i]->second = iterator2.Get()[];
  //     ++i;
  //   }
  //
  //   std::cout << "Output SpectResponse " << myResponse << std::endl;
  return EXIT_SUCCESS;
}
