/*
 * Copyright (C) 2005-2018 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbFastNLMeansImageFilter_hxx
#define otbFastNLMeansImageFilter_hxx

#include "otbFastNLMeansImageFilter.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkNumericTraits.h"
#include <vector>
#include <tuple>

namespace otb
{
  template<class TInputImage, class TOutputImage>
  NLMeansFilter<TInputImage, TOutputImage>::NLMeansFilter()
  {
    // Define default attributes values
    m_HalfSearchSize.Fill(7);
    m_HalfPatchSize.Fill(2);
    m_Var = 0.;
    m_CutoffDistance = 0.1;

    m_NormalizeDistance = m_CutoffDistance * m_CutoffDistance 
      * (2*m_HalfPatchSize[m_ROW]+1) * (2*m_HalfPatchSize[m_COL]+1);
  }

  template<class TInputImage, class TOutputImage> 
  std::tuple< typename NLMeansFilter<TInputImage, TOutputImage>::InRegionType,
	      int, int, int, int, bool> 
  NLMeansFilter<TInputImage, TOutputImage>::OutputRegionToInputRegion
  (const OutRegionType& outputRegion) const
  {
    InImageConstPointerType inputPtr = this->GetInput();
    auto const& inputSize = inputPtr->GetLargestPossibleRegion().GetSize();

    // Get output region specification
    auto const& outIndex = outputRegion.GetIndex();
    auto const& outSize = outputRegion.GetSize();

    // Define margin for processing
    const InSizeType halfMargin = m_HalfSearchSize + m_HalfPatchSize;
    const InSizeType sizeTwo = {{2,2}};
    const InSizeType fullMargin = sizeTwo*halfMargin;

    // Define region to read
    InIndexType inIndex = outIndex - halfMargin;
    InSizeType requestedSize = outSize + fullMargin;

    // Initialize parameters for mirror padding
    bool needMirrorPadding = false;
    int mirrorFirstRow = 0;
    int mirrorFirstCol = 0;
    int mirrorLastRow = 0;
    int mirrorLastCol = 0;
    // Check that the requested region is inside image boundaries
    // If not, store number of missing data and update region
    if (inIndex[m_COL] < 0){
      needMirrorPadding = true;
      mirrorFirstCol = -inIndex[m_COL];
      inIndex[m_COL] = 0;
      requestedSize[m_COL] -= mirrorFirstCol;
    }
    if (inIndex[m_ROW] < 0){
      needMirrorPadding = true;
      mirrorFirstRow = -inIndex[m_ROW];
      inIndex[m_ROW] = 0;
      requestedSize[m_ROW] -= mirrorFirstRow;
    }
    unsigned int lastCol = inIndex[m_COL] + requestedSize[m_COL];
    if (lastCol >= inputSize[m_COL]){
      needMirrorPadding = true;
      mirrorLastCol = lastCol - inputSize[m_COL];
      requestedSize[m_COL] -= mirrorLastCol;
    }
    unsigned int lastRow = inIndex[m_ROW] + requestedSize[m_ROW];
    if (lastRow >= inputSize[m_ROW]){
      needMirrorPadding = true;
      mirrorLastRow = lastRow - inputSize[m_ROW];
      requestedSize[m_ROW] -= mirrorLastRow;
    }

    InRegionType inRequestedRegion(inIndex, requestedSize);
    return std::make_tuple(inRequestedRegion, mirrorFirstRow, mirrorFirstCol, 
			   mirrorLastRow, mirrorLastCol, needMirrorPadding);
  }

  template <class TInputImage, class TOutputImage>
  void NLMeansFilter<TInputImage, TOutputImage>
  ::GenerateInputRequestedRegion()
  {
    // Call the superclass' implementation of this method
    Superclass::GenerateInputRequestedRegion();

    auto const& outputRequestedRegion = this->GetOutput()->GetRequestedRegion();
    auto regionAndMirror = this->OutputRegionToInputRegion(outputRequestedRegion);
    InRegionType inRequestedRegion = std::get<0>(regionAndMirror);

    InImageType * inputPtr = const_cast<InImageType * >(this->GetInput());
    inputPtr->SetRequestedRegion(inRequestedRegion);
  }

  template<class TInputImage, class TOutputImage>
  void 
  NLMeansFilter<TInputImage, TOutputImage>::ThreadedGenerateData
  (const OutRegionType& outputRegionForThread, 
   itk::ThreadIdType itkNotUsed(threadId))
  {
    InImageConstPointerType inputPtr = this->GetInput();
    auto regionAndMirror = OutputRegionToInputRegion(outputRegionForThread);
    // Unpack all values returned
    InRegionType inputRegionForThread = std::get<0>(regionAndMirror);
    int mirrorFirstRow = std::get<1>(regionAndMirror);
    int mirrorFirstCol = std::get<2>(regionAndMirror);
    int mirrorLastRow = std::get<3>(regionAndMirror);
    int mirrorLastCol = std::get<4>(regionAndMirror);
    bool needMirror = std::get<5>(regionAndMirror);

    // initialize and allocate vector to store temporary output values 
    // It makes it easier to store them in vectors to access various non-contiguous locations
    auto const& outSize = outputRegionForThread.GetSize();
    std::vector<double> outTemp(outSize[m_ROW]*outSize[m_COL]);
    // initialize and allocate buffer to store all weights
    std::vector<double> weights(outSize[m_ROW]*outSize[m_COL]);

    typedef itk::ImageRegionConstIterator<InImageType> InIteratorType;
    InIteratorType inIt(inputPtr, inputRegionForThread);
    auto const& inputSize = inputRegionForThread.GetSize();

    auto mirrorCol = inputSize[m_COL] + mirrorFirstCol + mirrorLastCol;
    auto mirrorRow = inputSize[m_ROW] + mirrorFirstRow + mirrorLastRow;
    InSizeType const& mirrorSize = {{mirrorCol, mirrorRow}};

    std::vector<double> dataInput(mirrorSize[m_ROW]*mirrorSize[m_COL]);
    inIt.GoToBegin();
    for (unsigned int row=static_cast<unsigned int>(mirrorFirstRow); 
	 row<static_cast<unsigned int>(mirrorFirstRow)+inputSize[m_ROW]; row++)
      for (unsigned int col=static_cast<unsigned int>(mirrorFirstCol); 
	   col<static_cast<unsigned int>(mirrorFirstCol)+inputSize[m_COL]; col++)
	{
	  auto index = row * mirrorSize[m_COL] + col;
	  dataInput[index] = static_cast<double>(inIt.Get());
	  ++inIt;
	}

    if (needMirror)
      {
	// Perform mirror on upper lines
	for (int row=0; row<mirrorFirstRow; row++)
	  {
	    int lineToCopy = (2*mirrorFirstRow - row)*mirrorSize[m_COL];
	    std::copy(dataInput.begin() + lineToCopy,
		      dataInput.begin() + lineToCopy + mirrorSize[m_COL],
		      dataInput.begin() + row*mirrorSize[m_COL] );
	  }
	// Perform mirror on lower lines
	int lastRowRead = mirrorFirstRow+inputSize[m_ROW];
	for (int row=0; row<mirrorLastRow; row++)
	  {
	    int lineToCopy = (lastRowRead - row -2)*mirrorSize[m_COL];
	    std::copy(dataInput.begin() + lineToCopy,
		      dataInput.begin() + lineToCopy + mirrorSize[m_COL],
		      dataInput.begin() + (lastRowRead + row)*mirrorSize[m_COL]);
	  }
	// Perform mirror on left-hand columns
	if (mirrorFirstCol > 0) {
	  for (unsigned int row=0; row<mirrorSize[m_ROW]; row++)
	    {
	      std::reverse_copy(dataInput.begin() + row*mirrorSize[m_COL] + mirrorFirstCol+1,
				dataInput.begin() + row*mirrorSize[m_COL] +2*mirrorFirstCol+1,
				dataInput.begin() + row*mirrorSize[m_COL]);
	    }
		
	}
	// Perform mirror on right-hand columns
	if (mirrorLastCol > 0){
	  for (unsigned int row=0; row<mirrorSize[m_ROW]; row++)
	    {
	      std::reverse_copy(dataInput.begin() + (row+1)*mirrorSize[m_COL] - 2*mirrorLastCol-1,
				dataInput.begin() + (row+1)*mirrorSize[m_COL] - mirrorLastCol-1,
				dataInput.begin() + (row+1)*mirrorSize[m_COL] - mirrorLastCol);
	    }
	}
      }

    // For loops on all shifts possible
    int fullMarginRow = static_cast<int>(m_HalfSearchSize[m_ROW]+m_HalfPatchSize[m_ROW]);
    int fullMarginCol = static_cast<int>(m_HalfSearchSize[m_COL]+m_HalfPatchSize[m_COL]);
    int searchSizeRow = static_cast<int>(m_HalfSearchSize[m_ROW]);
    int searchSizeCol = static_cast<int>(m_HalfSearchSize[m_COL]);
    // Allocate integral image
    const InSizeType sizeTwo = {{2,2}};
    auto const& inSize = outSize + sizeTwo * m_HalfPatchSize;
    
    std::vector<double> imIntegral(inSize[m_ROW]*inSize[m_COL]);
    for (int drow=-searchSizeRow; drow < searchSizeRow+1; drow++)
      for (int dcol=-searchSizeCol; dcol < searchSizeCol+1; dcol++)
	{
	  // Compute integral image for current shift (drow, dcol)
	  OutIndexType shift = {{dcol, drow}};
	  ComputeIntegralImage(dataInput, imIntegral, shift, inSize, mirrorSize);

	  for(unsigned int row=0; row<outSize[m_ROW]; row++)
	    for (unsigned int col=0; col<outSize[m_COL]; col++)
	      {
		// Compute distance from integral image for patch centered at 
		// (row, col) + (m_HalfPatchSize, m_HalfPatchSize)
		OutPixelType distance = ComputeDistance(row, col, imIntegral, inSize[m_COL]);
		if (distance < 5.0)
		  {
		    double weight = exp(static_cast<double>(-distance));
		    outTemp[row*outSize[m_COL] + col] += weight*dataInput[(row+drow+fullMarginRow)*mirrorSize[m_COL] 
									  + col+dcol+fullMarginCol];
		    weights[row*outSize[m_COL] + col] += weight;
		  }
	      }
	}

    // Normalize all results by dividing output by weights (store in output)
    typedef itk::ImageRegionIterator<OutImageType> OutputIteratorType;
    OutImagePointerType outputPtr = this->GetOutput();
    OutputIteratorType outIt(outputPtr, outputRegionForThread);
    outIt.GoToBegin();
    for(unsigned int index=0; index<outSize[m_ROW]*outSize[m_COL]; index++)
      {
	outIt.Set(static_cast<OutPixelType>(outTemp[index]/weights[index]));
	++outIt;
      }
  }

  template<class TInputImage, class TOutputImage>
  void 
  NLMeansFilter<TInputImage, TOutputImage>::ComputeIntegralImage
  (const std::vector<double> & dataInput, 
   std::vector<double> &imIntegral, 
   const OutIndexType shift, const InSizeType sizeIntegral, const InSizeType sizeInput) const
  {
    // dataInput has a margin of m_HalfSearchSize+m_HalfPatchSize to allow
    // computation of all shifts (computation of all integral images)
    // integral images just have the m_HalfPatchSize margin necessary
    // to compute patches differences for a given shift
    // hence, the first point used in computation for the non-shifted image
    // is located at m_HalfSearchSize
    auto const& offsetRef = m_HalfSearchSize;
    OutSizeType const& offsetShift = {{offsetRef[0] + shift[0], offsetRef[1] + shift[1]}};

    // Initialize integral image (compute position (0,0))
    auto indexInput = offsetRef[m_ROW]*sizeInput[m_COL] + offsetRef[m_COL];
    auto indexShift = offsetShift[m_ROW]*sizeInput[m_COL] + offsetShift[m_COL];
    double diff = dataInput[indexInput] - dataInput[indexShift];
    imIntegral[0] = (diff * diff) - m_Var;
    // Compute first line of integral image
    for (unsigned int col=1; col<sizeIntegral[m_COL]; col++)
      {
	auto indexInputCol = indexInput + col;
	auto indexShiftCol = indexShift + col;
	diff = dataInput[indexInputCol] - dataInput[indexShiftCol];
	double distance = diff * diff - m_Var;
	imIntegral[col] = distance + imIntegral[col-1];
	assert(imIntegral[col] < itk::NumericTraits<double>::max());
      }
    // Compute first column of integral image
    for (unsigned int row=1; row<sizeIntegral[m_ROW]; row++)
      {
	auto indexInputRow = indexInput + row*sizeInput[m_COL];
	auto indexShiftRow = indexShift + row*sizeInput[m_COL];
	diff = dataInput[indexInputRow] - dataInput[indexShiftRow];
	double distance = diff * diff - m_Var;
	imIntegral[row*sizeIntegral[m_COL]] = distance + imIntegral[(row-1)*sizeIntegral[m_COL]];
	assert(imIntegral[row*sizeIntegral[m_COL]] < itk::NumericTraits<double>::max());
      }
    // All initializations have been done previously
    // Remaining coefficients can be computed
    for (unsigned int row=1; row<sizeIntegral[m_ROW]; row++)
      for (unsigned int col=1; col<sizeIntegral[m_COL]; col++)
	{
	  indexInput = (offsetRef[m_ROW]+row)*sizeInput[m_COL] + offsetRef[m_COL]+col;
	  indexShift = (offsetShift[m_ROW]+row)*sizeInput[m_COL] + offsetShift[m_COL]+col;
	  diff = dataInput[indexInput] - dataInput[indexShift];
	  double distance = diff*diff - m_Var;
	  imIntegral[row*sizeIntegral[m_COL] + col] = distance + imIntegral[row*sizeIntegral[m_COL] + col-1] 
	    + imIntegral[(row-1)*sizeIntegral[m_COL] + col] - imIntegral[(row-1)*sizeIntegral[m_COL] + col-1];
	  assert(imIntegral[row*sizeIntegral[m_COL] + col] < itk::NumericTraits<double>::max());
	}
    
  }

  template <class TInputImage, class TOutputImage>
  typename NLMeansFilter<TInputImage, TOutputImage>::OutPixelType 
  NLMeansFilter<TInputImage, TOutputImage>::ComputeDistance
  (const unsigned int row, const unsigned int col, 
   const std::vector<double>& imIntegral, const unsigned int nbCols) const
  {
    // (row, col) is the central position of the local window in the output image
    // however, integral image is shifted by (m_HalfPatchSize, m_HalfPatchSize) compared to output image
    // Thus, (row, col) corresponds, in integral image, to the upper left corner of the local window
    double distance_patch = 
      imIntegral[(row+2*m_HalfPatchSize[m_ROW])*nbCols + col+2*m_HalfPatchSize[m_COL]] 
      - imIntegral[row*nbCols + col+2*m_HalfPatchSize[m_COL]]
      - imIntegral[(row+2*m_HalfPatchSize[m_ROW])*nbCols + col]
      + imIntegral[row*nbCols + col];

    distance_patch = std::max(distance_patch, 0.0) / (m_NormalizeDistance);
    return static_cast<OutPixelType>(distance_patch);
  }

  template<class TInputImage, class TOutputImage>
  void NLMeansFilter<TInputImage, TOutputImage>
  ::PrintSelf(std::ostream & os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os, indent);

    os<<indent<<"NL Means Patch Size : "<<2*m_HalfPatchSize[m_ROW]+1
      <<" x "<<2*m_HalfPatchSize[m_COL]+1<< std::endl;
    os<<indent<<"NL Means Window Search Size : "<<2*m_HalfSearchSize[m_ROW]+1
      <<" x "<<2*m_HalfSearchSize[m_COL]+1<< std::endl;
    os<<indent<<"NL Means variance : "<<m_Var<<std::endl;
    os<<indent<<"NL Means threshold for similarity : "<<m_CutoffDistance
      << std::endl;
  }

} // end namespace otb

#endif
