/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbWrapperExpParameter_h
#define otbWrapperExpParameter_h

#include <memory>

// #include "itkMacro.h"
#include "otbMacro.h"

#include "otbWrapperExpTypes.h"
#include "OTBApplicationEngineExport.h"
#include "boost/core/noncopyable.hpp"

namespace otb
{
namespace WrapperExp
{

#define SetMacro(name, type)                 \
  virtual void Set##name (const type _arg)      \
    {                                           \
      m_##name = _arg;                    \
    }

#define GetMacro(name, type)       \
  virtual type Get##name () const        \
    {                                      \
    return m_##name;               \
    }

#define GetConstRefMacro(name, type)               \
  virtual const type & Get##name () const                \
    {                                              \
    return m_##name;                       \
    }

/** \class Parameter
 *  \brief This class represent a parameter for the wrapper framework
 *  This class is a high level class representing a parameter for the
 *  wrapper framework. It should be subclassed to represent different
 *  kinds of parameters
 *
 * \ingroup OTBApplicationEngine
 */
class OTBApplicationEngine_EXPORT Parameter
  : private boost::noncopyable
{
public:
  /** Standard class typedef */
  typedef Parameter                    Self;
  typedef std::shared_ptr<Self>        Pointer;
  typedef std::shared_ptr<const Self>  ConstPointer;

  /** RTTI support */
  itkTypeMacroNoParent( Parameter );

  /** Set the parameter name */
  // itkSetStringMacro( Name );
  void SetName( const std::string & name )
    {
    m_Name = name;
    }

  /** Get the parameter name */
  GetConstRefMacro( Name , std::string );

  /** Set the parameter description */
  // itkSetStringMacro( Description );
  void SetDescription( const std::string & description )
    {
    m_Description = description;
    }

  /** Get the parameter description */
  GetConstRefMacro( Description , std::string );

  /** Set the parameter key */
  // itkSetStringMacro( Key );
  void SetKey( const std::string & key )
    {
    m_Key = key;
    }

  /** Get the parameter key */
  GetConstRefMacro( Key , std::string );

  /** Set the parameter Active flag */
  SetMacro( Active , bool );

  /** Get the parameter Active flag */
  bool GetActive( bool recurseParents = false ) const ;

  /** Set the parameter mandatory flag */
  SetMacro( Mandatory , bool );

  /** Get the parameter mandatory flag */
  GetMacro( Mandatory , bool );

  /** Toogle the parameter mandatory flag */
  itkBooleanMacro( Mandatory );

  /** Set the parameter uservalue flag */
  SetMacro( UserValueFlag , bool );

  /** Get the parameter uservalue flag */
  GetMacro( UserValueFlag , bool );

  /** Toogle the parameter uservalue flag */
  itkBooleanMacro( UserValueFlag );

  /** Set the user access level */
  SetMacro( UserLevel , UserLevel );

  /** Get the user access level */
  GetMacro( UserLevel , UserLevel) ;

  /** Set the parameter io type*/
  SetMacro( Role , Role );

  /** Get the user access level */
  GetMacro( Role , Role );

  /** Reset to the the default value. Default implementation does
   * nothing
   */
  virtual void Reset() {};

  /** Check wether the parameter has a value.
   */
  virtual bool HasValue() const 
  {
    return !m_Value.empty();
  }

  /** Check wether the parameter has a value and if the value has been set
   * by user.
   */
  virtual bool HasUserValue() const
  {
    return ( HasValue() && m_UserValueFlag ) ;
  }

  virtual void ClearValue()
  {
    SetActive( false );
  }

  /** Set/Get the root of the current parameter (direct parent) */
  virtual void SetRoot( Pointer  root)
  {
    m_Root = root;
  }

  virtual const Pointer GetRoot() const
  {
    return m_Root;
  }

  /** Is the parameter a root or a child of another param */
  virtual bool IsRoot() const
  {
    return !m_Root;
  }

  virtual std::string GetValue( int i = -1 ) const = 0;

  virtual void SetValue( const std::string & val , int i = 0 ) = 0;

  virtual ~Parameter() = default ;
  
protected:
  /** Constructor */
  Parameter() ;

  /** Destructor */

  /** Name of the parameter */
  std::string                   m_Name;

  /** Description of the parameter */
  std::string                   m_Description;

  /** Key of the parameter */
  std::string                   m_Key;

  /** True if the parameter is mandatory */
  bool                          m_Mandatory;

  /** True if activated (a mandatory parameter is always active) */
  bool                          m_Active;

  /** True if the value is set in user mode (otherwise, it is an automatic value)*/
  bool                          m_UserValueFlag;

  UserLevel                     m_UserLevel;

  /** Default iotype mode */
  Role                          m_Role;

  // There might be no need for m_root if parameter group is a map...
  /** List of parents Parameters */
  Pointer                       m_Root;

  std::string                   m_Value;

}; // End class Parameter

} // End namespace WrapperExp
} // End namespace otb

#endif
