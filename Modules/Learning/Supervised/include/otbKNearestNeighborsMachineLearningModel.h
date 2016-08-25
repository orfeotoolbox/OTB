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
#ifndef otbKNearestNeighborsMachineLearningModel_h
#define otbKNearestNeighborsMachineLearningModel_h

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

  /** Decision rule once the KNN are found :
   *  [for classification]
   *   - KNN_VOTING : output value with maximum occurences (for classification)
   *  [for regression]
   *   - KNN_MEAN : output mean value of neighbors
   *   - KNN_MEDIAN : output median value of neighbors
   */
  enum {KNN_VOTING, KNN_MEAN, KNN_MEDIAN};

  /** Setters/Getters to the decision rule */
  itkGetMacro(DecisionRule, int);
  itkSetMacro(DecisionRule, int);

  /** Train the machine learning model */
  void Train() ITK_OVERRIDE;

  /** Save the model to file */
  void Save(const std::string & filename, const std::string & name="") ITK_OVERRIDE;

  /** Load the model from file */
  void Load(const std::string & filename, const std::string & name="") ITK_OVERRIDE;

  /**\name Classification model file compatibility tests */
  //@{
  /** Is the input model file readable and compatible with the corresponding classifier ? */
  bool CanReadFile(const std::string &) ITK_OVERRIDE;

  /** Is the input model file writable and compatible with the corresponding classifier ? */
  bool CanWriteFile(const std::string &) ITK_OVERRIDE;
  //@}

protected:
  /** Constructor */
  KNearestNeighborsMachineLearningModel();

  /** Destructor */
  ~KNearestNeighborsMachineLearningModel() ITK_OVERRIDE;

  /** Predict values using the model */
  TargetSampleType DoPredict(const InputSampleType& input, ConfidenceValueType *quality=NULL) const ITK_OVERRIDE;

  
  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const ITK_OVERRIDE;

private:
  KNearestNeighborsMachineLearningModel(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  CvKNearest * m_KNearestModel;
  int m_K;

  int m_DecisionRule;
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbKNearestNeighborsMachineLearningModel.txx"
#endif

#endif
