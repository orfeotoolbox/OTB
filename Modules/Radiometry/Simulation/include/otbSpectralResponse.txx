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
#ifndef __otbSpectralResponse_txx
#define __otbSpectralResponse_txx

#include "itkNumericTraits.h"

#include "otbSpectralResponse.h"

#include <algorithm>

namespace otb
{

template<class TPrecision, class TValuePrecision>
SpectralResponse<TPrecision, TValuePrecision>::SpectralResponse()
{
  m_SensitivityThreshold = 0.01;
  m_IntervalComputed = false;
  m_PosGuess = 0;
  m_UsePosGuess=false;
}

template<class TPrecision, class TValuePrecision>
void SpectralResponse<TPrecision, TValuePrecision>::Load(const std::string & filename,
                                                         ValuePrecisionType coefNormalization)
{
  //Parse JPL file spectral response (ASCII file)
  //Begin line 27
  std::ifstream fin(filename.c_str());
  if (fin.fail())
    {
    itkExceptionMacro(<<"Error opening file" << filename);
    }

  int NumLigne = 26; // Go to the line 27
  //Ignore first 26th lines which are metadatas information
  for (int i = 0; i < NumLigne; ++i)
    fin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

  while (!fin.eof())
    {
    //For each
    std::pair<TPrecision, TValuePrecision> currentPair;

    fin >> currentPair.first;
    fin >> currentPair.second;
    currentPair.second = currentPair.second / coefNormalization;
    if (currentPair.first != itk::NumericTraits<TPrecision>::ZeroValue() && currentPair.second != itk::NumericTraits<
        TValuePrecision>::ZeroValue())
    //Add not null pair of values to the vector
    m_Response.push_back(currentPair);
    }
  fin.close();
  //Sort the vector using the specific functor sort_pair
  std::sort(m_Response.begin(), m_Response.end(), sort_pair());

  m_IntervalComputed = false;
}

template<class TPrecision, class TValuePrecision>
bool SpectralResponse<TPrecision, TValuePrecision>::Clear()
{
  m_Response.clear();
  m_IntervalComputed = false;
  return true;
}

template<class TPrecision, class TValuePrecision>
unsigned int SpectralResponse<TPrecision, TValuePrecision>::Size() const
{
  return m_Response.size();
}


template<class TPrecision, class TValuePrecision>
void SpectralResponse<TPrecision, TValuePrecision>::SetPosGuessMin(const PrecisionType & lambda)
{
  m_PosGuess = 0;
  if (m_Response.size() <= 1)
    {
    itkExceptionMacro(<<"ERROR spectral response need at least 2 value to perform interpolation.");
    }

  TPrecision lambdaMax = this->GetInterval().second;
  if (lambda > lambdaMax) return;
  typename VectorPairType::const_iterator it = m_Response.begin();

  while (((*it).first < lambda))
    {
    m_PosGuess++;
    ++it;
    if (it == (m_Response.end())) return;
    }

  if (m_PosGuess > 0) m_PosGuess--;
  return;
}


template<class TPrecision, class TValuePrecision>
inline typename SpectralResponse<TPrecision, TValuePrecision>::ValuePrecisionType SpectralResponse<TPrecision,
    TValuePrecision>::operator()(const PrecisionType & lambda)
{

  //Suppose that the vector is sorted

  //Guess a starting lambda
  if (m_Response.size() <= 1)
    {
    itkExceptionMacro(<<"ERROR spectral response need at least 2 value to perform interpolation.");
    }

  typename VectorPairType::const_iterator beg = m_Response.begin();
  typename VectorPairType::const_iterator last = m_Response.end();
  --last;

  TPrecision lambdaMin = this->GetInterval().first;
  TPrecision lambdaMax = this->GetInterval().second;

  if (lambda < lambdaMin) return static_cast<TValuePrecision> (0.0);
  if (lambda > lambdaMax) return static_cast<TValuePrecision> (0.0);

  typename VectorPairType::const_iterator it;

  if(m_UsePosGuess)
    it= beg + m_PosGuess;
  else
    it= beg;

  TPrecision lambda1 = (*beg).first;
  TValuePrecision SR1 = static_cast<TValuePrecision> (0.0);
  while (((*it).first < lambda))
    {

    lambda1 = (*it).first;
    SR1 = (*it).second;
    ++it;
    if (it == (m_Response.end()))
      {
      return static_cast<TValuePrecision> (0.0);
      }
    }
  TPrecision lambda2 = (*it).first;

  // if the guess is just right
  if (lambda2 == lambda)
    {
    return (*it).second;
    }
  else
    {

    TPrecision lambdaDist = lambda - lambda1;
    TPrecision ratio = lambdaDist / (lambda2 - lambda1);

    TValuePrecision SR2 = (*it).second;

    return static_cast<TValuePrecision> (ratio * SR1 + (1 - ratio) * SR2);

    }

}

template<class TPrecision, class TValuePrecision>
typename SpectralResponse<TPrecision, TValuePrecision>::ImagePointerType SpectralResponse<TPrecision, TValuePrecision>::GetImage(
                                                                                                                                 ImagePointerType image) const
{
  typename ImageType::IndexType start;
  start[0] = 0;
  start[1] = 0;

  typename ImageType::SizeType size;
  size[0] = 1;
  size[1] = 1;

  typename ImageType::PointType origin;
  origin[0] = 0;
  origin[1] = 0;

  typename ImageType::SpacingType spacing;
  spacing[0] = 1;
  spacing[1] = 1;

  typename ImageType::RegionType region;
  region.SetSize(size);
  region.SetIndex(start);

  image->SetRegions(region);
  image->SetNumberOfComponentsPerPixel(this->Size());
  image->Allocate();

  typename ImageType::IndexType idx;
  typename ImageType::PixelType pixel;
  pixel.SetSize(this->Size());

  for (unsigned int j = 0; j < this->Size(); ++j)
    {
    pixel[j] = m_Response[j].second;
    }
  idx[0] = 0;
  idx[1] = 0;
  image->SetPixel(idx, pixel);
  return image;
}

template<class TPrecision, class TValuePrecision>
void SpectralResponse<TPrecision, TValuePrecision>::SetFromImage(ImagePointerType image)
{

  typename ImageType::IndexType idx;
  idx[0] = 0;
  idx[1] = 0;

  for (unsigned int j = 0; j < this->Size(); ++j)
    {
    m_Response[j].second = image->GetPixel(idx)[j];
    }
  m_IntervalComputed = false;
}

template<class TPrecision, class TValuePrecision>
typename SpectralResponse<TPrecision, TValuePrecision>::FilterFunctionValuesPointerType SpectralResponse<TPrecision,
    TValuePrecision>::GetFilterFunctionValues(double step)
{

  //Assume that the SR is sorted
  typename FilterFunctionValuesType::ValuesVectorType valuesVector;
  Self& responseCalculator = *this;
  for (double i = m_Response.front().first; i <= m_Response.back().first; i += step)
    {
    valuesVector.push_back(responseCalculator(i));
    }
  FilterFunctionValuesPointerType functionValues = FilterFunctionValuesType::New();

  functionValues->SetFilterFunctionValues(valuesVector);
  functionValues->SetMinSpectralValue(m_Response.front().first);
  functionValues->SetMaxSpectralValue(m_Response.back().first);
  functionValues->SetUserStep(step);

  return functionValues;
}

template<class TPrecision, class TValuePrecision>
void SpectralResponse<TPrecision, TValuePrecision>::ComputeInterval()
{
  typename VectorPairType::const_iterator it = m_Response.begin();

  while ((*it).second <= m_SensitivityThreshold)
    {
    ++it;
    if (it == (m_Response.end() - 1))
      {
      m_Interval.first = static_cast<TPrecision> (0.0);
      m_Interval.second = static_cast<TPrecision> (0.0);
      m_IntervalComputed = true;
      return;
      }

    }
  m_Interval.first = (*it).first;
  it = (m_Response.end() - 1);
  while ((*it).second <= m_SensitivityThreshold)
    {
    if (it == (m_Response.begin()))
      {
      m_Interval.second = (*it).first;
      m_IntervalComputed = true;
      return;
      }
    --it;

    }

  m_Interval.second = (*it).first;
  m_IntervalComputed = true;
}

template<class TPrecision, class TValuePrecision>
void SpectralResponse<TPrecision, TValuePrecision>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << std::endl;
  os << indent << "[Wavelength (micrometers), Reflectance (percent)]" << std::endl;
  for (typename VectorPairType::const_iterator it = m_Response.begin(); it != m_Response.end(); ++it)
    {
    os << indent << "Num " << it - m_Response.begin() << ": [" << (*it).first << "," << (*it).second << "]"
        << std::endl;
    }
}

} // end namespace otb

#endif
