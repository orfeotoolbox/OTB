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
#ifndef __otbConfiguration_h
#define __otbConfiguration_h

#include "itkObject.h"
#include "itkObjectFactory.h"
#include "/home/christop/manuel/Cnes/otb/Dev/OTB/Utilities/otbconfigfile/ConfigFile.h"

namespace otb
{

/** \class Configuration
   *    \brief Manage OTB configuration file
*/
  
  class Configuration 
  : public itk::Object
  {
    public:
      /** Standard class typedef */
      
      typedef Configuration Self;
      typedef itk::Object Superclass;
      typedef itk::SmartPointer<Self> Pointer;
      typedef itk::SmartPointer<const Self> ConstPointer;

      /** Standard macro */
      itkNewMacro(Self);
      itkTypeMacro(Configuration,Object);

     
      /** Get arg */
//       itkGetStringMacro(Language);
//       itkGetStringMacro(GeodidePath);
//       itkGetStringMacro(WorldShapePath);
//       itkGetStringMacro(SRTMPath);
//       itkGetStringMacro(NodeId);
//       itkGetStringMacro(NodeId);
//       itkGetStringMacro(NodeId);
      /** Load parameters from the configuration file*/
//  void Load() {
//         
//       } 
      ConfigFile * GetOTBConfig()
      {
        return m_OTBConfig;
      };
      
      std::string GetLanguage()
      {
        return m_OTBConfig->read<std::string>( "LANG" );
      };
    protected:
      /** Constructor */
      Configuration()
      {
        m_OTBConfig = new ConfigFile("/home/christop/example.inp");
//         ConfigFile config( "/home/christop/example.inp" );
//         m_Language = config.read<std::string>( "LANG" );
//         m_GeodidePath = config.read<std::string>( "GEOIDE" );  
//         m_WorldShapePath =  config.read<std::string>( "WORLD" );   
//         m_SRTMPath = config.read<std::string>( "SRTM" );      
      };
      /** Destructor */
      ~Configuration() {};
      /** PrintSelf method */
      void PrintSelf(std::ostream& os, itk::Indent indent) const
      {
        Superclass::PrintSelf(os, indent);
      };
    private:
//       ConfigFile * m_ConfigFile;
//       std::string m_Language;
//       std::string m_GeodidePath;
//       std::string m_WorldShapePath;
//       std::string m_SRTMPath;
//       unsigned int m_TileSizeX;
//       unsigned int m_TileSizeY;
      ConfigFile * m_OTBConfig;
};
}// end namespace
#endif
