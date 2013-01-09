/*=========================================================================

  Program:   Monteverdi2
  Language:  C++


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See Copyright.txt for details.

  Monteverdi2 is distributed under the CeCILL licence version 2. See
  Licence_CeCILL_V2-en.txt or
  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt for more details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __mvdApplication_h
#define __mvdApplication_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi2.h"

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdI18nApplication.h"

//
// External classes pre-declaration.
namespace
{
}

namespace mvd
{
//
// Class declaration.
class AbstractModel;

/** \class Application
 *
 */
class Monteverdi2_EXPORT Application
  : public I18nApplication
{
  Q_OBJECT;

//
// Public methods.
public:

  /** Constructor */
  Application( int& argc, char** argv );

  /** Destructor */
  virtual ~Application();

  /**
   */
  // Method could be inline but it's better not new/delete in inline
  // methods (heap and memory-alignment contexts).
  void SetModel( AbstractModel* model );

 /**
  */
  inline const AbstractModel*
  GetModel() const
  {
    return const_cast< const Application* >( this )->GetModel();
  }

  /**
   */
  inline AbstractModel*
  GetModel()
  {
    return m_Model;
  }

//
// SIGNALS.
signals:
  void selectedModelChanged( const AbstractModel* );

//
// Protected methods.
protected:

//
// Private methods.
private:
  /**
   */
  void InitializeCore();

  /**
   */
  void InitializeModel();

//
// Private attributes.
private:
  AbstractModel* m_Model;

//
// SLOTS
private slots:
  /**
   */
  void OnAboutToQuit();
};

} // end namespace 'mvd'

#endif // __Application_h
