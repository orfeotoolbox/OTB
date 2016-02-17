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
#ifndef __otbSurfaceAdjacencyEffectCorrectionSchemeFilter_h
#define __otbSurfaceAdjacencyEffectCorrectionSchemeFilter_h

#include "itkNumericTraits.h"
#include <vector>
#include "otbUnaryFunctorNeighborhoodImageFilter.h"
#include "itkVariableSizeMatrix.h"
#include "otbRadiometryCorrectionParametersToAtmosphericRadiativeTerms.h"
#include <iomanip>

namespace otb
{
namespace Functor
{
/** \class ComputeNeighborhoodContributionFunctor
*  \brief Unary neighborhood functor to compute the value of a pixel which is a sum
*   of the surrounding pixels value ponderated by a coefficient.
*
*  \ingroup Functor
* \ingroup Radiometry
 *
 * \ingroup OTBOpticalCalibration
*/
template <class TNeighIter, class TOutput>
class ComputeNeighborhoodContributionFunctor
{
public:
  ComputeNeighborhoodContributionFunctor() {}
  virtual ~ComputeNeighborhoodContributionFunctor() {}

  typedef itk::VariableSizeMatrix<double>           WeightingMatrixType;
  typedef typename std::vector<WeightingMatrixType> WeightingValuesContainerType;
  typedef typename TOutput::RealValueType           RealValueType;
  typedef std::vector<double>                       DoubleContainerType;

  void SetWeightingValues(const WeightingValuesContainerType& cont)
  {
    m_WeightingValues = cont;
  }
  void SetUpwardTransmittanceRatio(DoubleContainerType upwardTransmittanceRatio)
  {
    m_UpwardTransmittanceRatio = upwardTransmittanceRatio;
  }
  void SetDiffuseRatio(DoubleContainerType diffuseRatio)
  {
    m_DiffuseRatio = diffuseRatio;
  }
  WeightingValuesContainerType GetWeightingValues()
  {
    return m_WeightingValues;
  }
  DoubleContainerType GetUpwardTransmittanceRatio()
  {
    return m_UpwardTransmittanceRatio;
  }
  DoubleContainerType GetDiffuseRatio()
  {
    return m_DiffuseRatio;
  }

  inline TOutput operator ()(const TNeighIter& it)
  {
    unsigned int neighborhoodSize = it.Size();
    double       contribution = 0.;
    TOutput      outPixel;
    outPixel.SetSize(it.GetCenterPixel().Size());

    // Loop over each component
    const unsigned int size = outPixel.GetSize();
    for (unsigned int j = 0; j < size; ++j)
    {
      contribution = 0;
      // Load the current channel ponderation value matrix
      WeightingMatrixType TempChannelWeighting = m_WeightingValues[j];
      // Loop over the neighborhood
      for (unsigned int i = 0; i < neighborhoodSize; ++i)
      {
        // Current neighborhood pixel index calculation
        unsigned int RowIdx = 0;
        unsigned int ColIdx = 0;
        RowIdx = i / TempChannelWeighting.Cols();
        ColIdx = i - RowIdx*TempChannelWeighting.Cols();
   
        // Extract the current neighborhood pixel ponderation
        double idVal = TempChannelWeighting(RowIdx, ColIdx);
        // Extract the current neighborhood pixel value
        TOutput tempPix = it.GetPixel(i);

        contribution += static_cast<double>(tempPix[j]) * idVal;

        }
        
        outPixel[j] = static_cast<RealValueType>(it.GetCenterPixel()[j]) *m_UpwardTransmittanceRatio[j]
                        + contribution * m_DiffuseRatio[j];
      }
    return outPixel;
  }

private:
  WeightingValuesContainerType m_WeightingValues;
  DoubleContainerType          m_UpwardTransmittanceRatio;
  DoubleContainerType          m_DiffuseRatio;
};

}

/** \class SurfaceAdjacencyEffectCorrectionSchemeFilter
 *  \brief Correct the scheme taking care of the surrounding pixels.
 *
 *   The SurfaceAdjacencyEffectCorrectionSchemeFilter class allows introducing a neighbor correction to the
 *   reflectance estimation. The satelite signal is considered as to be a combinaison of the signal coming from
 *   the target pixel and a weighting of the siganls coming from the neighbor pixels.
 *
 * \ingroup Radiometry
 *
 *
 * \ingroup OTBOpticalCalibration
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT SurfaceAdjacencyEffectCorrectionSchemeFilter :
  public UnaryFunctorNeighborhoodImageFilter<
      TInputImage,
      TOutputImage,
      typename Functor::ComputeNeighborhoodContributionFunctor<itk::
          ConstNeighborhoodIterator <TInputImage>,
      typename TOutputImage::PixelType> >
{
public:
  /** "typedef" to simplify the variables definition and the declaration. */
  typedef Functor::ComputeNeighborhoodContributionFunctor<itk::ConstNeighborhoodIterator<TInputImage>,
      typename TOutputImage::PixelType> FunctorType;

  /** "typedef" for standard classes. */
  typedef SurfaceAdjacencyEffectCorrectionSchemeFilter                              Self;
  typedef UnaryFunctorNeighborhoodImageFilter<TInputImage, TOutputImage, FunctorType> Superclass;
  typedef itk::SmartPointer<Self>                                                     Pointer;
  typedef itk::SmartPointer<const Self>                                               ConstPointer;

  typedef typename Superclass::InputImageType  InputImageType;
  typedef typename Superclass::OutputImageType OutputImageType;

  typedef std::vector<double> DoubleContainerType;
  /** object factory method. */
  itkNewMacro(Self);

  /** return class name. */
  itkTypeMacro(SurfaceAdjacencyEffectCorrectionSchemeFilter, UnaryFunctorNeighborhoodImageFilter);

  /**   Extract input and output images dimensions.*/
  itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int, TOutputImage::ImageDimension);

  /** Supported images definition. */
  typedef typename InputImageType::PixelType          InputPixelType;
  typedef typename InputImageType::InternalPixelType  InputInternalPixelType;
  typedef typename InputImageType::RegionType         InputImageRegionType;
  typedef typename InputImageType::SizeType           SizeType;
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

  /** Storage ponderation values types*/
  typedef itk::VariableSizeMatrix<double>           WeightingMatrixType;
  typedef typename std::vector<WeightingMatrixType> WeightingValuesContainerType;

  /** typedef for calculation*/
  typedef typename itk::ConstNeighborhoodIterator<InputImageType> NeighborIterType;

  /** Set/Get the Size of the neighbor window. */
  void SetWindowRadius(unsigned int rad)
  {
    this->SetRadius(rad);
    m_WindowRadius = rad;
    this->Modified();
  }
  itkGetConstReferenceMacro(WindowRadius, unsigned int);

  /** Set/Get the pixel spacing in kilometers */
  itkSetMacro(PixelSpacingInKilometers, double);
  itkGetMacro(PixelSpacingInKilometers, double);
  /** Set/Get the viewing angle */
  itkSetMacro(ZenithalViewingAngle, double);
  itkGetMacro(ZenithalViewingAngle, double);


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


  /** Compute radiative terms if necessary and then update functors attibuts. */
  void GenerateParameters();


  /** Set/Get IsSetAtmosphericRadiativeTerms */
  itkSetMacro(IsSetAtmosphericRadiativeTerms, bool);
  itkGetMacro(IsSetAtmosphericRadiativeTerms, bool);
  itkBooleanMacro(IsSetAtmosphericRadiativeTerms);

protected:
  SurfaceAdjacencyEffectCorrectionSchemeFilter();
  virtual ~SurfaceAdjacencyEffectCorrectionSchemeFilter() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** Initialize the parameters of the functor before the threads run. */
  virtual void BeforeThreadedGenerateData();

  /** Fill AtmosphericRadiativeTerms using image metadata*/
  void UpdateAtmosphericRadiativeTerms();

  /** Compute the functor parameters */
  void UpdateFunctors();

  /** If modified, we need to compute the functor parameters again */
  virtual void Modified() const;

private:

  bool m_IsSetAtmosphericRadiativeTerms;
  bool m_IsSetAtmoCorrectionParameters;
  bool m_IsSetAcquiCorrectionParameters;

  /** Radiative terms object */
  AtmosphericRadiativeTermsPointerType     m_AtmosphericRadiativeTerms;
  AtmoCorrectionParametersPointerType      m_AtmoCorrectionParameters;
  AcquiCorrectionParametersPointerType     m_AcquiCorrectionParameters;

  /** Size of the window. */
  unsigned int m_WindowRadius;
  /** Weighting values for the neighbor pixels.*/
  WeightingValuesContainerType m_WeightingValues;
  /** True if the functor parameters have been generated */
  mutable bool m_FunctorParametersHaveBeenComputed;
  /** Pixel spacing in kilometers */
  double m_PixelSpacingInKilometers;
  /** Viewing angle in degree */
  double m_ZenithalViewingAngle;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSurfaceAdjacencyEffectCorrectionSchemeFilter.txx"
#endif

#endif
