/*=========================================================================

  Program :   OTB (ORFEO ToolBox)
  Authors   :   CNES - J. Inglada
  Language  :   C++
  Date      :   4 May 2006
  Version   :   
  Role      :   SVM PointSet Model Estimator
  $Id$


=========================================================================*/
#ifndef _otbSVMPointSetModelEstimator_h
#define _otbSVMPointSetModelEstimator_h

#include "itkImageModelEstimatorBase.h"
#include "itkImageRegionIterator.h"
#include "otbSVMModel.h"
#include "otbSVMModelEstimator.h"

namespace otb
{

/** \class SVMPointSetModelEstimator
 * \brief Class for SVM model estimation from images used for classification.
 * 
 *
 * The basic functionality of the SVMPointSetModelEstimator framework base class is to    
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
 * PointSetModelEstimator populates.
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
template <class TInputPointSet, 
          class TTrainingPointSet>
class ITK_EXPORT SVMPointSetModelEstimator: 
    public otb::SVMModelEstimator<typename TInputPointSet::PixelType::value_type, typename TTrainingPointSet::PixelType>
{
public:
  /** Standard class typedefs. */
  typedef SVMPointSetModelEstimator   Self;
  typedef SVMModelEstimator<typename TInputPointSet::PixelType::value_type,
			    typename TTrainingPointSet::PixelType>
                            Superclass;

  typedef itk::SmartPointer<Self>  Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(SVMPointSetModelEstimator, otb::SVMModelEstimator);

  /** Type definition for the input image. */
    /** Type definitions for the training image. */
  typedef typename TInputPointSet::Pointer   InputPointSetPointer;
 
  /** Type definitions for the training image. */
  typedef typename TTrainingPointSet::Pointer TrainingPointSetPointer;
    
  /** Type definition for the vector associated with
   * input image pixel type. */     
  typedef typename TInputPointSet::PixelType InputPointSetPixelType;
  typedef typename TInputPointSet::PointType InputPointType;        

  /** Type definitions for the vector holding
   * training image pixel type. */
  typedef typename TTrainingPointSet::PixelType TrainingPointSetPixelType;

  /** Type definitions for the iterators for the input and training images. */
  typedef typename
  TInputPointSet::PointsContainerConstIterator  InputPointSetIteratorType;
  typedef typename
  TTrainingPointSet::PointsContainerConstIterator TrainingPointSetIteratorType;

  /** Set the input image. */
  itkSetObjectMacro(InputPointSet,TInputPointSet);
  
  /** Get the input image. */
  itkGetObjectMacro(InputPointSet,TInputPointSet);


  /** Set the training image. */
  itkSetMacro(TrainingPointSet,TrainingPointSetPointer);

  /** Get the training image. */
  itkGetMacro(TrainingPointSet,TrainingPointSetPointer);



protected: 
  SVMPointSetModelEstimator();
  ~SVMPointSetModelEstimator();
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;


  virtual void BuildProblem();

private:
  SVMPointSetModelEstimator(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented



  TrainingPointSetPointer  m_TrainingPointSet;
  InputPointSetPointer  m_InputPointSet;



}; // class SVMPointSetModelEstimator

} // namespace otb

#ifndef ITK_MANUAL_INSTANTIATION
#include "otbSVMPointSetModelEstimator.txx"
#endif



#endif

