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
#ifndef __mvdQtWidgetParameterInitializers_h
#define __mvdQtWidgetParameterInitializers_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi2.h"

#define USE_OTB_QT_WIDGET_PARAMETER_FACTORY 1


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
#include "otbWrapperQtWidgetInputFilenameParameter.h"
#include "otbWrapperQtWidgetInputVectorDataParameter.h"
#include "otbWrapperQtWidgetOutputImageParameter.h"

#if USE_OTB_QT_WIDGET_PARAMETER_FACTORY
#include "otbWrapperQtWidgetInputImageParameter.h"
#include "otbWrapperQtWidgetInputVectorDataParameter.h"
#include "otbWrapperQtWidgetParameterFactory.h"
#endif

//
// Monteverdi includes (sorted by alphabetic order)

#if !USE_OTB_QT_WIDGET_PARAMETER_FACTORY
#include "mvdWrapperQtWidgetInputImageParameter.h"
#include "mvdWrapperQtWidgetParameterFactory.h"
#endif


/*****************************************************************************/
/* PRE-DECLARATION SECTION                                                   */

//
// External classes pre-declaration.
namespace
{
}

namespace mvd
{

namespace Wrapper
{

//
// Internal classes pre-declaration.


/*****************************************************************************/
/* FUNCTIONS DEFINITION SECTION                                              */

/**
 */
template< typename W >
void
SetupForFilenameDrop( W* widget, const char* text =NULL );

/**
 */
template< typename W >
void
SetupForDatasetDrop( W* widget, const char* text =NULL );

/**
 */
template< typename W >
void
SetupOutputFilename( W* widget,
                     const QDir& dir,
                     const QString& prefix,
                     const QString& extension );


/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/**
 * \class InputImageInitializer
 *
 * \brief WIP.
 */
class InputImageInitializer : public std::unary_function<
#if USE_OTB_QT_WIDGET_PARAMETER_FACTORY
  otb::Wrapper::QtWidgetInputImageParameter*,
#else
  QtWidgetInputImageParameter*,
#endif
  void
  >
{
public:
  InputImageInitializer() {}

  virtual ~InputImageInitializer() {}

  inline result_type operator () ( argument_type widget ) const;
};

/**
 * \class InputVectorDataInitializer
 *
 * \brief WIP.
 */
class InputVectorDataInitializer : public std::unary_function<
  otb::Wrapper::QtWidgetInputVectorDataParameter*,
  void >
{
public:
  InputVectorDataInitializer() {}

  virtual ~InputVectorDataInitializer() {}

  inline result_type operator () ( argument_type widget ) const;
};

/**
 * \class InputFilenameInitializer
 *
 * \brief WIP.
 */
class InputFilenameInitializer : public std::unary_function<
  otb::Wrapper::QtWidgetInputFilenameParameter*,
  void >
{
public:
  InputFilenameInitializer() {}

  virtual ~InputFilenameInitializer() {}

  inline result_type operator () ( argument_type widget ) const;
};

/**
 * \class OutputImageInitializer
 *
 * \brief WIP.
 */
class OutputImageInitializer : public std::unary_function<
  otb::Wrapper::QtWidgetOutputImageParameter*,
  void
  >
{
public:
  inline OutputImageInitializer( const QString& prefix );

  virtual ~OutputImageInitializer() {}

  inline result_type operator () ( argument_type widget ) const;

private:
  QString m_Prefix;
};

/**
 * \class ToolTipInitializer
 *
 * \brief WIP.
 */
class ToolTipInitializer : public std::unary_function< QWidget*, void >
{
public:
  ToolTipInitializer() {}

  virtual ~ToolTipInitializer() {}

  inline result_type operator () ( argument_type widget ) const;
};

} // end namespace 'Wrapper'.

} // end namespace 'mvd'.

/*****************************************************************************/
/* INLINE SECTION                                                            */

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
#include "Core/mvdDatabaseModel.h"
#include "Core/mvdI18nCoreApplication.h"
#include "Gui/mvdDatasetDragAndDropEventFilter.h"
#include "Gui/mvdFilenameDragAndDropEventFilter.h"

namespace mvd
{

namespace Wrapper
{

/*****************************************************************************/
inline
InputImageInitializer::result_type
InputImageInitializer
::operator () ( argument_type widget ) const
{
  assert( widget!=NULL );

  SetupForFilenameDrop( widget, "You can drop dataset or filename here." );
  SetupForDatasetDrop( widget );
}

/*****************************************************************************/
inline
InputFilenameInitializer::result_type
InputFilenameInitializer
::operator () ( argument_type widget ) const
{
  assert( widget!=NULL );

  SetupForFilenameDrop( widget );
}

/*****************************************************************************/
inline
InputVectorDataInitializer::result_type
InputVectorDataInitializer
::operator () ( argument_type widget ) const
{
  assert( widget!=NULL );

  SetupForFilenameDrop( widget );
}

/*****************************************************************************/
inline
ToolTipInitializer::result_type
ToolTipInitializer
::operator () ( argument_type widget ) const
{
  assert( widget!=NULL );

#if defined( _DEBUG )

  widget->setToolTip(
    widget->toolTip() +
    "\n(" +
    widget->metaObject()->className() +
    ")"
  );

#endif // defined( _DEBUG )
}

/*****************************************************************************/
inline
OutputImageInitializer
::OutputImageInitializer( const QString& prefix ) :
  m_Prefix( prefix )
{
}

/*****************************************************************************/
inline
OutputImageInitializer::result_type
OutputImageInitializer
::operator () ( argument_type widget ) const
{
  assert( widget!=NULL );
  assert( I18nCoreApplication::ConstInstance()!=NULL );

  SetupOutputFilename(
    widget,
    I18nCoreApplication::ConstInstance()->GetResultsDir(),
    m_Prefix,
    ".tif"
  );
}

/*****************************************************************************/
template< typename W >
void
SetupForFilenameDrop( W* widget, const char* text )
{
  assert( widget!=NULL );

  QLineEdit* lineEdit = widget->GetInput();

  //
  // Setup widget.
#if USE_OTB_QT_WIDGET_PARAMETER_FACTORY

  bool signalsBlocked = lineEdit->blockSignals( true );
  {
  if( text!=NULL )
    {
    lineEdit->setText(
      QCoreApplication::translate(
        "mvd::Wrapper::QtWidgetView",
        text
      )
    );
    }

  // lineEdit->setReadOnly( true );

  lineEdit->setToolTip(
    lineEdit->toolTip() +
    "\n" +
    QCoreApplication::translate(
      "mvd::Wrapper::QtWidgetView",
      "You can drag filename from file-manager and drop it here."
    )
  );
  }
  lineEdit->blockSignals( signalsBlocked );

#endif // USE_OTB_QT_WIDGET_PARAMETER_FACTORY

  //
  // Install event-filters.

  QObject* eventFilter = new FilenameDragAndDropEventFilter( lineEdit );

  lineEdit->installEventFilter( eventFilter );

  QObject::connect(
    eventFilter,
    SIGNAL( FilenameDropped( const QString& ) ),
    // to:
    lineEdit,
    SLOT( setText( const QString& ) )
  );
}

/*****************************************************************************/
template< typename W >
void
SetupForDatasetDrop( W* widget, const char* text )
{
  assert( widget!=NULL );

  QLineEdit* lineEdit = widget->GetInput();

  //
  // Setup widget.
#if USE_OTB_QT_WIDGET_PARAMETER_FACTORY

  bool signalsBlocked = lineEdit->blockSignals( true );
  {
  if( text!=NULL )
    {
    lineEdit->setText(
      QCoreApplication::translate(
        "mvd::Wrapper::QtWidgetView",
        text
      )
    );
    }

  // lineEdit->setReadOnly( true );

  lineEdit->setToolTip(
    lineEdit->toolTip() +
    "\n" +
    QCoreApplication::translate(
      "mvd::Wrapper::QtWidgetView",
      "You can drag dataset from dataset-browser and drop it here."
    )
  );
  }
  lineEdit->blockSignals( signalsBlocked );

#endif // USE_OTB_QT_WIDGET_PARAMETER_FACTORY

  //
  // Install event-filters.

  assert(
    I18nCoreApplication::ConstInstance()->GetModel()==
    I18nCoreApplication::ConstInstance()->GetModel< DatabaseModel >()
  );

  QObject* eventFilter =
    new DatasetDragAndDropEventFilter(
      I18nCoreApplication::Instance()->GetModel< DatabaseModel >(),
      lineEdit
    );

  lineEdit->installEventFilter( eventFilter );

  QObject::connect(
    eventFilter,
    SIGNAL( ImageFilenameDropped( const QString& ) ),
    // to:
    lineEdit,
    SLOT( setText( const QString& ) )
  );
}

/*****************************************************************************/
template< typename W >
void
SetupOutputFilename( W* widget,
                     const QDir& dir,
                     const QString& prefix,
                     const QString& extension )
{
  QString id( QUuid::createUuid().toString() );

  id.replace( QRegExp( "[\\{|\\}]" ), "" );

  if( prefix!=NULL )
    id.prepend( "_" );

  widget->SetFileName(
    dir.absoluteFilePath( prefix + id + extension )
  );

  widget->UpdateGUI();
}

} // end namespace 'Wrapper'.

} // end namespace 'mvd'

#endif // __mvdQtWidgetParameterInitializers_h
