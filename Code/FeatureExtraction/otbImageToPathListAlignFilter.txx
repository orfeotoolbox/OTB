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
#ifndef __otbImageToPathListAlignFilter_txx
#define __otbImageToPathListAlignFilter_txx

#include "otbImageToPathListAlignFilter.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkPathIterator.h"
#include "itkNumericTraits.h"
#include "itkImageLinearIteratorWithIndex.h"
#include "otbMath.h"

namespace otb
{

struct one_segment
{
  short start;    /* starting position (distance from border) */
  short end;      /* ending position (hence, length is end-start+1) */
  double nfa;     /* number of false alarms */
  char ok;
};

/** Constructor */
template <class TInputImage, class TOutputPath>
ImageToPathListAlignFilter<TInputImage, TOutputPath>
::ImageToPathListAlignFilter()
{
  this->SetNumberOfRequiredInputs(1);
  m_Size.Fill(0);
  m_isMeaningfulSegment = false;
  m_NbGradDirection = 16;
  m_NbLineDirection = 96;
  m_MinGradNorm = 2.0;
  m_Eps = 0.0;

  for (unsigned int i = 0; i < InputImageDimension; ++i)
    {
    // Set an image spacing for the user
    m_Spacing[i] = 1.0;
    m_Origin[i] = 0;
    }

  m_PathValue = itk::NumericTraits<ValueType>::One;
  m_BackgroundValue = itk::NumericTraits<ValueType>::Zero;
}

/** Destructor */
template <class TInputImage, class TOutputPath>
ImageToPathListAlignFilter<TInputImage, TOutputPath>
::~ImageToPathListAlignFilter()
{
}

//----------------------------------------------------------------------------
template <class TInputImage, class TOutputPath>
void
ImageToPathListAlignFilter<TInputImage, TOutputPath>
::SetSpacing(const double* spacing)
{
  unsigned int i;
  for (i = 0; i < InputImageDimension; ++i)
    {
    if (spacing[i] != m_Spacing[i])
      {
      break;
      }
    }
  if (i < InputImageDimension)
    {
    for (i = 0; i < InputImageDimension; ++i)
      {
      m_Spacing[i] = spacing[i];
      }
    this->Modified();
    }
}

template <class TInputImage, class TOutputPath>
void
ImageToPathListAlignFilter<TInputImage, TOutputPath>
::SetSpacing(const float* spacing)
{
  unsigned int i;
  for (i = 0; i < InputImageDimension; ++i)
    {
    if ((double) spacing[i] != m_Spacing[i])
      {
      break;
      }
    }
  if (i < InputImageDimension)
    {
    for (i = 0; i < InputImageDimension; ++i)
      {
      m_Spacing[i] = spacing[i];
      }
    this->Modified();
    }
}

template <class TInputImage, class TOutputPath>
const double *
ImageToPathListAlignFilter<TInputImage, TOutputPath>
::GetSpacing() const
{
  return m_Spacing;
}

//----------------------------------------------------------------------------
template <class TInputImage, class TOutputPath>
void
ImageToPathListAlignFilter<TInputImage, TOutputPath>
::SetOrigin(const double* origin)
{
  unsigned int i;
  for (i = 0; i < InputImageDimension; ++i)
    {
    if (origin[i] != m_Origin[i])
      {
      break;
      }
    }
  if (i < InputImageDimension)
    {
    for (i = 0; i < InputImageDimension; ++i)
      {
      m_Origin[i] = origin[i];
      }
    }
}

template <class TInputImage, class TOutputPath>
void
ImageToPathListAlignFilter<TInputImage, TOutputPath>
::SetOrigin(const float* origin)
{
  unsigned int i;
  for (i = 0; i < InputImageDimension; ++i)
    {
    if ((double) origin[i] != m_Origin[i])
      {
      break;
      }
    }
  if (i < InputImageDimension)
    {
    for (i = 0; i < InputImageDimension; ++i)
      {
      m_Origin[i] = origin[i];
      }
    }
}

template <class TInputImage, class TOutputPath>
const double *
ImageToPathListAlignFilter<TInputImage, TOutputPath>
::GetOrigin() const
{
  return m_Origin;
}

//----------------------------------------------------------------------------
/* Algorithm */
template <class TInputImage, class TOutputPath>
std::vector<double>
ImageToPathListAlignFilter<TInputImage, TOutputPath>
::tab(int n, double p, double m)
{
  std::vector<double> out;
  int                 adr1, adr2, x, y;
//  double lambda;
  double q;

  q = 1.0 - p;
  out.resize((n + 1) * (n + 1));
  adr1 = 0;

  /*** compute proba (=x among y) ***/
  out[0] = 1.0;
  for (y = 1, adr2 = 0; y <= n; ++y)
    {
    adr1 = adr2;
    adr2 += n + 1;
    out[adr2] = q * out[adr1];
    for (x = 1; x <= y; ++x)
      out[adr2 + x] = p * out[adr1 + x - 1] + q * out[adr1 + x];
    }

  /*** sum to obtain proba (>=k among y) ***/
  for (y = 1, adr1 = n + 1; y <= n; ++y, adr1 += n + 1)
    for (x = y - 1; x >= 0; x--)
      out[adr1 + x] += out[adr1 + x + 1];

  /*** multiply by m (number of segments) to obtain expectation***/
  for (adr1 = (n + 1) * (n + 1); --adr1 >= 0; )
    out[adr1] *= m;

  return out;
}

template <class TInputImage, class TOutputPath>
void
ImageToPathListAlignFilter<TInputImage, TOutputPath>
::AngleCalculate(const InputImageType* InputImage)
{
  double threshold;
  int    n, p, x, y;

  typename InputImageType::SizeType Taille;
  typename RealImageType::IndexType IndexOut;

  Taille = InputImage->GetLargestPossibleRegion().GetSize();

  typename RealImageType::RegionType region;
  region.SetSize(InputImage->GetLargestPossibleRegion().GetSize());
  IndexOut[0] = 0;
  IndexOut[1] = 0;
//  region.SetIndex(InputImage->GetLargestPossibleRegion().GetIndex());
  region.SetIndex(IndexOut);
  m_AngleImage->SetRegions(region);
  m_AngleImage->SetOrigin(InputImage->GetOrigin());
  m_AngleImage->SetSpacing(InputImage->GetSpacing());
  m_AngleImage->Allocate();

  n = Taille[0];
  p = Taille[1];

  threshold = m_MinGradNorm;
  threshold *= threshold;

  typename InputImageType::IndexType idx;

  for (x = 0; x < p; ++x)
    {
    idx[0] = (n - 1);
    idx[1] = x;
//     indice = (n-1)*p +x
    m_AngleImage->SetPixel(idx, static_cast<RealType>(-1000.0));
    }
  for (y = 0; y < n; ++y)
    {
    idx[0] = y;
    idx[1] = p - 1;
//     indice = p*y+p-1
    m_AngleImage->SetPixel(idx, static_cast<RealType>(-1000.0));
    }

  typename InputImageType::IndexType adr;
  RealType                           PixelA, PixelB, PixelC, PixelD;
  RealType                           com1, com2, gx, gy, norm;

  for (x = 0; x < p - 1; ++x)
    for (y = 0; y < n - 1; ++y)
      {
// indice = y*p+x
      adr[0] = y;
      adr[1] = x;
      idx[0] = adr[0] + 1;
      idx[1] = adr[1] + 1;
      PixelA = static_cast<RealType>(InputImage->GetPixel(idx));
      idx[0] = adr[0];
      idx[1] = adr[1];
      assert(idx[0] < n);
      assert(idx[1] < p);
      assert(idx[0] >= 0);
      assert(idx[1] >= 0);
      PixelB = static_cast<RealType>(InputImage->GetPixel(idx));
      idx[0] = adr[0] + 1;
      idx[1] = adr[1];
      assert(idx[0] < n);
      assert(idx[1] < p);
      assert(idx[0] >= 0);
      assert(idx[1] >= 0);
      PixelC = static_cast<RealType>(InputImage->GetPixel(idx));
      idx[0] = adr[0];
      idx[1] = adr[1] + 1;
      assert(idx[0] < n);
      assert(idx[1] < p);
      assert(idx[0] >= 0);
      assert(idx[1] >= 0);
      PixelD = static_cast<RealType>(InputImage->GetPixel(idx));
      com1 = PixelA - PixelB;
      com2 = PixelC - PixelD;
      gx = 0.5 * (com1 + com2);
      gy = 0.5 * (com1 - com2);
      norm = gx * gx + gy * gy;

      if (norm <= threshold) m_AngleImage->SetPixel(adr, static_cast<RealType>(-1000.0));
      else m_AngleImage->SetPixel(adr, static_cast<RealType>(vcl_atan2(gx, -gy)));
      }
}

template <class TInputImage, class TOutputPath>
void
ImageToPathListAlignFilter<TInputImage, TOutputPath>
::GenerateData(void)
{
//  SizeType size;
//  double origin[InputImageDimension];
  typename InputImageType::SizeType Taille;
  RealImageTypeIndexType            indexAngle;
//  Flist result;
  int                      iseglist, size_seglist; /* associated counter and dynamic size */
  int                      iseg, size_seg;
  double                   nfa, max_nfa;
  std::vector<double>      test;
  std::vector<int>         count, startbloc, endbloc;
  std::vector<double>      seglist; /* list of recorded segments */
  std::vector<one_segment> seg;
  int                      mx, my, ox, oy, nx, ny, n;
  int                      xx, yy, pos, posmax, nblocs, inbloc, max_nblocs;
  int                      cur, i, j, side, l, lphase;
  // int tmp;
  int    itheta, ntheta;
  double theta, theta0, dtheta, dx, dy, prec;
  double error = 0.0;
  itkDebugMacro(<< "ImageToPathListAlignFilter::GenerateData() called");

  // Get the input and output pointers
  const InputImageType * InputImage   = this->GetInput();
  OutputPathListType *   OutputPath   = this->GetOutput();
  // Generate the image

  /* Filter algorithm */

  Taille = InputImage->GetLargestPossibleRegion().GetSize();
  nx = Taille[0];
  ny = Taille[1];
  max_nfa = vcl_pow(10.0, -(m_Eps));

//  typename InputImageType::IndexType adr;

  /*** maximal length for a line */
  n = (int) vcl_ceil(hypot((double) nx, (double) ny)) + 1;

  /*** compute angle map of u ***/
  RealImageTypePointer lAngleImagePointer = RealImageType::New();
  m_AngleImage = static_cast<RealImageType*>(lAngleImagePointer.GetPointer());
  this->AngleCalculate(InputImage);

  /*** compute P(k, l) ***/
  test = tab(n, 1.0 / (double) (m_NbGradDirection), (double) (nx * ny) * (double) (nx * ny));

  /*** initialization ***/
  prec = CONST_PI / (double) (m_NbGradDirection);
  ntheta = m_NbLineDirection / 2;  /* i.e. # directions of NON-ORIENTED lines */
  dtheta = CONST_PI / (double) ntheta;

  /******************** memory allocation ********************/

  max_nblocs = n / 2 + 1; /* maximal number of blocs */
  count.resize(max_nblocs);
  startbloc.resize(max_nblocs);
  endbloc.resize(max_nblocs);

  size_seg = 10000; /* initial allocation (may reallocate later) */
  seg.resize(size_seg);

  size_seglist = 10000; /* initial allocation (may reallocate later) */
  seglist.resize(5 * size_seglist);

  /* counter for recorded segments (seglist) */
  iseglist = 0;

  /******************** first loop : the four sides ********************/

  for (side = 0; side < 4; side++)
    {
    printf("side %d/4 ", side + 1);

    theta0 = CONST_PI_2 * (double) side;
    mx = ((side == 0 || side == 2) ? 1 : 0);
    my = ((side == 1 || side == 3) ? 1 : 0);
    ox = ((side == 1) ? nx - 1 : 0);
    oy = ((side == 2) ? ny - 1 : 0);

    posmax = nx * mx + ny * my;

    /*** second loop : angles ***/
    for (itheta = 0; itheta < ntheta; itheta++)
      {
      printf(".");
      fflush(stdout);
      theta = theta0 + (double) (itheta) * dtheta;
      dx = (double) vcl_cos((double) theta);
      dy = (double) vcl_sin((double) theta);

      /*** third loop : start positions ***/
      for (pos = 0; pos < posmax; ++pos)
        {

        /* clear segment array */
        iseg = 0;

        /*** fourth loop : phase for two-spaced pixels ***/
        for (lphase = 0; lphase < 2; lphase++)
          {

          /*** detect aligned points by blocs ***/
          inbloc = nblocs = cur = l = count[0] = 0;
          xx = ox + pos * mx + (int) (dx * (double) (l * 2 + lphase));
          yy = oy + pos * my + (int) (dy * (double) (l * 2 + lphase));

          for (; xx >= 0 && xx < nx && yy >= 0 && yy < ny; )
            {
            indexAngle[0] = xx;
            indexAngle[1] = yy;
            // indice  = yy*nx+xx
            assert(indexAngle[0] < nx);
            assert(indexAngle[1] < ny);
            assert(indexAngle[0] >= 0);
            assert(indexAngle[1] >= 0);

            error = static_cast<double>(m_AngleImage->GetPixel(indexAngle));
            if (error > -100.0)
              {
              error -= theta;
              while (error <= -CONST_PI)
                error += CONST_2PI;
              while (error > CONST_PI)
                error -= CONST_2PI;
              if (error < 0.0) error = -error;
              if (error < prec)
                {
                ++cur;
                if (!inbloc)
                  {
                  startbloc[nblocs] = l;
                  inbloc = 1;
                  }
                }
              else
                {
                if (inbloc)
                  {
                  endbloc[nblocs] = l - 1;
                  ++nblocs;
                  count[nblocs] = cur;
                  }
                inbloc = 0;
                }
              }
            /* compute next point */
            ++l;
            xx = ox + pos * mx + (int) (dx * (double) (l * 2 + lphase));
            yy = oy + pos * my + (int) (dy * (double) (l * 2 + lphase));
            }

          /*** detect meaningful segments ***/
          for (i = 0; i < nblocs; ++i)
            for (j = i; j < nblocs; ++j)
              if ((nfa = test[count[j + 1] - count[i]
                              + (n + 1) * (1 + endbloc[j] - startbloc[i])]) < max_nfa)
                {
                seg[iseg].start = startbloc[i] * 2 + lphase;
                seg[iseg].end = endbloc[j] * 2 + lphase;
                seg[iseg].nfa = nfa;
                seg[iseg].ok = 1;
                iseg++;
                /* reallocate if necessary */
                if (iseg == size_seg)
                  {
                  size_seg = (size_seg * 3) / 2;
                  seg.resize(size_seg);
//      if (!seg)
//        mwerror(FATAL, 1,"Not enough memory.");
                  }
                }
          }
        /*** end of phase loop ***/

        /*** remove non-maximal segments ***/
        if (!m_isMeaningfulSegment)
          for (i = 0; i < iseg; ++i)
            for (j = 0; j < iseg; ++j)
              if (i != j)

                /* seg[i] is included in seg[j] ? */
                if (seg[i].start >= seg[j].start && seg[i].end <= seg[j].end)
                  {

                  /* remove the less meaningful of seg[i] and seg[j] */
                  if (seg[i].nfa < seg[j].nfa) seg[j].ok = 0;
                  else seg[i].ok = 0;

                  }

        /*** store detected segments ***/
        for (i = 0; i < iseg; ++i)
          if (seg[i].ok)
            {
            seglist[iseglist * 5] = (double) (ox + pos * mx) + dx * (double) (seg[i].start);
            seglist[iseglist * 5 + 1] = (double) (oy + pos * my) + dy * (double) (seg[i].start);
            seglist[iseglist * 5 + 2] = (double) (ox + pos * mx) + dx * (double) (seg[i].end);
            seglist[iseglist * 5 + 3] = (double) (oy + pos * my) + dy * (double) (seg[i].end);
            seglist[iseglist * 5 + 4] = -(double) log10(seg[i].nfa);
            iseglist++;
            /* reallocate seglist if necessary */
            if (iseglist == size_seglist)
              {
              size_seglist = (size_seglist * 3) / 2;
              seglist.resize(size_seglist);
//        if (!seglist)
//    mwerror(FATAL, 1,"Not enough memory.");
              }
            }
        }
      }
    /*** end of second loop ***/

    printf("   nb de segments: %d\n", iseglist);
    }
  /******************** end of first loop ********************/

  seg.clear();
  endbloc.clear();
  startbloc.clear();
  count.clear();
  test.clear();

  /* build segments list */
  seglist.resize(5 * iseglist);

  /* build segments list */
  OutputPath->Clear();
//  OutputPath->Resize(iseglist);

  typedef typename OutputPathType::ContinuousIndexType ContinuousIndexType;
  typename InputImageType::PointType point;

  ContinuousIndexType cindex;
  for (i = 0; i < iseglist; ++i)
    {

    OutputPathPointerType path = OutputPathType::New();

    path->Initialize();
    point[0] = seglist[i * 5];
    point[1] = seglist[i * 5 + 1];
    InputImage->TransformPhysicalPointToContinuousIndex(point, cindex);
    path->AddVertex(cindex);
    cindex[0] = seglist[i * 5 + 2];
    cindex[1] = seglist[i * 5 + 3];
    path->AddVertex(cindex);

    OutputPath->PushBack(path);
    }
  itkDebugMacro(<< "ImageToPathListAlignFilter::GenerateData() finished");

} // end update function

template <class TInputImage, class TOutputPath>
void
ImageToPathListAlignFilter<TInputImage, TOutputPath>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
//  os << indent << "Size : " << m_Size << std::endl;
//  os << indent << "Path Value : " << m_PathValue << std::endl;
//  os << indent << "Background Value : " << m_BackgroundValue << std::endl;
}

} // end namespace otb

#endif
