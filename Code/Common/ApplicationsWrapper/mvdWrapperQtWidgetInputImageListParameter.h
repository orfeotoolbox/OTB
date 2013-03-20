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
#ifndef __mvdWrapperQtWidgetInputImageListParameter_h
#define __mvdWrapperQtWidgetInputImageListParameter_h

/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.
#include <QtCore>

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)
#include "otbWrapperQtWidgetParameterBase.h"
#include "otbWrapperInputImageListParameter.h"

//
// Monteverdi includes (sorted by alphabetic order)


/*****************************************************************************/
/* PRE-DECLARATION SECTION                                                   */

//
// External classes pre-declaration.
namespace
{}

namespace mvd
{
namespace Wrapper
{

class QtWidgetFileSelection;

/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/**
 * \class QtWidgetInputImageListParameter
 *
 * \brief WIP.
 */
class QtWidgetInputImageListParameter : public otb::Wrapper::QtWidgetParameterBase
{
  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT
  
  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

//
// Public methods.
public:

  /** \brief Constructor. */
  QtWidgetInputImageListParameter(otb::Wrapper::InputImageListParameter*, 
                                  otb::Wrapper::QtWidgetModel*);

  /** \brief Destructor. */
  virtual ~QtWidgetInputImageListParameter();


signals:
  void Change();

protected slots:
  virtual void UpFile();
  virtual void DownFile();
  virtual void AddFile();
  virtual void SupressFile();
  virtual void EraseFile();
  virtual void UpdateImageList();

private:
  QtWidgetInputImageListParameter(const QtWidgetInputImageListParameter&); //purposely not implemented
  void operator=(const QtWidgetInputImageListParameter&); //purposely not implemented

  virtual void DoCreateWidget();

  virtual void DoUpdateGUI();

  void RecreateImageList();
  void UpdateFileList( std::map<unsigned int, unsigned int> idMap );

  otb::Wrapper::InputImageListParameter::Pointer m_InputImageListParam;

  QHBoxLayout * m_HLayout;
  QVBoxLayout * m_FileLayout;
  QScrollArea * m_Scroll;
  std::vector< QtWidgetFileSelection*> m_FileSelectionList;
};


}
}

#endif
