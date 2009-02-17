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
#ifndef __otbDrawLineSpatialObjectListFilter_txx
#define __otbDrawLineSpatialObjectListFilter_txx

#include "otbDrawLineSpatialObjectListFilter.h"

#include "itkLineIterator.h"
#include "itkDataObject.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"

#include <math.h>

namespace otb
{

/**
 *
 */
template <class TInputImage, class TOutputImage>
DrawLineSpatialObjectListFilter<TInputImage, TOutputImage>
::DrawLineSpatialObjectListFilter()
{
  this->SetNumberOfRequiredInputs(2);
  this->SetNumberOfRequiredOutputs(1);

  m_Value = static_cast<OutputPixelType>(255.);
}

template <class TInputImage, class TOutputImage>
void
DrawLineSpatialObjectListFilter<TInputImage, TOutputImage>
::SetInputLineSpatialObjectList(const LinesListType * list)
{
  this->ProcessObjectType::SetNthInput(1,
                                       const_cast< LinesListType * >( list ) );

}

template <class TInputImage, class TOutputImage>
typename DrawLineSpatialObjectListFilter<TInputImage, TOutputImage>::LinesListType *
DrawLineSpatialObjectListFilter<TInputImage, TOutputImage>
::GetInputLineSpatialObjectList(void)
{
  //ROMAIN
  return static_cast</*const */LinesListType *>
         (this->ProcessObjectType::GetInput(1) );
}

template <class TInputImage, class TOutputImage>
void
DrawLineSpatialObjectListFilter<TInputImage, TOutputImage>
::ThreadedGenerateData( const OutputImageRegionType &outputRegionForThread, int threadId )

{
  typename InputImageType::ConstPointer input  = this->GetInput();
  typename OutputImageType::Pointer output  = this->GetOutput();
  typename LinesListType::Pointer    list = this->GetInputLineSpatialObjectList();
       
  /** Copy the input requested region in the output requested region*/
  typedef itk::ImageRegionIterator< OutputImageType >      OutputIteratorType;
  typedef itk::ImageRegionConstIterator< InputImageType >  InputIteratorType;

  OutputIteratorType    outputIt( output, outputRegionForThread);
  InputIteratorType     inputIt(  input,  outputRegionForThread);
  
  outputIt.GoToBegin();
  inputIt.GoToBegin();

  for (outputIt.GoToBegin(); !outputIt.IsAtEnd()  ; ++outputIt,++inputIt)
      outputIt.Set( static_cast<OutputPixelType>(inputIt.Get()) );
  
  /** Draw the lines in the ouput image using lineIterator*/
  typedef itk::LineIterator<OutputImageType>       LineIteratorFilter;
  OutputIndexType                                  indexBeginLine, indexEndLine;
  LineListIterator                                 itList = list->begin();

  /** Dimensions of the buffered region*/
  //   typename OutputImageRegionType::SizeType size = outputRegionForThread.GetSize();
  //   typename OutputImageRegionType::IndexType start = outputRegionForThread.GetIndex();
  
  while(itList != list->end())
    {
      PointListType & pointsList = (*itList)->GetPoints();
      typename PointListType::const_iterator itPoints = pointsList.begin();

      indexBeginLine[0] = static_cast<unsigned int>((*itPoints).GetPosition()[0]);
      indexBeginLine[1] = static_cast<unsigned int>((*itPoints).GetPosition()[1]);

      ++itPoints;  //Get the second extremity of the segment

      indexEndLine[0] = static_cast<unsigned int>((*itPoints).GetPosition()[0]);
      indexEndLine[1] = static_cast<unsigned int>((*itPoints).GetPosition()[1]);

      /** Instanciation of the line iterator with begin and ending index*/
      LineIteratorFilter   itLine(output,indexBeginLine ,indexEndLine  );
      
      /** Iteration over the line and writing white lines */
      while(!itLine.IsAtEnd())
	{
	if(outputRegionForThread.IsInside(itLine.GetIndex()))
	  itLine.Set(m_Value);
	++itLine;
	}
      
      ++itList;
    }
}


/**
 * Standard "PrintSelf" method
 */
template <class TInputImage, class TOutput>
void
DrawLineSpatialObjectListFilter<TInputImage, TOutput>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}


} // end namespace otb


#endif
