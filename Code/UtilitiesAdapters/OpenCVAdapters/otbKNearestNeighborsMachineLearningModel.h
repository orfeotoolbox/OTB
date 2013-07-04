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
#ifndef __otbKNearestNeighborsMachineLearningModel_h
#define __otbKNearestNeighborsMachineLearningModel_h

#include "itkLightObject.h"
#include "itkVariableLengthVector.h"
#include "itkFixedArray.h"
#include "itkListSample.h"
#include "otbMachineLearningModel.h"

class CvKNearest;

namespace otb
{
template <class TInputValue, class TTargetValue>
class ITK_EXPORT KNearestNeighborsMachineLearningModel
  : public MachineLearningModel <TInputValue, TTargetValue>
{
public:
  /** Standard class typedefs. */
  typedef KNearestNeighborsMachineLearningModel           Self;
  typedef MachineLearningModel<TInputValue, TTargetValue> Superclass;
  typedef itk::SmartPointer<Self>                         Pointer;
  typedef itk::SmartPointer<const Self>                   ConstPointer;

  // Input related typedefs
  typedef TInputValue                                     InputValueType;
  typedef itk::VariableLengthVector<InputValueType>       InputSampleType;
  typedef itk::Statistics::ListSample<InputSampleType>    InputListSampleType;

  // Target related typedefs
  typedef TTargetValue                                    TargetValueType;
  typedef itk::FixedArray<TargetValueType,1>              TargetSampleType;
  typedef itk::Statistics::ListSample<TargetSampleType>   TargetListSampleType;

  /** Run-time type information (and related methods). */
  itkNewMacro(Self);
  itkTypeMacro(KNearestNeighborsMachineLearningModel, itk::MachineLearningModel);

  /** Setters/Getters to the number of neighbors to use
   *  Default is 32
   *  see @http://docs.opencv.org/modules/ml/doc/k_nearest_neighbors.html
   */
  itkGetMacro(K, int);
  itkSetMacro(K, int);

  /** Setters/Getters to IsRegression flag
   *  Default is False
   *  see @http://docs.opencv.org/modules/ml/doc/k_nearest_neighbors.html
   */
  itkGetMacro(IsRegression, bool);
  itkSetMacro(IsRegression, bool);

  /** Train the machine learning model */
  virtual void Train();

  /** Predict values using the model */
  virtual TargetSampleType Predict(const InputSampleType & input) const;

  /** Save the model to file */
  virtual void Save(const std::string & filename, const std::string & name="");

  /** Load the model from file */
  virtual void Load(const std::string & filename, const std::string & name="");

  /**\name Classification model file compatibility tests */
  //@{
  /** Is the input model file readable and compatible with the corresponding classifier ? */
  virtual bool CanReadFile(const std::string &);

  /** Is the input model file writable and compatible with the corresponding classifier ? */
  virtual bool CanWriteFile(const std::string &);
  //@}

protected:
  /** Constructor */
  KNearestNeighborsMachineLearningModel();

  /** Destructor */
  virtual ~KNearestNeighborsMachineLearningModel();

  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  KNearestNeighborsMachineLearningModel(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  CvKNearest * m_KNearestModel;
  int m_K;
  int m_IsRegression;
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbKNearestNeighborsMachineLearningModel.txx"
#endif

#endif
