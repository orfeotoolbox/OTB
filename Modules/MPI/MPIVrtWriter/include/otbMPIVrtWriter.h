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

#ifndef otbMPIVrtWriter_h
#define otbMPIVrtWriter_h

#include "otbMPIConfig.h"
#include "otbImageFileWriter.h"
#include "itkRegionOfInterestImageFilter.h"
#include "otbNumberOfDivisionsTiledStreamingManager.h"
#include <vector>
#include <sstream>
#include <string>

#include <itksys/SystemTools.hxx>

#include <gdal.h>
#include <gdal_priv.h>
#if defined(__GNUC__) || defined(__clang__)
# pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wshadow"
#include <vrtdataset.h>
# pragma GCC diagnostic pop
#else
#include <vrtdataset.h>
#endif

#include <ogr_spatialref.h>

namespace otb {

/**
 * \class MPIVrtWriter
 *
 * \brief Write each tile of an image into a separate Tiff file and join them in a VRT
 *
 * \ingroup OTBMPIVrtWriter
 */
template <typename TImage>
class MPIVrtWriter: public itk::ProcessObject
{
public:
  /** Standard class typedefs. */
  typedef MPIVrtWriter                                      Self;
  typedef itk::ProcessObject                                Superclass;
  typedef itk::SmartPointer<Self>                           Pointer;
  typedef itk::SmartPointer<const Self>                     ConstPointer;

  typedef TImage InputImageType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(MPIVrtWriter, itk::ProcessObject);

  using Superclass::SetInput;
  virtual void SetInput(const InputImageType *input);

  /** Get writer only input */
  const InputImageType* GetInput();

  /** Does the real work. */
  virtual void Update() override;

  virtual void SetFileName(const std::string& extendedFileName);

  virtual const char* GetFileName () const;

  /** Specify the region to write. If left NULL, then the whole image
   * is written. */
  void SetIORegion(const itk::ImageIORegion& region);
  itkGetConstReferenceMacro(IORegion, itk::ImageIORegion);

  itkSetMacro(WriteVRT, bool);
  itkGetMacro(WriteVRT, bool);

  itkSetMacro(AvailableRAM, unsigned int);
  itkGetMacro(AvailableRAM, unsigned int);

protected:
  MPIVrtWriter();
  virtual ~MPIVrtWriter();
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  MPIVrtWriter(const MPIVrtWriter &) = delete;
  void operator =(const MPIVrtWriter&) = delete;

  unsigned int m_AvailableRAM;

  itk::ImageIORegion m_IORegion;

  std::string m_Filename;

  bool m_WriteVRT;

};

/**
 *\brief Write image data to multiple files with MPI processus and add a VRT file.
 *
 * The image is divided into several pieces.
 * Each pieces is distributed to a MPI processus.
 * Each MPI processus write their pieces into a separate
 * file.
 * The master processus writes a VRT file (optional).
 *
 *\param img Image
 *\param output Output Filename
 *\param availableRAM Available memory for streaming
 *\param writeVRTFile Activate the VRT file writing
 */
template <typename TImage> void WriteMPI(TImage *img, const std::string &output, unsigned int availableRAM = 0, bool writeVRTFile=true)
{
  typename MPIVrtWriter<TImage>::Pointer writer = MPIVrtWriter<TImage>::New();
  writer->SetInput(img);
  writer->SetFileName(output);
  writer->SetAvailableRAM(availableRAM);
  writer->SetWriteVRT(writeVRTFile);
  writer->Update();
}

} // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbMPIVrtWriter.hxx"
#endif

#endif //__otbMPIVrtWriter_h
