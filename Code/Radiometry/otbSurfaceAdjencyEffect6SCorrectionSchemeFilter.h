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
#ifndef __otbSurfaceAdjencyEffect6SCorrectionSchemeFilter_h
#define __otbSurfaceAdjencyEffect6SCorrectionSchemeFilter_h

#include "itkNumericTraits.h"
#include <vector>
#include "itkConstNeighborhoodIterator.h"
#include "otbUnaryFunctorNeighborhoodImageFilter.h"
#include "itkVariableSizeMatrix.h"
#include "otbAtmosphericRadiativeTerms.h"

namespace otb
{
 namespace Functor
    {
       /** \class ComputeNeighborhoodContributionFunctor
       *  \brief Unary neighborhood functor to compute the value of a pixel which is a sum
       *   of the surrounding pixels value ponderated by a coefficient.
       *
       *  \ingroup Functor
       */
      template <class TNeighIter, class TOutput>
  class ComputeNeighborhoodContributionFunctor
  {
  public:
    ComputeNeighborhoodContributionFunctor() {};
    ~ComputeNeighborhoodContributionFunctor() {};

    typedef itk::VariableSizeMatrix<double>             WeightingMatrixType;
    typedef typename std::vector<WeightingMatrixType>   WeightingValuesContainerType;
    typedef typename TOutput::RealValueType             RealValueType;
    typedef std::vector<double>                         DoubleContainerType;

    void SetWeightingValues(const WeightingValuesContainerType & cont){ m_WeightingValues = cont; };
    void SetUpwardTransmittanceRatio(DoubleContainerType upwardTransmittanceRatio){ m_UpwardTransmittanceRatio = upwardTransmittanceRatio;};
    void SetDiffuseRatio(DoubleContainerType diffuseRatio){ m_DiffuseRatio = diffuseRatio;};
    WeightingValuesContainerType GetWeightingValues(){ return m_WeightingValues;};
    DoubleContainerType GetUpwardTransmittanceRatio(){ return m_UpwardTransmittanceRatio;};
    DoubleContainerType GetDiffuseRatio(){ return m_DiffuseRatio;};

    inline TOutput operator()(const TNeighIter & it)
      {
        unsigned int neighborhoodSize = it.Size();
        double contribution = 0.;
        TOutput outPixel;
        outPixel.SetSize(it.GetCenterPixel().Size());

        // Loop over each component
        for (unsigned int j=0; j<outPixel.GetSize(); j++)
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
          RowIdx = i/TempChannelWeighting.Cols();
          if(RowIdx != 0)
      {
        ColIdx = (i+1)-RowIdx*TempChannelWeighting.Cols()-1;
      }
          else
      {
        ColIdx = i;
      }
          // Extract the current neighborhood pixel ponderation
          double idVal = TempChannelWeighting(RowIdx, ColIdx);
          // Extract the current neighborhood pixel value
          TOutput tempPix = it.GetPixel(i);

          contribution += static_cast<double>( tempPix[j] )*idVal;

        }
      outPixel[j] = static_cast<RealValueType>(it.GetCenterPixel()[j])*m_UpwardTransmittanceRatio[j] + contribution*m_DiffuseRatio[j];
    }
        return outPixel;
      }

  private:
    WeightingValuesContainerType m_WeightingValues;
    DoubleContainerType m_UpwardTransmittanceRatio;
    DoubleContainerType m_DiffuseRatio;
  };

    }

  /** \class SurfaceAdjencyEffect6SCorrectionSchemeFilter
   *  \brief Correct the scheme taking care of the surrounding pixels.
   *
   *   The SurfaceAdjencyEffect6SCorrectionSchemeFilter class allows to introduce a neighbor correction to the
   *   reflectance estimation. The satelite signal is considered as to be a combinaison of the signal coming from
   *   the target pixel and a weighting of the siganls coming from the neighbor pixels.
   *
   */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT SurfaceAdjencyEffect6SCorrectionSchemeFilter :
 public UnaryFunctorNeighborhoodImageFilter< TInputImage,
                                             TOutputImage,
                                             ITK_TYPENAME Functor::ComputeNeighborhoodContributionFunctor< itk::ConstNeighborhoodIterator<TInputImage>,
                                                                                                                 ITK_TYPENAME TOutputImage::PixelType >       >
{
public:
  /** "typedef" to simplify the variables definition and the declaration. */
  typedef Functor::ComputeNeighborhoodContributionFunctor<itk::ConstNeighborhoodIterator<TInputImage>,
                                                                   ITK_TYPENAME TOutputImage::PixelType> FunctorType;

  /** "typedef" for standard classes. */
  typedef SurfaceAdjencyEffect6SCorrectionSchemeFilter                                           Self;
  typedef UnaryFunctorNeighborhoodImageFilter< TInputImage, TOutputImage, FunctorType >    Superclass;
  typedef itk::SmartPointer<Self>                                                                Pointer;
  typedef itk::SmartPointer<const Self>                                                          ConstPointer;

  typedef typename Superclass::InputImageType         InputImageType;
  typedef typename Superclass::OutputImageType        OutputImageType;

  typedef std::vector<double>                         DoubleContainerType;
  /** object factory method. */
  itkNewMacro(Self);

  /** return class name. */
  itkTypeMacro(SurfaceAdjencyEffect6SCorrectionSchemeFilter, UnaryFunctorNeighborhoodImageFilter);

/**   Extract input and output images dimensions.*/
  itkStaticConstMacro( InputImageDimension, unsigned int, TInputImage::ImageDimension);
  itkStaticConstMacro( OutputImageDimension, unsigned int, TOutputImage::ImageDimension);

  /** Supported images definition. */
  typedef typename InputImageType::PixelType                           InputPixelType;
  typedef typename InputImageType::InternalPixelType                   InputInternalPixelType;
  typedef typename InputImageType::RegionType                          InputImageRegionType;
  typedef typename InputImageType::SizeType                            SizeType;
  typedef typename OutputImageType::PixelType                          OutputPixelType;
  typedef typename OutputImageType::InternalPixelType                  OutputInternalPixelType;
  typedef typename OutputImageType::RegionType                         OutputImageRegionType;

  typedef AtmosphericRadiativeTerms                                    AtmosphericRadiativeTermsType;
  typedef typename AtmosphericRadiativeTermsType::Pointer              AtmosphericRadiativeTermsPointerType;

  /** Storage ponderation values types*/
  typedef itk::VariableSizeMatrix<double>                               WeightingMatrixType;
  typedef typename std::vector<WeightingMatrixType>                   WeightingValuesContainerType;

  /** typedef for calculation*/
  typedef typename itk::ConstNeighborhoodIterator<InputImageType>          NeighborIterType;

   /** Set/Get the Size of the neighbor window. */
  void SetWindowRadius(unsigned int rad)
    {
      this->SetRadius(rad);
      m_WindowRadius = rad;
      this->Modified();
    }
  itkGetConstReferenceMacro(WindowRadius, unsigned int);

  /** Set/Get the pixel spacing in kilometers */
  itkSetMacro(PixelSpacingInKilometers,double);
  itkGetMacro(PixelSpacingInKilometers,double);
  /** Set/Get the viewing angle */
  itkSetMacro(ZenithalViewingAngle,double);
  itkGetMacro(ZenithalViewingAngle,double);

  /** Get/Set Atmospheric Radiative Terms. */
  void SetAtmosphericRadiativeTerms(AtmosphericRadiativeTermsPointerType atmo)
    {
      m_AtmosphericRadiativeTerms = atmo;
      this->SetNthInput(1, m_AtmosphericRadiativeTerms);
      this->Modified();
    }
  AtmosphericRadiativeTermsPointerType GetAtmosphericRadiativeTerms()
    {
      return m_AtmosphericRadiativeTerms;
    }

  /** Compute the functor parameters */
  void ComputeParameters();


 protected:
  SurfaceAdjencyEffect6SCorrectionSchemeFilter();
  virtual ~SurfaceAdjencyEffect6SCorrectionSchemeFilter(){};
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** GenerateOutputInformation method */
  virtual void GenerateOutputInformation();

  /** Initialize some accumulators before the threads run. */
  virtual void BeforeThreadedGenerateData();

  /** If modified, we need to compute the parameters again */
  virtual void Modified();

private:

  /** Size of the window. */
  unsigned int m_WindowRadius;
  /** Weighting values for the neighbor pixels.*/
  WeightingValuesContainerType m_WeightingValues;
  /** True if the parameters have been generated */
  bool m_ParametersHaveBeenComputed;
  /** Pixel spacing in kilometers */
  double m_PixelSpacingInKilometers;
  /** Viewing angle in degree */
  double m_ZenithalViewingAngle;
  /** Radiative terms object */
  AtmosphericRadiativeTermsPointerType m_AtmosphericRadiativeTerms;

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSurfaceAdjencyEffect6SCorrectionSchemeFilter.txx"
#endif

#endif
