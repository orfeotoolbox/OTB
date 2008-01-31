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
#ifndef _otbPrepareSRTMDirectory_txx
#define _otbPrepareSRTMDirectory_txx

#include "otbPrepareSRTMDirectory.h"


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
    this->Superclass::PrintSelf(os,indent);
    os << indent << " m_ULLon "  << m_ULLon << std::endl;
    os << indent << " m_ULLat "  << m_ULLat << std::endl;
    os << indent << " m_LRLon "  << m_LRLon << std::endl;
    os << indent << " m_LRLat "  << m_LRLat << std::endl;
    os << indent << " m_FullDEMDirectoryPath"  << m_FullDEMDirectoryPath  << std::endl;
    os << indent << " m_DEMDirectoryPath"  << m_DEMDirectoryPath  << std::endl;
  }


  bool PrepareSRTMDirectory::Evaluate()
  {
    
    int startX = static_cast<int>(floor(m_ULLon));
    int endX = static_cast<int>(ceil(m_LRLon));
    int startY = static_cast<int>(floor(m_LRLat));
    int endY = static_cast<int>(ceil(m_ULLat));
    
    std::cout << startX << std::endl;
    std::cout << endX << std::endl;
    std::cout << startY << std::endl;
    std::cout << endY << std::endl;
    
    for (int j=startY; j<endY; j++)
    {
      for (int i=startX; i< endX; i++)
      {
        std::ostringstream inputfilename;
        inputfilename << m_FullDEMDirectoryPath;
        inputfilename << "/";
        
        std::ostringstream outputfilename;
        outputfilename << m_DEMDirectoryPath;
        outputfilename << "/";
        
        if (j >= 0)
        {
          inputfilename << "N";
          inputfilename << setfill('0') << setw(2) << j;
          outputfilename << "N";
          outputfilename << setfill('0') << setw(2) << j;
        }
        else 
        {
          inputfilename << "S";
          inputfilename << setfill('0') << setw(2) << -j;
          outputfilename << "S";
          outputfilename << setfill('0') << setw(2) << -j;
        }
        if (i >= 0)
        {
          inputfilename << "E";
          inputfilename << setfill('0') << setw(3) << i;
          outputfilename << "E";
          outputfilename << setfill('0') << setw(3) << i;
        }
        else 
        {
          inputfilename << "W";
          inputfilename << setfill('0') << setw(3) << -i;
          outputfilename << "W";
          outputfilename << setfill('0') << setw(3) << -i;
        }
        
        inputfilename << ".hgt";
        outputfilename << ".hgt";
        
        std::cout << "Copying " << inputfilename.str() << " to " << outputfilename.str() << std::endl;
            
        //copy input file to output file
        ossimFilename inputFile(inputfilename.str().c_str());
        ossimFilename outputFile(outputfilename.str().c_str());
        inputFile.copyFileTo(outputFile);
        
        
      }
    }
    
    return true;
  }

} // namespace otb

#endif
