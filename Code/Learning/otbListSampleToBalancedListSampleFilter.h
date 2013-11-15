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
#ifndef __otbListSampleToBalancedListSampleFilter_h
#define __otbListSampleToBalancedListSampleFilter_h

#include "otbListSampleToListSampleFilter.h"
#include "otbGaussianAdditiveNoiseSampleListFilter.h"
#include "itkDataObjectDecorator.h"
#include "otbMacro.h"

namespace otb {
namespace Statistics {

/** \class ListSampleToBalancedListSampleFilter
 *  \brief This class generate a balanced ListSample in order to have
 *  fair distribution of learning samples.
 *
 * The maximum number of samples with same labels are first
 * computed. This maximum number by the m_BalacingFactor determines
 * the final number of samples belonging to each label.
 *
 * Mean and Variance are set via the methods SetMean() and SetVariance().
 *
 * \sa ListSampleToListSampleFilter, GaussianAdditiveNoiseSampleListFilter
 */
template < class TInputSampleList,
           class TLabelSampleList,
           class TOutputSampleList = TInputSampleList >
class ITK_EXPORT ListSampleToBalancedListSampleFilter :
    public otb::Statistics::ListSampleToListSampleFilter<TInputSampleList,
                                                         TOutputSampleList>
{
public:
  /** Standard class typedefs */
  typedef ListSampleToBalancedListSampleFilter       Self;
  typedef otb::Statistics::ListSampleToListSampleFilter
  <TInputSampleList, TOutputSampleList>               Superclass;
  typedef itk::SmartPointer< Self >                  Pointer;
  typedef itk::SmartPointer<const Self>              ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(ListSampleToBalancedListSampleFilter, otb::Statistics::ListSampleToListSampleFilter);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** InputSampleList typedefs */
  typedef TInputSampleList                                    InputSampleListType;
  typedef typename InputSampleListType::Pointer               InputSampleListPointer;
  typedef typename InputSampleListType::ConstPointer          InputSampleListConstPointer;
  typedef typename InputSampleListType::MeasurementVectorType InputMeasurementVectorType;
  typedef typename InputMeasurementVectorType::ValueType      InputValueType;

  /** LabelSampleList typedefs */
  typedef TLabelSampleList                                    LabelSampleListType;
  typedef typename LabelSampleListType::Pointer               LabelSampleListPointer;
  typedef typename LabelSampleListType::ConstPointer          LabelSampleListConstPointer;
  typedef typename LabelSampleListType::MeasurementVectorType LabelMeasurementVectorType;
  typedef typename LabelMeasurementVectorType::ValueType      LabelValueType;
  typedef itk::DataObjectDecorator< LabelSampleListType >     LabelSampleListObjectType;

  /** OutputSampleList typedefs */
  typedef TOutputSampleList                                    OutputSampleListType;
  typedef typename OutputSampleListType::Pointer               OutputSampleListPointer;
  typedef typename OutputSampleListType::ConstPointer          OutputSampleListConstPointer;
  typedef typename OutputSampleListType::MeasurementVectorType OutputMeasurementVectorType;
  typedef typename OutputMeasurementVectorType::ValueType      OutputValueType;

  /** DataObject typedef*/
  typedef typename Superclass::DataObjectPointer               DataObjectPointer;

  /** Filter adding noise to a ListSample */
  typedef otb::Statistics::GaussianAdditiveNoiseSampleListFilter
  <InputSampleListType, OutputSampleListType>                   GaussianAdditiveNoiseType;
  typedef typename GaussianAdditiveNoiseType::Pointer          GaussianAdditiveNoisePointerType;

  /** Get/Set the label sample list */
  void SetInputLabel( const LabelSampleListType * label );

  /** Returns the label sample list */
  const LabelSampleListType * GetInputLabel() const;

  /** Returns the label sample list as a data object */
  LabelSampleListType * GetOutputLabel();

  /** Set/Get the mean for the white gaussian noise to generate  */
  otbSetObjectMemberMacro(AddGaussianNoiseFilter, Mean, double);
  otbGetObjectMemberConstMacro(AddGaussianNoiseFilter, Mean, double);

  /** Set/Get the variance for the white gaussian noise to generate  */
  otbSetObjectMemberMacro(AddGaussianNoiseFilter, Variance, double);
  otbGetObjectMemberConstMacro(AddGaussianNoiseFilter, Variance, double);

  /** Set/Get the multiplicative factor : this value is used to
    * determine the maximum number of samples in each label in order
    * to reach a balanced output ListSample
    */
  itkSetMacro(BalancingFactor, unsigned int);
  itkGetMacro(BalancingFactor, unsigned int);

protected:
  /** This method causes the filter to generate its output. */
  virtual void GenerateData();

  /** In order to respect the fair data principle, the number of samples for
    * each label must be the same. This method computes the label that
    * have the higher number of sample.
    */
  void ComputeMaxSampleFrequency();

  /** Make Output */
  DataObjectPointer MakeOutput(unsigned int idx);

  ListSampleToBalancedListSampleFilter();
  virtual ~ListSampleToBalancedListSampleFilter() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  ListSampleToBalancedListSampleFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  GaussianAdditiveNoisePointerType       m_AddGaussianNoiseFilter;
  std::vector<unsigned int>              m_MultiplicativeCoefficient;
  unsigned int                           m_BalancingFactor;

}; // end of class ListSampleToBalancedListSampleFilter

} // end of namespace Statistics
} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbListSampleToBalancedListSampleFilter.txx"
#endif

#endif
