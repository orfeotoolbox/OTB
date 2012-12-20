/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbImageFileWriter_txx
#define __otbImageFileWriter_txx

#include "otbImageFileWriter.h"
#include "otbImageIOFactory.h"

#include "itkObjectFactoryBase.h"

#include "itkMetaDataObject.h"
#include "otbImageKeywordlist.h"
#include "otbMetaDataKey.h"

#include "otbMacro.h"

namespace otb
{

//---------------------------------------------------------
template <class TInputImage>
ImageFileWriter<TInputImage>
::ImageFileWriter() : itk::ImageFileWriter<TInputImage>()
  , m_WriteGeomFile(true)
{
  m_BufferMemorySize = 0;
  m_BufferNumberOfLinesDivisions = 0;
  // default to AUTOMATIC_NUMBER_OF_DIVISIONS
  m_CalculationDivision = SET_AUTOMATIC_NUMBER_OF_STREAM_DIVISIONS;

  // create default region splitter
  m_RegionSplitterUseToEstimateNumberOfStreamDivisions = itk::ImageRegionSplitter<InputImageDimension>::New();

  this->Superclass::SetNumberOfStreamDivisions(10);

  m_FilenameHelper = FNameHelperType::New();
}

//---------------------------------------------------------
template <class TInputImage>
ImageFileWriter<TInputImage>
::~ImageFileWriter()
{
}

//---------------------------------------------------------
template <class TInputImage>
void
ImageFileWriter<TInputImage>
::Write()
{
  // NOTE: this const_cast<> is due to the lack of const-correctness
  // of the ProcessObject.
  InputImageType * nonConstInput = const_cast<InputImageType *>(this->GetInput());

  // Update the meta data
  nonConstInput->UpdateOutputInformation();
  if (this->GetImageIO() == NULL)
    {
    this->SetImageIO(ImageIOFactory::CreateImageIO(this->GetFileName(), itk::ImageIOFactory::WriteMode));
    this->SetNumberOfStreamDivisions(static_cast<unsigned int> (CalculateNumberOfStreamDivisions()));
    }

  if ((strcmp(this->GetImageIO()->GetNameOfClass(), "GDALImageIO") == 0)
      && m_FilenameHelper->gdalCreationOptionsIsSet()   )
    {
    typename GDALImageIO::Pointer imageIO = dynamic_cast<GDALImageIO*>(this->GetImageIO());
    imageIO->SetOptions(m_FilenameHelper->GetgdalCreationOptions());
    }

  this->Superclass::Write();
  //TODO: Force ImageIO destructor. Should be fixed once GDALImageIO
  //will be refactored.
  this->SetImageIO(NULL);

  if (m_WriteGeomFile && m_FilenameHelper->GetWriteGEOMFile())
    {
    ImageKeywordlist otb_kwl;
    itk::MetaDataDictionary dict = this->GetInput()->GetMetaDataDictionary();
    itk::ExposeMetaData<ImageKeywordlist>(dict, MetaDataKey::OSSIMKeywordlistKey, otb_kwl);
    WriteGeometry(otb_kwl, this->GetFileName());
    }
}


/**
 *
 */
template <class TInputImage>
void
ImageFileWriter<TInputImage>
::SetBufferMemorySize(unsigned long memory_size_divisions)
{
  m_BufferMemorySize = memory_size_divisions;
  m_CalculationDivision = SET_BUFFER_MEMORY_SIZE;
  this->Modified();
}

/**
 *
 */
template <class TInputImage>
void
ImageFileWriter<TInputImage>
::SetBufferNumberOfLinesDivisions(unsigned long nb_lines_divisions)
{
  m_BufferNumberOfLinesDivisions = nb_lines_divisions;
  m_CalculationDivision = SET_BUFFER_NUMBER_OF_LINES;
  this->Modified();
}

/**
 *
 */
template <class TInputImage>
void
ImageFileWriter<TInputImage>
::SetNumberOfStreamDivisions(unsigned long nb_divisions)
{
  this->Superclass::SetNumberOfStreamDivisions(nb_divisions);
  m_CalculationDivision = SET_NUMBER_OF_STREAM_DIVISIONS;
  this->Modified();
}

/**
 *
 */
template <class TInputImage>
void
ImageFileWriter<TInputImage>
::SetAutomaticNumberOfStreamDivisions(void)
{
  m_CalculationDivision = SET_AUTOMATIC_NUMBER_OF_STREAM_DIVISIONS;
  this->Modified();
}

/**
 *
 */
/*
template <class TInputImage>
void
ImageFileWriter<TInputImage>
::SetTilingStreamDivisions(void)
{
  m_CalculationDivision = SET_TILING_WITH_SET_AUTOMATIC_NUMBER_OF_STREAM_DIVISIONS;
  m_RegionSplitter = itk::ImageRegionMultidimensionalSplitter<InputImageDimension>::New();
  this->Modified();
}


template <class TInputImage>
void
ImageFileWriter<TInputImage>
::SetTilingStreamDivisions(unsigned long nb_divisions)
{
  m_CalculationDivision = SET_TILING_WITH_SET_NUMBER_OF_STREAM_DIVISIONS;
  this->SetNumberOfStreamDivisions( nb_divisions );
  m_RegionSplitter = itk::ImageRegionMultidimensionalSplitter<InputImageDimension>::New();
  this->Modified();
}
*/

/**
 *
 */
template <class TInputImage>
unsigned long
ImageFileWriter<TInputImage>
::GetNumberOfStreamDivisions(void)
{
  return (CalculateNumberOfStreamDivisions());
}

/**
 *
 */
template <class TInputImage>
std::string
ImageFileWriter<TInputImage>
::GetMethodUseToCalculateNumberOfStreamDivisions(void)
{
  return (StreamingTraitsType::GetMethodUseToCalculateNumberOfStreamDivisions(m_CalculationDivision));
}

/**
 *
 */
template<class TInputImage>
unsigned long
ImageFileWriter<TInputImage>
::CalculateNumberOfStreamDivisions(void)
{

  return StreamingTraitsType
         ::CalculateNumberOfStreamDivisions(this->GetInput(),
                                            this->GetInput()->GetLargestPossibleRegion(),
                                            m_RegionSplitterUseToEstimateNumberOfStreamDivisions,
                                            m_CalculationDivision,
                                            this->Superclass::GetNumberOfStreamDivisions(),
                                            m_BufferMemorySize,
                                            m_BufferNumberOfLinesDivisions);
}

//---------------------------------------------------------
template <class TInputImage>
void
ImageFileWriter<TInputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

template <class TInputImage>
void
ImageFileWriter<TInputImage>
::SetFileName(std::string extendedFileName)
{
  this->SetFileName(extendedFileName.c_str());
}

template <class TInputImage>
void
ImageFileWriter<TInputImage>
::SetFileName(const char* extendedFileName)
{
  this->m_FilenameHelper->SetExtendedFileName(extendedFileName);
  Superclass::SetFileName(this->m_FilenameHelper->GetSimpleFileName());
}

template <class TInputImage>
const char*
ImageFileWriter<TInputImage>
::GetFileName () const
{
return this->m_FilenameHelper->GetSimpleFileName();
}

} // end namespace otb

#endif
