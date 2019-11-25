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
#include <vector>

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
  typename NLMeansFilter<TInputImage, TOutputImage>::InRegionType
  NLMeansFilter<TInputImage, TOutputImage>::OutputRegionToInputRegion
  (const OutRegionType& outputRegion, int& mirrorFirstRow, 
   int& mirrorFirstCol, int& mirrorLastRow, int& mirrorLastCol, 
   bool& needMirrorPadding) const
  {
    InImageConstPointerType inputPtr = this->GetInput();
    InSizeType inputSize = inputPtr->GetLargestPossibleRegion().GetSize();

    // Get output region specification
    OutIndexType outIndex = outputRegion.GetIndex();
    OutSizeType outSize = outputRegion.GetSize();

    // Define margin for processing
    InSizeType halfMargin;
    halfMargin = m_HalfSearchSize + m_HalfPatchSize;
    InSizeType fullMargin;
    fullMargin[m_COL] = 2*halfMargin[m_COL];
    fullMargin[m_ROW] = 2*halfMargin[m_ROW];

    // Define region to read
    InIndexType inIndex; 
    inIndex = outIndex - halfMargin;

    InSizeType requestedSize;
    requestedSize = outSize + fullMargin;

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
    return inRequestedRegion;
  }

  template <class TInputImage, class TOutputImage>
  void NLMeansFilter<TInputImage, TOutputImage>
  ::GenerateInputRequestedRegion()
  {
    // Call the superclass' implementation of this method
    Superclass::GenerateInputRequestedRegion();

    OutRegionType outputRequestedRegion = this->GetOutput()->GetRequestedRegion();
    int fr, fc, lr, lc;
    bool needMirror;
    InRegionType inRequestedRegion = this->OutputRegionToInputRegion
      (outputRequestedRegion, fr, fc, lr, lc, needMirror);

    InImageType * inputPtr = const_cast<InImageType * >(this->GetInput());
    inputPtr->SetRequestedRegion(inRequestedRegion);
  }

  template<class TInputImage, class TOutputImage>
  void 
  NLMeansFilter<TInputImage, TOutputImage>::ThreadedGenerateData
  (const OutRegionType& outputRegionForThread, 
   itk::ThreadIdType itkNotUsed(threadId))
  {
    int mirrorFirstRow=0, mirrorFirstCol=0;
    int mirrorLastRow=0, mirrorLastCol=0;
    bool needMirror=false;

    InImageConstPointerType inputPtr = this->GetInput();
    InRegionType inputRegionForThread = OutputRegionToInputRegion
      (outputRegionForThread, 
       mirrorFirstRow, 
       mirrorFirstCol, 
       mirrorLastRow, 
       mirrorLastCol,
       needMirror);

    // initialize and allocate vector to store temporary output values 
    // It makes it easier to store them in vectors to access various non-contiguous locations
    OutSizeType outSize = outputRegionForThread.GetSize();
    std::vector<std::vector<double> > outTemp(outSize[m_ROW], std::vector<double>(outSize[m_COL]));
    // initialize and allocate buffer to store all weights
    std::vector<std::vector<double> > weights(outSize[m_ROW], std::vector<double>(outSize[m_COL]));

    typedef itk::ImageRegionConstIterator<InImageType> InIteratorType;
    InIteratorType inIt(inputPtr, inputRegionForThread);
    InSizeType inputSize = inputRegionForThread.GetSize();

    InSizeType mirrorSize;
    mirrorSize[m_COL] = inputSize[m_COL] + mirrorFirstCol + mirrorLastCol;
    mirrorSize[m_ROW] = inputSize[m_ROW] + mirrorFirstRow + mirrorLastRow;
    std::vector< std::vector<double> > dataInput 
      (mirrorSize[m_ROW], std::vector<double>(mirrorSize[m_COL]));
    inIt.GoToBegin();
    for (unsigned int row=static_cast<unsigned int>(mirrorFirstRow); 
	 row<static_cast<unsigned int>(mirrorFirstRow)+inputSize[m_ROW]; row++)
      for (unsigned int col=static_cast<unsigned int>(mirrorFirstCol); 
	   col<static_cast<unsigned int>(mirrorFirstCol)+inputSize[m_COL]; col++)
	{
	  dataInput[row][col] = static_cast<double>(inIt.Get());
	  ++inIt;
	}

    if (needMirror)
      {
	// Perform mirror on upper lines
	for (int row=0; row<mirrorFirstRow; row++)
	  {
	    int lineToCopy = 2*mirrorFirstRow - row;
	    std::copy(dataInput[lineToCopy].begin(),
		      dataInput[lineToCopy].end(),
		      dataInput[row].begin());
	  }
	// Perform mirror on lower lines
	int lastRowRead = mirrorFirstRow+inputSize[m_ROW];
	for (int row=0; row<mirrorLastRow; row++)
	  {
	    int lineToCopy = lastRowRead - row -2;
	    std::copy(dataInput[lineToCopy].begin(),
		      dataInput[lineToCopy].end(),
		      dataInput[lastRowRead + row].begin());
	  }
	// Perform mirror on left-hand columns
	if (mirrorFirstCol > 0) {
	  for (unsigned int row=0; row<mirrorSize[m_ROW]; row++)
	    {
	      std::reverse_copy(dataInput[row].begin()+mirrorFirstCol+1,
				dataInput[row].begin()+2*mirrorFirstCol+1,
				dataInput[row].begin());
	    }
		
	}
	// Perform mirror on right-hand columns
	if (mirrorLastCol > 0){
	  for (unsigned int row=0; row<mirrorSize[m_ROW]; row++)
	    {
	      std::reverse_copy(dataInput[row].end()-2*mirrorLastCol-1,
				dataInput[row].end()-mirrorLastCol-1,
				dataInput[row].end()-mirrorLastCol);
	    }
	}
      }

    // For loops on all shifts possible
    int fullMarginRow = static_cast<int>(m_HalfSearchSize[m_ROW]+m_HalfPatchSize[m_ROW]);
    int fullMarginCol = static_cast<int>(m_HalfSearchSize[m_COL]+m_HalfPatchSize[m_COL]);
    int searchSizeRow = static_cast<int>(m_HalfSearchSize[m_ROW]);
    int searchSizeCol = static_cast<int>(m_HalfSearchSize[m_COL]);
    for (int drow=-searchSizeRow; drow < searchSizeRow+1; drow++)
      for (int dcol=-searchSizeCol; dcol < searchSizeCol+1; dcol++)
	{
	  // Allocate integral image
	  InSizeType inSize;
	  inSize[m_COL] = outSize[m_COL] + 2*m_HalfPatchSize[m_COL];
	  inSize[m_ROW] = outSize[m_ROW] + 2*m_HalfPatchSize[m_ROW];
	  std::vector<std::vector<double> > imIntegral
	    (inSize[m_ROW], std::vector<double>(inSize[m_COL]));
	  // Compute integral image for current shift (drow, dcol)
	  OutIndexType shift;
	  shift[m_COL] = dcol;
	  shift[m_ROW] = drow; 
	  ComputeIntegralImage(dataInput, imIntegral, shift, inSize);

	  for(unsigned int row=0; row<outSize[m_ROW]; row++)
	    for (unsigned int col=0; col<outSize[m_COL]; col++)
	      {
		// Compute distance from integral image for patch centered at 
		// (row, col) + (m_HalfPatchSize, m_HalfPatchSize)
		OutPixelType distance = ComputeDistance(row, col, imIntegral);
		if (distance < 5.0)
		  {
		    double weight = exp(static_cast<double>(-distance));
		    outTemp[row][col] += weight*dataInput[row+drow+fullMarginRow][col+dcol+fullMarginCol];
		    weights[row][col] += weight;
		  }
	      }
	}

    // Normalize all results by dividing output by weights (store in output)
    typedef itk::ImageRegionIterator<OutImageType> OutputIteratorType;
    OutImagePointerType outputPtr = this->GetOutput();
    OutputIteratorType outIt(outputPtr, outputRegionForThread);
    outIt.GoToBegin();
    for(unsigned int row=0; row<outSize[m_ROW]; row++)
      for (unsigned int col=0; col<outSize[m_COL]; col++)
	{
	  outIt.Set(static_cast<OutPixelType>(outTemp[row][col]/weights[row][col]));
	  ++outIt;
	}
  }

  template<class TInputImage, class TOutputImage>
  void 
  NLMeansFilter<TInputImage, TOutputImage>::ComputeIntegralImage
  (const std::vector<std::vector<double> > & dataInput, 
   std::vector<std::vector<double> > &imIntegral, 
   const OutIndexType shift, const InSizeType sizeIn)
  {
    // dataInput has a margin of m_HalfSearchSize+m_HalfPatchSize to allow
    // computation of all shifts (computation of all integral images)
    // integral images just have the m_HalfPatchSize margin necessary
    // to compute patches differences for a given shift
    // hence, the first point used in computation for the non-shifted image
    // is located at m_HalfSearchSize
    InSizeType offsetRef = m_HalfSearchSize;
    InSizeType offsetShift;
    offsetShift[m_COL] = offsetRef[m_COL] + shift[m_COL];
    offsetShift[m_ROW] = offsetRef[m_ROW] + shift[m_ROW];

    // Initialize integral image (compute position (0,0))
    double diff = dataInput[offsetRef[m_ROW]][offsetRef[m_COL]] - 
      dataInput[offsetShift[m_ROW]][offsetShift[m_COL]];
    imIntegral[0][0] = (diff * diff) - m_Var;
    // Compute first line of integral image
    for (unsigned int col=1; col<sizeIn[m_COL]; col++)
      {
	diff = dataInput[offsetRef[m_ROW]][col+offsetRef[m_COL]] - 
	  dataInput[offsetShift[m_ROW]][col+offsetShift[m_COL]];
	double distance = diff * diff - m_Var;
	imIntegral[0][col] = distance + imIntegral[0][col-1];
      }
    // Compute first column of integral image
    for (unsigned int row=1; row<sizeIn[m_ROW]; row++)
      {
	diff = dataInput[row+offsetRef[m_ROW]][offsetRef[m_COL]] - 
	  dataInput[row+offsetShift[m_ROW]][offsetShift[m_COL]];
	double distance = diff * diff - m_Var;
	imIntegral[row][0] = distance + imIntegral[row-1][0];
      }
    // All initializations have been done previously
    // Remaining coefficients can be computed
    for (unsigned int row=1; row<sizeIn[m_ROW]; row++)
      for (unsigned int col=1; col<sizeIn[m_COL]; col++)
	{
	  diff = dataInput[row+offsetRef[m_ROW]][col+offsetRef[m_COL]] - 
	    dataInput[row+offsetShift[m_ROW]][col+offsetShift[m_COL]];
	  double distance = diff*diff - m_Var;
	  imIntegral[row][col] = distance + imIntegral[row][col-1] 
	    + imIntegral[row-1][col] - imIntegral[row-1][col-1];
	}
    
  }

  template <class TInputImage, class TOutputImage>
  typename NLMeansFilter<TInputImage, TOutputImage>::OutPixelType 
  NLMeansFilter<TInputImage, TOutputImage>::ComputeDistance
  (const unsigned int row, const unsigned int col, 
   const std::vector<std::vector<double> >& imIntegral)
  {
    // (row, col) is the central position of the local window in the output image
    // however, integral image is shifted by (m_HalfPatchSize, m_HalfPatchSize) compared to output image
    // Thus, (row, col) corresponds, in integral image, to the upper left corner of the local window
    double distance_patch = 
      imIntegral[row+2*m_HalfPatchSize[m_ROW]][col+2*m_HalfPatchSize[m_COL]] 
      + imIntegral[row][col]
      - imIntegral[row][col+2*m_HalfPatchSize[m_COL]]
      - imIntegral[row+2*m_HalfPatchSize[m_ROW]][col];

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
