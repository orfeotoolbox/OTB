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

int otbHistogramStatisticFunction(int, char*[])
{
  typedef float MeasurementType ;
  typedef itk::Statistics::Histogram< MeasurementType, 1 > HistogramType ;
  HistogramType::Pointer histogram = HistogramType::New() ;

  // initialize histogram
  HistogramType::SizeType size;
  size.Fill(64) ;
  HistogramType::MeasurementVectorType lowerBound ;
  HistogramType::MeasurementVectorType upperBound ;
  lowerBound[0] = 0.0 ;
  upperBound[0] = 64.0 ;

  histogram->Initialize(size, lowerBound, upperBound ) ;
  

  // create histogram 
  for (HistogramType::Iterator iter = histogram->Begin(); iter != histogram->End(); ++iter)
    {
        iter.SetFrequency(1.0);
    }

  typedef otb::HistogramStatisticFunction<HistogramType,MeasurementType>  HistogramStatisticFunctionType;
  HistogramStatisticFunctionType::Pointer HistogramStatisticFunction = HistogramStatisticFunctionType::New();

  HistogramStatisticFunction->SetInputHistogram(histogram.GetPointer());
  
  
  MeasurementType Entropy;
  Entropy = HistogramStatisticFunction->GetEntropy();
  std::cout << "Entropy : " << Entropy << std::endl;
 
  if(fabs(Entropy)>0.0 ) 
  {
  	return EXIT_FAILURE;
  }
  
  return EXIT_SUCCESS;
}
