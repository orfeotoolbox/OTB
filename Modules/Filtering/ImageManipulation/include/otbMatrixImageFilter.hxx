/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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
 */

#ifndef otbMatrixImageFilter_hxx
#define otbMatrixImageFilter_hxx

#include "otbMatrixImageFilter.h"
#include "itkImageRegionIterator.h"
#include "itkProgressReporter.h"

namespace otb
{

/**
 *
 */
template <class TInputImage, class TOutputImage, class TMatrix>
MatrixImageFilter<TInputImage, TOutputImage, TMatrix>::MatrixImageFilter() : m_MatrixByVector(false)
{
}

template <class TInputImage, class TOutputImage, class TMatrix>
void MatrixImageFilter<TInputImage, TOutputImage, TMatrix>::GenerateOutputInformation()
{
  // call the superclass' implementation of this method
  Superclass::GenerateOutputInformation();

  if ( m_MatrixByVector )
    {
    if( this->GetInput()->GetNumberOfComponentsPerPixel() != m_Matrix.cols() )
      {
        itkExceptionMacro("Invalid Matrix size. Number of columns must be the same as the image number of channels.");
      }

    if( m_Matrix.rows() == 0 )
      {
        itkExceptionMacro("Invalid Matrix size. Number of rows can't be null.");
      }
    this->GetOutput()->SetNumberOfComponentsPerPixel( m_Matrix.rows() );
    }

  if ( !m_MatrixByVector )
    {
    if( this->GetInput()->GetNumberOfComponentsPerPixel() != m_Matrix.rows() )
      {
        itkExceptionMacro("Invalid Matrix size. Number of rows must be the same as the image number of channels.");
      }

    if( m_Matrix.cols() == 0 )
      {
        itkExceptionMacro("Invalid Matrix size. Number of columns can't be null.");
      }
    this->GetOutput()->SetNumberOfComponentsPerPixel( m_Matrix.cols() );
    }
}

template<class TInputImage, class TOutputImage, class TMatrix>
void MatrixImageFilter<TInputImage, TOutputImage, TMatrix>::ThreadedGenerateData(
  const OutputImageRegionType&     outputRegionForThread,
  itk::ThreadIdType threadId
  )
{
  // images pointer
  typename OutputImageType::Pointer     outputPtr = this->GetOutput();
  typename InputImageType::ConstPointer inputPtr  = this->GetInput();

  typename itk::ImageRegionConstIterator<InputImageType> inIt( inputPtr, outputRegionForThread);
  itk::ImageRegionIterator<OutputImageType> outIt( outputPtr, outputRegionForThread);


  // support progress methods/callbacks
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  inIt.GoToBegin();
  outIt.GoToBegin();

  const unsigned int inSize =  m_MatrixByVector ? m_Matrix.cols() : m_Matrix.rows();
  const unsigned int outSize = m_MatrixByVector ? m_Matrix.rows() : m_Matrix.cols();

  VectorType inVect(inSize, InputRealType(0.));
  VectorType outVect(outSize, InputRealType(0.));

  while (!outIt.IsAtEnd())
    {
      const InputPixelType & inPix = inIt.Get();
      OutputPixelType outPix;
      outPix.SetSize(outSize);

      for(unsigned int i=0; i<inSize; ++i)
        {
          inVect[i] = static_cast<InputRealType>(inPix[i]);
        }

      outVect = m_MatrixByVector ? m_Matrix*inVect : inVect*m_Matrix;

      for(unsigned int i=0; i<outSize; ++i)
        {
          outPix[i] = static_cast<OutputInternalPixelType>(outVect[i]);
        }
      outIt.Set(outPix);

      ++inIt;
      ++outIt;
      progress.CompletedPixel();
    }
}


/**
 * Standard "PrintSelf" method
 */
template <class TInputImage, class TOutputImage, class TMatrix>
void
MatrixImageFilter<TInputImage, TOutputImage, TMatrix>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Matrix: " << m_Matrix << std::endl;
  os << indent << "MatrixByVector: " << m_MatrixByVector << std::endl;
}

} // end namespace otb

#endif
