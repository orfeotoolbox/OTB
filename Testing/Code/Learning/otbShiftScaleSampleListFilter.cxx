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
#include "otbShiftScaleSampleListFilter.h"

typedef itk::VariableLengthVector<double> DoubleSampleType;
typedef itk::Statistics::ListSample<DoubleSampleType> DoubleSampleListType;

typedef itk::VariableLengthVector<float> FloatSampleType;
typedef itk::Statistics::ListSample<FloatSampleType> FloatSampleListType;
typedef otb::Statistics::ShiftScaleSampleListFilter<FloatSampleListType,DoubleSampleListType> ShiftScaleFilterType;


int otbShiftScaleSampleListFilterNew(int argc, char * argv[])
{
	ShiftScaleFilterType::Pointer instance = ShiftScaleFilterType::New();

	return EXIT_SUCCESS;
}

int otbShiftScaleSampleListFilter(int argc, char * argv[])
{
	FloatSampleListType::Pointer inputSampleList = FloatSampleListType::New();
	inputSampleList->SetMeasurementVectorSize(2);
	FloatSampleType sample(2);
	sample[0]=-1;
	sample[1]=-5;
	inputSampleList->PushBack(sample);
	sample[0]=-1;
	sample[1]=-2;
	inputSampleList->PushBack(sample);
	sample[0]=3;
	sample[1]=1;
	inputSampleList->PushBack(sample);
	ShiftScaleFilterType::Pointer filter = ShiftScaleFilterType::New();
	sample[0]=1;
	sample[1]=-3;
	filter->SetShifts(sample);
	sample[0]=2;
	sample[1]=3;
	filter->SetScales(sample);
	filter->Update();

	return EXIT_SUCCESS;
}


