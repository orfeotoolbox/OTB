#ifndef SIFT_FAST_H
#define SIFT_FAST_H

typedef struct ImageSt {
    int rows, cols;          // Dimensions of image.
    float *pixels;          // 2D array of image pixels.
    int stride;             // how many floats until the next row
                            // (used to add padding to make rows aligned to 16 bytes)
} *SiftFastImage;

typedef struct KeypointSt {
    float row, col;             // Subpixel location of keypoint.
    float scale, ori;           // Scale and orientation (range [-PI,PI])
    float descrip[128];     // Vector of descriptor values
    struct KeypointSt *next;    // Pointer to next keypoint in list.
} *Keypoint;

#ifdef __cplusplus
extern "C" {
#endif

Keypoint GetKeypoints(SiftFastImage porgimage,unsigned int nbScales);
SiftFastImage CreateImage(int rows, int cols);
SiftFastImage CreateImageFromMatlabData(double* pdata, int rows, int cols);
void DestroyAllImages();
void DestroyAllResources();
void FreeKeypoints(Keypoint keypt);

#ifdef __cplusplus
}
#endif

#endif
