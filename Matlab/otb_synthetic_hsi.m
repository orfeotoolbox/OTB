function hsi = otb_synthetic_hsi()

J = 5; % nb endmembers
iota = 0.9; % purity index - 0< iota < 1 with iota~<1
snr = 40; % noise
eta = 0; %parameter to control noise : white noise (eta = 0), color noise (eta > 0)
noise_type = 'additive'; % 'additive' or 'poisson'
% verbose = 0;
nbRows = 100;
nbColumns = 100;

libName = 'usgs';
specLib = class_specLib(libName);
hsi = class_lsmmSynthHsi(J, specLib, iota, snr, eta, noise_type, nbRows, nbColumns);

%disp(hsi.getEndmembers());
%disp(hsi.getSpecLib());
%disp(hsi.getNoiseMatrix());
%disp(hsi.getNbEndmembers());
%disp(hsi.getLibraryIdces());
%disp(hsi.nbColumns());

return;