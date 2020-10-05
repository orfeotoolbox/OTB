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

#ifndef otbRAMDrivenAdaptativeStreamingManager_h
#define otbRAMDrivenAdaptativeStreamingManager_h

#include "otbStreamingManager.h"

namespace otb
{

/** \class RAMDrivenAdaptativeStreamingManager
 *  \brief This class computes the divisions needed to stream an image
 *  according to the input image tiling scheme and a user-defined
 *  available RAM.
 *
 * This streaming manager uses the TileHint from the
 * MetaDataDictionary to find out the tiling sheme of the input file
 * if available.
 *
 * You can use SetAvailableRAMInMB to set the available RAM. An
 * estimation of the pipeline memory print will be done, and the
 * number of divisions will then be computed to fit the available RAM.
 *
 * \sa ImageRegionAdaptativeSplitter
 * \sa ImageFileWriter
 * \sa StreamingImageVirtualFileWriter
 *
 * \ingroup OTBStreaming
 */
template <class TImage>
class ITK_EXPORT RAMDrivenAdaptativeStreamingManager : public StreamingManager<TImage>
{
public:
  /** Standard class typedefs. */
  typedef RAMDrivenAdaptativeStreamingManager Self;
  typedef StreamingManager<TImage>            Superclass;
  typedef itk::SmartPointer<Self>             Pointer;
  typedef itk::SmartPointer<const Self>       ConstPointer;

  typedef TImage                          ImageType;
  typedef typename Superclass::RegionType RegionType;

  /** Creation through object factory macro */
  itkNewMacro(Self);

  /** Type macro */
  itkTypeMacro(RAMDrivenAdaptativeStreamingManager, itk::LightObject);

  /** Dimension of input image. */
  itkStaticConstMacro(ImageDimension, unsigned int, ImageType::ImageDimension);

  /** The number of Megabytes available (if 0, the configuration option is
    used)*/
  itkSetMacro(AvailableRAMInMB, unsigned int);

  /** The number of Megabytes available (if 0, the configuration option is
    used)*/
  itkGetConstMacro(AvailableRAMInMB, unsigned int);

  /** The multiplier to apply to the memory print estimation */
  itkSetMacro(Bias, double);

  /** The multiplier to apply to the memory print estimation */
  itkGetConstMacro(Bias, double);

  /** Actually computes the stream divisions, according to the specified streaming mode,
   * eventually using the input parameter to estimate memory consumption */
  void PrepareStreaming(itk::DataObject* input, const RegionType& region) override;

protected:
  RAMDrivenAdaptativeStreamingManager();
  ~RAMDrivenAdaptativeStreamingManager() override;

  /** The number of MegaBytes of RAM available */
  unsigned int m_AvailableRAMInMB;

  /** The multiplier to apply to the memory print estimation */
  double m_Bias;

private:
  RAMDrivenAdaptativeStreamingManager(const RAMDrivenAdaptativeStreamingManager&);
  void operator=(const RAMDrivenAdaptativeStreamingManager&);
};

} // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbRAMDrivenAdaptativeStreamingManager.hxx"
#endif

#endif
