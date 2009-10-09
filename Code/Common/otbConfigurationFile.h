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
#ifndef __otbConfigurationFile_h
#define __otbConfigurationFile_h

#include "otbConfigure.h"

#include "itkObject.h"
#include "itkObjectFactory.h"
#include "ConfigFile.h"

namespace otb
{

/** \class ConfigurationFile
   *    \brief Manage OTB ConfigurationFile file
*/
  
  class ConfigurationFile
  : public itk::Object
  {
    public:
      /** Standard class typedef */
      
      typedef ConfigurationFile Self;
      typedef itk::Object Superclass;
      typedef itk::SmartPointer<Self> Pointer;
      typedef itk::SmartPointer<const Self> ConstPointer;
      
      
      /** Standard macro */
      itkTypeMacro(ConfigurationFile,Object);
      /** This is protected for the singleton. Use GetInstance() instead. */
      itkNewMacro(Self);
      
      /** Get the unique instanc1e of the model */
//       static Pointer GetInstance()
//       {
//         if (!Instance)
//         {
//           Instance = Self::New();
//         }
//         return Instance;
//       };
      
      ConfigFile * GetOTBConfig()
      {
        return m_OTBConfig;
      };
      
      std::string GetLanguage()
      {
        return m_OTBConfig->read<std::string>( "LANG" );
      };
//       std::string lib(OTB_CONFIG);
      
    protected:
      
      /** Constructor */
      ConfigurationFile(){
        std::string OTBBinDir(OTB_CONFIG);
        m_OTBConfig = new ConfigFile(OTBBinDir + "/otb_config.inp");
      }
      ;
      /** Destructor */
      ~ConfigurationFile(){};
      /** PrintSelf method */
      void PrintSelf(std::ostream& os, itk::Indent indent) const {};
    private:
      /** The instance singleton */
//       static Pointer Instance = NULL;
      ConfigFile * m_OTBConfig;
};
}// end namespace
#endif
