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
#ifndef DimensionalityReductionModel_h
#define DimensionalityReductionModel_h

#include "itkObject.h"
#include "itkVariableLengthVector.h"
#include "itkListSample.h"

namespace otb
{

/** \class DimensionalityReductionModel
 * \brief DimensionalityReductionModel is the base class for all dimensionality Reduction objects (PCA, autoencoders and SOM) implemented in the dimensionality Reduction framework of the OTB.
 *
 * DimensionalityReductionModel is an abstract object that specifies behavior and
 * interface of dimensionality reduction algorithms (PCA, autoencoders and SOM) in the generic dimensionality Reduction framework of the OTB.
 * The main generic virtual methods specifically implemented in each model
 * derived from the DimensionalityReductionModel class are two learning-related methods:
 * Train() and Save(), and three dimensionality reduction related methods: Load(),
 * DoPredict() and optionnaly DoPredictBatch().
 *
 * Thus, each classifier derived from the MachineLearningModel class
 * computes its corresponding model with Train() and exports it with
 * the help of the Save() method.
 *
 * It is also possible to reduce the dimensionality of any input sample composed of several
 * features (or any number of bands in the case of a pixel extracted
 * from a multi-band image) with the help of the Predict() method which
 * needs a previous loading of the classification model with the Load() method.
 *
 * \sa DimensionalityReductionModelFactory
 * \sa SOMModel
 * \sa PCAModel
 * \sa AutoencderModel
 * \sa ImageDimensionalityReductionFilter
 *
 * \ingroup cbDimensionalityReduction
 */
template <class TInputValue, class TTargetValue>
class ITK_EXPORT DimensionalityReductionModel
  : public itk::Object
{
public:
  /**\name Standard ITK typedefs */
  //@{
  typedef DimensionalityReductionModel                  Self;
  typedef itk::Object                                   Superclass;
  typedef itk::SmartPointer<Self>                       Pointer;
  typedef itk::SmartPointer<const Self>                 ConstPointer;
  //@}

  /**\name Input related typedefs */
  //@{
  typedef TInputValue                                   InputValueType;
  typedef itk::VariableLengthVector<InputValueType>     InputSampleType;
  typedef itk::Statistics::ListSample<InputSampleType>  InputListSampleType;
  //@}

  /**\name Target related typedefs */
  //@{
  typedef TTargetValue                                  TargetValueType;
  typedef itk::VariableLengthVector<TargetValueType>    TargetSampleType;
  typedef itk::Statistics::ListSample<TargetSampleType> TargetListSampleType;
  //@}

  /**\name Standard macros */
  //@{
  /** Run-time type information (and related methods). */
  itkTypeMacro(DimensionalityReductionModel, itk::Object);
  //@}

  /** Train the machine learning model */
  virtual void Train() =0;

  /** Predict a single sample
    * \param input The sample
    * \param quality A pointer to the quality variable were to store
    * quality value, or NULL
    * \return The predicted label
     */
  TargetSampleType Predict(const InputSampleType& input) const;



  /** Predict a batch of samples (InputListSampleType)
    * \param input The batch of sample to predict
    * \param quality A pointer to the list were to store
    * quality value, or NULL
    * \return The predicted labels
    * Note that this method will be multi-threaded if OTB is built
    * with OpenMP.
     */
  typename TargetListSampleType::Pointer PredictBatch(const InputListSampleType * input) const;
  
  /** Get the size of the output after dimensionality reduction */
  virtual unsigned int GetDimension() = 0;

  /**\name Dimensionality Reduction model file manipulation */
  //@{
  /** Save the model to file */
  virtual void Save(const std::string & filename, const std::string & name="") = 0;

  /** Load the model from file */
  virtual void Load(const std::string & filename, const std::string & name="") = 0;
  //@}

  /**\name Classification model file compatibility tests */
  //@{
  /** Is the input model file readable and compatible with the corresponding model ? */
  virtual bool CanReadFile(const std::string &) = 0;

  /** Is the input model file writable and compatible with the corresponding model ? */
  virtual bool CanWriteFile(const std::string &)  = 0;
  //@}

  /**\name Input list of samples accessors */
  //@{
  itkSetObjectMacro(InputListSample,InputListSampleType);
  itkGetObjectMacro(InputListSample,InputListSampleType);
  itkGetConstObjectMacro(InputListSample,InputListSampleType);
  //@}


 

protected:
  /** Constructor */
  DimensionalityReductionModel();

  /** Destructor */
  ~DimensionalityReductionModel() ITK_OVERRIDE;
 
  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const ITK_OVERRIDE;

  /** Input list sample */
  typename InputListSampleType::Pointer m_InputListSample;

  /** Is DoPredictBatch multi-threaded ? */
  bool m_IsDoPredictBatchMultiThreaded;
  
private:
  /**  Actual implementation of BatchPredicition
    *  Default implementation will call DoPredict iteratively 
    *  \param input The input batch
    *  \param startIndex Index of the first sample to reduce
    *  \param size Number of samples to reduce
    *  \param target Pointer to the list of reduced samples
    * 
    * Override me if internal implementation allows for batch
    * prediction.
    * 
    * Also set m_IsDoPredictBatchMultiThreaded to true if internal
    * implementation allows for parallel batch prediction.
    */
  virtual void DoPredictBatch(const InputListSampleType * input, const unsigned int & startIndex, const unsigned int & size, TargetListSampleType * target) const;

  /** Actual implementation of single sample reduction
   *  \param input sample to reduce
   *  \return The reduced sample
   */ 
  virtual TargetSampleType DoPredict(const InputSampleType& input) const = 0;  
 
  DimensionalityReductionModel(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "DimensionalityReductionModel.txx"
#endif

#endif
