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
#ifndef __otbLabelImageToLabelMapWithAdjacencyFilter_txx
#define __otbLabelImageToLabelMapWithAdjacencyFilter_txx

#include "otbLabelImageToLabelMapWithAdjacencyFilter.h"
#include "itkNumericTraits.h"
#include "itkProgressReporter.h"
#include "itkImageLinearConstIteratorWithIndex.h"
#include "itkImageRegionConstIteratorWithIndex.h"

namespace otb {

template <class TInputImage, class TOutputImage>
LabelImageToLabelMapWithAdjacencyFilter<TInputImage, TOutputImage>
::LabelImageToLabelMapWithAdjacencyFilter()
{
  m_BackgroundValue = itk::NumericTraits<OutputImagePixelType>::NonpositiveMin();
}

template <class TInputImage, class TOutputImage>
void
LabelImageToLabelMapWithAdjacencyFilter<TInputImage, TOutputImage>
::GenerateInputRequestedRegion()
{
  for (unsigned int idx = 0; idx < this->GetNumberOfInputs(); ++idx)
        {
         InputImagePointer input = const_cast<InputImageType *>(this->GetInput(idx));
        if (!input.IsNull())
          {
          input->SetRequestedRegionToLargestPossibleRegion();
          // Check whether the input is an image of the appropriate
          // dimension (use ProcessObject's version of the GetInput()
          // method since it returns the input as a pointer to a
          // DataObject as opposed to the subclass version which
          // static_casts the input to an TInputImage).

          // Use the function object RegionCopier to copy the output region
          // to the input.  The default region copier has default implementations
          // to handle the cases where the input and output are the same
          // dimension, the input a higher dimension than the output, and the
          // input a lower dimension than the output.
          InputImageRegionType inputRegion;
          this->CallCopyOutputRegionToInputRegion(inputRegion, this->GetOutput()->GetRequestedRegion());
          input->SetRequestedRegion( inputRegion );
          }
        }
}


template <class TInputImage, class TOutputImage>
void
LabelImageToLabelMapWithAdjacencyFilter<TInputImage, TOutputImage>
::EnlargeOutputRequestedRegion(itk::DataObject *)
{
}


template<class TInputImage, class TOutputImage>
void
LabelImageToLabelMapWithAdjacencyFilter<TInputImage, TOutputImage>
::BeforeThreadedGenerateData()
{
  // init the temp images - one per thread
  m_TemporaryImages.resize( this->GetNumberOfThreads() );
  // Clear previous adjacency map
  m_TemporaryAdjacencyMaps.resize( this->GetNumberOfThreads() );

  for( unsigned int i=0; i<this->GetNumberOfThreads(); ++i )
    {
    if( i == 0 )
      {
      // the first one is the output image
      m_TemporaryImages[0] = this->GetOutput();
      }
    else
      {
      // the other must be created
      m_TemporaryImages[i] = OutputImageType::New();
      }

    // set the minimum data needed to create the objects properly
    m_TemporaryImages[i]->SetBackgroundValue( m_BackgroundValue );

    }
}


template<class TInputImage, class TOutputImage>
void
LabelImageToLabelMapWithAdjacencyFilter<TInputImage, TOutputImage>
::AddAdjacency(LabelType label1, LabelType label2, itk::ThreadIdType threadId)
{
  // Insert label1 in v neighbors
  if(m_TemporaryAdjacencyMaps[threadId].find(label2)!= m_TemporaryAdjacencyMaps[threadId].end())
    {
    m_TemporaryAdjacencyMaps[threadId][label2].insert(label1);
    }
  else
    {
    AdjacentLabelsContainerType newContainer;
    newContainer.insert(label1);
    m_TemporaryAdjacencyMaps[threadId][label2]=newContainer;
    }
  // Insert label2 in label1s neighbors
  if(m_TemporaryAdjacencyMaps[threadId].find(label1)!=m_TemporaryAdjacencyMaps[threadId].end())
    {
    m_TemporaryAdjacencyMaps[threadId][label1].insert(label2);
    }
  else
    {
    AdjacentLabelsContainerType newContainer;
    newContainer.insert(label2);
    m_TemporaryAdjacencyMaps[threadId][label1]=newContainer;
    }
}

template<class TInputImage, class TOutputImage>
void
LabelImageToLabelMapWithAdjacencyFilter<TInputImage, TOutputImage>
::ParseLine(const RLEVectorType & line, itk::ThreadIdType threadId)
{
  // Get the first label
  typename RLEVectorType::const_iterator it = line.begin();
  LabelType previousLabel = it->label;
  ++it;

  // Iterates on remaining RLE
  while(it!=line.end())
    {
    // Get the new label
    LabelType nextLabel = it->label;

    // Add the adjacency
    this->AddAdjacency(previousLabel, nextLabel, threadId);

    // Store previous label
    previousLabel = nextLabel;
    ++it;
    }
}

template<class TInputImage, class TOutputImage>
void
LabelImageToLabelMapWithAdjacencyFilter<TInputImage, TOutputImage>
::ParseConsecutiveLines(const RLEVectorType& line1, const RLEVectorType & line2, itk::ThreadIdType threadId)
{
  // Provided to disable fully connected if needed
  long offset = 1;

  // Iterate on line1
  for(typename RLEVectorType::const_iterator it1 = line1.begin(); it1!=line1.end(); ++it1)
    {
    // Delimitate RLE1
    long start1 = it1->where[0];
    long end1 = start1 + it1->length-1;
    LabelType label1 = it1->label;

    // Iterate on line2
    for(typename RLEVectorType::const_iterator it2 = line2.begin(); it2!=line2.end(); ++it2)
      {
      // Delimitate RLE2
      long start2 = it2->where[0];
      long end2 = start2 + it2->length-1;
      LabelType label2 = it2->label;

      // If labels are different
      if(label1 != label2)
  {
  //Check adjacency
  if(  (  (start1 >= start2 - offset) && (start1 <= end2 + offset) )
       || ( (end1 >= start2 - offset) && (end1  <= end2 + offset) )
       || ( (start2 >= start1 - offset) && (start2 <= end1 + offset) )
       || ( (end2 >= start1 - offset) && (end2 <= end1 + offset) ))
    {
    // Add the adjacency
    this->AddAdjacency(label1, label2, threadId);
    }
  }
      }
    }
}

template<class TInputImage, class TOutputImage>
void
LabelImageToLabelMapWithAdjacencyFilter<TInputImage, TOutputImage>
::ThreadedGenerateData( const OutputImageRegionType& regionForThread, itk::ThreadIdType threadId )
{
  itk::ProgressReporter progress( this, threadId, regionForThread.GetNumberOfPixels() );

  typedef itk::ImageLinearConstIteratorWithIndex< InputImageType > InputLineIteratorType;

  InputLineIteratorType it( this->GetInput(), regionForThread );
  it.SetDirection(0);

  RLEVectorType currentLine;
  RLEVectorType previousLine;

  // Parse previous line if exists
  typename InputImageType::RegionType previousLineRegion;
  typename InputImageType::SizeType previousLineRegionSize;
  typename InputImageType::IndexType previousLineRegionIndex;

  previousLineRegionIndex = regionForThread.GetIndex();
  previousLineRegionIndex[1]--;

  previousLineRegionSize = regionForThread.GetSize();
  previousLineRegionSize[1]=1;

  previousLineRegion.SetIndex(previousLineRegionIndex);
  previousLineRegion.SetSize(previousLineRegionSize);

  // If previous line is still in image
  if(previousLineRegion.Crop(this->GetInput()->GetRequestedRegion()))
    {
    // Build an iterator
    itk::ImageRegionConstIteratorWithIndex<InputImageType> pIt(this->GetInput(), previousLineRegion);
    pIt.GoToBegin();

    // Iterate on line
    while( !pIt.IsAtEnd() )
      {
      const InputImagePixelType & v = pIt.Get();

      if( v != m_BackgroundValue )
        {
        // We've hit the start of a run
        IndexType idx = pIt.GetIndex();
        long length=1;
        ++pIt;
        while( !pIt.IsAtEnd() && pIt.Get() == v )
          {
          ++length;
          ++pIt;
          }
        previousLine.push_back(RLE(idx, length, v));
        }
      else
        {
        // go the the next pixel
        ++pIt;
        }
      }
    }

  for( it.GoToBegin(); !it.IsAtEnd(); it.NextLine() )
    {
    // Go to beginning of line
    it.GoToBeginOfLine();

    // Clear the previous current line
    currentLine.clear();

    // Iterate on line
    while( !it.IsAtEndOfLine() )
      {
      const InputImagePixelType & v = it.Get();

      if( v != m_BackgroundValue )
        {
        // We've hit the start of a run
        IndexType idx = it.GetIndex();
        long length=1;
        ++it;
        while( !it.IsAtEndOfLine() && it.Get() == v )
          {
          ++length;
          ++it;
          }
        // create the run length object to go in the vector
        m_TemporaryImages[threadId]->SetLine( idx, length, v );
        currentLine.push_back(RLE(idx, length, v));
        }
      else
        {
        // go the the next pixel
        ++it;
        }
      }

    // if no label object  is present on current line we skip the process
    if(currentLine.size()>0)
      {
    // Parse lines for adjacency
    this->ParseLine(currentLine, threadId);
    this->ParseConsecutiveLines(previousLine, currentLine, threadId);
      }

    // Store previous line
    previousLine = currentLine;
    }
}


template<class TInputImage, class TOutputImage>
void
LabelImageToLabelMapWithAdjacencyFilter<TInputImage, TOutputImage>
::AfterThreadedGenerateData()
{

  OutputImageType * output = this->GetOutput();

  // merge the lines from the temporary images in the output image
  // don't use the first image - that's the output image
  for( unsigned int i=1; i<this->GetNumberOfThreads(); ++i )
    {
    typedef typename OutputImageType::LabelObjectVectorType LabelObjectVectorType;
    const LabelObjectVectorType & labelObjectContainer = m_TemporaryImages[i]->GetLabelObjects();

    for( typename LabelObjectVectorType::const_iterator it = labelObjectContainer.begin();
        it != labelObjectContainer.end();
        ++it )
      {
      LabelObjectType * labelObject = *it;
      if( output->HasLabel( labelObject->GetLabel() ) )
        {
        // merge the lines in the output's object
       //ITKv4 iterate over the 2 LabelObjects src and output
       typedef typename LabelObjectType::ConstLineIterator IteratorType;

  typename LabelObjectType::LabelObjectType * dest = output->GetLabelObject( labelObject->GetLabel() );

       IteratorType srcIt;
       srcIt = IteratorType( labelObject );
       srcIt.GoToBegin();

       //ITKv4 Iterate over all lines of the source and add them to the
       //output labelObject (can use the insert method over the
       //container anymore (it is now private)
       while ( !srcIt.IsAtEnd() )
         {
         dest->AddLine(srcIt.GetLine());
    ++srcIt;
         }
        }
      else
        {
        // simply take the object
        output->AddLabelObject( labelObject );
        }
      }
    }

  // Merge adjacency tables
  AdjacencyMapType adjMap = m_TemporaryAdjacencyMaps[0];

  // For each remaining thread
  for(itk::ThreadIdType threadId = 1; threadId < this->GetNumberOfThreads(); ++threadId)
    {
    // For each label in the thread adjacency map
    for(typename AdjacencyMapType::const_iterator mit = m_TemporaryAdjacencyMaps[threadId].begin();
  mit!= m_TemporaryAdjacencyMaps[threadId].end();
  ++mit)
      {
      // If the label exists in the main map
      if(adjMap.find(mit->first) != adjMap.end())
  {
  // We need to merge
  AdjacentLabelsContainerType adjLabels1 = adjMap[mit->first];
  AdjacentLabelsContainerType adjLabels2 = mit->second;
  std::vector<LabelType> mergedLabels(adjLabels1.size()+adjLabels2.size(), 0);

  // Merge
  typename std::vector<LabelType>::const_iterator vend = set_union(adjLabels1.begin(), adjLabels1.end(), adjLabels2.begin(), adjLabels2.end(), mergedLabels.begin());

  AdjacentLabelsContainerType mergedLabelsSet;

  for(typename std::vector<LabelType>::const_iterator vit = mergedLabels.begin();
      vit!=vend; ++vit)
    {
    mergedLabelsSet.insert(*vit);
    }

  // Set the final result
  adjMap[mit->first]=mergedLabelsSet;
  }
      else
  {
  // Set the labels
  adjMap[mit->first]=mit->second;
  }
      }
    }

  // Set the adjacency map to the output
  output->SetAdjacencyMap(adjMap);

  // release the data in the temp images
  m_TemporaryImages.clear();
  m_TemporaryAdjacencyMaps.clear();
}


template<class TInputImage, class TOutputImage>
void
LabelImageToLabelMapWithAdjacencyFilter<TInputImage, TOutputImage>
::PrintSelf(std::ostream &os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "BackgroundValue: "  << static_cast<typename itk::NumericTraits<OutputImagePixelType>::PrintType>(m_BackgroundValue) << std::endl;
}

}// end namespace otb
#endif
