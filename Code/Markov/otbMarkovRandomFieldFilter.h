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
#ifndef __otbMarkovRandomFieldFilter_h
#define __otbMarkovRandomFieldFilter_h

#include "otbMacro.h"

#include "vnl/vnl_vector.h"
#include "vnl/vnl_matrix.h"
#include "itkMersenneTwisterRandomVariateGenerator.h"

#include "itkImageToImageFilter.h"
#include "itkImageRegionIterator.h"

#include "itkConstNeighborhoodIterator.h"
#include "itkNeighborhoodIterator.h"
#include "itkNeighborhoodAlgorithm.h"
#include "itkNeighborhood.h"
#include "itkSize.h"
#include "otbMRFEnergy.h"
#include "otbMRFOptimizer.h"
#include "otbMRFSampler.h"

namespace otb
{
/**
 * \class MarkovRandomFieldFilter
 * \brief This is the class to use the Markov Random Field framework in OTB.
 *
 * This filter apply a Markov Random Field to an input image. Several
 * components need to be specify:
 *
 * - Fidelity energy (class derived from otb::MRFEnergy): the energy to make
 * sure that the output image is close enough to the reference.
 * - Regularization energy (class derived from otb::MRFEnergy): the energy to
 * make sure that neighborhood pixels have similar values.
 * - Sampler (class derived from otb::MRFSampler): the strategy to propose
 * variations for each pixel.
 * - Optimizer (class derived from otb::MRFOptimizer): the strategy to accept
 * or reject the proposed modification.
 *
 * An example of usage for this filter is:
 *
 * \code
 *   markovFilter->SetNumberOfClasses(4);
 *   markovFilter->SetMaximumNumberOfIterations(30);
 *   markovFilter->SetErrorTolerance(0.0);
 *   markovFilter->SetLambda(1.0);
 *   markovFilter->SetNeighborhoodRadius(1);
 *
 *   markovFilter->SetEnergyRegularization(energyRegularization);
 *   markovFilter->SetEnergyFidelity(energyFidelity);
 *   markovFilter->SetOptimizer(optimizer);
 *   markovFilter->SetSampler(sampler);
 * \endcode
 *
 *
 * \ingroup Markov
 *
 * \example  Markov/MarkovClassification1Example.cxx
 * \example  Markov/MarkovClassification2Example.cxx
 * \example  Markov/MarkovRegularizationExample.cxx
 * \example  Markov/MarkovRestaurationExample.cxx
 *
 */

template <class TInputImage, class TClassifiedImage>
class ITK_EXPORT MarkovRandomFieldFilter :
  public itk::ImageToImageFilter<TInputImage, TClassifiedImage>
{
public:
  /** Standard class typedefs. */
  typedef MarkovRandomFieldFilter                                Self;
  typedef itk::ImageToImageFilter<TInputImage, TClassifiedImage> Superclass;
  typedef itk::SmartPointer<Self>                                Pointer;
  typedef itk::SmartPointer<const Self>                          ConstPointer;
  typedef typename Superclass::OutputImagePointer                OutputImagePointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(MarkovRandomFieldFilter, itk::ImageToImageFilter);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Type definition for the input image. */
  typedef TInputImage                        InputImageType;
  typedef typename TInputImage::Pointer      InputImagePointer;
  typedef typename TInputImage::ConstPointer InputImageConstPointer;

  /** Type definition for the input image pixel type. */
  typedef typename TInputImage::PixelType InputImagePixelType;

  /** Type definition for the input image region type. */
  typedef typename TInputImage::RegionType InputImageRegionType;

  /** Type definition for the input image region iterator */
  typedef itk::ImageRegionIterator<TInputImage>      InputImageRegionIterator;
  typedef itk::ImageRegionConstIterator<TInputImage> InputImageRegionConstIterator;

  /** Image dimension */
  itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);

  /** Type definitions for the training image. */
  typedef TClassifiedImage                   TrainingImageType;
  typedef typename TClassifiedImage::Pointer TrainingImagePointer;

  /** Type definitions for the training image pixel type. */
  typedef typename TClassifiedImage::PixelType TrainingImagePixelType;

  /** Type definitions for the labelled image.
   * It is derived from the training image. */
  typedef TClassifiedImage                   LabelledImageType;
  typedef typename TClassifiedImage::Pointer LabelledImagePointer;

  /** Type definitions for the classified image pixel type.
   * It has to be the same type as the training image. */
  typedef typename TClassifiedImage::PixelType LabelledImagePixelType;

  /** Type definitions for the classified image pixel type.
   * It has to be the same type as the training image. */
  typedef typename TClassifiedImage::RegionType LabelledImageRegionType;

  /** Type definition for the classified image index type. */
  typedef typename TClassifiedImage::IndexType            LabelledImageIndexType;
  typedef typename LabelledImageIndexType::IndexValueType IndexValueType;

  /** Type definition for the classified image offset type. */
  typedef typename TClassifiedImage::OffsetType LabelledImageOffsetType;

  /** Type definition for the input image region iterator */
  typedef itk::ImageRegionIterator<TClassifiedImage>
  LabelledImageRegionIterator;

  typedef itk::ImageRegionConstIterator<TClassifiedImage>
  LabelledImageRegionConstIterator;

  /** Labelled Image dimension */
  itkStaticConstMacro(ClassifiedImageDimension, unsigned int,
                      TClassifiedImage::ImageDimension);

  /** Size and value typedef support. */
  typedef typename TInputImage::SizeType SizeType;

  /** Radius typedef support. */
  typedef typename TInputImage::SizeType NeighborhoodRadiusType;

  /** Input image neighborhood iterator and kernel size typedef */
  typedef itk::ConstNeighborhoodIterator<TInputImage>
  InputImageNeighborhoodIterator;

  typedef typename InputImageNeighborhoodIterator::RadiusType
  InputImageNeighborhoodRadiusType;

  typedef itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<TInputImage>
  InputImageFacesCalculator;

  typedef typename InputImageFacesCalculator::FaceListType
  InputImageFaceListType;

  typedef typename InputImageFaceListType::iterator
  InputImageFaceListIterator;

  /** Labelled image neighborhood interator typedef */
  typedef itk::NeighborhoodIterator<TClassifiedImage>
  LabelledImageNeighborhoodIterator;

  typedef typename LabelledImageNeighborhoodIterator::RadiusType
  LabelledImageNeighborhoodRadiusType;

  typedef itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<TClassifiedImage>
  LabelledImageFacesCalculator;

  typedef typename LabelledImageFacesCalculator::FaceListType
  LabelledImageFaceListType;

  typedef typename LabelledImageFaceListType::iterator
  LabelledImageFaceListIterator;

  /** Typedef for random values. */
  typedef itk::Statistics::MersenneTwisterRandomVariateGenerator RandomGeneratorType;

  /** Set pipeline elements */
  typedef MRFEnergy<TClassifiedImage, TClassifiedImage> EnergyRegularizationType;
  typedef MRFEnergy<TInputImage, TClassifiedImage>      EnergyFidelityType;

  typedef typename EnergyRegularizationType::Pointer EnergyRegularizationPointer;
  typedef typename EnergyFidelityType::Pointer       EnergyFidelityPointer;

  typedef MRFSampler<TInputImage, TClassifiedImage> SamplerType;
  typedef typename SamplerType::Pointer             SamplerPointer;

  typedef MRFOptimizer                    OptimizerType;
  typedef typename OptimizerType::Pointer OptimizerPointer;

  /**
  ************ ACCESSORS ************
  */
  itkSetObjectMacro(EnergyRegularization, EnergyRegularizationType);
  itkGetObjectMacro(EnergyRegularization, EnergyRegularizationType);

  itkSetObjectMacro(EnergyFidelity, EnergyFidelityType);
  itkGetObjectMacro(EnergyFidelity, EnergyFidelityType);

  itkSetObjectMacro(Sampler, SamplerType);
  itkGetObjectMacro(Sampler, SamplerType);

  itkSetObjectMacro(Optimizer, OptimizerType);
  itkGetObjectMacro(Optimizer, OptimizerType);

  /** Set/Get the number of classes. */
  itkSetMacro(NumberOfClasses, unsigned int);
  itkGetMacro(NumberOfClasses, unsigned int);

  /** Set/Get the number of iteration of the Iterated Conditional Mode
   * (ICM) algorithm. A default value is set at 50 iterations. */
  itkSetMacro(MaximumNumberOfIterations, unsigned int);
  itkGetMacro(MaximumNumberOfIterations, unsigned int);

  /** Set/Get the error tollerance level which is used as a threshold
   * to quit the iterations */
  itkSetMacro(ErrorTolerance, double);
  itkGetMacro(ErrorTolerance, double);

  /** Set/Get the degree of smoothing desired
   * */
  itkSetMacro(SmoothingFactor, double);
  itkGetMacro(SmoothingFactor, double);

  /** Set/Get the regularization coefficient
   * */
  itkSetMacro(Lambda, double);
  itkGetMacro(Lambda, double);

  /** Set the neighborhood radius */
  void SetNeighborhoodRadius(const NeighborhoodRadiusType&);

  /** Sets the radius for the neighborhood, calculates size from the
   * radius, and allocates storage. */

  void SetNeighborhoodRadius(const unsigned long);
  void SetNeighborhoodRadius(const unsigned long *radiusArray);

  /** Get the neighborhood radius */
  const NeighborhoodRadiusType GetNeighborhoodRadius() const
  {
    NeighborhoodRadiusType m_NeighborhoodRadius;

    for (int i = 0; i < InputImageDimension; ++i)
      m_NeighborhoodRadius[i] = m_InputImageNeighborhoodRadius[i];

    return m_NeighborhoodRadius;
  }

  /** Set training image for the starting point. This is not compulsory:
   * if the starting image is not specified, a random image will be used
   * instead.
   * One important restriction: in the case of classification, the training
   * image should contain values corresponding to the class number (consecutive
   * integers).
  */
  virtual void SetTrainingInput(const TrainingImageType * trainingImage);
  const TrainingImageType* GetTrainingInput(void);

  //Enum to get the stopping condition of the MRF filter
  typedef enum
    {
    MaximumNumberOfIterations = 1,
    ErrorTolerance
    } StopConditionType;

  /** Get condition that stops the MRF filter (Number of Iterations
   * / Error tolerance ) */
  itkGetConstReferenceMacro(StopCondition, StopConditionType);

  /** Get macro for number of iterations */
  itkGetConstReferenceMacro(NumberOfIterations, unsigned int);

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(UnsignedIntConvertibleToClassifiedCheck,
                  (itk::Concept::Convertible<unsigned int, LabelledImagePixelType>));
  itkConceptMacro(ClassifiedConvertibleToUnsignedIntCheck,
                  (itk::Concept::Convertible<LabelledImagePixelType, unsigned int> ));
  itkConceptMacro(ClassifiedConvertibleToIntCheck,
                  (itk::Concept::Convertible<LabelledImagePixelType, int> ));
  itkConceptMacro(IntConvertibleToClassifiedCheck,
                  (itk::Concept::Convertible<int, LabelledImagePixelType>));
  itkConceptMacro(SameDimensionCheck,
                  (itk::Concept::SameDimension<InputImageDimension, ClassifiedImageDimension>));
  /** End concept checking */
#endif

  /** Methods to cancel random effects.*/
  void InitializeSeed(int seed)
  {
    m_Generator->SetSeed(seed);
  }
  void InitializeSeed()
  {
    m_Generator->SetSeed();
  }

protected:
  MarkovRandomFieldFilter();
  virtual ~MarkovRandomFieldFilter(){}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** Allocate memory for labelled images. This is automatically called
   * in GenerateData().
  */
  void Allocate();

  /** Connect the pipeline and propagate the required parameters. This
  * is automatically called in GenerateData().
  */
  void Initialize()
    throw (itk::ExceptionObject);

  virtual void ApplyMarkovRandomFieldFilter();

  virtual void GenerateData();
  virtual void GenerateInputRequestedRegion();
  virtual void EnlargeOutputRequestedRegion(itk::DataObject *);
  virtual void GenerateOutputInformation();

  MarkovRandomFieldFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  typedef typename TInputImage::SizeType InputImageSizeType;

  InputImageNeighborhoodRadiusType    m_InputImageNeighborhoodRadius;
  LabelledImageNeighborhoodRadiusType m_LabelledImageNeighborhoodRadius;

  unsigned int m_NumberOfClasses;
  unsigned int m_MaximumNumberOfIterations;

  int    m_ErrorCounter;
  double m_ImageDeltaEnergy;

  int    m_NeighborhoodRadius;
  int    m_TotalNumberOfValidPixelsInOutputImage;
  int    m_TotalNumberOfPixelsInInputImage;
  double m_ErrorTolerance;
  double m_SmoothingFactor;

  unsigned int m_NumberOfIterations;

  double            m_Lambda;
  bool              m_ExternalClassificationSet;
  StopConditionType m_StopCondition;

  TrainingImagePointer m_TrainingImage;

  std::vector<double> m_MRFNeighborhoodWeight;
  std::vector<double> m_NeighborInfluence;
  std::vector<double> m_DummyVector;

  RandomGeneratorType::Pointer m_Generator;

  /** Pointer to different elements */

  EnergyRegularizationPointer m_EnergyRegularization;
  EnergyFidelityPointer       m_EnergyFidelity;
  OptimizerPointer            m_Optimizer;
  SamplerPointer              m_Sampler;

  virtual void MinimizeOnce();

private:

}; // class MarkovRandomFieldFilter

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbMarkovRandomFieldFilter.txx"
#endif

#endif
