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

  typedef ConfigurationFile             Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkTypeMacro(ConfigurationFile, Object);

  /** Get the unique instanc1e of the model */
  static Pointer GetInstance();

  ConfigFile * GetOTBConfig()
  {
    return m_OTBConfig;
  }

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

protected:
  /** This is protected for the singleton. Use GetInstance() instead. */
  itkNewMacro(Self);
  /** Constructor */
  ConfigurationFile();

  /** Destructor */
  ~ConfigurationFile();
  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;
private:
  /** The instance singleton */
  static Pointer Instance;
  ConfigFile *   m_OTBConfig;
};
} // end namespace
#endif
