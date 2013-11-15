/*=========================================================================

  Program:   ITK nSIFT Implemention - Command Line Wrapper
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005,2006,2007 Warren Cheung

All rights reserved.
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
* Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.
* The name of the Insight Consortium, nor the names of any consortium members,
nor of any contributors, may be used to endorse or promote products derived
from this software without specific prior written permission.
* Modified source versions must be plainly marked as such, and must not be
misrepresented as being the original software.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
=========================================================================*/

#define GENERATE_KEYS
#define VERBOSE
#define CROP

#include "itkScaleInvariantFeatureImageFilter.h"
#include "itkNumericSeriesFileNames.h"
#include "itkPointSetToImageFilter.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbMath.h"

int itk2DScaleInvariantFeatureImageFilterTest( int argc, char *argv[])
{
  const unsigned int Dimension = 2;

  const char* inputImage1 = argv[1];
  const char* outputImage1 = argv[2];
  const char* outputImage2 = argv[3];
  const char* outputImageKeys1 = argv[4];
  const char* outputImageKeys2 = argv[5];

  // Default parametres
  // scale is 1.0
  // rotate is 0 degrees
  // crop is 0.5
  // rotate_middle is 0 (rotating around the origin
  // mode is s (synthetic)

  double test_scale = atof(argv[6]);
  float test_rotate = static_cast<float>(atof(argv[7])* otb::CONST_PI_180);
  double test_crop = atof(argv[8]);
  int rotate_middle= atoi(argv[9]);
  int mode = 's';

  typedef float PixelType;
  typedef otb::Image<PixelType, Dimension> FixedImageType;
  typedef itk::ScaleInvariantFeatureImageFilter<FixedImageType, Dimension> SiftFilterType;
  typedef otb::Image<unsigned char, Dimension> OutputImageType;

  typedef itk::ImageSource<FixedImageType> ImageSourceType;

  ImageSourceType::Pointer fixedImageReader, fixedImageReader2;

  typedef itk::PointSetToImageFilter<SiftFilterType::PointSetType, OutputImageType> PointSetFilterType;

  std::cerr << "Dimension = " << Dimension << "\n";
  std::cerr << "Test Scale = " << test_scale << "\n";
  std::cerr << "Test Rotate = " << test_rotate << "\n";
  std::cerr << "Image Crop Ratio (first 3D) = " << test_crop << "\n";
  std::cerr << "Mode = " << (char) mode << "\n";
  std::cerr << "ImageFile1 = " << inputImage1 << "\n";
  std::cerr << "SIFT Feature\n" << std::endl;

  typedef otb::ImageFileReader< FixedImageType  > FixedImageReaderType;
  FixedImageReaderType::Pointer tmpImageReader  = FixedImageReaderType::New();
  tmpImageReader  = FixedImageReaderType::New();

  tmpImageReader->SetFileName(  inputImage1 );
  fixedImageReader=tmpImageReader;
  fixedImageReader->Update();

  typedef itk::ScalableAffineTransform< double, Dimension > ScaleType;
#ifdef CROP
  ScaleType::Pointer no_transform = ScaleType::New();
  no_transform->SetIdentity();

  SiftFilterType::ResampleFilterType::Pointer cropper = SiftFilterType::ResampleFilterType::New();
  cropper->SetInput(fixedImageReader->GetOutput());
  FixedImageType::SizeType cropsize =
    fixedImageReader->GetOutput()->GetLargestPossibleRegion().GetSize();
  for (unsigned int k = 0; k < Dimension; ++k) {
    if (k < 4)
      cropsize[k] = (int) (cropsize[k] * test_crop);
  }
  cropper->SetSize( cropsize );
  cropper->SetOutputSpacing(fixedImageReader->GetOutput()->GetSpacing());
  cropper->SetTransform(no_transform);
  cropper->Update();
  FixedImageType::Pointer fixedImage = cropper->GetOutput();
#else
  FixedImageType::Pointer fixedImage = fixedImageReader->GetOutput();
#endif

  SiftFilterType::PointSetTypePointer keypoints1, keypoints2;
  PointSetFilterType::Pointer pointSet1 = PointSetFilterType::New();
  PointSetFilterType::Pointer pointSet2 = PointSetFilterType::New();

  SiftFilterType siftFilter1, siftFilter2;

  siftFilter1.writeImage(fixedImage, outputImage1);
  std::cout << std::endl << "Starting SIFT Feature Extraction...\n";
  keypoints1 = siftFilter1.getSiftFeatures(fixedImage);

  pointSet1->SetInput(keypoints1);
  pointSet1->SetOutsideValue(0);
  pointSet1->SetInsideValue(255);
  pointSet1->SetSize(fixedImage->GetLargestPossibleRegion().GetSize());
  pointSet1->Update();

  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputImageKeys1);
  writer->SetInput(pointSet1->GetOutput());
  writer->Update();

  typedef itk::ScalableAffineTransform< double, Dimension > TestTransformType;
  TestTransformType::Pointer inv_test_transform;

  // Synthetic test image
  if (mode=='s') {
    std::cerr << std::endl << "Synthetic image mode\n";
    TestTransformType::Pointer test_transform = TestTransformType::New();

    inv_test_transform = TestTransformType::New();

    TestTransformType::InputPointType translate_vector;

    test_transform->SetIdentity();
    test_transform->Scale( 1.0 / test_scale);
    if (rotate_middle) {
      std::cerr << "Rotation centred at middle of image." << std::endl;
      /* Cycle through each dimension and shift by half*/
      FixedImageType::SizeType size =
	fixedImage->GetLargestPossibleRegion().GetSize();
      for (unsigned int k = 0; k < Dimension; ++k)
	translate_vector[k] = size[k]/2.0;
      test_transform->SetCenter( translate_vector);
    } else {
      std::cerr << "Rotation centred at origin." << std::endl;
    }
#if 0
    if (rotate_middle) {
      std::cerr << "Rotation centred at middle of image." << std::endl;
      /* Cycle through each dimension and shift by half*/
      FixedImageType::SizeType size =
	fixedImage->GetLargestPossibleRegion().GetSize();
      for (unsigned int k = 0; k < Dimension; ++k)
	translate_vector[k] = size[k]/2.0;
      test_transform->Translate( translate_vector);
    } else {
      std::cerr << "Rotation centred at origin." << std::endl;
    }
#endif

    test_transform->Rotate(0,1,test_rotate);
#if 0
    if (rotate_middle) {
      /* Cycle through each dimension and shift back*/
      for (unsigned int k = 0; k < Dimension; ++k)
	translate_vector[k] = -translate_vector[k];
      test_transform->Translate( translate_vector);
    }
#endif

    test_transform->GetInverse(inv_test_transform);

    {
      FixedImageType::Pointer scaledImage;

      SiftFilterType::ResampleFilterType::Pointer scaler = SiftFilterType::ResampleFilterType::New();
      scaler->SetInput(fixedImage);
      FixedImageType::SizeType size =
	fixedImage->GetLargestPossibleRegion().GetSize();
      for (unsigned int k = 0; k < Dimension; ++k)
	size[k] = (unsigned int) floor(size[k] * test_scale);
      scaler->SetSize( size );

      scaler->SetOutputSpacing(fixedImage->GetSpacing());
      scaler->SetTransform(test_transform);
      scaler->Update();
      scaledImage = scaler->GetOutput();

      siftFilter2.writeImage(scaledImage, outputImage2);
      std::cout << std::endl;

      keypoints2 = siftFilter2.getSiftFeatures(scaledImage);

      pointSet2->SetInput(keypoints2);
      pointSet2->SetOutsideValue(0);
      pointSet2->SetInsideValue(255);
      pointSet2->SetSize(scaledImage->GetLargestPossibleRegion().GetSize());
      pointSet2->Update();

      WriterType::Pointer writer2 = WriterType::New();
      writer2->SetFileName(outputImageKeys2);
      writer2->SetInput(pointSet2->GetOutput());
      writer2->Update();
    }

    std::cerr << "Test Image Scale: " << test_scale << std::endl;
    std::cerr << "Test Image Rotate: " << test_rotate << std::endl;
    std::cerr << std::endl << "Matching Keypoints\n";

    siftFilter2.MatchKeypointsPos(keypoints1, keypoints2, inv_test_transform);
    siftFilter2.MatchKeypointsFeatures(keypoints1, keypoints2, inv_test_transform);
  }
  return 0;
}
