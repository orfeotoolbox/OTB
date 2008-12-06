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
#ifndef __otbMirrorBoundaryCondition_txx
#define __otbMirrorBoundaryCondition_txx
#include "itkConstNeighborhoodIterator.h"
#include "otbMirrorBoundaryCondition.h"
namespace otb
{
template<class TImage>
typename MirrorBoundaryCondition<TImage>::PixelType
MirrorBoundaryCondition<TImage>
::operator()(const OffsetType& point_index, const OffsetType& boundary_offset,
             const NeighborhoodType *data) const
{
  typedef typename OffsetType::OffsetValueType OffsetValueType;
  const itk::ConstNeighborhoodIterator<TImage> * iterator
    = dynamic_cast<const itk::ConstNeighborhoodIterator<TImage> *>(data);
  typename TImage::InternalPixelType *ptr;
  int linear_index = 0;
  unsigned int i;

  // Find the pointer of the closest boundary pixel

  // Return the value of the pixel at the closest boundary point.
  for (i = 0; i < ImageDimension; ++i)
    {
    linear_index += (point_index[i] + boundary_offset[i]) * data->GetStride(i);
    }
  ptr =data->operator[](linear_index);

  // Wrap the pointer around the image in the necessary dimensions.  If we have
  // reached this point, we can assume that we are on the edge of the BUFFERED
  // region of the image.  Boundary conditions are only invoked if touching the
  // actual memory boundary.

  // These are the step sizes for increments in each dimension of the image.
  const typename TImage::OffsetValueType * offset_table
    = iterator->GetImagePointer()->GetOffsetTable();


  for (i = 0; i < ImageDimension; ++i)
    {
    if (boundary_offset[i] != 0)
      {
	// If the neighborhood overlaps on the low edge, then wrap from the
	// high edge of the image.
	if (point_index[i] < static_cast<OffsetValueType>(iterator->GetRadius(i)))
	  {
	    ptr +=  boundary_offset[i] * offset_table[i];
	  }
	else // wrap from the low side of the image
	  {
	    ptr -= boundary_offset[i] * offset_table[i];
	  }
      }
    }

  return *reinterpret_cast<PixelType *>(ptr);
}

template<class TImage>
typename MirrorBoundaryCondition<TImage>::PixelType
MirrorBoundaryCondition<TImage>
::operator()(const OffsetType& point_index, const OffsetType& boundary_offset,
             const NeighborhoodType *data,
             const NeighborhoodAccessorFunctorType &neighborhoodAccessorFunctor) const
{
  typedef typename OffsetType::OffsetValueType OffsetValueType;
  const itk::ConstNeighborhoodIterator<TImage> * iterator
    = dynamic_cast<const itk::ConstNeighborhoodIterator<TImage> *>(data);
  typename TImage::InternalPixelType *ptr;
  int linear_index = 0;
  unsigned int i;

  // Find the pointer of the closest boundary pixel
  //  std::cout << "Boundary offset = " << boundary_offset << std::endl;
  // std::cout << "point index = " << point_index << std::endl;


  // Return the value of the pixel at the closest boundary point.
  for (i = 0; i < ImageDimension; ++i)
    {
    linear_index += (point_index[i] + boundary_offset[i]) * data->GetStride(i);
    }
  ptr = data->operator[](linear_index);

  // Wrap the pointer around the image in the necessary dimensions.  If we have
  // reached this point, we can assume that we are on the edge of the BUFFERED
  // region of the image.  Boundary conditions are only invoked if touching the
  // actual memory boundary.

  // These are the step sizes for increments in each dimension of the image.
  const typename TImage::OffsetValueType * offset_table
    = iterator->GetImagePointer()->GetOffsetTable();


  for (i = 0; i < ImageDimension; ++i)
    {
    if (boundary_offset[i] != 0)
      {
	// If the neighborhood overlaps on the low edge, then wrap from the
	// high edge of the image.
	if (point_index[i] < static_cast<OffsetValueType>(iterator->GetRadius(i)))
	  {
	    ptr +=  boundary_offset[i] * offset_table[i];
	  }
	else // wrap from the low side of the image
	  {
	    ptr -= boundary_offset[i] * offset_table[i];
	  }
      }
    }

  return neighborhoodAccessorFunctor.Get(ptr);
}
} // end namespace otb

#endif
