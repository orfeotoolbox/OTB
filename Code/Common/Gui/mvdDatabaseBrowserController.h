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
#ifndef __mvdDatabaseBrowserController_h
#define __mvdDatabaseBrowserController_h

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
#include "Gui/mvdAbstractModelController.h"
#include "Gui/mvdGui.h"


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
// Internal classes pre-declaration.
class DatabaseBrowserWidget;
class DatasetModel;

/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/**
 * \class DatabaseBrowserController
 *
 * \brief Controller of database browser widget (for
 * DatabaseModeleModel objects).
 */
class Monteverdi2_EXPORT DatabaseBrowserController :
    public AbstractModelController
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

//
// Public methods.
public:

  /**
   * \brief Constructor.
   *
   * \param widget Controlled widget.
   * \param parent Parent QObject of this QObject.
   */
  DatabaseBrowserController( DatabaseBrowserWidget* widget,
			     QObject* parent =NULL );

  /**
   * \brief Destructor.
   */
  virtual ~DatabaseBrowserController();


  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

//
// Signals.
signals:

  /**
   */
  void SelectedDatasetFilenameChanged(const QString &);

  void ImageToImportDropped(const QString & );

  /*-[ PUBLIC SLOTS SECTION ]------------------------------------------------*/

//
// Slots.
public slots:

  /**
   *  \brief
   */
  void CheckDatasetsConsistency();

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
  void ResetDatasetTree( const StringPairListType& datasets );

  //
  // AbstractModelController overrides.

  virtual void Connect( AbstractModel* );

  virtual void ClearWidget();

  virtual void ResetWidget();

  virtual void Disconnect( AbstractModel* );


//
// Private attributes.
private:

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

//
// Slots.
private slots:

  /**
   * \brief Slot called when the currently selected dataset has changed.
   *
   * \param id The ID of the newly selected dataset.
   * \param previousId The ID of the previously selected dataset.
   */
  void OnCurrentDatasetChanged( const QString& id, const QString& previousId );

  /**
   */
  void OnSelectedDatasetModelChanged( DatasetModel* );

  /**
   */
  void OnImageDropped( const QString & imagefname );

  /**
   */
  void OnDatasetToDeleteSelected( const QString& id );

  //
  // AbstractModelController overloads.

  virtual void RefreshWidget();
};

} // end namespace 'mvd'.

/*****************************************************************************/
/* INLINE SECTION                                                            */

namespace mvd
{
} // end namespace 'mvd'

#endif // __mvdDatabaseBrowserController_h
