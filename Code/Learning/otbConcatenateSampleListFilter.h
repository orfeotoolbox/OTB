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
 * For the sake of genericity on the MeasurementVector type, no casting is applied so
 * this filter does not allow different input/output SampleList.
 *
 * \sa ListSampleToListSampleFilter
 */
template < class TSampleList >
class ITK_EXPORT ConcatenateSampleListFilter :
  public otb::Statistics::ListSampleToListSampleFilter<TSampleList, TSampleList>
{
public:
  /** Standard class typedefs */
  typedef ConcatenateSampleListFilter                Self;
  typedef otb::Statistics::ListSampleToListSampleFilter
  <TSampleList, TSampleList>                          Superclass;
  typedef itk::SmartPointer< Self >                  Pointer;
  typedef itk::SmartPointer<const Self>              ConstPointer;
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(ConcatenateSampleListFilter, otb::Statistics::ListSampleToListSampleFilter);
  
  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  
  /** SampleList typedefs */
  typedef TSampleList                                    SampleListType;
  typedef typename SampleListType::Pointer               SampleListPointer;
  typedef typename SampleListType::ConstPointer          SampleListConstPointer;
  typedef typename SampleListType::MeasurementVectorType MeasurementVectorType;
  typedef typename MeasurementVectorType::ValueType      ValueType;
  
  /** Method to set/get the input list sample */
  void AddInput( const SampleListType * inputPtr );

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
