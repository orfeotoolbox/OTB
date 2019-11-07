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

#ifndef mvdQtWidgetParameterInitializers_h
#define mvdQtWidgetParameterInitializers_h

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
#include "otbMacro.h"
#include "otbQtFileSelectionWidget.h"
#include "otbWrapperQtWidgetInputFilenameParameter.h"
#include "otbWrapperQtWidgetInputFilenameListParameter.h"
#include "otbWrapperQtWidgetInputImageParameter.h"
#include "otbWrapperQtWidgetInputImageListParameter.h"
#include "otbWrapperQtWidgetInputVectorDataParameter.h"
#include "otbWrapperQtWidgetInputVectorDataListParameter.h"
#include "otbWrapperQtWidgetOutputFilenameParameter.h"
#include "otbWrapperQtWidgetOutputImageParameter.h"
#include "otbWrapperQtWidgetOutputVectorDataParameter.h"
#include "otbWrapperQtWidgetParameterFactory.h"
#include "otbWrapperQtWidgetListEditWidget.h"

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

namespace Wrapper
{

//
// Internal classes pre-declaration.


/*****************************************************************************/
/* FUNCTIONS DEFINITION SECTION                                              */

/**
 */
template <typename W>
void SetupForFilenameDrop(W* widget);

/**
 */
template <typename W>
void SetupOutputFilename(W* widget, const QDir& dir, const QString& prefix, const QString& extension);

/**
 */
template <typename W>
void SetupOutputFilename(W* widget, const QDir& = QDir::current());

/**
 */
template <typename F>
inline void SetupWidget(QWidget* widget, const F& functor);


/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/**
 * \class FileSelectionInitializer
 *
 * \ingroup OTBMonteverdiGUI
 *
 * \brief WIP.
 */
class FileSelectionInitializer : public std::unary_function<otb::Wrapper::QtFileSelectionWidget*, void>
{
public:
  inline result_type operator()(argument_type widget) const;
};

/**
 * \class InputImageInitializer
 *
 * \ingroup OTBMonteverdiGUI
 *
 * \brief WIP.
 */
class InputImageInitializer : public std::unary_function<otb::Wrapper::QtWidgetInputImageParameter*, void>
{
public:
  inline result_type operator()(argument_type widget) const;
};

/**
 * \class InputImageListInitializer
 *
 * \ingroup OTBMonteverdiGUI
 *
 * \brief WIP.
 */
class InputImageListInitializer : public std::unary_function<otb::Wrapper::QtWidgetInputImageListParameter*, void>
{
public:
  inline result_type operator()(argument_type widget) const;
};

/**
 * \class InputVectorDataInitializer
 *
 * \ingroup OTBMonteverdiGUI
 *
 * \brief WIP.
 */
class InputVectorDataInitializer : public std::unary_function<otb::Wrapper::QtWidgetInputVectorDataParameter*, void>
{
public:
  inline result_type operator()(argument_type widget) const;
};

/**
 * \class InputVectorDataListInitializer
 *
 * \ingroup OTBMonteverdiGUI
 *
 * \brief WIP.
 */
class InputVectorDataListInitializer : public std::unary_function<otb::Wrapper::QtWidgetInputVectorDataListParameter*, void>
{
public:
  inline result_type operator()(argument_type widget) const;
};

/**
 * \class InputFilenameInitializer
 *
 * \ingroup OTBMonteverdiGUI
 *
 * \brief WIP.
 */
class InputFilenameInitializer : public std::unary_function<otb::Wrapper::QtWidgetInputFilenameParameter*, void>
{
public:
  inline result_type operator()(argument_type widget) const;
};

/**
 * \class InputFilenameListInitializer
 *
 * \ingroup OTBMonteverdiGUI
 *
 * \brief WIP.
 */
class InputFilenameListInitializer : public std::unary_function<otb::Wrapper::QtWidgetInputFilenameListParameter*, void>
{
public:
  inline result_type operator()(argument_type widget) const;
};

/**
 * \class OutputImageInitializer
 *
 * \ingroup OTBMonteverdiGUI
 *
 * \brief WIP.
 */
class OutputImageInitializer : public std::unary_function<otb::Wrapper::QtWidgetOutputImageParameter*, void>
{
public:
  inline OutputImageInitializer(const QString& prefix);

  inline result_type operator()(argument_type widget) const;

private:
  QString m_Prefix;
};

/**
 * \class OutputVectorDataInitializer
 *
 * \ingroup OTBMonteverdiGUI
 *
 * \brief WIP.
 */
class OutputVectorDataInitializer : public std::unary_function<otb::Wrapper::QtWidgetOutputVectorDataParameter*, void>
{
public:
  inline result_type operator()(argument_type widget) const;
};

/**
 * \class OutputFilenameInitializer
 *
 * \ingroup OTBMonteverdiGUI
 *
 * \brief WIP.
 */
class OutputFilenameInitializer : public std::unary_function<otb::Wrapper::QtWidgetOutputFilenameParameter*, void>
{
public:
  inline result_type operator()(argument_type widget) const;
};

/**
 * \class ToolTipInitializer
 *
 * \ingroup OTBMonteverdiGUI
 *
 * \brief WIP.
 */
class ToolTipInitializer : public std::unary_function<QWidget*, void>
{
public:
  inline result_type operator()(argument_type widget) const;
};

/**
 * \class ParameterListInitializer
 *
 * \ingroup OTBMonteverdiGUI
 *
 * \brief WIP.
 */
class ParameterListInitializer : public std::unary_function<otb::Wrapper::QtWidgetParameterList*, void>
{
public:
  inline result_type operator()(argument_type widget) const;
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
#include "mvdI18nCoreApplication.h"
#include "mvdFilenameDragAndDropEventFilter.h"

namespace mvd
{

namespace Wrapper
{

/*****************************************************************************/
inline FileSelectionInitializer::result_type FileSelectionInitializer::operator()(argument_type widget) const
{
  assert(widget != NULL);

  SetupForFilenameDrop(widget);
}

/*****************************************************************************/
inline InputImageInitializer::result_type InputImageInitializer::operator()(argument_type widget) const
{
  assert(widget != NULL);

  SetupForFilenameDrop(widget);
}

/*****************************************************************************/
inline InputImageListInitializer::result_type InputImageListInitializer::operator()(argument_type /* widget */) const
{
  // assert( widget!=NULL );

  // Drop support is done by ParameterListInitializer
}

/*****************************************************************************/
inline InputFilenameInitializer::result_type InputFilenameInitializer::operator()(argument_type widget) const
{
  assert(widget != NULL);

  SetupForFilenameDrop(widget);
}

/*****************************************************************************/
inline InputFilenameListInitializer::result_type InputFilenameListInitializer::operator()(argument_type /* widget */) const
{
  // assert( widget!=NULL );

  // Drop support is done by ParameterListInitializer
}

/*****************************************************************************/
inline InputVectorDataInitializer::result_type InputVectorDataInitializer::operator()(argument_type widget) const
{
  assert(widget != NULL);

  SetupForFilenameDrop(widget);
}

/*****************************************************************************/
inline InputVectorDataListInitializer::result_type InputVectorDataListInitializer::operator()(argument_type /* widget */) const
{
  // assert( widget!=NULL );

  // Drop support is done by ParameterListInitializer
}

/*****************************************************************************/
inline ToolTipInitializer::result_type ToolTipInitializer::operator()(argument_type otbUseInDebug(widget)) const
{
#if defined(OTB_DEBUG)
  assert(widget != NULL);

  widget->setToolTip(widget->toolTip() + "\n(" + widget->metaObject()->className() + ")");

#endif // defined( OTB_DEBUG )
}

/*****************************************************************************/
inline OutputImageInitializer::OutputImageInitializer(const QString& prefix) : m_Prefix(prefix)
{
}

/*****************************************************************************/
inline OutputImageInitializer::result_type OutputImageInitializer::operator()(argument_type widget) const
{
  assert(widget != NULL);
  assert(I18nCoreApplication::ConstInstance() != NULL);

  if (m_Prefix.isEmpty())
  {
    SetupForFilenameDrop(widget);

    assert(qApp != NULL);
    assert(!qApp->arguments().empty());

    SetupOutputFilename(widget);
  }
  else
    SetupOutputFilename(widget, I18nCoreApplication::ConstInstance()->GetResultsDir(), m_Prefix, ".tif");
}

/*****************************************************************************/
inline OutputVectorDataInitializer::result_type OutputVectorDataInitializer::operator()(argument_type widget) const
{
  assert(widget != NULL);

  SetupForFilenameDrop(widget);

  assert(qApp != NULL);
  assert(!qApp->arguments().empty());

  SetupOutputFilename(widget);
}

/*****************************************************************************/
inline OutputFilenameInitializer::result_type OutputFilenameInitializer::operator()(argument_type widget) const
{
  assert(widget != NULL);

  SetupForFilenameDrop(widget);

  assert(qApp != NULL);
  assert(!qApp->arguments().empty());

  SetupOutputFilename(widget);
}

/*****************************************************************************/
inline ParameterListInitializer::result_type ParameterListInitializer::operator()(argument_type widget) const
{
  assert(widget != nullptr);

  QWidget* listWidget = widget->layout()->itemAt(0)->widget();

  assert(listWidget);

  otb::Wrapper::ListEditWidget* castListEdit = dynamic_cast<otb::Wrapper::ListEditWidget*>(listWidget);

  assert(castListEdit);

  QObject* eventFilter = new FilenameDragAndDropEventFilter(castListEdit);
  castListEdit->installEventFilter(eventFilter);
  QObject::connect(eventFilter, SIGNAL(FilenameDropped(const QString&)),
                   // to:
                   castListEdit, SLOT(OnFilenameDropped(const QString&)));
}

/*****************************************************************************/
template <typename W>
void SetupForFilenameDrop(W* widget)
{
  assert(widget != NULL);

  QLineEdit* lineEdit = widget->GetInput();

  //
  // Setup widget.
  bool signalsBlocked = lineEdit->blockSignals(true);
  {
    lineEdit->setPlaceholderText(QCoreApplication::translate("mvd::Wrapper::QtWidgetView", "You can drop a file here"));

    // lineEdit->setReadOnly( true );

    lineEdit->setToolTip(lineEdit->toolTip() + "\n" +
                         QCoreApplication::translate("mvd::Wrapper::QtWidgetView", "You can drag filename from file-manager and drop it here."));
  }
  lineEdit->blockSignals(signalsBlocked);


  //
  // Install event-filters.

  QObject* eventFilter = new FilenameDragAndDropEventFilter(lineEdit);

  lineEdit->installEventFilter(eventFilter);

  // BUG : temporary fix for drag & drop in InputImageParameter
  // in the future, all "filename" parameters should have the same behaviour
  if (dynamic_cast<otb::Wrapper::QtWidgetInputImageParameter*>(widget) || dynamic_cast<otb::Wrapper::QtFileSelectionWidget*>(widget))
  {
    QObject::connect(eventFilter, SIGNAL(FilenameDropped(const QString&)),
                     // to:
                     widget, SLOT(SetFileName(const QString&)));
  }
  else
  {
    QObject::connect(eventFilter, SIGNAL(FilenameDropped(const QString&)),
                     // to:
                     lineEdit, SLOT(setText(const QString&)));
  }
}

/*****************************************************************************/
template <typename W>
void SetupOutputFilename(W* widget, const QDir& dir, const QString& prefix, const QString& extension)
{
  if (widget->isEnabled())
  {
    QString id(QUuid::createUuid().toString());

    id.replace(QRegExp("[\\{|\\}]"), "");

    if (prefix != NULL)
      id.prepend("_");

    widget->SetFileName(dir.absoluteFilePath(prefix + id + extension));

    widget->UpdateGUI();
  }
}

/*****************************************************************************/
template <typename W>
void SetupOutputFilename(W* widget, const QDir& dir)
{
  if (widget->isEnabled())
  {
    widget->SetFileName(dir.path());

    widget->UpdateGUI();
  }
}

/*******************************************************************************/
template <typename F>
inline void SetupWidget(QWidget* widget, const F& functor)
{
  typedef typename F::argument_type Widget;
  typedef QList<Widget>             WidgetList;

  WidgetList list(widget->findChildren<Widget>());

  for (typename WidgetList::iterator it(list.begin()); it != list.end(); ++it)
  {
    functor(*it);
  }
}

} // end namespace 'Wrapper'.

} // end namespace 'mvd'

#endif // mvdQtWidgetParameterInitializers_h
