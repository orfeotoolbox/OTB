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
#ifndef __otbListSampleSource_h
#define __otbListSampleSource_h

#include "itkProcessObject.h"
#include "itkDataObjectDecorator.h"

namespace otb {
namespace Statistics {

/** \class ListSampleSource
 *  \brief This class is a base class for filters using ListSample as
 *  input and returning ListSample.
 *
 *
 *  This filter provides pipeline support since
 *  itk::Statistics::ListSample is a DataObject
 *  the output sample list.
 *
 */
template < class TInputSampleList, class TOutputSampleList = TInputSampleList >
class ITK_EXPORT ListSampleSource :
  public itk::ProcessObject
{
public:
  /** Standard class typedefs */
  typedef ListSampleSource               Self;
  typedef itk::ProcessObject                         Superclass;
  typedef itk::SmartPointer< Self >                  Pointer;
  typedef itk::SmartPointer<const Self>              ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(ListSampleSource, itk::ProcessObject);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** OutputSampleList typedefs */
  typedef TOutputSampleList                                    OutputSampleListType;
  typedef typename OutputSampleListType::Pointer               OutputSampleListPointer;
  typedef typename OutputSampleListType::ConstPointer          OutputSampleListConstPointer;
  typedef typename OutputSampleListType::MeasurementVectorType OutputMeasurementVectorType;
  typedef typename OutputMeasurementVectorType::ValueType      OutputValueType;

  /** ListSample is not a DataObject, we need to decorate it to push it down
   * a ProcessObject's pipeline */
  typedef itk::DataObject::Pointer                             DataObjectPointer;

  /** Returns the output sample list as a data object */
  OutputSampleListType * GetOutput();

protected:
  /** Standard itk::ProcessObject subclass method. */
  virtual DataObjectPointer MakeOutput(unsigned int idx);

  ListSampleSource();
  virtual ~ListSampleSource() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  ListSampleSource(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

}; // end of class ListSampleSource

} // end of namespace Statistics
} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbListSampleSource.txx"
#endif

#endif
