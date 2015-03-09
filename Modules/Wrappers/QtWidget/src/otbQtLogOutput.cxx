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

#include<iostream>
#include"otbQtLogOutput.h"


namespace otb
{

/** Constructor */
QtLogOutput::QtLogOutput()
{
}

/** Destructor */
QtLogOutput::~QtLogOutput()
{
}


/** flush a buffer */
void QtLogOutput::Flush()
{
}


/** Write to a buffer */
void QtLogOutput::Write(double timestamp)
{
  std::ostringstream oss;
  oss<<timestamp;
  emit NewContentLog( QString(oss.str().c_str()) );
}


/** Write to a buffer */
void QtLogOutput::Write(std::string const &content)
{
  emit NewContentLog( QString(content.c_str()) );
}


/** Write to a buffer */
void QtLogOutput::Write(std::string const &content, double timestamp)
{
  std::ostringstream oss;
  oss<<timestamp << "  :  " << content;
  emit NewContentLog( QString(oss.str().c_str()) );
}

void QtLogOutput::PrintSelf(std::ostream &os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

}

