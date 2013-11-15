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

#include "vcl_deprecated_header.h"

#include "itkImageModelEstimatorBase.h"
#include "itkImageRegionIterator.h"
#include "otbSVMModel.h"
#include "otbSVMModelEstimator.h"

namespace otb
{
namespace Functor
{
/** \class VectorToMeasurementVectorFunctor
*   \brief Helper class to convert itk::Vector and itk::FixedArray samples to internal
*   training vector type.
*
*   \sa Vector
*   \sa FixedArray
*/
template <class TVector>
class VectorToMeasurementVectorFunctor
{
public:
  typedef TVector                                     VectorType;
  typedef std::vector<typename VectorType::ValueType> MeasurementVectorType;

  inline MeasurementVectorType operator ()(const VectorType& value) const
  {
    MeasurementVectorType output;

    for (unsigned int i = 0; i < value.Size(); ++i)
      {
      output.push_back(value[i]);
      }
    return output;
  }
};

/** \class VariableLengthVectorToMeasurementVectorFunctor
*   \brief Helper class to convert itk::VariableLengthVector samples to internal
*   training vector type.
*   \sa VariableLengthVector
*/
template <class TVector>
class VariableLengthVectorToMeasurementVectorFunctor
{
public:
  typedef TVector                                     VectorType;
  typedef std::vector<typename VectorType::ValueType> MeasurementVectorType;

  inline MeasurementVectorType operator ()(const VectorType& value) const
  {
    MeasurementVectorType output;

    for (unsigned int i = 0; i < value.GetNumberOfElements(); ++i)
      {
      output.push_back(value.GetElement(i));
      }

    return output;
  }
};

}

/**
 * \class SVMSampleListModelEstimator
 * \brief Class for SVM model estimation from SampleLists used for classification.
 *
 *
 * The basic functionality of the SVMSampleListModelEstimator is to
 * generate the models used in SVM classification. It
 * requires one input poinset and a training pointset to be provided by the
 * user. This object supports data handling of multiband data. The
 * object accepts the input image as a VectorImage only, where each
 * pixel is a vector and each element of the vector corresponds to an
 * entry from 1 particular band of a multiband dataset. A single band
 * data set is treated as a vector with a single element for every
 * vector. The classified data is treated as a single band scalar
 * data.
 *
 * \deprecated
 *
 * \sa MachineLearningModel
 * \sa LibSVMMachineLearningModel
 * \sa ImageClassificationFilter
 *
 * \ingroup ClassificationFilters
 */
template <class TInputSampleList,
    class TTrainingSampleList, class TMeasurementFunctor =
      Functor::VectorToMeasurementVectorFunctor<typename TInputSampleList::MeasurementVectorType> >
class ITK_EXPORT SVMSampleListModelEstimator :
  public SVMModelEstimator<typename TInputSampleList::MeasurementType, typename TTrainingSampleList::MeasurementType>
{
public:
  /** Standard class typedefs. */
  typedef SVMSampleListModelEstimator                                       Self;
  typedef SVMModelEstimator<typename TInputSampleList::MeasurementType,
                            typename TTrainingSampleList::MeasurementType>
                                                                            Superclass;

  typedef itk::SmartPointer<Self>                                           Pointer;
  typedef itk::SmartPointer<const Self>                                     ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(SVMSampleListModelEstimator, SVMModelEstimator);

  /** Type definition for the input image. */
  typedef TInputSampleList                   InputSampleListType;
  typedef typename TInputSampleList::Pointer InputSampleListPointer;

  /** Type definitions for the training image. */
  typedef TTrainingSampleList                   TrainingSampleListType;
  typedef typename TTrainingSampleList::Pointer TrainingSampleListPointer;

  /** Type definition for the vector associated with
   * input image pixel type. */
  typedef typename TInputSampleList::ValueType InputSampleListValueType;

  /** Type definitions for the vector holding
   * training image pixel type. */
  typedef typename TTrainingSampleList::ValueType TrainingSampleListValueType;

  /** Type definitions for the iterators for the input and training images. */
  typedef typename
  TInputSampleList::ConstIterator InputSampleListIteratorType;
  typedef typename
  TTrainingSampleList::ConstIterator TrainingSampleListIteratorType;

  /** Measurement functor typedef */
  typedef TMeasurementFunctor MeasurementFunctorType;

  /** Set the input image. */
  //itkSetObjectMacro(InputSampleList, TInputSampleList);
  void SetInputSampleList(const InputSampleListType* inputSampleList);

  /** Get the input image. */
  //itkGetObjectMacro(InputSampleList, TInputSampleList);
  const InputSampleListType* GetInputSampleList();

  /** Set the training image. */
  //itkSetMacro(TrainingSampleList, TrainingSampleListPointer);
  void SetTrainingSampleList(const TrainingSampleListType* trainingSampleList);

  /** Get the training image. */
  //itkGetMacro(TrainingSampleList, TrainingSampleListPointer);
  const TrainingSampleListType* GetTrainingSampleList();

protected:
  /** Constructor */
  SVMSampleListModelEstimator();
  /** Destructor */
  virtual ~SVMSampleListModelEstimator();
  /** PrintSelf */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** PrepareData method */
  virtual void PrepareData();

private:
  SVMSampleListModelEstimator(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  TrainingSampleListPointer m_TrainingSampleList;
  InputSampleListPointer    m_InputSampleList;

}; // class SVMSampleListModelEstimator

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSVMSampleListModelEstimator.txx"
#endif

#endif
