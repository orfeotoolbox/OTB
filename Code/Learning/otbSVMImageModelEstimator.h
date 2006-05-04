/*=========================================================================

  Program :   OTB (ORFEO ToolBox)
  Authors   :   CNES - J. Inglada
  Language  :   C++
  Date      :   26 April 2006
  Version   :   
  Role      :   SVM Image Model Estimator
  $Id$


=========================================================================*/
#ifndef _otbSVMImageModelEstimator_h
#define _otbSVMImageModelEstimator_h

#include "itkImageModelEstimatorBase.h"
#include "itkImageRegionIterator.h"
#include "otbSVMModel.h"
#include "otbSVMModelEstimator.h"

namespace otb
{

/** \class SVMImageModelEstimator
 * \brief Class for SVM model estimation from images used for classification.
 * 
 *
 * The basic functionality of the SVMImageModelEstimator framework base class is to    
 * generate the models used in SVM classification. It requires input 
 * images and a training image to be provided by the user.
 * This object supports data handling of multiband images. The object
 * accepts the input image in vector format only, where each pixel is a 
 * vector and each element of the vector corresponds to an entry from
 * 1 particular band of a multiband dataset. A single band image is treated 
 * as a vector image with a single element for every vector. The classified
 * image is treated as a single band scalar image. 
 *
 * EstimateModels() is a pure virtual function making this an abstract class. 
 * The template parameter is the type of a membership function the 
 * ImageModelEstimator populates.
 *
 * A membership function represents a specific knowledge about
 * a class. In other words, it should tell us how "likely" is that a
 * measurement vector (pattern) belong to the class. 
 *
 * As the method name indicates, you can have more than one membership 
 * function. One for each classes. The order you put the membership 
 * calculator becomes the class label for the class that is represented
 * by the membership calculator. 
 *

 *
 * \ingroup ClassificationFilters 
 */
template <class TInputImage, 
          class TTrainingImage>
class ITK_EXPORT SVMImageModelEstimator: 
    public otb::SVMModelEstimator<typename TInputImage::PixelType::ComponentType, typename TTrainingImage::PixelType>
{
public:
  /** Standard class typedefs. */
  typedef SVMImageModelEstimator   Self;
  typedef SVMModelEstimator<typename TInputImage::PixelType::ComponentType,
			    typename TTrainingImage::PixelType>
                            Superclass;

  typedef itk::SmartPointer<Self>  Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(SVMImageModelEstimator, otb::SVMModelEstimator);

  /** Type definition for the input image. */
    /** Type definitions for the training image. */
  typedef typename TInputImage::Pointer   InputImagePointer;
 
  /** Type definitions for the training image. */
  typedef typename TTrainingImage::Pointer TrainingImagePointer;
    
  /** Type definition for the vector associated with
   * input image pixel type. */     
  typedef typename TInputImage::PixelType InputImagePixelType;        

  /** Type definitions for the vector holding
   * training image pixel type. */
  typedef typename TTrainingImage::PixelType TrainingImagePixelType;

  /** Type definitions for the iterators for the input and training images. */
  typedef
  itk::ImageRegionIterator< TInputImage >  InputImageIterator;
  typedef
  itk::ImageRegionIterator< TTrainingImage > TrainingImageIterator;

  /** Set the input image. */
  itkSetObjectMacro(InputImage,TInputImage);
  
  /** Get the input image. */
  itkGetObjectMacro(InputImage,TInputImage);


  /** Set the training image. */
  itkSetMacro(TrainingImage,TrainingImagePointer);

  /** Get the training image. */
  itkGetMacro(TrainingImage,TrainingImagePointer);



protected: 
  SVMImageModelEstimator();
  ~SVMImageModelEstimator();
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;


  virtual void BuildProblem();

private:
  SVMImageModelEstimator(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented


  typedef typename TInputImage::SizeType InputImageSizeType;


  TrainingImagePointer  m_TrainingImage;
  InputImagePointer  m_InputImage;



}; // class SVMImageModelEstimator

} // namespace otb

#ifndef ITK_MANUAL_INSTANTIATION
#include "otbSVMImageModelEstimator.txx"
#endif



#endif

