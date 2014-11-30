// exact C++ implementation of lowe's sift program
// Copyright (C) zerofrog(@gmail.com), 2008-2009
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//Lesser GNU General Public License for more details.
//
//You should have received a copy of the GNU Lesser General Public License
//along with this program.  If not, see <http://www.gnu.org/licenses/>.

// This source code was carefully calibrated to match David Lowe's SIFT features program
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>

#include <vector>
#include <iostream>
#include <map>
#include <list>

#include <sys/timeb.h>    // ftime(), struct timeb

#ifndef _MSC_VER
#include <sys/time.h>
#else
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#endif

#if defined(__SSE3__)
#include <pmmintrin.h>
#elif defined(__SSE2__)
#include <emmintrin.h>
#elif defined(__SSE__)
#include <xmmintrin.h>
#else
#ifndef _MSC_VER
#warning "Not compiling with SSE extenions"
#endif
#endif

#ifdef _OPENMP
#include <omp.h>
#else
#ifndef _MSC_VER
#warning "OpenMP not enabled. Use -fopenmp (>=gcc-4.2) or -openmp (icc) for speed enhancements on SMP machines."
#endif
#endif

using namespace std;

#define PI 3.141592654f
#define SQRT2 1.4142136f

// if defined, will profile the critical functions and write results to prof.txt
//#define DVPROFILE

// if defined will align all image rows to 16 bytes
// usually aligning is faster (can save ~100ms), however for 1024x768
// cache misses with the enlarged rows make it ~400-500ms slower
//#define ALIGNED_IMAGE_ROWS

#ifdef ALIGNED_IMAGE_ROWS
#define _MM_LOAD_ALIGNED _mm_load_ps
#define _MM_STORE_ALIGNED _mm_store_ps
#else
#define _MM_LOAD_ALIGNED _mm_loadu_ps
#define _MM_STORE_ALIGNED _mm_storeu_ps
#endif

#ifdef DVPROFILE
#include "profiler.h"
#else
#define DVSTARTPROFILE()
#endif

typedef struct ImageSt {
    int rows, cols;          // Dimensions of image.
    float *pixels;          // 2D array of image pixels.
    int stride;             // how many floats until the next row
                            // (used to add padding to make rows aligned to 16 bytes)
} *Image;


// Data structure for a keypoint.  Lists of keypoints are linked by the "next" field.
typedef struct KeypointSt {
    float row, col;             // Subpixel location of keypoint.
    float scale, ori;           // Scale and orientation (range [-PI,PI])
    float descrip[128];     // Vector of descriptor values
    struct KeypointSt *next;    // Pointer to next keypoint in list.
} *Keypoint;


int DoubleImSize = 1;
int Scales = 3;
float InitSigma = 1.6f;
float PeakThresh;

static list<Keypoint> s_listKeypoints;

typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

#if defined(__SSE__) && !defined(SIMDMATH_H)

#ifdef _MSC_VER
typedef __m128           vec_int4;
typedef __m128           vec_float4;
#else
// need an implementation of atan2 in SSE
typedef int             vec_int4   __attribute__ ((vector_size (16)));
typedef float           vec_float4 __attribute__ ((vector_size (16)));
#endif

inline vec_float4 atanf4(  vec_float4 x );
inline vec_float4 atan2f4( vec_float4 y, vec_float4 x );

#endif

inline u64 GetMicroTime()
{
//OTB - 28/11/2014
/*For mingw LARGE_INTEGER is not defined but timeval struct exists in winsock2.h */
#if defined(_WIN32) && defined(_MSC_VER)
    LARGE_INTEGER count, freq;
    QueryPerformanceCounter(&count);
    QueryPerformanceFrequency(&freq);
    return (count.QuadPart * 1000000) / freq.QuadPart;
#else
    struct timeval t;
    gettimeofday(&t, NULL);
    return (u64)t.tv_sec*1000000+t.tv_usec;
#endif
}

// aligned malloc and free
inline void* sift_aligned_malloc(size_t size, size_t align)
{
    assert( align < 0x10000 );
	char* p = (char*)malloc(size+align);
	int off = 2+align - ((int)(size_t)(p+2) % align);

	p += off;
	*(u16*)(p-2) = off;

	return p;
}

void sift_aligned_free(void* pmem)
{
    if( pmem != NULL ) {
        char* p = (char*)pmem;
        free(p - (int)*(u16*)(p-2));
    }
}

#if defined(_MSC_VER)
#define SIFT_ALIGNED16(x) __declspec(align(16)) x
#else
#define SIFT_ALIGNED16(x) x __attribute((aligned(16)))
#endif

extern "C" {
Image CreateImage(int rows, int cols);
Image CreateImageFromMatlabData(double* pdata, int rows, int cols);
void DestroyAllImages();
Keypoint GetKeypoints(Image porgimage, unsigned int nbScales);
Image SiftDoubleSize(Image p);
Image SiftCopyImage(Image p);
Image HalfImageSize(Image curimage);
Keypoint OctaveKeypoints(Image pimage, Image* phalfimage, float fscale, Keypoint prevkeypts, unsigned int nbScales);
void SubtractImage(Image imgdst, Image img0, Image img1);
void GaussianBlur(Image imgdst, Image image, float fblur);
void ConvHorizontal(Image imgdst, Image image, float* kernel, int ksize);
void ConvHorizontalFast(Image imgdst, Image image, float* kernel, int ksize);
void ConvVertical(Image image, float* kernel, int ksize);
void ConvVerticalFast(Image image, float* kernel, int ksize);
void ConvBuffer(float* buf, float* kernel, int cols, int ksize);
Keypoint FindMaxMin(Image* imdiff, Image* imgaus, float fscale, Keypoint prevkeypts,unsigned int nbScales);
void GradOriImages(Image imgaus, Image imgrad, Image imorient);
void GradOriImagesFast(Image imgaus, Image imgrad, Image imorient);
int LocalMaxMin(float fval, Image imdiff, int row, int col);
int NotOnEdge(Image imdiff, int row, int col);
Keypoint InterpKeyPoint(Image* imdiff, int index, int rowstart, int colstart,
                        Image imgrad, Image imorient, char* pMaxMinArray, float fscale,Keypoint keypts, int steps, unsigned int nbScales);
float FitQuadratic(float* X, Image* imdiff, int index, int rowstart, int colstart);
void SolveLinearSystem(float* Y, float* H, int dim);
Keypoint AssignOriHist(Image imgrad, Image imorient, float fscale, float fSize,
                       float frowstart, float fcolstart, Keypoint keypts);
float InterpPeak(float f0, float f1, float f2);
void SmoothHistogram(float* phist, int numbins);
Keypoint MakeKeypoint(Image imgrad, Image imorient, float fscale, float fSize,
                      float frowstart,float fcolstart,float forient, Keypoint keypts);
void MakeKeypointSample(Keypoint pnewkeypt, Image imgrad, Image imorient,
                        float fSize, float frowstart, float fcolstart);
void NormalizeVec(float* pf, int num);
void KeySample(float* fdesc, Keypoint pnewkeypt, Image imgrad, Image imorient,
               float fSize, float frowstart, float fcolstart);
void AddSample(float* fdesc, Keypoint pkeypt, Image imgrad, Image imorient, int r, int c,
               float rpos, float cpos, float rx, float cx);
void PlaceInIndex(float* fdesc, float fgrad, float forient, float fnewrow, float fnewcol);
void FreeKeypoints(Keypoint keypt);
void DestroyAllResources();
}

static list<Image> s_listImages;
Image CreateImage(int rows, int cols)
{
    Image im;

    im = (Image) sift_aligned_malloc(sizeof(struct ImageSt),16);
    im->rows = rows;
    im->cols = cols;

    // cannot make 16 byte aligned since 1024x768 images
#if defined(ALIGNED_IMAGE_ROWS) && defined(__SSE__)
    im->stride = (cols+3)&~3;
#else
    im->stride = cols;
#endif

    im->pixels = (float*)sift_aligned_malloc(rows*im->stride*sizeof(float)+16,128); // add padding (for sse)
    s_listImages.push_back(im);
    return im;
}

void DestroyAllImages()
{
    for(list<Image>::iterator it = s_listImages.begin(); it != s_listImages.end(); ++it) {
        sift_aligned_free((*it)->pixels);
        sift_aligned_free(*it);
    }
    s_listImages.clear();
}

Image CreateImageFromMatlabData(double* pdata, int rows, int cols)
{
    Image image = CreateImage(rows,cols);
    float* pixels = image->pixels;
    int stride = image->stride;

#ifdef __SSE2__
    for(int i = 0; i < (rows&~1); i += 2, pixels+=2*stride) {
        for(int j = 0; j < (cols&~3); j += 4) {
            double* pf = &pdata[i+j*rows];
#ifdef ALIGNED_IMAGE_ROWS
            __m128d m0 = _mm_load_pd(pf);
            __m128d m1 = _mm_load_pd(pf+rows);
            __m128d m2 = _mm_load_pd(pf+2*rows);
            __m128d m3 = _mm_load_pd(pf+3*rows);
#else
            __m128d m0 = _mm_loadu_pd(pf);
            __m128d m1 = _mm_loadu_pd(pf+rows);
            __m128d m2 = _mm_loadu_pd(pf+2*rows);
            __m128d m3 = _mm_loadu_pd(pf+3*rows);
#endif

            __m128 mrows0 = _mm_shuffle_ps(_mm_cvtpd_ps(m0),_mm_cvtpd_ps(m1),0x44);
            __m128 mrows1 = _mm_shuffle_ps(_mm_cvtpd_ps(m2),_mm_cvtpd_ps(m3),0x44);

            _mm_storeu_ps(pixels+j,_mm_shuffle_ps(mrows0,mrows1,0x88));
            _mm_storeu_ps(pixels+j+stride,_mm_shuffle_ps(mrows0,mrows1,0xdd));
        }

        for(int j = (cols&~3); j < cols; j++) {
            pixels[j] = pdata[i+j*rows];
            pixels[j+stride] = pdata[i+j*rows+1];
        }
    }

    if( rows & 1 ) {
        for(int j = 0; j < cols; ++j)
            pixels[j] = (float)pdata[rows-1+j*rows];
    }
#else
    for(int i = 0; i < rows; ++i, pixels+=stride) {
        for(int j = 0; j < cols; ++j)
            pixels[j] = (float)pdata[i+j*rows];
    }
#endif

    return image;
}

static Image* s_imgaus = NULL, *s_imdiff = NULL;
static Image s_imgrad = NULL, s_imorient = NULL;
static char* s_MaxMinArray = NULL;

Keypoint GetKeypoints(Image porgimage, unsigned int nbScales)
{
#ifdef DVPROFILE
    DVProfClear();
#endif

    Image pimage = NULL;
    float fscale = 1.0f;
    Image halfimage = NULL;
    Keypoint keypts = 0;

    {
        DVSTARTPROFILE();

        PeakThresh = 0.04f/(float)nbScales;
        s_imgaus = new Image[((27 + 4*nbScales)&0xfffffff0)/4];
        s_imdiff = new Image[((23 + 4*nbScales)&0xfffffff0)/4];

        if( DoubleImSize ) {
            pimage = SiftDoubleSize(porgimage);
            fscale = 0.5f;
        }
        else
            pimage = SiftCopyImage(porgimage);

        float fnewscale = 1.0f;
        if( !DoubleImSize )
            fnewscale = 0.5f;

        if( InitSigma > fnewscale ) {
            GaussianBlur(pimage, pimage, sqrtf(InitSigma*InitSigma - fnewscale*fnewscale));
//            {
//                FILE* f = fopen("test.txt","w");
//                int rows = pimage->rows, cols = pimage->cols, stride = pimage->stride;
//                float *_pdst = pimage->pixels;
//                for(int j = 0; j < rows; ++j, _pdst += stride ) {
//                    for(int k = 0; k < cols; ++k) {
//                        fprintf(f,"%f ",_pdst[k]);
//                    }
//                    fprintf(f,"\n");
//                }
//                fclose(f);
//            }
        }

        // create the images
        s_imgaus[0] = pimage;
        for(int i = 1; i < (int)nbScales+3; ++i)
            s_imgaus[i] = CreateImage(pimage->rows,pimage->cols);
        for(int i = 0; i < (int)nbScales+2; ++i)
            s_imdiff[i] = CreateImage(pimage->rows,pimage->cols);
        s_imgrad = CreateImage(pimage->rows,pimage->cols);
        s_imorient = CreateImage(pimage->rows,pimage->cols);
        s_MaxMinArray = (char*)sift_aligned_malloc(pimage->rows*pimage->cols,16);

        while( pimage->rows > 12 && pimage->cols > 12 ) {
            keypts = OctaveKeypoints(pimage, &halfimage, fscale, keypts,nbScales);
            pimage = HalfImageSize(halfimage);
            fscale += fscale;
        }

        delete[] s_imgaus;
        delete[] s_imdiff;
        sift_aligned_free(s_MaxMinArray);

    }

#ifdef DVPROFILE
    DVProfWrite("prof.txt");
#endif

    return keypts;
}

Image SiftDoubleSize(Image im)
{
    int rows = im->rows, cols = im->cols;
    int newrows = 2*rows-2, newcols = 2*cols-2;
    Image newim = CreateImage(newrows, newcols);
    float* psrc = im->pixels, *pdst = newim->pixels;
    int stride = im->stride, newstride = newim->stride;
    for(int i = 0; i < rows-1; ++i, psrc += stride, pdst += 2*newstride) {
        for(int j = 0; j < cols-1; ++j) {
            pdst[2*j] = psrc[j];
            pdst[newstride+2*j] = 0.5f*(psrc[j] + psrc[stride+j]);
            pdst[2*j+1] = 0.5f*(psrc[j] + psrc[j+1]);
            pdst[newstride+2*j+1] = 0.25f*(psrc[j] + psrc[j+1] + psrc[stride+j] + psrc[stride+j+1]);
        }
    }

    return newim;
}

Image SiftCopyImage(Image im)
{
    DVSTARTPROFILE();
    Image newim = CreateImage(im->rows,im->cols);
    memcpy(newim->pixels, im->pixels, sizeof(float)*im->rows*im->stride);
    return newim;
}

Image HalfImageSize(Image curimage)
{
    int cols = curimage->cols;
    int newrows = (curimage->rows+(curimage->rows<0))>>1;
    int newcols = (cols+(curimage->cols<0))>>1;

    Image halfimage = CreateImage(newrows, newcols);
    float* psrc = curimage->pixels, *pdst = halfimage->pixels;
    int stride = curimage->stride, newstride = halfimage->stride;


    for(int halfrow = 0; halfrow < newrows; ++halfrow, pdst += newstride, psrc += 2*stride ) {
        for(int halfcol = 0; halfcol < newcols; ++halfcol) {
            pdst[halfcol] = psrc[halfcol*2];
        }
    }

    return halfimage;
}

Keypoint OctaveKeypoints(Image pimage, Image* phalfimage, float fscale, Keypoint prevkeypts,unsigned int nbScales)
{
    DVSTARTPROFILE();

    float fwidth = powf(2.0f,1.0f / (float)nbScales);
    float fincsigma = sqrtf(fwidth * fwidth - 1.0f);

    int rows = pimage->rows, cols = pimage->cols, stride = pimage->stride;

    s_imgaus[0] = pimage;
    float sigma = InitSigma;
    for(int i = 1; i < (int)nbScales+3; ++i) {

        s_imgaus[i]->rows = rows; s_imgaus[i]->cols = cols; s_imgaus[i]->stride = stride;
        GaussianBlur(s_imgaus[i], s_imgaus[i-1], fincsigma * sigma);

        s_imdiff[i-1]->rows = rows; s_imdiff[i-1]->cols = cols; s_imdiff[i-1]->stride = stride;
        SubtractImage(s_imdiff[i-1],s_imgaus[i-1],s_imgaus[i]);

        sigma *= fwidth;
    }

    s_imgrad->rows = rows; s_imgrad->cols = cols; s_imgrad->stride = stride;
    s_imorient->rows = rows; s_imorient->cols = cols; s_imorient->stride = stride;

    *phalfimage = s_imgaus[nbScales];
    return FindMaxMin(s_imdiff, s_imgaus, fscale, prevkeypts,nbScales);
}

// imgdst = img0 - img1
void SubtractImage(Image imgdst, Image img0, Image img1)
{
    int rows = imgdst->rows, cols = imgdst->cols, stride = imgdst->stride;
    float* _pixels0 = img0->pixels, *_pixels1 = img1->pixels, *_pdst = imgdst->pixels;
#ifdef __SSE__
    #pragma omp parallel for schedule(dynamic,32)
    for(int j = 0; j < rows; ++j ) {
        float* pixels0 = _pixels0+j*stride;
        float* pixels1 = _pixels1+j*stride;
        float* pdst = _pdst + j*stride;

        for(int k = 0; k < (cols&~7); k += 8) {
            _MM_STORE_ALIGNED(pdst+k,_mm_sub_ps(_MM_LOAD_ALIGNED(pixels0+k), _MM_LOAD_ALIGNED(pixels1+k)));
            _MM_STORE_ALIGNED(pdst+k+4,_mm_sub_ps(_MM_LOAD_ALIGNED(pixels0+k+4), _MM_LOAD_ALIGNED(pixels1+k+4)));
        }

        for(int k = (cols&~7); k < cols; ++k)
            pdst[k] = pixels0[k]-pixels1[k];
    }
#else
    for(int j = 0; j < rows; ++j, _pixels0 += stride, _pixels1 += stride, _pdst += stride ) {
        for(int k = 0; k < cols; ++k) {
            _pdst[k] = _pixels0[k]-_pixels1[k];
        }
    }
#endif
}

static map<float, float* > s_mapkernel; // assumes GaussTruncate doesn't change!, if freeing second, subtract 4 bytes

void GaussianBlur(Image imgdst, Image image, float fblur)
{
    DVSTARTPROFILE();

    const float GaussTruncate = 4.0f;


    int ksize = (int)(2.0f * GaussTruncate * fblur + 1.0f);
    if( ksize < 3 )
        ksize = 3;
    ksize += !(ksize&1); // make it odd

    float* kernel = NULL;
    for( map<float, float* >::iterator it = s_mapkernel.begin(); it != s_mapkernel.end(); ++it) {
        if( fabsf(fblur-it->first) < 0.001f ) {
            kernel = it->second;
            break;
        }
    }

    if( kernel == NULL ) { // have to create a new one
        double faccum = 0;

        // +4 for alignment and padding issues with sse
        kernel = (float*)sift_aligned_malloc((ksize+9)*sizeof(float),16)+1;

        int width = (ksize >= 0 ? ksize : ksize-1)>>1;
        for(int i = 0; i <= ksize; ++i) {
            float fweight = expf( - (float)(i-width)*(i-width) / (2.0f*fblur*fblur) );
            faccum += (double)fweight;
            kernel[i] = fweight;
        }

        for(int i = 0; i < ksize; ++i) // shouldn't it be <=?
            kernel[i] /= (float)faccum;
        memset(kernel+ksize,0,sizeof(float)*8);

        s_mapkernel[fblur] = kernel;
    }

#ifdef __SSE__
    if( image->cols < 12 )
        ConvHorizontal(imgdst, image,kernel,ksize);
    else
        ConvHorizontalFast(imgdst, image,kernel,ksize);

    ConvVerticalFast(imgdst,kernel,ksize);
#else
    ConvHorizontal(imgdst, image,kernel,ksize);
    ConvVertical(imgdst,kernel,ksize);
#endif
}

void ConvHorizontal(Image imgdst, Image image, float* kernel, int ksize)
{
    DVSTARTPROFILE();

    static vector<float> _buf; //TODO, make 16 byte aligned
    _buf.resize(image->cols + ksize);
    float* buf = &_buf[0];
    int rows = image->rows, cols = image->cols, stride = image->stride;
    int width = (ksize >= 0 ? ksize : ksize-1)>>1;
    float* pixels = image->pixels;
    float* pdst = imgdst->pixels;

    for(int i = 0; i < rows; ++i, pixels += stride, pdst += stride) {
        for(int j = 0; j < width; ++j)
            buf[j] = pixels[0];
        for(int j = 0; j < cols; ++j)
            buf[width+j] = pixels[j];
        for(int j = 0; j < width; ++j)
            buf[cols+width+j] = pixels[cols-1];
        ConvBuffer(buf,kernel,cols,ksize);

        memcpy(pdst,buf,sizeof(float)*cols);
    }
}

void ConvVertical(Image image, float* kernel, int ksize)
{
    DVSTARTPROFILE();

    static vector<float> _buf; //TODO, make 16 byte aligned
    _buf.resize(image->rows + ksize);
    float* buf = &_buf[0];
    int rows = image->rows, cols = image->cols, stride = image->stride;
    int width = (ksize >= 0 ? ksize : ksize-1)>>1;
    float* pixels = image->pixels;

    for(int j = 0; j < cols; ++j, pixels += 1) {
        for(int i = 0; i < width; ++i)
            buf[i] = pixels[0];
        for(int i = 0; i < rows; ++i)
            buf[width+i] = pixels[i*stride];
        for(int i = 0; i < width; ++i)
            buf[rows+width+i] = pixels[(rows-1)*stride];
        ConvBuffer(buf,kernel,rows,ksize);

        for(int i = 0; i < rows; ++i)
            pixels[i*stride] = buf[i];
    }
}

void ConvBuffer(float* buf, float* kernel, int bufsize, int ksize)
{
    for(int i = 0; i < bufsize; ++i) {
        float faccum = 0;
        for(int j = 0; j < ksize; ++j)
            faccum += buf[i+j]*kernel[j];
        buf[i] = (float)faccum;
    }
}

#ifdef __SSE__

typedef vector<float*> LISTBUF;
static LISTBUF  s_listconvbuf; //TODO, make 16 byte aligned
static int s_convbufsize = 0; // the size of all the buffers in s_listconvbuf
static int SIFT_ALIGNED16(s_convmask[4]) = {static_cast<int>(0xffffffff),static_cast<int>(0xffffffff),static_cast<int>(0xffffffff),0};

struct myaccum { float SIFT_ALIGNED16(faccum[2][4]); };

void ConvHorizontalFast(Image imgdst, Image image, float* kernel, int ksize)
{
    int rows = image->rows, cols = image->cols, stride = image->stride;
    assert( ksize >= 3 && cols >= 3 ); // 3 is cutting it close

#ifdef ALIGNED_IMAGE_ROWS
    assert( !(image->stride&3) );
#endif

    DVSTARTPROFILE();

    int width = (ksize >= 0 ? ksize : ksize-1)>>1;
    float* _pixels = image->pixels, *_pdst = imgdst->pixels;

    int convsize = max(100000,4*(cols + ksize)+36);

    if( s_listconvbuf.size() == 0 || s_convbufsize < convsize ) {
        for(LISTBUF::iterator it = s_listconvbuf.begin(); it != s_listconvbuf.end(); ++it)
            sift_aligned_free(*it);
        s_listconvbuf.clear();

        // create at least one
        s_listconvbuf.push_back((float*)sift_aligned_malloc(convsize,16));
        s_convbufsize = convsize;
    }
#ifdef _OPENMP
    else {
        for(LISTBUF::iterator it = s_listconvbuf.begin(); it != s_listconvbuf.end(); ++it)
            memset(*it+cols+ksize+1,0,32);
    }
#endif

#ifdef _OPENMP
    for(int i = s_listconvbuf.size(); i < omp_get_max_threads(); ++i) {
        s_listconvbuf.push_back((float*)sift_aligned_malloc(convsize,16));
        memset(s_listconvbuf.back()+cols+ksize+1,0,32);
    }
#else
    float* pconvbuf = s_listconvbuf.back();
    memset(pconvbuf+cols+ksize+1,0,32);
#endif

    #pragma omp parallel for schedule(dynamic,16)
    for(int i = 0; i < rows; i++) {

#ifdef _OPENMP
        float* pconvbuf;

        // need to get a free buffer
        #pragma omp critical
        {
            if( s_listconvbuf.size() == 0 ) {
                // for some reason, crashes if this is ever executed....
                pconvbuf = (float*)sift_aligned_malloc(convsize,16);
                memset(pconvbuf+cols+ksize+1,0,32);
            }
            else {
                pconvbuf = s_listconvbuf.back();
                s_listconvbuf.pop_back();
            }
        }
#endif
        // get 16 byte aligned array
        myaccum ac;

        float* pixels = _pixels+i*stride;
        float* pdst = _pdst + i*stride;

        float* buf = pconvbuf+1;
        float f0 = pixels[0], f0e = pixels[cols-1];
        for(int j = 0; j < width; ++j)
            buf[j] = f0;
        memcpy(buf+width,pixels,cols*sizeof(float));
        for(int j = 0; j < width; ++j)
            buf[cols+width+j] = f0e;

        __m128 mkerbase = _mm_and_ps(_mm_loadu_ps(kernel), _mm_load_ps((float*)s_convmask));

        for(int j = 0; j < 2*(cols>>2); ++j) {
            int off = 2*j-(j&1);
            buf = pconvbuf+1+off;
            __m128 maccum0 = _mm_mul_ps(_mm_loadu_ps(buf), mkerbase);
            __m128 maccum1 = _mm_mul_ps(_mm_loadu_ps(buf+2), mkerbase);

            __m128 mbufprev = _mm_loadu_ps(buf+3);
            for(int k = 3; k < ksize; k += 8) {
                __m128 mbuf0 = mbufprev;
                __m128 mker0 = _mm_load_ps(kernel+k);
                __m128 mbuf1 = _mm_loadu_ps(buf+k+4);
                __m128 mker1 = _mm_load_ps(kernel+k+4);

                // accumulate the first row
                maccum0 = _mm_add_ps(maccum0,_mm_mul_ps(mbuf0,mker0));
                maccum0 = _mm_add_ps(maccum0,_mm_mul_ps(mbuf1,mker1));

                mbufprev = _mm_loadu_ps(buf+k+8); // load new
                mbuf0 = _mm_shuffle_ps(mbufprev,mbuf0,0xe4); // remove first 2 elts

                // arrange kernel to match mbuf0 and mbuf1 for second row
                __m128 mkertemp = mker0;
                mker0 = _mm_shuffle_ps(mker1,mker0,0x4e);
                mker1 = _mm_shuffle_ps(mkertemp,mker1,0x4e);

                // accumulate the second row
                maccum1 = _mm_add_ps(maccum1,_mm_mul_ps(mbuf0,mker0));
                maccum1 = _mm_add_ps(maccum1,_mm_mul_ps(mbuf1,mker1));
            }

#ifdef __SSE3__
            maccum0 = _mm_hadd_ps(maccum0,maccum1);
            maccum0 = _mm_hadd_ps(maccum0,maccum0);
            _mm_storel_pi((__m64*)ac.faccum[0],maccum0);
            pdst[off] = ac.faccum[0][0];
            pdst[off+2] = ac.faccum[0][1];
#else
            _mm_store_ps(ac.faccum[0],maccum0);
            _mm_store_ps(ac.faccum[1],maccum1);
            pdst[off] = ac.faccum[0][0]+ac.faccum[0][1]+ac.faccum[0][2]+ac.faccum[0][3];
            pdst[off+2] = ac.faccum[1][0]+ac.faccum[1][1]+ac.faccum[1][2]+ac.faccum[1][3];
#endif
        }

        // take care of the left over columns
        for(int j=(cols&~3); j < cols; ++j) {
            buf = pconvbuf+j+1;
            __m128 maccum0 = _mm_mul_ps(_mm_loadu_ps(buf), mkerbase);

            for(int k = 3; k < ksize; k += 4) {
                __m128 mbuf0 = _mm_loadu_ps(buf+k);
                __m128 mker0 = _mm_load_ps(kernel+k);
                maccum0 = _mm_add_ps(maccum0,_mm_mul_ps(mbuf0,mker0));
            }

#ifdef __SSE3__
            maccum0 = _mm_hadd_ps(maccum0,maccum0);
            maccum0 = _mm_hadd_ps(maccum0,maccum0);
            _mm_store_ss(&pdst[j],maccum0);
#else
            _mm_store_ps(ac.faccum[0],maccum0);
            pdst[j] = ac.faccum[0][0]+ac.faccum[0][1]+ac.faccum[0][2]+ac.faccum[0][3];
#endif
        }

#ifdef _OPENMP
        #pragma omp critical
        {
            s_listconvbuf.push_back(pconvbuf);
        }
#endif
    }
}

void ConvVerticalFast(Image image, float* kernel, int ksize)
{
    int rows = image->rows, stride = image->stride;

    assert( ksize >= 3); // 3 is cutting it close
#ifdef ALIGNED_IMAGE_ROWS
    assert( !(image->stride&3) );
#endif

    DVSTARTPROFILE();

    int convsize = max(100000,32*(image->rows + ksize+4));

    if( s_listconvbuf.size() == 0 || s_convbufsize < convsize ) {
        for(LISTBUF::iterator it = s_listconvbuf.begin(); it != s_listconvbuf.end(); ++it)
            sift_aligned_free(*it);

        s_listconvbuf.clear();

        // create at least one
        s_listconvbuf.push_back((float*)sift_aligned_malloc(convsize,16));
        s_convbufsize = convsize;
    }

#ifdef _OPENMP
    for(int i = s_listconvbuf.size(); i < omp_get_max_threads(); ++i) {
        s_listconvbuf.push_back((float*)sift_aligned_malloc(convsize,16));
    }
#else
    float* pconvbuf = s_listconvbuf.back();
#endif

    int width = (ksize >= 0 ? ksize : ksize-1)>>1;
    float* _pixels = image->pixels;

    #pragma omp parallel for
    for(int j = 0; j < stride; j += 4) {

        float* pixels = _pixels+j;
#ifndef ALIGNED_IMAGE_ROWS
        myaccum ac;
#endif

#ifdef _OPENMP
        float* pconvbuf;

        // need to get a free buffer
        #pragma omp critical
        {
            if( s_listconvbuf.size() == 0 ) {
                pconvbuf = (float*)sift_aligned_malloc(convsize,16);
            }
            else {
                pconvbuf = s_listconvbuf.back();
                s_listconvbuf.pop_back();
            }
        }
#endif

        __m128 mpprev = _MM_LOAD_ALIGNED(pixels);

        __m128 mprev = mpprev;
        __m128 mker0 = _mm_load1_ps(kernel);
        __m128 mker1 = _mm_load1_ps(kernel+1);
        __m128 mker2 = _mm_load1_ps(kernel+2);

        float* buf = pconvbuf;
        for(int i = 2; i <= width; ++i) {
            _mm_store_ps(buf,mpprev);
            __m128 maccum = _mm_add_ps(_mm_mul_ps(mpprev,mker0), _mm_mul_ps(mprev,mker1));
            maccum = _mm_add_ps(maccum,_mm_mul_ps(mprev,mker2));
            _mm_store_ps(buf+4,maccum);
            buf += 8;
        }
        for(int i = 1; i < rows-width+2; ++i) {
            __m128 mnew = _MM_LOAD_ALIGNED(pixels+i*stride);

            _mm_store_ps(buf,mpprev);
            __m128 maccum = _mm_add_ps(_mm_mul_ps(mpprev,mker0), _mm_mul_ps(mprev,mker1));
            maccum = _mm_add_ps(maccum,_mm_mul_ps(mnew,mker2));
            mpprev = mprev;
            _mm_store_ps(buf+4,maccum);
            mprev = mnew;
            buf += 8;
        }

        _mm_store_ps(buf,mpprev); buf += 8;
        for(int i = rows-width+2; i < rows; ++i) {
            __m128 mnew = _mm_loadu_ps(pixels+i*stride);
            _mm_store_ps(buf,mprev);
            mprev = mnew;
            buf += 8;
        }

        // mprev points to the last row
        for(int i = 0; i < width; ++i) {
            _mm_store_ps(buf,mprev);
            buf += 8;
        }
        // have to pad rest with zeros
        memset(buf,0,convsize-((char*)buf-(char*)pconvbuf));

        //// finally convolve
        buf = pconvbuf;

        for(int i = 0; i < rows; ++i, buf += 8) {
            __m128 maccum = _mm_load_ps(buf+4);
            if( ksize > 3 ) {
                for(int k = 3; k < ksize; k += 4) {
                    float* psrc = buf + 8*k;
                    __m128 mkerall = _mm_load_ps(kernel+k);
                    __m128 mnew0 = _mm_load_ps(psrc);
                    mker0 = _mm_shuffle_ps(mkerall,mkerall,0);
                    __m128 mnew1 = _mm_load_ps(psrc + 8);
                    mker1 = _mm_shuffle_ps(mkerall,mkerall,0x55);
                    __m128 mnew2 = _mm_load_ps(psrc + 16);
                    maccum = _mm_add_ps(maccum,_mm_mul_ps(mker0,mnew0));
                    maccum = _mm_add_ps(maccum,_mm_mul_ps(mker1,mnew1));
                    __m128 mnew3 = _mm_load_ps(psrc + 24);
                    mker2 = _mm_shuffle_ps(mkerall,mkerall,0xaa);
                    maccum = _mm_add_ps(maccum,_mm_mul_ps(mker2,mnew2));
                    maccum = _mm_add_ps(maccum,_mm_mul_ps(_mm_shuffle_ps(mkerall,mkerall,0xff),mnew3));
                }
            }

#ifdef ALIGNED_IMAGE_ROWS
            _mm_store_ps(pixels+i*stride,maccum);
#else
            if( j <= stride-4 )
                _mm_storeu_ps(pixels+i*stride,maccum);
            else {
                _mm_store_ps(ac.faccum[0],maccum);
                for(int k = 0; k < ((stride-j)&3); ++k)
                    pixels[i*stride+k] = ac.faccum[0][k];
            }
#endif
        }

#ifdef _OPENMP
        #pragma omp critical
        {
            s_listconvbuf.push_back(pconvbuf);
        }
#endif
    }
}

#endif

Keypoint FindMaxMin(Image* imdiff, Image* imgaus, float fscale, Keypoint keypts, unsigned int nbScales)
{
    DVSTARTPROFILE();

    int rows = imdiff[0]->rows, cols = imdiff[0]->cols, stride = imdiff[0]->stride;
    memset(s_MaxMinArray,0,rows*cols);

    for( int index = 1; index < (int)nbScales+1; ++index) {

#if !defined(OTB_DISABLE_FAST_FUNCTIONS) && !defined(_MSC_VER) && defined(__SSE__)
        GradOriImagesFast(imgaus[index],s_imgrad,s_imorient);
#else
        GradOriImages(imgaus[index],s_imgrad,s_imorient);
#endif
        assert( imdiff[index]->stride == stride );
        float* _diffpixels = imdiff[index]->pixels;

//        for(int i = 0; i < rows; ++i) {
//            for(int j = 0; j < cols; ++j) {
//                if( isnan(imgaus[index]->pixels[i*cols+j]) ) {
//                    fprintf(stderr, "gaus: %d %d %d %d %d %f %f\n", index,i,j,rows,cols,s_imgrad->pixels[i*cols+j],s_imorient->pixels[i*cols+j]);
//                    //exit(0);
//                }
////                if( isnan(s_imorient->pixels[i*cols+j]) ) {
////                    //GradOriImagesFast(imgaus[index],s_imgrad,s_imorient);
////                    fprintf(stderr,"rc %d %d\n",rows,cols);
////                    fprintf(stderr,"wtf %d %d %f %f %f %f\n",i,j, s_imgrad->pixels[i*cols+j], imgaus[index]->pixels[i*cols+j], imgaus[index]->pixels[i*cols+j-1], imgaus[index]->pixels[i*cols+j+1]);
////                    fprintf(stderr,"%f %f\n",imgaus[index]->pixels[(i-1)*cols+j], imgaus[index]->pixels[(i+1)*cols+j]);
////                    exit(0);
////                }
//            }
//        }

        #pragma omp parallel for schedule(dynamic,8)
        for( int rowstart = 5; rowstart < rows-5; ++rowstart ) {
            Keypoint newkeypts = NULL;
            float* diffpixels = _diffpixels + rowstart*stride;
            for( int colstart = 5; colstart < cols-5; ++colstart ) {

                float fval = diffpixels[colstart];
                if( fabsf(fval) > PeakThresh*0.8f ) {
                    if( LocalMaxMin(fval, imdiff[index],rowstart,colstart) &&
                        LocalMaxMin(fval, imdiff[index-1],rowstart,colstart) &&
                        LocalMaxMin(fval, imdiff[index+1],rowstart,colstart) &&
                        NotOnEdge(imdiff[index],rowstart,colstart) ) {
                        newkeypts = InterpKeyPoint(imdiff,index,rowstart,colstart,s_imgrad,s_imorient,s_MaxMinArray,fscale,newkeypts,5,nbScales);
                    }
                }
            }

            if( newkeypts != NULL ) {
                // find the last keypoint
                Keypoint lastkeypt = newkeypts;
                while(lastkeypt->next)
                    lastkeypt = lastkeypt->next;;

                #pragma omp critical
                {
                    lastkeypt->next = keypts;
                    keypts = newkeypts;
                }
            }
        }
    }

    return keypts;
}

void GradOriImages(Image image, Image imgrad, Image imorient)
{
    DVSTARTPROFILE();

    int rows = image->rows, cols = image->cols, stride = image->stride;
    float* _pixels = image->pixels, *_pfgrad = imgrad->pixels, *_pforient = imorient->pixels;
    float fdiffc, fdiffr;

    #pragma omp parallel for schedule(dynamic,16) // might crash Matlab mex files
    for(int i = 0; i < rows; ++i) {
        float* pixels = _pixels + i*stride;
        float* pfgrad = _pfgrad + i*stride;
        float* pforient = _pforient + i*stride;

        for(int j = 0; j < cols; ++j) {
            if( j == 0 )
                fdiffc = 2.0f*(pixels[1]-pixels[0]);
            else if( j == cols-1 )
                fdiffc = 2.0f*(pixels[j]-pixels[j-1]);
            else
                fdiffc = pixels[j+1] - pixels[j-1];

            if( i == 0 )
                fdiffr = 2.0f*(pixels[j] - pixels[stride+j]);
            else if( i == rows-1 )
                fdiffr = 2.0f*(pixels[-stride+j] - pixels[j]);
            else
                fdiffr = pixels[-stride+j] - pixels[stride+j];

            pfgrad[j] = sqrtf(fdiffc*fdiffc + fdiffr*fdiffr);
            pforient[j] = atan2f(fdiffr,fdiffc);
        }
    }
}

#if !defined(OTB_DISABLE_FAST_FUNCTIONS) && !defined(_MSC_VER) && defined(__SSE__)
void GradOriImagesFast(Image image, Image imgrad, Image imorient)
{
    DVSTARTPROFILE();

    int rows = image->rows, cols = image->cols, stride = image->stride;
    float* _pixels = image->pixels, *_pfgrad = imgrad->pixels, *_pforient = imorient->pixels;
    int endcol = ((cols-1)&~3);

    {
        // first row is special 2*(_pixels[0]-_pixels[stride])
        float fdiffc, fdiffr;

        // first and last elt is 2*([1]-[0]), have to improvise for sse
        __m128 mprevj = _mm_set_ps(_pixels[2],_pixels[1],_pixels[0],2.0f*_pixels[0]-_pixels[1]);

        for(int j = 0; j < endcol; j += 4) {
            float* pf = _pixels+j;
            __m128 mnewj = _mm_loadu_ps(pf+3);
            __m128 mgradr = _MM_LOAD_ALIGNED(pf);
            __m128 mgradc = _mm_sub_ps(_mm_shuffle_ps(mprevj,mnewj,0x4e),mprevj);
            mgradr = _mm_sub_ps(mgradr, _MM_LOAD_ALIGNED(pf+stride));
            mgradr = _mm_add_ps(mgradr,mgradr);

            __m128 mrad = _mm_sqrt_ps(_mm_add_ps(_mm_mul_ps(mgradr,mgradr),_mm_mul_ps(mgradc,mgradc)));
            __m128 morient = atan2f4(mgradr,mgradc);

            _MM_STORE_ALIGNED(_pfgrad+j,mrad);
            mprevj = mnewj;
            _MM_STORE_ALIGNED(_pforient+j,morient);
        }


        // compute the rest the old way
        for(int j = endcol; j < cols; ++j) {
            if( j == 0 )
                fdiffc = 2.0f*(_pixels[1]-_pixels[0]);
            else if( j == cols-1 )
                fdiffc = 2.0f*(_pixels[j]-_pixels[j-1]);
            else
                fdiffc = _pixels[j+1] - _pixels[j-1];

            fdiffr = 2.0f*(_pixels[j] - _pixels[stride+j]);

            _pfgrad[j] = sqrtf(fdiffc*fdiffc + fdiffr*fdiffr);
            _pforient[j] = atan2f(fdiffr,fdiffc);
        }
    }

    #pragma omp parallel for schedule(dynamic,16) // might crash Matlab mex files
    for(int i = 1; i < rows-1; ++i) {

        float fdiffc, fdiffr;
        float* pixels = _pixels + i*stride;
        float* pfgrad = _pfgrad + i*stride;
        float* pforient = _pforient + i*stride;

        // first and last elt is 2*([1]-[0]), have to improvise for sse
        __m128 mprevj = _mm_set_ps(pixels[2],pixels[1],pixels[0],2.0f*pixels[0]-pixels[1]);

        for(int j = 0; j < endcol; j += 4) {
            float* pf = pixels+j;
            __m128 mnewj = _mm_loadu_ps(pf+3);
            __m128 mgradr = _MM_LOAD_ALIGNED(pf-stride);
            __m128 mgradc = _mm_sub_ps(_mm_shuffle_ps(mprevj,mnewj,0x4e),mprevj);
            mgradr = _mm_sub_ps(mgradr,_MM_LOAD_ALIGNED(pf+stride));

            __m128 mrad = _mm_sqrt_ps(_mm_add_ps(_mm_mul_ps(mgradr,mgradr),_mm_mul_ps(mgradc,mgradc)));
            __m128 morient = atan2f4(mgradr,mgradc);

            _MM_STORE_ALIGNED(pfgrad+j,mrad);
            mprevj = mnewj;
            _MM_STORE_ALIGNED(pforient+j,morient);
        }

        assert( i != 0 && i != rows-1 );
        // compute the rest the old way
        for(int j = endcol; j < cols; ++j) {
            if( j == cols-1 )
                fdiffc = 2.0f*(pixels[j]-pixels[j-1]);
            else
                fdiffc = pixels[j+1] - pixels[j-1];

            fdiffr = pixels[-stride+j] - pixels[stride+j];

            pfgrad[j] = sqrtf(fdiffc*fdiffc + fdiffr*fdiffr);
            pforient[j] = atan2f(fdiffr,fdiffc);
        }
    }

    {
        float fdiffc, fdiffr;
        float* pixels = _pixels + (rows-1)*stride;
        float* pfgrad = _pfgrad + (rows-1)*stride;
        float* pforient = _pforient + (rows-1)*stride;

        // last row is special 2*(pixels[stride*(cols-1)]-pixels[stride*(cols-2)])
        // first and last elt is 2*([1]-[0]), have to improvise for sse
        __m128 mprevj = _mm_set_ps(pixels[2],pixels[1],pixels[0],2.0f*pixels[0]-pixels[1]);

        for(int j = 0; j < endcol; j += 4) {
            float* pf = pixels+j;
            __m128 mnewj = _mm_loadu_ps(pf+3);
            __m128 mgradr = _MM_LOAD_ALIGNED(pf-stride);
            __m128 mgradc = _mm_sub_ps(_mm_shuffle_ps(mprevj,mnewj,0x4e),mprevj);
            mgradr = _mm_sub_ps(mgradr,_MM_LOAD_ALIGNED(pf));
            mgradr = _mm_add_ps(mgradr,mgradr);

            __m128 mrad = _mm_sqrt_ps(_mm_add_ps(_mm_mul_ps(mgradr,mgradr),_mm_mul_ps(mgradc,mgradc)));
            __m128 morient = atan2f4(mgradr,mgradc);

            _MM_STORE_ALIGNED(pfgrad+j,mrad);
            mprevj = mnewj;
            _MM_STORE_ALIGNED(pforient+j,morient);
        }

        // compute the rest the old way
        for(int j = endcol; j < cols; ++j) {
            if( j == cols-1 )
                fdiffc = 2.0f*(pixels[j]-pixels[j-1]);
            else
                fdiffc = pixels[j+1] - pixels[j-1];

            fdiffr = 2.0f*(pixels[-stride+j] - pixels[j]);

            pfgrad[j] = sqrtf(fdiffc*fdiffc + fdiffr*fdiffr);
            pforient[j] = atan2f(fdiffr,fdiffc);
        }
    }
}
#endif

int LocalMaxMin(float fval, Image imdiff, int rowstart, int colstart)
{
    int stride = imdiff->stride;
    float* pixels = imdiff->pixels;

    if( fval > 0 ) {
        for( int row = rowstart-1; row <= rowstart+1; ++row) {
            float* pf = pixels + row*stride + colstart - 1;
            if( pf[0] > fval || pf[1] > fval || pf[2] > fval )
                return 0;
        }
    }
    else {
        for( int row = rowstart-1; row <= rowstart+1; ++row) {
            float* pf = pixels + row*stride + colstart-1;
            if( fval > pf[0] || fval > pf[1] || fval > pf[2] )
                return 0;
        }
    }

    return 1;
}

int NotOnEdge(Image imdiff, int row, int col)
{
    // probably wrong, something to do with plane normals?
    int stride = imdiff->stride;
    float* pixels = imdiff->pixels + row*stride;
    float f1 = pixels[-stride+col] - pixels[col]*2 + pixels[stride+col];
    float f2 = pixels[col-1] - pixels[col]*2 + pixels[col+1];
    float f3 = pixels[stride+col+1] - pixels[stride+col-1];
    float f4 = pixels[-stride+col+1] - pixels[-stride+col-1];
    float f5 = (f3 - f4)*0.25f;
    float f6 = f1*f2 - f5*f5;
    float f8 = f1+f2;
    return f6*11*11 > f8*f8*10;
}

Keypoint InterpKeyPoint(Image* imdiff, int index, int rowstart, int colstart,
                        Image imgrad, Image imorient, char* pMaxMinArray, float fscale,Keypoint keypts, int steps, unsigned int nbScales)
{
    float X[3];
    float fquadvalue = FitQuadratic(X, imdiff, index, rowstart, colstart);

    int newrow = rowstart;
    int newcol = colstart;
    if( X[1] > 0.6f && rowstart < imdiff[0]->rows-3 )
        newrow++;
    if( X[1] < -0.6f && rowstart>3 )
        newrow--;
    if( X[2] > 0.6f && colstart < imdiff[0]->cols-3 )
        newcol++;
    if( X[2] < -0.6f && colstart>3 )
        newcol--;

    // check if local min/max is stable at (newrow,newcol). If not and steps haven't surprassed,
    // recompute at new location
    if( steps > 0 && (newrow != rowstart || newcol != colstart) )
      return InterpKeyPoint(imdiff,index,newrow,newcol,imgrad,imorient,pMaxMinArray,fscale,keypts,steps-1,nbScales);

    if(fabsf(X[0]) <= 1.5f && fabsf(X[1]) <= 1.5f && fabsf(X[2]) <= 1.5f && fabsf(fquadvalue) >= PeakThresh ) {

        char* pmaxmin = pMaxMinArray + rowstart*imgrad->cols+colstart;
        bool bgetkeypts = false;
        #pragma omp critical
        {
            if( !pmaxmin[0] ) {
                bgetkeypts = true;
                pmaxmin[0] = 1;
            }
        }

        if( bgetkeypts ) {
            float fSize = InitSigma * powf(2.0f,((float)index + X[0])/(float)nbScales);
            return AssignOriHist(imgrad,imorient,fscale,fSize,(float)rowstart+X[1],(float)colstart+X[2],keypts);
        }
    }

    return keypts;
}

// fits a quadratic to a 3x3x3 box, returns the value of the quadratic at the center
float FitQuadratic(float* X, Image* imdiff, int index, int r, int c)
{
    float H[9];
    int stride = imdiff[index-1]->stride;
    assert( stride == imdiff[index]->stride && stride == imdiff[index+1]->stride );
    float* pixels0 = imdiff[index-1]->pixels + r*stride;
    float* pixels1 = imdiff[index]->pixels + r*stride;
    float* pixels2 = imdiff[index+1]->pixels + r*stride;

    float Y[3];
    Y[0] = 0.5f*(pixels2[c] - pixels0[c]);
    Y[1] = 0.5f*(pixels1[stride+c] - pixels1[-stride+c]);
    Y[2] = 0.5f*(pixels1[c+1] - pixels1[c-1]);
    H[0] = pixels0[c] - 2.0f*pixels1[c] + pixels2[c];
    H[4] = pixels1[-stride+c] - 2.0f*pixels1[c] + pixels1[stride+c];
    H[8] = pixels1[c-1] - 2.0f*pixels1[c] + pixels1[c+1];
    H[3] = H[1] = 0.25f*((pixels2[stride+c] - pixels2[-stride+c]) - (pixels0[stride+c] - pixels0[-stride+c]));
    H[6] = H[2] = 0.25f*((pixels2[c+1] - pixels2[c-1]) - (pixels0[c+1] - pixels0[c-1]));
    H[7] = H[5] = 0.25f*((pixels1[stride+c+1] - pixels1[stride+c-1]) - (pixels1[-stride+c+1] - pixels1[-stride+c-1]));

    X[0] = -Y[0]; X[1] = -Y[1]; X[2] = -Y[2];
    SolveLinearSystem(X,H,3); // writes answer to X?
    return pixels1[c] + 0.5f * (X[0]*Y[0] + X[1]*Y[1] + X[2]*Y[2]);
}

// solve for X in H*X = Y using Gauss-Jordan method
// write the result back in Y
void SolveLinearSystem(float* Y, float* H, int dim)
{
    float fmax;
    int bestj = 0;

    for(int i = 0; i < dim-1; ++i) {

        fmax = -1;
        for(int j = i; j < dim; ++j) {
            //if( i < dim ) {
            float f = H[j*dim+i];
            if( f < 0 )
                f = -f;
            if( f > fmax ) {
                fmax = f;
                bestj = j;
            }
        }

        if( bestj != i ) {
            for(int j = 0; j < dim; ++j)
                swap(H[bestj*dim+j], H[i*dim+j]);
            swap(Y[bestj],Y[i]);
        }

        for(int j = i+1; j < dim; ++j) {
            float f = H[j*dim+i]/H[i*dim+i];
            for(int k = i; k < dim; ++k)
                H[j*dim+k] -= f*H[i*dim+k];
            Y[j] -= Y[i]*f;
        }
    }

    // extract solution
    for(int i = dim-1; i >= 0; --i) {
        for(int j = dim-1; j > i; --j)
            Y[i] -= Y[j]*H[i*dim+j];
        Y[i] /= H[i*dim+i];
    }
}

Keypoint AssignOriHist(Image imgrad, Image imorient, float fscale, float fSize,
                       float frowstart, float fcolstart, Keypoint keypts)
{
    int rowstart = (int)(frowstart+0.5f);
    int colstart = (int)(fcolstart+0.5f);
    int rows = imgrad->rows, cols = imgrad->cols, stride = imgrad->stride;
    float SIFT_ALIGNED16(hists[36]);
    float fexpmult = -1.0f / (2.0f*1.5f*1.5f*fSize*fSize);
    memset(hists,0,sizeof(hists));

    const float fbinmult = 36.0f/(2*PI);
    const float fbinadd = (float)(PI+0.001f)*fbinmult;

    // gather votes for orientation
    int windowsize = (int)(fSize*1.5f*3.0f);
    float* pfgrad = imgrad->pixels + (rowstart-windowsize)*stride, *pforient = imorient->pixels;
    for( int rowcur = rowstart-windowsize; rowcur <= rowstart+windowsize; ++rowcur, pfgrad += stride ) {

        if( rowcur < 0 || rowcur >= rows-2 )
            continue;

        for( int colcur = colstart-windowsize; colcur <= colstart+windowsize; ++colcur ) {

            if( colcur < 0 || colcur >= cols-2 )
                continue;

            float fdx = pfgrad[colcur];
            if( fdx > 0 ) {
                float fdrow = (float)rowcur-frowstart, fdcol = (float)colcur-fcolstart;
                float fradius2 = fdrow*fdrow+fdcol*fdcol;

                if( (float)(windowsize*windowsize) + 0.5f > fradius2 ) {
                    float fweight = expf(fradius2*fexpmult);
                    int binindex = (int)(pforient[rowcur*stride+colcur]*fbinmult+fbinadd);

                    // there is a bug in pforient where it could be 2*PI sometimes
                    if( binindex > 36 ) {
                        //if( binindex != 54 )
                            fprintf(stderr,"bin %d\n",binindex);
                        binindex = 0;
                    }

                    assert( binindex >= 0 && binindex <= 36 );
                    if( binindex == 36 )
                        binindex = 35;

                    hists[binindex] += fdx*fweight;
                }
            }
        }
    }

    // pick an orientation with the highest votes
    for(int i = 0; i < 6; ++i)
        SmoothHistogram(hists,36);

#ifdef __SSE__
    float SIFT_ALIGNED16(fmaxval);
    __m128 m0 = _mm_load_ps(&hists[0]);
    __m128 m1 = _mm_load_ps(&hists[4]);
    __m128 m2 = _mm_load_ps(&hists[8]);
    __m128 m3 = _mm_load_ps(&hists[12]);
    m0 = _mm_max_ps(m0,m1);
    m2 = _mm_max_ps(m2,m3);
    __m128 m4 = _mm_load_ps(&hists[16]);
    __m128 m5 = _mm_load_ps(&hists[20]);
    __m128 m6 = _mm_load_ps(&hists[24]);
    __m128 m7 = _mm_load_ps(&hists[28]);
    m4 = _mm_max_ps(m4,m5);
    m6 = _mm_max_ps(m6,m7);
    m0 = _mm_max_ps(m0,_mm_load_ps(&hists[32]));
    m2 = _mm_max_ps(m2,_mm_max_ps(m4,m6));
    m0 = _mm_max_ps(m0,m2);
    m0 = _mm_max_ps(m0, _mm_shuffle_ps(m0,m0,0x4e));
    _mm_store_ss(&fmaxval,_mm_max_ps(m0, _mm_shuffle_ps(m0,m0,0x11)));
#else
    float fmaxval = 0;
    for(int i = 0; i < 36; ++i) {
        if( hists[i] > fmaxval )
            fmaxval = hists[i];
    }
#endif

    fmaxval *= 0.8f;
    const float foriadd = 0.5f*2*PI/36.0f - PI, forimult = 2*PI/36.0f;

    int previndex = 35;
    for(int index = 0; index < 36; ++index) {
        if( index != 0 )
            previndex = index-1;

        int nextindex = 0;
        if( index != 35 )
            nextindex = index+1;

        if( hists[index] <= hists[previndex] || hists[index] <= hists[nextindex] || hists[index] < fmaxval )
            continue;

        float fpeak = InterpPeak(hists[previndex],hists[index],hists[nextindex]);
        float forient = (index + fpeak)*forimult + foriadd;
        assert( forient >= -PI && forient <= PI ); // error, bad orientation

        keypts = MakeKeypoint(imgrad,imorient,fscale,fSize,frowstart,fcolstart,forient,keypts);
    }

    return keypts;
}

float InterpPeak(float f0, float f1, float f2)
{
    if( f1 < 0 ) {
        f0 = -f0;
        f1 = -f1;
        f2 = -f2;
    }
    assert( f1 >= f0 && f1 > f2 );
    return 0.5f*(f0 - f2) / (f0 - 2.0f*f1 + f2);
}

void SmoothHistogram(float* phist, int numbins)
{
    float ffirst = phist[0];
    float fprev = phist[numbins-1];
    for(int i = 0; i < numbins-1; ++i) {
        float forg = phist[i];
        phist[i] = (fprev + forg + phist[i+1])*0.33333333f;
        fprev = forg;
    }

    // TODO: replace phist[0] with ffirst
    phist[numbins-1] = (fprev+phist[numbins-1]+ffirst)*0.3333333f;
}

Keypoint MakeKeypoint(Image imgrad, Image imorient, float fscale, float fSize,
                      float frowstart,float fcolstart,float forient, Keypoint keypts)
{
    Keypoint pnewkeypt;
    #pragma omp critical
    {
        if( s_listKeypoints.size() > 0 ) {
            pnewkeypt = s_listKeypoints.back();
            s_listKeypoints.pop_back();
        }
        else
            pnewkeypt = (Keypoint)sift_aligned_malloc(sizeof(KeypointSt),16);
    }

    pnewkeypt->next = keypts;
    pnewkeypt->ori = forient;
    pnewkeypt->row = fscale*frowstart;
    pnewkeypt->col = fscale*fcolstart;
    pnewkeypt->scale = fscale*fSize;
    MakeKeypointSample(pnewkeypt,imgrad,imorient,fSize,frowstart,fcolstart);

    return pnewkeypt;
}

void MakeKeypointSample(Keypoint pkeypt, Image imgrad, Image imorient,
                        float fSize, float frowstart, float fcolstart)
{
    float* fdesc = pkeypt->descrip;
    memset(fdesc,0,sizeof(float)*128);
    KeySample(fdesc, pkeypt, imgrad, imorient, fSize, frowstart, fcolstart);

#ifdef __SSE__
    __m128 maccum0 = _mm_load_ps(fdesc);
    __m128 maccum1 = _mm_load_ps(fdesc+4);
    maccum0 = _mm_mul_ps(maccum0,maccum0);
    maccum1 = _mm_mul_ps(maccum1,maccum1);
    for(int i = 8; i < 128; i += 8 ) {
        __m128 m0 = _mm_load_ps(fdesc+i);
        __m128 m1 = _mm_load_ps(fdesc+i+4);
        maccum0 = _mm_add_ps(maccum0,_mm_mul_ps(m0,m0));
        maccum1 = _mm_add_ps(maccum1,_mm_mul_ps(m1,m1));
    }

    maccum0 = _mm_add_ps(maccum0,maccum1);
#ifdef __SSE3__
    maccum0 = _mm_hadd_ps(maccum0,maccum0);
    maccum0 = _mm_hadd_ps(maccum0,maccum0);
#else
    maccum0 = _mm_add_ps(maccum0,_mm_shuffle_ps(maccum0,maccum0,0x4e));
    maccum0 = _mm_add_ss(maccum0,_mm_shuffle_ps(maccum0,maccum0,0x55));
#endif

    float fthresh;
    float SIFT_ALIGNED16(flength2);
    _mm_store_ss(&flength2, maccum0);
    fthresh = 0.2f*sqrtf(flength2);

    for(int i = 0; i < 128; ++i) {
        if( fdesc[i] > fthresh ) {
            flength2 += fthresh*fthresh-fdesc[i]*fdesc[i];
            fdesc[i] = fthresh;
        }
    }

    // normalizing
    float flength = 1.0f/sqrtf(flength2);
    maccum0 = _mm_load1_ps(&flength);
    for(int i = 0; i < 128; i += 16 ) {
        __m128 m0 = _mm_load_ps(fdesc+i);
        __m128 m1 = _mm_load_ps(fdesc+i+4);
        __m128 m2 = _mm_load_ps(fdesc+i+8);
        __m128 m3 = _mm_load_ps(fdesc+i+12);
        _mm_store_ps(fdesc+i,_mm_mul_ps(m0,maccum0));
        _mm_store_ps(fdesc+i+4,_mm_mul_ps(m1,maccum0));
        _mm_store_ps(fdesc+i+8,_mm_mul_ps(m2,maccum0));
        _mm_store_ps(fdesc+i+12,_mm_mul_ps(m3,maccum0));
    }

    // converting to unsigned char
//    float flength = 512.0f/sqrtf(flength2);
//    maccum0 = _mm_load1_ps(&flength);
//    unsigned char* pkeydesc = pkeypt->descrip;
//
//    for(int i = 0; i < 128; i += 16 ) {
//        __m128 m0 = _mm_load_ps(fdesc+i);
//        __m128 m1 = _mm_load_ps(fdesc+i+4);
//        __m128 m2 = _mm_load_ps(fdesc+i+8);
//        __m128 m3 = _mm_load_ps(fdesc+i+12);
//        __m128i mi0 = _mm_cvttps_epi32(_mm_mul_ps(m0,maccum0));
//        __m128i mi1 = _mm_cvttps_epi32(_mm_mul_ps(m1,maccum0));
//        __m128i mi2 = _mm_cvttps_epi32(_mm_mul_ps(m2,maccum0));
//        __m128i mi3 = _mm_cvttps_epi32(_mm_mul_ps(m3,maccum0));
//        _mm_store_si128((__m128i*)(pkeydesc+i), _mm_packus_epi16(_mm_packs_epi32(mi0,mi1),_mm_packs_epi32(mi2,mi3)));
//    }
#else
    NormalizeVec(fdesc,128);

    bool brenormalize = false;
    for(int i = 0; i < 128; ++i) {
        if( fdesc[i] > 0.2f ) {
            fdesc[i] = 0.2f;
            brenormalize = true;
        }
    }

    if( brenormalize )
        NormalizeVec(fdesc,128);
#endif
}

void NormalizeVec(float* pf, int num)
{
    assert( (num&3) == 0 );
    float faccum = 0;
    for(int i = 0; i < num; ++i)
        faccum += pf[i]*pf[i];
    faccum = 1/sqrtf(faccum);
    for(int i = 0; i < num; ++i)
        pf[i] *= faccum;
}

void KeySample(float* fdesc, Keypoint pkeypt, Image imgrad, Image imorient,
               float fSize, float frowstart, float fcolstart)
{
    int rowstart = (int)(frowstart+0.5f);
    int colstart = (int)(fcolstart+0.5f);
    float sinang = sinf(pkeypt->ori), cosang = cosf(pkeypt->ori);
    float fdrow = frowstart-(float)rowstart;
    float fdcol = fcolstart-(float)colstart;
    float frealsize = 3.0f*fSize;
    float firealsize = 1.0f/(3.0f*fSize);
    int windowsize = (int)(frealsize*SQRT2*5.0f*0.5f+0.5f);

    float fsr = sinang*firealsize, fcr = cosang*firealsize, fdrr = -fdrow*firealsize, fdcr = -fdcol*firealsize;

    for(int row = -windowsize; row <= windowsize; ++row) {
//#ifdef _OPENMP
//        float SIFT_ALIGNED16(fnewdesc[128]) = {0};
//        bool badd = false;
//#else
        float* fnewdesc = fdesc;
//#endif

        float frow = (float)row;
        float fcol = -(float)windowsize;
        for(int col = -windowsize; col <= windowsize; ++col, fcol += 1) {
            float rpos = fsr*fcol + fcr*frow + fdrr;
            float cpos = fcr*fcol - fsr*frow + fdcr;
            float rx = rpos + (2.0f - 0.5f);
            float cx = cpos + (2.0f - 0.5f);

            if( rx > -0.9999f && rx < 3.9999f && cx > -0.9999f && cx < 3.9999f ) {
                AddSample(fnewdesc, pkeypt, imgrad, imorient, rowstart+row, colstart+col, rpos, cpos, rx, cx);
//#ifdef _OPENMP
//                badd = true;
//#endif
            }
        }

//#ifdef _OPENMP
//        if( badd ) {
//            #pragma omp critical
//            {
//#ifdef __SSE__
//                for(int j = 0; j < 128; j += 16) {
//                    _mm_store_ps(&fdesc[j], _mm_add_ps(_mm_load_ps(&fdesc[j]), _mm_load_ps(&fnewdesc[j])));
//                    _mm_store_ps(&fdesc[j+4], _mm_add_ps(_mm_load_ps(&fdesc[j+4]), _mm_load_ps(&fnewdesc[j+4])));
//                    _mm_store_ps(&fdesc[j+8], _mm_add_ps(_mm_load_ps(&fdesc[j+8]), _mm_load_ps(&fnewdesc[j+8])));
//                    _mm_store_ps(&fdesc[j+12], _mm_add_ps(_mm_load_ps(&fdesc[j+12]), _mm_load_ps(&fnewdesc[j+12])));
//                }
//#else
//                for(int j = 0; j < 128; ++j)
//                    fdesc[j] += fnewdesc[j];
//#endif
//            }
//        }
//#endif
    }
}

void AddSample(float* fdesc, Keypoint pkeypt, Image imgrad, Image imorient, int r, int c,
               float rpos, float cpos, float rx, float cx)
{
    int rows = imgrad->rows, cols = imgrad->cols, stride = imgrad->stride;
    if( r < 0 || r >= rows || c < 0 || c >= cols )
        return;

    float fgrad = imgrad->pixels[r*stride+c] * expf(-0.125f*(rpos*rpos+cpos*cpos));
    float forient = imorient->pixels[r*stride+c] - pkeypt->ori;
    while( forient > 2*PI )
        forient -= 2*PI;
    while( forient < 0 )
        forient += 2*PI;

//    if( isnan(forient) )
//        fprintf(stderr,"%f %f (%d,%d,%d,%d)\n", imorient->pixels[r*stride+c],pkeypt->ori,r,c,rows,cols);

    PlaceInIndex(fdesc, fgrad, forient, rx, cx);
}

void PlaceInIndex(float* fdesc, float mag, float ori, float rx, float cx)
{
    float oribin = ori*(8.0f/(2*(float)PI));
    int newrow, newcol, neworient;
    float rfrac, cfrac, ofrac;
    if( rx < 0 ) {
        newrow = (int)(rx-1);
    }
    else {
        newrow = (int)rx;
    }
    rfrac = rx-(float)newrow;

    if( cx < 0 )
        newcol = (int)(cx-1);
    else
        newcol = (int)cx;
    cfrac = cx - (float)newcol;

    if( oribin < 0 )
        neworient = (int)(oribin-1);
    else
        neworient = (int)oribin;
    ofrac = oribin-(float)neworient;

    assert( newrow >= -1 && newrow < 4 && neworient >= 0 && neworient <= 8 && rfrac >= 0 && rfrac < 1);

    for(int i = 0; i < 2; ++i) {
        if( (unsigned int)(i+newrow) >= 4 )
            continue;

        float frowgrad;
        if( i == 0 )
            frowgrad = mag*(1-rfrac);
        else
            frowgrad = mag*rfrac;

        for(int j = 0; j < 2; ++j) {
            if( (unsigned int)(j+newcol) >= 4 )
                continue;

            float fcolgrad;
            if( j == 0 )
                fcolgrad = frowgrad*(1-cfrac);
            else
                fcolgrad = frowgrad*cfrac;

            float* pfdescorient = fdesc + 8*(4*(i+newrow)+j+newcol);
            for(int k = 0; k < 2; ++k) {
                float forigrad;
                if( k == 0 )
                    forigrad = fcolgrad*(1-ofrac);
                else
                    forigrad = fcolgrad*ofrac;

                pfdescorient[(neworient+k)&7] += forigrad;
            }
        }
    }
}

void FreeKeypoints(Keypoint keypt)
{
    while(keypt != NULL) {
        s_listKeypoints.push_back(keypt);
        keypt = keypt->next;
    }
}

void DestroyAllResources()
{
    DestroyAllImages();
    for( map<float, float* >::iterator it = s_mapkernel.begin(); it != s_mapkernel.end(); ++it)
        sift_aligned_free(it->second-1);
    s_mapkernel.clear();
#ifdef __SSE__
    for(LISTBUF::iterator it = s_listconvbuf.begin(); it != s_listconvbuf.end(); ++it)
        sift_aligned_free(*it);
    s_listconvbuf.clear();
    s_convbufsize = 0;
#endif
    for(list<Keypoint>::iterator it = s_listKeypoints.begin(); it != s_listKeypoints.end(); ++it)
        sift_aligned_free(*it);
    s_listKeypoints.clear();
}

#if !defined(OTB_DISABLE_FAST_FUNCTIONS) && !defined(_MSC_VER) && defined(__SSE__) && !defined(SIMDMATH_H) // copied from libsimdmath

#define DEF_CONST(a,b) static  const vec_float4 a = {b,b,b,b};
#define DEI_CONST(a,b) static  const vec_int4   a = {b,b,b,b};


DEF_CONST(CF4_2414213562373095, 2.414213562373095f)
DEF_CONST(CF4_04142135623730950, 0.4142135623730950f)
DEF_CONST(CF4_805374449538e_2,      8.05374449538e-2f)
DEF_CONST(CF4_138776856032E_1,      1.38776856032E-1f)
DEF_CONST(CF4_199777106478E_1,      1.99777106478E-1f)
DEF_CONST(CF4_333329491539E_1,      3.33329491539E-1f)

#define VEC_F2I(a,b)   asm("cvttps2dq %1, %0":"=x" (a) :"x" (b))
#define VEC_I2F(a,b)   asm("cvtdq2ps  %1, %0":"=x" (a) :"x" (b))
/* definitions for a &= b; etc
gcc generates very slow code for corresponding
vec_float4 C-style expressions
*/
#define VEC_AND(a,b)   asm ("andps %2, %1":"=x" (a) :"0" (a),"x" (b))
#define VEC_NAND(a,b)  asm ("andnps %2, %1":"=x" (a) :"0" (a),"x" (b))
#define VEC_NAND3(a,b,c)  a=(typeof(a))_mm_andnot_ps((vec_float4)(c),(vec_float4)(b))
#define VEC_OR(a,b)    asm ("orps  %2, %1":"=x" (a) :"0" (a),"x" (b))
#define VEC_XOR(a,b)   asm ("xorps %2, %1":"=x" (a) :"0" (a),"x" (b))
#define VEC_SUB(a,b)   asm ("subps %2, %1":"=x" (a) :"0" (a),"x" (b))

#define VEC_GT(a,b)     _mm_cmpgt_ps((vec_float4)a,(vec_float4)b)
#define VEC_LT(a,b)     _mm_cmplt_ps(a,b)
#define VEC_EQ(a,b)     _mm_cmpeq_ps(a,b)
#define VEC_NE(a,b)     _mm_cmpneq_ps(a,b)
#define VEC_GE(a,b)     _mm_cmpge_ps(a,b)
#define VEC_SR(v,n)     _mm_srli_epi32(v,n)
#define VEC_SL(v,n)     _mm_slli_epi32(v,n)


#define vec_re(x)             _mm_rcp_ps(x)
#define vec_sr(x,y)           _mm_srli_epi32(x,y)
#define vec_sl(x,y)           _mm_slli_epi32(x,y)
#define vec_or(a,b)           ((a)|(b))
#define vec_and(a,b)          ((a)&(b))
#define vec_add(a,b)          ((a)+(b))
#define vec_madd(a,b,c)       ((a)*(b)+(c))
#define vec_nmsub(a,b,c)      ((c)-(a)*(b))
#define vec_splat(x,n)        (typeof(x))_mm_shuffle_ps(x,x,_MM_SHUFFLE(n,n,n,n))

DEF_CONST(CF4_0,        0.0f)
DEF_CONST(CF4_2,        2.0f)
DEI_CONST(CI4_SIGN,     static_cast<int>(0x80000000u))
DEF_CONST(CF4__1,       -1.0f)
DEF_CONST(CF4_1,        1.0f)
DEF_CONST(CF4_SMALL,    1.0E-35f)
DEF_CONST(CF4_PIO2F,    1.570796326794896619f)
DEF_CONST(CF4_PIO4F,    0.7853981633974483096f)
DEF_CONST(CF4_PIF,      3.14159265358979323846f)

inline vec_int4  __attribute__((__always_inline__))
      _signf4( vec_float4 x ){
      vec_int4 a = CI4_SIGN;
      VEC_AND(a, x);
      return a;
}

inline vec_float4 __attribute__((__always_inline__))
            _atanf4( vec_float4 x ){
      vec_float4 y, z,z1,z2;
      vec_int4 a1, a2, a3;
      /* make argument positive and save the sign */
      vec_int4 sign = _signf4( x );
      VEC_XOR(x, sign);

      /* range reduction */
      a1 = (vec_int4)VEC_GT (x , CF4_2414213562373095 );
      a2 = (vec_int4)VEC_GT (x , CF4_04142135623730950 );
      a3 = ~a2;
      a2 ^= a1;

      z1 = CF4__1 / (x+CF4_SMALL);
      z2 = (x-CF4_1)/(x+CF4_1);
      VEC_AND(z1, a1);
      VEC_AND(z2, a2);
      VEC_AND(x, a3);
      VEC_OR(x, z1);
      VEC_OR(x, z2);

      y = CF4_PIO2F;
      z1 = CF4_PIO4F;
      VEC_AND(y, a1);
      VEC_AND(z1, a2);
      VEC_OR(y, z1);

      z = x * x;
      y +=
            ((( CF4_805374449538e_2 * z
            - CF4_138776856032E_1) * z
            + CF4_199777106478E_1) * z
            - CF4_333329491539E_1) * z * x
            + x;

      VEC_XOR(y, sign);
      return y;
}

inline vec_float4  __attribute__((__always_inline__))
      atan2f4( vec_float4 y, vec_float4 x ){
      vec_float4 z, w;
      vec_float4 x_neg_PI    = CF4_PIF;
      VEC_AND(x_neg_PI, VEC_GT( CF4_0, x ));
      vec_float4 y_negativ_2 = CF4_2;
      VEC_AND(y_negativ_2, VEC_GT( CF4_0, y ));

      vec_int4 i_x_zero  = (vec_int4)VEC_EQ ( CF4_0, x );
      vec_int4 i_y_zero  = (vec_int4)VEC_EQ ( CF4_0, y );
      vec_float4 x_zero_PIO2 = CF4_PIO2F;
      VEC_AND(x_zero_PIO2, i_x_zero);
      vec_float4 y_zero    = CF4_1;
      VEC_AND(y_zero, i_y_zero);


      w = x_neg_PI *  ( CF4_1  - y_negativ_2 );

      z = _atanf4( y / (x+x_zero_PIO2));
      VEC_AND(z, ~(i_x_zero|i_y_zero));

      return w + z + x_zero_PIO2 * ( CF4_1 - y_zero - y_negativ_2 );
}

#endif
