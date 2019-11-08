/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbSparseWvltToAngleMapperListFilter_h
#define otbSparseWvltToAngleMapperListFilter_h

#include "otbMath.h"
#include "otbImageList.h"

#include "itkProcessObject.h"
#include "itkDataObjectDecorator.h"
#include "itkImageRegionConstIterator.h"

namespace otb
{


/** \class SparseWvltToAngleMapperListFilter
 * \brief This class select N-uple join-wvlt coeff for sparse unmixing
 *
 * This class performs a kind of scatter plot of wavelet coeff of N images and yields
 * the angle values (through a generalized spherical coordinate representation)) of
 * the most significant coeff (up to a threshold).
 *
 * \ingroup Threaded
 *
 * \ingroup OTBDimensionalityReduction
 */
template <class TInputImageList, class TOutputSampleList, unsigned int VNbInputImages>
class SparseWvltToAngleMapperListFilter : public ITK_EXPORT itk::ProcessObject
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
  typedef TInputImageList                            InputImageListType;
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
  typedef itk::ImageRegionConstIterator<InputImageType> ImageConstIteratorType;
  typedef std::vector<ImageConstIteratorType>           ImageConstIteratorVectorType;
  typedef std::vector<InputImageListConstIteratorType>  InputImageListConstIteratorVectorType;

  void SetInput(unsigned int i, const InputImageListType*);
  using Superclass::SetInput;

  const InputImageListType* GetInput(unsigned int i) const;

  /** ListSample is not a DataObject, we need to decorate it to push it down
   * a ProcessObject's pipeline */
  typedef itk::DataObject::Pointer                           DataObjectPointer;
  typedef itk::DataObjectDecorator<OutputSampleListType>     OutputSampleListObjectType;
  typedef itk::ProcessObject::DataObjectPointerArraySizeType DataObjectPointerArraySizeType;

  /** Returns the output sample list */
  OutputSampleListType* GetOutputSampleList();

  /** Returns the output sample list as a data object */
  OutputSampleListObjectType* GetOutput();

  /** Get/Set the threshold value */
  itkGetMacro(ThresholdValue, ValueType);
  itkSetMacro(ThresholdValue, ValueType);

protected:
  SparseWvltToAngleMapperListFilter();
  ~SparseWvltToAngleMapperListFilter() override
  {
  }

  /** Standard itk::ProcessObject subclass method. */
  DataObjectPointer MakeOutput(DataObjectPointerArraySizeType idx) override;
  using Superclass::MakeOutput;

  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  /** This method causes the filter to generate its output. */
  void GenerateData() override;

  /* Internal functions */
  virtual bool                        IsToGenerate(const ImageConstIteratorVectorType&) const;
  virtual OutputMeasurementVectorType GenerateData(const ImageConstIteratorVectorType&) const;
  virtual OutputMeasurementVectorType FromEuclideanToSphericalSpace(const ImageConstIteratorVectorType&) const;

private:
  SparseWvltToAngleMapperListFilter(const Self&); // not implemented
  void operator=(const Self&);

  ValueType m_ThresholdValue;

}; // end of class

} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSparseWvltToAngleMapperListFilter.hxx"
#endif

#endif
