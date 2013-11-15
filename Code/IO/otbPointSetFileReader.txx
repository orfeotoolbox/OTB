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
#ifndef __otbPointSetFileReader_txx
#define __otbPointSetFileReader_txx

#include "otbPointSetFileReader.h"
#include "otbMacro.h"

#include <liblas/capi/liblas.h>

#include <fstream>  // std::ifstream
#include <iostream> // std::cout
#include <iomanip>

namespace otb
{
template <class TOutputPointSet>
PointSetFileReader<TOutputPointSet>
::PointSetFileReader() : otb::PointSetSource<TOutputPointSet>()
{
  m_NumberOfPoints = -1;
  m_MinX = 0;
  m_MaxX = 0;
  m_MinY = 0;
  m_MaxY = 0;
}

template <class TOutputPointSet>
PointSetFileReader<TOutputPointSet>
::~PointSetFileReader()
{
}

template <class TOutputPointSet>
void
PointSetFileReader<TOutputPointSet>
::GenerateOutputInformation(void)
{
  typename TOutputPointSet::Pointer output = this->GetOutput();

  otbDebugMacro(<< "Reading file for GenerateOutputInformation()" << m_FileName);

  // Check to see if we can read the file given the name or prefix
  //
  if (m_FileName == "")
    {
    throw ImageFileReaderException(__FILE__, __LINE__, "FileName must be specified", ITK_LOCATION);
    }

  this->TestFileExistanceAndReadability();

  LASReaderH reader = LASReader_Create(m_FileName.c_str());
  LASHeaderH header = LASReader_GetHeader(reader);
  
  otbDebugMacro(<< "Signature: " << LASHeader_GetFileSignature(header));
  otbDebugMacro(<< "Points count: " << LASHeader_GetPointRecordsCount(header));

  m_NumberOfPoints = LASHeader_GetPointRecordsCount(header);

  m_MinX = LASHeader_GetMinX(header);
  m_MaxX = LASHeader_GetMaxX(header);
  m_MinY = LASHeader_GetMinY(header);
  m_MaxY = LASHeader_GetMaxY(header);
  
  LASReader_Destroy(reader);
}

template <class TOutputPointSet>
void
PointSetFileReader<TOutputPointSet>
::TestFileExistanceAndReadability()
{
  // Test if the file exists.
  if (!itksys::SystemTools::FileExists(m_FileName.c_str()))
    {
    ImageFileReaderException e(__FILE__, __LINE__);
    std::ostringstream msg;
    msg << "The file doesn't exist. "
        << std::endl << "Filename = " << m_FileName
        << std::endl;
    e.SetDescription(msg.str().c_str());
    throw e;
    return;
    }

  // Test if the file can be open for reading access.
  std::ifstream readTester;
  readTester.open(m_FileName.c_str());
  if (readTester.fail())
    {
    readTester.close();
    std::ostringstream msg;
    msg << "The file couldn't be opened for reading. "
        << std::endl << "Filename: " << m_FileName
        << std::endl;
    ImageFileReaderException e(__FILE__, __LINE__, msg.str().c_str(), ITK_LOCATION);
    throw e;
    return;

    }
  readTester.close();
}

template <class TOutputPointSet>
void PointSetFileReader<TOutputPointSet>
::GenerateData()
{
  typename TOutputPointSet::Pointer output = this->GetOutput();
  
  LASReaderH reader = LASReader_Create(m_FileName.c_str());
  LASHeaderH header = LASReader_GetHeader(reader);

  otbDebugMacro(<< "Signature: " << LASHeader_GetFileSignature(header));
  otbDebugMacro(<< "Points count: " << LASHeader_GetPointRecordsCount(header));

  m_NumberOfPoints = LASHeader_GetPointRecordsCount(header);

  //If the output pointset is of dimension 2, altitude is stored as information
  if (PointType::PointDimension == 2)
    {
    LASPointH pt = LASPoint_Create();
    while ((pt = LASReader_GetNextPoint(reader)))
      {
      
      PointType point;
      point[0] = LASPoint_GetX(pt);
      point[1] = LASPoint_GetY(pt);

      unsigned long i = output->GetNumberOfPoints();
      output->SetPoint(i, point);

      PixelType V;
      V = static_cast<PixelType>(LASPoint_GetZ(pt));
      output->SetPointData(i, V);
      }
    }
  //If the output pointset is of dimension 3, store the altitude as information
  else if (PointType::PointDimension == 3)
    {
    LASPointH p = LASPoint_Create();
    while ((p = LASReader_GetNextPoint(reader)))
      {
      //liblas::Point const& p = reader.GetPoint();

      PointType point;
      point[0] = LASPoint_GetX(p);
      point[1] = LASPoint_GetY(p);
      point[2] = LASPoint_GetZ(p);

      unsigned long i = output->GetNumberOfPoints();
      output->SetPoint(i, point);

      PixelType V;
      V = static_cast<PixelType>(LASPoint_GetZ(p));
      output->SetPointData(i, V);

      }
    }
  else
    {
    itkExceptionMacro(<< "Can't handle pointset dimension other than 2 and 3");
    }

  LASReader_Destroy(reader);
}

template <class TOutputPointSet>
void PointSetFileReader<TOutputPointSet>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Number of points: " << this->m_NumberOfPoints << std::endl;
  os << indent << std::setprecision(15);
  os << indent << "Min X: " << this->m_MinX << std::endl;
  os << indent << "Max X: " << this->m_MaxX << std::endl;
  os << indent << "Min Y: " << this->m_MinY << std::endl;
  os << indent << "Max Y: " << this->m_MaxY << std::endl;
  os << indent << "m_FileName: " << this->m_FileName << "\n";
}

} //namespace otb

#endif
