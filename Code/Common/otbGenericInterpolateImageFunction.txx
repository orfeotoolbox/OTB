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
#include "otbGenericInterpolateImageFunction.h"
#include "vnl/vnl_math.h"

namespace otb
{

/** Constructor */
template<class TInputImage, class TFunction, class TBoundaryCondition, class TCoordRep>
GenericInterpolateImageFunction<TInputImage, TFunction, TBoundaryCondition, TCoordRep>
::GenericInterpolateImageFunction()
{
  m_Radius[0] = 0;
  m_Radius[1] = 0;
}

/** Destructor */
template<class TInputImage, class TFunction, class TBoundaryCondition, class TCoordRep>
GenericInterpolateImageFunction<TInputImage, TFunction,TBoundaryCondition, TCoordRep>
::~GenericInterpolateImageFunction()
{
}
/** Evaluate at image index position */
template<class TInputImage, class TFunction, class TBoundaryCondition, class TCoordRep>
void
GenericInterpolateImageFunction<TInputImage, TFunction, TBoundaryCondition, TCoordRep>
::SetInputImage(const InputImageType *image)
{
  unsigned int dim;
  
  // Call the parent implementation
  Superclass::SetInputImage(image);
  unsigned int imDim = image->GetImageDimension();
  if( image == NULL )
    {
    return;
    }

  if(m_Radius[0] != 0 && m_Radius[1] != 0)
    {
      // Initialize the neighborhood
      IteratorType it = IteratorType(m_Radius, image, image->GetBufferedRegion());
      
      // Compute the offset tables (we ignore all the zero indices
      // in the neighborhood)
      unsigned int iOffset = 0;
      //int empty = VRadius;
      for(unsigned int iPos = 0; iPos < it.Size(); iPos++)
	{
	  // Get the offset (index)
	  typename IteratorType::OffsetType off = it.GetOffset(iPos);
	  
	  // Check if the offset has zero weights
	  bool nonzero = true;
	  for(dim = 0; dim < imDim; dim++)
	    {
	      if(off[dim] == -m_Radius[dim]) 
		{
		  nonzero = false;
		  break;
		}
	    }
	  
	  // Only use offsets with non-zero indices
	  if(nonzero)
	    {
	      // Set the offset index
	      m_OffsetTable[iOffset] = iPos;
	      
	      // Set the weight table indices
	      for(dim = 0; dim < imDim; dim++)
		{
		  m_WeightOffsetTable[iOffset][dim] = m_Radius[dim] + m_Radius[dim] - 1;
		}
	      
	      // Increment the index
	      iOffset++;
	    }
	}
    }
  else
    {
      itkExceptionMacro(<<"A radius has to be set");
    }
}


/** Evaluate at image index position */
template<class TInputImage, class TFunction, class TBoundaryCondition, class TCoordRep>
typename GenericInterpolateImageFunction<TInputImage, TFunction, TBoundaryCondition, TCoordRep>::OutputType
GenericInterpolateImageFunction<TInputImage, TFunction, TBoundaryCondition, TCoordRep>
::EvaluateAtContinuousIndex(const ContinuousIndexType& index) const
{
  unsigned int dim;
  unsigned int imDim = this->GetInputImage()->GetImageDimension();
  IndexType baseIndex;
  double distance[imDim];
  
  
  // Compute the integer index based on the continuous one by 
  // 'flooring' the index
  for( unsigned int dim = 0; dim < imDim; dim++ )
    {
      // The following "if" block is equivalent to the following line without
      // having to call floor.
      //    baseIndex[dim] = (long) vcl_floor(index[dim] );
      if (index[dim] >= 0.0)
	{
	  baseIndex[dim] = (long) index[dim];
	}
      else
	{
	  long tIndex = (long) index[dim];
	  if (double(tIndex) != index[dim])
	    {
	      tIndex--;
	    }
	  baseIndex[dim] = tIndex;
	}
      distance[dim] = index[dim] - double( baseIndex[dim] );
    }
  
  
  // Position the neighborhood at the index of interest
  
  IteratorType nit = IteratorType( m_Radius, this->GetInputImage(), this->GetInputImage()->GetBufferedRegion());
  nit.SetLocation( baseIndex );
    
  // Compute the sinc function for each dimension
  unsigned int maxRadius = std::max(m_Radius[0], m_Radius[1]);
  double xWeight[imDim][ 2*maxRadius];
  // Fill xWeight with zeros
  for( unsigned int dim = 0; dim < imDim; dim++ )
    {
      for (unsigned j = 0; j< 2*maxRadius; j++)
	{
	  xWeight[dim][j] = 0.;
	}
    }
  
  for( unsigned int dim = 0; dim < imDim; dim++ )
    {
    // x is the offset, hence the parameter of the kernel
      double x = distance[dim] + m_Radius[dim];

    // If distance is zero, i.e. the index falls precisely on the
    // pixel boundary, the weights form a delta function.
    if(distance[dim] == 0.0)
      {
      for( unsigned int i = 0; i < m_WindowSize[dim]; i++)
        {
	  //xWeight[dim][i] = static_cast<double>(i) == m_Radius[dim] - 1 ? 1 : 0;
	  xWeight[dim][i] = i == m_Radius[dim] - 1 ? 1. : 0.;
        }
      }
    else
      {
      // i is the relative offset in dimension dim.
      for( unsigned int i = 0; i < m_WindowSize[dim]; i++)
        {
        // Increment the offset, taking it through the range
        // (dist + rad - 1, ..., dist - rad), i.e. all x
        // such that vcl_abs(x) <= rad
        x -= 1.0;

        // Compute the weight for this m
	xWeight[dim][i] = m_Function(x);
        }
      }
    }

  // Iterate over the neighborhood, taking the correct set
  // of weights in each dimension 
  double xPixelValue = 0.0;
  for(unsigned int j = 0; j < m_OffsetTableSize; j++)
    {
    // Get the offset for this neighbor
    unsigned int off = m_OffsetTable[j];
    
    // Get the intensity value at the pixel
    double xVal = nit.GetPixel(off);

    // Multiply the intensity by each of the weights. Gotta hope
    // that the compiler will unwrap this loop and pipeline this!
    for(dim = 0; dim < imDim; dim++)
      {
      xVal *= xWeight[ dim ][ m_WeightOffsetTable[j][dim] ];
      }

    // Increment the pixel value
    xPixelValue += xVal;
    }
  
  // Return the interpolated value
  return static_cast<OutputType>(xPixelValue);
}


template<class TInputImage, class TFunction, class TBoundaryCondition, class TCoordRep>
void
GenericInterpolateImageFunction<TInputImage, TFunction, TBoundaryCondition, TCoordRep>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{ 
  Superclass::PrintSelf( os, indent ); 
}

}//namespace otb
