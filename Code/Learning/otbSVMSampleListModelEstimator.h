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
#ifndef __otbSVMSampleListModelEstimator_h
#define __otbSVMSampleListModelEstimator_h

#include "itkImageModelEstimatorBase.h"
#include "itkImageRegionIterator.h"
#include "otbSVMModel.h"
#include "otbSVMModelEstimator.h"

namespace otb
{

/** \class SVMSampleListModelEstimator

 * \brief Class for SVM model
 * estimation from SampleLists used for classification.
 *
 *
 * The basic functionality of the SVMSampleListModelEstimator is to
 * generate the models used in SVM classification. It
 * requires one input poinset and a training pointset to be provided by the
 * user.  This object supports data handling of multiband data. The
 * object accepts the input image in vector format only, where each
 * pixel is a vector and each element of the vector corresponds to an
 * entry from 1 particular band of a multiband dataset. A single band
 * data set is treated as a vector with a single element for every
 * vector. The classified data is treated as a single band scalar
 * data.
 *
 * EstimateModels() uses the libsvm library for SVM learning.
 *
 * \ingroup ClassificationFilters
 */
template <class TInputSampleList,
          class TTrainingSampleList>
class ITK_EXPORT SVMSampleListModelEstimator:
    public SVMModelEstimator<typename TInputSampleList::MeasurementType, typename TTrainingSampleList::MeasurementType>
{
public:
  /** Standard class typedefs. */
  typedef SVMSampleListModelEstimator   Self;
  typedef SVMModelEstimator<typename TInputSampleList::MeasurementType,
          typename TTrainingSampleList::MeasurementType>
                            Superclass;

  typedef itk::SmartPointer<Self>  Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(SVMSampleListModelEstimator, SVMModelEstimator);

  /** Type definition for the input image. */
    /** Type definitions for the training image. */
  typedef typename TInputSampleList::Pointer   InputSampleListPointer;

  /** Type definitions for the training image. */
  typedef typename TTrainingSampleList::Pointer TrainingSampleListPointer;

  /** Type definition for the vector associated with
   * input image pixel type. */
  typedef typename TInputSampleList::ValueType InputSampleListValueType;

  /** Type definitions for the vector holding
   * training image pixel type. */
  typedef typename TTrainingSampleList::ValueType TrainingSampleListValueType;

  /** Type definitions for the iterators for the input and training images. */
  typedef typename
  TInputSampleList::ConstIterator  InputSampleListIteratorType;
  typedef typename
  TTrainingSampleList::ConstIterator TrainingSampleListIteratorType;

  /** Set the input image. */
  itkSetObjectMacro(InputSampleList,TInputSampleList);

  /** Get the input image. */
  itkGetObjectMacro(InputSampleList,TInputSampleList);


  /** Set the training image. */
  itkSetMacro(TrainingSampleList,TrainingSampleListPointer);

  /** Get the training image. */
  itkGetMacro(TrainingSampleList,TrainingSampleListPointer);



protected:
  SVMSampleListModelEstimator();
  ~SVMSampleListModelEstimator();
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;


  virtual void BuildProblem();

private:
  SVMSampleListModelEstimator(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented



  TrainingSampleListPointer  m_TrainingSampleList;
  InputSampleListPointer  m_InputSampleList;



}; // class SVMSampleListModelEstimator

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSVMSampleListModelEstimator.txx"
#endif



#endif

