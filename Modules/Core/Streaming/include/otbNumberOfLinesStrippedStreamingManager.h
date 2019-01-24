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

#ifndef otbNumberOfLinesStrippedStreamingManager_h
#define otbNumberOfLinesStrippedStreamingManager_h

#include "otbStreamingManager.h"
#include "itkImageRegionSplitter.h"

namespace otb
{

/** \class NumberOfLinesStrippedStreamingManager
 *  \brief This class computes the divisions needed to stream an image by strips,
 *  driven by a user-defined desired number of lines per strips.
 *
 * You can use SetNumberOfLinesPerStrip to ask for a specific number of lines per strip.
 * The number of strips will be computed to fit this requirements as close as possible.
 *
 * \sa ImageFileWriter
 * \sa StreamingImageVirtualFileWriter
 *
 * \ingroup OTBStreaming
 */
template<class TImage>
class ITK_EXPORT NumberOfLinesStrippedStreamingManager : public StreamingManager<TImage>
{
public:
  /** Standard class typedefs. */
  typedef NumberOfLinesStrippedStreamingManager Self;
  typedef StreamingManager<TImage>              Superclass;
  typedef itk::SmartPointer<Self>               Pointer;
  typedef itk::SmartPointer<const Self>         ConstPointer;

  typedef TImage                          ImageType;
  typedef typename Superclass::RegionType RegionType;

  /** Creation through object factory macro */
  itkNewMacro(Self);

  /** Type macro */
  itkTypeMacro(NumberOfLinesStrippedStreamingManager, itk::LightObject);

  /** Dimension of input image. */
  itkStaticConstMacro(ImageDimension, unsigned int, ImageType::ImageDimension);

  /** The number of lines per strip desired */
  itkSetMacro(NumberOfLinesPerStrip, unsigned int);

  /** The number of lines per strip desired */
  itkGetMacro(NumberOfLinesPerStrip, unsigned int);

  /** Actually computes the stream divisions, according to the specified streaming mode,
   * eventually using the input parameter to estimate memory consumption */
  void PrepareStreaming(itk::DataObject * /*input*/, const RegionType &region) override;

protected:
  NumberOfLinesStrippedStreamingManager();
  ~NumberOfLinesStrippedStreamingManager() override;

  /** The splitter type used to generate the different strips */
  typedef itk::ImageRegionSplitter<itkGetStaticConstMacro(ImageDimension)> SplitterType;

  /** The number of lines per strip desired by the user.
   *  This may be different than the one computed by the Splitter */
  unsigned int m_NumberOfLinesPerStrip;
private:
  NumberOfLinesStrippedStreamingManager(const NumberOfLinesStrippedStreamingManager &);
  void operator =(const NumberOfLinesStrippedStreamingManager&);
};

} // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbNumberOfLinesStrippedStreamingManager.hxx"
#endif

#endif

