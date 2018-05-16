

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
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

#include "otbLocalGradientVectorImageFilter.h"




	
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








 
	return EXIT_SUCCESS;

}
