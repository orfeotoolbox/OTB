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

#ifndef _otbPathListSource_h
#define _otbPathListSource_h

#include "itkPathSource.h"

#include <list>
#include <vector>

namespace otb
{
  
/** \class PathListSource
 *  \brief Classe de base de tous les traitements ayant pour sortie des données PathList.
 *
 * PathListSource est la classe de base pour tous traitements ayant pour 
 * données du PathList. Cette classe défini la méthode du GetOutput()
 * qui retourne un pointeur à la sortie du PathList.
 *
 * \ingroup DataSources
 * \ingroup Paths
 * \ingroup Lists
 */

template <class TOutputPath >
class ITK_EXPORT PathListSource : public itk::PathSource< TOutputPath >
{
public:
  /** Standard class typedefs. */
  typedef PathListSource                 Self;
  typedef itk::PathSource<TOutputPath>   Superclass;
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;
    
  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(PathListSource,itk::PathSource);
  
  /** Some convenient typedefs. */
  
  typedef TOutputPath                                   	OutputPathType;
  typedef typename OutputPathType::Pointer                      OutputPathPointerType;
  typedef std::vector< OutputPathPointerType >   		OutputPathListType;

  typedef OutputPathListType *                         OutputPathListPointerType;
  typedef const OutputPathListType *                   OutputPathListConstPointerType;
  
  OutputPathListType * GetOutput(void)
  {
  	OutputPathListType * ptr = ( & (this->m_ListPath) );
	return (ptr);
  }
  
//  virtual DataObjectPointer MakeOutput(unsigned int idx);

protected:
  PathListSource();
  virtual ~PathListSource() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;
  
//  OutputPathListPointer m_ListPath;
  OutputPathListType m_ListPath;
    
private:
  PathListSource(const Self&); //purposely not implemented
  void operator=(const Self&);   //purposely not implemented
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbPathListSource.txx"
#endif

#endif
