/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbSFSTexturesFunctor_h
#define otbSFSTexturesFunctor_h

#include "otbMath.h"
#include "itkNumericTraits.h"
#include <vector>

namespace otb
{
/** \class SFSTexturesFunctor
 *  \brief This functor computes textures based on line direction analysis through the central pixel.
 *
 *  Directions are computed using NumberOfDirection, used to compute a constant step angle.
 *  A direction is defined as : \f$ \mathit{d_{i} = \sqrt{(m^{e1}-m{e2})^{2}+(n^{e1}-n{e2})^{2}}} \f$
 *  From  \f$ \mathit{d_{i}} \f$, histograms are defined :
 *  \f$ \mathit{H(c) : \{c \in I \mid \lbrack d_{1}(c), \ldots , d_{i}(c), \ldots , d_{D}(c)\rbrack  \}} \f$
 *  Thus, 6 textures are defined :
 *  \f$ \mathit{length = \max_{i \in \lbrack1; D\rbrack}(d_{i}(c)} \f$
 *  \f$ \mathit{width = \min_{i \in \lbrack1; D\rbrack}(d_{i}(c)} \f$
 *  \f$ \mathit{PSI = \frac{1}{D}\sum_{1=1}^{D}d_{i}(c)} \f$
 *  \f$ \mathit{\omega-mean = \frac{1}{D}\sum_{1=1}^{D}\frac{\alpha.(k_{i}-1)}{st_{i}}d_{i}(c)} \f$
 *  \f$ \mathit{ratio = \arctan{\frac{\sum_{j=1}^{n}{sort_{min}^{j}(H(c))}}{\sum_{j=1}^{n}{sort_{max}^{j}(H(c))}}}} \f$
 *  \f$ \mathit{SD = \frac{1}{D-1}\sqrt{\sum_{1=1}^{D}(d_{i}(c)-PSI)^{2}}} \f$
 *
 *  For more details, please refer to refer to Xin Huang, Liangpei Zhang and Pingxiang Li publication,
 *  Classification and Extraction of Spatial Features in Urban Areas
 *  Using High-Resolution Multispectral Imagery.
 *  IEEE Geoscience and Remote Sensing Letters,
 *  vol. 4, n. 2, 2007, pp 260-264
 *
 * \ingroup Textures
 *
 * \ingroup OTBTextures
 */

namespace Functor
{
template <class TIter, class TOutputValue>
class SFSTexturesFunctor
{
public:
  SFSTexturesFunctor()
  {
    m_SpatialThreshold            = 100;
    m_SpectralThreshold           = 50;
    m_RatioMaxConsiderationNumber = 5;
    m_Alpha                       = 1;
    this->SetNumberOfDirections(20); // set the step too
    m_SelectedTextures = std::vector<bool>(6, 1);
  }
  virtual ~SFSTexturesFunctor()
  {
  }

  typedef typename TIter::InternalPixelType InternalPixelType;
  typedef typename TIter::SizeType          SizeType;
  typedef typename TIter::IndexType         IndexType;
  typedef typename TIter::OffsetType        OffsetType;
  typedef TOutputValue                      OutputValueType;
  typedef std::vector<OutputValueType>      OutputType;

  void SetSpatialThreshold(unsigned int thresh)
  {
    m_SpatialThreshold = thresh;
  }
  void SetSpectralThreshold(InternalPixelType thresh)
  {
    m_SpectralThreshold = thresh;
  }
  void SetRatioMaxConsiderationNumber(unsigned int value)
  {
    m_RatioMaxConsiderationNumber = value;
  }
  void SetAlpha(double alpha)
  {
    m_Alpha = alpha;
  }
  void SetNumberOfDirections(unsigned int D)
  {
    m_NumberOfDirections = D;
    m_DirectionStep      = CONST_PI / static_cast<double>(D);
  }
  void SetDirectionStep(double step)
  {
    m_DirectionStep = step;
  }
  void SetSelectedTextures(std::vector<bool> vect)
  {
    m_SelectedTextures.clear();
    m_SelectedTextures = vect;
  }
  void SetTextureStatus(unsigned int id, bool isSelected)
  {
    m_SelectedTextures[id] = isSelected;
  }

  // Define some getters using itkGetMacro
  itkGetMacro(SpatialThreshold, unsigned int);
  itkGetMacro(SpectralThreshold, InternalPixelType);
  itkGetMacro(RatioMaxConsiderationNumber, unsigned int);
  itkGetMacro(Alpha, double);
  itkGetMacro(NumberOfDirections, unsigned int);

  std::vector<bool> GetTexturesStatus()
  {
    return m_SelectedTextures;
  }

  inline OutputType operator()(const TIter& it)
  {
    double                    length                   = itk::NumericTraits<double>::NonpositiveMin();
    double                    width                    = itk::NumericTraits<double>::max();
    double                    SpatialThresholdDouble   = static_cast<double>(m_SpatialThreshold);
    double                    NumberOfDirectionsDouble = static_cast<double>(m_NumberOfDirections);
    double                    dist                     = 0.;
    double                    angle                    = 0.;
    double                    sdiVal                   = 0.;
    double                    sumWMean                 = 0.;
    double                    sum                      = 0.;
    std::vector<double>       di(m_NumberOfDirections, 0.);
    std::vector<double>       minSorted(m_RatioMaxConsiderationNumber, width);
    std::vector<double>       maxSorted(m_RatioMaxConsiderationNumber, length);
    std::vector<double>       sti(m_NumberOfDirections, 0.);
    std::vector<unsigned int> lengthLine(m_NumberOfDirections, 0);

    std::vector<double>::iterator itVector;
    OutputType                    out(6, 0);

    OffsetType off;
    off.Fill(0);

    for (unsigned int d = 0; d < m_NumberOfDirections; d++)
    {
      // Current angle direction
      angle = m_DirectionStep * static_cast<double>(d);

      // last offset in the direction respecting spatial threshold
      off[0] = static_cast<int>(std::floor(SpatialThresholdDouble * std::cos(angle) + 0.5));
      off[1] = static_cast<int>(std::floor(SpatialThresholdDouble * std::sin(angle) + 0.5));
      // last indices in the direction respecting spectral threshold
      OffsetType offEnd = this->FindLastOffset(it, off);

      // Check the opposite side
      off[0] *= -1.0;
      off[1] *= -1.0;
      OffsetType offStart = this->FindLastOffset(it, off);

      // computes distance = dist between the 2 segment point.
      dist = std::sqrt(std::pow(static_cast<double>(offEnd[0] - offStart[0]), 2) + std::pow(static_cast<double>(offEnd[1] - offStart[1]), 2));

      // for length computation
      if (m_SelectedTextures[0] == true)
        if (dist > length)
          length = dist;

      // for width computation
      if (m_SelectedTextures[1] == true)
        if (dist < width)
          width = dist;

      // for PSI computation
      if (m_SelectedTextures[2] == true || m_SelectedTextures[5] == true)
        sum += dist;

      // for w-mean computation
      if (m_SelectedTextures[3] == true)
        sdiVal = this->ComputeSDi(it, offEnd);

      // for Ratio computation
      if (m_SelectedTextures[4] == true)
      {
        bool doo = false;
        itVector = maxSorted.begin();
        while (itVector != maxSorted.end() && doo == false)
        {
          if (dist > (*itVector))
          {
            itVector = maxSorted.insert(itVector, dist);
            maxSorted.pop_back();
            doo = true;
          }
          ++itVector;
        }
        doo      = false;
        itVector = minSorted.begin();
        while (itVector != minSorted.end() && doo == false)
        {
          if (dist < (*itVector))
          {
            itVector = minSorted.insert(itVector, dist);
            minSorted.pop_back();
            doo = true;
          }
          ++itVector;
        }
      }

      di[d] = dist;
      if (m_SelectedTextures[3] == true)
      {
        lengthLine[d] = static_cast<unsigned int>(
            dist); // static_cast<unsigned int>( std::sqrt(std::pow(static_cast<double>(offEnd[0]), 2) + std::pow(static_cast<double>(offEnd[1]), 2)) );
        sti[d] = sdiVal;
        if (sdiVal != 0.)
          sumWMean += (m_Alpha * (dist - 1) * dist /*lengthLine[n]*di[n]*/) / sdiVal;
      }
    }

    /////// FILL OUTPUT
    // length
    if (m_SelectedTextures[0] == true)
      out[0] = static_cast<OutputValueType>(length);
    // width
    if (m_SelectedTextures[1] == true)
      out[1] = static_cast<OutputValueType>(width);
    // PSI
    if (m_SelectedTextures[2] == true)
      out[2] = static_cast<OutputValueType>(sum / NumberOfDirectionsDouble);
    // w-mean
    if (m_SelectedTextures[3] == true)
      out[3] = static_cast<OutputValueType>(sumWMean / NumberOfDirectionsDouble);
    // ratio
    if (m_SelectedTextures[4] == true)
    {
      double sumMin = 0;
      double sumMax = 0;
      for (unsigned int t = 0; t < m_RatioMaxConsiderationNumber; t++)
      {
        sumMin += minSorted[t];
        sumMax += maxSorted[t];
      }
      if (sumMax != 0.)
        out[4] = static_cast<OutputValueType>(std::atan(sumMin / sumMax));
      else if (sumMax == 0. && sumMin == 0.)
        out[4] = static_cast<OutputValueType>(1.);
    }
    // SD
    if (m_SelectedTextures[5] == true)
    {
      double sumPSI = 0;
      for (unsigned int n = 0; n < di.size(); ++n)
        sumPSI += std::pow(di[n] - sumWMean / NumberOfDirectionsDouble, 2);
      out[5] = static_cast<OutputValueType>(std::sqrt(sumPSI) / (NumberOfDirectionsDouble - 1.));
    }

    return out;
  }

  /** Checks spectral threshold condition
   *  the last point in the directiuon is the first that doesn't
   *  respect the spectral condition.
   */
  OffsetType FindLastOffset(const TIter& it, const OffsetType& stopOffset)
  {
    bool res   = true;
    int  signX = this->ComputeStep(stopOffset[0]);
    int  signY = this->ComputeStep(stopOffset[1]);

    OffsetType currentOff;
    currentOff.Fill(0);
    currentOff[0] = signX;

    double slop = 0.;
    if (stopOffset[0] != 0)
      slop = static_cast<double>(stopOffset[1] / static_cast<double>(stopOffset[0]));

    bool isInside = true;
    while (isInside == true && res == true)
    {
      this->ComputePointLine(currentOff, slop, signY, stopOffset[0]);

      if (std::abs(it.GetPixel(currentOff) - it.GetCenterPixel()) > m_SpectralThreshold)
      {
        res = false;
      }
      else
        currentOff[0] += signX;

      isInside = this->CheckIsInside(signX, signY, currentOff, stopOffset);
    }

    return currentOff;
  }

  /** Computes SD in the ith direction */
  double ComputeSDi(const TIter& it, const OffsetType& stopOffset)
  {
    bool         canGo = true;
    unsigned int nbElt = 0;
    double       SDi   = 0.;
    double       mean  = 0.;
    double       slop  = 0.;
    if (stopOffset[0] != 0)
      slop = static_cast<double>(stopOffset[1] / static_cast<double>(stopOffset[0]));

    int signX = this->ComputeStep(stopOffset[0]);
    int signY = this->ComputeStep(stopOffset[1]);

    OffsetType currentOff;
    currentOff.Fill(0);
    currentOff[0] = signX;

    bool isInside = true;
    // First compute mean
    while (isInside == true && canGo == true)
    {
      this->ComputePointLine(currentOff, slop, signY, stopOffset[0]);

      mean += static_cast<double>(it.GetPixel(currentOff));
      nbElt++;

      if (std::abs(it.GetPixel(currentOff) - it.GetCenterPixel()) >= m_SpectralThreshold)
        canGo = false;
      else
        currentOff[0] += signX;

      isInside = this->CheckIsInside(signX, signY, currentOff, stopOffset);
    }

    mean /= static_cast<double>(nbElt);
    currentOff[0] = signX;
    currentOff[1] = 0;
    isInside      = true;

    while (isInside == true && canGo == true)
    {
      this->ComputePointLine(currentOff, slop, signY, stopOffset[0]);

      SDi += std::pow((static_cast<double>(it.GetPixel(currentOff)) - mean), 2);
      if (std::abs(it.GetPixel(currentOff) - it.GetCenterPixel()) >= m_SpectralThreshold)
        canGo = false;
      else
        currentOff[0] += signX;

      isInside = this->CheckIsInside(signX, signY, currentOff, stopOffset);
    }
    return std::sqrt(SDi);
  }

  /** Check if the current offset is inside the stop one. */
  bool CheckIsInside(const int& signX, const int& signY, const OffsetType& currentOff, const OffsetType& stopOffset)
  {
    bool isInside = true;
    if (signX * currentOff[0] >= signX * stopOffset[0] && stopOffset[0] != 0)
      isInside = false;
    else if (signY * currentOff[1] >= signY * stopOffset[1] && stopOffset[1] != 0)
      isInside = false;

    return isInside;
  }

  /** Compute the y coordinate according to a given x coordinate.
   *  Use the Bresenham algo if the line is not horizontal (stopOffsetX==0).
   *  Otherwise, it increments of 1 Y.
   */
  void ComputePointLine(OffsetType& currentOff, const double& slop, const int& signY, const int& stopOffsetX)
  {
    if (stopOffsetX != 0)
      currentOff[1] = static_cast<int>(std::floor(slop * static_cast<double>(currentOff[0]) + 0.5));
    else
      currentOff[1] += signY;
  }

  /** Compute the to give at x for line computation.
   *  according to the signof stopOffset.
   */
  int ComputeStep(const int& stopOffset)
  {
    int sign = 1;
    if (stopOffset < 0)
      sign = -1;

    return sign;
  }

protected:
  /** spectral threshold conditon*/
  InternalPixelType m_SpectralThreshold;
  /** spatial threshold condition */
  unsigned int m_SpatialThreshold;
  /** Max nulber of min and considered for Ration computation */
  unsigned int m_RatioMaxConsiderationNumber;
  /** constant to adjust w-mean values */
  double m_Alpha;
  /** Number of direction considered */
  unsigned int m_NumberOfDirections;
  /** Angular step between 2 directions (between  and 2*pi). */
  double m_DirectionStep;
  /** List of wanted textures :
   *  0: length
   *  1: width
   *  2: PSI
   *  3: w-mean
   *  4: ratio
   *  5: SD
   *  Set to 1 means the texture will be computed.
   **/
  std::vector<bool> m_SelectedTextures;
};

} // end namespace functor
} // end namespace otb

#endif
