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

#include "itkListSample.h"
#include "itkVariableLengthVector.h"
#include "otbCenteredReducedSampleListFilter.h"

typedef itk::VariableLengthVector<double> DoubleSampleType;
typedef itk::Statistics::ListSample<DoubleSampleType> DoubleSampleListType;

typedef itk::VariableLengthVector<float> FloatSampleType;
typedef itk::Statistics::ListSample<FloatSampleType> FloatSampleListType;
typedef otb::Statistics::CenteredReducedSampleListFilter<FloatSampleListType,DoubleSampleListType> CenteredReducedFilterType;


int otbCenteredReducedSampleListFilterNew(int argc, char * argv[])
{
	CenteredReducedFilterType::Pointer instance = CenteredReducedFilterType::New();

	return EXIT_SUCCESS;
}

int otbCenteredReducedSampleListFilter(int argc, char * argv[])
{
	FloatSampleListType::Pointer inputSampleList = FloatSampleListType::New();
	inputSampleList->SetMeasurementVectorSize(2);
	FloatSampleListType sample(2);
	sample[0]=-1;
	sample[1]=-5;
	inputSampleList->PushBack(sample);
	sample[0]=-1;
	sample[1]=-2;
	inputSampleList->PushBack(sample);
	sample[0]=3;
	sample[1]=1;
	inputSampleList->PushBack(sample);

	CenteredReducedFilterType::Pointer filter = CenteredReducedFilterType::New();
	sample[0]=1;
	sample[1]=-3;
	filter->SetMeans(sample);


	return EXIT_SUCCESS;
}


