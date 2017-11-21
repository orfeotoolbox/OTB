/*
 * Copyright (C) CS SI
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

#ifndef otbMultiImageFileWriter_txx
#define otbMultiImageFileWriter_txx

#include "otbMultiImageFileWriter.h"
#include "otbImageIOFactory.h"
//~ #include "itkImageFileWriter.h"
#include "otbMacro.h"
//#include "s2ipfFileUtils.h"

namespace otb
{

template <class TImage>
MultiImageFileWriter::Sink<TImage>
::Sink(typename TImage::ConstPointer inputImage,
       const std::string & fileName):
  SinkBase(dynamic_cast<const ImageBaseType*>(inputImage.GetPointer())),
  //~ m_FileName(fileName),
  m_Writer(otb::ImageFileWriter<TImage>::New()),
  m_ImageIO(NULL)
{
  m_Writer->SetFileName(fileName);
  m_Writer->SetInput(inputImage);
}


template <class TImage>
bool
MultiImageFileWriter::Sink<TImage>
::CanStreamWrite()
{
  if (m_ImageIO.IsNull())
    return false;
  return m_ImageIO->CanStreamWrite();
}

template <class TImage>
void
MultiImageFileWriter::Sink<TImage>
::WriteImageInformation()
{
  m_Writer->UpdateOutputInformation();
  m_ImageIO = m_Writer->GetImageIO();
  
  
      //~ // new ImageIO: close current ImageIO if exists and open the new one
      //~ if (! m_ImageIO.IsNull()) { /* nothing to do to close IO ? */ }
      //~ m_CurrentFileIndex = fileIndex;
      //~ m_ImageIO = ImageIOFactory::CreateImageIO( m_FileNameList[fileIndex].c_str(), otb::ImageIOFactory::WriteMode );
//~ 
      //~ if (m_ImageIO.IsNull())
        //~ {
        //~ itk::ImageFileWriterException e(__FILE__, __LINE__);
        //~ std::ostringstream msg;
        //~ msg << " Could not create IO object for file " << m_FileNameList[fileIndex].c_str() << std::endl;
        //~ msg << "  Tried to create one of the following:" << std::endl;
        //~ std::list<itk::LightObject::Pointer> allobjects = itk::ObjectFactoryBase::CreateAllInstance("itkImageIOBase");
        //~ for (std::list<itk::LightObject::Pointer>::iterator i = allobjects.begin(); i != allobjects.end(); ++i)
          //~ {
          //~ itk::ImageIOBase* io = dynamic_cast<itk::ImageIOBase*>(i->GetPointer());
          //~ msg << "    " << io->GetNameOfClass() << std::endl;
          //~ }
        //~ msg << "  You probably failed to set a file suffix, or" << std::endl;
        //~ msg << "    set the suffix to an unsupported type." << std::endl;
        //~ e.SetDescription(msg.str().c_str());
        //~ e.SetLocation(ITK_LOCATION);
        //~ throw e;
        //~ }
//~ 
      //~ m_ImageIO->SetFileName(m_FileNameList[fileIndex]);
      //~ if( ! m_ImageIO->CanStreamWrite() )
        //~ {
        //~ std::ostringstream oss;
        //~ oss << " The ImageFactory selected for the image file <" << m_FileNameList[fileIndex] << "> does not support streaming.";
        //~ throw std::runtime_error(oss.str());
        //~ }
      //~ m_ImageIO->SetNumberOfDimensions(TImage::ImageDimension);
      //~ // Set the region of data to write to the output file (independently of
      //~ // current stream region)
      //~ m_CurrentFileRegion = m_InputImage->GetLargestPossibleRegion();
      //~ m_CurrentFileRegion.SetIndex(TImage::ImageDimension - 1, fileIndex * m_FileHeight + m_TopMarginTrimSize);
      //~ m_CurrentFileRegion.SetSize(TImage::ImageDimension - 1, m_FileHeight);
//~ 
      //~ const typename TImage::SpacingType& spacing = m_InputImage->GetSpacing();
      //~ const typename TImage::PointType& origin = m_InputImage->GetOrigin();
      //~ const typename TImage::DirectionType& direction = m_InputImage->GetDirection();
//~ 
      //~ for (unsigned int i = 0; i < TImage::ImageDimension; ++i)
        //~ {
        //~ // Final image size
        //~ m_ImageIO->SetDimensions(i, m_CurrentFileRegion.GetSize(i));
        //~ m_ImageIO->SetSpacing(i, spacing[i]);
        //~ m_ImageIO->SetOrigin(i, origin[i]); // TODO: fix origin for each file
        //~ vnl_vector<double> axisDirection(TImage::ImageDimension);
        //~ // Please note: direction cosines are stored as columns of the
        //~ // direction matrix
        //~ for (unsigned int j = 0; j < TImage::ImageDimension; ++j)
          //~ {
          //~ axisDirection[j] = direction[j][i];
          //~ }
        //~ m_ImageIO->SetDirection(i, axisDirection);
        //~ }
      //~ m_ImageIO->SetUseCompression(m_Writer->m_UseCompression);
//~ /*
      //~ if (! hasHDRExtension(m_FileNameList[fileIndex])) // This test will disable the creation of a .raw.aux.xml file in the case of raw/hdr format
        //~ {
        //~ m_ImageIO->SetMetaDataDictionary(m_InputImage->GetMetaDataDictionary());
        //~ }
//~ */
      //~ m_ImageIO->SetMetaDataDictionary(m_InputImage->GetMetaDataDictionary());
//~ 
      //~ /** Create Image file */
      //~ m_ImageIO->WriteImageInformation();
//~ 
    //~ if (strcmp(m_InputImage->GetNameOfClass(), "VectorImage") == 0)
      //~ {
      //~ typedef typename TImage::InternalPixelType VectorImagePixelType;
      //~ m_ImageIO->SetPixelTypeInfo(typeid(VectorImagePixelType));
//~ 
      //~ typedef typename TImage::AccessorFunctorType AccessorFunctorType;
      //~ const TImage* inputImage = dynamic_cast<const TImage*>(m_InputImage.GetPointer());
      //~ m_ImageIO->SetNumberOfComponents(AccessorFunctorType::GetVectorLength(inputImage));
      //~ }
    //~ else
      //~ {
      //~ // Set the pixel and component type; the number of components.
      //~ typedef typename TImage::PixelType ImagePixelType;
      //~ m_ImageIO->SetPixelTypeInfo(typeid(ImagePixelType));
      //~ }


}

template <class TImage>
void
MultiImageFileWriter::Sink<TImage>
::Write(const RegionType & streamRegion)
{
  // Write the whole image
  itk::ImageIORegion ioRegion(TImage::ImageDimension);
  for (unsigned int i = 0; i < TImage::ImageDimension; ++i)
    {
    ioRegion.SetSize(i, streamRegion.GetSize(i));
    ioRegion.SetIndex(i, streamRegion.GetIndex(i));
    //Set the ioRegion index using the shifted index ( (0,0 without box parameter))
    //~ ioRegion.SetIndex(i, streamRegion.GetIndex(i) - m_ShiftOutputIndex[i]);
    }
  //~ this->SetIORegion(ioRegion);
  m_ImageIO->SetIORegion(ioRegion);
  m_Writer->UpdateOutputData(nullptr);

  //~ if( ! m_UseImageIO ) return;
//~ 
  //~ if(!m_Writer->GetGranuleGenerationMode() && m_FileNameList.size() > 1)
    //~ {
    //~ throw std::runtime_error( "There must be at most one output image filename when granule generation mode is disabled" );
    //~ }
//~ 
  //~ // Find first file into which streamRegion should write
  //~ const IndexType & streamIndex = streamRegion.GetIndex();
  //~ const SizeType & streamSize = streamRegion.GetSize();
//~ 
//~ 
  //~ int firstFileIndex = 0;
  //~ int lastFileIndex = 0;
  //~ if(m_Writer->m_GranuleGenerationMode)
    //~ {
    //~ firstFileIndex = (streamIndex[1] - m_TopMarginTrimSize) / (m_Writer->m_NumberOfRowsPerGranule * m_ResolutionFactor);
    //~ lastFileIndex = (streamIndex[1] - m_TopMarginTrimSize + streamSize[1] - 1) / (m_Writer->m_NumberOfRowsPerGranule * m_ResolutionFactor);
    //~ // If lastFileIndex is greater than the index of the last filename, then use
    //~ // the last filename
    //~ lastFileIndex = std::min<int>(lastFileIndex, m_FileNameList.size() - 1);
    //~ }
//~ 
  //~ m_FileHeight = (m_Writer->m_GranuleGenerationMode)?m_Writer->m_NumberOfRowsPerGranule * m_ResolutionFactor : (m_InputImage->GetLargestPossibleRegion().GetSize(1) - m_TopMarginTrimSize - m_BottomMarginTrimSize);
//~ 
  //~ for( int fileIndex = firstFileIndex; fileIndex <= lastFileIndex; ++fileIndex )
    //~ {
    //~ // Create the image file only if the imageIO is not already open for this file
    //~ if(m_CurrentFileIndex != fileIndex)
      //~ {
      //~ CreateImageFile(fileIndex);
      //~ }
    //~ // Write part of streamRegion matching m_CurrentFileRegion
    //~ IndexValueType streamLastIndex = streamRegion.GetIndex(TImage::ImageDimension-1) + streamRegion.GetSize(TImage::ImageDimension - 1) - 1;
    //~ IndexValueType fileLastIndex = m_CurrentFileRegion.GetIndex(TImage::ImageDimension-1) + m_CurrentFileRegion.GetSize(TImage::ImageDimension - 1) - 1;
    //~ RegionType streamRegionForCurrentFile = streamRegion;
    //~ IndexValueType streamStartIndexForCurrentFile = std::max(
      //~ streamRegion.GetIndex(TImage::ImageDimension - 1),
      //~ m_CurrentFileRegion.GetIndex(TImage::ImageDimension - 1)
    //~ );
    //~ streamRegionForCurrentFile.SetIndex(TImage::ImageDimension - 1, streamStartIndexForCurrentFile );
    //~ streamRegionForCurrentFile.SetSize(
      //~ TImage::ImageDimension-1, std::min(
        //~ streamLastIndex - streamStartIndexForCurrentFile + 1,
        //~ fileLastIndex - streamStartIndexForCurrentFile + 1
      //~ )
    //~ );
//~ 
    //~ const TImage* inputImage = dynamic_cast<const TImage*>(m_InputImage.GetPointer());
    //~ const void* dataPtr = (const void*) inputImage->GetBufferPointer();
//~ 
    //~ itk::ImageIORegion ioRegion(TImage::ImageDimension);
    //~ itk::ImageIORegionAdaptor<TImage::ImageDimension>::Convert(streamRegionForCurrentFile, ioRegion, m_CurrentFileRegion.GetIndex());
    //~ m_ImageIO->SetIORegion(ioRegion);
//~ 
    //~ RegionType bufferedRegion = m_InputImage->GetBufferedRegion();
//~ 
    //~ typename TImage::Pointer cacheImage;
    //~ // before this test, bad stuff would happen when they don't match
    //~ if (bufferedRegion != streamRegionForCurrentFile)
      //~ {
      //~ otbGenericMsgDebugMacro("Requested stream region does not match generated output");
      //~ otbGenericMsgDebugMacro("Extracting output region from buffered region");
//~ 
      //~ cacheImage = TImage::New();
      //~ cacheImage->CopyInformation(m_InputImage);
      //~ cacheImage->SetBufferedRegion(streamRegionForCurrentFile);
      //~ cacheImage->Allocate();
      //~ typedef itk::ImageRegionConstIterator<TImage> ConstIteratorType;
      //~ typedef itk::ImageRegionIterator<TImage>      IteratorType;
//~ 
      //~ ConstIteratorType in(inputImage, streamRegionForCurrentFile);
      //~ IteratorType out(cacheImage, streamRegionForCurrentFile);
//~ 
      //~ // copy the data into a buffer to match the ioregion
      //~ for (in.GoToBegin(), out.GoToBegin(); !in.IsAtEnd(); ++in, ++out)
        //~ {
        //~ out.Set(in.Get());
        //~ }
//~ 
      //~ dataPtr = (const void*) cacheImage->GetBufferPointer();
//~ 
      //~ }
//~ 
    //~ // Write to file
    //~ m_ImageIO->Write(dataPtr);
    //~ }
}

} // end of namespace otb

#endif // otbMultiImageFileWriter_txx
