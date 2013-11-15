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
#ifndef __otbSparseWvltToAngleMapperListFilter_h
#define __otbSparseWvltToAngleMapperListFilter_h

#include "otbMath.h"
#include "otbImageList.h"

#include "itkProcessObject.h"
#include "itkDataObject.h"
#include "itkDataObjectDecorator.h"
#include "itkNumericTraits.h"
#include "itkImageRegionConstIterator.h"

namespace otb {


/** \class SparseWvltToAngleMapperListFilter
 * \brief This class select N-uple join-wvlt coeff for sparse unmixing
 *
 * This class performs a kind of scatter plot of wavelet coeff of N images and yields
 * the angle values (through a generalized spherical coordinate representation)) of
 * the most significant coeff (up to a threashold).
 *
 * \ingroup Threaded
 */
template < class TInputImageList, class TOutputSampleList, unsigned int VNbInputImages >
class SparseWvltToAngleMapperListFilter
  : public ITK_EXPORT itk::ProcessObject
{
public:
  /** Standard typedefs. */
  typedef SparseWvltToAngleMapperListFilter Self;
  typedef itk::ProcessObject                Superclass;
  typedef itk::SmartPointer<Self>           Pointer;
  typedef itk::SmartPointer<const Self>     ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(SparseWvltToAngleMapperListFilter, ProcessObject);

  /** Number of input images */
  itkStaticConstMacro(NumberOfInputImages, unsigned int, VNbInputImages);

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

  /** Iterator */
  typedef itk::ImageRegionConstIterator< InputImageType > ImageConstIteratorType;
  typedef std::vector< ImageConstIteratorType >           ImageConstIteratorVectorType;
  typedef std::vector< InputImageListConstIteratorType >  InputImageListConstIteratorVectorType;

  void SetInput ( unsigned int i, const InputImageListType * );
  const InputImageListType * GetInput( unsigned int i ) const;

  /** ListSample is not a DataObject, we need to decorate it to push it down
   * a ProcessObject's pipeline */
  typedef itk::DataObject::Pointer                             DataObjectPointer;
  typedef itk::DataObjectDecorator< OutputSampleListType >     OutputSampleListObjectType;

  /** Returns the output sample list */
  OutputSampleListType * GetOutputSampleList();

  /** Returns the output sample list as a data object */
  OutputSampleListObjectType * GetOutput();

  /** Get/Set the threshold value */
  itkGetMacro(ThresholdValue, ValueType);
  itkSetMacro(ThresholdValue, ValueType);

protected:
  SparseWvltToAngleMapperListFilter();
  virtual ~SparseWvltToAngleMapperListFilter() { }

  /** Standard itk::ProcessObject subclass method. */
  virtual DataObjectPointer MakeOutput(unsigned int idx);
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** This method causes the filter to generate its output. */
  virtual void GenerateData();

  /* Internal functions */
  virtual bool IsToGenerate ( const ImageConstIteratorVectorType & ) const;
  virtual OutputMeasurementVectorType GenerateData ( const ImageConstIteratorVectorType & ) const;
  virtual OutputMeasurementVectorType FromEuclideanToSphericalSpace ( const ImageConstIteratorVectorType & ) const;

private:
  SparseWvltToAngleMapperListFilter(const Self &); // not implemented
  void operator=(const Self &);

  ValueType m_ThresholdValue;

}; // end of class

} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSparseWvltToAngleMapperListFilter.txx"
#endif

#endif


