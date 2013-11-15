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
#ifndef __otbShapeAttributesLabelMapFilter_txx
#define __otbShapeAttributesLabelMapFilter_txx

#include "otbShapeAttributesLabelMapFilter.h"
#include "itkProgressReporter.h"
#include "itkNeighborhoodIterator.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkConstShapedNeighborhoodIterator.h"
#include "itkLabelMapToLabelImageFilter.h"
#include "itkConstantBoundaryCondition.h"
#include "itkGeometryUtilities.h"
#include "itkConnectedComponentAlgorithm.h"
#include "vnl/algo/vnl_real_eigensystem.h"
#include "vnl/algo/vnl_symmetric_eigensystem.h"

#include "otbMacro.h"
#include <deque>

namespace otb {

namespace Functor {

template <class TLabelObject, class TLabelImage>
ShapeAttributesLabelObjectFunctor<TLabelObject, TLabelImage>
::ShapeAttributesLabelObjectFunctor() : m_ComputeFeretDiameter(false),
  m_ComputePerimeter(false),
  m_ComputeFlusser(true),
  m_ComputePolygon(true),
  m_ReducedAttributeSet(true),
  m_LabelImage(NULL)
{}

/** The comparator (!=) */
template <class TLabelObject, class TLabelImage>
bool
ShapeAttributesLabelObjectFunctor<TLabelObject, TLabelImage>
::operator != (const Self &self)
  {
  // Initialize response
  bool resp = true;

  // Check for differences
  resp = resp && (m_ComputeFeretDiameter != self.m_ComputeFeretDiameter);
  resp = resp && (m_ComputePerimeter != self.m_ComputePerimeter);
  resp = resp && (m_ReducedAttributeSet != self.m_ReducedAttributeSet);
  resp = resp && (m_LabelImage != self.m_LabelImage);

  // Return
  return resp;
  }

/** The comparator (==)*/
template <class TLabelObject, class TLabelImage>
bool
ShapeAttributesLabelObjectFunctor<TLabelObject, TLabelImage>
::operator == (const Self &self)
  {
  // Call the != implementation
  return !(this != self);
  }

/** Set the compute perimeter flag */
template <class TLabelObject, class TLabelImage>
void
ShapeAttributesLabelObjectFunctor<TLabelObject, TLabelImage>
::SetComputePerimeter(bool flag)
{
  m_ComputePerimeter = flag;
}

/** Get the compute perimeter flag */
template <class TLabelObject, class TLabelImage>
bool
ShapeAttributesLabelObjectFunctor<TLabelObject, TLabelImage>
::GetComputePerimeter() const
{
  return m_ComputePerimeter;
}

/** Set the compute polygon flag */
template <class TLabelObject, class TLabelImage>
void
ShapeAttributesLabelObjectFunctor<TLabelObject, TLabelImage>
::SetComputePolygon(bool flag)
{
  m_ComputePolygon = flag;
}

/** Get the compute polygon flag */
template <class TLabelObject, class TLabelImage>
bool
ShapeAttributesLabelObjectFunctor<TLabelObject, TLabelImage>
::GetComputePolygon() const
{
  return m_ComputePolygon;
}

/** Set the compute Flusser flag */
template <class TLabelObject, class TLabelImage>
void
ShapeAttributesLabelObjectFunctor<TLabelObject, TLabelImage>
::SetComputeFlusser(bool flag)
{
  m_ComputeFlusser = flag;
}

/** Get the compute Flusser flag */
template <class TLabelObject, class TLabelImage>
bool
ShapeAttributesLabelObjectFunctor<TLabelObject, TLabelImage>
::GetComputeFlusser() const
{
  return m_ComputeFlusser;
}

/** Set the compute feret diameter flag */
template <class TLabelObject, class TLabelImage>
void
ShapeAttributesLabelObjectFunctor<TLabelObject, TLabelImage>
::SetComputeFeretDiameter(bool flag)
{
  m_ComputeFeretDiameter = flag;
}

/** Get the compute feret diameter flag */
template <class TLabelObject, class TLabelImage>
bool
ShapeAttributesLabelObjectFunctor<TLabelObject, TLabelImage>
::GetComputeFeretDiameter() const
{
  return m_ComputeFeretDiameter;
}

/** Set the compute reduced attributes set flag */
template <class TLabelObject, class TLabelImage>
void
ShapeAttributesLabelObjectFunctor<TLabelObject, TLabelImage>
::SetReducedAttributeSet(bool flag)
{
  m_ReducedAttributeSet = flag;
}

/** Get the compute reduced attributes set flag */
template <class TLabelObject, class TLabelImage>
bool
ShapeAttributesLabelObjectFunctor<TLabelObject, TLabelImage>
::GetReducedAttributeSet() const
{
  return m_ReducedAttributeSet;
}

/** Set the label image (used only to compute
 *  the Feret diameter */
template <class TLabelObject, class TLabelImage>
void
ShapeAttributesLabelObjectFunctor<TLabelObject, TLabelImage>
::SetLabelImage(const TLabelImage * image)
{
  m_LabelImage = image;
}

/** Get the label image */
template <class TLabelObject, class TLabelImage>
const TLabelImage *
ShapeAttributesLabelObjectFunctor<TLabelObject, TLabelImage>
::GetLabelImage() const
{
  return m_LabelImage;
}


/** This is the functor implementation
 *  Calling the functor on a label object
 *  will update its shape attributes */
template <class TLabelObject, class TLabelImage>
void
ShapeAttributesLabelObjectFunctor<TLabelObject, TLabelImage>
::operator() (LabelObjectType * lo)
{
  const typename LabelObjectType::LabelType& label = lo->GetLabel();

  // TODO: compute sizePerPixel, borderMin and borderMax in BeforeThreadedGenerateData() ?

  // compute the size per pixel, to be used later
  double sizePerPixel = 1;
  for (int i = 0; i < LabelObjectType::ImageDimension; ++i)
    {
    sizePerPixel *= vcl_abs(m_LabelImage->GetSpacing()[i]);
    }

  typename std::vector<double> sizePerPixelPerDimension;
  for (int i = 0; i < LabelObjectType::ImageDimension; ++i)
    {
    sizePerPixelPerDimension.push_back(sizePerPixel / vcl_abs(m_LabelImage->GetSpacing()[i]));
    }

  // compute the max the index on the border of the image
  typename LabelObjectType::IndexType borderMin = m_LabelImage->GetLargestPossibleRegion().GetIndex();
  typename LabelObjectType::IndexType borderMax = borderMin;
  for (int i = 0; i < LabelObjectType::ImageDimension; ++i)
    {
    borderMax[i] += borderMin[i] + m_LabelImage->GetLargestPossibleRegion().GetSize()[i] - 1;
    }

  // init the vars
  unsigned long                                                 size = 0;
  itk::ContinuousIndex<double, LabelObjectType::ImageDimension> centroid;
  centroid.Fill(0);
  typename LabelObjectType::IndexType mins;
  mins.Fill(itk::NumericTraits<long>::max());
  typename LabelObjectType::IndexType maxs;
  maxs.Fill(itk::NumericTraits<long>::NonpositiveMin());
  unsigned long                                                                         sizeOnBorder = 0;
  double                                                                                physicalSizeOnBorder = 0;
  itk::Matrix<double, LabelObjectType::ImageDimension, LabelObjectType::ImageDimension> centralMoments;
  centralMoments.Fill(0);

  ConstLineIteratorType lit = ConstLineIteratorType(lo);
  lit.GoToBegin();
  //lit = IteratorType( ref );
  //typename LabelObjectType::LineContainerType&                lineContainer = lo->GetLineContainer();

  // iterate over all the lines
  while ( !lit.IsAtEnd() )
    //for (lit = lineContainer.begin(); lit != lineContainer.end(); lit++)
    {
    const typename LabelObjectType::IndexType& idx = lit.GetLine().GetIndex();
    unsigned long                              length = lit.GetLine().GetLength();

    // update the size
    size += length;

    // update the centroid - and report the progress
    // first, update the axes which are not 0
    for (int i = 1; i < LabelObjectType::ImageDimension; ++i)
      {
      centroid[i] += length * idx[i];
      }
    // then, update the axis 0
    centroid[0] += idx[0] * length + (length * (length - 1)) / 2.0;

    // update the mins and maxs
    for (int i = 0; i < LabelObjectType::ImageDimension; ++i)
      {
      if (idx[i] < mins[i])
        {
        mins[i] = idx[i];
        }
      if (idx[i] > maxs[i])
        {
        maxs[i] = idx[i];
        }
      }
    // must fix the max for the axis 0
    if (idx[0] + (long) length > maxs[0])
      {
      maxs[0] = idx[0] + length - 1;
      }

    // object is on a border ?
    bool isOnBorder = false;
    for (int i = 1; i < LabelObjectType::ImageDimension; ++i)
      {
      if (idx[i] == borderMin[i] || idx[i] == borderMax[i])
        {
        isOnBorder = true;
        break;
        }
      }
    if (isOnBorder)
      {
      // the line touch a border on a dimension other than 0, so
      // all the line touch a border
      sizeOnBorder += length;
      }
    else
      {
      // we must check for the dimension 0
      bool isOnBorder0 = false;
      if (idx[0] == borderMin[0])
        {
        // one more pixel on the border
        sizeOnBorder++;
        isOnBorder0 = true;
        }
      if (!isOnBorder0 || length > 1)
        {
        // we can check for the end of the line
        if (idx[0] + (long) length - 1 == borderMax[0])
          {
          // one more pixel on the border
          sizeOnBorder++;
          }
        }
      }

    // physical size on border
    // first, the dimension 0
    if (idx[0] == borderMin[0])
      {
      // the begining of the line
      physicalSizeOnBorder += sizePerPixelPerDimension[0];
      }
    if (idx[0] + (long) length - 1 == borderMax[0])
      {
      // and the end of the line
      physicalSizeOnBorder += sizePerPixelPerDimension[0];
      }
    // then the other dimensions
    for (int i = 1; i < LabelObjectType::ImageDimension; ++i)
      {
      if (idx[i] == borderMin[i])
        {
        // one border
        physicalSizeOnBorder += sizePerPixelPerDimension[i] * length;
        }
      if (idx[i] == borderMax[i])
        {
        // and the other
        physicalSizeOnBorder += sizePerPixelPerDimension[i] * length;
        }
      }

    /*
    // moments computation
    // ****************************************************************
    // that commented code is the basic implementation. The next piece of code
    // give the same result in a much efficient way, by using expanded formulae
    // allowed by the binary case instead of loops.
    // ****************************************************************
         long endIdx0 = idx[0] + length;
         for( typename LabelObjectType::IndexType iidx = idx; iidx[0]<endIdx0; iidx[0]++)
           {
           typename TLabelImage::PointType pP;
           m_LabelImage->TransformIndexToPhysicalPoint(iidx, pP);

           for(unsigned int i=0; i<LabelObjectType::ImageDimension; ++i)
             {
             for(unsigned int j=0; j<LabelObjectType::ImageDimension; ++j)
               {
               centralMoments[i][j] += pP[i] * pP[j];
               }
             }
           }
    */

    // get the physical position and the spacing - they are used several times later
    typename TLabelImage::PointType physicalPosition;
    m_LabelImage->TransformIndexToPhysicalPoint(idx, physicalPosition);
    const typename TLabelImage::SpacingType& spacing = m_LabelImage->GetSpacing();
    // the sum of x positions, also reused several times
    double sumX = length * (physicalPosition[0] + (spacing[0] * (length - 1)) / 2.0);
    // the real job - the sum of square of x positions
    // that's the central moments for dims 0, 0
    centralMoments[0][0] += length * (physicalPosition[0] * physicalPosition[0]
                                      + spacing[0] *
                                      (length - 1) * ((spacing[0] * (2 * length - 1)) / 6.0 + physicalPosition[0]));
    // the other ones
    for (int i = 1; i < LabelObjectType::ImageDimension; ++i)
      {
      // do this one here to avoid the double assigment in the following loop
      // when i == j
      centralMoments[i][i] += length * physicalPosition[i] * physicalPosition[i];
      // central moments are symetrics, so avoid to compute them 2 times
      for (int j = i + 1; j < LabelObjectType::ImageDimension; ++j)
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
    ++lit;
    }

  // final computation
  typename TLabelImage::SizeType regionSize;
  double                         minSize = itk::NumericTraits<double>::max();
  double                         maxSize = itk::NumericTraits<double>::NonpositiveMin();
  for (int i = 0; i < LabelObjectType::ImageDimension; ++i)
    {
    centroid[i] /= size;
    regionSize[i] = maxs[i] - mins[i] + 1;
    double s = regionSize[i] * vcl_abs(m_LabelImage->GetSpacing()[i]);
    minSize = std::min(s, minSize);
    maxSize = std::max(s, maxSize);
    for (unsigned int j = 0; j < LabelObjectType::ImageDimension; ++j)
      {
      centralMoments[i][j] /= size;
      }
    }
  typename TLabelImage::RegionType region(mins, regionSize);
  typename TLabelImage::PointType physicalCentroid;
  m_LabelImage->TransformContinuousIndexToPhysicalPoint(centroid, physicalCentroid);

  // Center the second order moments
  for (unsigned int i = 0; i < LabelObjectType::ImageDimension; ++i)
    {
    for (unsigned int j = 0; j < LabelObjectType::ImageDimension; ++j)
      {
      centralMoments[i][j] -= physicalCentroid[i] * physicalCentroid[j];
      }
    }

  // Compute principal moments and axes
  itk::Vector<double, LabelObjectType::ImageDimension> principalMoments;
  vnl_symmetric_eigensystem<double> eigen(centralMoments.GetVnlMatrix());
  vnl_diag_matrix<double> pm = eigen.D;
  for (unsigned int i = 0; i < LabelObjectType::ImageDimension; ++i)
    {
//    principalMoments[i] = 4 * vcl_sqrt( pm(i, i) );
    principalMoments[i] = pm(i, i);
    }
  itk::Matrix<double, LabelObjectType::ImageDimension, LabelObjectType::ImageDimension>
  principalAxes = eigen.V.transpose();

  // Add a final reflection if needed for a proper rotation,
  // by multiplying the last row by the determinant
  vnl_real_eigensystem eigenrot(principalAxes.GetVnlMatrix());
  vnl_diag_matrix<vcl_complex<double> > eigenval = eigenrot.D;
  vcl_complex<double> det(1.0, 0.0);

  for (unsigned int i = 0; i < LabelObjectType::ImageDimension; ++i)
    {
    det *= eigenval(i, i);
    }

  for (unsigned int i = 0; i < LabelObjectType::ImageDimension; ++i)
    {
    principalAxes[LabelObjectType::ImageDimension - 1][i] *= std::real(det);
    }

  double elongation = 0;
  if (principalMoments[LabelObjectType::ImageDimension - 2] != 0)
    {
    elongation =
      vcl_sqrt(principalMoments[LabelObjectType::ImageDimension -
                                1] / principalMoments[LabelObjectType::ImageDimension - 2]);
    }

  double physicalSize = size * sizePerPixel;
  double equivalentRadius = hyperSphereRadiusFromVolume(physicalSize);
  double equivalentPerimeter = hyperSpherePerimeter(equivalentRadius);

  // compute equilalent ellipsoid radius
  itk::Vector<double, LabelObjectType::ImageDimension> ellipsoidSize;
  double                                               edet = 1.0;
  for (unsigned int i = 0; i < LabelObjectType::ImageDimension; ++i)
    {
    edet *= principalMoments[i];
    }
  edet = vcl_pow(edet, 1.0 / LabelObjectType::ImageDimension);
  for (unsigned int i = 0; i < LabelObjectType::ImageDimension; ++i)
    {
    ellipsoidSize[i] = 2.0 * equivalentRadius * vcl_sqrt(principalMoments[i] / edet);
    }


  if (m_ComputeFlusser)
  {
    // Flusser moments (only make sense when ImageDimension == 2)
    if (LabelObjectType::ImageDimension == 2)
      {
      // complex centered moments
      std::complex<double> c11, c20, c12, c21, c22, c30, c31, c40;
      c11 = c20 = c12 = c21 = c22 = c30 = c31 = c40 = std::complex<double>(0, 0);
      // Update the line iterator to the beginning
      lit.GoToBegin();
      //for (lit = lineContainer.begin(); lit != lineContainer.end();
      //lit++)
      while ( ! lit.IsAtEnd() )
        {
        const typename LabelObjectType::IndexType& idx = lit.GetLine().GetIndex();
        unsigned long length = lit.GetLine().GetLength();
        //
        long endIdx0 = idx[0] + length;
        for (typename LabelObjectType::IndexType iidx = idx; iidx[0] < endIdx0; iidx[0]++)
          {
          typename TLabelImage::PointType cPP;
          m_LabelImage->TransformIndexToPhysicalPoint(iidx, cPP);
          cPP -= physicalCentroid.GetVectorFromOrigin();
          std::complex<double> xpiy(cPP[0], cPP[1]); // x + i y
          std::complex<double> xmiy(cPP[0], -cPP[1]); // x - i y

          c11 += xpiy * xmiy;
          c20 += xpiy * xpiy;
          c12 += xpiy * xmiy * xmiy;
          c21 += xpiy * xpiy * xmiy;
          c30 += xpiy * xpiy * xpiy;
          c22 += xpiy * xpiy * xmiy * xmiy;
          c31 += xpiy * xpiy * xpiy * xmiy;
          c40 += xpiy * xpiy * xpiy * xpiy;
          }
	++lit;
        }

      // normalize
      c11 /= physicalSize * physicalSize;
      c20 /= physicalSize * physicalSize;
      c12 /= vcl_pow(physicalSize, 5.0 / 2);
      c21 /= vcl_pow(physicalSize, 5.0 / 2);
      c30 /= vcl_pow(physicalSize, 5.0 / 2);
      c22 /= vcl_pow(physicalSize, 3);
      c31 /= vcl_pow(physicalSize, 3);
      c40 /= vcl_pow(physicalSize, 3);

      lo->SetAttribute("SHAPE::Flusser01", c11.real());
      lo->SetAttribute("SHAPE::Flusser02", (c21 * c12).real());
      lo->SetAttribute("SHAPE::Flusser03", (c20 * vcl_pow(c12, 2)).real());
      lo->SetAttribute("SHAPE::Flusser04", (c20 * vcl_pow(c12, 2)).imag());
      lo->SetAttribute("SHAPE::Flusser05", (c30 * vcl_pow(c12, 3)).real());
      lo->SetAttribute("SHAPE::Flusser06", (c30 * vcl_pow(c12, 3)).imag());
      lo->SetAttribute("SHAPE::Flusser07", c22.real());
      lo->SetAttribute("SHAPE::Flusser08", (c31 * vcl_pow(c12, 2)).real());
      lo->SetAttribute("SHAPE::Flusser09", (c31 * vcl_pow(c12, 2)).imag());
      lo->SetAttribute("SHAPE::Flusser10", (c40 * vcl_pow(c12, 4)).real());
      lo->SetAttribute("SHAPE::Flusser11", (c40 * vcl_pow(c12, 4)).imag());
      }
    }

  // Set the attributes
  if (m_ComputePolygon)
    {
    PolygonFunctorType polygonFunctor;
    SimplifyPolygonFunctorType simplifyFunctor;
    polygonFunctor.SetStartIndex(m_LabelImage->GetLargestPossibleRegion().GetIndex());
    polygonFunctor.SetOrigin(m_LabelImage->GetOrigin());
    polygonFunctor.SetSpacing(m_LabelImage->GetSpacing());
    typename PolygonType::Pointer polygon = simplifyFunctor(polygonFunctor(lo));
    lo->SetPolygon(polygon);
    }

  // Physical size
  lo->SetAttribute("SHAPE::PhysicalSize", physicalSize);

  // Elongation
  lo->SetAttribute("SHAPE::Elongation", elongation);

  if (m_ComputeFeretDiameter)
    {
    // init the vars
    unsigned long size = 0;
    typedef typename std::deque<typename LabelObjectType::IndexType> IndexListType;
    IndexListType idxList;

    // Line iterator
    ConstLineIteratorType lit = ConstLineIteratorType(lo);
    lit.GoToBegin();

    typedef typename itk::ConstNeighborhoodIterator<LabelImageType> NeighborIteratorType;
    typename TLabelImage::SizeType neighborHoodRadius;
    neighborHoodRadius.Fill(1);
    NeighborIteratorType it(neighborHoodRadius, m_LabelImage, m_LabelImage->GetBufferedRegion());
    itk::ConstantBoundaryCondition<LabelImageType> lcbc;
    // use label + 1 to have a label different of the current label on the border
    lcbc.SetConstant(label + 1);
    it.OverrideBoundaryCondition(&lcbc);
    it.GoToBegin();

    // iterate over all the lines
    while ( !lit.IsAtEnd() )
      {
      const typename LabelObjectType::IndexType& firstIdx = lit.GetLine().GetIndex();
      unsigned long                              length = lit.GetLine().GetLength();

      long endIdx0 = firstIdx[0] + length;
      for (typename LabelObjectType::IndexType idx = firstIdx; idx[0] < endIdx0; idx[0]++)
        {

        // move the iterator to the new location
        it += idx - it.GetIndex();

        // push the pixel in the list if it is on the border of the object
        for (unsigned i = 0; i < it.Size(); ++i)
          {
          if (it.GetPixel(i) != label)
            {
            idxList.push_back(idx);
            size++;
            break;
            }
          }
        }
      ++lit;
      }

    // we can now search the feret diameter
    double feretDiameter = 0;
    for (typename IndexListType::const_iterator iIt1 = idxList.begin();
         iIt1 != idxList.end();
         iIt1++)
      {
      typename IndexListType::const_iterator iIt2 = iIt1;
      for (iIt2++; iIt2 != idxList.end(); iIt2++)
        {
        // Compute the length between the 2 indexes
        double length = 0;
        for (int i = 0; i < LabelObjectType::ImageDimension; ++i)
          {
          length += vcl_pow((iIt1->operator[] (i) - iIt2->operator[] (i)) * m_LabelImage->GetSpacing()[i], 2);
          }
        if (feretDiameter < length)
          {
          feretDiameter = length;
          }
        }
      }
    // final computation
    feretDiameter = vcl_sqrt(feretDiameter);

    // finally put the values in the label object
    lo->SetAttribute("SHAPE::FeretDiameter", feretDiameter);
    }

  // be sure that the calculator has the perimeter estimation for that label.
  // The calculator may not have the label if the object is only on a border.
  // It will occur for sure when processing a 2D image with a 3D filter.
  if (m_ComputePerimeter)
    {
    double perimeter = this->ComputePerimeter(lo,region);
    //double perimeter = lo->ComputePerimeter();
    lo->SetAttribute("SHAPE::Perimeter", perimeter);
    lo->SetAttribute("SHAPE::Roundness", equivalentPerimeter / perimeter);
    }

  // Complete feature set

  if (!m_ReducedAttributeSet)
    {
    lo->SetAttribute("SHAPE::Size", size);
    std::ostringstream oss;
    for (unsigned int dim = 0; dim < LabelObjectType::ImageDimension; ++dim)
      {
      oss.str("");
      oss << "SHAPE::RegionIndex" << dim;
      lo->SetAttribute(oss.str().c_str(), region.GetIndex()[dim]);
      oss.str("");
      oss << "SHAPE::RegionSize" << dim;
      lo->SetAttribute(oss.str().c_str(), region.GetSize()[dim]);
      oss.str("");
      oss << "SHAPE::PhysicalCentroid" << dim;
      lo->SetAttribute(oss.str().c_str(), physicalCentroid[dim]);
      oss.str("");
      oss << "SHAPE::EquivalentEllipsoidRadius" << dim;
      lo->SetAttribute(oss.str().c_str(), ellipsoidSize[dim]);
      oss.str("");
      oss << "SHAPE::PrincipalMoments" << dim;
      lo->SetAttribute(oss.str().c_str(), principalMoments[dim]);

      for (unsigned int dim2 = 0; dim2 < LabelObjectType::ImageDimension; ++dim2)
        {
        oss.str("");
        oss << "SHAPE::PrincipalAxis" << dim << dim2;
        lo->SetAttribute(oss.str().c_str(), principalAxes(dim, dim2));
        }
      }

    lo->SetAttribute("SHAPE::RegionElongation", maxSize / minSize);
    lo->SetAttribute("SHAPE::RegionRatio", size / (double) region.GetNumberOfPixels());
    lo->SetAttribute("SHAPE::SizeOnBorder", sizeOnBorder);
    lo->SetAttribute("SHAPE::PhysicalSizeOnBorder", physicalSizeOnBorder);
    lo->SetAttribute("SHAPE::EquivalentPerimeter", equivalentPerimeter);
    lo->SetAttribute("SHAPE::EquivalentRadius",    equivalentRadius);
    }
}

template <class TLabelObject, class TLabelImage>
double
ShapeAttributesLabelObjectFunctor<TLabelObject, TLabelImage>
::ComputePerimeter(LabelObjectType *labelObject, const RegionType & region)
{
  // store the lines in a N-1D image of vectors
  typedef std::deque< typename LabelObjectType::LineType > VectorLineType;
  typedef itk::Image< VectorLineType, ImageDimension - 1 > LineImageType;
  typename LineImageType::Pointer lineImage = LineImageType::New();
  typename LineImageType::IndexType lIdx;
  typename LineImageType::SizeType lSize;
  RegionType boundingBox = region;
  for( int i=0; i<ImageDimension-1; i++ )
    {
    lIdx[i] = boundingBox.GetIndex()[i+1];
    lSize[i] = boundingBox.GetSize()[i+1];
    }
  typename LineImageType::RegionType lRegion;
  lRegion.SetIndex( lIdx );
  lRegion.SetSize( lSize );
  // enlarge the region a bit to avoid boundary problems
  typename LineImageType::RegionType elRegion(lRegion);
  lSize.Fill(1);
  elRegion.PadByRadius(lSize);
  // std::cout << boundingBox << "  " << lRegion << "  " << elRegion << std::endl;
  // now initialize the image
  lineImage->SetRegions( elRegion );
  lineImage->Allocate();
  lineImage->FillBuffer( VectorLineType() );

  // std::cout << "lineContainer.size(): " << lineContainer.size() << std::endl;

  // Iterate over all the lines and fill the image of lines
  typename LabelObjectType::ConstLineIterator lit( labelObject );
  while( ! lit.IsAtEnd() )
    {
    const typename TLabelObject::IndexType & idx = lit.GetLine().GetIndex();
    for( int i=0; i<ImageDimension-1; i++ )
      {
      lIdx[i] = idx[i+1];
      }
    lineImage->GetPixel( lIdx ).push_back( lit.GetLine() );
    ++lit;
    }

  // a data structure to store the number of intercepts on each direction
  typedef typename std::map<OffsetType, itk::SizeValueType, typename OffsetType::LexicographicCompare> MapInterceptType;
  MapInterceptType intercepts;
  // int nbOfDirections = (int)vcl_pow( 2.0, (int)ImageDimension ) - 1;
  // intecepts.resize(nbOfDirections + 1);  // code begins at position 1

  // now iterate over the vectors of lines
  typedef itk::ConstShapedNeighborhoodIterator< LineImageType > LineImageIteratorType;
  LineImageIteratorType lIt( lSize, lineImage, lRegion ); // the original, non padded region
  setConnectivity( &lIt, true );
  for( lIt.GoToBegin(); !lIt.IsAtEnd(); ++lIt )
    {
    const VectorLineType & ls = lIt.GetCenterPixel();

    // there are two intercepts on the 0 axis for each line
    OffsetType no;
    no.Fill(0);
    no[0] = 1;
    // std::cout << no << "-> " << 2 * ls.size() << std::endl;
    intercepts[no] += 2 * ls.size();

    // and look at the neighbors
    typename LineImageIteratorType::ConstIterator ci;
    for (ci = lIt.Begin(); ci != lIt.End(); ci++)
      {
          // std::cout << "-------------" << std::endl;
      // the vector of lines in the neighbor
      const VectorLineType & ns = ci.Get();
      // prepare the offset to be stored in the intercepts map
      typename LineImageType::OffsetType lno = ci.GetNeighborhoodOffset();
      no[0] = 0;
      for( int i=0; i<ImageDimension-1; i++ )
        {
        no[i+1] = vnl_math_abs(lno[i]);
        }
      OffsetType dno = no; // offset for the diagonal
      dno[0] = 1;

      // now process the two lines to search the pixels on the contour of the object
      if( ls.empty() )
        {
        // std::cout << "ls.empty()" << std::endl;
        // nothing to do
        }
      if( ns.empty() )
        {
        // no line in the neighbors - all the lines in ls are on the contour
        for( typename VectorLineType::const_iterator li = ls.begin(); li != ls.end(); ++li )
          {
          // std::cout << "ns.empty()" << std::endl;
          const typename LabelObjectType::LineType & l = *li;
          // add as much intercepts as the line size
          intercepts[no] += l.GetLength();
          // and 2 times as much diagonal intercepts as the line size
          intercepts[dno] += l.GetLength() * 2;
          }
        }
      else
        {
        // std::cout << "else" << std::endl;
        // TODO - fix the code when the line starts at  NumericTraits<IndexValueType>::NonpositiveMin()
        // or end at  NumericTraits<IndexValueType>::max()
        typename VectorLineType::const_iterator li = ls.begin();
        typename VectorLineType::const_iterator ni = ns.begin();

        itk::IndexValueType lZero = 0;
        itk::IndexValueType lMin = 0;
        itk::IndexValueType lMax = 0;

        itk::IndexValueType nMin = itk::NumericTraits<itk::IndexValueType>::NonpositiveMin() + 1;
        itk::IndexValueType nMax = ni->GetIndex()[0] - 1;

        while( li!=ls.end() )
          {
          // update the current line min and max. Neighbor line data is already up to date.
          lMin = li->GetIndex()[0];
          lMax = lMin + li->GetLength() - 1;

          // add as much intercepts as intersections of the 2 lines
          intercepts[no] += vnl_math_max( lZero, vnl_math_min(lMax, nMax) - vnl_math_max(lMin, nMin) + 1 );
          // std::cout << "============" << std::endl;
          // std::cout << "  lMin:" << lMin << " lMax:" << lMax << " nMin:" << nMin << " nMax:" << nMax;
          // std::cout << " count: " << vnl_math_max( 0l, vnl_math_min(lMax, nMax) - vnl_math_max(lMin, nMin) + 1 ) << std::endl;
          // std::cout << "  " << no << ": " << intercepts[no] << std::endl;
          // std::cout << vnl_math_max( lZero, vnl_math_min(lMax, nMax+1) - vnl_math_max(lMin, nMin+1) + 1 ) << std::endl;
          // std::cout << vnl_math_max( lZero, vnl_math_min(lMax, nMax-1) - vnl_math_max(lMin, nMin-1) + 1 ) << std::endl;
          // left diagonal intercepts
          intercepts[dno] += vnl_math_max( lZero, vnl_math_min(lMax, nMax+1) - vnl_math_max(lMin, nMin+1) + 1 );
          // right diagonal intercepts
          intercepts[dno] += vnl_math_max( lZero, vnl_math_min(lMax, nMax-1) - vnl_math_max(lMin, nMin-1) + 1 );

          // go to the next line or the next neighbor depending on where we are
          if(nMax <= lMax )
            {
            // go to next neighbor
            nMin = ni->GetIndex()[0] + ni->GetLength();
            ni++;

            if( ni != ns.end() )
              {
              nMax = ni->GetIndex()[0] - 1;
              }
            else
              {
              nMax = itk::NumericTraits<itk::IndexValueType>::max() - 1;
              }
            }
          else
            {
            // go to next line
            li++;
            }
          }

        }
      }
    }

  // compute the perimeter based on the intercept counts
  double perimeter = PerimeterFromInterceptCount( intercepts, m_LabelImage->GetSpacing() );
  return perimeter;
}

template <class TLabelObject, class TLabelImage>
template<class TMapIntercept, class TSpacing>
double
ShapeAttributesLabelObjectFunctor<TLabelObject, TLabelImage>
::PerimeterFromInterceptCount( TMapIntercept & intercepts, const TSpacing & spacing )
{
  // std::cout << "PerimeterFromInterceptCount<>" << std::endl;
  double perimeter = 0.0;
  double pixelSize = 1.0;
  int dim = TSpacing::GetVectorDimension();
  for ( int i = 0; i < dim; i++ )
    {
    pixelSize *= spacing[i];
    }

  for( int i=0; i<dim; i++ )
    {
    OffsetType no;
    no.Fill(0);
    no[i] = 1;
    // std::cout << no << ": " << intercepts[no] << std::endl;
    perimeter += pixelSize / spacing[i] * intercepts[no]/2.0;
    }

  // Crofton's constant
  perimeter *= itk::GeometryUtilities::HyperSphereVolume( dim, 1.0 )
                 / itk::GeometryUtilities::HyperSphereVolume( dim - 1, 1.0 );
  return perimeter;
}

#if ! defined(ITK_DO_NOT_USE_PERIMETER_SPECIALIZATION)
template <class TLabelObject, class TLabelImage>
double
ShapeAttributesLabelObjectFunctor<TLabelObject, TLabelImage>
::PerimeterFromInterceptCount( MapIntercept2Type & intercepts, const Spacing2Type spacing )
{
  // std::cout << "PerimeterFromInterceptCount2" << std::endl;
  double dx = spacing[0];
  double dy = spacing[1];

  Offset2Type nx =  {{1, 0}};
  Offset2Type ny =  {{0, 1}};
  Offset2Type nxy = {{1, 1}};

  // std::cout << "nx: " << intercepts[nx] << std::endl;
  // std::cout << "ny: " << intercepts[ny] << std::endl;
  // std::cout << "nxy: " << intercepts[nxy] << std::endl;

  double perimeter = 0.0;
  perimeter += dy * intercepts[nx]/2.0;
  perimeter += dx * intercepts[ny]/2.0;
  perimeter += dx*dy / spacing.GetNorm() * intercepts[nxy]/2.0;
  perimeter *= itk::Math::pi / 4.0;
  return perimeter;
};

template <class TLabelObject, class TLabelImage>
double
ShapeAttributesLabelObjectFunctor<TLabelObject, TLabelImage>
::PerimeterFromInterceptCount( MapIntercept3Type & intercepts, const Spacing3Type spacing )
{
  // std::cout << "PerimeterFromInterceptCount3" << std::endl;
  double dx = spacing[0];
  double dy = spacing[1];
  double dz = spacing[2];
  double dxy = vcl_sqrt( spacing[0]*spacing[0] + spacing[1]*spacing[1] );
  double dxz = vcl_sqrt( spacing[0]*spacing[0] + spacing[2]*spacing[2] );
  double dyz = vcl_sqrt( spacing[1]*spacing[1] + spacing[2]*spacing[2] );
  double dxyz = vcl_sqrt( spacing[0]*spacing[0] + spacing[1]*spacing[1] + spacing[2]*spacing[2] );
  double vol = spacing[0]*spacing[1]*spacing[2];

  // 'magical numbers', corresponding to area of voronoi partition on the
  // unit sphere, when germs are the 26 directions on the unit cube
  // Sum of (c1+c2+c3 + c4*2+c5*2+c6*2 + c7*4) equals 1.
  double c1 = 0.04577789120476 * 2;  // Ox
  double c2 = 0.04577789120476 * 2;  // Oy
  double c3 = 0.04577789120476 * 2;  // Oz
  double c4 = 0.03698062787608 * 2;  // Oxy
  double c5 = 0.03698062787608 * 2;  // Oxz
  double c6 = 0.03698062787608 * 2;  // Oyz
  double c7 = 0.03519563978232 * 2;  // Oxyz
  // TODO - recompute those values if the spacing is non isotrope

  Offset3Type nx =   {{1, 0, 0}};
  Offset3Type ny =   {{0, 1, 0}};
  Offset3Type nz =   {{0, 0, 1}};
  Offset3Type nxy =  {{1, 1, 0}};
  Offset3Type nxz =  {{1, 0, 1}};
  Offset3Type nyz =  {{0, 1, 1}};
  Offset3Type nxyz = {{1, 1, 1}};

  // std::cout << "nx: " << intercepts[nx] << std::endl;
  // std::cout << "ny: " << intercepts[ny] << std::endl;
  // std::cout << "nz: " << intercepts[nz] << std::endl;
  // std::cout << "nxy: " << intercepts[nxy] << std::endl;
  // std::cout << "nxz: " << intercepts[nxz] << std::endl;
  // std::cout << "nyz: " << intercepts[nyz] << std::endl;
  // std::cout << "nxyz: " << intercepts[nxyz] << std::endl;

  double perimeter = 0.0;
  perimeter += vol/dx * intercepts[nx]/2.0 * c1;
  perimeter += vol/dy * intercepts[ny]/2.0 * c2;
  perimeter += vol/dz * intercepts[nz]/2.0 * c3;
  perimeter += vol/dxy * intercepts[nxy]/2.0 * c4;
  perimeter += vol/dxz * intercepts[nxz]/2.0 * c5;
  perimeter += vol/dyz * intercepts[nyz]/2.0 * c6;
  perimeter += vol/dxyz * intercepts[nxyz]/2.0 * c7;
  perimeter *= 4;
  return perimeter;
};
#endif

/** Convenience internal method */
template <class TLabelObject, class TLabelImage>
long ShapeAttributesLabelObjectFunctor<TLabelObject, TLabelImage>
::factorial(long n)
{
  if (n < 1)
    {
    return 1;
    }
  return n * factorial(n - 1);
}

/** Convenience internal method */
template <class TLabelObject, class TLabelImage>
long ShapeAttributesLabelObjectFunctor<TLabelObject, TLabelImage>
::doubleFactorial(long n)
{
  if (n < 2)
    {
    return 1;
    }
  return n * doubleFactorial(n - 2);
}

/** Convenience internal method  */
template <class TLabelObject, class TLabelImage>
double ShapeAttributesLabelObjectFunctor<TLabelObject, TLabelImage>
::gammaN2p1(long n)
{
  bool even = n % 2 == 0;
  if (even)
    {
    return factorial(n / 2);
    }
  else
    {
    return otb::CONST_SQRTPI * doubleFactorial(n) / vcl_pow(2, (n + 1) / 2.0);
    }
}

/** Convenience internal method  */
template <class TLabelObject, class TLabelImage>
double ShapeAttributesLabelObjectFunctor<TLabelObject, TLabelImage>
::hyperSphereVolume(double radius)
{
  return vcl_pow(otb::CONST_PI, LabelObjectType::ImageDimension /
                 2.0) * vcl_pow(radius, LabelObjectType::ImageDimension) / gammaN2p1(LabelObjectType::ImageDimension);
}

/** Convenience internal method  */
template <class TLabelObject, class TLabelImage>
double ShapeAttributesLabelObjectFunctor<TLabelObject, TLabelImage>
::hyperSpherePerimeter(double radius)
{
  return LabelObjectType::ImageDimension * hyperSphereVolume(radius) / radius;
}

/** Convenience internal method  */
template <class TLabelObject, class TLabelImage>
double ShapeAttributesLabelObjectFunctor<TLabelObject, TLabelImage>
::hyperSphereRadiusFromVolume(double volume)
{
  return vcl_pow(volume * gammaN2p1(LabelObjectType::ImageDimension) /
                 vcl_pow(otb::CONST_PI, LabelObjectType::ImageDimension / 2.0), 1.0 / LabelObjectType::ImageDimension);
}

} // End namespace Functor

template<class TImage, class TLabelImage>
void
ShapeAttributesLabelMapFilter<TImage, TLabelImage>
::SetComputeFeretDiameter(bool flag)
{
  if (this->GetFunctor().GetComputeFeretDiameter() != flag)
    {
    this->GetFunctor().SetComputeFeretDiameter(flag);
    this->Modified();
    }
}

template<class TImage, class TLabelImage>
bool
ShapeAttributesLabelMapFilter<TImage, TLabelImage>
::GetComputeFeretDiameter() const
{
  return this->GetFunctor().GetComputeFeretDiameter();
}

template<class TImage, class TLabelImage>
void
ShapeAttributesLabelMapFilter<TImage, TLabelImage>
::SetComputePerimeter(bool flag)
{
  if (this->GetFunctor().GetComputePerimeter() != flag)
    {
    this->GetFunctor().SetComputePerimeter(flag);
    this->Modified();
    }
}

template<class TImage, class TLabelImage>
bool
ShapeAttributesLabelMapFilter<TImage, TLabelImage>
::GetComputePerimeter() const
{
  return this->GetFunctor().GetComputePerimeter();
}

template<class TImage, class TLabelImage>
void
ShapeAttributesLabelMapFilter<TImage, TLabelImage>
::SetComputePolygon(bool flag)
{
  if (this->GetFunctor().GetComputePolygon() != flag)
    {
    this->GetFunctor().SetComputePolygon(flag);
    this->Modified();
    }
}

template<class TImage, class TLabelImage>
bool
ShapeAttributesLabelMapFilter<TImage, TLabelImage>
::GetComputePolygon() const
{
  return this->GetFunctor().GetComputePolygon();
}

template<class TImage, class TLabelImage>
void
ShapeAttributesLabelMapFilter<TImage, TLabelImage>
::SetComputeFlusser(bool flag)
{
  if (this->GetFunctor().GetComputeFlusser() != flag)
    {
    this->GetFunctor().SetComputeFlusser(flag);
    this->Modified();
    }
}

template<class TImage, class TLabelImage>
bool
ShapeAttributesLabelMapFilter<TImage, TLabelImage>
::GetComputeFlusser() const
{
  return this->GetFunctor().GetComputeFlusser();
}


template<class TImage, class TLabelImage>
void
ShapeAttributesLabelMapFilter<TImage, TLabelImage>
::SetReducedAttributeSet(bool flag)
{
  if (this->GetFunctor().GetReducedAttributeSet() != flag)
    {
    this->GetFunctor().SetReducedAttributeSet(flag);
    this->Modified();
    }
}

template<class TImage, class TLabelImage>
bool
ShapeAttributesLabelMapFilter<TImage, TLabelImage>
::GetReducedAttributeSet() const
{
  return this->GetFunctor().GetReducedAttributeSet();
}

template<class TImage, class TLabelImage>
void
ShapeAttributesLabelMapFilter<TImage, TLabelImage>
::SetLabelImage(const TLabelImage * img)
{
  if (this->GetFunctor().GetLabelImage() != img)
    {
    this->GetFunctor().SetLabelImage(img);
    this->Modified();
    }
}

template<class TImage, class TLabelImage>
const TLabelImage *
ShapeAttributesLabelMapFilter<TImage, TLabelImage>
::GetLabelImage() const
{
  return this->GetFunctor().GetLabelImage();
}

template <class TImage, class TLabelImage>
void
ShapeAttributesLabelMapFilter<TImage, TLabelImage>
::AllocateOutputs()
 {
 // if told to run in place and the types support it,
  if( this->GetInPlace() && this->CanRunInPlace() )
    {
    // Graft this first input to the output.  Later, we'll need to
    // remove the input's hold on the bulk data.
    //
   ImagePointer inputAsOutput = dynamic_cast<TImage *>(const_cast<TImage *>(this->GetInput()));

    if( inputAsOutput )
      {

      this->GraftOutput( inputAsOutput );
      this->GetOutput()->SetLargestPossibleRegion(this->GetOutput()->GetLargestPossibleRegion());
      this->GetOutput()->SetRequestedRegion(this->GetOutput()->GetRequestedRegion());
      this->GetOutput()->SetBufferedRegion(this->GetOutput()->GetBufferedRegion());

      }

    // If there are more than one outputs, allocate the remaining outputs
    for (unsigned int i=1; i < this->GetNumberOfOutputs(); ++i)
      {
      ImagePointer outputPtr;

      outputPtr = this->GetOutput(i);
      outputPtr->SetBufferedRegion(outputPtr->GetRequestedRegion());
      outputPtr->Allocate();
      }
    }
  else
    {
    //
     Superclass::AllocateOutputs();
    // copy the content of the input image to the output image (will be done by ImageSource AllocateOutputs Method)
    // would never occur : inputasoutput condition is always true, since output and input type is TImage for
   // ShapeAttributesLabelMapFilter class
    }
}

template <class TImage, class TLabelImage>
void
ShapeAttributesLabelMapFilter<TImage, TLabelImage>
::GenerateInputRequestedRegion()
{
  itk::ImageToImageFilter<TImage, TImage>::GenerateInputRequestedRegion();
}


template<class TImage, class TLabelImage>
void
ShapeAttributesLabelMapFilter<TImage, TLabelImage>
::BeforeThreadedGenerateData()
{
  Superclass::BeforeThreadedGenerateData();
  if (!this->GetFunctor().GetLabelImage())
    {
    // generate an image of the labelized image
    typedef itk::LabelMapToLabelImageFilter<TImage, TLabelImage> LCI2IType;
    typename LCI2IType::Pointer lci2i = LCI2IType::New();
    lci2i->SetInput(this->GetInput());
    // respect the number of threads of the filter
    lci2i->SetNumberOfThreads(this->GetNumberOfThreads());
    lci2i->Update();
    this->GetFunctor().SetLabelImage(lci2i->GetOutput());
    }

/*   // delegate the computation of the perimeter to a dedicated calculator */
/*   if (this->GetFunctor().GetComputePerimeter()) */
/*     { */
/*     typename PerimeterCalculatorType::Pointer pc = PerimeterCalculatorType::New(); */
/*     pc->SetImage(this->GetFunctor().GetLabelImage()); */
/*     pc->Compute(); */
/*     this->GetFunctor().SetPerimeterCalculator(pc); */
/*     } */

}

template<class TImage, class TLabelImage>
void
ShapeAttributesLabelMapFilter<TImage, TLabelImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "ComputeFeretDiameter: " << this->GetFunctor().GetComputeFeretDiameter() << std::endl;
  os << indent << "ComputePerimeter: " << this->GetFunctor().GetComputePerimeter() << std::endl;
}

} // end namespace otb
#endif
