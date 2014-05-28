/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbWrapperQtWidgetListViewParameter_h
#define __otbWrapperQtWidgetListViewParameter_h

#include <QtGui>
#include "otbWrapperListViewParameter.h"
#include "otbWrapperParameterGroup.h"
#include "otbWrapperQtWidgetParameterBase.h"

namespace otb
{
namespace Wrapper
{

/** \class
 * \brief
 */
class ITK_ABI_EXPORT QtWidgetListViewParameter : public QtWidgetParameterBase
{
  Q_OBJECT
public:
  QtWidgetListViewParameter(ListViewParameter*, QtWidgetModel*);
  virtual ~QtWidgetListViewParameter();

  std::vector<int>  GetSelectedItems()
  {
    return m_SelectedItems;
  }

protected slots:
  void SelectedItems();

private:
  QtWidgetListViewParameter(const QtWidgetListViewParameter&); //purposely not implemented
  void operator=(const QtWidgetListViewParameter&); //purposely not implemented

  virtual void DoCreateWidget();

  virtual void DoUpdateGUI();

  ListViewParameter::Pointer m_ListViewParam;

  QListWidget*               m_ListView;

  QHBoxLayout*               m_VLayout;

  std::vector<int>           m_SelectedItems;

  int                        m_LineHeight;
};

}
}

#endif
