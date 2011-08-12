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
#ifndef __otbWrapperEvent_h
#define __otbWrapperEvent_h

namespace otb
{
namespace Wrapper
{

class ITK_EXPORT Event
{
public:
  /** Constructor */
  Event(): m_Type("Unknown"), m_InstanceId("Unknown") {}
  Event(const std::string& type, const std::string& id): m_Type(type), m_InstanceId(id) {}
  virtual ~Event(){}

  void SetType(const std::string& type)
  {
    m_Type = type;
  }

  const std::string& GetType() const
  {
    return m_Type;
  }

  void SetInstanceId(const std::string& id)
  {
    m_InstanceId = id;
  }

  const std::string& GetInstanceId() const
  {
    return m_InstanceId;
  }

private:
  /// Type
  std::string m_Type;

  /// Instance Id
  std::string m_InstanceId;
};

}
}
#endif
