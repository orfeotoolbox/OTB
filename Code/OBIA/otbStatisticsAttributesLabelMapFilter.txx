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
#ifndef __otbStatisticsAttributesLabelMapFilter_txx
#define __otbStatisticsAttributesLabelMapFilter_txx

#include "otbStatisticsAttributesLabelMapFilter.h"

#include "vnl/algo/vnl_real_eigensystem.h"
#include "vnl/algo/vnl_symmetric_eigensystem.h"

namespace otb
{
namespace Functor
{
/** Constructor */
template <class TLabelObject, class TFeatureImage>
StatisticsAttributesLabelObjectFunctor<TLabelObject, TFeatureImage>
::StatisticsAttributesLabelObjectFunctor() : m_FeatureName("Default"),
  m_FeatureImage(),
  m_ReducedAttributeSet(true)
{}

/** Destructor */
template <class TLabelObject, class TFeatureImage>
StatisticsAttributesLabelObjectFunctor<TLabelObject, TFeatureImage>
::~StatisticsAttributesLabelObjectFunctor()
{}

/** The comparators */
template <class TLabelObject, class TFeatureImage>
bool
StatisticsAttributesLabelObjectFunctor<TLabelObject, TFeatureImage>
::operator != (const Self &self)
  {
  // Initialize response
  bool resp = true;

  resp = resp && (m_FeatureName != self.m_FeatureName);
  resp = resp && (m_FeatureImage != self.m_FeatureImage);

  // Return
  return resp;
  }

template <class TLabelObject, class TFeatureImage>
bool
StatisticsAttributesLabelObjectFunctor<TLabelObject, TFeatureImage>
::operator == (const Self &self)
  {
  // Call the != implementation
  return !(this != self);
  }

/** This is the functor implementation
 *  Calling the functor on a label object
 *  will update its statistics attributes */
template <class TLabelObject, class TFeatureImage>
void
StatisticsAttributesLabelObjectFunctor<TLabelObject, TFeatureImage>
::operator() (LabelObjectType * lo) const
{
  ConstLineIteratorType lit = ConstLineIteratorType(lo);
  lit.GoToBegin();

  std::ostringstream oss;

  FeatureType min = itk::NumericTraits<FeatureType>::max();
  FeatureType max = itk::NumericTraits<FeatureType>::NonpositiveMin();
  double sum = 0;
  double sum2 = 0;
  double sum3 = 0;
  double sum4 = 0;
  unsigned int totalFreq = 0;
  typename TFeatureImage::IndexType minIdx;
  minIdx.Fill(0);
  typename TFeatureImage::IndexType maxIdx;
  maxIdx.Fill(0);
  typename TFeatureImage::PointType centerOfGravity;
  centerOfGravity.Fill(0);
  MatrixType centralMoments;
  centralMoments.Fill(0);
  MatrixType principalAxes;
  principalAxes.Fill(0);
  VectorType principalMoments;
  principalMoments.Fill(0);

  // iterate over all the lines
  while ( !lit.IsAtEnd() )
    {
    const typename TFeatureImage::IndexType& firstIdx = lit.GetLine().GetIndex();
    unsigned long length = lit.GetLine().GetLength();

    long endIdx0 = firstIdx[0] + length;
    for (typename TFeatureImage::IndexType idx = firstIdx; idx[0] < endIdx0; idx[0]++)
      {
      const FeatureType& v = m_FeatureImage->GetPixel(idx);
      ++totalFreq;

      // update min and max
      if (v <= min)
        {
        min = v;
        minIdx = idx;
        }
      if (v >= max)
        {
        max = v;
        maxIdx = idx;
        }

      //increase the sums
      const double v2 = v * v;

      sum += v;
      sum2 += v2;
      sum3 += v2 * v;
      sum4 += v2 * v2;

      if (!m_ReducedAttributeSet)
        {
        // moments
        typename TFeatureImage::PointType physicalPosition;
        m_FeatureImage->TransformIndexToPhysicalPoint(idx, physicalPosition);
        for (unsigned int i = 0; i < TFeatureImage::ImageDimension; ++i)
          {
          centerOfGravity[i] += physicalPosition[i] * v;
          centralMoments[i][i] += v * physicalPosition[i] * physicalPosition[i];
          for (unsigned int j = i + 1; j < TFeatureImage::ImageDimension; ++j)
            {
            const double weight = v * physicalPosition[i] * physicalPosition[j];
            centralMoments[i][j] += weight;
            centralMoments[j][i] += weight;
            }
          }
        }
      }
    ++lit;
    }

  // final computations
  const double mean = sum / totalFreq;
  const double variance = (sum2 - (sum * sum / totalFreq)) / (totalFreq - 1);
  const double sigma = vcl_sqrt(variance);
  const double mean2 = mean * mean;
  double skewness = 0;
  double kurtosis = 0;

  const double epsilon = 1E-10;
  if (vcl_abs(variance) > epsilon)
    {
    skewness = ((sum3 - 3.0 * mean * sum2) / totalFreq + 2.0 * mean * mean2) / (variance * sigma);
    kurtosis = ((sum4 - 4.0 * mean * sum3 + 6.0 * mean2 * sum2) / totalFreq - 3.0 * mean2 * mean2) / (variance
        * variance) - 3.0;
    }

  oss.str("");
  oss << "STATS::" << m_FeatureName << "::Mean";
  lo->SetAttribute(oss.str().c_str(), mean);

  oss.str("");
  oss << "STATS::" << m_FeatureName << "::Variance";
  lo->SetAttribute(oss.str().c_str(), variance);

  oss.str("");
  oss << "STATS::" << m_FeatureName << "::Skewness";
  lo->SetAttribute(oss.str().c_str(), skewness);

  oss.str("");
  oss << "STATS::" << m_FeatureName << "::Kurtosis";
  lo->SetAttribute(oss.str().c_str(), kurtosis);

  if (!m_ReducedAttributeSet)
    {
    double elongation = std::numeric_limits<double>::quiet_NaN();
    if (sum != 0)
      {
      // Normalize using the total mass
      for (unsigned int i = 0; i < TFeatureImage::ImageDimension; ++i)
        {
        centerOfGravity[i] /= sum;
        for (unsigned int j = 0; j < TFeatureImage::ImageDimension; ++j)
          {
          centralMoments[i][j] /= sum;
          }
        }

      // Center the second order moments
      for (unsigned int i = 0; i < TFeatureImage::ImageDimension; ++i)
        {
        for (unsigned int j = 0; j < TFeatureImage::ImageDimension; ++j)
          {
          centralMoments[i][j] -= centerOfGravity[i] * centerOfGravity[j];
          }
        }

      // Compute principal moments and axes
      vnl_symmetric_eigensystem<double> eigen(centralMoments.GetVnlMatrix());
      vnl_diag_matrix<double> pm = eigen.D;
      for (unsigned int i = 0; i < TFeatureImage::ImageDimension; ++i)
        {
        //    principalMoments[i] = 4 * vcl_sqrt( pm(i, i) );
        principalMoments[i] = pm(i, i);
        }
      principalAxes = eigen.V.transpose();

      // Add a final reflection if needed for a proper rotation,
      // by multiplying the last row by the determinant
      vnl_real_eigensystem eigenrot(principalAxes.GetVnlMatrix());
      vnl_diag_matrix<vcl_complex<double> > eigenval = eigenrot.D;
      vcl_complex<double> det(1.0, 0.0);

      for (unsigned int i = 0; i < TFeatureImage::ImageDimension; ++i)
        {
        det *= eigenval(i, i);
        }

      for (unsigned int i = 0; i < TFeatureImage::ImageDimension; ++i)
        {
        principalAxes[TFeatureImage::ImageDimension - 1][i] *= std::real(det);
        }

      if (principalMoments[0] != 0)
        {
        //    elongation = principalMoments[TFeatureImage::ImageDimension-1] / principalMoments[0];
          elongation = vcl_sqrt(principalMoments[TFeatureImage::ImageDimension - 1] / principalMoments[0]);
          }
        }
      else
        {
        // can't compute anything in that case - just set everything to a default value
        // Normalize using the total mass
        for (unsigned int i = 0; i < TFeatureImage::ImageDimension; ++i)
          {
          centerOfGravity[i] = 0;
          principalMoments[i] = 0;
          for (unsigned int j = 0; j < TFeatureImage::ImageDimension; ++j)
            {
            principalAxes[i][j] = 0;
            }
          }
        }


      oss.str("");
      oss << "STATS::" << m_FeatureName << "::Elongation";
      lo->SetAttribute(oss.str().c_str(), (double) elongation);

      oss.str("");
      oss << "STATS::" << m_FeatureName << "::Minimum";
      lo->SetAttribute(oss.str().c_str(), (double) min);

      oss.str("");
      oss << "STATS::" << m_FeatureName << "::Maximum";
      lo->SetAttribute(oss.str().c_str(), (double) max);

      oss.str("");
      oss << "STATS::" << m_FeatureName << "::Sum";
      lo->SetAttribute(oss.str().c_str(), sum);

      oss.str("");
      oss << "STATS::" << m_FeatureName << "::Sigma";
      lo->SetAttribute(oss.str().c_str(), sigma);

      for (unsigned int dim = 0; dim < TFeatureImage::ImageDimension; ++dim)
        {
        oss.str("");
        oss << "STATS::" << m_FeatureName << "::CenterOfGravity" << dim;
        lo->SetAttribute(oss.str().c_str(), centerOfGravity[dim]);

        oss.str("");
        oss << "STATS::" << m_FeatureName << "::PrincipalMoments" << dim;
        lo->SetAttribute(oss.str().c_str(), principalMoments[dim]);

        oss.str("");
        oss << "STATS::" << m_FeatureName << "::FirstMinimumIndex" << dim;
        lo->SetAttribute(oss.str().c_str(), minIdx[dim]);

        oss.str("");
        oss << "STATS::" << m_FeatureName << "::FirstMaximumIndex" << dim;
        lo->SetAttribute(oss.str().c_str(), maxIdx[dim]);

        for (unsigned int dim2 = 0; dim2 < TFeatureImage::ImageDimension; ++dim2)
          {
          oss.str("");
          oss << "STATS::" << m_FeatureName << "::PrincipalAxis" << dim << dim2;
          lo->SetAttribute(oss.str().c_str(), principalAxes(dim, dim2));
          }
        }
      }
}


/** Set the name of the feature */
template <class TLabelObject, class TFeatureImage>
void
StatisticsAttributesLabelObjectFunctor<TLabelObject, TFeatureImage>
::SetFeatureName(const std::string& name)
{
  m_FeatureName = name;
}

/** Get the feature name */
template <class TLabelObject, class TFeatureImage>
const std::string&
StatisticsAttributesLabelObjectFunctor<TLabelObject, TFeatureImage>
::GetFeatureName() const
{
  return m_FeatureName;
}

/** Set the feature image */
template <class TLabelObject, class TFeatureImage>
void
StatisticsAttributesLabelObjectFunctor<TLabelObject, TFeatureImage>
::SetFeatureImage(const TFeatureImage * img)
{
  m_FeatureImage = img;
}

/** Get the feature image */
template <class TLabelObject, class TFeatureImage>
const TFeatureImage *
StatisticsAttributesLabelObjectFunctor<TLabelObject, TFeatureImage>
::GetFeatureImage() const
{
  return m_FeatureImage;
}

/** Set the reduced attribute set */
template <class TLabelObject, class TFeatureImage>
void
StatisticsAttributesLabelObjectFunctor<TLabelObject, TFeatureImage>
::SetReducedAttributeSet(bool flag)
{
  m_ReducedAttributeSet = flag;
}

/** Get the reduced attribute set */
template <class TLabelObject, class TFeatureImage>
bool
StatisticsAttributesLabelObjectFunctor<TLabelObject, TFeatureImage>
::GetReducedAttributeSet() const
{
  return m_ReducedAttributeSet;
}

} // End namespace Functor

template <class TImage, class TFeatureImage>
StatisticsAttributesLabelMapFilter<TImage, TFeatureImage>
::StatisticsAttributesLabelMapFilter()
{}

template <class TImage, class TFeatureImage>
StatisticsAttributesLabelMapFilter<TImage, TFeatureImage>
::~StatisticsAttributesLabelMapFilter()
{}

/** Set the feature image */
template <class TImage, class TFeatureImage>
void
StatisticsAttributesLabelMapFilter<TImage, TFeatureImage>
::SetFeatureImage(const TFeatureImage *input)
{
  // Set the Nth input
  this->SetNthInput(1, const_cast<TFeatureImage*>(input));
}

/** Get the feature image */
template <class TImage, class TFeatureImage>
const typename StatisticsAttributesLabelMapFilter<TImage, TFeatureImage>
::FeatureImageType * StatisticsAttributesLabelMapFilter<TImage, TFeatureImage>
::GetFeatureImage() const
{
  return static_cast<const TFeatureImage *>(this->itk::ProcessObject::GetInput(1));
}

/** Set Input1 (for backward compatibility) */
template <class TImage, class TFeatureImage>
void
StatisticsAttributesLabelMapFilter<TImage, TFeatureImage>
::SetInput1(const TImage * input)
{
  this->SetInput(input);
}

/** Get Input1 (for backward compatibility) */
template <class TImage, class TFeatureImage>
const TImage *
StatisticsAttributesLabelMapFilter<TImage, TFeatureImage>
::GetInput1() const
{
  return this->GetInput();
}

/** Set Input2 (for backward compatibility) */
template <class TImage, class TFeatureImage>
void
StatisticsAttributesLabelMapFilter<TImage, TFeatureImage>
::SetInput2(const TFeatureImage * input)
{
  this->SetFeatureImage(input);
}

/** Get Input2 (for backward compatibility) */
template <class TImage, class TFeatureImage>
const TFeatureImage *
StatisticsAttributesLabelMapFilter<TImage, TFeatureImage>
::GetInput2() const
{
  return this->GetFeatureImage();
}

/** Set the name of the feature */
template <class TImage, class TFeatureImage>

void StatisticsAttributesLabelMapFilter<TImage, TFeatureImage>
::SetFeatureName(const std::string& name)
{
  if (name != this->GetFunctor().GetFeatureName())
    {
    this->GetFunctor().SetFeatureName(name);
    this->Modified();
    }
}

/** Get the feature name */
template <class TImage, class TFeatureImage>

const std::string& StatisticsAttributesLabelMapFilter<TImage, TFeatureImage>
::GetFeatureName() const
{
  return this->GetFunctor().GetFeatureName();
}

/** Set the reduced attribute set */
template <class TImage, class TFeatureImage>

void StatisticsAttributesLabelMapFilter<TImage, TFeatureImage>
::SetReducedAttributeSet(bool flag)
{
  if (this->GetFunctor().GetReducedAttributeSet() != flag)
    {
    this->GetFunctor().SetReducedAttributeSet(flag);
    this->Modified();
    }
}

/** Get the reduced attribute set */
template <class TImage, class TFeatureImage>

bool StatisticsAttributesLabelMapFilter<TImage, TFeatureImage>
::GetReducedAttributeSet() const
{
  return this->GetFunctor().GetReducedAttributeSet();
}

template <class TImage, class TFeatureImage>
void
StatisticsAttributesLabelMapFilter<TImage, TFeatureImage>
::BeforeThreadedGenerateData()
{
  // First call superclass implementation
  Superclass::BeforeThreadedGenerateData();

  // Set the feature image to the functor
  this->GetFunctor().SetFeatureImage(this->GetFeatureImage());
}

template <class TImage, class TFeatureImage>
void
StatisticsAttributesLabelMapFilter<TImage, TFeatureImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace itk
#endif
