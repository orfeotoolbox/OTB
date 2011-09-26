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
#include "otbWrapperQtWidgetInputImageListParameter.h"

namespace otb
{
namespace Wrapper
{

QtWidgetInputImageListParameter::QtWidgetInputImageListParameter(InputImageListParameter* param, QtWidgetModel* m)
: QtWidgetParameterBase(m),
  m_InputImageListParam(param)/*,
  m_FileSelectionList()*/
{
}

QtWidgetInputImageListParameter::~QtWidgetInputImageListParameter()
{
}

void QtWidgetInputImageListParameter::DoUpdateGUI()
{

}

void QtWidgetInputImageListParameter::DoCreateWidget()
{

  //m_FileSelectionList.clear();
  const unsigned int sp = 2;
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
  addButton->setToolTip("Add a file selector...");
  addButton->setMaximumWidth(addButton->width());
  connect( addButton, SIGNAL(clicked()), this, SLOT(AddFile()) );
  addSupLayout->addWidget(addButton);

  // Supress file button
  QPushButton * supButton = new QPushButton;
  supButton->setText("-");
  supButton->setToolTip("Supress the selected file...");
  supButton->setMaximumWidth(supButton->width());
  connect( supButton, SIGNAL(clicked()), this, SLOT(SupressFile()) );
  addSupLayout->addWidget(supButton);
  buttonLayout->addLayout(addSupLayout);

  // Up file edit
  QPushButton * upButton = new QPushButton;
  upButton->setText("Up");
  upButton->setToolTip("Up the selected file in the list...");
  upButton->setMaximumWidth(upButton->width());
  connect( upButton, SIGNAL(clicked()), this, SLOT(UpFile()) );
  upDownLayout->addWidget(upButton);

  // Down file edit
  QPushButton * downButton = new QPushButton;
  downButton->setText("Down");
  downButton->setToolTip("Down the selected file in the list...");
  downButton->setMaximumWidth(downButton->width());
  connect( downButton, SIGNAL(clicked()), this, SLOT(DownFile()) );
  upDownLayout->addWidget(downButton);
  buttonLayout->addLayout(upDownLayout);

  // Erase file edit
  QPushButton * eraseButton = new QPushButton;
  eraseButton->setText("Erase");
  eraseButton->setToolTip("Erase the selected file of the list...");
  eraseButton->setMaximumWidth(eraseButton->width());
  connect( eraseButton, SIGNAL(clicked()), this, SLOT(EraseFile()) );
  buttonLayout->addWidget(eraseButton);

  QVBoxLayout * fileLayout = new QVBoxLayout();

  QtFileSelectionWidget * fileSelection = new QtFileSelectionWidget();
  fileSelection->setFixedHeight(40);
  fileLayout->addWidget( fileSelection );
/*
  QtFileSelectionWidget * fileSelection1 = new QtFileSelectionWidget();
  fileSelection1->setFixedHeight(40);
  fileLayout->addWidget( fileSelection1 );
  QtFileSelectionWidget * fileSelection2 = new QtFileSelectionWidget();
  fileSelection2->setFixedHeight(40);
  fileLayout->addWidget( fileSelection2 );
  QtFileSelectionWidget * fileSelection3 = new QtFileSelectionWidget();
  fileSelection3->setFixedHeight(40);
  fileLayout->addWidget( fileSelection3 );
  QtFileSelectionWidget * fileSelection4 = new QtFileSelectionWidget();
  fileSelection4->setFixedHeight(40);
  fileLayout->addWidget( fileSelection4 );
  QtFileSelectionWidget * fileSelection5 = new QtFileSelectionWidget();
  fileSelection5->setFixedHeight(40);
  fileLayout->addWidget( fileSelection5 );
  QtFileSelectionWidget * fileSelection6 = new QtFileSelectionWidget();
  fileSelection6->setFixedHeight(40);
  fileLayout->addWidget( fileSelection6 );
*/
  m_FileSelectionList.push_back(fileSelection);

  QGroupBox *mainGroup = new QGroupBox();
  mainGroup->setLayout(fileLayout);
  QScrollArea * scroll = new QScrollArea();
  scroll->setWidget(mainGroup);
  scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);//ScrollBarAlwaysOn);
  scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);//ScrollBarAlwaysOn);

  hLayout->addWidget(scroll);
  hLayout->addLayout(buttonLayout);

  hLayout->addStretch();
 
  this->setLayout(hLayout);

 m_FileLayout = fileLayout;
 m_HLayout = hLayout;
 m_Scroll = scroll;

}

void QtWidgetInputImageListParameter::SelectFile()
{
  QFileDialog fileDialog;
  fileDialog.setConfirmOverwrite(true);
  fileDialog.setFileMode(QFileDialog::ExistingFile);
  fileDialog.setNameFilter("Raster files (*)");

  if (fileDialog.exec())
    {
    //this->SetFileName(fileDialog.selectedFiles().at(0));
    //m_Input->setText(fileDialog.selectedFiles().at(0));
    }
}


void
QtWidgetInputImageListParameter::UpFile()
{
  std::cout<<"upupup"<<std::endl;
}
void
QtWidgetInputImageListParameter::DownFile()
{
 std::cout<<"downdowndown"<<std::endl;
}
void
QtWidgetInputImageListParameter::AddFile()
{
  m_FileLayout = new QVBoxLayout();
  for(unsigned int i=0; i<m_FileSelectionList.size(); i++ )
    {
    m_FileLayout->addWidget( m_FileSelectionList[i] );
    }

  QtFileSelectionWidget * fileSelection = new QtFileSelectionWidget();
  fileSelection->setFixedHeight( 40 ); 
  m_FileLayout->addWidget( fileSelection );
  m_FileSelectionList.push_back(fileSelection);

  QGroupBox *mainGroup = new QGroupBox();
  mainGroup->setLayout(m_FileLayout);
  m_Scroll->setWidget(mainGroup);

  this->update();
}

void
QtWidgetInputImageListParameter::SupressFile()
{
 std::cout<<"supsupsup"<<std::endl;
}
void
QtWidgetInputImageListParameter::EraseFile()
{
  m_FileSelectionList.clear();

  //m_HLayout->removeItem( m_FileLayout );
  //m_HLayout->removeWidget( m_Scroll );
  //m_HLayout->update();

  m_FileLayout = new QVBoxLayout();
  //QtFileSelectionWidget * fileSelection = new QtFileSelectionWidget();
  //QLineEdit * fileSelection = new QLineEdit();
  //fileSelection->setFixedHeight( 40 );
  //fileLayout->addWidget( fileSelection );

  QtFileSelectionWidget * fileSelection = new QtFileSelectionWidget();
  fileSelection->setFixedHeight( 40 ); 
  m_FileLayout->addWidget( fileSelection );
  m_FileSelectionList.push_back(fileSelection);

  QGroupBox *mainGroup = new QGroupBox();
  mainGroup->setLayout(m_FileLayout);
  //m_Scroll = new QScrollArea();
  //m_Scroll->setLayout(m_FileLayout);
  m_Scroll->setWidget(mainGroup);
  //scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);//ScrollBarAlwaysOn);
  //scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);//ScrollBarAlwaysOn)

  //m_HLayout->insertWidget(0, m_Scroll);
  //m_HLayout->update();
  this->update();
  //m_FileLayout = fileLayout;
  //m_Scroll = scroll;
  
 std::cout<<"cleancleanclean"<<std::endl;
}
/*
void QtWidgetInputImageListParameter::SetFileName(const QString& value)
{
  // save value
  m_InputImageParam->SetFromFileName(value.toStdString());

  // notify of value change
  QString key( QString::fromStdString(m_InputImageParam->GetKey()) );
  emit ParameterChanged(key);
}
*/


}
}
