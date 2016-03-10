/*=========================================================================

  Program:   Monteverdi
  Language:  C++


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See Copyright.txt for details.

  Monteverdi is distributed under the CeCILL licence version 2. See
  Licence_CeCILL_V2-en.txt or
  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt for more details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __mvdImportImagesDialog_h
#define __mvdImportImagesDialog_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.
#include <QtGui>

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)
#include "otbGDALOverviewsBuilder.h"

//
// Monteverdi includes (sorted by alphabetic order)


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
namespace Ui
{
class ImportImagesDialog;
};


/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/**
 * \class ImportImagesDialog
 *
 * \brief Widget template skeleton to copy-paste when adding a new
 * widget class.
 */
class Monteverdi_EXPORT ImportImagesDialog :
    public QDialog
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

//
// Private types.
public:
  typedef std::vector< otb::GDALOverviewsBuilder::Pointer > GDALOverviewsBuilderVector;

//
// Public methods.
public:

  /** \brief Constructor. */
  ImportImagesDialog( const QStringList & filename,
		      QWidget * parent =NULL,
		      Qt::WindowFlags flags =0 );

  /** \brief Destructor. */
  virtual ~ImportImagesDialog();

  /**
   */
  int GetEffectiveCount() const;

  /**
   */
  const GDALOverviewsBuilderVector & GetGDALOverviewsBuilders() const;

  /*-[ PUBLIC SLOTS SECTION ]------------------------------------------------*/

//
// Public SLOTS.
public slots:

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

//
// Signals.
signals:

  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

//
// Protected methods.
protected:

//
// Protected attributes.
protected:

  /*-[ PRIVATE SECTION ]-----------------------------------------------------*/

//
// Private types
private:

//
// Private methods.
private:
  /**
   */
  void SetFilenames( const QStringList & );
  /**
   */
  void SetHeaders();
  /**
   */
  const otb::GDALOverviewsBuilder::Pointer & GetBuilder( const QModelIndex & ) const;

//
// Private attributes.
private:
  /**
   * \brief uic generated.
   */
  Ui::ImportImagesDialog * m_UI;

  /**
   */
  QPushButton * m_IgnoreButton;

  /**
   */
  GDALOverviewsBuilderVector m_GDALOverviewsBuilders;

  /**
   */
  int m_EffectiveCount;

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

//
// Slots.
private slots:
  void on_buttonBox_clicked( QAbstractButton * );
  /**
   */
  void OnCurrentChanged( const QModelIndex & , const QModelIndex & );

  /**
   */
  void OnItemChanged( QStandardItem * );
};

} // end namespace 'mvd'

/*****************************************************************************/
/* INLINE SECTION                                                            */

namespace mvd
{
} // end namespace 'mvd'

#endif // __mvdImportImagesDialog_h
