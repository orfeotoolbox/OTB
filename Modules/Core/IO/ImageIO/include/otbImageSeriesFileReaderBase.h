/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
 * Copyright (C) 2007-2012 Institut Mines Telecom / Telecom Bretagne
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


#ifndef otbImageSeriesFileReaderBase_h
#define otbImageSeriesFileReaderBase_h

#include <string>

#include "itkMacro.h"

#include "otbImage.h"
#include "otbVectorImage.h"

#include "otbImageListSource.h"
#include "otbImageFileReader.h"

namespace otb
{

class ImageSeriesFileReaderException : public itk::ExceptionObject
{
public:
  itkTypeMacro(ImageSeriesFileReaderException, ExceptionObject);

  ImageSeriesFileReaderException(const char* file, unsigned int line, const char* message = "Error in IO", const char* loc = "Unknown")
    : itk::ExceptionObject(file, line, message, loc)
  {
  }

  ImageSeriesFileReaderException(const std::string& file, unsigned int line, const char* message = "Error in IO", const char* loc = "Unknown")
    : itk::ExceptionObject(file, line, message, loc)
  {
  }

  ImageSeriesFileReaderException(const std::string& file, unsigned int line, const std::string& message = "Error in IO", const std::string& loc = "Unknown")
    : itk::ExceptionObject(file, line, message, loc)
  {
  }
};

/** \class ImageSeriesFileReaderBase
 * \brief
 * \sa ImageSeriesFileReader
 *
 * \ingroup OTBImageIO
 */

template <class TImage, class TInternalImage = TImage>
class ImageSeriesFileReaderBase : public ImageListSource<TImage>
{
public:
  /** Standard typedefs */
  typedef ImageSeriesFileReaderBase     Self;
  typedef ImageListSource<TImage>       Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Creation through object factory macro */
  itkNewMacro(Self);
  /** Runtime information macro */
  itkTypeMacro(ImageSeriesFileReaderBase, ImageListSource);

  typedef TImage                                      OutputImageType;
  typedef typename OutputImageType::Pointer           OutputImagePointerType;
  typedef typename OutputImageType::InternalPixelType PixelType;
  typedef typename OutputImageType::ValueType         ValueType;
  typedef typename OutputImageType::IndexType         IndexType;
  typedef typename OutputImageType::SizeType          SizeType;
  typedef typename OutputImageType::RegionType        RegionType;

  typedef ImageList<OutputImageType>                 OutputImageListType;
  typedef typename OutputImageListType::Pointer      OutputImageListPointerType;
  typedef typename OutputImageListType::ConstPointer OutputImageListConstPointerType;

  typedef TInternalImage                                InternalImageType;
  typedef typename InternalImageType::Pointer           InternalImagePointerType;
  typedef typename InternalImageType::InternalPixelType InternalPixelType;
  typedef typename InternalImageType::ValueType         InternalValueType;
  typedef typename InternalImageType::IndexType         InternalIndexType;
  typedef typename InternalImageType::SizeType          InternalSizeType;
  typedef typename InternalImageType::RegionType        InternalRegionType;

  typedef ImageFileReader<InternalImageType> ReaderType;
  typedef typename ReaderType::Pointer       ReaderPointerType;

  typedef ObjectList<ReaderType>           ReaderListType;
  typedef typename ReaderListType::Pointer ReaderListPointerType;

  typedef itk::ProcessObject::DataObjectPointerArraySizeType DataObjectPointerArraySizeType;

  /** Get the file to be read */
  itkGetStringMacro(FileName);

  /**
   * Set the file to be read. Once the Filename is set, ReadMeatFile is called in order to get
   * the number of image files to be read, the images file names, the band and region
   * selection
   */
  virtual void SetFileName(const std::string& file);

  /** get the Filenames */
  std::string GetFileName(unsigned int i) const
  {
    return this->m_ListOfFileNames.at(i);
  }

  /** Get the readers */
  itkGetObjectMacro(ImageFileReaderList, ReaderListType);
  ReaderType* GetImageFileReader(unsigned int i) const
  {
    return this->m_ImageFileReaderList->GetNthElement(i);
  }

  /** Get the output list */
  virtual unsigned int GetNumberOfOutputs() const
  {
    return m_ListOfFileNames.size();
  }
  OutputImageListType*     GetOutput(void) override;
  virtual OutputImageType* GetOutput(DataObjectPointerArraySizeType idx);

  /** Performs selective file extraction */
  virtual OutputImageListType* GenerateOutput(void);
  virtual OutputImageType* GenerateOutput(DataObjectPointerArraySizeType idx);

  /** Synchronization */
  void Update() override
  {
    this->GenerateData();
  }

protected:
  ImageSeriesFileReaderBase();
  ~ImageSeriesFileReaderBase() override = default;

  enum FileType
  {
    kFileName = 0,
    kImageFileName,
    kAnyFileName
  };
  /**
   * Test files.
   * If the filename to test is an imageFileName, the file name may be modified in
   * order to add the appropriated path
   */
  virtual void TestFileExistenceAndReadability(std::string& file, FileType fileType);
  virtual void TestBandSelection(std::vector<unsigned int>& itkNotUsed(bands))
  {
  }

  void GenerateData(void) override;

  /** GenerateData
   * This method will be specialised if template definitions follow:
   * - TImage is a VectorImage
   * - TImage is an Image and TInteranalImage is a VectorImage
   * - TImage and TInternalImage are of Image type.
   */
  virtual void GenerateData(DataObjectPointerArraySizeType idx);

  /**
   * Read the MetaFile and test its effectiveness once inputFileName is set.
   */
  virtual void ReadMetaFile();
  /**
   * Once MetaFile is read, allocation of lists are performed in SetFileName.
   * This allows specific (or global) initialization in the GenerateData methods,
   * that the user may invoke through GenerateOutput() or GenerateOutput( idx ).
   */
  virtual void AllocateListOfComponents(void);

  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  std::string                m_FileName;
  OutputImageListPointerType m_OutputList;

  std::vector<std::string>               m_ListOfFileNames;
  std::vector<std::vector<unsigned int>> m_ListOfBandSelection;
  std::vector<InternalRegionType>        m_ListOfRegionSelection;

  ReaderListPointerType m_ImageFileReaderList;

private:
  ImageSeriesFileReaderBase(const Self&);
  void operator=(const Self&);
}; // end of class

} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageSeriesFileReaderBase.hxx"
#endif

#endif
