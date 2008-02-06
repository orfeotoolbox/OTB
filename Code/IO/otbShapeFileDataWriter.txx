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

#ifndef __otbShapeFileDataWriter_txx
#define __otbShapeFileDataWriter_txx


#include "otbMacro.h"
#include "otbSystem.h"
#include "otbShapeFileDataWriter.h"

namespace otb
{
/**
 * Constructor
 */
template <class TOutputShapeFile>
ShapeFileDataWriter<TOutputShapeFile>
::ShapeFileDataWriter()
{
  m_FileName = "";
}
/**
 * Destructor
 */
template <class TOutputShapeFile>
ShapeFileDataWriter<TOutputShapeFile>
::~ShapeFileDataWriter()
{
}



/**
 * Main computation method
 */
template <class TOutputShapeFile>
void
ShapeFileDataWriter<TOutputShapeFile>
::GenerateData()
{
	typename InputShapeFileType::Pointer shapeFile = this->GetInput();
	
	OGRRegisterAll();

  OGRDataSource       *poDS;

  poDS = OGRSFDriverRegistrar::Open( m_FileName.c_str(), FALSE );
  if( poDS == NULL )
  {
      itkExceptionMacro(<< "ShapeFile Open failed.\n" );
  }
	
	shapeFile->SetOGRDataSource(poDS);	
}

template <class TOutputShapeFile>
void
ShapeFileDataWriter<TOutputShapeFile>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
	Superclass::PrintSelf(os, indent);
	os << indent << "ShapeFileWriter" << std::endl;
}



} //namespace otb

#endif
