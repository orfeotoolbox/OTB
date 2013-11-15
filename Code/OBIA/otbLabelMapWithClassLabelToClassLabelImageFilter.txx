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
#ifndef __otbLabelMapWithClassLabelToClassLabelImageFilter_txx
#define __otbLabelMapWithClassLabelToClassLabelImageFilter_txx

#include "otbLabelMapWithClassLabelToClassLabelImageFilter.h"
#include "itkNumericTraits.h"
#include "itkProgressReporter.h"
#include "itkImageRegionConstIteratorWithIndex.h"

namespace otb {

template <class TInputImage, class TOutputImage>
LabelMapWithClassLabelToClassLabelImageFilter<TInputImage, TOutputImage>
::LabelMapWithClassLabelToClassLabelImageFilter()
{
}


template<class TInputImage, class TOutputImage>
void
LabelMapWithClassLabelToClassLabelImageFilter<TInputImage, TOutputImage>
::BeforeThreadedGenerateData()
{
  OutputImageType * output = this->GetOutput();
  const InputImageType * input = this->GetInput();
  output->FillBuffer( input->GetBackgroundValue() );
  Superclass::BeforeThreadedGenerateData();
}


template<class TInputImage, class TOutputImage>
void
LabelMapWithClassLabelToClassLabelImageFilter<TInputImage, TOutputImage>
::ThreadedProcessLabelObject( LabelObjectType * labelObject )
{
  typename LabelObjectType::ClassLabelType label = itk::NumericTraits<typename LabelObjectType::ClassLabelType>::max();
  if(labelObject->HasClassLabel())
    {
     label = labelObject->GetClassLabel();
    }

  ConstLineIteratorType lit = ConstLineIteratorType (labelObject);
  
  while( !lit.IsAtEnd() )
    {
    IndexType idx = lit.GetLine().GetIndex();
    unsigned long length = lit.GetLine().GetLength();
    for( unsigned int i=0; i<length; ++i)
      {
      this->GetOutput()->SetPixel( idx, label );
      idx[0]++;
      }
    ++lit;
    }
}

}// end namespace otb
#endif
