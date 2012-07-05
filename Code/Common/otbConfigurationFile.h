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
#include "itksys/FundamentalType.h"

namespace otb
{

/** \class ConfigurationFile
 *  \brief Manage OTB ConfigurationFile file
 *
 *  The path to the file can be set with environment variable OTB_CONFIG_FILE
 *
*/

class ConfigurationFile
  : public itk::Object
{
public:
  /** Standard class typedef */

  typedef ConfigurationFile             Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkTypeMacro(ConfigurationFile, Object);

  /** Get the unique instanc1e of the model */
  static Pointer GetInstance();

  ConfigFile * GetOTBConfig();

  bool IsValid() const;

  bool HasKey(const std::string& key) const;

  /** Get parameter*/
  template<typename T> T GetParameter(const std::string& key) const
  {
    if (m_OTBConfig == NULL)
      {
      itkExceptionMacro(<< "Configuration file not found.");
      }

    try
      {
      return m_OTBConfig->read<T>(key);
      }
    catch (ConfigFile::key_not_found& e)
      {
      itkExceptionMacro(<< "Error - Key '" << e.key << "' not found.");
      }
  }

  /** Returns the DEM directory entry in the config file, or an empty string if not found */
  std::string GetDEMDirectory() const;

  /** Returns the Geoid entry in the config file, or an empty string if not found */
  std::string GetGeoidFile() const;

  /** Returns the available RAM in bytes  */
  ::itksysFundamentalType_UInt64 GetAvailableRAMInBytes() const;

  /** Returns the available RAM in MBytes*/
  ::itksysFundamentalType_UInt64 GetAvailableRAMInMBytes() const
  {
    return static_cast< ::itksysFundamentalType_UInt64 >(GetAvailableRAMInBytes() / 1024 / 1024);
  }

protected:
  /** This is protected for the singleton. Use GetInstance() instead. */
  itkNewMacro(Self);
  /** Constructor */
  ConfigurationFile();

  /** Destructor */
  virtual ~ConfigurationFile();
  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:

  /** Try to instanciate a ConfigFile from a path. Return NULL on error */
  ConfigFile * LoadSilent(std::string path);

  /** The instance singleton */
  static Pointer Instance;
  ConfigFile *   m_OTBConfig;
};
} // end namespace
#endif
