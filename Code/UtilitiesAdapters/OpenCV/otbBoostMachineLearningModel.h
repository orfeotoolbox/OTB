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
#ifndef __otbBoostMachineLearningModel_h
#define __otbBoostMachineLearningModel_h

#include "itkLightObject.h"
#include "itkVariableLengthVector.h"
#include "itkFixedArray.h"
#include "itkListSample.h"
#include "otbMachineLearningModel.h"


class CvBoost;

namespace otb
{
template <class TInputValue, class TTargetValue>
class ITK_EXPORT BoostMachineLearningModel
  : public MachineLearningModel <TInputValue, TTargetValue>
{
public:
  /** Standard class typedefs. */
  typedef BoostMachineLearningModel           Self;
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
  itkTypeMacro(BoostMachineLearningModel, itk::MachineLearningModel);

  /** Setters/Getters to the Boost type
   *  It can be CvBoost::DISCRETE, CvBoost::REAL, CvBoost::LOGIT, CvBoost::GENTLE
   *  Default is CvBoost::REAL.
   *  \see http://docs.opencv.org/modules/ml/doc/boosting.html#cvboostparams-cvboostparams
   */
  itkGetMacro(BoostType, int);
  itkSetMacro(BoostType, int);

  /** Setters/Getters to the split criteria
   *  It can be CvBoost::DEFAULT, CvBoost::GINI, CvBoost::MISCLASS, CvBoost::SQERR
   *  Default is CvBoost::DEFAULT. It uses default value according to \c BoostType
   *  \see http://docs.opencv.org/modules/ml/doc/boosting.html#cvboost-predict
   */
  itkGetMacro(SplitCrit, int);
  itkSetMacro(SplitCrit, int);

  /** Setters/Getters to the number of weak classifiers.
   *  Default is 100.
   *  \see http://docs.opencv.org/modules/ml/doc/boosting.html#cvboostparams-cvboostparams
   */
  itkGetMacro(WeakCount, int);
  itkSetMacro(WeakCount, int);

  /** Setters/Getters to the threshold WeightTrimRate.
   *  A threshold between 0 and 1 used to save computational time.
   *  Samples with summary weight \leq 1 - WeightTrimRate do not participate in the next iteration of training.
   *  Set this parameter to 0 to turn off this functionality.
   *  Default is 0.95
   *  \see http://docs.opencv.org/modules/ml/doc/boosting.html#cvboostparams-cvboostparams
   */
  itkGetMacro(WeightTrimRate, double);
  itkSetMacro(WeightTrimRate, double);

  /** Setters/Getters to the maximum depth of the tree.
   * Default is 1
   * \see http://docs.opencv.org/modules/ml/doc/decision_trees.html#CvDTreeParams::CvDTreeParams%28%29
   */
  itkGetMacro(MaxDepth, int);
  itkSetMacro(MaxDepth, int);


  /** Train the machine learning model */
  virtual void Train();

  /** Predict values using the model */
  virtual TargetSampleType Predict(const InputSampleType & input) const;

  /** Save the model to file */
  virtual void Save(const std::string & filename, const std::string & name="");

  /** Load the model from file */
  virtual void Load(const std::string & filename, const std::string & name="");

  /** Determine the file type. Returns true if this ImageIO can read the
   * file specified. */
  virtual bool CanReadFile(const std::string &);

  /** Determine the file type. Returns true if this ImageIO can write the
   * file specified. */
  virtual bool CanWriteFile(const std::string &);

protected:
  /** Constructor */
  BoostMachineLearningModel();

  /** Destructor */
  virtual ~BoostMachineLearningModel();

  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  BoostMachineLearningModel(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  CvBoost * m_BoostModel;
  int m_BoostType;
  int m_SplitCrit;
  int m_WeakCount;
  double m_WeightTrimRate;
  int m_MaxDepth;
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbBoostMachineLearningModel.txx"
#endif

#endif
