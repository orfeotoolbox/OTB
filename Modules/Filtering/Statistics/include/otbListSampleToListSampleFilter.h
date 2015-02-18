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
#ifndef __otbListSampleToListSampleFilter_h
#define __otbListSampleToListSampleFilter_h

#include "otbListSampleSource.h"

namespace otb {
namespace Statistics {

/** \class ListSampleToListSampleFilter
 *  \brief This class is a base class for fitlers using ListSample as
 *  input and returning ListSample.
 *
 *
 *  This filter provide pipeline support for itk::Statistics::ListSample via itk::DataObjectDecorator for
 *  both the input and the output sample list.
 *
 */
template < class TInputSampleList, class TOutputSampleList = TInputSampleList >
class ITK_EXPORT ListSampleToListSampleFilter :
  public ListSampleSource<TOutputSampleList>
{
public:
  /** Standard class typedefs */
  typedef ListSampleToListSampleFilter               Self;
  typedef ListSampleSource<TOutputSampleList>        Superclass;
  typedef itk::SmartPointer< Self >                  Pointer;
  typedef itk::SmartPointer<const Self>              ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(ListSampleToListSampleFilter, ListSampleSource);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** InputSampleList typedefs */
  typedef TInputSampleList                                    InputSampleListType;
  typedef typename InputSampleListType::Pointer               InputSampleListPointer;
  typedef typename InputSampleListType::ConstPointer          InputSampleListConstPointer;
  typedef typename InputSampleListType::MeasurementVectorType InputMeasurementVectorType;
  typedef typename InputMeasurementVectorType::ValueType      InputValueType;

  /** OutputSampleList typedefs */
  typedef typename Superclass::OutputSampleListType            OutputSampleListType;
  typedef typename Superclass::OutputSampleListPointer         OutputSampleListPointer;
  typedef typename Superclass::OutputSampleListConstPointer    OutputSampleListConstPointer;
  typedef typename Superclass::OutputMeasurementVectorType     OutputMeasurementVectorType;
  typedef typename Superclass::OutputValueType                 OutputValueType;

  /** ListSample is not a DataObject, we need to decorate it to push it down
   * a ProcessObject's pipeline */
  typedef itk::DataObject::Pointer                             DataObjectPointer;
  //typedef itk::DataObjectDecorator< InputSampleListType >      InputSampleListObjectType;
  //typedef typename Superclass::OutputSampleListObjectType      OutputSampleListObjectType;

  /** Method to set/get the input list sample */
  void SetInput( const InputSampleListType * inputPtr );
  //void SetInput( const InputSampleListObjectType * inputPtr );

  /** Returns the input sample list */
  //const InputSampleListType * GetInputSampleList() const;

  /** Returns the input sample list as a data object */
  const InputSampleListType * GetInput() const;

protected:

  ListSampleToListSampleFilter();
  virtual ~ListSampleToListSampleFilter() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  ListSampleToListSampleFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

}; // end of class ListSampleToListSampleFilter

} // end of namespace Statistics
} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbListSampleToListSampleFilter.txx"
#endif

#endif
