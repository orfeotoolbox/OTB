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
#ifndef __otbVectorDataProjectionFilter_h
#define __otbVectorDataProjectionFilter_h

#include "otbVectorDataToVectorDataFilter.h"
#include "itkTransform.h"
#include "otbCompositeTransform.h"
#include "itkPreOrderTreeIterator.h"

namespace otb
{

/** \class VectorDataProjectionFilter
   * \brief Reproject vector data in a different coordinate system
   *
   *
   * \ingroup VectorDataFilter
 */

  template <class TInputVectorData, class TOutputVectorData>
      class ITK_EXPORT VectorDataProjectionFilter :
          public otb::VectorDataToVectorDataFilter<TInputVectorData,TOutputVectorData>
  {

    public:
      /** Standard class typedefs. */
      typedef VectorDataProjectionFilter  Self;
      typedef otb::VectorDataToVectorDataFilter<TInputVectorData,TOutputVectorData>  Superclass;
      typedef itk::SmartPointer<Self>   Pointer;
      typedef itk::SmartPointer<const Self>  ConstPointer;

      typedef TInputVectorData InputVectorDataType;
      typedef TOutputVectorData OutputVectorDataType;
      typedef typename TInputVectorData::ConstPointer InputVectorDataPointer;
      typedef typename TOutputVectorData::Pointer OutputVectorDataPointer;

      /** Some typedefs. */
      typedef itk::Transform<double, 2, 2> GenericTransformType;
      typedef typename GenericTransformType::Pointer GenericTransformPointerType;
      typedef otb::CompositeTransform<GenericTransformType, GenericTransformType, double, 2, 2> InternalTransformType;
      typedef typename InternalTransformType::Pointer InternalTransformPointerType;

      typedef itk::Vector<double, 2> SpacingType;
      typedef itk::Point<double, 2> OriginType;

      typedef itk::PreOrderTreeIterator<typename InputVectorDataType::DataTreeType>       InputTreeIteratorType;
      typedef typename InputVectorDataType::DataNodePointerType InputDataNodePointerType;
      typedef typename OutputVectorDataType::DataNodeType OutputDataNodeType;
      typedef typename OutputVectorDataType::DataNodePointerType OutputDataNodePointerType;
      typedef typename OutputVectorDataType::DataTreePointerType OutputDataTreePointerType;

      typedef typename OutputDataNodeType::LineType LineType;
      typedef typename OutputDataNodeType::LineConstPointerType LineConstPointerType;
      typedef typename OutputDataNodeType::LinePointerType LinePointerType;


      /** Method for creation through the object factory. */
      itkNewMacro(Self);

      /** Run-time type information (and related methods). */
      itkTypeMacro(VectorDataProjectionFilter, VectorDataToVectorDataFilter);

      /** Set/Get for input and output projections.
       Note that there is not Set for the input projection which is specified
       by the input data itself
      */
      itkGetStringMacro(InputProjectionRef);

      itkSetStringMacro(OutputProjectionRef);
      itkGetStringMacro(OutputProjectionRef);

      itkSetStringMacro(DEMDirectory);
      itkGetStringMacro(DEMDirectory);

       /** Set the origin of the vector data.
        * \sa GetOrigin() */
      itkSetMacro(InputOrigin, OriginType);
      virtual void SetInputOrigin( const double origin[2] );
      virtual void SetInputOrigin( const float origin[2] );

      itkGetConstReferenceMacro(InputOrigin, OriginType);


       /** Set the spacing (size of a pixel) of the vector data.
        * \sa GetSpacing() */
      virtual void SetInputSpacing (const SpacingType & spacing);
      virtual void SetInputSpacing (const double spacing[2]);
      virtual void SetInputSpacing (const float spacing[2]);

      itkGetConstReferenceMacro(InputSpacing, SpacingType);

    protected:
      VectorDataProjectionFilter();
      virtual ~VectorDataProjectionFilter() {};

      LinePointerType ReprojectLine(LinePointerType line) const;

      void InstanciateTransform(void);

      void GenerateOutputInformation(void);
      void GenerateData(void);

    private:
      VectorDataProjectionFilter(const Self&); //purposely not implemented
      void operator=(const Self&); //purposely not implemented

      InternalTransformPointerType m_Transform;
      GenericTransformPointerType m_InputTransform;
      GenericTransformPointerType m_OutputTransform;
      std::string m_InputProjectionRef;
      std::string m_OutputProjectionRef;
      ossimKeywordlist m_InputKeywordList;
      ossimKeywordlist m_OutputKeywordList;
      std::string m_DEMDirectory;

      SpacingType         m_InputSpacing;
      OriginType          m_InputOrigin;

  };

} // end namespace otb

#ifndef ITK_MANUAL_INSTANTIATION
#include "otbVectorDataProjectionFilter.txx"
#endif

#endif
