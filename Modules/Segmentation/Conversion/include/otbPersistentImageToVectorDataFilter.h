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

#ifndef otbPersistentImageToVectorDataFilter_h
#define otbPersistentImageToVectorDataFilter_h

#include "otbPersistentImageFilter.h"
#include "itkExtractImageFilter.h"

#include "otbConcatenateVectorDataFilter.h"
#include "otbOGRVectorDataIO.h"
#include "itkMacro.h"
#include <string>

namespace otb
{

/** \class PersistentImageToVectorDataFilter
 *  \brief Perform vectorization in a persistent way.
 *
 *  This filter is a generic PersistentImageFilter, which encapsulate any filter
 *  which produces VectorData from an input Image.
 *
 *  It provides a default implementation which merge
 *  the VectorData from the different tiles/strips used during streaming into
 *  a single VectorData, which can be accessed via GetVectorData()
 *
 * \sa PersistentImageFilter
 *
 *
 * \ingroup OTBConversion
 */
template <class TImage, class TOutputVectorData>
class ITK_EXPORT PersistentImageToVectorDataFilter : public PersistentImageFilter<TImage, TImage>
{
public:
  /** Standard Self typedef */
  typedef PersistentImageToVectorDataFilter Self;
  typedef PersistentImageFilter<TImage, TImage> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Runtime information support. */
  itkTypeMacro(PersistentImageToVectorDataFilter, PersistentImageFilter);

  typedef TImage                                     InputImageType;
  typedef typename InputImageType::Pointer           InputImagePointer;
  typedef typename InputImageType::RegionType        RegionType;
  typedef typename InputImageType::SizeType          SizeType;
  typedef typename InputImageType::IndexType         IndexType;
  typedef typename InputImageType::PixelType         PixelType;
  typedef typename InputImageType::InternalPixelType InternalPixelType;

  typedef TOutputVectorData                   OutputVectorDataType;
  typedef typename TOutputVectorData::Pointer OutputVectorDataPointerType;

  typedef itk::ExtractImageFilter<InputImageType, InputImageType> ExtractImageFilterType;
  typedef typename ExtractImageFilterType::Pointer ExtractImageFilterPointerType;

  typedef otb::ConcatenateVectorDataFilter<OutputVectorDataType> ConcatenateVectorDataFilterType;
  typedef typename ConcatenateVectorDataFilterType::Pointer      ConcatenateVectorDataFilterPointerType;

  typedef otb::OGRVectorDataIO                  OGRVectorDataIOType;
  typedef typename OGRVectorDataIOType::Pointer OGRVectorDataIOPointerType;

  /** Smart Pointer type to a DataObject. */
  typedef itk::DataObject::Pointer DataObjectPointer;

  OutputVectorDataType* GetOutputVectorData() const;

  void AllocateOutputs() override;

  void Reset(void) override;

  void Synthetize(void) override;

  /** Specify the name of the output shapefile to write. */
  itkSetStringMacro(FileName);
  itkGetStringMacro(FileName);

protected:
  PersistentImageToVectorDataFilter();
  ~PersistentImageToVectorDataFilter() override
  {
  }

  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  void GenerateData() override;

  ExtractImageFilterPointerType m_ExtractFilter;

  OutputVectorDataPointerType m_OutputVectorData;

private:
  PersistentImageToVectorDataFilter(const Self&) = delete;
  void operator=(const Self&) = delete;

  virtual OutputVectorDataPointerType ProcessTile() = 0;

  OGRVectorDataIOPointerType m_VectorDataIO;
  std::string                m_FileName;

}; // end of class
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbPersistentImageToVectorDataFilter.hxx"
#endif

#endif
