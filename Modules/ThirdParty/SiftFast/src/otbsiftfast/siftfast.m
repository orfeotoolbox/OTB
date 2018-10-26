% [frames, descriptors] = siftfast(I)
%
% Returns the sift feature of a grayscale image
% I - grayscale in real format, all values have to be in [0,1]
% frames - 4xN matrix of the sift locations each column is (X,Y,Scale,Orientation)
% descriptors - KxN matrix (usually K=128) of the sift descriptors
function [frames, descriptors] = siftfast(I)
