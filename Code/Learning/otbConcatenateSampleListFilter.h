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

#include "otbListSampleToListSampleFilter.h"

namespace otb {
namespace Statistics {

/** \class ConcatenateSampleListFilter
 *  \brief This class concatenates several sample list into a single one.
 *
 * Standard casting is applied between input and output type.
 *
 *
 * \sa ListSampleToListSampleFilter
 */
template < class TInputSampleList, class TOutputSampleList = TInputSampleList >
class ITK_EXPORT ConcatenateSampleListFilter :
  public otb::Statistics::ListSampleToListSampleFilter<TInputSampleList,TOutputSampleList>
{
public:
  /** Standard class typedefs */
  typedef ConcatenateSampleListFilter            Self;
  typedef otb::Statistics::ListSampleToListSampleFilter
  <TInputSampleList,TOutputSampleList>               Superclass;
  typedef itk::SmartPointer< Self >                  Pointer;
  typedef itk::SmartPointer<const Self>              ConstPointer;
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(ConcatenateSampleListFilter,otb::Statistics::ListSampleToListSampleFilter);
  
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
  
  typedef typename Superclass::InputSampleListObjectType       InputSampleListObjectType;  
  typedef typename Superclass::OutputSampleListObjectType      OutputSampleListObjectType;

  /** Method to set/get the input list sample */
  void AddInput( const InputSampleListType * inputPtr );
  void AddInput( const InputSampleListObjectType * inputPtr );

protected:
  /** This method causes the filter to generate its output. */
   virtual void GenerateData();

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
