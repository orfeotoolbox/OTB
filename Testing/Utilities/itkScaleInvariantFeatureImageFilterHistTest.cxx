/*=========================================================================
Program: ITK nSIFT Implemention - Command Line Wrapper
Module: $RCSfile: testnD.cxx,v $
Language: C++
Date: $Date: 2007/11/25 15:51:48 $
Version: $Revision: 1.0 $
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

#define VERBOSE
//#define DEBUG
//#define DEBUG_VERBOSE

//#define SIFT_FEATURE
//#define REORIENT

#define GENERATE_KEYS 
//#define SUBSAMPLE
#define CROP
//#define DO_DOUBLE

#include "itkScaleInvariantFeatureImageFilter.h"
#include "itkImageSeriesReader.h"
#include "itkNumericSeriesFileNames.h"
#include "itkPointSetToImageFilter.h"
#include "itkImageFileWriter.h"
#include <getopt.h>

int itk2DScaleInvariantFeatureImageFilterHist( int argc, char *argv[] )
{
  // Command Line Arguments
  unsigned int ARG_IMG1=2;
  unsigned int ARG_IMG2=3;

  const unsigned int    Dimension = 2;
  
  // Default scale is 1.0
  double test_scale = 1.0;
  float test_rotate = 0.0;  // 0 degrees
  double test_crop = 0.5;
  //  float test_rotate = 0.0874;  // 5 degrees
  //float test_rotate = 0.1748;  // 10 degrees
  int series_start = 1;
  int series_end = 9;
  int series_inc = 1;

  int mode = 'i';  /* defaults to comparing 2 images */;
  int rotate_middle=0; /* defaults to rotating around the origin */
  
#define OPT_SCALE 'x'
#define OPT_ROTATE 'r'
#define OPT_DIM 'd'
#define OPT_CROP 'c'
#define OPT_SERIES_START 's'
#define OPT_SERIES_END 'e'
#define OPT_SERIES_INC 'i'

  /* new Getopt code */
  while(1) {
    static struct option long_options[] =
      {
	/* These options set a flag.   */
	{"synthetic", 0, &mode, 's'},
	{"image", 0, &mode, 'i'},
	{"rotate-mid", 0, &rotate_middle, 1},
	/* These options don't set a flag.
	   We distinguish them by their indices.  */
	{"scale", required_argument, 0, OPT_SCALE},
	{"rotate", required_argument, 0, OPT_ROTATE},
	{"crop", required_argument, 0, OPT_CROP},
	{"series-start", required_argument, 0, OPT_SERIES_START},
	{"series-end", required_argument, 0, OPT_SERIES_END},
	{"series-inc", required_argument, 0, OPT_SERIES_INC},
	//	{"dimension", required_argument, 0, OPT_DIM},
	{0, 0, 0, 0}
      };    

    int optindex;
    int val = getopt_long(argc, argv, "", long_options, &optindex);

    if (val == -1)
      break;

    switch(val) {
    case OPT_SCALE:
      test_scale = atof(optarg);
      break;
    case OPT_ROTATE:
      if (atof(optarg) >= 0.0 && atof(optarg) <= 360.0)
      test_rotate = atof(optarg) * PI * 2.0 / 360.0;
      break;
    case OPT_CROP:
      if (atof(optarg) <= 1.0) {
	test_crop = atof(optarg);
      }
      break;
    case OPT_SERIES_START:
      series_start = atoi(optarg);
      break;
    case OPT_SERIES_END:
      series_end = atoi(optarg);
      break;
    case OPT_SERIES_INC:
      series_inc = atoi(optarg);
      break;
      /*
    case OPT_DIM:
      Dimension = atoi(optarg);
      break;
      */
    }
  }

  ARG_IMG1 = optind;
  ARG_IMG2 = optind+1;
  
  typedef  float  PixelType;
  typedef itk::Image< PixelType, Dimension >  FixedImageType;
  typedef itk::ScaleInvariantFeatureImageFilter<FixedImageType, Dimension> SiftFilterType;
  typedef itk::Image<unsigned char, Dimension> OutputImageType;
  
  typedef itk::ImageSource< FixedImageType > ImageSourceType;

  ImageSourceType::Pointer fixedImageReader, fixedImageReader2;

  typedef itk::PointSetToImageFilter<SiftFilterType::PointSetType, OutputImageType> PointSetFilterType;
  
  if( static_cast<unsigned int>(argc) <= ARG_IMG1 || (mode == 'i' && static_cast<unsigned int>(argc) <= ARG_IMG2))
    {
      std::cerr << "Incorrect number of parameters " << std::endl;
      std::cerr << std::endl;
      std::cerr << "siftkeys program ( ";
      std::cerr << Dimension << "D ";      
#ifdef SIFT_FEATURE
      std::cerr << "sift-feature ";
#else
      std::cerr << "histogram-feature ";
#endif
#ifdef REORIENT
      std::cerr << "reoriented ";
#endif
      std::cerr << ")" << std::endl;

      std::cerr << "Usage: \n";
      std::cerr  << argv[0] << " [options] ImageFile [ImageFile2]\n"; 
      std::cerr << "This program takes an input image file(s) and generates scale invariant features." << std::endl;
      std::cerr << std::endl;
      std::cerr << "Image Processing Options (Choose ONE):" << std::endl;
      std::cerr << "--image" << std::endl;
      std::cerr << " compare ImageFile and ImageFile2" << std::endl;
      std::cerr << "\nOR\n" << std::endl;
      std::cerr << "--synthetic" << std::endl;
      std::cerr << " compare ImageFile to synthetically generated version" << std::endl;
      std::cerr << "Synthetic Image Options:" << std::endl;
      std::cerr << "--rotate value" << std::endl;
      std::cerr << "  rotate synthetic image on first axis by value degrees" << std::endl;
      std::cerr << "--rotate-middle" << std::endl;
      std::cerr << "  centre of rotation at the centre of image (defaults to origin)" << std::endl;
      std::cerr << "--scale value" << std::endl;
      std::cerr << "  scale all axes of synthetic image by value" << std::endl;
      std::cerr << "--tmp directory" << std::endl;
      std::cerr << "  temporary directory" << std::endl;
      std::cerr << std::endl;
      return 1;
    }

  std::cerr << "Dimension = " << Dimension << "\n";
  std::cerr << "Test Scale = " << test_scale << "\n";
  std::cerr << "Test Rotate = " << test_rotate << "\n";
  std::cerr << "Image Crop Ratio (first 3D) = " << test_crop << "\n";
  std::cerr << "Mode = " << (char) mode << "\n";
  std::cerr << "ImageFile1 = " << argv[optind] << "\n";
  if (Dimension == 4) {
    std::cerr << "Image Series Start = " << series_start << "\n";
    std::cerr << "Image Series End = " << series_end << "\n";
    std::cerr << "Image Series Inc = " << series_inc << "\n";
  }

#ifdef SIFT_FEATURE
  std::cerr << "SIFT Feature\n" << std::endl;
#else
  std::cerr << "Histogram Feature\n" << std::endl;
#endif

#ifdef REORIENT
  std::cerr << "Reorientation enabled\n" << std::endl;  
#endif

  if (Dimension == 4) {
    /* Assume fileseries reader */
    typedef itk::ImageSeriesReader< FixedImageType  > FixedImageReaderType;
    FixedImageReaderType::Pointer tmpImageReader  = FixedImageReaderType::New();

    typedef itk::NumericSeriesFileNames NameGeneratorType;
    NameGeneratorType::Pointer nameGenerator = NameGeneratorType::New();
    
    nameGenerator->SetSeriesFormat( argv[ARG_IMG1] );
    nameGenerator->SetStartIndex( series_start );
    nameGenerator->SetEndIndex( series_end );
    nameGenerator->SetIncrementIndex( series_inc );
        
    tmpImageReader->SetFileNames( nameGenerator->GetFileNames() );

    fixedImageReader = tmpImageReader;
  } else {
    typedef itk::ImageFileReader< FixedImageType  > FixedImageReaderType;
    FixedImageReaderType::Pointer tmpImageReader  = FixedImageReaderType::New();
    tmpImageReader  = FixedImageReaderType::New();

    tmpImageReader->SetFileName(  argv[ARG_IMG1] );
    fixedImageReader=tmpImageReader;
  }
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
  
#ifdef DEBUG
  siftFilter1.writeImage(fixedImage, "tmp0.png");
  std::cout << std::endl << "Starting SIFT Feature Extraction...\n";
#endif
  
  keypoints1 = siftFilter1.getSiftFeatures(fixedImage);
  
  pointSet1->SetInput(keypoints1);
  pointSet1->SetOutsideValue(0);
  pointSet1->SetInsideValue(255);
  pointSet1->SetSize(fixedImageReader->GetOutput()->GetLargestPossibleRegion().GetSize());
  
  typedef itk::ImageFileWriter<OutputImageType> WriterType;
  
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName("tmp0_keys.png");
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
      
#ifdef DEBUG
      siftFilter2.writeImage(scaledImage, "tmp1.png");
      std::cout << std::endl;
#endif
      keypoints2 = siftFilter2.getSiftFeatures(scaledImage);
    }
    
    std::cerr << "Test Image Scale: " << test_scale << std::endl;
    // std::cerr << "Test Translate: " << test_translate << std::endl;
    std::cerr << "Test Image Rotate: " << test_rotate << std::endl;    
  } else if (mode == 'i') {
    std::cerr << std::endl << "Image Comparison mode\n";  
    inv_test_transform = NULL;

    if (Dimension == 4) {
      typedef itk::ImageSeriesReader< FixedImageType  > FixedImageReaderType;
      FixedImageReaderType::Pointer tmpImageReader  = FixedImageReaderType::New();
      tmpImageReader  = FixedImageReaderType::New();

      typedef itk::NumericSeriesFileNames NameGeneratorType;
      NameGeneratorType::Pointer nameGenerator = NameGeneratorType::New();
      
      nameGenerator->SetSeriesFormat( argv[ARG_IMG2] );
      nameGenerator->SetStartIndex( series_start );
      nameGenerator->SetEndIndex( series_end );
      nameGenerator->SetIncrementIndex( series_inc );
      
      tmpImageReader->SetFileNames( nameGenerator->GetFileNames() );
      fixedImageReader2 = tmpImageReader;
    } else {      
      typedef itk::ImageFileReader< FixedImageType  > FixedImageReaderType;
      FixedImageReaderType::Pointer tmpImageReader  = FixedImageReaderType::New();
      tmpImageReader  = FixedImageReaderType::New();

      tmpImageReader->SetFileName(  argv[ARG_IMG2] );
      fixedImageReader2 = tmpImageReader;
    }
    fixedImageReader2->Update();

#ifdef CROP
    SiftFilterType::ResampleFilterType::Pointer cropper2 = SiftFilterType::ResampleFilterType::New();
    cropper2->SetInput(fixedImageReader2->GetOutput());
    FixedImageType::SizeType cropsize2 = 
      fixedImageReader2->GetOutput()->GetLargestPossibleRegion().GetSize();
    for (unsigned int k = 0; k < Dimension; ++k) {
      if (k < 4)
	cropsize2[k] /= 2;
    }
    cropper2->SetSize( cropsize2 );
    cropper2->SetOutputSpacing(fixedImageReader2->GetOutput()->GetSpacing());
    cropper2->SetTransform(no_transform);
    cropper2->Update();
    FixedImageType::Pointer fixedImage2 = cropper2->GetOutput();
#else
    FixedImageType::Pointer fixedImage2 = fixedImageReader2->GetOutput();  
#endif

    keypoints2 = siftFilter2.getSiftFeatures(fixedImage2);
  }
  
  std::cerr << std::endl << "Matching Keypoints\n";  
  siftFilter2.MatchKeypointsPos(keypoints1, keypoints2, inv_test_transform);
#ifdef GENERATE_KEYS
  siftFilter2.MatchKeypointsFeatures(keypoints1, keypoints2, inv_test_transform);
#endif
  return 0;

}
