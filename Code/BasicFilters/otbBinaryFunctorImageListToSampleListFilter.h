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
#ifndef __otbBinaryFunctorImageListToSampleListFilter_h
#define __otbBinaryFunctorImageListToSampleListFilter_h

#include "itkProcessObject.h"
#include "itkDataObject.h"
#include "itkDataObjectDecorator.h"

#include "otbImageList.h"
#include "itkImageRegionConstIterator.h"

namespace otb {

/** \class BinaryFunctorImageListToSampleListFilter
 * \brief Generate join sample from 2 set of ImageList
 *
 * This class transform the initial data contained in 2 imageList to
 * yield a set of join sample \f$(x_i,y_i)\f$ or a set of parameters
 * extracted from the 2 image lists.
 *
 * The Function gives the possibility to select some of the samples only 
 * and to transform the output data. Hence, the functor should have also a IsToGenerate 
 * boolean function...
 *
 * This filter provides pipeline support for itk::Statistics::ListSample via
 * itk::DataObjectDecorator for the output sample list.
 *
 * \sa SparseWvltToAngleMapperListFilter
 *
 */
template < class TInputImageList, class TOutputSampleList, class TFunction >
class ITK_EXPORT BinaryFunctorImageListToSampleListFilter
  : public itk::ProcessObject
{
public:
  /** Standard class typedefs. */
  typedef BinaryFunctorImageListToSampleListFilter Self;
  typedef itk::ProcessObject                       Superclass;
  typedef itk::SmartPointer< Self >                Pointer;
  typedef itk::SmartPointer<const Self>            ConstPointer;
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(BinaryFunctorImageListToSampleListFilter, itk::ProcessObject);
  
  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** InputList typedefs */
  typedef TInputImageList InputImageListType;
  typedef typename InputImageListType::Pointer       InputImageListPointerType;
  typedef typename InputImageListType::ConstIterator InputImageListConstIteratorType;
  typedef typename InputImageListType::ImageType     InputImageType;
  typedef typename InputImageType::Pointer           InputImagePointerType;
  typedef typename InputImageType::RegionType        InputImageRegionType;
  typedef typename InputImageType::PixelType         InputImagePixelType;
  typedef typename InputImageType::SizeType          SizeType;
  typedef typename InputImageType::ValueType         ValueType;

  /** OutputSampleList typedefs */
  typedef TOutputSampleList                                    OutputSampleListType;
  typedef typename OutputSampleListType::Pointer               OutputSampleListPointer;
  typedef typename OutputSampleListType::ConstPointer          OutputSampleListConstPointer;
  typedef typename OutputSampleListType::MeasurementVectorType OutputMeasurementVectorType;
  typedef typename OutputMeasurementVectorType::ValueType      OutputValueType;

  /** Functor typedef */
  typedef TFunction FunctorType;

  typedef itk::ImageRegionConstIterator< InputImageType > ImageConstIteratorType;
  
  /** ListSample is not a DataObject, we need to decorate it to push it down
   * a ProcessObject's pipeline */
  typedef itk::DataObject::Pointer                             DataObjectPointer;

  typedef itk::DataObjectDecorator< OutputSampleListType >     OutputSampleListObjectType;

  /** Returns the output sample list */
  OutputSampleListType * GetOutputSampleList();

  /** Returns the output sample list as a data object */
  OutputSampleListObjectType * GetOutput();

  /** Get the functor object.  The functor is returned by reference.
   * (Functors do not have to derive from itk::LightObject, so they do
   * not necessarily have a reference count. So we cannot return a
   * SmartPointer.) */
  FunctorType& GetFunctor()
  {
    return m_Functor;
  }

  /** Set the functor object.  This replaces the current Functor with a
   * copy of the specified Functor. This allows the user to specify a
   * functor that has ivars set differently than the default functor.
   * This method requires an operator!=() be defined on the functor
   * (or the compiler's default implementation of operator!=() being
   * appropriate). */
  void SetFunctor(const FunctorType& functor)
  {
    m_Functor = functor;
    this->Modified();
  }

  /** Set/Get the input imageList */
  void SetInput1 ( const InputImageListType * );
  const InputImageListType * GetInput1() const;

  void SetInput2 ( const InputImageListType * );
  const InputImageListType * GetInput2() const;

protected:
  BinaryFunctorImageListToSampleListFilter();
  virtual ~BinaryFunctorImageListToSampleListFilter() { }

  /** Standard itk::ProcessObject subclass method. */
  virtual DataObjectPointer MakeOutput(unsigned int idx);
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** This method causes the filter to generate its output. */
  virtual void GenerateData();

private:
  BinaryFunctorImageListToSampleListFilter(const Self &); // not implemented
  void operator=(const Self &); // not implemeted

  FunctorType m_Functor;
}; // end of class 

} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbBinaryFunctorImageListToSampleListFilter.h"
#endif

#endif 

