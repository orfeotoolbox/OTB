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
#include "otbWrapperQtWidgetInputFilenameListParameter.h"

namespace otb
{
namespace Wrapper
{

QtWidgetInputFilenameListParameter::QtWidgetInputFilenameListParameter(InputFilenameListParameter* param, QtWidgetModel* m)
: QtWidgetParameterBase(param, m),
  m_InputFilenameListParam(param)
{
 connect(this, SIGNAL(Change()), GetModel(), SLOT(NotifyUpdate()));
}

QtWidgetInputFilenameListParameter::~QtWidgetInputFilenameListParameter()
{
}

void QtWidgetInputFilenameListParameter::DoUpdateGUI()
{
  if(!m_InputFilenameListParam)
    return;

  std::vector<std::string> fileList = m_InputFilenameListParam->GetFileNameList();
  for( unsigned int i = m_FileSelectionList.size(); i < fileList.size(); i++ )
    {
      this->AddFile();
    }
  int i = 0;
  std::vector<std::string>::iterator it;
  for (it = fileList.begin(); it != fileList.end(); ++it)
    {
      m_FileSelectionList[i++]->GetInput()->setText(
	QFile::decodeName(
	  it->c_str()
	)
      );
    }
}

void QtWidgetInputFilenameListParameter::DoCreateWidget()
{
  m_FileSelectionList.clear();
  const unsigned int sp(2);
  const unsigned int buttonSize(30);

  // Global layout
  QHBoxLayout * hLayout = new QHBoxLayout;
  hLayout->setSpacing(sp);
  hLayout->setContentsMargins(sp, sp, sp, sp);

  // Button layout
  QVBoxLayout * buttonLayout = new QVBoxLayout;
  buttonLayout->setSpacing(sp);
  buttonLayout->setContentsMargins(sp, sp, sp, sp);

  QHBoxLayout * addSupLayout = new QHBoxLayout;
  addSupLayout->setSpacing(sp);
  addSupLayout->setContentsMargins(sp, sp, sp, sp);

  QHBoxLayout * upDownLayout = new QHBoxLayout;
  upDownLayout->setSpacing(sp);
  upDownLayout->setContentsMargins(sp, sp, sp, sp);

  // Add file button
  QPushButton * addButton = new QPushButton;
  addButton->setText("+");
  addButton->setFixedWidth(buttonSize);
  addButton->setToolTip("Add a file selector...");
  connect(addButton, SIGNAL(clicked()), this, SLOT(AddFile()));
  addSupLayout->addWidget(addButton);

  // Suppress file button
  QPushButton * supButton = new QPushButton;
  supButton->setText("-");
  supButton->setFixedWidth(buttonSize);
  supButton->setToolTip("Suppress the selected file...");
  connect(supButton, SIGNAL(clicked()), this, SLOT(SuppressFile()));
  addSupLayout->addWidget(supButton);
  buttonLayout->addLayout(addSupLayout);

  // Up file edit
  QPushButton * upButton = new QPushButton;
  upButton->setText("Up");
  upButton->setFixedWidth(buttonSize);
  upButton->setToolTip("Up the selected file in the list...");
  connect(upButton, SIGNAL(clicked()), this, SLOT(UpFile()));
  upDownLayout->addWidget(upButton);

  // Down file edit
  QPushButton * downButton = new QPushButton;
  downButton->setText("Down");
  downButton->setFixedWidth(buttonSize);
  downButton->setToolTip("Down the selected file in the list...");
  connect(downButton, SIGNAL(clicked()), this, SLOT(DownFile()));
  upDownLayout->addWidget(downButton);
  buttonLayout->addLayout(upDownLayout);

  // Erase file edit
  QPushButton * eraseButton = new QPushButton;
  eraseButton->setText("Erase");
  eraseButton->setFixedWidth(2*(buttonSize+sp));
  eraseButton->setToolTip("Erase the selected file of the list...");
  connect(eraseButton, SIGNAL(clicked()), this, SLOT(EraseFile()));
  buttonLayout->addWidget(eraseButton);

  QVBoxLayout * fileLayout = new QVBoxLayout();
  fileLayout->setSpacing(0);

  QtFileSelectionWidget * fileSelection = new QtFileSelectionWidget();
  fileSelection->setFixedHeight(30);
  fileLayout->addWidget(fileSelection);
  m_InputFilenameListParam->AddNullElement();
  connect(fileSelection->GetInput(), SIGNAL(textChanged(const QString&)), this, SLOT(UpdateFilenameList()));

  m_FileSelectionList.push_back(fileSelection);

  QGroupBox *mainGroup = new QGroupBox();
  mainGroup->setLayout(fileLayout);
  QScrollArea * s = new QScrollArea();
  s->setWidget(mainGroup);
  s->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  s->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  s->setWidgetResizable(true);

  hLayout->addWidget(s);
  hLayout->addLayout(buttonLayout);

  this->setLayout(hLayout);

  m_FileLayout = fileLayout;
  m_HLayout = hLayout;
  m_Scroll = s;
}

void
QtWidgetInputFilenameListParameter::UpdateFilenameList()
{
  for(unsigned int j = 0; j < m_InputFilenameListParam->GetFileList()->Size(); j++)
    {
    if(m_InputFilenameListParam->SetNthFileName(j, m_FileSelectionList[j]->GetFilename()) == false)
      {
      std::ostringstream oss;
      oss << "The given file " << m_FileSelectionList[j]->GetFilename() << " is not valid.";
      this->GetModel()->SendLogWARNING(oss.str());
      m_FileSelectionList[j]->ClearFilename();
      }
    }

  emit Change();
}


void
QtWidgetInputFilenameListParameter::UpFile()
{
 if(m_FileSelectionList.size() < 2)
    return;

  m_FileLayout = new QVBoxLayout();
  m_FileLayout->setSpacing(2);

  // Map link between old and new index in the list
  std::map<unsigned int, unsigned int> idMap;

  // Init map
  for(unsigned int i = 0; i < m_FileSelectionList.size(); i++)
    {
    idMap[i] = i;
    }

  // If the first item is checked, uncheck it...
  // It won't be moved
  if(m_FileSelectionList[0]->IsChecked())
    {
    m_FileSelectionList[0]->SetChecked(false);
    }


  // If other item are checked, up the index
  // Starts at 1 because the first item mustn't move
  for(unsigned int i = 1; i < m_FileSelectionList.size(); i++)
    {
    if(m_FileSelectionList[i]->IsChecked())
      {
      unsigned int tmp = idMap[i];
      idMap[i] = i-1;
      idMap[idMap[i-1]] = tmp;
      }
    }

  this->UpdateFileList(idMap);

  this->RecreateFilenameList();
}

void
QtWidgetInputFilenameListParameter::DownFile()
{
  if(m_FileSelectionList.size() < 2) return;

  m_FileLayout = new QVBoxLayout();
  m_FileLayout->setSpacing(0);

  // Map link between old and new index in the list
  std::map<unsigned int, unsigned int> idMap;

  // Init map
  for(unsigned int i = 0; i < m_FileSelectionList.size(); i++)
    {
    idMap[i] = i;
    }

  // If the last item is checked, uncheck it...
  // It won't be moved
  if(m_FileSelectionList[m_FileSelectionList.size() - 1]->IsChecked())
    {
    m_FileSelectionList[m_FileSelectionList.size() - 1]->SetChecked(false);
    }

  // If other item are checked, up the index
  // Stops at size-1 because the last item mustn't move
  for(int i = m_FileSelectionList.size() - 2; i >= 0; i--)
    {
    if(m_FileSelectionList[i]->IsChecked())
      {
      unsigned int tmp = idMap[i];
      idMap[i] = i + 1;
      idMap[idMap[i + 1]] = tmp;
      }
    }

  this->UpdateFileList(idMap);

  this->RecreateFilenameList();
}


void
QtWidgetInputFilenameListParameter::UpdateFileList(std::map<unsigned int, unsigned int> idMap)
{
  std::vector<QtFileSelectionWidget *> tmpList;
  // Keys become values and inverse
  std::map<unsigned int, unsigned int> idMapBis;
  for(unsigned int i = 0; i < idMap.size(); i++)
    {
    idMapBis[idMap[i]] = i;
    }

  // Create the new item list
  for(unsigned int i = 0; i < m_FileSelectionList.size(); i++)
    {
    m_FileLayout->addWidget(m_FileSelectionList[idMapBis[i]]);
    tmpList.push_back(m_FileSelectionList[idMapBis[i]]);
    }

  m_FileSelectionList = tmpList;
  QGroupBox *mainGroup = new QGroupBox();
  mainGroup->setLayout(m_FileLayout);
  m_Scroll->setWidget(mainGroup);

  this->update();

  // notify of value change
  QString key(m_InputFilenameListParam->GetKey());
  emit ParameterChanged(key);
}


void
QtWidgetInputFilenameListParameter::AddFile()
{
  m_FileLayout = new QVBoxLayout();
  m_FileLayout->setSpacing(0);

  for(unsigned int i = 0; i < m_FileSelectionList.size(); i++)
    {
    m_FileLayout->addWidget(m_FileSelectionList[i]);
    }

  QtFileSelectionWidget * fileSelection = new QtFileSelectionWidget();
  fileSelection->setFixedHeight(30);
  m_FileLayout->addWidget(fileSelection);
  m_FileSelectionList.push_back(fileSelection);
  m_InputFilenameListParam->AddNullElement();
  connect(fileSelection->GetInput(), SIGNAL(textChanged(const QString&)), this, SLOT(UpdateFilenameList()));

  QGroupBox *mainGroup = new QGroupBox();
  mainGroup->setLayout(m_FileLayout);
  m_Scroll->setWidget(mainGroup);

  this->update();

  emit FileSelectionWidgetAdded( fileSelection );
}

void
QtWidgetInputFilenameListParameter::SuppressFile()
{
  m_FileLayout = new QVBoxLayout();
  m_FileLayout->setSpacing(0);
  std::vector<QtFileSelectionWidget *> tmpList;
  for(unsigned int i = 0; i < m_FileSelectionList.size(); i++)
    {
    if(!m_FileSelectionList[i]->IsChecked())
      {
      m_FileLayout->addWidget(m_FileSelectionList[i]);
      tmpList.push_back(m_FileSelectionList[i]);
      }
    }

  m_FileSelectionList = tmpList;

  QGroupBox *mainGroup = new QGroupBox();
  mainGroup->setLayout(m_FileLayout);
  m_Scroll->setWidget(mainGroup);

  this->update();
  this->RecreateFilenameList();
}


void
QtWidgetInputFilenameListParameter::EraseFile()
{
  m_FileSelectionList.clear();

  m_FileLayout = new QVBoxLayout();

  QtFileSelectionWidget * fileSelection = new QtFileSelectionWidget();
  fileSelection->setFixedHeight(30);
  m_FileLayout->addWidget(fileSelection);
  m_FileSelectionList.push_back(fileSelection);
  m_InputFilenameListParam->AddNullElement();
  connect(fileSelection->GetInput(), SIGNAL(textChanged(const QString&)), this, SLOT(UpdateFilenameList()));

  QGroupBox *mainGroup = new QGroupBox();
  mainGroup->setLayout(m_FileLayout);
  m_Scroll->setWidget(mainGroup);

  this->update();
  this->RecreateFilenameList();
}


void QtWidgetInputFilenameListParameter::RecreateFilenameList()
{
  // save value
  m_InputFilenameListParam->ClearValue();

  if(m_FileSelectionList.size() == 0)
    {
    this->AddFile();
    }
  else
    {
    for(unsigned int j = 0; j < m_FileSelectionList.size(); j++)
      {
      m_InputFilenameListParam->AddFromFileName(m_FileSelectionList[j]->GetFilename());
      connect(m_FileSelectionList[j]->GetInput(), SIGNAL(textChanged(const QString&)), this, SLOT(UpdateFilenameList()));
      }

    emit Change();
    // notify of value change
    QString key(m_InputFilenameListParam->GetKey());
    emit ParameterChanged(key);
    }
}


}
}
