/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:          $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) Institut Mines-Telecom. All rights reserved.
  See IMTCopyright.txt for details.

         This software is distributed WITHOUT ANY WARRANTY; without even
         the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
         PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __otbImageSeriesFileReaderBase_h
#define __otbImageSeriesFileReaderBase_h

#include <iostream>
#include <fstream>
#include <string>

#include "itkMacro.h"

#include "otbImage.h"
#include "otbVectorImage.h"

#include "otbImageListSource.h"
#include "otbImageFileReader.h"

namespace otb {

class ImageSeriesFileReaderException : public itk::ExceptionObject
{
public:
  itkTypeMacro(ImageSeriesFileReaderException, ExceptionObject);

  ImageSeriesFileReaderException(const char *file, unsigned int line,
                                 const char* message = "Error in IO",
                                 const char* loc = "Unknown") :
    itk::ExceptionObject(file, line, message, loc) {}

  ImageSeriesFileReaderException(const std::string& file, unsigned int line,
                                 const char* message = "Error in IO",
                                 const char* loc = "Unknown") :
    itk::ExceptionObject(file, line, message, loc) {}
};

/** \class ImageSeriesFileReaderBase
 * \brief
 * \sa ImageSeriesFileReader
 *
 * \ingroup OTBImageIO
 */

template <class TImage, class TInternalImage = TImage>
class ITK_EXPORT ImageSeriesFileReaderBase
  : public ImageListSource<TImage>
{
public:
  /** Standart typedefs */
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
  virtual void SetFileName(const char * file);

  /** get the Filenames */
  std::string GetFileName(unsigned int i) const
  {
    return this->m_ListOfFileNames.at(i);
  }

  /** Get the readers */
  itkGetObjectMacro(ImageFileReaderList, ReaderListType);
  ReaderType * GetImageFileReader(unsigned int i) const
  {
    return this->m_ImageFileReaderList->GetNthElement(i);
  }

  /** Get the output list */
  virtual unsigned int GetNumberOfOutputs() const
  {
    return m_ListOfFileNames.size();
  }
  virtual OutputImageListType * GetOutput(void);
  virtual OutputImageType *     GetOutput(DataObjectPointerArraySizeType idx);

  /** Performs selective file extraction */
  virtual OutputImageListType * GenerateOutput(void);
  virtual OutputImageType * GenerateOutput(DataObjectPointerArraySizeType idx);

  /** Synchronization */
  void Update()
  {
    this->GenerateData();
  }

protected:
  ImageSeriesFileReaderBase();
  virtual ~ImageSeriesFileReaderBase () {}

  enum FileType { kFileName = 0, kImageFileName, kAnyFileName };
  /**
   * Test files.
   * If the filename to test is an imageFileName, the file name may be modified in
   * order to add the appropriated path
   */
  virtual void TestFileExistanceAndReadability(std::string& file, FileType fileType);
  virtual void TestBandSelection(std::vector<unsigned int>& itkNotUsed(bands)) {}

  virtual void GenerateData(void);

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
   * that the user may invoke throught GenerateOutput() or GenerateOutput( idx ).
   */
  virtual void AllocateListOfComponents(void);

  /** PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

  std::string                m_FileName;
  OutputImageListPointerType m_OutputList;

  std::vector<std::string>                m_ListOfFileNames;
  std::vector<std::vector<unsigned int> > m_ListOfBandSelection;
  std::vector<InternalRegionType>         m_ListOfRegionSelection;

  ReaderListPointerType m_ImageFileReaderList;

private:
  ImageSeriesFileReaderBase (const Self &);
  void operator =(const Self&);
}; // end of class

} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageSeriesFileReaderBase.txx"
#endif

#endif
