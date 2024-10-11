/*
 * Copyright (C) 1999-2011 Insight Software Consortium
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbLabelImageToLabelMapWithAdjacencyFilter_h
#define otbLabelImageToLabelMapWithAdjacencyFilter_h

#include "itkImageToImageFilter.h"
#include "otbLabelMapWithAdjacency.h"
#include "itkLabelObject.h"

namespace otb
{

/** \class LabelImageToLabelMapWithAdjacencyFilter
 * \brief convert a labeled image to a label map with adjacency information.
 *
 *
 * \ingroup OTBLabelMap
 */
template <class TInputImage, class TOutputImage = otb::LabelMapWithAdjacency<itk::LabelObject<typename TInputImage::PixelType, TInputImage::ImageDimension>>>
class ITK_EXPORT LabelImageToLabelMapWithAdjacencyFilter : public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef LabelImageToLabelMapWithAdjacencyFilter Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Some convenient typedefs. */
  typedef TInputImage                           InputImageType;
  typedef TOutputImage                          OutputImageType;
  typedef typename InputImageType::Pointer      InputImagePointer;
  typedef typename InputImageType::ConstPointer InputImageConstPointer;
  typedef typename InputImageType::RegionType   InputImageRegionType;
  typedef typename InputImageType::PixelType    InputImagePixelType;
  typedef typename InputImageType::IndexType    IndexType;

  typedef typename OutputImageType::Pointer                     OutputImagePointer;
  typedef typename OutputImageType::ConstPointer                OutputImageConstPointer;
  typedef typename OutputImageType::RegionType                  OutputImageRegionType;
  typedef typename OutputImageType::PixelType                   OutputImagePixelType;
  typedef typename OutputImageType::LabelObjectType             LabelObjectType;
  typedef typename OutputImageType::AdjacencyMapType            AdjacencyMapType;
  typedef typename OutputImageType::AdjacentLabelsContainerType AdjacentLabelsContainerType;
  typedef typename OutputImageType::LabelType                   LabelType;

  /** Const iterator over LabelObject lines */
  typedef typename LabelObjectType::ConstLineIterator ConstLineIteratorType;

  /** ImageDimension constants */
  itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int, TOutputImage::ImageDimension);

  /** Standard New method. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(LabelImageToLabelMapWithAdjacencyFilter, ImageToImageFilter);

  /**
   * Set/Get the value used as "background" in the output image.
   * Defaults to NumericTraits<PixelType>::NonpositiveMin().
   */
  itkSetMacro(BackgroundValue, OutputImagePixelType);
  itkGetConstMacro(BackgroundValue, OutputImagePixelType);

protected:
  /** Constructor */
  LabelImageToLabelMapWithAdjacencyFilter();
  /** Destructor */
  ~LabelImageToLabelMapWithAdjacencyFilter() override{};
  /** Printself */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  // class to store a RLE
  class RLE
  {
  public:
    // Location
    IndexType where;
    // Length
    long unsigned int length;
    // Label
    unsigned long int label;
    // Constructor
    RLE(const IndexType& w, long unsigned int l, unsigned long int la)
    {
      where  = w;
      length = l;
      label  = la;
    }

  }; // end class RLE

  // RLE Vector
  typedef std::vector<RLE> RLEVectorType;

  /** LabelImageToLabelMapWithAdjacencyFilter needs the entire input be
   * available. Thus, it needs to provide an implementation of
   * GenerateInputRequestedRegion(). */
  void GenerateInputRequestedRegion() override;

  /** LabelImageToLabelMapWithAdjacencyFilter will produce the entire output. */
  void EnlargeOutputRequestedRegion(itk::DataObject* itkNotUsed(output)) override;

  void BeforeThreadedGenerateData() override;

  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId) override;

  void AfterThreadedGenerateData() override;

  /** Add a new adjacency */
  void AddAdjacency(LabelType label1, LabelType label2, itk::ThreadIdType threadId);

  /** Parse one line for horizontal adjacency */
  void ParseLine(const RLEVectorType& line, itk::ThreadIdType threadId);

  /** Parse two consecutive lines for vertical adjacency */
  void ParseConsecutiveLines(const RLEVectorType& line1, const RLEVectorType& line2, itk::ThreadIdType threadId);


private:
  LabelImageToLabelMapWithAdjacencyFilter(const Self&) = delete;
  void operator=(const Self&) = delete;

  OutputImagePixelType m_BackgroundValue;

  typename std::vector<OutputImagePointer> m_TemporaryImages;
  typename std::vector<AdjacencyMapType>   m_TemporaryAdjacencyMaps;

}; // end of class

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLabelImageToLabelMapWithAdjacencyFilter.hxx"
#endif

#endif
