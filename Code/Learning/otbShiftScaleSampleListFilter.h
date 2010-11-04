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
#ifndef __otbShiftScaleSampleListFilter_h
#define __otbShiftScaleSampleListFilter_h

#include "itkProcessObject.h"
#include "itkDataObject.h"
#include "itkDataObjectDecorator.h"

namespace otb {
namespace Statistics {

/** \class ShiftScaleSampleListFilter
 *  \brief This class generate a shifted and scaled version of the input sample list
 *
 *  For each component of the sample, the following formula is applied :
 *
 *  \f[ output = \frac{input - shift}{scale} \f]
 *
 * Standard casting is applied between input and output type.
 *
 * Shifts and scales can be set via the SetShift() and SetScales() methods.
 *
 * This filter provide pipeline support for itk::Statistics::ListSample via itk::DataObjectDecorator for
 * both the input and the output sample list.
 *
 */
template < class TInputSampleList, class TOutputSampleList = TInputSampleList >
class ITK_EXPORT ShiftScaleSampleListFilter :
  public itk::ProcessObject
{
public:
  /** Standard class typedefs */
  typedef ShiftScaleSampleListFilter            Self;
  typedef itk::ProcessObject                         Superclass;
  typedef itk::SmartPointer< Self >                  Pointer;
  typedef itk::SmartPointer<const Self>              ConstPointer;
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(ShiftScaleSampleListFilter,itk::ProcessObject);
  
  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  
  /** InputSampleList typedefs */
  typedef TInputSampleList                                    InputSampleListType;
  typedef typename InputSampleListType::Pointer               InputSampleListPointer;
  typedef typename InputSampleListType::ConstPointer          InputSampleListConstPointer;
  typedef typename InputSampleListType::MeasurementVectorType InputMeasurementVectorType;
  typedef typename InputMeasurementVectorType::ValueType      InputValueType;

  /** OutputSampleList typedefs */
  typedef TOutputSampleList                                    OutputSampleListType;
  typedef typename OutputSampleListType::Pointer               OutputSampleListPointer;
  typedef typename OutputSampleListType::ConstPointer          OutputSampleListConstPointer;
  typedef typename OutputSampleListType::MeasurementVectorType OutputMeasurementVectorType;
  typedef typename OutputMeasurementVectorType::ValueType      OutputValueType;
  
  /** ListSample is not a DataObject, we need to decorate it to push it down
   * a ProcessObject's pipeline */
  typedef itk::DataObject::Pointer                             DataObjectPointer;

  typedef itk::DataObjectDecorator< InputSampleListType >      InputSampleListObjectType;
  typedef itk::DataObjectDecorator< OutputSampleListType >     OutputSampleListObjectType;

  /** Set/Get the Shifts for this sample list */
  itkSetMacro(Shifts,InputMeasurementVectorType);
  itkGetMacro(Shifts,InputMeasurementVectorType);

  /** Set/Get the Scales for this sample list */
  itkSetMacro(Scales,InputMeasurementVectorType);
  itkGetMacro(Scales,InputMeasurementVectorType);

  /** Method to set/get the input list sample */
  void SetInput( const InputSampleListType * inputPtr );
  void SetInput( const InputSampleListObjectType * inputPtr );

  /** Returns the input sample list */
  const InputSampleListType * GetInputSampleList() const;

  /** Returns the input sample list as a data object */
  const InputSampleListObjectType * GetInput() const;

  /** Returns the output sample list */
  OutputSampleListType * GetOutputSampleList();

  /** Returns the output sample list as a data object */
  OutputSampleListObjectType * GetOutput();


protected:
  /** This method causes the filter to generate its output. */
   virtual void GenerateData();

   /** Standard itk::ProcessObject subclass method. */
   virtual DataObjectPointer MakeOutput(unsigned int idx);

  ShiftScaleSampleListFilter();
  virtual ~ShiftScaleSampleListFilter() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  ShiftScaleSampleListFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** The vector of Shifts */
  InputMeasurementVectorType m_Shifts;

  /** The vector of Scales */
  InputMeasurementVectorType m_Scales;

}; // end of class ImageToListGenerator

} // end of namespace Statistics
} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbShiftScaleSampleListFilter.txx"
#endif

#endif
