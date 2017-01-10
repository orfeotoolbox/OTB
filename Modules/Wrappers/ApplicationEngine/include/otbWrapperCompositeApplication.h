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
#ifndef otbWrapperCompositeApplication_h
#define otbWrapperCompositeApplication_h

#include "otbWrapperApplication.h"
#include "itkStdStreamLogOutput.h"

namespace otb
{
namespace Wrapper
{

/** \class CompositeApplication
 *  \brief This class is a base class for composite applications
 *
 * This class allows to create & store internal applications with the same logic
 * as parameters. You choose the application type to create, you choose an
 * identifier (alphanumeric string), and you can give a short description.
 * Later, you will refer to this application using the identifier. In the
 * functions of this class, you can refer to parameters from internal
 * applications by using their identifier as prefix. For instance, "app1.in"
 * will refer to parameter "in" from internal application named "app1"
 * (if such application exists, if not it will refer to a parameter of this
 * composite application).
 *
 * \ingroup OTBApplicationEngine
 */
class OTBApplicationEngine_EXPORT CompositeApplication: public Application
{
public:
  /** Standard class typedefs. */
  typedef CompositeApplication          Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** RTTI support */
  itkTypeMacro(CompositeApplication, Application);

  /** Filters typedef */
  typedef itk::MemberCommand< Self >        AddProcessCommandType;

  typedef struct
    {
    Application::Pointer App;
    std::string Desc;
    } InternalApplication;

  typedef std::map<std::string, InternalApplication> InternalAppContainer;

protected:
  /** Constructor */
  CompositeApplication();

  /** Destructor */
  ~CompositeApplication() ITK_OVERRIDE;

  /**
   * Callback function to retrieve the process watchers on internal filters
   */
  void LinkWatchers(itk::Object * itkNotUsed(caller), const itk::EventObject & event);

  /**
   * Method to instanciate and register a new internal application
   * \param appType Type of the application to instanciate
   * \param key Identifier associated to the created application
   * \param desc Description of the internal application
   */
  bool AddApplication(std::string appType, std::string key, std::string desc);

  /**
   * Method to remove all internal applications. Application deriving from
   * CompositeApplication should call this method at the begining of their
   * DoInit().
   */
  void ClearApplications();

  /**
   * Connect two existing parameters together. The first parameter will point to
   * the second parameter.
   */
  bool Connect(std::string fromKey, std::string toKey);

  /**
   * Share a parameter between the composite application and an internal application
   * The local parameter is created as a proxy to the internal parameter.
   * \param localKey New parameter key in the composite application
   * \param internalKey Key to the internal parameter to expose
   * \param name Name for the local parameter, if empty the target's name is used
   * \param desc Description for the local parameter, if empty the target's description is used
   */
  bool ShareParameter(std::string localKey,
                      std::string internalKey,
                      std::string name = std::string(),
                      std::string desc = std::string());

  /**
   * Decode a key to extract potential prefix for internal applications
   * If a valid prefix (corresponding to an internal app) is found:
   *   - prefix is removed from the input key which is altered.
   *   - the function returns a pointer to the internal application
   * If no valid prefix is found, the input key is not modified, and the
   * function returns 'this'.
   */
  Application* DecodeKey(std::string &key);

  /**
   * Get the internal application with the given identifier
   */
  Application* GetInternalApplication(std::string id);

  /**
   * Get the description of an internal application
   */
  std::string GetInternalAppDescription(std::string id);

  /**
   * Utility function to call Execute() on an internal app and get its output logs
   */
  void ExecuteInternal(std::string key);

  /**
   * Utility function to call UpdateParameters() on an internal app
   */
  void UpdateInternalParameters(std::string key);

private:
  CompositeApplication(const CompositeApplication &); //purposely not implemented
  void operator =(const CompositeApplication&); //purposely not implemented

  InternalAppContainer m_AppContainer;

  itk::StdStreamLogOutput::Pointer  m_LogOutput;

  std::ostringstream m_Oss;

  AddProcessCommandType::Pointer    m_AddProcessCommand;
};

}
}
#endif
