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
#include "otbQtFileSelectionWidget.h"
#include "otbWrapperQtWidgetInputFilenameParameter.h"
#include "otbWrapperQtWidgetInputFilenameListParameter.h"
#include "otbWrapperQtWidgetInputImageParameter.h"
#include "otbWrapperQtWidgetInputImageListParameter.h"
#include "otbWrapperQtWidgetInputVectorDataParameter.h"
#include "otbWrapperQtWidgetInputVectorDataListParameter.h"
#include "otbWrapperQtWidgetOutputImageParameter.h"
#include "otbWrapperQtWidgetParameterFactory.h"


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

/**
 */
template< typename F >
inline void SetupWidget( QWidget * widget, const F& functor );


/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/**
 * \class FileSelectionInitializer
 *
 * \brief WIP.
 */
class FileSelectionInitializer : public std::unary_function<
  otb::Wrapper::QtFileSelectionWidget*,
  void
  >
{
public:
  inline FileSelectionInitializer( bool supportsDataset );
  inline result_type operator () ( argument_type widget ) const;

private:
  bool m_SupportsDataset;
};

/**
 * \class InputImageInitializer
 *
 * \brief WIP.
 */
class InputImageInitializer : public std::unary_function<
  otb::Wrapper::QtWidgetInputImageParameter*,
  void
  >
{
public:
  inline result_type operator () ( argument_type widget ) const;
};

/**
 * \class InputImageListInitializer
 *
 * \brief WIP.
 */
class InputImageListInitializer : public std::unary_function<
  otb::Wrapper::QtWidgetInputImageListParameter*,
  void
  >
{
public:
  inline InputImageListInitializer( QWidget * view );

  inline result_type operator () ( argument_type widget ) const;

private:
  QWidget * m_View;
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
  inline result_type operator () ( argument_type widget ) const;
};

/**
 * \class InputVectorDataListInitializer
 *
 * \brief WIP.
 */
class InputVectorDataListInitializer : public std::unary_function<
  otb::Wrapper::QtWidgetInputVectorDataListParameter*,
  void >
{
public:
  inline InputVectorDataListInitializer( QWidget * view );

  inline result_type operator () ( argument_type widget ) const;

private:
  QWidget * m_View;
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
  inline result_type operator () ( argument_type widget ) const;
};

/**
 * \class InputFilenameListInitializer
 *
 * \brief WIP.
 */
class InputFilenameListInitializer : public std::unary_function<
  otb::Wrapper::QtWidgetInputFilenameListParameter*,
  void >
{
public:
  inline InputFilenameListInitializer( QWidget * view );

  inline result_type operator () ( argument_type widget ) const;

private:
  QWidget * m_View;
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
FileSelectionInitializer
::FileSelectionInitializer( bool supportsDataset ) :
  m_SupportsDataset( supportsDataset )
{
}

/*****************************************************************************/
inline
FileSelectionInitializer::result_type
FileSelectionInitializer
::operator () ( argument_type widget ) const
{
  assert( widget!=NULL );
  if( m_SupportsDataset )
    {
    SetupForFilenameDrop( widget, "You can drop dataset or filename here." );
    SetupForDatasetDrop( widget );
    }
  else
    SetupForFilenameDrop( widget, "You can drop filename here." );   
}

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
InputImageListInitializer
::InputImageListInitializer( QWidget * view ) :
  m_View( view )
{
}

/*****************************************************************************/
inline
InputImageListInitializer::result_type
InputImageListInitializer
::operator () ( argument_type widget ) const
{
  assert( widget!=NULL );

  QObject::connect(
    widget, SIGNAL( FileSelectionWidgetAdded( QWidget * ) ),
    m_View, SLOT( OnFileSelectionWidgetAdded1( QWidget * ) )
  );

  SetupWidget( widget, FileSelectionInitializer( true  ) );            
}

/*****************************************************************************/
inline
InputFilenameInitializer::result_type
InputFilenameInitializer
::operator () ( argument_type widget ) const
{
  assert( widget!=NULL );

  SetupForFilenameDrop( widget, "You can drop filename here." );
}

/*****************************************************************************/
inline
InputFilenameListInitializer
::InputFilenameListInitializer( QWidget * view ) :
  m_View( view )
{
}

/*****************************************************************************/
inline
InputFilenameListInitializer::result_type
InputFilenameListInitializer
::operator () ( argument_type widget ) const
{
  assert( widget!=NULL );

  QObject::connect(
    widget, SIGNAL( FileSelectionWidgetAdded( QWidget * ) ),
    m_View, SLOT( OnFileSelectionWidgetAdded0( QWidget * ) )
  );

  SetupWidget( widget, FileSelectionInitializer( false ) );
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
InputVectorDataListInitializer
::InputVectorDataListInitializer( QWidget * view ) :
  m_View( view )
{
}

/*****************************************************************************/
inline
InputVectorDataListInitializer::result_type
InputVectorDataListInitializer
::operator () ( argument_type widget ) const
{
  assert( widget!=NULL );

  QObject::connect(
    widget, SIGNAL( FileSelectionWidgetAdded( QWidget * ) ),
    m_View, SLOT( OnFileSelectionWidgetAdded0( QWidget * ) )
  );

  SetupWidget( widget, FileSelectionInitializer( false ) );
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

/*******************************************************************************/
template< typename F >
inline
void
SetupWidget( QWidget* widget, const F& functor )
{
  typedef typename F::argument_type Widget;
  typedef QList< Widget > WidgetList;

  WidgetList list( widget->findChildren< Widget >() );
  
  for( typename WidgetList::iterator it( list.begin() );
       it!=list.end();
       ++it )
    {
    functor( *it );
    }
}

} // end namespace 'Wrapper'.

} // end namespace 'mvd'

#endif // __mvdQtWidgetParameterInitializers_h
