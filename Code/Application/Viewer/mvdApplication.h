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


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

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


/*****************************************************************************/
/* PRE-DECLARATION SECTION                                                   */

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
class DatasetModel;

/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/** \class Application
 *
 */
class Monteverdi2_EXPORT Application
  : public I18nApplication
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

//
// Public constants.
public:
  static const char* DATASET_EXT;

//
// Public methods.
public:

  /**
   * \brief Constructor.
   *
   * \param argc Command-line argument count in the argv array.
   * \param argv Array of command-argument (whitespace-separated) strings.
   */
  Application( int& argc, char** argv );

  /** \brief Destructor. */
  virtual ~Application();

 /**
  */
  inline const AbstractModel* GetModel() const;

  /**
   */
  inline AbstractModel* GetModel();

 /**
  */
  template< typename TModel >
  inline const TModel* GetModel() const;

  /**
   */
  template< typename TModel >
  inline TModel* GetModel();

  /**
   *
   */
  bool HasSettingsFile();

  /**
   *
   */
  void ReadCacheDirFromSettings();

  /**
    *
    */
  void WriteCacheDirIntoSettings();

  //
  // STATIC METHODS.

  /**
   * \brief Get the singleton instance of application as an
   * Application pointer.
   *
   * \return The singleton instance of Application.
   */
  inline static Application* Instance();

  /**
   * \brief Get the singleton constant instance of application as an
   * Application pointer.
   *
   * \return The singleton constant instance of I18nApplication.
   */
  inline static const Application* ConstInstance();

  /**
   */
  static
    void DatasetPathName( QString& path,
			  QString& name,
			  const QString& imageFilename );

  /**
   */
  static
    DatasetModel* LoadDatasetModel( const QString& imageFilename,
				     int width,
				     int height );

             
public slots:
  /**
   */
  // Method could be inline but it's better not new/delete in inline
  // methods (heap and memory-alignment contexts).
  void SetModel( AbstractModel* model );
  
  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

//
// Signals.
signals:
  /**
   */
  void AboutToChangeSelectedModel( const AbstractModel* );

  /**
   */
  void SelectedModelChanged( AbstractModel* );

  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

//
// Protected methods.
protected:

//
// Protected attributes.
protected:

  /*-[ PRIVATE SECTION ]-----------------------------------------------------*/

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

  /**
   */
  AbstractModel* m_Model;

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

//
// Slots
private slots:

  /** */
  void OnAboutToQuit();
};

/*****************************************************************************/
/* INLINE SECTION                                                            */

/*****************************************************************************/
inline
Application*
Application
::Instance()
{
  return I18nApplication::Instance< Application >();
}

/*****************************************************************************/
inline
const Application*
Application
::ConstInstance()
{
  return I18nApplication::ConstInstance< Application >();
}

/*****************************************************************************/
inline
const AbstractModel*
Application
::GetModel() const
{
  return m_Model;
}

/*****************************************************************************/
inline
AbstractModel*
Application
::GetModel()
{
  return m_Model;
}

/*****************************************************************************/
template< typename TModel >
inline
const TModel*
Application
::GetModel() const
{
  return qobject_cast< const TModel* >( m_Model );
}

/*****************************************************************************/
template< typename TModel >
inline
TModel*
Application
::GetModel()
{
  return qobject_cast< TModel* >( m_Model );
}

/*****************************************************************************/

} // end namespace 'mvd'

#endif // __Application_h
