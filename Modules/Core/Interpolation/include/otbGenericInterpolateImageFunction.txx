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
#ifndef __otbGenericInterpolateImageFunction_txx
#define __otbGenericInterpolateImageFunction_txx
#include "otbGenericInterpolateImageFunction.h"
#include "vnl/vnl_math.h"

namespace otb
{

/** Constructor */
template<class TInputImage, class TFunction, class TBoundaryCondition, class TCoordRep>
GenericInterpolateImageFunction<TInputImage, TFunction, TBoundaryCondition, TCoordRep>
::GenericInterpolateImageFunction()
{
  m_WindowSize = 1;
  this->SetRadius(1);
  m_OffsetTable = NULL;
  m_WeightOffsetTable = NULL;
  m_TablesHaveBeenGenerated = false;
  m_NormalizeWeight =  false;
}

/** Destructor */
template<class TInputImage, class TFunction, class TBoundaryCondition, class TCoordRep>
GenericInterpolateImageFunction<TInputImage, TFunction, TBoundaryCondition, TCoordRep>
::~GenericInterpolateImageFunction()
{
  this->ResetOffsetTable();
}

/** Delete every tables. */
template<class TInputImage, class TFunction, class TBoundaryCondition, class TCoordRep>
void
GenericInterpolateImageFunction<TInputImage, TFunction, TBoundaryCondition, TCoordRep>
::ResetOffsetTable()
{
  // Clear the offset table
  if (m_OffsetTable != NULL)
    {
    delete[] m_OffsetTable;
    m_OffsetTable = NULL;
    }

  // Clear the weights tales
  if (m_WeightOffsetTable != NULL)
    {
    for (unsigned int i = 0; i < m_OffsetTableSize; ++i)
      {
      delete[] m_WeightOffsetTable[i];
      }
    delete[] m_WeightOffsetTable;
    m_WeightOffsetTable = NULL;
    }
}

template<class TInputImage, class TFunction, class TBoundaryCondition, class TCoordRep>
void
GenericInterpolateImageFunction<TInputImage, TFunction, TBoundaryCondition, TCoordRep>
::SetRadius(unsigned int rad)
{
  //m_Radius = rad;
  this->GetFunction().SetRadius(rad);
  m_WindowSize = rad << 1;
  this->Modified();
}

template<class TInputImage, class TFunction, class TBoundaryCondition, class TCoordRep>
void
GenericInterpolateImageFunction<TInputImage, TFunction, TBoundaryCondition, TCoordRep>
::Modified()
{
  Superclass::Modified();
  m_TablesHaveBeenGenerated = false;

}

/** Initialize used tables*/
template<class TInputImage, class TFunction, class TBoundaryCondition, class TCoordRep>
void
GenericInterpolateImageFunction<TInputImage, TFunction, TBoundaryCondition, TCoordRep>
::InitializeTables()
{
  // Compute the offset table size
  m_OffsetTableSize = 1;
  for (unsigned dim = 0; dim < ImageDimension; ++dim)
    {
    m_OffsetTableSize *= m_WindowSize;
    }

  // Allocate the offset table
  m_OffsetTable = new unsigned int[m_OffsetTableSize];

  // Allocate the weights tables
  m_WeightOffsetTable = new unsigned int *[m_OffsetTableSize];
  for (unsigned int i = 0; i < m_OffsetTableSize; ++i)
    {
    m_WeightOffsetTable[i] = new unsigned int[ImageDimension];
    }
}

/** Fill the weight offset table*/
template<class TInputImage, class TFunction, class TBoundaryCondition, class TCoordRep>
void
GenericInterpolateImageFunction<TInputImage, TFunction, TBoundaryCondition, TCoordRep>
::FillWeightOffsetTable()
{
  // Initialize the neighborhood
  SizeType radius;
  radius.Fill(this->GetRadius());
  if (this->GetInputImage() != NULL)
    {
    IteratorType it = IteratorType(radius,  this->GetInputImage(), this->GetInputImage()->GetBufferedRegion());
    // Compute the offset tables (we ignore all the zero indices
    // in the neighborhood)
    unsigned int iOffset = 0;
    int          empty = static_cast<int>(this->GetRadius());

    for (unsigned int iPos = 0; iPos < it.Size(); ++iPos)
      {
      // Get the offset (index)
      typename IteratorType::OffsetType off = it.GetOffset(iPos);

      // Check if the offset has zero weights
      bool nonzero = true;
      for (unsigned int dim = 0; dim < ImageDimension; ++dim)
        {
        if (off[dim] == -empty)
          {
          nonzero = false;
          break;
          }
        }
      // Only use offsets with non-zero indices
      if (nonzero)
        {
        // Set the offset index
        m_OffsetTable[iOffset] = iPos;

        // Set the weight table indices
        for (unsigned int dim = 0; dim < ImageDimension; ++dim)
          {
          m_WeightOffsetTable[iOffset][dim] = off[dim] + this->GetRadius() - 1;
          }
        // Increment the index
        iOffset++;
        }
      }
    }
  else
    {
    itkExceptionMacro(<< "An input has to be set");
    }
}

/** Initialize tables: need to be call explicitely */
template<class TInputImage, class TFunction, class TBoundaryCondition, class TCoordRep>
void
GenericInterpolateImageFunction<TInputImage, TFunction, TBoundaryCondition, TCoordRep>
::Initialize()
{
  // Delete existing tables
  this->ResetOffsetTable();
  // Tables initialization
  this->InitializeTables();
  // fill the weigth table
  this->FillWeightOffsetTable();
  m_TablesHaveBeenGenerated = true;
}

/** Evaluate at image index position */
template<class TInputImage, class TFunction, class TBoundaryCondition, class TCoordRep>
typename GenericInterpolateImageFunction<TInputImage, TFunction, TBoundaryCondition, TCoordRep>::OutputType
GenericInterpolateImageFunction<TInputImage, TFunction, TBoundaryCondition, TCoordRep>
::EvaluateAtContinuousIndex(const ContinuousIndexType& index) const
{
  if (!m_TablesHaveBeenGenerated)
    {
    itkExceptionMacro(<< "The Interpolation functor need to be explicitly intanciated with the method Initialize()");
    }

  //unsigned int dim;
  IndexType baseIndex;
  double    distance[ImageDimension];

  // Compute the integer index based on the continuous one by
  // 'flooring' the index
  for (unsigned int dim = 0; dim < ImageDimension; ++dim)
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
    distance[dim] = index[dim] - double(baseIndex[dim]);
    }

  // Position the neighborhood at the index of interest
  SizeType radius;
  radius.Fill(this->GetRadius());
  IteratorType nit = IteratorType(radius, this->GetInputImage(), this->GetInputImage()->GetBufferedRegion());
  nit.SetLocation(baseIndex);

  const unsigned int twiceRadius = static_cast<const unsigned int>(2 * this->GetRadius());
  /*  double xWeight[ImageDimension][ twiceRadius]; */
  std::vector<std::vector<double> > xWeight;
  xWeight.resize(ImageDimension);
  for (unsigned int cpt = 0; cpt < xWeight.size(); ++cpt)
    {
    xWeight[cpt].resize(twiceRadius);
    }

  for (unsigned int dim = 0; dim < ImageDimension; ++dim)
    {
    // x is the offset, hence the parameter of the kernel
    double x = distance[dim] + this->GetRadius();

    // If distance is zero, i.e. the index falls precisely on the
    // pixel boundary, the weights form a delta function.
    /*
    if(distance[dim] == 0.0)
    {
    for( unsigned int i = 0; i < m_WindowSize; ++i)
      {
    xWeight[dim][i] = static_cast<int>(i) == (static_cast<int>(this->GetRadius()) - 1) ? 1. : 0.;
      }
    }
    else
    {
    */
    // i is the relative offset in dimension dim.
    for (unsigned int i = 0; i < m_WindowSize; ++i)
      {
      // Increment the offset, taking it through the range
      // (dist + rad - 1, ..., dist - rad), i.e. all x
      // such that vcl_abs(x) <= rad
      x -= 1.0;
      // Compute the weight for this m
      xWeight[dim][i] = m_Function(x);
      }
    //}
    }
  if (m_NormalizeWeight == true)
    {
    for (unsigned int dim = 0; dim < ImageDimension; ++dim)
      {
      double sum = 0.;
      // Compute the weights sum
      for (unsigned int i = 0; i < m_WindowSize; ++i)
        {
        sum += xWeight[dim][i];
        }
      if (sum != 1.)
        {
        // Normalize the weights
        for (unsigned int i = 0; i < m_WindowSize; ++i)
          {
          xWeight[dim][i] =  xWeight[dim][i] / sum;
          }
        }
      }
    }

  // Iterate over the neighborhood, taking the correct set
  // of weights in each dimension
  RealType xPixelValue;
  itk::NumericTraits<RealType>::SetLength(xPixelValue, this->GetInputImage()->GetNumberOfComponentsPerPixel());
  xPixelValue=static_cast<RealType>(0.0);

  for (unsigned int j = 0; j < m_OffsetTableSize; ++j)
    {
    // Get the offset for this neighbor
    unsigned int off = m_OffsetTable[j];

    // Get the intensity value at the pixel
    RealType xVal = nit.GetPixel(off);

    // Multiply the intensity by each of the weights. Gotta hope
    // that the compiler will unwrap this loop and pipeline this!
    for (unsigned int dim = 0; dim < ImageDimension; ++dim)
      {
      xVal *= xWeight[dim][m_WeightOffsetTable[j][dim]];
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
  Superclass::PrintSelf(os, indent);
}

} //namespace otb

#endif
