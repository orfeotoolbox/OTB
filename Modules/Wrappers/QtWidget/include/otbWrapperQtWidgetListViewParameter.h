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
#ifndef otbWrapperQtWidgetListViewParameter_h
#define otbWrapperQtWidgetListViewParameter_h

#include <QtGui>
#ifndef Q_MOC_RUN  // See: https://bugreports.qt-project.org/browse/QTBUG-22829  //tag=QT4-boost-compatibility
#include "otbWrapperListViewParameter.h"
#include "otbWrapperQtWidgetParameterBase.h"
#endif //tag=QT4-boost-compatibility

namespace otb
{
namespace Wrapper
{

/** \class QtWidgetListViewParameter
 * \brief
 *
 * \ingroup OTBQtWidget
 */
class OTBQtWidget_EXPORT QtWidgetListViewParameter : public QtWidgetParameterBase
{
  Q_OBJECT
public:
  QtWidgetListViewParameter(ListViewParameter*, QtWidgetModel*);
  ~QtWidgetListViewParameter() ITK_OVERRIDE;

  std::vector<int>  GetSelectedItems()
  {
    return m_SelectedItems;
  }

protected slots:
  void SelectedItems();

private:
  QtWidgetListViewParameter(const QtWidgetListViewParameter&); //purposely not implemented
  void operator=(const QtWidgetListViewParameter&); //purposely not implemented

  void DoCreateWidget() ITK_OVERRIDE;

  void DoUpdateGUI() ITK_OVERRIDE;

  ListViewParameter::Pointer m_ListViewParam;

  QListWidget*               m_ListView;

  QHBoxLayout*               m_VLayout;

  std::vector<int>           m_SelectedItems;

  int                        m_LineHeight;
};

}
}

#endif
