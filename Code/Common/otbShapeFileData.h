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

#ifndef __otbShapeFileData_h
#define __otbShapeFileData_h

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "itkDataObject.h"
#include "itkObjectFactory.h"
#include "itkMacro.h"
#include "ogrsf_frmts.h"

namespace otb
{
/** \class ShapeFileData
 * \brief Stockage des donnees du format ShapeFile (.shp)
 *
 * 
 * \ingroup IO
 *
 */
	
class ITK_EXPORT ShapeFileData : public itk::DataObject
{
public:

	/** Standard class typedefs. */
	typedef ShapeFileData											Self;
  typedef itk::DataObject  									Superclass;
  typedef itk::SmartPointer<Self> 					Pointer;
  typedef itk::SmartPointer<const Self>  		ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ShapeFileData, itk::DataObject);
	
	itkGetConstMacro(OGRDataSource, OGRDataSource*);
	
	itkSetObjectMacro(OGRDataSource, OGRDataSource);
	
	
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

protected:
	ShapeFileData();
	~ShapeFileData();
	
private:
  ShapeFileData(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
	
	OGRDataSource* m_OGRDataSource;

} ;

} // end namespace otb

#endif // __otbShapeFileData_h

