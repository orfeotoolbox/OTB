/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:          $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) Institut Telecom / Telecom Bretagne. All rights reserved.
  See ITCopyright.txt for details.

         This software is distributed WITHOUT ANY WARRANTY; without even
         the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
         PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __otbImageSeriesFileReaderBase_txx
#define __otbImageSeriesFileReaderBase_txx
#include "otbImageSeriesFileReaderBase.h"

namespace otb {

template <class TImage, class TInternalImage>
ImageSeriesFileReaderBase<TImage, TInternalImage>
::ImageSeriesFileReaderBase ()
{
  m_OutputList = OutputImageListType::New();
  m_ImageFileReaderList = ReaderListType::New();
  m_FileName = "";
  m_ListOfFileNames.clear();
  m_ListOfBandSelection.clear();
  m_ListOfRegionSelection.clear();
}

template <class TImage, class TInternalImage>
void
ImageSeriesFileReaderBase<TImage, TInternalImage>
::SetFileName(const std::string& file)
{
  return SetFileName(file.c_str());
}

template <class TImage, class TInternalImage>
void
ImageSeriesFileReaderBase<TImage, TInternalImage>
::SetFileName(const char * file)
{
  if (file && (file == m_FileName)) return;

  if (file)
    {
    this->m_FileName = file;
    ReadMetaFile();
    AllocateListOfComponents();
    }
  else this->m_FileName = "";

  this->Modified();
}

template <class TImage, class TInternalImage>
typename ImageSeriesFileReaderBase<TImage, TInternalImage>::OutputImageListType *
ImageSeriesFileReaderBase<TImage, TInternalImage>
::GetOutput()
{
  if (this->GetNumberOfOutputs() < 1)
    throw ImageSeriesFileReaderException(__FILE__,
                                         __LINE__,
                                         "No data to output",
                                         ITK_LOCATION);

  return static_cast<OutputImageListType*>(this->m_OutputList);
}

template <class TImage, class TInternalImage>
typename ImageSeriesFileReaderBase<TImage, TInternalImage>::OutputImageType *
ImageSeriesFileReaderBase<TImage, TInternalImage>
::GetOutput(unsigned int idx)
{
  if (this->GetNumberOfOutputs() < 1)
    throw ImageSeriesFileReaderException(__FILE__,
                                         __LINE__,
                                         "No data to output",
                                         ITK_LOCATION);

  if (idx >= m_OutputList->Size())
    throw ImageSeriesFileReaderException(__FILE__,
                                         __LINE__,
                                         "Index out of bounds",
                                         ITK_LOCATION);

  return static_cast<OutputImageType*>(this->m_OutputList->GetNthElement(idx));
}

template <class TImage, class TInternalImage>
void
ImageSeriesFileReaderBase<TImage, TInternalImage>
::ReadMetaFile()
{
  m_ListOfFileNames.clear();
  m_ListOfBandSelection.clear();
  m_ListOfRegionSelection.clear();

  this->TestFileExistanceAndReadability(m_FileName, kFileName);

  std::string aLine;
  std::ifstream inputFile(m_FileName.c_str(), std::ios_base::in);

  inputFile >> aLine;
  if (aLine != "ENVI")
    {
    inputFile.close();
    std::ostringstream msg;
    msg << "The file " << m_FileName << " is not a \"ENVI META FILE\" format\n";
    ImageSeriesFileReaderException e(__FILE__, __LINE__, msg.str().c_str(), ITK_LOCATION);
    throw e;
    return;
    }

  inputFile >> aLine;
  if (aLine != "META")
    {
    inputFile.close();
    std::ostringstream msg;
    msg << "The file " << m_FileName << " is not a \"ENVI META FILE\" format\n";
    ImageSeriesFileReaderException e(__FILE__, __LINE__, msg.str().c_str(), ITK_LOCATION);
    throw e;
    return;
    }

  inputFile >> aLine;
  if (aLine != "FILE")
    {
    inputFile.close();
    std::ostringstream msg;
    msg << "The file " << m_FileName << " is not a \"ENVI META FILE\" format\n";
    ImageSeriesFileReaderException e(__FILE__, __LINE__, msg.str().c_str(), ITK_LOCATION);
    throw e;
    return;
    }

  while (1)
    {
    /*
     * Reading the filenames
     */
    do
      {
      inputFile >> aLine;
      //std::cerr << "-> '" << aLine << "'\n";
      }
    while (aLine != "File" && inputFile.good());

    if (inputFile.good())
      {
      do
        {
        inputFile >> aLine;
        //std::cerr << "--> '" << aLine << "'\n";
        }
      while (aLine != ":" && inputFile.good());

      if (!inputFile.good())
        {
        inputFile.close();
        std::ostringstream msg;
        msg << "Unable to read image files in the  \"ENVI META FILE\" file \n";
        msg << "FileName: " << m_FileName << "\n";
        ImageSeriesFileReaderException e(__FILE__, __LINE__, msg.str().c_str(), ITK_LOCATION);
        throw e;
        return;
        }
      }
    else return;  // normal exit

    inputFile >> aLine;

    std::cerr << "-> " << aLine << "\n";

    // Get the Image fileName
    // The test may modify image file name to add the path
    // Or throw an exception when not found nor readable
    TestFileExistanceAndReadability(aLine, kImageFileName);
    m_ListOfFileNames.push_back(aLine);
    std::string imageFileName = aLine;

    /*
     * Reading the Band number
     */
    do
      {
      inputFile >> aLine;
      //std::cerr << "-> '" << aLine << "'\n";
      }
    while (aLine != "Bands:" && inputFile.good());

    if (!inputFile.good())
      {
      inputFile.close();
      std::ostringstream msg;
      msg << "Unable to read the number of bands in the images in the  \"ENVI META FILE\" file \n";
      msg << "FileName: " << m_FileName << "\n";
      ImageSeriesFileReaderException e(__FILE__, __LINE__, msg.str().c_str(), ITK_LOCATION);
      throw e;
      return;
      }

    std::vector<unsigned int> bands;
    int                       aBand, oldBand = -1;
    char                      sep;
    while (1)
      {
      inputFile >> aBand;
      if (oldBand != -1)
        {
        for (int i = oldBand; i <= aBand; ++i)
          bands.push_back(i);
        oldBand = -1;
        }
      else bands.push_back(aBand);

      int posRef = inputFile.tellg();
      inputFile >> sep;
      if (sep == '-')
        {
        oldBand = aBand + 1;
        }
      else if (sep != ',')
        {
        inputFile.seekg(posRef, std::ios_base::beg);
        break;
        }
      }

    // Storing band selection
    try {
      TestBandSelection(bands);
      }
    catch (ImageSeriesFileReaderException & e)
      {
      std::ostringstream msg;
      msg << e.GetDescription();
      msg << "Image FileName             : " << imageFileName << "\n";
      e.SetDescription(msg.str().c_str());
      throw e;
      }
    m_ListOfBandSelection.push_back(bands);

    /*
     * Reading the Region selection
     */
    do
      {
      inputFile >> aLine;
      //std::cerr << "-> '" << aLine << "'\n";
      }
    while (aLine != "Dims" && inputFile.good());

    if (inputFile.good())
      {
      do
        {
        inputFile >> aLine;
        //std::cerr << "--> '" << aLine << "'\n";
        }
      while (aLine != ":" && inputFile.good());

      if (!inputFile.good())
        {
        inputFile.close();
        std::ostringstream msg;
        msg << "Unable to read image region in the  \"ENVI META FILE\" file \n";
        msg << "FileName : " << m_FileName << "\n";
        msg << "ImageName: " << imageFileName << "\n";
        ImageSeriesFileReaderException e(__FILE__, __LINE__, msg.str().c_str(), ITK_LOCATION);
        throw e;
        return;
        }
      }

    int  beg_line, end_line, beg_col, end_col;
    char sep1, sep2, sep3;
    inputFile >> beg_col >> sep1 >> end_col >> sep2 >> beg_line >> sep3 >> end_line;

    if (!inputFile.good())
      {
      inputFile.close();
      std::ostringstream msg;
      msg << "Unable to read image region selection in the  \"ENVI META FILE\" file \n";
      msg << "FileName : " << m_FileName << "\n";
      msg << "ImageName: " << imageFileName << "\n";
      ImageSeriesFileReaderException e(__FILE__, __LINE__, msg.str().c_str(), ITK_LOCATION);
      throw e;
      return;
      }

    IndexType index;
    index[0] = beg_col - 1;
    index[1] = beg_line - 1;

    SizeType size;
    size[0] = end_col - beg_col + 1;
    size[1] = end_line - beg_line + 1;

    RegionType region;
    region.SetSize(size);
    region.SetIndex(index);

    m_ListOfRegionSelection.push_back(region);
    }
}

template <class TImage, class TInternalImage>
void
ImageSeriesFileReaderBase<TImage, TInternalImage>
::AllocateListOfComponents()
{
  for (unsigned int i = 0; i < GetNumberOfOutputs(); ++i)
    {
    m_ImageFileReaderList->PushBack(ReaderType::New());
    m_OutputList->PushBack(OutputImageType::New());
    }
}

/**
 * GenerateData
 */
template <class TImage, class TInternalImage>
void
ImageSeriesFileReaderBase<TImage, TInternalImage>
::GenerateData(unsigned int itkNotUsed(idx))
{
  std::ostringstream msg;
  msg << "Something wrong... Check the template definition of this class in the program...\n";
  msg << "\"ENVI META FILE\" FileName: " << m_FileName << "\n";
  ImageSeriesFileReaderException e(__FILE__, __LINE__, msg.str().c_str(), ITK_LOCATION);
  throw e;
}

template <class TImage, class TInternalImage>
void
ImageSeriesFileReaderBase<TImage, TInternalImage>
::GenerateData()
{
  for (unsigned int i = 0; i < GetNumberOfOutputs(); ++i)
    GenerateData(i);
}

/**
 * GenerateOutput
 */
template <class TImage, class TInternalImage>
typename ImageSeriesFileReaderBase<TImage, TInternalImage>::OutputImageListType *
ImageSeriesFileReaderBase<TImage, TInternalImage>
::GenerateOutput()
{
  this->Update();
  return this->GetOutput();
}

template <class TImage, class TInternalImage>
typename ImageSeriesFileReaderBase<TImage, TInternalImage>::OutputImageType *
ImageSeriesFileReaderBase<TImage, TInternalImage>
::GenerateOutput(unsigned int idx)
{
  this->GenerateData(idx);
  return this->GetOutput(idx);
}

template <class TImage, class TInternalImage>
void
ImageSeriesFileReaderBase<TImage, TInternalImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "File to be read : " << m_FileName << "\n";

  if (m_ListOfFileNames.size() > 0)
    {
    std::vector<std::vector<unsigned int> >::const_iterator bandSelection = m_ListOfBandSelection.begin();
    os << indent << "Image File(s) to be read\n";
    for (unsigned int i = 0; i < GetNumberOfOutputs(); ++i)
      {
      os << indent << "  " << "Filename       : " << m_ListOfFileNames[i] << "\n";
      os << indent << "  " << "RegionSelection: Index( "
         << m_ListOfRegionSelection[i].GetIndex()[0] << ", "
         << m_ListOfRegionSelection[i].GetIndex()[1] << ") Size( "
         << m_ListOfRegionSelection[i].GetSize()[0] << ", "
         << m_ListOfRegionSelection[i].GetSize()[1] << ")\n";
      os << indent << "  " << "BandSelection  : ";

      for (std::vector<unsigned int>::const_iterator bd = (*bandSelection).begin();
           bd != (*bandSelection).end();
           ++bd)
        {
        os << *bd << " ";
        }
      os << "\n";

      ++bandSelection;
      }
    }
}

template <class TImage, class TInternalImage>
void
ImageSeriesFileReaderBase<TImage, TInternalImage>
::TestFileExistanceAndReadability(std::string& file, FileType fileType)
{
  // Test if the file exists.
  if (!itksys::SystemTools::FileExists(file.c_str()))
    {
    if (fileType != kImageFileName)
      {
      ImageSeriesFileReaderException e(__FILE__, __LINE__);
      std::ostringstream msg;
      msg << "The file doesn't exist. \n";
      if (fileType == kFileName) msg << "Filename = " << file << "\n";
      else msg << "File = " << file << "\n";
      e.SetDescription(msg.str().c_str());
      throw e;
      return;
      }
    else
      {
      std::vector<std::string> fullPath;
      fullPath.push_back(itksys::SystemTools::GetFilenamePath(m_FileName));
      fullPath.push_back("/");
      fullPath.push_back(file);

      std::string fullFileName = itksys::SystemTools::JoinPath(fullPath);

      if (!itksys::SystemTools::FileExists(fullFileName.c_str()))
        {
        ImageSeriesFileReaderException e(__FILE__, __LINE__);
        std::ostringstream msg;
        msg << "The image file doesn't exist. \n";
        msg << "ImageFileName     = " << file << "\n";
        msg << "tested path       = " << itksys::SystemTools::GetFilenamePath(m_FileName) << "\n";
        msg << "Other Tested File = " << fullFileName << "\n";
        e.SetDescription(msg.str().c_str());
        throw e;
        return;
        }
      else
        {
        // At this step, image file name is modified to add its path
        file = fullFileName;
        }
      }
    }

  // Test if the file can be open for reading access.
  std::ifstream readTester;
  readTester.open(file.c_str());
  if (readTester.fail())
    {
    readTester.close();
    std::ostringstream msg;
    msg << "The file couldn't be opened for reading. "
        << std::endl << "Filename: " << file << std::endl;
    ImageSeriesFileReaderException e(__FILE__, __LINE__, msg.str().c_str(), ITK_LOCATION);
    throw e;
    return;
    }
  readTester.close();
}

} // end of namespace otb

#endif
