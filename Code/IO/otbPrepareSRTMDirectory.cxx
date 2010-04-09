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

#include "otbPrepareSRTMDirectory.h"
#include <iostream>
#include <iomanip>
#include "otbMath.h"

namespace otb
{

/**
   * Constructor
 */

PrepareSRTMDirectory::PrepareSRTMDirectory()
{
  m_ULLon = 0.0;
  m_ULLat = 0.0;
  m_LRLon = 0.0;
  m_LRLat = 0.0;
  m_FullDEMDirectoryPath = "";
  m_DEMDirectoryPath = "";
}

/**
   *
 */

void
PrepareSRTMDirectory
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  this->Superclass::PrintSelf(os, indent);
  os << indent << " m_ULLon "  << m_ULLon << std::endl;
  os << indent << " m_ULLat "  << m_ULLat << std::endl;
  os << indent << " m_LRLon "  << m_LRLon << std::endl;
  os << indent << " m_LRLat "  << m_LRLat << std::endl;
  os << indent << " m_FullDEMDirectoryPath"  << m_FullDEMDirectoryPath  << std::endl;
  os << indent << " m_DEMDirectoryPath"  << m_DEMDirectoryPath  << std::endl;
}

bool PrepareSRTMDirectory::Evaluate()
{
  // Check directories
  //
  ossimFilename fullDir(m_FullDEMDirectoryPath.c_str());
  if (!fullDir.exists() || !fullDir.isDir())
    {
    itkExceptionMacro(<< "Invalid FullDEMDirectoryPath: " << m_FullDEMDirectoryPath);
    }

  ossimFilename DEMDir(m_DEMDirectoryPath.c_str());
  if (!DEMDir.exists())
    {
    DEMDir.createDirectory();
    }

  // Check input points
  int startX = static_cast<int>(vcl_floor(m_ULLon));
  int endX = static_cast<int>(vcl_ceil(m_LRLon));
  int startY = static_cast<int>(vcl_floor(m_LRLat));
  int endY = static_cast<int>(vcl_ceil(m_ULLat));

  if (startX > endX || startY > endY)
    {
    itkExceptionMacro(<< "Invalid boundariy points");
    }
  if (startX < -180 || startX > 180 || endX < -180 || endX > 180)
    {
    itkExceptionMacro(<< "Invalid longitude coordinates, must be in [-180; 180].");
    }
  if (startY < -90 || startY > 90 || endY < -90 || endY > 90)
    {
    itkExceptionMacro(<< "Invalid latitude coordinates, must be in [-90; 90]");
    }

  for (int j = startY; j <= endY; ++j)
    {
    for (int i = startX; i <= endX; ++i)
      {
      std::ostringstream inputfilename;
      inputfilename << m_FullDEMDirectoryPath;
      inputfilename << "/";

      std::ostringstream outputfilename;
      outputfilename << m_DEMDirectoryPath;

      outputfilename << "/";

      // Build the file name
      if (j >= 0)
        {
        inputfilename << "N";
        outputfilename << "N";
        }
      else
        {
        inputfilename << "S";
        outputfilename << "S";
        }

      inputfilename << std::setfill('0') << std::setw(2) << vcl_abs(j);
      outputfilename << std::setfill('0') << std::setw(2) << vcl_abs(j);

      if (i >= 0)
        {
        inputfilename << "E";
        outputfilename << "E";
        }
      else
        {
        inputfilename << "W";
        outputfilename << "W";
        }

      inputfilename << std::setfill('0') << std::setw(3) << vcl_abs(i);
      outputfilename << std::setfill('0') << std::setw(3) << vcl_abs(i);

      inputfilename << ".hgt";
      outputfilename << ".hgt";

      std::cout << "Copying " << inputfilename.str() << " to " << outputfilename.str() << std::endl;

      //copy input file to output file
      ossimFilename inputFile(inputfilename.str().c_str());
      ossimFilename outputFile(outputfilename.str().c_str());
      if (!inputFile.exists())
        {
        itkExceptionMacro(<< "ERROR, can't find file " << inputFile);
        }
      inputFile.copyFileTo(outputFile);

      }
    }

  return true;
}

} // namespace otb
