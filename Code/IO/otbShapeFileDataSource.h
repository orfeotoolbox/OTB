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

#ifndef __otbShapeFileDataSource_h
#define __otbShapeFileDataSource_h

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "itkProcessObject.h"
#include "itkObjectFactory.h"
#include "itkMacro.h"

#include "otbShapeFileData.h"


namespace otb
{
/** \class ShapeFileDataSource
 * \brief Famille de filtres ayant un object de type shapeFile en sortie
 *
 * 
 * \ingroup IO
 *
 */
 
template <class TOutputShapeFile>
class ITK_EXPORT ShapeFileDataSource : public itk::ProcessObject
{
public :

	/** Standard class typedefs. */
	typedef ShapeFileDataSource								Self;
  typedef itk::ProcessObject								Superclass;
  typedef itk::SmartPointer<Self> 					Pointer;
  typedef itk::SmartPointer<const Self>  		ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ShapeFileDataSource, itk::ProcessObject);
	
	typedef TOutputShapeFile									OutputShapeFileType;
	

  /** Overriding GetOutput() method */
  virtual OutputShapeFileType* GetOutput(void);


protected:
	ShapeFileDataSource();
	~ShapeFileDataSource();

  void PrintSelf(std::ostream& os, itk::Indent indent) const;

	
private:
  ShapeFileDataSource(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

} ;

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbShapeFileDataSource.txx"
#endif

#endif // __otbShapeFileDataSource_h


