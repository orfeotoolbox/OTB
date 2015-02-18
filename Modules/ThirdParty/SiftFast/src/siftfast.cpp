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
#include "siftfast.h"

#include <iostream>

#ifdef __FreeBSD__
#  include <sys/time.h>
#else
#  include <sys/timeb.h>    // ftime(), struct timeb
#endif

#ifdef _WIN32
#include <io.h>
#include <fcntl.h>
#endif

#include <cstdlib>
#include <cstdio>
#include <assert.h>

using namespace std;

typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

inline u32 timeGetTime()
{
  u32 millisec = 0;
#ifdef _WIN32
    _timeb t;
    _ftime(&t);
    millisec = (u32)(t.time*1000+t.millitm);
#else
#  ifdef __FreeBSD__
    // tv_sec is multiplied by 1000 to maintain same result as above
    // tv_usec is in micro-seconds, so multiply by 1000 to maintain same
    // result as above
    timeval t;
    gettimeofday(&t, NULL);
    millisec = (u32)(t.tv_sec*1000+t.tv_usec/1000);
#  else
    timeb t;
    ftime(&t);
    millisec = (u32)(t.time*1000+t.millitm);
#  endif
#endif
  return millisec;
}

// code from david lowe
void SkipComments(FILE *fp)
{
    int ch;

    fscanf(fp," "); // Skip white space.
    while ((ch = fgetc(fp)) == '#') {
        while ((ch = fgetc(fp)) != '\n'  &&  ch != EOF)
            ;
        fscanf(fp," ");
    }
    ungetc(ch, fp); // Replace last character read.
}

// code from david lowe
Image ReadPGM(FILE *fp)
{
    int char1, char2, width, height, max, c1, c2, c3, r, c;
    Image image, nextimage;

    char1 = fgetc(fp);
    char2 = fgetc(fp);
    SkipComments(fp);
    c1 = fscanf(fp, "%d", &width);
    SkipComments(fp);
    c2 = fscanf(fp, "%d", &height);
    SkipComments(fp);
    c3 = fscanf(fp, "%d", &max);

    if (char1 != 'P' || char2 != '5' || c1 != 1 || c2 != 1 || c3 != 1 || max > 255) {
        cerr << "Input is not a standard raw 8-bit PGM file." << endl
             << "Use xv or pnmdepth to convert file to 8-bit PGM format." << endl;
        exit(1);
    }

    fgetc(fp);  // Discard exactly one byte after header.

    // Create floating point image with pixels in range [0,1].
    image = CreateImage(height, width);
    for (r = 0; r < height; r++)
        for (c = 0; c < width; c++)
            image->pixels[r*image->stride+c] = ((float) fgetc(fp)) / 255.0;

    //Check if there is another image in this file, as the latest PGM
    // standard allows for multiple images.
    SkipComments(fp);
    if (getc(fp) == 'P') {
        cerr << "ignoring other images" << endl;
        ungetc('P', fp);
        nextimage = ReadPGM(fp);
        //image->next = nextimage;
    }
    return image;
}

int main(int argc, char **argv)
{
#ifdef _WIN32
    // have to set to binary
     _setmode(_fileno(stdin),_O_BINARY);
#endif

    Image image = ReadPGM(stdin);
    Keypoint keypts;
    float fproctime;

    cerr << "Finding keypoints (image " << image->cols << "x" << image->rows << ")..." << endl;
    {
        u32 basetime = timeGetTime();
        keypts = GetKeypoints(image);
        fproctime = (timeGetTime()-basetime)*0.001f;
    }

    // write the keys to the output
    int numkeys = 0;
    Keypoint key = keypts;
    while(key) {
        numkeys++;
        key = key->next;
    }

    cerr << numkeys << " keypoints found in " << fproctime << " seconds." << endl;

    cout << numkeys << " " << 128 << endl;
    key = keypts;
    while(key) {
        cout << key->row << " " << key->col << " " << key->scale << " " << key->ori << endl;

        for(int i = 0; i < 128; ++i) {
            int intdesc = (int)(key->descrip[i]*512.0f);
            assert( intdesc >= 0 );
            
            if( intdesc > 255 )
                intdesc = 255;
            cout << intdesc << " ";
            if( (i&15)==15 )
                cout << endl;
        }
        cout << endl;
        key = key->next;
    }

    FreeKeypoints(keypts);
    DestroyAllResources();

    return 0;
}
