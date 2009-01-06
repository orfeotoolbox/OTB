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
#ifndef __otbVectorDataProjectionFilter_txx
#define __otbVectorDataProjectionFilter_txx

#include "otbVectorDataProjectionFilter.h"
#include "itkProgressReporter.h"
#include "itkMetaDataObject.h"
#include "otbMetaDataKey.h"
#include "otbGenericMapProjection.h"
#include "itkIdentityTransform.h"
#include "otbInverseSensorModel.h"
#include "otbDataNode.h"

namespace otb
{

/**
   * Constructor
 */
  template <class TInputVectorData, class TOutputVectorData >
      VectorDataProjectionFilter<TInputVectorData,TOutputVectorData>
  ::VectorDataProjectionFilter()
  {
    m_InputProjectionRef.clear();
    m_OutputProjectionRef.clear();
    m_InputKeywordList.clear();
    m_OutputKeywordList.clear();
    m_InputSpacing.Fill(1);
    m_InputOrigin.Fill(0);
  }


//----------------------------------------------------------------------------
  template <class TInputVectorData, class TOutputVectorData >
      void
          VectorDataProjectionFilter<TInputVectorData,TOutputVectorData>
  ::SetInputSpacing(const SpacingType & spacing )
  {
    itkDebugMacro("setting Spacing to " << spacing);
    if( this->m_InputSpacing != spacing )
    {
      this->m_InputSpacing = spacing;
      this->Modified();
    }
  }


//----------------------------------------------------------------------------
  template <class TInputVectorData, class TOutputVectorData >
      void
          VectorDataProjectionFilter<TInputVectorData,TOutputVectorData>
  ::SetInputSpacing(const double spacing[2] )
  {
    SpacingType s(spacing);
    this->SetInputSpacing(s);
  }


//----------------------------------------------------------------------------
  template <class TInputVectorData, class TOutputVectorData >
      void
          VectorDataProjectionFilter<TInputVectorData,TOutputVectorData>
  ::SetInputSpacing(const float spacing[2] )
  {
    itk::Vector<float, 2> sf(spacing);
    SpacingType s;
    s.CastFrom( sf );
    this->SetInputSpacing(s);
  }

//----------------------------------------------------------------------------
  template <class TInputVectorData, class TOutputVectorData >
      void
          VectorDataProjectionFilter<TInputVectorData,TOutputVectorData>
  ::SetInputOrigin(const double origin[2] )
  {
    OriginType p(origin);
    this->SetInputOrigin( p );
  }


//----------------------------------------------------------------------------
  template <class TInputVectorData, class TOutputVectorData >
      void
          VectorDataProjectionFilter<TInputVectorData,TOutputVectorData>
  ::SetInputOrigin(const float origin[2] )
  {
    itk::Point<float, 2> of(origin);
    OriginType p;
    p.CastFrom( of );
    this->SetInputOrigin( p );
  }



  template <class TInputVectorData, class TOutputVectorData >
      void
          VectorDataProjectionFilter<TInputVectorData,TOutputVectorData>
  ::GenerateOutputInformation(void)
  {
    Superclass::GenerateOutputInformation();

    OutputVectorDataPointer output = this->GetOutput();
    itk::MetaDataDictionary & dict = output->GetMetaDataDictionary();

    itk::EncapsulateMetaData<std::string>(dict, MetaDataKey::m_ProjectionRefKey, m_OutputProjectionRef );

  }

  /**
   * Convert line
   */
  template <class TInputVectorData, class TOutputVectorData >
      typename VectorDataProjectionFilter<TInputVectorData,TOutputVectorData>::LinePointerType
          VectorDataProjectionFilter<TInputVectorData,TOutputVectorData>
  ::ReprojectLine(LinePointerType line) const
  {
    typedef typename LineType::VertexListType::ConstPointer VertexListConstPointerType;
    typedef typename LineType::VertexListConstIteratorType VertexListConstIteratorType;
    VertexListConstPointerType  vertexList = line->GetVertexList();
    VertexListConstIteratorType it = vertexList->Begin();
    typename LineType::Pointer newLine = LineType::New();
    itk::Point<double,2> point;
    itk::ContinuousIndex<double,2> index;
    while ( it != vertexList->End())
    {
      typename LineType::VertexType pointCoord = it.Value();
      pointCoord[0] = pointCoord[0] * m_InputSpacing[0] + m_InputOrigin[0];
      pointCoord[1] = pointCoord[1] * m_InputSpacing[1] + m_InputOrigin[1];
      point = m_Transform->TransformPoint(pointCoord);
      index[0]=point[0];
      index[1]=point[1];
      otbMsgDevMacro(<< "Converting: " << it.Value() << " -> " << index);
      newLine->AddVertex(index);
      it++;
    }

    return newLine;
  }

  /**
   * Instanciate the transformation according to informations
   */
  template <class TInputVectorData, class TOutputVectorData >
      void
          VectorDataProjectionFilter<TInputVectorData,TOutputVectorData>
  ::InstanciateTransform(void)
  {
    InputVectorDataPointer input = this->GetInput();
    OutputVectorDataPointer output = this->GetOutput();
    const itk::MetaDataDictionary & inputDict = input->GetMetaDataDictionary();
    itk::MetaDataDictionary & outputDict = output->GetMetaDataDictionary();

    //If the information were not specified by the user, it is filled from the metadata
    if (m_InputKeywordList.getSize()  == 0)
    {
      itk::ExposeMetaData<ossimKeywordlist>(inputDict, MetaDataKey::m_OSSIMKeywordlistKey, m_InputKeywordList );
    }
    if (m_InputProjectionRef.empty())
    {
      itk::ExposeMetaData<std::string>(inputDict, MetaDataKey::m_ProjectionRefKey, m_InputProjectionRef );
    }


    //*****************************
    //Set the input transformation
    //*****************************
    if (m_InputKeywordList.getSize()  > 0)
    {
      typedef otb::InverseSensorModel<double> InverseSensorModelType;
      InverseSensorModelType* sensorModel = InverseSensorModelType::New();
      sensorModel->SetImageGeometry(m_InputKeywordList);
      if ( !m_DEMDirectory.empty())
      {
        sensorModel->SetDEMDirectory(m_DEMDirectory);
      }
      m_InputTransform = sensorModel;
      otbMsgDevMacro(<< "Input projection set to sensor model");
    }


    if ((m_InputTransform.IsNull()) && ( !m_InputProjectionRef.empty() ))//map projection
    {
      typedef otb::GenericMapProjection<otb::INVERSE> InverseMapProjectionType;
      InverseMapProjectionType::Pointer mapTransformSmartPointer = InverseMapProjectionType::New();
      InverseMapProjectionType* mapTransform = mapTransformSmartPointer.GetPointer();
      mapTransform->SetWkt(m_InputProjectionRef);
      if (mapTransform->GetMapProjection() != NULL)
      {
        m_InputTransform = mapTransform;
        otbMsgDevMacro(<< "Input projection set to map trasform");
      }

    }

    if(m_InputTransform.IsNull())//default if we didn't manage to instantiate it before
    {
      m_InputTransform = itk::IdentityTransform< double, 2 >::New();
      otbMsgDevMacro(<< "Input projection set to identity")
    }

    //*****************************
    //Set the output transformation
    //*****************************
    if (m_OutputKeywordList.getSize()  > 0)
    {
      typedef otb::InverseSensorModel<double> InverseSensorModelType;
      InverseSensorModelType* sensorModel = InverseSensorModelType::New();
      sensorModel->SetImageGeometry(m_OutputKeywordList);
      if ( !m_DEMDirectory.empty())
      {
        sensorModel->SetDEMDirectory(m_DEMDirectory);
      }
      m_OutputTransform = sensorModel;
      otbMsgDevMacro(<< "Output projection set to sensor model");
    }


    if ((m_OutputTransform.IsNull()) && ( !m_OutputProjectionRef.empty() ))//map projection
    {
      typedef otb::GenericMapProjection<otb::INVERSE> InverseMapProjectionType;
      InverseMapProjectionType::Pointer mapTransformSmartPointer = InverseMapProjectionType::New();
      InverseMapProjectionType* mapTransform = mapTransformSmartPointer.GetPointer();
      mapTransform->SetWkt(m_OutputProjectionRef);
      if (mapTransform->GetMapProjection() != NULL)
      {
        m_OutputTransform = mapTransform;
        otbMsgDevMacro(<< "Output projection set to map trasform");
      }

    }

    if(m_OutputTransform.IsNull())//default if we didn't manage to instantiate it before
    {
      m_OutputTransform = itk::IdentityTransform< double, 2 >::New();
      otbMsgDevMacro(<< "Output projection set to identity")
    }


    m_Transform = InternalTransformType::New();
    m_Transform->SetFirstTransform(m_InputTransform);
    m_Transform->SetSecondTransform(m_OutputTransform);

  }


/**
   * GenerateData Performs the coordinate convertion for each element in the tree
 */
  template <class TInputVectorData, class TOutputVectorData >
      void
          VectorDataProjectionFilter<TInputVectorData,TOutputVectorData>
  ::GenerateData(void)
  {
    this->AllocateOutputs();
    InputVectorDataPointer inputPtr = this->GetInput();
    OutputVectorDataPointer outputPtr = this->GetOutput();

    //Instanciate the transform
    this->InstanciateTransform();

    itk::ProgressReporter progress(this, 0, inputPtr->Size());

    InputTreeIteratorType it(inputPtr->GetDataTree());
    OutputDataTreePointerType tree = outputPtr->GetDataTree();
    if (tree->GetRoot() == NULL)
    {
      itkExceptionMacro(<<"Data tree is empty: Root == NULL");
    }
    OutputDataNodePointerType currentContainer = tree->GetRoot()->Get();

    while(!it.IsAtEnd())//FIXME this VectorData tree processing would better be in a generic class
    {
      InputDataNodePointerType dataNode = it.Get();
      OutputDataNodePointerType newDataNode = OutputDataNodeType::New();
      newDataNode->SetNodeType(dataNode->GetNodeType());
      newDataNode->SetNodeId(dataNode->GetNodeId());
      switch(dataNode->GetNodeType())
      {
        case ROOT:
        {
          //do nothing
          break;
        }
        case DOCUMENT:
        {
          tree->Add(newDataNode,currentContainer);
          break;
        }
        case FOLDER:
        {
          tree->Add(newDataNode,currentContainer);
          break;
        }
        case FEATURE_POINT:
        {
          break;
        }
        case FEATURE_LINE:
        {
          newDataNode->SetLine(this->ReprojectLine(dataNode->GetLine()));
          break;
        }
        case FEATURE_POLYGON:
        {
          break;
        }
        case FEATURE_MULTIPOINT:
        {
          break;
        }
        case FEATURE_MULTILINE:
        {
          break;
        }
        case FEATURE_MULTIPOLYGON:
        {
          break;
        }
        case FEATURE_COLLECTION:
        {
          break;
        }
      }
      progress.CompletedPixel();
      ++it;
    }

  }

} // end namespace otb

#endif
