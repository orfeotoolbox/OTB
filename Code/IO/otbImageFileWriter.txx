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
#include "vnl/vnl_vector.h"

#include "imaging/ossimImageHandlerRegistry.h"
// #include "ossim/imaging/ossimImageHandlerSarFactory.h"
#include "imaging/ossimImageHandler.h"
#include "init/ossimInit.h"
#include "base/ossimKeywordlist.h"

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

{
  m_BufferMemorySize = 0;
  m_BufferNumberOfLinesDivisions = 0;
  // default to AUTOMATIC_NUMBER_OF_DIVISIONS
  m_CalculationDivision = SET_AUTOMATIC_NUMBER_OF_STREAM_DIVISIONS;

  // create default region splitter
  m_RegionSplitterUseToEstimateNumberOfStreamDivisions = itk::ImageRegionSplitter<InputImageDimension>::New();

  this->Superclass::SetNumberOfStreamDivisions(10);
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
  if(this->GetImageIO() == NULL)
    {
    this->SetImageIO(ImageIOFactory::CreateImageIO(this->GetFileName(), itk::ImageIOFactory::WriteMode));
    this->SetNumberOfStreamDivisions(static_cast<unsigned int> (CalculateNumberOfStreamDivisions()));
    }

  this->Superclass::Write();
  //TODO: Force ImageIO desctructor. Should be fixed once GDALImageIO
  //will be refactored.
  this->SetImageIO(NULL);
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
  this->Superclass::SetNumberOfStreamDivisions( nb_divisions );
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
  return(CalculateNumberOfStreamDivisions());
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
  Superclass::PrintSelf(os,indent);
}

} // end namespace otb

#endif
