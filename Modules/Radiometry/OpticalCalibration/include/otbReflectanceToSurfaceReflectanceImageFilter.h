/*
 * Copyright (C) 1999-2011 Insight Software Consortium
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbReflectanceToSurfaceReflectanceImageFilter_h
#define otbReflectanceToSurfaceReflectanceImageFilter_h

#include "otbUnaryImageFunctorWithVectorImageFilter.h"

#include "otbRadiometryCorrectionParametersToAtmosphericRadiativeTerms.h"
#include "otbAtmosphericCorrectionParameters.h"

#include "otbMacro.h"
#include "itkMetaDataDictionary.h"

#include <iomanip>

namespace otb
{
namespace Functor
{
/**
   * \class ReflectanceToSurfaceReflectanceImageFunctor
   *  \brief Compute the surface reflectance pixel from a TOA reflectance.
   *
   * \ingroup Functor
   * \ingroup Radiometry
   *
 *
 * \ingroup OTBOpticalCalibration
 */
template <class TInput, class TOutput>
class ReflectanceToSurfaceReflectanceImageFunctor
{
public:
  ReflectanceToSurfaceReflectanceImageFunctor()
  {
    m_Coefficient = 1.;
    m_Residu = 1.;
    m_SphericalAlbedo = 1.;
  }
  virtual ~ReflectanceToSurfaceReflectanceImageFunctor() {}

  /**
   * Set/Get the spherical albedo of the atmosphere.
   */
  void SetSphericalAlbedo(double albedo)
  {
    m_SphericalAlbedo = albedo;
  }
  double GetSphericalAlbedo()
  {
    return m_SphericalAlbedo;
  }

  /**
   * Set/Get Coefficient, computed from AtmosphericRadiativeTermsPointerType datas.
   */
  void SetCoefficient(double coef)
  {
    m_Coefficient = coef;
  }
  double GetCoefficient()
  {
    return m_Coefficient;
  }

  /**
   * Set/Get Residu, computed from AtmosphericRadiativeTermsPointerType datas.
   */
  void SetResidu(double res)
  {
    m_Residu = res;
  }
  double GetResidu()
  {
    return m_Residu;
  }

  inline TOutput operator ()(const TInput& inPixel)
  {
    TOutput outPixel;
    double  temp, temp2;
    temp = (static_cast<double>(inPixel) + m_Residu)* m_Coefficient;
    temp2 =  temp / (1. + m_SphericalAlbedo *  temp);

    outPixel = static_cast<TOutput>(temp2);

    return outPixel;
  }

private:
  double m_SphericalAlbedo;
  double m_Coefficient;
  double m_Residu;
};
}

/** \class ReflectanceToSurfaceReflectanceImageFilter
 *  \brief Calculates the slope, the orientation incidence and exitance radius values for each pixel.
 *
 *
 * \ingroup AtmosphericRadiativeTerms
 * \ingroup AtmosphericCorrectionParameters
 * \ingroup RadianceToReflectanceImageFilter
 * \ingroup ImageToReflectanceImageFilter
 * \ingroup ImageMetadataCorrectionParameters
 * \ingroup otbRadiometryCorrectionParametersToAtmosphericRadiativeTerms
 * \ingroup Radiometry
 *
 * \example Radiometry/AtmosphericCorrectionSequencement.cxx
 *
 * \ingroup OTBOpticalCalibration
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT ReflectanceToSurfaceReflectanceImageFilter :
  public UnaryImageFunctorWithVectorImageFilter<TInputImage,
      TOutputImage,
      typename Functor::ReflectanceToSurfaceReflectanceImageFunctor<
          typename TInputImage::InternalPixelType,
          typename
          TOutputImage::InternalPixelType> >
{
public:
  /** Extract input and output images dimensions.*/
  itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int, TOutputImage::ImageDimension);

  /** "typedef" to simplify the variables definition and the declaration. */
  typedef TInputImage  InputImageType;
  typedef TOutputImage OutputImageType;

  typedef typename Functor::ReflectanceToSurfaceReflectanceImageFunctor<typename InputImageType::InternalPixelType,
      typename OutputImageType::InternalPixelType>
  FunctorType;
  /** "typedef" for standard classes. */
  typedef ReflectanceToSurfaceReflectanceImageFilter                                           Self;
  typedef UnaryImageFunctorWithVectorImageFilter<InputImageType, OutputImageType, FunctorType> Superclass;
  typedef itk::SmartPointer<Self>                                                              Pointer;
  typedef itk::SmartPointer<const Self>                                                        ConstPointer;

  /** object factory method. */
  itkNewMacro(Self);

  /** return class name. */
  itkTypeMacro(ReflectanceToSurfaceReflectanceImageFilter, UnaryImageFunctorWithVectorImageFilter);

  /** Supported images definition. */
  typedef typename InputImageType::PixelType          InputPixelType;
  typedef typename InputImageType::InternalPixelType  InputInternalPixelType;
  typedef typename InputImageType::RegionType         InputImageRegionType;
  typedef typename OutputImageType::PixelType         OutputPixelType;
  typedef typename OutputImageType::InternalPixelType OutputInternalPixelType;
  typedef typename OutputImageType::RegionType        OutputImageRegionType;


  typedef otb::RadiometryCorrectionParametersToAtmosphericRadiativeTerms     CorrectionParametersToRadiativeTermsType;

  typedef otb::AtmosphericCorrectionParameters                               AtmoCorrectionParametersType;
  typedef typename AtmoCorrectionParametersType::Pointer                    AtmoCorrectionParametersPointerType;

  typedef otb::ImageMetadataCorrectionParameters                             AcquiCorrectionParametersType;
  typedef typename AcquiCorrectionParametersType::Pointer                   AcquiCorrectionParametersPointerType;

  typedef otb::AtmosphericRadiativeTerms                                     AtmosphericRadiativeTermsType;
  typedef typename AtmosphericRadiativeTermsType::Pointer                   AtmosphericRadiativeTermsPointerType;


  typedef otb::FilterFunctionValues                                     FilterFunctionValuesType;
  typedef FilterFunctionValuesType::WavelengthSpectralBandType          ValueType;                //float
  typedef FilterFunctionValuesType::ValuesVectorType                    ValuesVectorType;         //std::vector<float>

  typedef typename AcquiCorrectionParametersType::WavelengthSpectralBandVectorType        WavelengthSpectralBandVectorType;


  typedef itk::MetaDataDictionary MetaDataDictionaryType;

  /** Get/Set Atmospheric Radiative Terms. */
  void SetAtmosphericRadiativeTerms(AtmosphericRadiativeTermsPointerType atmoRadTerms)
  {
    m_AtmosphericRadiativeTerms = atmoRadTerms;
    this->SetNthInput(1, m_AtmosphericRadiativeTerms);
    m_IsSetAtmosphericRadiativeTerms = true;
    this->Modified();
  }
  itkGetObjectMacro(AtmosphericRadiativeTerms, AtmosphericRadiativeTermsType);

  /** Get/Set Atmospheric Correction Parameters. */
  void SetAtmoCorrectionParameters(AtmoCorrectionParametersPointerType atmoCorrTerms)
  {
    m_AtmoCorrectionParameters = atmoCorrTerms;
    this->SetNthInput(2, m_AtmoCorrectionParameters);
    m_IsSetAtmoCorrectionParameters = true;
    this->Modified();
  }
  itkGetObjectMacro(AtmoCorrectionParameters, AtmoCorrectionParametersType);

  /** Get/Set Acquisition Correction Parameters. */
  void SetAcquiCorrectionParameters(AcquiCorrectionParametersPointerType acquiCorrTerms)
  {
    m_AcquiCorrectionParameters = acquiCorrTerms;
    this->SetNthInput(3, m_AcquiCorrectionParameters);
    m_IsSetAcquiCorrectionParameters = true;
    this->Modified();
  }
  itkGetObjectMacro(AcquiCorrectionParameters, AcquiCorrectionParametersType);


  /** Compute radiative terms if necessary and then update functors attributs. */
  void GenerateParameters();

  /** Set/Get UseGenerateParameters. */
  itkSetMacro(UseGenerateParameters, bool);
  itkGetMacro(UseGenerateParameters, bool);
  itkBooleanMacro(UseGenerateParameters);

  /** Set/Get IsSetAtmosphericRadiativeTerms */
  itkSetMacro(IsSetAtmosphericRadiativeTerms, bool);
  itkGetMacro(IsSetAtmosphericRadiativeTerms, bool);
  itkBooleanMacro(IsSetAtmosphericRadiativeTerms);

protected:
  /** Constructor */
  ReflectanceToSurfaceReflectanceImageFilter();
  /** Destructor */
  ~ReflectanceToSurfaceReflectanceImageFilter() override {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  /** Initialize the parameters of the functor before the threads run. */
  void BeforeThreadedGenerateData() override;
  /** Fill AtmosphericRadiativeTerms using image metadata*/
  void UpdateAtmosphericRadiativeTerms();
  /** Update Functors parameters */
  void UpdateFunctors();

  /** If modified, we need to compute the functor parameters again */
  void Modified() const override;

private:

  bool m_IsSetAtmosphericRadiativeTerms;
  bool m_IsSetAtmoCorrectionParameters;
  bool m_IsSetAcquiCorrectionParameters;

  /** Enable/Disable GenerateParameters in GenerateOutputInformation.
   *  Useful for image view that call GenerateOutputInformation each time you move the full area.
   */
  bool m_UseGenerateParameters;

  /** True if the functor parameters have been generated */
  mutable bool m_FunctorParametersHaveBeenComputed;

  AtmosphericRadiativeTermsPointerType     m_AtmosphericRadiativeTerms;
  AtmoCorrectionParametersPointerType      m_AtmoCorrectionParameters;
  AcquiCorrectionParametersPointerType     m_AcquiCorrectionParameters;


};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbReflectanceToSurfaceReflectanceImageFilter.hxx"
#endif

#endif
