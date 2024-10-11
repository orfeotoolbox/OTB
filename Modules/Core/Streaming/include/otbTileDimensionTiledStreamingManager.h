/*
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

#ifndef otbTileDimensionTiledStreamingManager_h
#define otbTileDimensionTiledStreamingManager_h

#include "otbStreamingManager.h"

namespace otb
{

/** \class TileDimensionTiledStreamingManager
 *  \brief This class computes the divisions needed to stream an image in square tiles,
 *  driven by a user-defined tile dimension
 *
 * You can use SetTileDimension to ask for a specific tile size (number of pixels
 * for each dimension of the tile)
 *
 * The number of tiles will be computed to fit this requirement as close as possible.
 *
 * The tile dimension is always aligned on a multiple of 16 (as in TIFF spec)
 *
 * \sa ImageFileWriter
 * \sa StreamingImageVirtualFileWriter
 *
 * \ingroup OTBStreaming
 */
template <class TImage>
class ITK_EXPORT TileDimensionTiledStreamingManager : public StreamingManager<TImage>
{
public:
  /** Standard class typedefs. */
  typedef TileDimensionTiledStreamingManager Self;
  typedef StreamingManager<TImage>           Superclass;
  typedef itk::SmartPointer<Self>            Pointer;
  typedef itk::SmartPointer<const Self>      ConstPointer;

  typedef TImage                          ImageType;
  typedef typename Superclass::RegionType RegionType;

  /** Creation through object factory macro */
  itkNewMacro(Self);

  /** Type macro */
  itkTypeMacro(TileDimensionTiledStreamingManager, StreamingManager);

  /** Dimension of input image. */
  itkStaticConstMacro(ImageDimension, unsigned int, ImageType::ImageDimension);

  /** The desired tile dimension */
  itkSetMacro(TileDimension, unsigned int);

  /** The desired tile dimension */
  itkGetMacro(TileDimension, unsigned int);

  /** Actually computes the stream divisions, according to the specified streaming mode,
   * eventually using the input parameter to estimate memory consumption */
  void PrepareStreaming(itk::DataObject* input, const RegionType& region) override;

protected:
  TileDimensionTiledStreamingManager();
  ~TileDimensionTiledStreamingManager() override;

  /** The number of lines per strip desired by the user.
   *  This may be different than the one computed by the Splitter */
  unsigned int m_TileDimension;

private:
  TileDimensionTiledStreamingManager(const TileDimensionTiledStreamingManager&);
  void operator=(const TileDimensionTiledStreamingManager&);
};

} // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbTileDimensionTiledStreamingManager.hxx"
#endif

#endif
