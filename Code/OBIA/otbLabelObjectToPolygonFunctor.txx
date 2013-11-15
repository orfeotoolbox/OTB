
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
#ifndef __otbLabelObjectToPolygonFunctor_txx
#define __otbLabelObjectToPolygonFunctor_txx


#include "otbLabelObjectToPolygonFunctor.h"

namespace otb
{
namespace Functor
{

template<class TLabelObject, class TPolygon>
bool
LabelObjectToPolygonFunctor<TLabelObject, TPolygon>
::LexicographicalLineCompare(const LineType&  l1, const LineType& l2)
{
  bool resp = l2.GetIndex()[1] > l1.GetIndex()[1];
  resp = resp
         || (l2.GetIndex()[1] == l1.GetIndex()[1]
             && (l1.GetIndex()[0] + static_cast<long>(l1.GetLength())) < l2.GetIndex()[0]);
  return resp;
}

template<class TLabelObject, class TPolygon>
inline typename LabelObjectToPolygonFunctor<TLabelObject, TPolygon>
::PolygonType *
LabelObjectToPolygonFunctor<TLabelObject, TPolygon>
::operator() (LabelObjectType * labelObject)
  {
  // Clear any previous context
  m_CurrentState = DOWN_LEFT;
  m_PositionFlag = LEFT_END;
  m_InternalDataSet.clear();

  m_Polygon = PolygonType::New();

  // Get the internal container
  //LineContainerType lcontainer = labelObject->GetLineContainer();

  // Sort it
  //sort(lcontainer.begin(), lcontainer.end(), &LexicographicalLineCompare);
  //Use the Optimize method
  labelObject->Optimize();
  // Step 1: Fill the internal data set

  // Iterates on the line container
  // typename LineContainerType::const_iterator lIt = lcontainer.begin();
  ConstLineIteratorType lIt = ConstLineIteratorType(labelObject);
  lIt.GoToBegin();
  // Stores the current line
  long currentLine = lIt.GetLine().GetIndex()[1];
  long lineIndex = 0;

  // Push back the first line of runs
  RunsPerLineType firstRunsPerLine;
  m_InternalDataSet.push_back(firstRunsPerLine);

  while ( !lIt.IsAtEnd() )
    {
    // if we are still in the same image line
    if (currentLine == lIt.GetLine().GetIndex()[1])
      {
      m_InternalDataSet.back().push_back(lIt.GetLine());
      }
    else
      {
      ++lineIndex;
      currentLine = lIt.GetLine().GetIndex()[1];
      RunsPerLineType newRunsPerLine;
      newRunsPerLine.push_back(lIt.GetLine());
      m_InternalDataSet.push_back(newRunsPerLine);
      }
    ++lIt;
    }

  // Begin the boundaries tracking procedure
  m_StartingPoint = m_InternalDataSet.at(0).at(0).GetIndex();
  m_LineOffset = m_StartingPoint[1];
  m_CurrentPoint = m_StartingPoint;
  m_CurrentRun.Fill(0);
  m_CurrentLine = 0;

  bool goesOn = true;

  while (goesOn)
    {
    switch (m_CurrentState)
      {
      case UP_RIGHT:
        {
        // First candidate run
        IndexType firstCandidateRun = Within(m_CurrentPoint, m_CurrentLine - 1);
        if (IsRunIndexValid(firstCandidateRun))
          {
          // Second candidate run
          IndexType secondCandidateRun = RightMostLeftEndInside(m_CurrentLine, RightEnd(
                                                                  m_CurrentRun), firstCandidateRun);
          if (IsRunIndexValid(secondCandidateRun))
            {
            // Down-Right case
            m_CurrentRun = secondCandidateRun;
            m_CurrentState = DOWN_RIGHT;
            m_PositionFlag = LEFT_END;
            WalkRight(m_CurrentLine - 1, m_CurrentPoint, LeftEnd(m_CurrentRun), m_Polygon, m_CurrentState);

            }
          else
            {
            // Up-Right case
            m_CurrentLine--;
            m_CurrentRun = firstCandidateRun;
            m_CurrentState = UP_RIGHT;
            m_PositionFlag = RIGHT_END;
            WalkRight(m_CurrentLine, m_CurrentPoint, RightEnd(m_CurrentRun), m_Polygon, m_CurrentState);

            }
          }
        else
          {
          IndexType secondCandidateRun = LeftMostRightEndInside(m_CurrentLine - 1, RightEnd(m_CurrentRun), m_CurrentRun);

          if (IsRunIndexValid(secondCandidateRun))
            {
            // Up-Left Case
            m_CurrentState = UP_LEFT;
            m_PositionFlag = RIGHT_END;
            WalkLeft(m_CurrentLine, m_CurrentPoint, RightEnd(secondCandidateRun), m_Polygon, m_CurrentState);
            m_CurrentLine--;
            m_CurrentRun = secondCandidateRun;
            }
          else
            {
            // Down-Left case
            m_CurrentState = DOWN_LEFT;
            m_PositionFlag = LEFT_END;
            WalkLeft(m_CurrentLine, m_CurrentPoint, LeftEnd(m_CurrentRun), m_Polygon, m_CurrentState);
            }
          }
        break;
        }
      case DOWN_LEFT:
        {
        // First candidate run
        IndexType firstCandidateRun = Within(m_CurrentPoint, m_CurrentLine + 1);

        if (IsRunIndexValid(firstCandidateRun))
          {
          // Second candidate run
          IndexType secondCandidateRun = LeftMostRightEndInside(m_CurrentLine, LeftEnd(m_CurrentRun), firstCandidateRun);

          if (IsRunIndexValid(secondCandidateRun))
            {
            // Up-Left case
            m_CurrentRun = secondCandidateRun;
            m_CurrentState = UP_LEFT;
            m_PositionFlag = RIGHT_END;
            WalkLeft(m_CurrentLine + 1, m_CurrentPoint, RightEnd(m_CurrentRun), m_Polygon, m_CurrentState);
            }
          else
            {
            // Down-Left case
            m_CurrentLine++;
            m_CurrentRun = firstCandidateRun;
            m_CurrentState = DOWN_LEFT;
            m_PositionFlag = LEFT_END;
            WalkLeft(m_CurrentLine, m_CurrentPoint, LeftEnd(m_CurrentRun), m_Polygon, m_CurrentState);

            }
          }
        else
          {
          IndexType secondCandidateRun = RightMostLeftEndInside(m_CurrentLine + 1, LeftEnd(m_CurrentRun), m_CurrentRun);

          if (IsRunIndexValid(secondCandidateRun))
            {
            // Down-Right case
            m_CurrentRun = secondCandidateRun;
            m_CurrentLine++;
            m_CurrentState = DOWN_RIGHT;
            m_PositionFlag = LEFT_END;
            WalkRight(m_CurrentLine, m_CurrentPoint, LeftEnd(m_CurrentRun), m_Polygon, m_CurrentState);

            }
          else
            {
            // Up-Right case
            m_CurrentState = UP_RIGHT;
            m_PositionFlag = RIGHT_END;
            WalkRight(m_CurrentLine, m_CurrentPoint, RightEnd(m_CurrentRun), m_Polygon, m_CurrentState);

            }
          }
        break;
        }
      case DOWN_RIGHT:
        {
        IndexType firstCandidateRun = Within(m_CurrentPoint, m_CurrentLine + 1);

        if (IsRunIndexValid(firstCandidateRun))
          {
          IndexType secondCandidateRun = LeftMostRightEndInside(m_CurrentLine, RightEnd(
                                                                  m_CurrentRun), firstCandidateRun);

          if (IsRunIndexValid(secondCandidateRun))
            {
            // Up-Left case
            m_CurrentState = UP_LEFT;
            m_PositionFlag = RIGHT_END;
            m_CurrentRun = secondCandidateRun;
            WalkLeft(m_CurrentLine + 1, m_CurrentPoint, RightEnd(m_CurrentRun), m_Polygon, m_CurrentState);

            }
          else
            {
            // Down-Left case
            m_CurrentLine++;
            m_CurrentRun = firstCandidateRun;
            m_CurrentState = DOWN_LEFT;
            m_PositionFlag = LEFT_END;
            WalkLeft(m_CurrentLine, m_CurrentPoint, LeftEnd(m_CurrentRun), m_Polygon, m_CurrentState);

            }
          }
        else
          {
          IndexType secondCandidateRun = RightMostLeftEndInside(m_CurrentLine + 1, LeftEnd(m_CurrentRun), m_CurrentRun);

          if (IsRunIndexValid(secondCandidateRun))
            {
            // Down-Right case
            m_CurrentLine++;
            m_CurrentRun = secondCandidateRun;
            m_CurrentState = DOWN_RIGHT;
            m_PositionFlag = LEFT_END;
            WalkRight(m_CurrentLine, m_CurrentPoint, LeftEnd(m_CurrentRun), m_Polygon, m_CurrentState);
            }
          else
            {
            // Up-Right case
            m_CurrentState = UP_RIGHT;
            m_PositionFlag = RIGHT_END;
            WalkRight(m_CurrentLine, m_CurrentPoint, RightEnd(m_CurrentRun), m_Polygon, m_CurrentState);

            }
          }
        break;
        }
      case UP_LEFT:
        {
        IndexType firstCandidateRun = Within(m_CurrentPoint, m_CurrentLine - 1);

        if (IsRunIndexValid(firstCandidateRun))
          {
          IndexType secondCandidateRun = RightMostLeftEndInside(m_CurrentLine, LeftEnd(m_CurrentRun), firstCandidateRun);

          if (IsRunIndexValid(secondCandidateRun))
            {
            // Down-Right case
            m_CurrentRun = secondCandidateRun;
            m_CurrentState = DOWN_RIGHT;
            m_PositionFlag = LEFT_END;
            WalkRight(m_CurrentLine - 1, m_CurrentPoint, LeftEnd(m_CurrentRun), m_Polygon, m_CurrentState);

            }
          else
            {
            // Up-Right case
            m_CurrentLine--;
            m_CurrentRun = firstCandidateRun;
            m_CurrentState = UP_RIGHT;
            m_PositionFlag = RIGHT_END;
            WalkRight(m_CurrentLine, m_CurrentPoint, RightEnd(m_CurrentRun), m_Polygon, m_CurrentState);
            }
          }
        else
          {
          IndexType secondCandidateRun = LeftMostRightEndInside(m_CurrentLine - 1, RightEnd(m_CurrentRun), m_CurrentRun);

          if (IsRunIndexValid(secondCandidateRun))
            {
            // Up-Left case
            m_CurrentState = UP_LEFT;
            m_PositionFlag = RIGHT_END;
            WalkLeft(m_CurrentLine, m_CurrentPoint, RightEnd(secondCandidateRun), m_Polygon, m_CurrentState);
            m_CurrentLine--;
            m_CurrentRun = secondCandidateRun;

            }
          else
            {
            // Down-Left case
            m_CurrentState = DOWN_LEFT;
            m_PositionFlag = LEFT_END;
            WalkLeft(m_CurrentLine, m_CurrentPoint, LeftEnd(m_CurrentRun), m_Polygon, m_CurrentState);

            }
          }
        break;
        }
      }
    goesOn = m_CurrentPoint != m_StartingPoint;
    }

  return m_Polygon;
  }

template<class TLabelObject, class TPolygon>
inline bool
LabelObjectToPolygonFunctor<TLabelObject, TPolygon>
::IsRunIndexValid(const IndexType& index) const
{
  return (index[0] >= 0 && index[1] >= 0);
}

template<class TLabelObject, class TPolygon>
inline typename LabelObjectToPolygonFunctor<TLabelObject, TPolygon>
::IndexType
LabelObjectToPolygonFunctor<TLabelObject, TPolygon>
::Within(const IndexType& point, unsigned int line) const
{
  unsigned int idx = 0;
  IndexType    resp;
  resp[0] = -1;

  if (line >= m_InternalDataSet.size())
    {
    resp[1] = -1;
    return resp;
    }

  long leftoffset = 0;
  long rightoffset = 0;

  switch (m_PositionFlag)
    {
    case LEFT_END:
      rightoffset = 1;
      break;

    case RIGHT_END:
      leftoffset = -1;
      break;
    }

  typename RunsPerLineType::const_iterator it = m_InternalDataSet.at(line).begin();
  resp[1] = line;

  while (resp[0] < 0 && it != m_InternalDataSet.at(line).end())
    {
    if (point[0] >= (it->GetIndex()[0]) + leftoffset && point[0] <
        (it->GetIndex()[0] + static_cast<long>(it->GetLength())) + rightoffset)
      {
      resp[0] = idx;
      }
    ++idx;
    ++it;
    }
  return resp;
}

template<class TLabelObject, class TPolygon>
inline typename LabelObjectToPolygonFunctor<TLabelObject, TPolygon>
::IndexType
LabelObjectToPolygonFunctor<TLabelObject, TPolygon>
::LeftEnd(const IndexType& runIndex) const
{
  return m_InternalDataSet.at(runIndex[1]).at(runIndex[0]).GetIndex();
}

template<class TLabelObject, class TPolygon>
inline typename LabelObjectToPolygonFunctor<TLabelObject, TPolygon>
::IndexType
LabelObjectToPolygonFunctor<TLabelObject, TPolygon>
::RightEnd(const IndexType& runIndex) const
{
  IndexType point = m_InternalDataSet.at(runIndex[1]).at(runIndex[0]).GetIndex();
  point[0] += static_cast<long>(m_InternalDataSet.at(runIndex[1]).at(runIndex[0]).GetLength() - 1);
  return point;
}

template<class TLabelObject, class TPolygon>
inline typename LabelObjectToPolygonFunctor<TLabelObject, TPolygon>
::IndexType
LabelObjectToPolygonFunctor<TLabelObject, TPolygon>
::RightMostLeftEndInside(unsigned int line, const IndexType& point, const IndexType& run) const
{
  unsigned int idx = 0;
  IndexType    resp;
  resp[0] = -1;

  if (line >= m_InternalDataSet.size())
    {
    resp[1] = -1;
    return resp;
    }

  typename RunsPerLineType::const_iterator it = m_InternalDataSet.at(line).begin();
  resp[1] = line;

  LineType lrun = m_InternalDataSet.at(run[1]).at(run[0]);

  while (resp[0] < 0 && it != m_InternalDataSet.at(line).end())
    {
    /// Index is a the right of point
    if (it->GetIndex()[0] > point[0]
        /// Index is inside run range
        && it->GetIndex()[0] - 1 >= lrun.GetIndex()[0]
        && it->GetIndex()[0] - 1 < lrun.GetIndex()[0] + static_cast<long>(lrun.GetLength()))
      {
      resp[0] = idx;
      }
    ++idx;
    ++it;
    }
  return resp;
}

template<class TLabelObject, class TPolygon>
inline typename LabelObjectToPolygonFunctor<TLabelObject, TPolygon>
::IndexType
LabelObjectToPolygonFunctor<TLabelObject, TPolygon>
::LeftMostRightEndInside(unsigned int line, const IndexType& point, const IndexType& run) const
{
  IndexType resp;
  resp[0] = -1;

  if (line >= m_InternalDataSet.size())
    {
    resp[1] = -1;
    return resp;
    }
  unsigned int idx = m_InternalDataSet.at(line).size() - 1;

  typename RunsPerLineType::const_reverse_iterator it = m_InternalDataSet.at(line).rbegin();
  resp[1] = line;

  LineType lrun = m_InternalDataSet.at(run[1]).at(run[0]);

  while (resp[0] < 0 && it != m_InternalDataSet.at(line).rend())
    {
    /// Index is a the left of point
    if (it->GetIndex()[0] + static_cast<long>(it->GetLength()) <= point[0]
        /// right end is inside run range
        && it->GetIndex()[0] + static_cast<long>(it->GetLength()) >= lrun.GetIndex()[0]
        && it->GetIndex()[0] + static_cast<long>(it->GetLength()) < lrun.GetIndex()[0] +
        static_cast<long>(lrun.GetLength()))
      {
      resp[0] = idx;
      }
    --idx;
    ++it;
    }
  return resp;
}

template<class TLabelObject, class TPolygon>
void
LabelObjectToPolygonFunctor<TLabelObject, TPolygon>
::WalkLeft(unsigned int line,
           const IndexType& startPoint,
           const IndexType& endPoint,
           PolygonType * polygon,
           const StateType state)
{
   if( vcl_abs(static_cast<long int>(line + m_LineOffset - endPoint[1])) > 1 )
     {
       itkExceptionMacro("End point not with +/-1 line from line")
     }

  typename PolygonType::VertexType::VectorType offset;

  switch (state)
    {
    case UP_RIGHT:
      offset[1] = 1;
      break;
    case UP_LEFT:
      offset[1] = 1;
      break;
    case DOWN_RIGHT:
      offset[1] = 0;
      break;
    case DOWN_LEFT:
      offset[1] = 0;
      break;
    }

  switch (m_PositionFlag)
    {
    case LEFT_END:
      offset[0] = 0;
      break;
    case RIGHT_END:
      offset[0] = 1;
      break;
    }
  typename PolygonType::VertexType newPoint;

  m_CurrentPoint = startPoint;
  m_CurrentPoint[0] -= 1;

  if (m_CurrentPoint[0] > endPoint[0] + 1)
    {
    m_CurrentPoint[1] = line + m_LineOffset;
    newPoint = m_CurrentPoint;
    newPoint += offset;
    polygon->AddVertex(IndexToPoint(newPoint));
    }

  // if the end point is not on line n, add an intermediate point
  if (static_cast<int>(line + m_LineOffset) != endPoint[1] && m_CurrentPoint[0] > endPoint[0] + 1)
    {
    m_CurrentPoint[0] = endPoint[0] + 1;
    newPoint = m_CurrentPoint;
    newPoint += offset;
    polygon->AddVertex(IndexToPoint(newPoint));
    }

  if (m_CurrentPoint != endPoint)
    {
    m_CurrentPoint = endPoint;
    newPoint = m_CurrentPoint;
    newPoint += offset;
    polygon->AddVertex(IndexToPoint(newPoint));

    }
}

template<class TLabelObject, class TPolygon>
void
LabelObjectToPolygonFunctor<TLabelObject, TPolygon>
::WalkRight(unsigned int line,
            const IndexType& startPoint,
            const IndexType& endPoint,
            PolygonType * polygon,
            const StateType state)
{

   if( vcl_abs(static_cast<long int>(line + m_LineOffset - endPoint[1])) > 1 )
     {
       itkExceptionMacro("End point not with +/-1 line from line")
     }

  typename PolygonType::VertexType::VectorType offset;

  switch (state)
    {
    case UP_RIGHT:
      offset[0] = 1;
      offset[1] = 1;
      break;
    case UP_LEFT:
      offset[0] = 0;
      offset[1] = 1;
      break;
    case DOWN_RIGHT:
      offset[0] = 1;
      offset[1] = 0;
      break;
    case DOWN_LEFT:
      offset[0] = 0;
      offset[1] = 0;
      break;
    }

  typename PolygonType::VertexType newPoint;

  m_CurrentPoint = startPoint;
  m_CurrentPoint[0] += 1;

  if (m_CurrentPoint[0] < endPoint[0] - 1)
    {
    m_CurrentPoint[1] = line + m_LineOffset;
    newPoint = m_CurrentPoint;
    newPoint += offset;
    polygon->AddVertex(IndexToPoint(newPoint));
    }

  // if the end point is not on line n, add an intermediate point
  if (static_cast<int>(line + m_LineOffset) != endPoint[1] && m_CurrentPoint[0] < endPoint[0] - 1)
    {
    m_CurrentPoint[0] = endPoint[0] - 1;
    newPoint = m_CurrentPoint;
    newPoint += offset;
    polygon->AddVertex(IndexToPoint(newPoint));
    }

  if (m_CurrentPoint != endPoint)
    {
    m_CurrentPoint = endPoint;
    newPoint = m_CurrentPoint;
    newPoint += offset;
    polygon->AddVertex(IndexToPoint(newPoint));
    }
}

// Apply origin and spacing
template<class TLabelObject, class TPolygon>
typename LabelObjectToPolygonFunctor<TLabelObject, TPolygon>
::VertexType
LabelObjectToPolygonFunctor<TLabelObject, TPolygon>
::IndexToPoint(const VertexType& index) const
{
  VertexType resp;

  // Apply origin and spacing
  resp[0] = (index[0] - m_StartIndex[0]) * m_Spacing[0] + m_Origin[0];
  resp[1] = (index[1] - m_StartIndex[1]) * m_Spacing[1] + m_Origin[1];

  return resp;
}

} // end namespace Functor

} // end namespace otb

#endif
