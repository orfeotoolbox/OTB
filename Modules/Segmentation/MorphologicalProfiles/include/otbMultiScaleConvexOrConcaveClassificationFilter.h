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
#ifndef __otbMultiScaleConvexOrConcaveClassificationFilter_h
#define __otbMultiScaleConvexOrConcaveClassificationFilter_h

#include "otbQuaternaryFunctorImageFilter.h"
namespace otb
{
namespace Functor
{
/** \class MultiScaleConvexOrConcaveDecisionRule
 *  \brief This Functor apply a classification rule on two membership value along with two labels.
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
template<class TInput, class TLabeled>
class MultiScaleConvexOrConcaveDecisionRule
{

public:
  /**
   * Constructor
   */
  MultiScaleConvexOrConcaveDecisionRule()
  {
    m_Sigma = 0.0;
    m_LabelSeparator = 10;
  }
  /**
   * Destructor
   */
  virtual ~MultiScaleConvexOrConcaveDecisionRule() {}
  /**
   * Label the pixel to convex, concave or flat
   * \return The label of the pixel
   * \param opDeMax The max of the opening profile derivative
   * \param cloDeMax  The max of the closing profile derivative
   * \param opDeChar The characteristic of the opening profile
   * \param cloDeChar The characteristic of the closing profile
   */
  inline TLabeled operator ()(const TInput& opDeMax,
                              const TInput& cloDeMax,
                              const TLabeled& opDeChar,
                              const TLabeled& cloDeChar)
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
  /** Seperate between convex and concave labels */
  TLabeled m_LabelSeparator;

};
} //end namespace Functor

/** \class MultiScaleConvexOrConcaveClassificationFilter
 *  \brief Apply the MultiScaleConvexOrConcaveDecisionRule to whole images.
 *
 * See MultiScaleConvexOrConcaveDecisionRule functor documentation for more details.
 *
 *
 * \ingroup OTBMorphologicalProfiles
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT MultiScaleConvexOrConcaveClassificationFilter
  : public QuaternaryFunctorImageFilter<TInputImage, TInputImage, TOutputImage, TOutputImage, TOutputImage,
      Functor::MultiScaleConvexOrConcaveDecisionRule<typename TInputImage::PixelType,
          typename TOutputImage::PixelType> >
{
public:
  /** Standard typedefs */
  typedef MultiScaleConvexOrConcaveClassificationFilter Self;
  typedef QuaternaryFunctorImageFilter<TInputImage, TInputImage, TOutputImage, TOutputImage, TOutputImage,
      Functor::MultiScaleConvexOrConcaveDecisionRule<typename TInputImage::PixelType,
          typename TOutputImage::PixelType> >
  Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(MultiScaleConvexOrConcaveClassificationFilter, QuaternaryFunctorImageFilter);

  /** Template class typedef */
  typedef TInputImage                         InputImageType;
  typedef TOutputImage                        OutputImageType;
  typedef typename OutputImageType::PixelType LabelType;
  typedef Functor::MultiScaleConvexOrConcaveDecisionRule<typename TInputImage::PixelType,
      typename TOutputImage::PixelType> DecisionFunctorType;
  /**
   * Set the opening profile derivative maxima image
   * \param derivativeMaxima the opening profile derivative maxima image
   *
   */
  void SetOpeningProfileDerivativeMaxima(const TInputImage * derivativeMaxima)
  {
    this->SetInput1(derivativeMaxima);
  }
  /**
   * Set the opening profile characteristics image
   * \param characteristics the opening profile characteristics image
   *
   */
  void SetOpeningProfileCharacteristics(const TOutputImage * characteristics)
  {
    this->SetInput3(characteristics);
  }
  /**
   * Set the closing profile derivative maxima image
   * \param derivativeMaxima the closing profile derivative maxima image
   *
   */
  void SetClosingProfileDerivativeMaxima(const TInputImage * derivativeMaxima)
  {
    this->SetInput2(derivativeMaxima);
  }
  /**
   * Set the closing profile characteristics image
   * \param characteristics the closing profile characteristics image
   *
   */
  void SetClosingProfileCharacteristics(const TOutputImage * characteristics)
  {
    this->SetInput4(characteristics);
  }

  /** Set/Get the tolerance value */
  itkSetMacro(Sigma, double);
  itkGetMacro(Sigma, double);
  /** Set/Get the label separator */
  itkSetMacro(LabelSeparator, LabelType);
  itkGetMacro(LabelSeparator, LabelType);

  /** Set the functor parameters before calling the ThreadedGenerateData() */
  void BeforeThreadedGenerateData(void) ITK_OVERRIDE
  {
    this->GetFunctor().SetLabelSeparator(m_LabelSeparator);
    this->GetFunctor().SetSigma(m_Sigma);
  }

protected:
  /** Constructor */
  MultiScaleConvexOrConcaveClassificationFilter()
    {
    m_LabelSeparator = 10;
    m_Sigma          = 0.0;
    };
  /** Destructor */
  ~MultiScaleConvexOrConcaveClassificationFilter() ITK_OVERRIDE {}
  /**PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const ITK_OVERRIDE
  {
    Superclass::PrintSelf(os, indent);
    os << indent << "LabelSeparator: " << m_LabelSeparator << std::endl;
    os << indent << "Sigma: " << m_Sigma << std::endl;
  }

private:
  MultiScaleConvexOrConcaveClassificationFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  /** Label separator between convex and concave labels */
  LabelType m_LabelSeparator;
  /** Tolerance value */
  double m_Sigma;
};
} // End namespace otb
#endif
