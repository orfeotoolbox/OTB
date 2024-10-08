/*
 * Copyright (C) 2005-2022 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbNumberOfDivisionsTiledStreamingManager_h
#define otbNumberOfDivisionsTiledStreamingManager_h

#include "otbStreamingManager.h"

namespace otb
{

/** \class NumberOfDivisionsTiledStreamingManager
 *  \brief This class computes the divisions needed to stream an image by strips,
 *  driven by a user-defined number of divisions in which to divide the images.
 *
 * You can use SetNumberOfDivisions to ask for a specific number of divisions.
 * The number of strips will be computed to fit this requirements as close as possible.
 *
 * \sa ImageFileWriter
 * \sa StreamingImageVirtualFileWriter
 *
 * \ingroup OTBStreaming
 */
template <class TImage>
class ITK_EXPORT NumberOfDivisionsTiledStreamingManager : public StreamingManager<TImage>
{
public:
  /** Standard class typedefs. */
  typedef NumberOfDivisionsTiledStreamingManager Self;
  typedef StreamingManager<TImage>               Superclass;
  typedef itk::SmartPointer<Self>                Pointer;
  typedef itk::SmartPointer<const Self>          ConstPointer;

  typedef TImage                          ImageType;
  typedef typename Superclass::RegionType RegionType;

  /** Creation through object factory macro */
  itkNewMacro(Self);

  /** Type macro */
  itkTypeMacro(NumberOfDivisionsTiledStreamingManager, itk::LightObject);

  /** Dimension of input image. */
  itkStaticConstMacro(ImageDimension, unsigned int, ImageType::ImageDimension);

  /** The number of lines per strip desired */
  itkSetMacro(NumberOfDivisions, unsigned int);

  /** The number of lines per strip desired */
  itkGetMacro(NumberOfDivisions, unsigned int);

  /** Actually computes the stream divisions given a DataObject and its region to write */
  void PrepareStreaming(itk::DataObject* input, const RegionType& region) override;

protected:
  NumberOfDivisionsTiledStreamingManager();
  ~NumberOfDivisionsTiledStreamingManager() override;

  /** The number of lines per strip desired by the user.
   *  This may be different than the one computed by the Splitter */
  unsigned int m_NumberOfDivisions;

private:
  NumberOfDivisionsTiledStreamingManager(const NumberOfDivisionsTiledStreamingManager&);
  void operator=(const NumberOfDivisionsTiledStreamingManager&);
};

} // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbNumberOfDivisionsTiledStreamingManager.hxx"
#endif

#endif
