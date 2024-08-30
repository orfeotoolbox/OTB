/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbMultiScaleConvexOrConcaveClassificationFilter_h
#define otbMultiScaleConvexOrConcaveClassificationFilter_h

#include "otbFunctorImageFilter.h"

namespace otb
{
namespace Functor
{
namespace MultiScaleConvexOrConcaveDecisionRule_tags
{
struct max_opening_profile_derivative
{
};
struct max_closing_profile_derivative
{
};
struct opening_profile_characteristics
{
};
struct closing_profile_characteristics
{
};

} // End namespace MultiScaleConvexOrConcaveDecisionRule_tags

/** \class MultiScaleConvexOrConcaveDecisionRule
 *  \brief This Functor apply a classification rule on two membership value along with two labels.
 *
 * Use otb::MultiScaleConvexOrConcaveClassificationFilter to apply it image-wise.
 *
 * \par
 * This algorithm is based on the following publication:
 * \par
 * Martino Pesaresi and Jon Alti Benediktsson, Member, IEEE:
 * A new approach for the morphological segmentation of high resolution
 * satellite imagery. IEEE Transactions on geoscience and remote sensing, vol. 39,
 * NO. 2, February 2001, p. 309-320.
 * \par
 *
 * Given \f$x_1\f$ and \f$x_2\f$ two membership values, \f$L_1, L_2\f$ two labels associated,
 * and \f$\sigma\f$ a tolerance value, the following decision rule is applied:
 *
 *  \f[
 *  L=\left\{\begin{array}{lcl}
 * L_{1} &:& x_{1}>x_{2} \quad and \quad x_{1}>\sigma\\
 * L_{2} &:& x_{2}>x_{1} \quad and \quad x_{2}>\sigma\\
 * 0   &:& otherwise.
 *  \end{array}\right.
 * \f]
 *
 * To help the distinction between \f$L_{1}\f$ labels and \f$L_{2}\f$,
 * this functor adds the m_LabelSeparators value to the \f$L_{1}\f$ labels.
 *
 * This functor is the decision rule used for multi-scale classification using
 * morphological profiles.
 *
 * \ingroup OTBMorphologicalProfiles
 */
template <class TInput, class TLabeled>
class MultiScaleConvexOrConcaveDecisionRule
{

public:
  /**
   * Constructor
   */
  MultiScaleConvexOrConcaveDecisionRule()
  {
    m_Sigma          = 0.0;
    m_LabelSeparator = 10;
  }
  /**
   * Destructor
   */
  virtual ~MultiScaleConvexOrConcaveDecisionRule()
  {
  }
  /**
   * Label the pixel to convex, concave or flat
   * \return The label of the pixel
   * \param opDeMax The max of the opening profile derivative
   * \param cloDeMax  The max of the closing profile derivative
   * \param opDeChar The characteristic of the opening profile
   * \param cloDeChar The characteristic of the closing profile
   */
  TLabeled operator()(const TInput& opDeMax, const TInput& cloDeMax, const TLabeled& opDeChar, const TLabeled& cloDeChar) const
  {
    TLabeled resp = 0;

    if (opDeMax > cloDeMax && static_cast<double>(opDeMax) > m_Sigma)
    {
      resp = m_LabelSeparator + opDeChar;
    }
    else if (cloDeMax > opDeMax && static_cast<double>(cloDeMax) > m_Sigma)
    {
      resp = cloDeChar;
    }
    return resp;
  }
  /**
   * Set the tolerance value
   * \param sigma the tolerance value
   */
  void SetSigma(const double& sigma)
  {
    m_Sigma = sigma;
  }
  /**
   * Get the tolerance value
   * \return the tolerance value
   */
  double GetSigma(void)
  {
    return m_Sigma;
  }
  /**
   * Set the label separator
   * \param labelSeparator the label separator
   */
  void SetLabelSeparator(const TLabeled& labelSeparator)
  {
    m_LabelSeparator = labelSeparator;
  }
  /**
   * Get the label separator
   * \return the label separator
   */
  TLabeled GetLabelSeparator(void)
  {
    return m_LabelSeparator;
  }

private:
  /** Sigma (tolerance) parameter */
  double m_Sigma;
  /** Separate between convex and concave labels */
  TLabeled m_LabelSeparator;
};
} // end namespace Functor

/** \typedef MultiScaleConvexOrConcaveClassificationFilter
 *  \brief Apply the otb::Functor::MultiScaleConvexOrConcaveDecisionRule to whole images.
 *
 * See otb::Functor::MultiScaleConvexOrConcaveDecisionRule functor documentation for more details.
 *
 * Set inputs with:
 * \code
 *
 * SetInput<MultiScaleConvexOrConcaveDecisionRule_tags::max_opening_profile_derivative>(in1);
 * SetInput<MultiScaleConvexOrConcaveDecisionRule_tags::max_closing_profile_derivative>(in2);
 * SetInput<MultiScaleConvexOrConcaveDecisionRule_tags::opening_profile_characteristics>(in3);
 * SetInput<MultiScaleConvexOrConcaveDecisionRule_tags::closing_profile_characteristics>(in4);
 *
 * \endcode
 *
 * \ingroup OTBMorphologicalProfiles
 */
template <class TInputImage, class TOutputImage>
using MultiScaleConvexOrConcaveClassificationFilter =
    FunctorImageFilter<Functor::MultiScaleConvexOrConcaveDecisionRule<typename TInputImage::PixelType, typename TOutputImage::PixelType>,
                       std::tuple<Functor::MultiScaleConvexOrConcaveDecisionRule_tags::max_opening_profile_derivative,
                                  Functor::MultiScaleConvexOrConcaveDecisionRule_tags::max_closing_profile_derivative,
                                  Functor::MultiScaleConvexOrConcaveDecisionRule_tags::opening_profile_characteristics,
                                  Functor::MultiScaleConvexOrConcaveDecisionRule_tags::closing_profile_characteristics>>;
} // End namespace otb
#endif
