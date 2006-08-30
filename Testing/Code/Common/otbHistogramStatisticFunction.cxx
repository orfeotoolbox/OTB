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

#define MAIN


#include "itkHistogram.h"
#include "otbHistogramStatisticFunction.h"

int otbHistogramStatisticFunction(int argc, char* argv[])
{
  unsigned int  NbOfBins((unsigned int)::atoi(argv[1]));

  typedef float MeasurementType ;
  typedef itk::Statistics::Histogram< MeasurementType, 1 > HistogramType ;
  HistogramType::Pointer histogram = HistogramType::New() ;

  // initialize histogram
  HistogramType::SizeType size;
  size.Fill(NbOfBins) ;
  HistogramType::MeasurementVectorType lowerBound ;
  HistogramType::MeasurementVectorType upperBound ;
  lowerBound[0] = 0.0 ;
  upperBound[0] = NbOfBins ;

  histogram->Initialize(size, lowerBound, upperBound ) ;
  

  // create histogram with same value for each frequency
  for (HistogramType::Iterator iter = histogram->Begin(); iter != histogram->End(); ++iter)
    {
        iter.SetFrequency(1.0);
    }
  
  typedef otb::HistogramStatisticFunction<HistogramType,MeasurementType>  HistogramStatisticFunctionType;
  HistogramStatisticFunctionType::Pointer HistogramStatisticFunction = HistogramStatisticFunctionType::New();

  HistogramStatisticFunction->SetInputHistogram(histogram.GetPointer());
  
  
  MeasurementType Entropy;
  Entropy = HistogramStatisticFunction->GetEntropy();
  std::cout << "Entropy 1 : " << Entropy << std::endl;
 
  if(fabs(Entropy-log(NbOfBins))>0.00001 ) 
  {
  	std::cout << "Error in entropy estimation" << std::endl;
	return EXIT_FAILURE;
  }
 
  // create histogram just all value equal to zero except the first one
  for (HistogramType::Iterator iter = histogram->Begin(); iter != histogram->End(); ++iter)
    {
        if(iter == histogram->Begin())
	{
		iter.SetFrequency(1.0);
	}
	else
	{
		iter.SetFrequency(0.0);	
	}
    }
 
  HistogramStatisticFunction->Update();
  Entropy = HistogramStatisticFunction->GetEntropy();
  std::cout << "Entropy 2 : " << Entropy << std::endl;
  if( Entropy!=0.0 ) 
  {
  	std::cout << "Error in entropy estimation" << std::endl;
	return EXIT_FAILURE;
  }
  
  return EXIT_SUCCESS;
}
