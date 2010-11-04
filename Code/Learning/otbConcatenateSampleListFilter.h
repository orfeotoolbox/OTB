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
#ifndef __otbConcatenateSampleListFilter_h
#define __otbConcatenateSampleListFilter_h

#include "itkProcessObject.h"
#include "itkDataObject.h"
#include "itkDataObjectDecorator.h"

namespace otb {
namespace Statistics {

/** \class ConcatenateSampleListFilter
 *  \brief This class concatenates several sample list into a single one.
 *
 * Standard casting is applied between input and output type.
 *
 * This filter provide pipeline support for itk::Statistics::ListSample via itk::DataObjectDecorator for
 * both the input and the output sample list.
 *
 * \sa ImageToListAdaptor
 */
template < class TInputSampleList, class TOutputSampleList = TInputSampleList >
class ITK_EXPORT ConcatenateSampleListFilter :
  public itk::ProcessObject
{
public:
  /** Standard class typedefs */
  typedef ConcatenateSampleListFilter            Self;
  typedef itk::ProcessObject                         Superclass;
  typedef itk::SmartPointer< Self >                  Pointer;
  typedef itk::SmartPointer<const Self>              ConstPointer;
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(ConcatenateSampleListFilter,itk::ProcessObject);
  
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

  /** Method to set/get the input list sample */
  void AddInput( const InputSampleListType * inputPtr );
  void AddInput( const InputSampleListObjectType * inputPtr );

  /** Returns the output sample list */
  OutputSampleListType * GetOutputSampleList();

  /** Returns the output sample list as a data object */
  OutputSampleListObjectType * GetOutput();


protected:
  /** This method causes the filter to generate its output. */
   virtual void GenerateData();

   /** Standard itk::ProcessObject subclass method. */
   virtual DataObjectPointer MakeOutput(unsigned int idx);

  ConcatenateSampleListFilter();
  virtual ~ConcatenateSampleListFilter() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  ConcatenateSampleListFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
}; // end of class ImageToListGenerator

} // end of namespace Statistics
} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbConcatenateSampleListFilter.txx"
#endif

#endif
