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
#ifndef __otbLabelMapWithClassLabelToLabeledSampleListFilter_h
#define __otbLabelMapWithClassLabelToLabeledSampleListFilter_h

#include "otbLabelMapToSampleListFilter.h"

namespace otb
{
/** \class LabelMapWithClassLabelToLabeledSampleListFilter
 * \brief This class converts a LabelObjectMap with some class labeled
 * objets to a SampleList and a TrainingSampleList suitable for supervised learning,
 * and compatible with otb::SVMSampleListModelEstimator
 *
 * The conversion from label object to measurement vector is made
 * through a functor. This allows supporting any kind of LabelObject
 * via proper redefinition of the functor.
 *
 * \sa otb::SVMSampleListModelEstimator
 *
 * \ingroup OTBLabelMap
 */

template <class TInputLabelMap, class TOutputSampleList, class TOutputTrainingSampleList,
    class TMeasurementFunctor = Functor::AttributesMapMeasurementFunctor
    <typename TInputLabelMap::LabelObjectType, typename TOutputSampleList::MeasurementVectorType > >
class ITK_EXPORT LabelMapWithClassLabelToLabeledSampleListFilter :
    public LabelMapToSampleListFilter<TInputLabelMap, TOutputSampleList,TMeasurementFunctor>
{
public:
  /** Standard class typedefs. */
  typedef LabelMapWithClassLabelToLabeledSampleListFilter Self;
  typedef LabelMapToSampleListFilter
  <TInputLabelMap, TOutputSampleList,TMeasurementFunctor> Superclass;
  typedef itk::SmartPointer<Self>                         Pointer;
  typedef itk::SmartPointer<const Self>                   ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(LabelMapWithClassLabelToLabeledSampleListFilter, LabelMapToSampleListFilter);

  /** template typedefs */
  typedef TInputLabelMap                              InputLabelMapType;
  typedef typename InputLabelMapType::ConstPointer    InputLabelMapConstPointerType;
  typedef typename InputLabelMapType::LabelObjectType LabelObjectType;
  typedef typename InputLabelMapType::ConstIterator   ConstIteratorType;
  typedef itk::ProcessObject::DataObjectPointerArraySizeType DataObjectPointerArraySizeType;

  /** Output sample list */
  typedef TOutputSampleList                           OutputSampleListType;
  typedef typename OutputSampleListType::Pointer      OutputSampleListPointerType;
  typedef typename OutputSampleListType
  ::MeasurementVectorType                             MeasurementVectorType;

  /** Output training sample list */
  typedef TOutputTrainingSampleList                      OutputTrainingSampleListType;
  typedef typename OutputTrainingSampleListType::Pointer OutputTrainingSampleListPointerType;
  typedef typename OutputTrainingSampleListType
  ::MeasurementVectorType                                TraningVectorType;

  /** Measurement functor */
  typedef TMeasurementFunctor                            MeasurementFunctorType;

  // DataObject type definition from superclass
  typedef typename Superclass::DataObjectPointerType     DataObjectPointerType;

  // Get the output training ListSample
  const OutputTrainingSampleListType* GetOutputTrainingSampleList();

  // Get a hook on the functor for settings
  void SetMeasurementFunctor(const MeasurementFunctorType& functor)
  {
    m_MeasurementFunctor = functor;
  }

  MeasurementFunctorType & GetMeasurementFunctor()
  {
    return m_MeasurementFunctor;
  }

protected:
  LabelMapWithClassLabelToLabeledSampleListFilter();
  virtual ~LabelMapWithClassLabelToLabeledSampleListFilter();

  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

  virtual void GenerateData();

  /** Make Output */
  DataObjectPointerType MakeOutput(DataObjectPointerArraySizeType idx);
  using Superclass::MakeOutput;

private:
  LabelMapWithClassLabelToLabeledSampleListFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** The functor used to build the measurement vector */
  MeasurementFunctorType              m_MeasurementFunctor;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLabelMapWithClassLabelToLabeledSampleListFilter.txx"
#endif

#endif
