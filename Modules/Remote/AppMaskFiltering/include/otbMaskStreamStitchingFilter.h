/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbMaskStreamStitchingFilter_h
#define otbMaskStreamStitchingFilter_h

#include "otbOGRDataSourceWrapper.h"
#include "otbMacro.h"

//#if(GDAL_VERSION_NUM < 1800)
//#error MaskStreamStitchingFilter requires GDAL version >= 1.8.0
//#endif

#include "itkProgressReporter.h"

#include <algorithm>

namespace otb
{

/** \class MaskStreamStitchingFilter
 *  \brief This filter fusion the geometries in a layer (\c OGRLayer) along streaming lines.
 *  It is a in-line filter which means that the result of the fusion overwrites the input layer.
 *  The strategy for merging polygons is quite simple. A polygon P1 is merge with a polygon P2 if:
 *  - P1 and P2 are on different side of the streaming line
 *  - P1 and P2 intersect each other.
 *  - P2 has the largest intersection with P1 among all other polygons Pi intersecting P1.
 *  The \c SetStreamSize() method allows retrieving the number of streams in row and column,
 *  and their pixel coordinates.
 *  The input image is used to transform pixel coordinates of the streaming lines into
 *  coordinate system of the image, which must be the same as the one in the OGR input file.
 *  This filter is intended to be used after \c StreamingVectorizedSegmentationOGR.
 *  @see Example/StreamingMeanShiftSegmentation.cxx
 *
 *  \ingroup OBIA
 *
 *
 *
 * \ingroup OTBOGRProcessing
 */
template <class TInputImage>
class ITK_EXPORT MaskStreamStitchingFilter :
    public itk::ProcessObject
{
public:

   /** typedef for the classes standards. */
  typedef MaskStreamStitchingFilter        Self;
  typedef itk::ProcessObject                   Superclass;
  typedef itk::SmartPointer<Self>              Pointer;
  typedef itk::SmartPointer<const Self>        ConstPointer;

  /** Definition of the input image */
  typedef TInputImage                          InputImageType;
  typedef typename InputImageType::PixelType   InputPixelType;
  typedef typename InputImageType::IndexType   InputIndexType;
  typedef typename InputImageType::SizeType    SizeType;
  typedef typename InputImageType::RegionType  RegionType;
  typedef typename InputImageType::SpacingType SpacingType;
  typedef typename InputImageType::PointType   OriginType;
  typedef typename InputImageType::IndexType   IndexType;

  typedef ogr::Layer                           OGRLayerType;
  typedef ogr::Feature                         OGRFeatureType;

  /** Set the input image of this process object.  */
  using Superclass::SetInput;
  virtual void SetInput(const InputImageType *input);
  /** Get the input image. */
  virtual const InputImageType * GetInput(void);

  /** Method for management of the object factory. */
  itkNewMacro(Self);

  /** Return the name of the class. */
  itkTypeMacro(MaskStreamStitchingFilter, ProcessObject);

  /** Set the input OGRLayer */
  void SetOGRLayer( const OGRLayerType & ogrLayer );
  /** Get the input OGRLayer */
  const OGRLayerType & GetOGRLayer( void ) const;

  /** Set the stream size.
   * As this filter is intended to be used right after the \c StreamingVectorizedSegmentation,
   * use the \c GetStreamSize() method on it to get the correct stream size.
   */
  itkSetMacro(StreamSize, SizeType);
  /** Get stream size*/
  itkGetMacro(StreamSize, SizeType);

  /** Generate Data method. This method must be called explicitly (not through the \c Update method). */
  void GenerateData() ITK_OVERRIDE;

  /** ------------------------------- */
  class IntersectionGraph
  {
  public:
    struct NodeType
    {
      bool isVisited;
      std::vector<int> adjacencyList;
    };

    typedef std::map<int, NodeType > GraphType;  
        
    IntersectionGraph() {graph.clear();} // Constructor
    ~IntersectionGraph() {}; // Destructor
        
    void registerEdge(int idx1, int idx2)
    {
      if (idx1 != idx2)
      {
        if (graph.find(idx1) == graph.end()) // The node (key in the map) doesn't exist
        {
          NodeType node;
          node.adjacencyList.push_back(idx2);
          node.isVisited = false;
          graph[idx1] = node; 
        }
        else
        {
          graph[idx1].adjacencyList.push_back(idx2);
        }
                  
        if (graph.find(idx2) == graph.end())
        {
          NodeType node;
          node.adjacencyList.push_back(idx1);
          node.isVisited = false;
          graph[idx2] = node; 
        }
        else
        {
          graph[idx2].adjacencyList.push_back(idx1);
        }
      }
      else
      {
        std::cout << "warning : trying to merge polygon " <<idx1 << " with itself" << std::endl;
      }	
    }	
    void printGraph()
    {
      for (typename GraphType::iterator it = graph.begin(); it != graph.end(); it++)
      {
        std::cout << "Node " << (*it).first << " Adj : " ;
        for (std::vector<int>::iterator itAdj = (*it).second.adjacencyList.begin(); itAdj != (*it).second.adjacencyList.end(); itAdj++)
        {
        std::cout << (*itAdj) << " ";
        }
        std::cout << " Visited : " << (*it).second.isVisited;
        std::cout << std::endl;
      }
    }
    
    std::vector< std::vector<int> > findConnectedComponents()
    {
      std::vector< std::vector<int> > fusionList;
      for (typename GraphType::iterator it = graph.begin(); it != graph.end(); it++)
      {
        std::vector<int> fusionIndexes;
        VisitNode( (*it).first, fusionIndexes);
        if (fusionIndexes.empty() != true)
        {
          fusionList.push_back(fusionIndexes);
        }
      }
      return fusionList;
    }
        
        
    void VisitNode(int idx, std::vector<int>& fusionIndexes)
    {
      if (graph[idx].isVisited == false) 
      {
        fusionIndexes.push_back(idx);
        graph.find(idx)->second.isVisited = true;
        for (std::vector<int>::iterator it = graph[idx].adjacencyList.begin(); it != graph[idx].adjacencyList.end(); it++)
        {
          VisitNode( *it,fusionIndexes);
        }
      }
    }	
  private:
    GraphType graph;
  };

protected:
  MaskStreamStitchingFilter();
  ~MaskStreamStitchingFilter() ITK_OVERRIDE {}

  struct FusionStruct
  {
    unsigned int indStream1;
    unsigned int indStream2;
    double overlap;
  };
  struct FeatureStruct
  {
    FeatureStruct(OGRFeatureDefn & defn) : feat(defn), fusioned(false),fusionIndex(0)
    {
    }
    OGRFeatureType feat;
    bool fusioned;
    unsigned int fusionIndex;
  };
  struct SortFeatureStruct
  {
    bool operator() (FusionStruct f1, FusionStruct f2) { return (f1.overlap > f2.overlap); }
  } SortFeature;

  /**
   Main computation method. if line is true process row part, else process column part.
   */
  void ProcessStreamingLine(bool line, itk::ProgressReporter &progress);
  /** get length in case of  OGRGeometryCollection.
   * This function recodes the get_lenght method available since gdal 1.8.0
   * in the case of OGRGeometryCollection. The aim is to allow accessing polygon stiching
   * functionalities with gdal 1.6.
   */
  double GetLengthOGRGeometryCollection(OGRGeometryCollection * intersection);

private:
  MaskStreamStitchingFilter(const Self &);  //purposely not implemented
  void operator =(const Self&);      //purposely not implemented

  SizeType m_StreamSize;
  unsigned int m_Radius;
  OGRLayerType m_OGRLayer;
	int m_count;

};


} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbMaskStreamStitchingFilter.txx"
#endif

#endif
