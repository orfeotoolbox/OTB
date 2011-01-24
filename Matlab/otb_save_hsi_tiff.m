function otb_save_hsi_tiff(hsi, path)

otb_save_cube_tiff(hsi.getDataCube(), [path  '/hsi_cube.tif']);
otb_save_matrix_Spectras_tiff(hsi.getEndmembers(), [path '/endmembers.tif']);
otb_save_cube_tiff(hsi.getAbundanceCube(), [path '/abundance.tif']);
otb_save_cube_tiff(hsi.getNoiseCube(), [path '/noise.tif']);

return;


function otb_save_matrix_2D_tiff(matrix, filename)

t = Tiff(filename,'w');
tagstruct.ImageLength = size(matrix,1);
tagstruct.ImageWidth = size(matrix,2);
tagstruct.Photometric = Tiff.Photometric.MinIsBlack;
tagstruct.BitsPerSample = 64;
tagstruct.SampleFormat = 3;
tagstruct.SamplesPerPixel = 1;
tagstruct.RowsPerStrip = 1;
tagstruct.PlanarConfiguration = Tiff.PlanarConfiguration.Chunky;
tagstruct.Software = 'MATLAB';
t.setTag(tagstruct);

t.write(matrix);

return;


% each column of matrix is saved as a multiband pixel
function otb_save_matrix_Spectras_tiff(matrix, filename)

t = Tiff(filename,'w');
tagstruct.ImageLength = 1;
tagstruct.ImageWidth = size(matrix,2);
tagstruct.Photometric = Tiff.Photometric.MinIsBlack;
tagstruct.BitsPerSample = 64;
tagstruct.SampleFormat = 3;
tagstruct.SamplesPerPixel = size(matrix,1);
tagstruct.RowsPerStrip = 1;
tagstruct.PlanarConfiguration = Tiff.PlanarConfiguration.Chunky;
tagstruct.Software = 'MATLAB';
t.setTag(tagstruct);

t.write(reshape(matrix',1,size(matrix,2), size(matrix,1)));

return;

% save a cube as a multiband image
function otb_save_cube_tiff(cube, filename)

t = Tiff(filename,'w');
tagstruct.ImageLength = size(cube,1);
tagstruct.ImageWidth = size(cube,2);
tagstruct.Photometric = Tiff.Photometric.MinIsBlack;
tagstruct.BitsPerSample = 64;
tagstruct.SampleFormat = 3;
tagstruct.SamplesPerPixel = size(cube,3);
tagstruct.RowsPerStrip = 1;
tagstruct.PlanarConfiguration = Tiff.PlanarConfiguration.Chunky;
tagstruct.Software = 'MATLAB';
t.setTag(tagstruct);

t.write(cube);

return;


