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
#ifndef __otbTreeSource_h
#define __otbTreeSource_h

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif
#include "itkProcessObject.h"

namespace otb
{

/** \class MeshSource
 *  \brief Base class for all process objects that output mesh data.
 *
 * MeshSource is the base class for all process objects that output
 * mesh data. Specifically, this class defines the GetOutput() method
 * that returns a pointer to the output mesh. The class also defines
 * some internal private data members that are used to manage streaming
 * of data.
 *
 * \ingroup DataSources
 */
template <class TOutputTree>
class ITK_EXPORT TreeSource : public itk::ProcessObject
{
public:
  /** Standard class typedefs. */
  typedef TreeSource                     Self;
  typedef itk::ProcessObject             Superclass;
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;
  
  /** Method for creation through the object factory. */
  itkNewMacro(Self);  

  /** Run-time type information (and related methods). */
  itkTypeMacro(TreeSource,itk::ProcessObject);

  /** Some convenient typedefs. */
  typedef itk::DataObject                       DataObject;
  typedef itk::DataObject::Pointer              DataObjectPointer;
  typedef TOutputTree                           OutputTreeType;
  typedef typename OutputTreeType::Pointer      OutputTreePointer;
  typedef typename OutputTreeType::ConstPointer OutputTreeConstPointer;
  
  /** Get the mesh output of this process object.  */
  OutputTreeType * GetOutput(void);
  OutputTreeType * GetOutput(unsigned int idx);
  
  void SetOutput(TOutputTree *output);

  virtual void GraftOutput(DataObject *output);
  virtual void GraftNthOutput(unsigned int idx, DataObject *output);

  virtual DataObjectPointer MakeOutput(unsigned int idx);

protected:
  TreeSource();
  virtual ~TreeSource() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;
  
  void GenerateInputRequestedRegion();
  
private:
  TreeSource(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** Used by streaming: The requested region of the output being processed
   * by the execute method. Set in the GenerateInputRequestedRegion method. */
  int m_GenerateDataRegion;
  int m_GenerateDataNumberOfRegions;
  };

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbTreeSource.txx"
#endif

#endif
