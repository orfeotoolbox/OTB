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

#include "otbImage.h"
#include "otbImageToNoDataMaskFilter.h"
#include "itkImageRegionIterator.h"

int otbImageToNoDataMaskFilter(int itkNotUsed(argc),char * itkNotUsed(argv) [])
{

  // Build an image
  typedef otb::Image<double> ImageType;
  ImageType::Pointer img = ImageType::New();

  ImageType::SizeType size;
  size.Fill(20);

  ImageType::RegionType region;
  region.SetSize(size);

  // Fill it with a default value
  img->SetRegions(region);
  img->Allocate();
  img->FillBuffer(10);

  // Write no-data flags to it
  std::vector<bool> flags(1,true);
  std::vector<double> values(1,-10.);
  otb::WriteNoDataFlags(flags,values,img->GetMetaDataDictionary());

  // Fill half of the pixels with no-data values
  itk::ImageRegionIterator<ImageType> it(img,region);
  unsigned int count = 0;
  for(it.GoToBegin();!it.IsAtEnd();++it,++count)
    {
    if (count%2 == 0)
      it.Set(-10.);
    }

  // Instanciate filter
  typedef otb::ImageToNoDataMaskFilter<ImageType,ImageType> FilterType;
  FilterType::Pointer filter = FilterType::New();

  filter->SetInput(img);
  filter->SetInsideValue(255);
  filter->SetOutsideValue(0);
  filter->Update();

  // Check output
  it = itk::ImageRegionIterator<ImageType>(filter->GetOutput(),region);
  count = 0;

  bool failed = false;
  
  for(it.GoToBegin();!it.IsAtEnd();++it,++count)
    {
    if (count%2 == 0 && it.Get()!=0)
      {
      std::cerr<<"Pixel should be masked"<<std::endl;
      failed = true;
      }
    else if(count%2 == 1 && it.Get()!=255)
      {
      std::cerr<<"Pixel should not be masked"<<std::endl;
      failed = true;
      }
    }
  
  if(failed)
    return EXIT_FAILURE;

  return EXIT_SUCCESS;
}
