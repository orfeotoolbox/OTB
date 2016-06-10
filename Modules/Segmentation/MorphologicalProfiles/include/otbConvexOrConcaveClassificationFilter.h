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
#ifndef __otbConvexOrConcaveClassificationFilter_h
#define __otbConvexOrConcaveClassificationFilter_h

#include "itkBinaryFunctorImageFilter.h"
namespace otb
{
namespace Functor
{
/** \class ConvexOrConcaveDecisionRule
 *  \brief This functor labels the given pixel between three classes Convex, Concave and Flat.
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
 * This functor implements the following decision rule to classify a pixel between the three classes
 * Convex, Concave and Flat. Let \f$f\f$ denote the input image and \f$\psi_{N}(f)\f$ the geodesic leveling
 * of \f$f\f$ with a structuring element of size \f$N\f$. One can derive the following decision rule to
 * classify \f$f\f$ into Convex (label \f$\stackrel{\smile}{k}\f$), Concave (label \f$\stackrel{\frown}{k}\f$)
 * and Flat (label \f$\bar{k}\f$):
 *
 * \f[
 * \phi_{N}^{\sigma}=\left\{\begin{array}{lcl}
 * \stackrel{\smile}{k} &:& f-\psi_{N}(f)>\sigma\\
 * \stackrel{\frown}{k} &:& \psi_{N}(f)-f>\sigma\\
 *  \bar{k}&:&\mid f - \psi_{N}(f) \mid \leq \sigma
 *  \end{array}\right.
 * \f]
 *
 *
 * \ingroup OTBMorphologicalProfiles
 */
template<class TInput, class TOutput>
class ConvexOrConcaveDecisionRule
{

public:
  /**
   * Constructor
   */
  ConvexOrConcaveDecisionRule()
  {
    m_ConvexLabel = 1;
    m_ConcaveLabel = 2;
    m_FlatLabel = 0;
    m_Sigma = 0.0;
  }
  /**
   * Destructor
   */
  virtual ~ConvexOrConcaveDecisionRule() {}
  /**
   * Label the pixel to convex, concave or flat
   * \return The label of the pixel
   * \param x The image value
   * \param xlevel The leveling value
   */
  inline TOutput operator ()(const TInput& x, const TInput& xlevel)
  {
    TOutput resp = m_FlatLabel;

    if (static_cast<double>(x - xlevel) > m_Sigma)
      {
      resp = m_ConvexLabel;
      }
    else if (static_cast<double>(xlevel - x) > m_Sigma)
      {
      resp = m_ConcaveLabel;
      }
    return resp;
  }
  /**
   * Set the convex label
   * \param label the convex label
   */
  void SetConvexLabel(const TOutput& label)
  {
    m_ConvexLabel = label;
  }
  /**
   * Get the convex label
   * \return the convex label
   */
  TOutput GetConvexLabel(void)
  {
    return m_ConvexLabel;
  }
  /**
   * Set the concave label
   * \param label the concave label
   */
  void SetConcaveLabel(const TOutput& label)
  {
    m_ConcaveLabel = label;
  }
  /**
   * Get the concave label
   * \return the concave label
   */
  TOutput GetConcaveLabel(void)
  {
    return m_ConcaveLabel;
  }
  /**
   * Set the flat label
   * \param label the flat label
   */
  void SetFlatLabel(const TOutput& label)
  {
    m_FlatLabel = label;
  }
  /**
   * Get the flat label
   * \return the flat label
   */
  TOutput GetFlatLabel(void)
  {
    return m_FlatLabel;
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
private:
  /** Label of the convex class */
  TOutput m_ConvexLabel;
  /** Label of the concave class */
  TOutput m_ConcaveLabel;
  /** Label of the flat class */
  TOutput m_FlatLabel;
  /** Sigma (tolerance) parameter */
  double m_Sigma;

};
} //end namespace Functor

/** \class ConvexOrConcaveClassificationFilter
 *  \brief This filter apply the Convex, Concave and flat classification functor to the whole image.
 *
 *  Please refer to the documentation of the Functor::ConvexOrConcaveDecisionRule for more details.
 *
 * \sa GeodesicMorphologyDecompositionImageFilter
 * \sa GeodesicMorphologyLevelingFilter
 *
 * \ingroup OTBMorphologicalProfiles
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT ConvexOrConcaveClassificationFilter
  : public itk::BinaryFunctorImageFilter<TInputImage, TInputImage, TOutputImage,
      Functor::ConvexOrConcaveDecisionRule<typename TInputImage::PixelType,
          typename TOutputImage::PixelType> >
{
public:
  /** Standard typedefs */
  typedef ConvexOrConcaveClassificationFilter Self;
  typedef itk::BinaryFunctorImageFilter<TInputImage, TInputImage, TOutputImage,
      Functor::ConvexOrConcaveDecisionRule<typename TInputImage::PixelType,
          typename TOutputImage::PixelType> >
  Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(ConvexOrConcaveClassificationFilter, BinaryFunctorImageFilter);

  /** Template class typedef */
  typedef TInputImage                         InputImageType;
  typedef TOutputImage                        OutputImageType;
  typedef typename OutputImageType::PixelType LabelType;
  typedef Functor::ConvexOrConcaveDecisionRule<typename TInputImage::PixelType,
      typename TOutputImage::PixelType> DecisionFunctorType;
  /**
   * Set the input image
   * \param image the input image
   *
   */
  using Superclass::SetInput;
  void SetInput(const TInputImage * image) ITK_OVERRIDE
  {
    this->SetInput1(image);
  }
  /**
   * Set the input leveling image
   * \param leveling the input leveling image
   *
   */
  void SetInputLeveling(const TInputImage * leveling)
  {
    this->SetInput2(leveling);
  }
  /** Set/Get the convex label */
  itkSetMacro(ConvexLabel, LabelType);
  itkGetMacro(ConvexLabel, LabelType);
  /** Set/Get the concave label */
  itkSetMacro(ConcaveLabel, LabelType);
  itkGetMacro(ConcaveLabel, LabelType);
  /** Set/Get the flat label */
  itkSetMacro(FlatLabel, LabelType);
  itkGetMacro(FlatLabel, LabelType);
  /** Set/Get the tolerance value */
  itkSetMacro(Sigma, double);
  itkGetMacro(Sigma, double);

  /** Set the functor parameters before calling the ThreadedGenerateData() */
  void BeforeThreadedGenerateData(void) ITK_OVERRIDE
  {
    this->GetFunctor().SetConvexLabel(m_ConvexLabel);
    this->GetFunctor().SetConcaveLabel(m_ConcaveLabel);
    this->GetFunctor().SetFlatLabel(m_FlatLabel);
    this->GetFunctor().SetSigma(m_Sigma);
  }

protected:
  /** Constructor */
  ConvexOrConcaveClassificationFilter()
    {
    m_ConvexLabel  = 1;
    m_ConcaveLabel = 2;
    m_FlatLabel    = 0;
    m_Sigma        = 0.0;
    };
  /** Destructor */
  ~ConvexOrConcaveClassificationFilter() ITK_OVERRIDE {}
  /**PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const ITK_OVERRIDE
  {
    Superclass::PrintSelf(os, indent);
    os << indent << "ConvexLabel: " << m_ConvexLabel << std::endl;
    os << indent << "ConcaveLabel: " << m_ConcaveLabel << std::endl;
    os << indent << "FlatLabel: " << m_FlatLabel << std::endl;
    os << indent << "Sigma: " << m_Sigma << std::endl;
  }

private:
  ConvexOrConcaveClassificationFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  /** Label of the convex class */
  LabelType m_ConvexLabel;
  /** Label of the concave class */
  LabelType m_ConcaveLabel;
  /** Label of the flat class */
  LabelType m_FlatLabel;
  /** Tolerance value */
  double m_Sigma;
};
} // End namespace otb
#endif
