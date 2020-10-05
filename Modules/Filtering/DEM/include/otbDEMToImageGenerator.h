/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbDEMToImageGenerator_h
#define otbDEMToImageGenerator_h

#include <stdio.h>
#include <string>

#include "itkImageSource.h"
#include "otbImage.h"
#include "otbDEMHandler.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "otbGenericRSTransform.h"

namespace otb
{
/** \class DEMToImageGenerator
 *
 * \brief Class to generate an image from DEM data
 *
 * This class is based on DEMHandler. It takes in input the upper left longitude
 * and latitude, the spacing and the output image size.
 * Handle DTED and SRTM formats.
 *
 * \ingroup Images
 *
 * \example IO/DEMToImageGenerator.cxx
 *
 *
 * \ingroup OTBDEM
 */
template <class TDEMImage>
class ITK_EXPORT DEMToImageGenerator : public itk::ImageSource<TDEMImage>
{
public:
  /** Standard class typedefs. */
  typedef TDEMImage                        DEMImageType;
  typedef typename DEMImageType::Pointer   DEMImagePointerType;
  typedef typename DEMImageType::PixelType PixelType;

  typedef DEMToImageGenerator            Self;
  typedef itk::ImageSource<DEMImageType> Superclass;
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;
  typedef DEMImageType                   OutputImageType;

  typedef typename Superclass::Pointer                    OutputImagePointer;
  typedef typename OutputImageType::SpacingType           SpacingType;
  typedef typename OutputImageType::SizeType              SizeType;
  typedef typename OutputImageType::PointType             PointType;
  typedef typename OutputImageType::IndexType             IndexType;
  typedef typename Superclass::OutputImageRegionType      OutputImageRegionType;
  typedef itk::ImageRegionIteratorWithIndex<DEMImageType> ImageIteratorType;

  typedef otb::DEMHandler DEMHandlerType;

  /** Specialisation of OptResampleFilter with a remote
    * sensing  transform
    */
  typedef GenericRSTransform<>                     GenericRSTransformType;
  typedef typename GenericRSTransformType::Pointer GenericRSTransformPointerType;


  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(DEMToImageGenerator, ImageSource);

  /** Set/Get the Output Origin coordinates. */
  itkSetMacro(OutputOrigin, PointType);
  itkGetConstReferenceMacro(OutputOrigin, PointType);

  /** Set/Get the Output Size. */
  itkSetMacro(OutputSize, SizeType);
  itkGetConstReferenceMacro(OutputSize, SizeType);

  /** Set/Get the Output Spacing. */
  itkSetMacro(OutputSpacing, SpacingType);
  itkGetConstReferenceMacro(OutputSpacing, SpacingType);

  /** Set/Get the Default Unknown Value. */
  itkSetObjectMacro(Transform, GenericRSTransformType);
  itkGetConstObjectMacro(Transform, GenericRSTransformType);

  /** Set/Get the above ellipsoid flag. If false, height is given
  above MSL */
  itkSetMacro(AboveEllipsoid, bool);
  itkGetMacro(AboveEllipsoid, bool);
  itkBooleanMacro(AboveEllipsoid);

  void InstantiateTransform();

  /**
   * Set/Get input & output projections.
   * Set/Get input & output keywordlist
   * The macro are not used here cause the input and the output are
   * inversed.
   */
  void SetInputProjectionRef(const std::string& ref)
  {
    m_Transform->SetOutputProjectionRef(ref);
    this->Modified();
  }

  std::string GetInputProjectionRef() const
  {
    return m_Transform->GetOutputProjectionRef();
  }

  void SetOutputProjectionRef(const std::string& ref)
  {
    m_Transform->SetInputProjectionRef(ref);
    this->Modified();
  }

  std::string GetOutputProjectionRef() const
  {
    return m_Transform->GetInputProjectionRef();
  }

  /** Set/Get Input Keywordlist*/
  void SetInputKeywordList(const ImageKeywordlist& kwl)
  {
    m_Transform->SetOutputKeywordList(kwl);
    this->Modified();
  }
  const ImageKeywordlist GetInputKeywordList()
  {
    return m_Transform->GetOutputKeywordList();
  }

  /** Set/Get output Keywordlist*/
  void SetOutputKeywordList(const ImageKeywordlist& kwl)
  {
    m_Transform->SetInputKeywordList(kwl);
    this->Modified();
  }

  const ImageKeywordlist GetOutputKeywordList()
  {
    return m_Transform->GetInputKeywordList();
  }

  /** Useful to set the output parameters from an existing image*/
  template <class TImageType>
  void SetOutputParametersFromImage(const TImageType* image)
  {
    this->SetOutputOrigin(image->GetOrigin());
    this->SetOutputSpacing(image->GetSignedSpacing());
    // this->SetOutputStartIndex ( image->GetLargestPossibleRegion().GetIndex() );
    this->SetOutputSize(image->GetLargestPossibleRegion().GetSize());
    this->SetOutputProjectionRef(image->GetProjectionRef());
    this->SetOutputKeywordList(image->GetImageKeywordlist());

    InstantiateTransform();
  }

protected:
  DEMToImageGenerator();
  ~DEMToImageGenerator() override
  {
  }

  void PrintSelf(std::ostream& os, itk::Indent indent) const override;
  void BeforeThreadedGenerateData() override;
  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId) override;
  void GenerateOutputInformation() override;

  DEMHandlerType::Pointer m_DEMHandler;
  PointType               m_OutputOrigin{0.0};
  SpacingType             m_OutputSpacing{0.0};
  SizeType                m_OutputSize{0,0};
  PixelType               m_DefaultUnknownValue;
  bool                    m_AboveEllipsoid;

private:
  DEMToImageGenerator(const Self&) = delete;
  void operator=(const Self&) = delete;

  GenericRSTransformPointerType m_Transform;
};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbDEMToImageGenerator.hxx"
#endif

#endif
