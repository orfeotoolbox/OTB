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
#ifndef __otbTileImageFilter_txx
#define __otbTileImageFilter_txx

#include "otbTileImageFilter.h"
#include "itkImageRegionConstIterator.h"
#include "itkImageRegionIterator.h"

namespace otb
{
template <class TImage>
TileImageFilter<TImage>
::TileImageFilter()
{}

template <class TImage>
TileImageFilter<TImage>
::~TileImageFilter()
{}

template <class TImage>
void
TileImageFilter<TImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  os<<indent<<"Layout: "<<m_Layout<<std::endl;
}

template <class TImage>
void
TileImageFilter<TImage>
::GenerateOutputInformation()
{
  // First, call superclass implementation
  Superclass::GenerateOutputInformation();

  // Get the number of images
  unsigned int numberOfImages = m_Layout[0] * m_Layout[1];

  // Check we have enough inputs
  if(numberOfImages != this->GetNumberOfInputs())
    {
    itkExceptionMacro(<<"Layout has "<<numberOfImages<<" tiles, but only "<<this->GetNumberOfInputs()<<" inputs are found.");
    }

  typename ImageType::SpacingType spacing = this->GetInput()->GetSpacing();
  unsigned int nbComp = this->GetInput()->GetNumberOfComponentsPerPixel();

  m_ColumnsSizes.clear();
  m_RowsSizes.clear();

  // Loop on the layout
  for(unsigned int col = 0; col < m_Layout[0]; ++col)
    {
    for(unsigned int row = 0; row < m_Layout[1]; ++row)
      {
      // First, get current tile
      const ImageType * currentTile = this->GetInput(col + row * m_Layout[0]);
      typename ImageType::SizeType currentSize = currentTile->GetLargestPossibleRegion().GetSize();

      // Retrieve row and column sizes
      if(col == 0)
        {
        m_RowsSizes.push_back(currentSize[1]);
        }
      if(row == 0)
        {
        m_ColumnsSizes.push_back(currentSize[0]);
        }

      // Check for consistent layout
      if(currentSize[1] != m_RowsSizes[row]
         || currentSize[0] != m_ColumnsSizes[col])
        {
        itkExceptionMacro(<<"Inconsistent sizes in layout detected!");
        }

      if(spacing != currentTile->GetSpacing())
        {
        itkExceptionMacro(<<"Inconsistent spacings in layout detected!");
        }

      if(nbComp != currentTile->GetNumberOfComponentsPerPixel())
        {
        itkExceptionMacro(<<"Inconsistent number of components in layout detected!");
        }
      }
    }

  // Compute total size
  typename ImageType::SizeType totalSize;
  totalSize.Fill(0);

  for(unsigned int i = 0; i < m_Layout[0]; ++i)
    {
    totalSize[0]+=m_ColumnsSizes[i];
    }

  for(unsigned int i = 0; i < m_Layout[1]; ++i)
    {
    totalSize[1]+=m_RowsSizes[i];
    }

  // Fill out output image settings
  typename ImageType::RegionType outRegion;
  outRegion.SetSize(totalSize);

  //std::cout<<"Output largest region: "<<outRegion<<std::endl;

  ImageType * outputPtr = this->GetOutput();

  // Copy information from first tile
  outputPtr->CopyInformation(this->GetInput());

  // Set region
  outputPtr->SetLargestPossibleRegion(outRegion);
}

template <class TImage>
void
TileImageFilter<TImage>
::GenerateInputRequestedRegion()
{
  // Get output requested region
  RegionType outRegion = this->GetOutput()->GetRequestedRegion();

  // Loop on al input images
  unsigned int numberOfImages = m_Layout[0] * m_Layout[1];

  //std::cout<<"Number of input images: "<<this->GetNumberOfInputs()<<std::endl;

  //std::cout<<"Out region: "<<outRegion<<std::endl;

  for(unsigned int i = 0; i < numberOfImages; ++i)
    {
    ImageType * inputTile = const_cast<ImageType *>(this->GetInput(i));

    RegionType inRegion = OutputRegionToInputRegion(i,outRegion);

    //std::cout<<"In region: "<<i<<", "<<inRegion<<std::endl;
    //std::cout<<"tile pointer: "<<inputTile<<std::endl;

    inputTile->SetRequestedRegion(inRegion);
    }
}

template <class TImage>
void
TileImageFilter<TImage>
::ThreadedGenerateData(const RegionType& outputRegionForThread, itk::ThreadIdType itkNotUsed(threadId))
{
  // Retrieve output image pointer
  ImageType * outputPtr = this->GetOutput();

  // Loop on all input tiles
  unsigned int numberOfImages = m_Layout[0] * m_Layout[1];

  for(unsigned int i = 0; i < numberOfImages; ++i)
    {
    const ImageType * inputTile = this->GetInput(i);

    RegionType inRegion = OutputRegionToInputRegion(i,outputRegionForThread);
    RegionType outRegion = InputRegionToOutputRegion(i,inRegion);

    //std::cout<<"[thread] outRegion: "<<outRegion<<std::endl;

    if(inRegion.GetNumberOfPixels() > 0)
      {
      //std::cout<<"[thread] "<<i<<" inRegion: "<<inRegion<<std::endl;

      //std::cout<<"[thread] "<<i<< inputTile->GetBufferedRegion()<<std::endl;

      itk::ImageRegionConstIterator<ImageType> inIt(inputTile,inRegion);
      itk::ImageRegionIterator<ImageType>      outIt(outputPtr,outRegion);

      inIt.GoToBegin();
      outIt.GoToBegin();

      while(!inIt.IsAtEnd() && !outIt.IsAtEnd())
        {
        outIt.Set(inIt.Get());
        ++inIt;
        ++outIt;
        }
      }
    }
}

template <class TImage>
typename TileImageFilter<TImage>
::RegionType
TileImageFilter<TImage>
::OutputRegionToInputRegion(unsigned int tileIndex, const RegionType & requestedRegion)
{
  // Retrieve the nth tile pointer
  const ImageType * tile = this->GetInput(tileIndex);

  unsigned int tileYIndex = tileIndex / m_Layout[0];
  unsigned int tileXIndex = tileIndex % m_Layout[0];

  RegionType out2inRegion = requestedRegion;
  typename RegionType::IndexType regionIndex = out2inRegion.GetIndex();

  // Compute tile offsets
  for(unsigned int i = 0; i < tileXIndex; ++i)
    {
    regionIndex[0]-=m_ColumnsSizes.at(i);
    }

  for(unsigned int i = 0; i < tileYIndex; ++i)
    {
    regionIndex[1]-=m_RowsSizes.at(i);
    }

  out2inRegion.SetIndex(regionIndex);


  // Crop input region
  if(!out2inRegion.Crop(tile->GetLargestPossibleRegion()))
    {
    typename RegionType::IndexType nullIndex;
    nullIndex.Fill(0);
    out2inRegion.SetIndex(nullIndex);

    SizeType nullSize;
    nullSize.Fill(0);
    out2inRegion.SetSize(nullSize);
    }

  return out2inRegion;
}

template <class TImage>
typename TileImageFilter<TImage>
::RegionType
TileImageFilter<TImage>
::InputRegionToOutputRegion(unsigned int tileIndex, const RegionType & requestedRegion)
{
  // Retrieve the nth tile pointer
//  const ImageType * tile = this->GetInput(tileIndex);

  unsigned int tileYIndex = tileIndex / m_Layout[0];
  unsigned int tileXIndex = tileIndex % m_Layout[0];

  RegionType out2inRegion = requestedRegion;
  typename RegionType::IndexType regionIndex = out2inRegion.GetIndex();


  // Compute tile offsets
  for(unsigned int i = 0; i < tileXIndex; ++i)
    {
    regionIndex[0]+=m_ColumnsSizes.at(i);
    }

  for(unsigned int i = 0; i < tileYIndex; ++i)
    {
    regionIndex[1]+=m_RowsSizes.at(i);
    }

  out2inRegion.SetIndex(regionIndex);

  // Crop input region
  if(!out2inRegion.Crop(this->GetOutput()->GetLargestPossibleRegion()))
    {
    SizeType nullSize;
    nullSize.Fill(0);
    out2inRegion.SetSize(nullSize);
    }

  return out2inRegion;
}


} // end namepsace otb

#endif
