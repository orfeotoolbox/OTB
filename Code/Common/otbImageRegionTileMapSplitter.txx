
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
#ifndef __otbImageRegionTileMapSplitter_txx
#define __otbImageRegionTileMapSplitter_txx
#include "otbImageRegionTileMapSplitter.h"
#include <math.h>

namespace otb
{

/**
   *
 */
  template <unsigned int VImageDimension>
      unsigned int 
      ImageRegionTileMapSplitter<VImageDimension>
  ::GetNumberOfSplits(const RegionType &region, unsigned int requestedNumber)
  {
    int splitAxis;
    const SizeType &regionSize = region.GetSize();

  // split on the outermost dimension available
    splitAxis = VImageDimension - 1;
    while (regionSize[splitAxis] == 1)
    {
      --splitAxis;
      if (splitAxis < 0)
      { // cannot split
        itkDebugMacro("  Cannot Split");
        return 1;
      }
    }

  // determine the actual number of pieces that will be generated
    SizeValueType range = regionSize[splitAxis];
    int valuesPerPiece = ((int)::ceil(range/((double)requestedNumber*256))) * 256;
    int maxPieceUsed = (int)::ceil(range/(double)valuesPerPiece);

    return maxPieceUsed + 1;
  }

  
/**
   *
 */
  template <unsigned int VImageDimension>
      itk::ImageRegion<VImageDimension>
      ImageRegionTileMapSplitter<VImageDimension>
  ::GetSplit(unsigned int i, unsigned int numberOfPieces, const RegionType &region)
      {
        int splitAxis;
        RegionType splitRegion;
        IndexType splitIndex;
        SizeType splitSize, regionSize;
  
  // Initialize the splitRegion to the requested region
        splitRegion = region;
        splitIndex = splitRegion.GetIndex();
        splitSize = splitRegion.GetSize();

        regionSize = region.GetSize();
  
  // split on the outermost dimension available
        splitAxis = VImageDimension - 1;
        while (regionSize[splitAxis] == 1)
        {
          --splitAxis;
          if (splitAxis < 0)
          { // cannot split
            itkDebugMacro("  Cannot Split");
            return splitRegion;
          }
        }

  // determine the actual number of pieces that will be generated
        SizeValueType range = regionSize[splitAxis];
        int valuesPerPiece = ((int)::ceil(range/((double)numberOfPieces*256))) * 256;
        int maxPieceUsed = (int)::ceil(range/(double)valuesPerPiece);

  // Split the region
        if ((int) i == maxPieceUsed-1)
        {
          splitIndex[splitAxis] += i*valuesPerPiece;
    // last piece needs to process the "rest" dimension being split
//     splitSize[splitAxis] = splitSize[splitAxis] - i*valuesPerPiece;
          splitSize[splitAxis] = regionSize[splitAxis] - splitIndex[splitAxis];
        }
        if (((int) i == 0) && ((int) i != maxPieceUsed-1))
        {
    //First piece may not contain a whole piece
    //splitIndex stay at splitRegion.GetIndex();
          splitSize[splitAxis] = (((int)::floor(splitIndex[splitAxis]/valuesPerPiece))+1)
              *valuesPerPiece - splitIndex[splitAxis];
        }
        if ((int) i < maxPieceUsed-1)
        {
          splitIndex[splitAxis] += i*valuesPerPiece;
          splitSize[splitAxis] = valuesPerPiece;
        }

               
  // set the split region ivars
        splitRegion.SetIndex( splitIndex );
        splitRegion.SetSize( splitSize );
               
               
        itkDebugMacro("  Split Piece: " << splitRegion );

        return splitRegion;
      }
  
  

/**
       *
 */
      template <unsigned int VImageDimension>
          void 
          ImageRegionTileMapSplitter<VImageDimension>
  ::PrintSelf(std::ostream& os, itk::Indent indent) const
      {
        Superclass::PrintSelf(os,indent);
      }


} // end namespace itk

#endif
