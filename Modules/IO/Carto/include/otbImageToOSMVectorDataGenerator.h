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

#ifndef otbImageToOSMVectorDataGenerator_h
#define otbImageToOSMVectorDataGenerator_h

#include "otbOSMDataToVectorDataGenerator.h"

namespace otb
{

/** \class ImageToOSMVectorDataGenerator
 *
 *  \brief Helper class to compute the extent of the input image and
 *   use it to fill the bbox for the OSM request.
 *
 *  This class compute the extent of the input image. The extent is
 *  then used to fill the bbox values.
 *  The class OSMDataToVectorDataGenerator request the OSM server to get
 *  the OSM XML file storing the datas within this input image bbox.
 *
 * \sa otb::OSMDataToVectorDataGenerator
 *
 * \ingroup OTBCarto
 */
template <class TImage>
class ImageToOSMVectorDataGenerator : public OSMDataToVectorDataGenerator
{
public:
  /** Standard class typedefs */
  typedef ImageToOSMVectorDataGenerator Self;
  typedef OSMDataToVectorDataGenerator  Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(ImageToOSMVectorDataGenerator, OSMDataToVectorDataGenerator);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  // Image typedef
  typedef TImage                        ImageType;
  typedef typename ImageType::SizeType  SizeType;
  typedef typename ImageType::IndexType IndexType;
  typedef typename ImageType::PointType PointType;

  // VectorData
  typedef Superclass::VectorDataType VectorDataType;

  // Struct to store the extent of the image
  struct ImageExtentType
  {
    double minX;
    double maxX;
    double minY;
    double maxY;
  };

  /** Method to set/get the input image */
  using Superclass::SetInput;
  void SetInput(const ImageType* input);

  /** Returns the input image */
  const ImageType* GetInput() const;

protected:
  void GenerateData() override;

  // Method to compute the extent of the image
  void EstimateImageExtent();

  ImageToOSMVectorDataGenerator();
  ~ImageToOSMVectorDataGenerator() override
  {
  }

private:
  ImageToOSMVectorDataGenerator(const Self&) = delete;
  void operator=(const Self&) = delete;

  ImageExtentType m_ImageExtent;

}; // end of class

} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageToOSMVectorDataGenerator.hxx"
#endif

#endif
