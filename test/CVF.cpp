

/* * Copyright (C) Damia Belazouz - 2017
 *
 *  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
 * See OTBCopyright.txt for details.
 *
 *
 *     This software is distributed WITHOUT ANY WARRANTY; without even
 *     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 *    PURPOSE.  See the above copyright notices for more information.
 *
 *
 * Copyright (C) CS SI
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
 
=========================================================================*/


#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <iostream>


#include "otbVectorImage.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

#include "otbLocalGradientVectorImageFilter.h"
#include "otbCostVolumeImageFilter.h"
#include "otbMinimumNBandsImageFilter.h"
#include "otbWeightsGuidedFilter.h"
#include "otbMeanVectorImageFilter.h"




	
int testCVF(int argc, char *argv[])
{
	if(argc < 8) {
		std::cerr << "Usage: " << argv[0] << " leftImage rightImage minDisp maxDisp radius radiusToMeanFilter outputPathFolder" << std::endl;
		return EXIT_FAILURE;
	}

	typedef otb::VectorImage<float> FloatVectorImageType;
	typedef otb::ImageFileReader<FloatVectorImageType> ReaderType;
	typedef otb::ImageFileWriter<FloatVectorImageType> ImageWriterType;


    ReaderType::Pointer inLeft = ReaderType::New();
    inLeft->SetFileName(argv[1]); //LeftImage  
    inLeft->UpdateOutputInformation();

    ReaderType::Pointer inRight = ReaderType::New();
    inRight->SetFileName(argv[2]);//RightImage
    inRight->UpdateOutputInformation();

    int dispMin = atoi(argv[3]);
	int dispMax  = atoi(argv[4]);
    long unsigned int r = atoi(argv[5]);	
    int rmf = atoi(argv[6]) ;

    std::string argv7 = std::string(argv[7]);
    #define FILENAME(n) std::string( argv7 + std::string(n)).c_str()

    // CALCUL GRADIENT
    typedef otb::LocalGradientVectorImageFilter<FloatVectorImageType, FloatVectorImageType> GradientType;
    GradientType::Pointer gradX = GradientType::New();
    GradientType::Pointer gradY = GradientType::New();
    gradX->SetInput(inLeft->GetOutput());
    gradY->SetInput(inRight->GetOutput());




    ImageWriterType::Pointer writer_gradX = ImageWriterType::New();
    writer_gradX->SetFileName( FILENAME("GradientLeft.tif"));
    writer_gradX->SetInput(gradX->GetOutput());
  	writer_gradX->Update();
    ImageWriterType::Pointer writer_gradY = ImageWriterType::New();
    writer_gradY->SetFileName( FILENAME("GradientRight.tif"));
    writer_gradY->SetInput(gradY->GetOutput());
  	writer_gradY->Update();


    // COST VOLUME

    typedef otb::CostVolumeImageFilter< FloatVectorImageType, FloatVectorImageType, FloatVectorImageType > CostVolumeType;
    CostVolumeType::Pointer m_LeftCost = CostVolumeType::New();
    m_LeftCost->SetLeftInputImage(inLeft->GetOutput() );
    m_LeftCost->SetRightInputImage(inRight->GetOutput() );  
    m_LeftCost->SetLeftGradientXInput(gradX->GetOutput() ); 
    m_LeftCost->SetRightGradientXInput(gradY->GetOutput() );      
    m_LeftCost->SetDisp(dispMax);
    m_LeftCost->Update();
       

/*
    ImageWriterType::Pointer writer_LeftCost = ImageWriterType::New();
    writer_LeftCost->SetFileName( FILENAME("LeftCost.tif"));
    writer_LeftCost->SetInput(m_LeftCost->GetOutput());
    writer_LeftCost->Update();
*/

    typedef otb::MinimumNBandsImageFilter< FloatVectorImageType, FloatVectorImageType > MinCostVolume;  
    MinCostVolume::Pointer m_minCost = MinCostVolume::New();
    m_minCost->SetInput(m_LeftCost->GetOutput());
/*
    ImageWriterType::Pointer writer_m_minCost = ImageWriterType::New();
    writer_m_minCost->SetFileName( FILENAME("MinLeftCost.tif"));
    writer_m_minCost->SetInput(m_minCost->GetOutput());
    writer_m_minCost->Update();
*/


  typedef otb::WeightsGuidedFilter< FloatVectorImageType, FloatVectorImageType, FloatVectorImageType > Weights_ak_bk;
  Weights_ak_bk::Pointer m_meanLeftCost = Weights_ak_bk::New();
 
  m_meanLeftCost->SetInput1(inLeft->GetOutput());
  m_meanLeftCost->SetInput2(m_LeftCost->GetOutput());  
  m_meanLeftCost->SetRadius(0,9);


  ImageWriterType::Pointer writer_meanLeftCost = ImageWriterType::New();
  writer_meanLeftCost->SetFileName( FILENAME("Weights.tif"));
  writer_meanLeftCost->SetInput(m_meanLeftCost->GetOutput());
  writer_meanLeftCost->Update();





  typedef otb::MeanVectorImageFilter< FloatVectorImageType, FloatVectorImageType, FloatVectorImageType > MeanVectorImage;
  MeanVectorImage::Pointer m_meanWeights = MeanVectorImage::New();
  m_meanWeights->SetInput1(m_meanLeftCost->GetOutput());
  m_meanWeights->SetInput2(inLeft->GetOutput());
  m_meanWeights->SetRadius(0,9);


  ImageWriterType::Pointer writer_meanWeights = ImageWriterType::New();
  writer_meanWeights->SetFileName( FILENAME("MeanWeights.tif"));
  writer_meanWeights->SetInput(m_meanWeights->GetOutput());
  writer_meanWeights->Update();  


  MinCostVolume::Pointer m_minCostWeights = MinCostVolume::New();
  m_minCostWeights->SetInput(m_meanWeights->GetOutput());

  ImageWriterType::Pointer writerminCostWeights = ImageWriterType::New();
  writerminCostWeights->SetFileName( FILENAME("MinCostWeights.tif"));
  writerminCostWeights->SetInput(m_minCostWeights->GetOutput());
  writerminCostWeights->Update(); 



/*
  typedef otb::Image< float >                FloatImageType;
  typedef otb::ImageFileReader<FloatImageType> ReaderType2;
  typedef otb::ImageFileWriter<FloatImageType> ImageWriterType2;

  typedef itk::MeanImageFilter< FloatImageType, FloatImageType > MeanImage ;

    ReaderType2::Pointer im1b1 = ReaderType2::New();
    im1b1->SetFileName("/home/julie/Documents/c++/data/im1b1.tif");  
    im1b1->UpdateOutputInformation();

    MeanImage::Pointer m_meanImage = MeanImage::New();
    m_meanImage->SetInput(im1b1->GetOutput());
    m_meanImage->SetRadius(9);

    ImageWriterType2::Pointer writer_meanImage = ImageWriterType2::New();
    writer_meanImage->SetFileName( FILENAME("m_meanImage.tif"));
    writer_meanImage->SetInput(m_meanImage->GetOutput());
    writer_meanImage->Update();

*/






  return EXIT_SUCCESS;

}
