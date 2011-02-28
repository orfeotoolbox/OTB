/*
 * otbCudaFCLSFilter.cu
 *
 */

#include <cstdio>
#include <cuda.h>
#include <driver_types.h>
#include <vector_types.h>

//Block thread size
#define BLOCK_SIZE (16*16)

//#define num_bands 224
//#define num_endmembers 5

#define CUDA_SAFE_CALL( call ) \
  do \
  { \
  cudaError_t status = call; \
  check_error(status); \
  } while(0)

void check_error(cudaError_t error)
{

#define ELSEIF_CUDA_HANDLE_ERROR_CODE( code ) \
  else if(error == code) \
    printf("Cuda error : %d %s\n", error, #code);

  if(error == cudaSuccess) {}
  ELSEIF_CUDA_HANDLE_ERROR_CODE(cudaErrorMissingConfiguration)
  ELSEIF_CUDA_HANDLE_ERROR_CODE(cudaErrorMemoryAllocation)
  ELSEIF_CUDA_HANDLE_ERROR_CODE(cudaErrorInitializationError)
  ELSEIF_CUDA_HANDLE_ERROR_CODE(cudaErrorLaunchFailure)
  ELSEIF_CUDA_HANDLE_ERROR_CODE(cudaErrorPriorLaunchFailure)
  ELSEIF_CUDA_HANDLE_ERROR_CODE(cudaErrorLaunchTimeout)
  ELSEIF_CUDA_HANDLE_ERROR_CODE(cudaErrorLaunchOutOfResources)
  ELSEIF_CUDA_HANDLE_ERROR_CODE(cudaErrorInvalidDeviceFunction)
  ELSEIF_CUDA_HANDLE_ERROR_CODE(cudaErrorInvalidConfiguration)
  ELSEIF_CUDA_HANDLE_ERROR_CODE(cudaErrorInvalidDevice)
  ELSEIF_CUDA_HANDLE_ERROR_CODE(cudaErrorInvalidValue)
  ELSEIF_CUDA_HANDLE_ERROR_CODE(cudaErrorInvalidPitchValue)
  ELSEIF_CUDA_HANDLE_ERROR_CODE(cudaErrorInvalidSymbol)
  ELSEIF_CUDA_HANDLE_ERROR_CODE(cudaErrorMapBufferObjectFailed)
  ELSEIF_CUDA_HANDLE_ERROR_CODE(cudaErrorUnmapBufferObjectFailed)
  ELSEIF_CUDA_HANDLE_ERROR_CODE(cudaErrorInvalidHostPointer)
  ELSEIF_CUDA_HANDLE_ERROR_CODE(cudaErrorInvalidDevicePointer)
  ELSEIF_CUDA_HANDLE_ERROR_CODE(cudaErrorInvalidTexture)
  ELSEIF_CUDA_HANDLE_ERROR_CODE(cudaErrorInvalidTextureBinding)
  ELSEIF_CUDA_HANDLE_ERROR_CODE(cudaErrorInvalidChannelDescriptor)
  ELSEIF_CUDA_HANDLE_ERROR_CODE(cudaErrorInvalidMemcpyDirection)
  ELSEIF_CUDA_HANDLE_ERROR_CODE(cudaErrorAddressOfConstant)

#undef ELSEIF_HANDLE_ERROR_CODE
}


__global__ void UnconstrainedKernel(float* d_image_vector,
                                    float* d_image_unmixed,
                                    float* d_endmembersInv,
                                    int numSamples,
                                    int num_endmembers,
                                    int num_bands)
{
  int x = blockIdx.x * blockDim.x + threadIdx.x;
  
  if (x < numSamples)
    {
    // Unconstrained    
    for(int e = 0; e < num_endmembers; e++)
      {
      d_image_unmixed[e + num_endmembers*x] = 0;
      for (int t = 0; t < num_bands; t++)
        {
        d_image_unmixed[e + num_endmembers*x] += d_endmembersInv[t + e*num_bands] * d_image_vector[ t + num_bands*x ];
        }
      }
    }
}


__global__ void FCLSUKernel(float* d_image_unmixed,int numSamples,
                                    int num_endmembers)
{
  float sum;
  
  int x = blockIdx.x * blockDim.x + threadIdx.x;
  if (x < numSamples)
    {
    sum = 0;
    for(int i = 0; i < num_endmembers; i++)
      {
      sum += d_image_unmixed[ i + num_endmembers*x ];
      }
      
    for (int k = 0; k < num_endmembers; k++)
      {
      d_image_unmixed[k + num_endmembers*x] = d_image_unmixed[k + num_endmembers*x ] / sum;
      }
    }
}


__global__ void UnconstrainedISRAKernel(float* d_image_vector,
                                        float* d_image_unmixed,
                                        float* d_image_unmixed_tmp,
                                        float* d_endmembers, 
                                        float* d_endmembersT,
                                        float* d_endmembersInv,
                                        int numSamples,
                                        int num_endmembers,
                                        int num_bands,
                                        int maxiter)
{
  int x = blockIdx.x * blockDim.x + threadIdx.x;
  
  float numerator = 0;
  float denominator = 0;
  float dot = 0;
  
  if (x < numSamples)
    {
    // Unconstrained    
    for(int e = 0; e < num_endmembers; e++)
      {
      d_image_unmixed[e + num_endmembers*x] = 0;
      for (int t = 0; t < num_bands; t++)
        {
        d_image_unmixed[e + num_endmembers*x] += d_endmembersInv[t + e*num_bands] * d_image_vector[ t + num_bands*x ];
        }
      d_image_unmixed_tmp[e + num_endmembers*x] = d_image_unmixed[e + num_endmembers*x];
      }

    // ISRA
    for(int it = 0; it < maxiter; it++)
      {      
      for(int e = 0; e < num_endmembers; e++)
        {
        numerator = 0;
        denominator = 0;
        
        // For all bands
        for (int k = 0; k < num_bands; k++)
          {
//          numerator = numerator + d_endmembers[k + e*num_bands] * l_pixel[k];
          numerator = numerator + d_endmembers[k + e*num_bands] * d_image_vector[ k + num_bands*x ];;
  
          // Calculate dot product
          dot = 0;
          for (int s = 0; s < num_endmembers; s++)
            {
//            dot += d_endmembersT[s + k*num_endmembers] * l_abu[s];
            dot += d_endmembersT[s + k*num_endmembers] * d_image_unmixed_tmp[s + num_endmembers*x];
            }
  
          denominator += dot * d_endmembers[k + e*num_bands];        
          
          }
  
//        l_abu[e] = l_abu[e] * (numerator/denominator);
//        l_abu[e] = numerator/denominator;
        d_image_unmixed[e + num_endmembers*x] = d_image_unmixed_tmp[e + num_endmembers*x] * (numerator/denominator);
        }
        
      for(int e = 0; e < num_endmembers; e++)
        {
        d_image_unmixed_tmp[e + num_endmembers*x] = d_image_unmixed[e + num_endmembers*x];
        }
      }

    }
}

extern "C" void fclsProcessing( float* d_image_vector, 
                                float* d_image_unmixed,
                                float* d_image_unmixed_tmp,
                                float* d_endmembers,
                                float* d_endmembersT,             
                                float* d_endmembersInv,  
                                int numSamples,
                                int numBands,
                                int nbEndmembers,
                                int maxIter,
                                int blockSize)
{
  dim3 dimBlock( blockSize );
  dim3 dimGrid ( (numSamples + blockSize) / blockSize );
  
  printf( " %d \n " , (numSamples + blockSize) / blockSize );
  
  UnconstrainedKernel<<<dimGrid,dimBlock>>>(d_image_vector, d_image_unmixed, d_endmembersInv, numSamples, nbEndmembers, numBands);
  cudaThreadSynchronize();

  //UnconstrainedISRAKernel<<<dimGrid,dimBlock>>>(d_image_vector, d_image_unmixed, d_image_unmixed_tmp, d_endmembers, d_endmembersT, d_endmembersInv, numSamples, nbEndmembers, numBands, maxIter);
  //cudaThreadSynchronize();
  
  //FCLSUKernel<<<dimGrid,dimBlock>>>(d_image_unmixed, numSamples);
  //cudaThreadSynchronize();
}


extern "C" void fclsMallocEndMembers( 
                            float** d_endmembers,
                            float** d_endmembersT,
                            float** d_endmembersInv,
                            int numBands,
                            int nbEndmembers)
{

  float* d_endmembers_ = 0;
  float* d_endmembersT_ = 0;
  float* d_endmembersInv_ = 0;

  printf( "Allocating %d KB\n", numBands*nbEndmembers*sizeof(float) /  1024 );
  CUDA_SAFE_CALL( cudaMalloc((void**) &d_endmembers_, nbEndmembers*numBands*sizeof(float)));
  CUDA_SAFE_CALL( cudaMemset(d_endmembers_, 0, nbEndmembers*numBands*sizeof(float)));

  printf( "Allocating %d KB\n", numBands*nbEndmembers*sizeof(float) /  1024 );
  CUDA_SAFE_CALL( cudaMalloc((void**) &d_endmembersT_, nbEndmembers*numBands*sizeof(float)));
  CUDA_SAFE_CALL( cudaMemset(d_endmembersT_, 0, nbEndmembers*numBands*sizeof(float)));

  printf( "Allocating %d KB\n", numBands*nbEndmembers*sizeof(float) /  1024 );
  CUDA_SAFE_CALL( cudaMalloc((void**) &d_endmembersInv_, nbEndmembers*numBands*sizeof(float)));
  CUDA_SAFE_CALL( cudaMemset(d_endmembersInv_, 0, nbEndmembers*numBands*sizeof(float)));
  
  
  *d_endmembers = d_endmembers_;
  *d_endmembersT = d_endmembersT_;
  *d_endmembersInv = d_endmembersInv_;
}

extern "C" void fclsMallocImage( float** d_image_vector,
                            float** d_image_unmixed,
                            float** d_image_unmixed_tmp,
                            int imageWidth,
                            int imageHeight,
                            int numBands,
                            int nbEndmembers)
{
  printf( "Allocating %d KB\n", numBands*imageWidth*imageHeight*sizeof(float) /  1024);
  CUDA_SAFE_CALL(cudaMalloc((void**) d_image_vector, numBands*imageWidth*imageHeight*sizeof(float)));
  CUDA_SAFE_CALL( cudaMemset(*d_image_vector, 0, nbEndmembers*numBands*sizeof(float)));

  printf( "Allocating %d KB\n", nbEndmembers*imageWidth*imageHeight*sizeof(float) /  1024);
  CUDA_SAFE_CALL( cudaMalloc((void**) d_image_unmixed, nbEndmembers*imageWidth*imageHeight*sizeof(float)));
  CUDA_SAFE_CALL( cudaMemset(*d_image_unmixed, 0, nbEndmembers*numBands*sizeof(float)));

  printf( "Allocating %d KB\n", nbEndmembers*imageWidth*imageHeight*sizeof(float) /  1024);
  CUDA_SAFE_CALL( cudaMalloc((void**) d_image_unmixed_tmp, nbEndmembers*imageWidth*imageHeight*sizeof(float)));
  CUDA_SAFE_CALL( cudaMemset(*d_image_unmixed_tmp, 0, nbEndmembers*numBands*sizeof(float)));
}

         

extern "C" void fclsCopyHostToDevice( float* d_ptr,
                                      const float* h_ptr,
                                      int nb_bytes)
{
  CUDA_SAFE_CALL( cudaMemcpy(d_ptr, h_ptr, nb_bytes, cudaMemcpyHostToDevice));
}

extern "C" void fclsCopyDeviceToHost( float* h_ptr,
                                      const float* d_ptr,
                                      int nb_bytes)
{
  CUDA_SAFE_CALL( cudaMemcpy(h_ptr, d_ptr, nb_bytes, cudaMemcpyDeviceToHost));
}

extern "C" void fclsFree( float* d_ptr)
{
  CUDA_SAFE_CALL( cudaFree(d_ptr) );
}

extern "C" void fclsInit( void )
{
  if (cuInit(0) != CUDA_SUCCESS)
    exit (0);
}

