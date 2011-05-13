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
#include "itkLabelMapToLabelImageFilter.h"
#include "itkConstantBoundaryCondition.h"
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
  m_ComputePolygon(true),
  m_ReducedAttributeSet(true),
  m_PerimeterCalculator(NULL),
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

/** Set the compute perimeter flag */
template <class TLabelObject, class TLabelImage>
void
ShapeAttributesLabelObjectFunctor<TLabelObject, TLabelImage>
::SetComputePolygon(bool flag)
{
  m_ComputePolygon = flag;
}

/** Get the compute perimeter flag */
template <class TLabelObject, class TLabelImage>
bool
ShapeAttributesLabelObjectFunctor<TLabelObject, TLabelImage>
::GetComputePolygon() const
{
  return m_ComputePolygon;
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

template <class TLabelObject, class TLabelImage>
void
ShapeAttributesLabelObjectFunctor<TLabelObject, TLabelImage>
::SetPerimeterCalculator(const PerimeterCalculatorType * pc)
{
  m_PerimeterCalculator = pc;
}

/** This is the functor implementation
 *  Calling the functor on a label object
 *  will update its shape attributes */
template <class TLabelObject, class TLabelImage>
void
ShapeAttributesLabelObjectFunctor<TLabelObject, TLabelImage>
::operator() (LabelObjectType * lo) const
{
  const typename LabelObjectType::LabelType& label = lo->GetLabel();

  // TODO: compute sizePerPixel, borderMin and borderMax in BeforeThreadedGenerateData() ?

  // compute the size per pixel, to be used later
  double sizePerPixel = 1;
  for (int i = 0; i < LabelObjectType::ImageDimension; i++)
    {
    sizePerPixel *= vcl_abs(m_LabelImage->GetSpacing()[i]);
    }

  typename std::vector<double> sizePerPixelPerDimension;
  for (int i = 0; i < LabelObjectType::ImageDimension; i++)
    {
    sizePerPixelPerDimension.push_back(sizePerPixel / vcl_abs(m_LabelImage->GetSpacing()[i]));
    }

  // compute the max the index on the border of the image
  typename LabelObjectType::IndexType borderMin = m_LabelImage->GetLargestPossibleRegion().GetIndex();
  typename LabelObjectType::IndexType borderMax = borderMin;
  for (int i = 0; i < LabelObjectType::ImageDimension; i++)
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

  typename LabelObjectType::LineContainerType::const_iterator lit;
  typename LabelObjectType::LineContainerType&                lineContainer = lo->GetLineContainer();

  // iterate over all the lines
  for (lit = lineContainer.begin(); lit != lineContainer.end(); lit++)
    {
    const typename LabelObjectType::IndexType& idx = lit->GetIndex();
    unsigned long                              length = lit->GetLength();

    // update the size
    size += length;

    // update the centroid - and report the progress
    // first, update the axes which are not 0
    for (int i = 1; i < LabelObjectType::ImageDimension; i++)
      {
      centroid[i] += length * idx[i];
      }
    // then, update the axis 0
    centroid[0] += idx[0] * length + (length * (length - 1)) / 2.0;

    // update the mins and maxs
    for (int i = 0; i < LabelObjectType::ImageDimension; i++)
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
    for (int i = 1; i < LabelObjectType::ImageDimension; i++)
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
    for (int i = 1; i < LabelObjectType::ImageDimension; i++)
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

           for(unsigned int i=0; i<LabelObjectType::ImageDimension; i++)
             {
             for(unsigned int j=0; j<LabelObjectType::ImageDimension; j++)
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
    for (int i = 1; i < LabelObjectType::ImageDimension; i++)
      {
      // do this one here to avoid the double assigment in the following loop
      // when i == j
      centralMoments[i][i] += length * physicalPosition[i] * physicalPosition[i];
      // central moments are symetrics, so avoid to compute them 2 times
      for (int j = i + 1; j < LabelObjectType::ImageDimension; j++)
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
  typename TLabelImage::SizeType regionSize;
  double                         minSize = itk::NumericTraits<double>::max();
  double                         maxSize = itk::NumericTraits<double>::NonpositiveMin();
  for (int i = 0; i < LabelObjectType::ImageDimension; i++)
    {
    centroid[i] /= size;
    regionSize[i] = maxs[i] - mins[i] + 1;
    double s = regionSize[i] * vcl_abs(m_LabelImage->GetSpacing()[i]);
    minSize = std::min(s, minSize);
    maxSize = std::max(s, maxSize);
    for (unsigned int j = 0; j < LabelObjectType::ImageDimension; j++)
      {
      centralMoments[i][j] /= size;
      }
    }
  typename TLabelImage::RegionType region(mins, regionSize);
  typename TLabelImage::PointType physicalCentroid;
  m_LabelImage->TransformContinuousIndexToPhysicalPoint(centroid, physicalCentroid);

  // Center the second order moments
  for (unsigned int i = 0; i < LabelObjectType::ImageDimension; i++)
    {
    for (unsigned int j = 0; j < LabelObjectType::ImageDimension; j++)
      {
      centralMoments[i][j] -= physicalCentroid[i] * physicalCentroid[j];
      }
    }

  // Compute principal moments and axes
  itk::Vector<double, LabelObjectType::ImageDimension> principalMoments;
  vnl_symmetric_eigensystem<double> eigen(centralMoments.GetVnlMatrix());
  vnl_diag_matrix<double> pm = eigen.D;
  for (unsigned int i = 0; i < LabelObjectType::ImageDimension; i++)
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

  for (unsigned int i = 0; i < LabelObjectType::ImageDimension; i++)
    {
    det *= eigenval(i, i);
    }

  for (unsigned int i = 0; i < LabelObjectType::ImageDimension; i++)
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
  for (unsigned int i = 0; i < LabelObjectType::ImageDimension; i++)
    {
    edet *= principalMoments[i];
    }
  edet = vcl_pow(edet, 1.0 / LabelObjectType::ImageDimension);
  for (unsigned int i = 0; i < LabelObjectType::ImageDimension; i++)
    {
    ellipsoidSize[i] = 2.0 * equivalentRadius * vcl_sqrt(principalMoments[i] / edet);
    }

  // Flusser moments (only make sense when ImageDimension == 2)
  if (LabelObjectType::ImageDimension == 2)
    {
    // complex centered moments
    std::complex<double> c11, c20, c12, c21, c22, c30, c31, c40;
    c11 = c20 = c12 = c21 = c22 = c30 = c31 = c40 = std::complex<double>(0, 0);
    for (lit = lineContainer.begin(); lit != lineContainer.end(); lit++)
      {
      const typename LabelObjectType::IndexType& idx = lit->GetIndex();
      unsigned long                              length = lit->GetLength();

      //
      long endIdx0 = idx[0] + length;
      for (typename LabelObjectType::IndexType iidx = idx; iidx[0] < endIdx0; iidx[0]++)
        {
        typename TLabelImage::PointType cPP;
        m_LabelImage->TransformIndexToPhysicalPoint(iidx, cPP);
        cPP -= physicalCentroid.GetVectorFromOrigin();
        std::complex<double> xpiy(cPP[0],  cPP[1]); // x + i y
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

    // the iterators
    typename LabelObjectType::LineContainerType::const_iterator lit;
    typename LabelObjectType::LineContainerType&                lineContainer = lo->GetLineContainer();

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
    for (lit = lineContainer.begin(); lit != lineContainer.end(); lit++)
      {
      const typename LabelObjectType::IndexType& firstIdx = lit->GetIndex();
      unsigned long                              length = lit->GetLength();

      long endIdx0 = firstIdx[0] + length;
      for (typename LabelObjectType::IndexType idx = firstIdx; idx[0] < endIdx0; idx[0]++)
        {

        // move the iterator to the new location
        it += idx - it.GetIndex();

        // push the pixel in the list if it is on the border of the object
        for (unsigned i = 0; i < it.Size(); i++)
          {
          if (it.GetPixel(i) != label)
            {
            idxList.push_back(idx);
            size++;
            break;
            }
          }
        }
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
        for (int i = 0; i < LabelObjectType::ImageDimension; i++)
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
  if (m_ComputePerimeter && m_PerimeterCalculator->HasLabel(label))
    {
    double perimeter = m_PerimeterCalculator->GetPerimeter(label);
    lo->SetAttribute("SHAPE::Perimeter", perimeter);
    lo->SetAttribute("SHAPE::Roundness", equivalentPerimeter / perimeter);
    }

  // Complete feature set

  if (!m_ReducedAttributeSet)
    {
    lo->SetAttribute("SHAPE::Size", size);
    itk::OStringStream oss;
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
::GenerateInputRequestedRegion()
{

  for (unsigned int idx = 0; idx < this->GetNumberOfInputs(); ++idx)
      {
       ImagePointer input = const_cast<ImageType *>(this->GetInput(idx));
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

  // delegate the computation of the perimeter to a dedicated calculator
  if (this->GetFunctor().GetComputePerimeter())
    {
    typename PerimeterCalculatorType::Pointer pc = PerimeterCalculatorType::New();
    pc->SetImage(this->GetFunctor().GetLabelImage());
    pc->Compute();
    this->GetFunctor().SetPerimeterCalculator(pc);
    }

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
