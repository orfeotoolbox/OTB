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
#ifndef __otbSharkRandomForestsMachineLearningModel_h
#define __otbSharkRandomForestsMachineLearningModel_h

#include "otbRequiresSharkCheck.h"

#include "itkLightObject.h"
#include "otbMachineLearningModel.h"

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Woverloaded-virtual"
#pragma GCC diagnostic ignored "-Wignored-qualifiers"
#endif
#include "shark/Algorithms/Trainers/RFTrainer.h"
#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif


/** \class SharkRandomForestsMachineLearningModel
 *  \brief Shark version of Random Forests algorithm
 *
 *  This is a specialization of MachineLearningModel class allowing to
 *  use Shark implementation of the Random Forests algorithm.
 *
 *  It is noteworthy that training step is parallel.
 * 
 *  For more information, see
 *  http://image.diku.dk/shark/doxygen_pages/html/classshark_1_1_r_f_trainer.html
 */

namespace otb
{
template <class TInputValue, class TTargetValue>
class ITK_EXPORT SharkRandomForestsMachineLearningModel
  : public MachineLearningModel <TInputValue, TTargetValue>
{
public:
  /** Standard class typedefs. */
  typedef SharkRandomForestsMachineLearningModel               Self;
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
  typedef typename Superclass::ConfidenceSampleType       ConfidenceSampleType;
  typedef typename Superclass::ConfidenceListSampleType   ConfidenceListSampleType;
  
  /** Run-time type information (and related methods). */
  itkNewMacro(Self);
  itkTypeMacro(SharkRandomForestsMachineLearningModel, MachineLearningModel);

  /** Train the machine learning model */
  virtual void Train() ITK_OVERRIDE;

  /** Save the model to file */
  virtual void Save(const std::string & filename, const std::string & name="") ITK_OVERRIDE;

  /** Load the model from file */
  virtual void Load(const std::string & filename, const std::string & name="") ITK_OVERRIDE;

  /**\name Classification model file compatibility tests */
  //@{
  /** Is the input model file readable and compatible with the corresponding classifier ? */
  virtual bool CanReadFile(const std::string &) ITK_OVERRIDE;

  /** Is the input model file writable and compatible with the corresponding classifier ? */
  virtual bool CanWriteFile(const std::string &) ITK_OVERRIDE;
  //@}

  /** From Shark doc: Get the number of trees to grow.*/
  itkGetMacro(NumberOfTrees,unsigned int);
  /** From Shark doc: Set the number of trees to grow.*/
  itkSetMacro(NumberOfTrees,unsigned int);

  /** From Shark doc: Get the number of random attributes to investigate at each node.*/
  itkGetMacro(MTry, unsigned int);
  /** From Shark doc: Set the number of random attributes to investigate at each node.*/
  itkSetMacro(MTry, unsigned int);

  /** From Shark doc: Controls when a node is considered pure. If set
* to 1, a node is pure when it only consists of a single node.
*/
  itkGetMacro(NodeSize, unsigned int);
    /** From Shark doc: Controls when a node is considered pure. If
* set to 1, a node is pure when it only consists of a single node.
 */
  itkSetMacro(NodeSize, unsigned int);

  /** From Shark doc: Get the fraction of the original training
* dataset to use as the out of bag sample. The default value is
* 0.66.*/
  itkGetMacro(OobRatio, float);

  /** From Shark doc: Set the fraction of the original training
* dataset to use as the out of bag sample. The default value is 0.66.
*/
  itkSetMacro(OobRatio, float);

  /** If true, margin confidence value will be computed */
  itkGetMacro(ComputeMargin, bool);
  /** If true, margin confidence value will be computed */
  itkSetMacro(ComputeMargin, bool);

protected:
  /** Constructor */
  SharkRandomForestsMachineLearningModel();

  /** Destructor */
  virtual ~SharkRandomForestsMachineLearningModel();

  /** Predict values using the model */
  virtual TargetSampleType DoPredict(const InputSampleType& input, ConfidenceValueType *quality=ITK_NULLPTR) const ITK_OVERRIDE;

  
  virtual void DoPredictBatch(const InputListSampleType *, const unsigned int & startIndex, const unsigned int & size, TargetListSampleType *, ConfidenceListSampleType * = ITK_NULLPTR) const ITK_OVERRIDE;
  
  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  SharkRandomForestsMachineLearningModel(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  shark::RFClassifier m_RFModel;
  shark::RFTrainer m_RFTrainer;

  unsigned int m_NumberOfTrees;
  unsigned int m_MTry;
  unsigned int m_NodeSize;
  float m_OobRatio;
  bool m_ComputeMargin;

  /** Confidence list sample */
  ConfidenceValueType ComputeConfidence(shark::RealVector & probas, 
                                        bool computeMargin) const;

};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSharkRandomForestsMachineLearningModel.txx"
#endif

#endif
