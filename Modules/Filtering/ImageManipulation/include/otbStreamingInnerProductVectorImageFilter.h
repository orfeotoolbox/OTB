/*
 * Copyright (C) 1999-2011 Insight Software Consortium
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

#ifndef otbStreamingInnerProductVectorImageFilter_h
#define otbStreamingInnerProductVectorImageFilter_h

#include "otbPersistentImageFilter.h"
#include "otbPersistentFilterStreamingDecorator.h"
#include "itkSimpleDataObjectDecorator.h"
#include "itkImageRegionSplitter.h"
#include "itkVariableSizeMatrix.h"
#include "itkVariableLengthVector.h"
#include "vnl/vnl_matrix.h"

namespace otb
{

/** \class PersistentInnerProductVectorImageFilter
 * \brief Compute the inner product of a large image using streaming
 *
 *  This filter persists its temporary data. It means that if you Update it n times on n different
 * requested regions, the output statistics will be the statitics of the whole set of n regions.
 *
 * To reset the temporary data, one should call the Reset() function.
 *
 * To get the statistics once the regions have been processed via the pipeline, use the Synthetize() method.
 *
 * \sa PersistentImageFilter
 * \ingroup Streamed
 * \ingroup Multithreaded
 * \ingroup MathematicalStatisticsImageFilters
 *
 *
 * \ingroup OTBImageManipulation
 */
template<class TInputImage>
class ITK_EXPORT PersistentInnerProductVectorImageFilter :
  public PersistentImageFilter<TInputImage, TInputImage>
{
public:
  /** Standard Self typedef */
  typedef PersistentInnerProductVectorImageFilter         Self;
  typedef PersistentImageFilter<TInputImage, TInputImage> Superclass;
  typedef itk::SmartPointer<Self>                         Pointer;
  typedef itk::SmartPointer<const Self>                   ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(PersistentInnerProductVectorImageFilter, PersistentImageFilter);

  /** Image related typedefs. */
  typedef TInputImage                      ImageType;
  typedef typename TInputImage::Pointer    InputImagePointer;
  typedef typename TInputImage::RegionType RegionType;
  typedef typename TInputImage::PixelType  PixelType;

  itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);

  /** Image related typedefs. */
  itkStaticConstMacro(ImageDimension, unsigned int, TInputImage::ImageDimension);

  /** Smart Pointer type to a DataObject. */
  typedef typename itk::DataObject::Pointer DataObjectPointer;
  typedef itk::ProcessObject::DataObjectPointerArraySizeType DataObjectPointerArraySizeType;

  /** Type definition for a double matrix. */
  typedef vnl_matrix<double>               MatrixType;
  typedef typename std::vector<MatrixType> ArrayMatrixType;

  /** Type of DataObjects used for scalar outputs */
  typedef itk::SimpleDataObjectDecorator<MatrixType> MatrixObjectType;

  /** Return the computed inner product matrix. */

  MatrixType GetInnerProduct() const
  {
    return this->GetInnerProductOutput()->Get();
  }
  MatrixObjectType* GetInnerProductOutput();
  const MatrixObjectType* GetInnerProductOutput() const;

  /** Make a DataObject of the correct type to be used as the specified
   * output.
   */
  DataObjectPointer MakeOutput(DataObjectPointerArraySizeType idx) override;
  using Superclass::MakeOutput;

  /** Pass the input through unmodified. Do this by Grafting in the
   *  AllocateOutputs method.
   */
  void AllocateOutputs() override;
  void GenerateOutputInformation() override;
  void Synthetize(void) override;
  void Reset(void) override;

  /** Enable/Disable center data */
  itkSetMacro(CenterData, bool);
  itkGetMacro(CenterData, bool);
  itkBooleanMacro(CenterData);

protected:
  PersistentInnerProductVectorImageFilter();
  ~PersistentInnerProductVectorImageFilter() override {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;
  /** Multi-thread version GenerateData. */
  void  ThreadedGenerateData(const RegionType& outputRegionForThread, itk::ThreadIdType threadId) override;

private:
  PersistentInnerProductVectorImageFilter(const Self &) = delete;
  void operator =(const Self&) = delete;

  ArrayMatrixType m_ThreadInnerProduct;

  /** Enable/Disable center data */
  bool m_CenterData;

}; // end of class PersistentStatisticsVectorImageFilter

/**===========================================================================*/

/** \class StreamingInnerProductVectorImageFilter
 * \brief This class streams the whole input image through the PersistentStatisticsImageFilter.
 *
 * This way, it allows computing the inner product of this image. It calls the
 * Reset() method of the PersistentStatisticsImageFilter before streaming the image and the
 * Synthetize() method of the PersistentStatisticsImageFilter after having streamed the image
 * to compute the statistics. The accessor on the results are wrapping the accessors of the
 * internal PersistentStatisticsImageFilter.
 *
 * \sa PersistentStatisticsVectorImageFilter
 * \sa PersistentImageFilter
 * \sa PersistentFilterStreamingDecorator
 * \sa StreamingImageVirtualWriter
 * \ingroup Streamed
 * \ingroup Multithreaded
 * \ingroup MathematicalStatisticsImageFilters
 *
 * \ingroup OTBImageManipulation
 */

template<class TInputImage>
class ITK_EXPORT StreamingInnerProductVectorImageFilter :
  public PersistentFilterStreamingDecorator<PersistentInnerProductVectorImageFilter<TInputImage> >
{
public:
  /** Standard Self typedef */
  typedef StreamingInnerProductVectorImageFilter Self;
  typedef PersistentFilterStreamingDecorator
  <PersistentInnerProductVectorImageFilter<TInputImage> > Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(StreamingInnerProductVectorImageFilter, PersistentFilterStreamingDecorator);

  typedef TInputImage                         InputImageType;
  typedef typename Superclass::FilterType     StatFilterType;
  typedef typename StatFilterType::MatrixType MatrixType;

  /** Type of DataObjects used for scalar outputs */
  typedef typename StatFilterType::MatrixObjectType MatrixObjectType;

  using Superclass::SetInput;
  void SetInput(TInputImage * input)
  {
    this->GetFilter()->SetInput(input);
  }
  TInputImage * GetInput()
  {
    return this->GetFilter()->GetInput();
  }

  /** Return the computed inner product. */
  MatrixType GetInnerProduct() const
  {
    return this->GetFilter()->GetInnerProductOutput()->Get();
  }
  MatrixObjectType* GetInnerProductOutput()
  {
    return this->GetFilter()->GetInnerProductOutput();
  }
  const MatrixObjectType* GetInnerProductOutput() const
  {
    return this->GetFilter()->GetInnerProductOutput();
  }

  /** Enable/Disable center data */
  void SetCenterData(bool centerdata)
  {
    this->GetFilter()->SetCenterData(centerdata);
  }

protected:
  /** Constructor */
  StreamingInnerProductVectorImageFilter() {};
  /** Destructor */
  ~StreamingInnerProductVectorImageFilter() override {}

private:
  StreamingInnerProductVectorImageFilter(const Self &) = delete;
  void operator =(const Self&) = delete;

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbStreamingInnerProductVectorImageFilter.hxx"
#endif

#endif
