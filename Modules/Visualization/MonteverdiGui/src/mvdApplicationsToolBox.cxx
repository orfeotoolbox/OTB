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

#include "mvdApplicationsToolBox.h"
#include "ui_mvdApplicationsToolBox.h"


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
#include "mvdAlgorithm.h"

namespace mvd
{
/*
  TRANSLATOR mvd::ApplicationsToolBox

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/


/*****************************************************************************/
/* CONSTANTS                                                                 */

enum COLUMN
{
  COLUMN_NAME = 0,
  //
  COLUMN_COUNT,
};

enum ItemType
{
  ITEM_TYPE_ROOT = 0,
  ITEM_TYPE_TAG,
  ITEM_TYPE_APPLICATION,
  //
  TYPE_COUNT,
};

/*****************************************************************************/
/* STATIC IMPLEMENTATION SECTION                                             */


/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */

/*******************************************************************************/
ApplicationsToolBox::ApplicationsToolBox(QWidget* p, Qt::WindowFlags flags)
  : QWidget(p, flags), m_UI(new mvd::Ui::ApplicationsToolBox()), m_AppTags(), m_SearchText()
{
  m_UI->setupUi(this);

  //
  // connect search box changed
  QObject::connect(m_UI->m_SearchLine, SIGNAL(textChanged(const QString&)), this, SLOT(OnSearchBoxChanged(const QString&)));

  //
  // connect double click on the tree Widget
  QObject::connect(m_UI->m_AlgorithmsTree, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(OnAlgorithmTreeDoubleClick(QTreeWidgetItem*, int)));

// put a place holder text in search area
#if (QT_VERSION >= 0x407000)
  m_UI->m_SearchLine->setPlaceholderText(tr("Search Application ..."));
#endif
}

/*******************************************************************************/
ApplicationsToolBox::~ApplicationsToolBox()
{
  delete m_UI;
  m_UI = NULL;
}

/*******************************************************************************/
// QTreeWidget *
// ApplicationsToolBox
// ::GetAlgorithmsTree()
// {
//   return m_UI->m_AlgorithmsTree;
// }

/*******************************************************************************/
void ApplicationsToolBox::FillTreeUsingTags()
{
  assert(m_UI->m_AlgorithmsTree != NULL);

  //
  // clear algorithms tree
  m_UI->m_AlgorithmsTree->clear();

  if (m_AppTags.empty())
    return;

//
// main item (title)
#if USE_ALGORITHM_ROOT_NODE
  QTreeWidgetItem* mainItem = new QTreeWidgetItem(m_UI->m_AlgorithmsTree, ITEM_TYPE_ROOT);

  mainItem->setText(COLUMN_NAME, tr("Orfeo Toolbox Algorithms"));
  mainItem->setExpanded(!m_SearchText.isEmpty());
  mainItem->setFlags(Qt::NoItemFlags | Qt::ItemIsEnabled);
#endif // USE_ALGORITHME_ROOT_NODE

  QString search(m_SearchText.simplified());

  //
  // iterate on map: key as high-level item / algorithms as lower-level items
  for (ApplicationsTagContainer::const_iterator itTag(m_AppTags.begin()); itTag != m_AppTags.end(); ++itTag)
  {
    //
    // current Doctag name
    QString qcurrentTag((*itTag).first.c_str());

    // If a current tag applicaton name match the searchText, add the
    // tag as an item to the tree
    if (m_SearchText.isEmpty() || IsSearchTextMatchAnyAlgorithm(qcurrentTag, search) || qcurrentTag.contains(search, Qt::CaseInsensitive))
    {
      //
      // step #1 ->  DocTag is a main item
      QTreeWidgetItem* cmainItem = new QTreeWidgetItem(
#if USE_ALGORITHM_ROOT_NODE
          mainItem,
#else  // USE_ALGORITHM_ROOT_NODE
          m_UI->m_AlgorithmsTree,
#endif // USE_ALGORITHM_ROOT_NODE
          ITEM_TYPE_TAG);

      cmainItem->setText(COLUMN_NAME, qcurrentTag);
      cmainItem->setExpanded(!m_SearchText.isEmpty());
      cmainItem->setFlags(Qt::NoItemFlags | Qt::ItemIsEnabled);

      //
      // add category icon
      cmainItem->setIcon(COLUMN_NAME, QIcon(":/icons/otb"));

      //
      // step #2 -> Add algorithms name if matching the search label
      for (StringVector::const_iterator itApps(itTag->second.begin()); itApps != itTag->second.end(); ++itApps)
      {
        // get current app name
        QString name(itApps->c_str());

        assert(!name.isEmpty());

        // does the current algorithm name match the search text
        if (m_SearchText.isEmpty() || name.contains(search, Qt::CaseInsensitive))
        {
          //
          // set current application name as secondary item
          QTreeWidgetItem* secItem = new QTreeWidgetItem(cmainItem, ITEM_TYPE_APPLICATION);

          secItem->setText(COLUMN_NAME, name);
          secItem->setIcon(COLUMN_NAME, QIcon(":/icons/process"));
        }
      }
    }
  }

  // Expand algorithms tree by default
  m_UI->m_AlgorithmsTree->expandAll();

  m_UI->m_AlgorithmsTree->resizeColumnToContents(COLUMN_NAME);
}

/*******************************************************************************/
bool ApplicationsToolBox::IsSearchTextMatchAnyAlgorithm(const QString& tagName, const QString& search)
{
  // find the pair corresponding to the tagName
  ApplicationsTagContainer::const_iterator itTag(m_AppTags.find(ToStdString(tagName)));

  if (itTag == m_AppTags.end())
    return false;

  for (StringVector::const_iterator itApps(itTag->second.begin()); itApps != itTag->second.end(); ++itApps)
  {
    QString name(FromStdString(*itApps));

    if (name.contains(search, Qt::CaseInsensitive))
    {
      return true;
    }
  }

  return false;
}

/*******************************************************************************/
void ApplicationsToolBox::LaunchApplication(const QString& appName)
{
  assert(!appName.isEmpty());

  emit ApplicationToLaunchSelected(appName);
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/
void ApplicationsToolBox::OnAvailableApplicationsTagsChanged(const ApplicationsTagContainer& appsTags)
{
  // rememeber the map
  m_AppTags = appsTags;

  // fill the tree with the application
  FillTreeUsingTags();
}

/*******************************************************************************/
void ApplicationsToolBox::OnSearchBoxChanged(const QString& search)
{
  //
  // get the search text
  m_SearchText = search;

  // fill the tree with the application
  FillTreeUsingTags();
}

/*******************************************************************************/
void ApplicationsToolBox::OnAlgorithmTreeDoubleClick(QTreeWidgetItem* item, int column)
{
  if (item->type() != ITEM_TYPE_APPLICATION)
    return;

  QString text(item->text(column));

  if (text.isEmpty())
    return;

  LaunchApplication(text);
}

/*******************************************************************************/

} // end namespace 'mvd'
