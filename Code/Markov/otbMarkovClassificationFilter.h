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
#ifndef _otbMarkovClassificationFilter_h
#define _otbMarkovClassificationFilter_h

#include "otbMacro.h"

#include "vnl/vnl_vector.h"
#include "vnl/vnl_matrix.h"

#include "itkImageClassifierBase.h"

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

//should be done with generic base class later...
// #include "otbGaussianMRFEnergy.h"
// #include "otbMRFSamplerRandom.h"
// #include "otbMRFOptimizerMetropolis.h"
// #include "otbGaussianClassesMRFEnergy.h"
// #include "otbMRFSamplerMAP.h"
// #include "otbMRFOptimizerICM.h"


namespace otb
{

template <class TInputImage, class TClassifiedImage>
class ITK_EXPORT MarkovClassificationFilter : 
public itk::ImageToImageFilter<TInputImage,TClassifiedImage>
{
 public:       
  /** Standard class typedefs. */
  typedef MarkovClassificationFilter                            Self;
  typedef itk::ImageToImageFilter<TInputImage,TClassifiedImage> Superclass;
  typedef itk::SmartPointer<Self>                               Pointer;
  typedef itk::SmartPointer<const Self>                         ConstPointer;
  typedef typename Superclass::OutputImagePointer               OutputImagePointer;
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(MarkovClassificationFilter,itk::ImageToImageFilter);
  
  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  
  /** Type definition for the input image. */
  typedef TInputImage                                InputImageType;
  typedef typename TInputImage::Pointer              InputImagePointer;
  typedef typename TInputImage::ConstPointer         InputImageConstPointer;  
  
  /** Type definition for the input image pixel type. */
  typedef typename TInputImage::PixelType            InputImagePixelType;
  
  /** Type definition for the input image region type. */
  typedef typename TInputImage::RegionType           InputImageRegionType;
  
  /** Type definition for the input image region iterator */
  typedef itk::ImageRegionIterator<TInputImage>      InputImageRegionIterator;
  typedef itk::ImageRegionConstIterator<TInputImage> InputImageRegionConstIterator;
  
  /** Image dimension */
  itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);
  
  /** Type definitions for the training image. */
  typedef TClassifiedImage                               TrainingImageType;
  typedef typename TClassifiedImage::Pointer             TrainingImagePointer;
  
  /** Type definitions for the training image pixel type. */
  typedef typename TClassifiedImage::PixelType           TrainingImagePixelType;
  
  /** Type definitions for the labelled image.
   * It is derived from the training image. */
  typedef TClassifiedImage                               LabelledImageType;
  typedef typename TClassifiedImage::Pointer             LabelledImagePointer;
    /** Type definitions for the classified image pixel type.
   * It has to be the same type as the training image. */
  typedef typename TClassifiedImage::PixelType            LabelledImagePixelType;
  
  /** Type definitions for the classified image pixel type.
   * It has to be the same type as the training image. */
  typedef typename TClassifiedImage::RegionType           LabelledImageRegionType;
  
  /** Type definition for the classified image index type. */
  typedef typename TClassifiedImage::IndexType            LabelledImageIndexType;
  typedef typename LabelledImageIndexType::IndexValueType IndexValueType;
  
  /** Type definition for the classified image offset type. */
  typedef typename TClassifiedImage::OffsetType            LabelledImageOffsetType;
  
  /** Type definition for the input image region iterator */
  typedef itk::ImageRegionIterator<TClassifiedImage>       LabelledImageRegionIterator;
  
  /** Labelled Image dimension */
  itkStaticConstMacro(ClassifiedImageDimension, unsigned int, TClassifiedImage::ImageDimension);
  
  /** Type definitions for classifier to be used for the MRF lavbelling. */
  typedef itk::ImageClassifierBase<TInputImage,TClassifiedImage> ClassifierType;
  
  /** Size and value typedef support. */
  typedef typename TInputImage::SizeType SizeType;
  
  /** Radius typedef support. */
  typedef typename TInputImage::SizeType NeighborhoodRadiusType;
  
  /** Input image neighborhood iterator and kernel size typedef */
  typedef itk::ConstNeighborhoodIterator< TInputImage >                           InputImageNeighborhoodIterator; 
  typedef typename InputImageNeighborhoodIterator::RadiusType                     InputImageNeighborhoodRadiusType;
  typedef itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator< TInputImage > InputImageFacesCalculator;
  typedef typename InputImageFacesCalculator::FaceListType                        InputImageFaceListType;
  typedef typename InputImageFaceListType::iterator                               InputImageFaceListIterator;
  
  /** Labelled image neighborhood interator typedef */
  typedef itk::NeighborhoodIterator< TClassifiedImage >                                LabelledImageNeighborhoodIterator;
  typedef typename LabelledImageNeighborhoodIterator::RadiusType                       LabelledImageNeighborhoodRadiusType;
  typedef itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator< TClassifiedImage > LabelledImageFacesCalculator;
  typedef typename LabelledImageFacesCalculator::FaceListType                          LabelledImageFaceListType;
  typedef typename LabelledImageFaceListType::iterator                                 LabelledImageFaceListIterator;
  
  /** Set the pointer to the classifer being used. */
  void SetClassifier( typename ClassifierType::Pointer ptrToClassifier );
  
  
  /** Set pipeline elements */
  // typedef GaussianMRFEnergy< TInputImage, TClassifiedImage> EnergyType;      
  typedef MRFEnergy< TClassifiedImage, TClassifiedImage> EnergyRegularizationType;
  typedef MRFEnergy< TInputImage, TClassifiedImage>      EnergyFidelityType;
  typedef typename EnergyRegularizationType::Pointer     EnergyRegularizationPointer;
  typedef typename EnergyFidelityType::Pointer           EnergyFidelityPointer;        
  
  // typedef RandomMRFSampler< TInputImage, TClassifiedImage> SamplerType;
  // typedef MRFSamplerMAP< TInputImage, TClassifiedImage> SamplerType;
  typedef MRFSampler< TInputImage, TClassifiedImage> SamplerType;    
  typedef typename SamplerType::Pointer              SamplerPointer;         
  
  // typedef MetropolisMRFOptimizer OptimizerType;
  // typedef MRFOptimizerICM OptimizerType;   
  typedef MRFOptimizer                    OptimizerType;   
  typedef typename OptimizerType::Pointer OptimizerPointer;
  


  /*
   ************ ACCESSORS ************
   */
  itkSetObjectMacro( EnergyRegularization, EnergyRegularizationType);
  itkGetObjectMacro( EnergyRegularization, EnergyRegularizationType);
  
  itkSetObjectMacro( EnergyFidelity, EnergyFidelityType);
  itkGetObjectMacro( EnergyFidelity, EnergyFidelityType);      
  
  itkSetObjectMacro( Sampler, SamplerType);
  itkGetObjectMacro( Sampler, SamplerType);
  
  itkSetObjectMacro( Optimizer, OptimizerType);
  itkGetObjectMacro( Optimizer, OptimizerType);  

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
  
  /** Set/Get the temperature 
   * */
  //         itkSetMacro(Temperature, double);
  //         itkGetMacro(Temperature, double);
    
  /** Set/Get the regularization coefficient 
   * */
  itkSetMacro(Lambda, double);
  itkGetMacro(Lambda, double);      
  
  
  /** Set the neighborhood radius */
  void SetNeighborhoodRadius(const NeighborhoodRadiusType &);  
  
  /** Sets the radius for the neighborhood, calculates size from the
   * radius, and allocates storage. */
  
  void SetNeighborhoodRadius( const unsigned long );
  void SetNeighborhoodRadius( const unsigned long *radiusArray );  
  
  /** Get the neighborhood radius */
  const NeighborhoodRadiusType GetNeighborhoodRadius() const
    { 
      NeighborhoodRadiusType m_NeighborhoodRadius;
      
      for(int i=0; i<InputImageDimension; ++i)
	m_NeighborhoodRadius[i] = m_InputImageNeighborhoodRadius[i];
      
      return m_NeighborhoodRadius;
    }
    
  
  /** Set training image */
  //         virtual void SetTrainingImage (const TrainingImagePointer _arg) 
  //         { 
  //           itkDebugMacro("setting TrainingImage to " << _arg); 
  //           if (this->m_TrainingImage != _arg) 
  //                 { 
  // //                   this->SetInput(1,_arg);
  // //                   this->m_TrainingImage = this->GetInput(1); 
  //                   this->m_TrainingImage = _arg;
  //                   this->Modified(); 
  //                 } 
  //                 m_ExternalClassificationSet = true;
  //         } 
  //         itkSetMacro(TrainingImage,TrainingImagePointer);
  //         itkGetMacro(TrainingImage,TrainingImagePointer);
  //         itkSetInputMacro(TrainingImage,TrainingImagePointer,1);
  //         itkGetInputMacro(TrainingImage,TrainingImagePointer,1);
  virtual void SetTrainingInput( const TrainingImageType * trainningImage);
  TrainingImageType* GetTrainingInput(void);
  
  /** Store a value to be used instead of random value.. FOR TEST ONLY*/
  void SetValueInsteadRandom( int val )
    {
      m_ValueInsteadRandom = val;
      std::cout<<"The m_ValueInsteadRandom varaible has to be used only for tests..."<<std::endl;
      this->Modified();
    };
  
  //Enum to get the stopping condition of the MRF filter
  typedef enum{ MaximumNumberOfIterations=1, ErrorTolerance } StopConditionType;
  
  /** Get condition that stops the MRF filter (Number of Iterations 
   * / Error tolerance ) */
  itkGetConstReferenceMacro( StopCondition, StopConditionType );
  
  /* Get macro for number of iterations */
  itkGetConstReferenceMacro( NumberOfIterations, unsigned int );
  
#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(UnsignedIntConvertibleToClassifiedCheck, (itk::Concept::Convertible<unsigned int, LabelledImagePixelType>));
  itkConceptMacro(ClassifiedConvertibleToUnsignedIntCheck, (itk::Concept::Convertible<LabelledImagePixelType, unsigned int> ));
  itkConceptMacro(ClassifiedConvertibleToIntCheck, (itk::Concept::Convertible<LabelledImagePixelType, int> ));
  itkConceptMacro(IntConvertibleToClassifiedCheck, (itk::Concept::Convertible<int, LabelledImagePixelType>));
  itkConceptMacro(SameDimensionCheck, (itk::Concept::SameDimension<InputImageDimension, ClassifiedImageDimension>));
  /** End concept checking */
#endif
  
  
 protected:
  MarkovClassificationFilter();
  ~MarkovClassificationFilter();
  void PrintSelf(std::ostream& os, itk::Indent indent) const;
  
  /** Allocate memory for labelled images. */
  void Allocate();
  
  void Initialize() throw (itk::ExceptionObject);
  
  virtual void ApplyMarkovClassificationFilter();  
  virtual void GenerateData();
  virtual void GenerateInputRequestedRegion();
  virtual void EnlargeOutputRequestedRegion( itk::DataObject * );
  virtual void GenerateOutputInformation();
  
  
  MarkovClassificationFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
  
  typedef typename TInputImage::SizeType InputImageSizeType;
  
  
  InputImageNeighborhoodRadiusType       m_InputImageNeighborhoodRadius;
  LabelledImageNeighborhoodRadiusType    m_LabelledImageNeighborhoodRadius;
  
  unsigned int         m_NumberOfClasses;
  unsigned int         m_MaximumNumberOfIterations;
  unsigned int         m_KernelSize;
  unsigned int         m_NumberOfIterations;
  int                  m_ErrorCounter;
  // int                 m_NeighborhoodSize;
  int                  m_NeighborhoodRadius;
  int                  m_TotalNumberOfValidPixelsInOutputImage;
  int                  m_TotalNumberOfPixelsInInputImage;
  double               m_ErrorTolerance;
  double               m_SmoothingFactor;
  double               m_Lambda;
  // double              *m_ClassProbability; //Class liklihood
  // double              m_Temperature;
  bool                 m_ExternalClassificationSet;
  StopConditionType    m_StopCondition; 
  std::vector<double>  m_MRFNeighborhoodWeight;
  std::vector<double>  m_NeighborInfluence;
  std::vector<double>  m_DummyVector;
  
  
  /** Pointer to different elements */
  // TrainingImagePointer m_TrainingImage;
  EnergyRegularizationPointer m_EnergyRegularization;
  EnergyFidelityPointer       m_EnergyFidelity;
  OptimizerPointer            m_Optimizer;    
  SamplerPointer              m_Sampler;        
  
  virtual void MinimizeOnce();
  
 private: 
  /** Store a value to be used instead of random value.. FOR TEST ONLY*/
  int m_ValueInsteadRandom;
  
}; // class MarkovClassificationFilter
 
 
} // namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "otbMarkovClassificationFilter.txx"
#endif

#endif

