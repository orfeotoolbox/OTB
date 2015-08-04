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

#include "otbRequiresOpenCVCheck.h"

#include "itkLightObject.h"
#include "itkFixedArray.h"
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

  typedef typename Superclass::InputValueType             InputValueType;
  typedef typename Superclass::InputSampleType            InputSampleType;
  typedef typename Superclass::InputListSampleType        InputListSampleType;
  typedef typename Superclass::TargetValueType            TargetValueType;
  typedef typename Superclass::TargetSampleType           TargetSampleType;
  typedef typename Superclass::TargetListSampleType       TargetListSampleType;
  typedef typename Superclass::ConfidenceValueType        ConfidenceValueType;

  /** Run-time type information (and related methods). */
  itkNewMacro(Self);
  itkTypeMacro(KNearestNeighborsMachineLearningModel, MachineLearningModel);

  /** Setters/Getters to the number of neighbors to use
   *  Default is 32
   *  \see http://docs.opencv.org/modules/ml/doc/k_nearest_neighbors.html
   */
  itkGetMacro(K, int);
  itkSetMacro(K, int);

  /** Setters/Getters to IsRegression flag
   *  Default is False
   *  \see http://docs.opencv.org/modules/ml/doc/k_nearest_neighbors.html
   */
  itkGetMacro(IsRegression, bool);
  itkSetMacro(IsRegression, bool);

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

  /** Train the machine learning model */
  virtual void TrainClassification();
  /** Predict values using the model */
  virtual TargetSampleType PredictClassification(const InputSampleType& input, ConfidenceValueType *quality=NULL) const;

private:
  KNearestNeighborsMachineLearningModel(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  CvKNearest * m_KNearestModel;
  int m_K;
  bool m_IsRegression;
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbKNearestNeighborsMachineLearningModel.txx"
#endif

#endif
