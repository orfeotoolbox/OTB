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

#ifndef otbImageFileWriterBase_h
#define otbImageFileWriterBase_h

#include "otbImageIOBase.h"
#include "itkProcessObject.h"
#include "otbStreamingManager.h"
#include "otbExtendedFilenameToWriterOptions.h"
#include "itkFastMutexLock.h"
#include <string>
#include "OTBImageIOExport.h"

namespace otb
{

/** \class ImageFileWriterBase
 * \brief Writes image data to a single file with streaming process.
 *
 * ImageFileWriter writes its input data to a single output file.
 * ImageFileWriter interfaces with an ImageIO class to write out the
 * data with streaming process.
 *
 * ImageFileWriter will divide the output into several pieces
 * (controlled by SetNumberOfDivisionsStrippedStreaming, SetNumberOfLinesStrippedStreaming,
 * SetAutomaticStrippedStreaming, SetTileDimensionTiledStreaming or SetAutomaticTiledStreaming),
 * and call the upstream pipeline for each piece, tiling the individual outputs into one large
 * output. This reduces the memory footprint for the application since
 * each filter does not have to process the entire dataset at once.
 *
 * ImageFileWriter will write directly the streaming buffer in the image file, so
 * that the output image never needs to be completely allocated
 *
 * ImageFileWriter supports extended filenames, which allow controlling
 * some properties of the output file. See
 * http://wiki.orfeo-toolbox.org/index.php/ExtendedFileName for more
 * information.
 *
 * \sa ImageFileReader
 * \sa ImageSeriesReader
 * \sa ImageIOBase
 * \sa ExtendedFilenameToReaderOptions
 *
 * \ingroup OTBImageIO
 */
class OTBImageIO_EXPORT_TEMPLATE ImageFileWriterBase : public itk::ProcessObject
{
public:
  /** Standard class typedefs. */
  typedef ImageFileWriterBase               Self;
  typedef itk::ProcessObject                     Superclass;
  typedef itk::SmartPointer<Self>                Pointer;
  typedef itk::SmartPointer<const Self>          ConstPointer;

  typedef itk::ImageBase<2>             ImageBaseType;
  
  itkTypeMacro(otb::ImageFileWriterBase, itk::ProcessObject);

  void SetIORegion(const itk::ImageIORegion& region)
  {
      
    if (m_IORegion != region)
    {
      m_IORegion = region;
      this->Modified();
      m_UserSpecifiedIORegion = true;
    }
    
  }
  
  virtual bool CanStreamWrite() const = 0;
  
  itkGetConstReferenceMacro(IORegion, itk::ImageIORegion);

  virtual const ImageBaseType* GetImageBaseInput() = 0;

protected:
  ImageFileWriterBase() {};
  ~ImageFileWriterBase() override {};
  void PrintSelf(std::ostream& os, itk::Indent indent) const override {   Superclass::PrintSelf(os, indent);
    
};

  //TODO private
  itk::ImageIORegion m_IORegion;
  bool               m_UserSpecifiedIORegion;   // track whether the region is user specified

private:
  ImageFileWriterBase(const ImageFileWriterBase&) = delete;
  void operator=(const ImageFileWriterBase&) = delete;
  
};

} // end namespace otb


#include "otbImage.h"
#include "otbVectorImage.h"
#include <complex>

#endif
