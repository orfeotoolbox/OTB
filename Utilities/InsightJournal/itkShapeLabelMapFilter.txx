/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkShapeLabelMapFilter.txx,v $
  Language:  C++
  Date:      $Date: 2005/08/23 15:09:03 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkShapeLabelMapFilter_txx
#define __itkShapeLabelMapFilter_txx

#include "itkShapeLabelMapFilter.h"
#include "itkProgressReporter.h"
#include "itkNeighborhoodIterator.h"
#include "itkLabelMapToLabelImageFilter.h"
#include "itkConstantBoundaryCondition.h"
#include "vnl/algo/vnl_real_eigensystem.h"
#include "vnl/algo/vnl_symmetric_eigensystem.h"


namespace itk {

template <class TImage, class TLabelImage>
ShapeLabelMapFilter<TImage, TLabelImage>
::ShapeLabelMapFilter()
{
  m_ComputeFeretDiameter = false;
  m_ComputePerimeter = false;
}


template<class TImage, class TLabelImage>
void
ShapeLabelMapFilter<TImage, TLabelImage>
::BeforeThreadedGenerateData()
{
  Superclass::BeforeThreadedGenerateData();

  // generate the label image, if needed
  if( m_ComputeFeretDiameter || m_ComputePerimeter )
    {
    if( !m_LabelImage )
      {
      // generate an image of the labelized image
      typedef LabelMapToLabelImageFilter< TImage, LabelImageType > LCI2IType;
      typename LCI2IType::Pointer lci2i = LCI2IType::New();
      lci2i->SetInput( this->GetOutput() );
      // respect the number of threads of the filter
      lci2i->SetNumberOfThreads( this->GetNumberOfThreads() );
      lci2i->Update();
      m_LabelImage = lci2i->GetOutput();
      }
    }

  // delegate the computation of the perimeter to a dedicated calculator
  if( m_ComputePerimeter )
    {
    m_PerimeterCalculator = PerimeterCalculatorType::New();
    m_PerimeterCalculator->SetImage( m_LabelImage );
//     m_PerimeterCalculator->SetNumberOfThreads( this->GetNumberOfThreads() );
    m_PerimeterCalculator->Compute();
    }

}


template<class TImage, class TLabelImage>
void
ShapeLabelMapFilter<TImage, TLabelImage>
::ThreadedGenerateData( LabelObjectType * labelObject )
{
  ImageType * output = this->GetOutput();
  const LabelPixelType & label = labelObject->GetLabel();

  // TODO: compute sizePerPixel, borderMin and borderMax in BeforeThreadedGenerateData() ?

  // compute the size per pixel, to be used later
  double sizePerPixel = 1;
  for( int i=0; i<ImageDimension; i++ )
    {
    sizePerPixel *= output->GetSpacing()[i];
    }
  
  typename std::vector< double > sizePerPixelPerDimension;
  for( int i=0; i<ImageDimension; i++ )
    {
    sizePerPixelPerDimension.push_back( sizePerPixel / output->GetSpacing()[i] );
    }
  
  // compute the max the index on the border of the image
  IndexType borderMin = output->GetLargestPossibleRegion().GetIndex();
  IndexType borderMax = borderMin;
  for( int i=0; i<ImageDimension; i++ )
    {
    borderMax[i] += borderMin[i] + output->GetLargestPossibleRegion().GetSize()[i] - 1;
    }

  // init the vars
  unsigned long size = 0;
  ContinuousIndex< double, ImageDimension> centroid;
  centroid.Fill( 0 );
  IndexType mins;
  mins.Fill( NumericTraits< long >::max() );
  IndexType maxs;
  maxs.Fill( NumericTraits< long >::NonpositiveMin() );
  unsigned long sizeOnBorder = 0;
  double physicalSizeOnBorder = 0;
  MatrixType centralMoments;
  centralMoments.Fill( 0 );

  typename LabelObjectType::LineContainerType::const_iterator lit;
  typename LabelObjectType::LineContainerType & lineContainer = labelObject->GetLineContainer();

  // iterate over all the lines
  for( lit = lineContainer.begin(); lit != lineContainer.end(); lit++ )
    {
    const IndexType & idx = lit->GetIndex();
    unsigned long length = lit->GetLength();

    // update the size
    size += length;

    // update the centroid - and report the progress
    // first, update the axes which are not 0
    for( int i=1; i<ImageDimension; i++ )
      {
      centroid[i] += length * idx[i];
      }
    // then, update the axis 0
    centroid[0] += idx[0] * length + ( length * ( length - 1 ) ) / 2.0;

    // update the mins and maxs
    for( int i=0; i<ImageDimension; i++)
      {
      if( idx[i] < mins[i] )
        {
        mins[i] = idx[i];
        }
      if( idx[i] > maxs[i] )
        {
        maxs[i] = idx[i];
        }
      }
    // must fix the max for the axis 0
    if( idx[0] + (long)length > maxs[0] )
      {
      maxs[0] = idx[0] + length - 1;
      }

    // object is on a border ?
    bool isOnBorder = false;
    for( int i=1; i<ImageDimension; i++)
      {
      if( idx[i] == borderMin[i] || idx[i] == borderMax[i])
        {
        isOnBorder = true;
        break;
        }
      }
    if( isOnBorder )
      {
      // the line touch a border on a dimension other than 0, so
      // all the line touch a border
      sizeOnBorder += length;
      }
    else
      {
      // we must check for the dimension 0
      bool isOnBorder0 = false;
      if( idx[0] == borderMin[0] )
        {
        // one more pixel on the border
        sizeOnBorder++;
        isOnBorder0 = true;
        }
      if( !isOnBorder0 || length > 1 )
        {
        // we can check for the end of the line
        if( idx[0] + (long)length - 1 == borderMax[0] )
          {
          // one more pixel on the border
          sizeOnBorder++;
          }
        }
      }
      
    // physical size on border
    // first, the dimension 0
    if( idx[0] == borderMin[0] )
      {
      // the begining of the line
      physicalSizeOnBorder += sizePerPixelPerDimension[0];
      }
    if( idx[0] + (long)length - 1 == borderMax[0] )
      {
      // and the end of the line
      physicalSizeOnBorder += sizePerPixelPerDimension[0];
      }
    // then the other dimensions
    for( int i=1; i<ImageDimension; i++ )
      {
      if( idx[i] == borderMin[i] )
        {
        // one border
        physicalSizeOnBorder += sizePerPixelPerDimension[i] * length;
        }
      if( idx[i] == borderMax[i] )
        {
        // and the other
        physicalSizeOnBorder += sizePerPixelPerDimension[i] * length;
        }
      }
    
    // moments computation
// ****************************************************************
// that commented code is the basic implementation. The next peace of code
// give the same result in a much efficient way, by using expended formulae
// allowed by the binary case instead of loops.
// ****************************************************************
//     long endIdx0 = idx[0] + length;
//     for( IndexType iidx = idx; iidx[0]<endIdx0; iidx[0]++)
//       {
//       typename LabelObjectType::CentroidType pP;
//       output->TransformIndexToPhysicalPoint(iidx, pP);
// 
//       for(unsigned int i=0; i<ImageDimension; i++)
//         {
//         for(unsigned int j=0; j<ImageDimension; j++)
//           {
//           centralMoments[i][j] += pP[i] * pP[j];
//           }
//         }
//       }
    // get the physical position and the spacing - they are used several times later
    typename LabelObjectType::CentroidType physicalPosition;
    output->TransformIndexToPhysicalPoint( idx, physicalPosition );
    const typename ImageType::SpacingType & spacing = output->GetSpacing();
    // the sum of x positions, also reused several times
    double sumX = length * ( physicalPosition[0] + ( spacing[0] * ( length - 1 ) ) / 2.0 );
    // the real job - the sum of square of x positions
    // that's the central moments for dims 0, 0
    centralMoments[0][0] += length * ( physicalPosition[0] * physicalPosition[0]
            + spacing[0] * ( length - 1 ) * ( ( spacing[0] * ( 2 * length - 1 ) ) / 6.0 + physicalPosition[0] ) );
    // the other ones
    for( int i=1; i<ImageDimension; i++ )
      {
      // do this one here to avoid the double assigment in the following loop
      // when i == j
      centralMoments[i][i] += length * physicalPosition[i] * physicalPosition[i];
     // central moments are symetrics, so avoid to compute them 2 times
      for( int j=i+1; j<ImageDimension; j++ )
        {
        // note that we won't use that code if the image dimension is less than 3
        // --> the tests should be in 3D at least
        double cm = length * physicalPosition[i] * physicalPosition[j];
        centralMoments[i][j] += cm;
        centralMoments[j][i] += cm;
        }
      // the last moments: the ones for the dimension 0
      double cm = sumX * physicalPosition[i];
      centralMoments[i][0] += cm;
      centralMoments[0][i] += cm;
      }

    }

  // final computation
  typename LabelObjectType::RegionType::SizeType regionSize;
  double minSize = NumericTraits< double >::max();
  double maxSize = NumericTraits< double >::NonpositiveMin();
  for( int i=0; i<ImageDimension; i++ )
    {
    centroid[i] /= size;
    regionSize[i] = maxs[i] - mins[i] + 1;
    double s = regionSize[i] * output->GetSpacing()[i];
    minSize = std::min( s, minSize );
    maxSize = std::max( s, maxSize );
    for(unsigned int j=0; j<ImageDimension; j++)
      {
      centralMoments[i][j] /= size;
      }
    }
  typename LabelObjectType::RegionType region( mins, regionSize );
  typename LabelObjectType::CentroidType physicalCentroid;
  output->TransformContinuousIndexToPhysicalPoint( centroid, physicalCentroid );

  // Center the second order moments
  for(unsigned int i=0; i<ImageDimension; i++)
    {
    for(unsigned int j=0; j<ImageDimension; j++)
      {
      centralMoments[i][j] -= physicalCentroid[i] * physicalCentroid[j];
      }
    }

  // Compute principal moments and axes
  VectorType principalMoments;
  vnl_symmetric_eigensystem<double> eigen( centralMoments.GetVnlMatrix() );
  vnl_diag_matrix<double> pm = eigen.D;
  for(unsigned int i=0; i<ImageDimension; i++)
    {
//    principalMoments[i] = 4 * vcl_sqrt( pm(i,i) );
    principalMoments[i] = pm(i,i);
    }
  MatrixType principalAxes = eigen.V.transpose();

  // Add a final reflection if needed for a proper rotation,
  // by multiplying the last row by the determinant
  vnl_real_eigensystem eigenrot( principalAxes.GetVnlMatrix() );
  vnl_diag_matrix< vcl_complex<double> > eigenval = eigenrot.D;
  vcl_complex<double> det( 1.0, 0.0 );

  for(unsigned int i=0; i<ImageDimension; i++)
    {
    det *= eigenval( i, i );
    }

  for(unsigned int i=0; i<ImageDimension; i++)
    {
    principalAxes[ ImageDimension-1 ][i] *= std::real( det );
    }

  double elongation = 0;
  if( principalMoments[0] != 0 )
    {
//    elongation = principalMoments[ImageDimension-1] / principalMoments[0];
    elongation = vcl_sqrt(principalMoments[ImageDimension-1] / principalMoments[0]);
    }

  double physicalSize = size * sizePerPixel;
  double equivalentRadius = hyperSphereRadiusFromVolume( physicalSize );
  double equivalentPerimeter = hyperSpherePerimeter( equivalentRadius );

  // compute equilalent ellipsoid radius
  VectorType ellipsoidSize;
  double edet = 1.0;
  for(unsigned int i=0; i<ImageDimension; i++)
    {
    edet *= principalMoments[i];
    }
  edet = vcl_pow( edet, 1.0/ImageDimension );
  for(unsigned int i=0; i<ImageDimension; i++)
    {
    ellipsoidSize[i] = 2.0 * equivalentRadius * vcl_sqrt( principalMoments[i] / edet );
    }

  // set the values in the object
  labelObject->SetSize( size );
  labelObject->SetPhysicalSize( physicalSize );
  labelObject->SetRegion( region );
  labelObject->SetCentroid( physicalCentroid );
  labelObject->SetRegionElongation( maxSize / minSize );
  labelObject->SetSizeRegionRatio( size / (double)region.GetNumberOfPixels() );
  labelObject->SetSizeOnBorder( sizeOnBorder );
  labelObject->SetPhysicalSizeOnBorder( physicalSizeOnBorder );
  labelObject->SetBinaryPrincipalMoments( principalMoments );
  labelObject->SetBinaryPrincipalAxes( principalAxes );
  labelObject->SetBinaryElongation( elongation );
  labelObject->SetEquivalentRadius( equivalentRadius );
  labelObject->SetEquivalentPerimeter( equivalentPerimeter );
  labelObject->SetEquivalentEllipsoidSize( ellipsoidSize );

  if( m_ComputeFeretDiameter )
    {
    const PixelType & label = labelObject->GetLabel();

    // init the vars
    unsigned long size = 0;
    typedef typename std::deque< IndexType > IndexListType;
    IndexListType idxList;
    
    // the iterators
    typename LabelObjectType::LineContainerType::const_iterator lit;
    typename LabelObjectType::LineContainerType & lineContainer = labelObject->GetLineContainer();

    typedef typename itk::ConstNeighborhoodIterator< LabelImageType > NeighborIteratorType;
    SizeType neighborHoodRadius;
    neighborHoodRadius.Fill( 1 );
    NeighborIteratorType it( neighborHoodRadius, m_LabelImage, m_LabelImage->GetBufferedRegion() );
    ConstantBoundaryCondition<LabelImageType> lcbc;
    // use label + 1 to have a label different of the current label on the border
    lcbc.SetConstant( label + 1 );
    it.OverrideBoundaryCondition( &lcbc );
    it.GoToBegin();

    // iterate over all the lines
    for( lit = lineContainer.begin(); lit != lineContainer.end(); lit++ )
      {
      const IndexType & firstIdx = lit->GetIndex();
      unsigned long length = lit->GetLength();

      long endIdx0 = firstIdx[0] + length;
      for( IndexType idx = firstIdx; idx[0]<endIdx0; idx[0]++)
        {

        // move the iterator to the new location
        it += idx - it.GetIndex();

        // push the pixel in the list if it is on the border of the object
        for (unsigned i = 0; i < it.Size(); i++)
          {
          if( it.GetPixel(i) != label )
            {
            idxList.push_back( idx );
            size++;
            break;
            }
          }
        }
      }

    // we can now search the feret diameter
    double feretDiameter = 0;
    for( typename IndexListType::const_iterator iIt1 = idxList.begin();
      iIt1 != idxList.end();
      iIt1++ )
      {
      typename IndexListType::const_iterator iIt2 = iIt1;
      for( iIt2++; iIt2 != idxList.end(); iIt2++ )
        {
        // Compute the length between the 2 indexes
        double length = 0;
        for( int i=0; i<ImageDimension; i++ )
          {
          length += vcl_pow( ( iIt1->operator[]( i ) - iIt2->operator[]( i ) ) * output->GetSpacing()[i], 2 );
          }
        if( feretDiameter < length )
          {
          feretDiameter = length;
          }
        }
      }
    // final computation
    feretDiameter = vcl_sqrt( feretDiameter );

    // finally put the values in the label object
    labelObject->SetFeretDiameter( feretDiameter );

    }


  // be sure tha the calculator has the perimeter estimation for that label.
  // The calculator may not have the label if the object is only on a border.
  // It will occurre for sure when processing a 2D image with a 3D filter.
  if( m_ComputePerimeter && m_PerimeterCalculator->HasLabel( label ) )
    {
    double perimeter = m_PerimeterCalculator->GetPerimeter( label );
    labelObject->SetPerimeter( perimeter );
    labelObject->SetRoundness( equivalentPerimeter / perimeter );
    }

//   std::cout << std::endl;
//   labelObject->Print( std::cout );
//   std::cout << std::endl;

}


template<class TImage, class TLabelImage>
void
ShapeLabelMapFilter<TImage, TLabelImage>
::AfterThreadedGenerateData()
{
  Superclass::AfterThreadedGenerateData();

  // release the label image
  m_LabelImage = NULL;
  // and the perimeter calculator
  m_PerimeterCalculator = NULL;
}


template<class TImage, class TLabelImage>
void
ShapeLabelMapFilter<TImage, TLabelImage>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  
  os << indent << "ComputeFeretDiameter: " << m_ComputeFeretDiameter << std::endl;
  os << indent << "ComputePerimeter: " << m_ComputePerimeter << std::endl;
}


template<class TImage, class TLabelImage>
long
ShapeLabelMapFilter<TImage, TLabelImage>
::factorial( long n )
{
  if( n < 1 )
    {
    return 1;
    }
  return n * factorial( n - 1 );
}


template<class TImage, class TLabelImage>
long
ShapeLabelMapFilter<TImage, TLabelImage>
::doubleFactorial( long n )
{
  if( n < 2 )
    {
    return 1;
    }
  return n * doubleFactorial( n - 2 );
}


template<class TImage, class TLabelImage>
double
ShapeLabelMapFilter<TImage, TLabelImage>
::gammaN2p1( long n )
{
  bool even = n % 2 == 0;
  if( even )
    {
    return factorial( n / 2 );
    }
  else
    {
    return  vcl_sqrt( PI ) * doubleFactorial( n ) / vcl_pow( 2, ( n + 1 ) / 2.0 );
    }
}


template<class TImage, class TLabelImage>
double
ShapeLabelMapFilter<TImage, TLabelImage>
::hyperSphereVolume( double radius )
{
  return vcl_pow( PI, ImageDimension / 2.0 ) * vcl_pow( radius, ImageDimension ) / gammaN2p1( ImageDimension );
}


template<class TImage, class TLabelImage>
double
ShapeLabelMapFilter<TImage, TLabelImage>
::hyperSpherePerimeter( double radius )
{
  return ImageDimension * hyperSphereVolume( radius ) / radius;
}


template<class TImage, class TLabelImage>
double
ShapeLabelMapFilter<TImage, TLabelImage>
::hyperSphereRadiusFromVolume( double volume )
{
  return vcl_pow( volume * gammaN2p1( ImageDimension ) / vcl_pow( PI, ImageDimension / 2.0 ), 1.0 / ImageDimension );
}




}// end namespace itk
#endif
