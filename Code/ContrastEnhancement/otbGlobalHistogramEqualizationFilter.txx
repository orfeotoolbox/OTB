/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Some parts of this code are derived from ITK. See ITKCopyright.txt
  for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbGlobalHistogramEqualizationFilter_txx
#define __otbGlobalHistogramEqualizationFilter_txx

#include "otbMacro.h"
#include "itkProgressReporter.h"
#include "itkImageRegionConstIteratorWithIndex.h"

#include "otbGlobalHistogramEqualizationFilter.h"

#include "itkHistogram.h"
#include "itkNumericTraits.h"

namespace otb
{
template <class TInput, class TOutput>
void
GlobalHistogramEqualizationFilter<TInput,TOutput>
::BeforeThreadedGenerateData()
{
  unsigned long inputSize= this->GetInput()->GetLargestPossibleRegion().GetSize()[0] *
    this->GetInput()->GetLargestPossibleRegion().GetSize()[1];

  unsigned long histoSize = m_Histogram->Size();

  m_MinimumRange = ( histoSize==256 ) ? 0 : m_MinimumRange;
  double lutScale  =   static_cast<double>( inputSize ) /  static_cast<double>( histoSize );


  ArrayType CumulativeArray(histoSize);

  m_LookupArray.SetSize(histoSize);

  CumulativeArray.Fill(0);
  m_LookupArray.Fill(0);

  CumulativeArray[0] = static_cast<unsigned int>(m_Histogram->GetFrequency(0));
  for( unsigned int i = 1;  i < histoSize; i++ )
    {
    CumulativeArray[i] = m_Histogram->GetFrequency(i)+ CumulativeArray[i-1];
    }

  for (unsigned int i = 0; i< histoSize; i++)
    {
    m_LookupArray[i] = static_cast<InternalPixelType>(
      vnl_math_max( static_cast<double>(m_MinimumRange), -1.0 +
                    vcl_floor( static_cast<double>(CumulativeArray[i]) / lutScale + 0.5)));
      }


}

template <class TInput, class TOutput>
void
GlobalHistogramEqualizationFilter<TInput, TOutput>
::ThreadedGenerateData(const RegionType&
                                     outputRegion, int threadId)
{
    typename TInput::ConstPointer input = this->GetInput();
    typename TOutput::Pointer output = this->GetOutput();
//    output->SetNumberOfComponentsPerPixel(1);

    // Allocate the output
    this->AllocateOutputs();
    itk::ProgressReporter progress(this,threadId,outputRegion.GetNumberOfPixels());

    // Iterator which traverse the input
    itk::ImageRegionConstIteratorWithIndex<TInput> itInput(input,outputRegion);


      itInput.GoToBegin();
      while ( !itInput.IsAtEnd() )
        {
        PixelType outPixel;
//        int numComponents = output->GetNumberOfComponentsPerPixel();

//     outPixel.Reserve(numComponents);
//       outPixel.Reserve(1);
        //          for (int k =0; k < numComponents; k++)
        outPixel  = m_LookupArray[ static_cast<InternalPixelType>( itInput.Get() - m_MinimumRange ) ];

      output->SetPixel(itInput.GetIndex(), outPixel);
              ++itInput;
              progress.CompletedPixel();
        }



}
} // end namespace otb
#endif
