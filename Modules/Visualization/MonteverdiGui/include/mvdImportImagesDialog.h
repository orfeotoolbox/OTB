/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#ifndef mvdImportImagesDialog_h
#define mvdImportImagesDialog_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.
#include <QtWidgets>

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)
#include "otbGDALOverviewsBuilder.h"
#include "OTBMonteverdiGUIExport.h"
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
 * \ingroup OTBMonteverdiGUI
 *
 * \brief Widget template skeleton to copy-paste when adding a new
 * widget class.
 */
class OTBMonteverdiGUI_EXPORT ImportImagesDialog :
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
		      QWidget * p =NULL,
		      Qt::WindowFlags flags =0 );

  /** \brief Destructor. */
  ~ImportImagesDialog() override;

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
  const otb::GDALOverviewsBuilder::Pointer & GetBuilder( const QModelIndex & ) const;
  /**
   */
  void SetFilenames( const QStringList & );
  /**
   */
  void SetHeaders();
  /**
   */
  void UpdateFileSize( size_t row );

//
// Private attributes.
private:
  /**
   * \brief uic generated.
   */
  Ui::ImportImagesDialog * m_UI;

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

  void on_pyramidWidget_BaseValueChanged( int );
  void on_pyramidWidget_LevelsValueChanged( int );
  void on_pyramidWidget_SizeValueChanged( int );

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

#endif // mvdImportImagesDialog_h
