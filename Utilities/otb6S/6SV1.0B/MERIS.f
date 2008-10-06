	subroutine meris(iwa)
c
c created: J. Nieke 19/Nov/2002
c MERIS response function values 
c are based on version (ESA sep 2001)
c SChar_MERIS_Band_Shape_files
c Values are interpolated to 2.5nm wavelenght intervals
c 
c
      real s,wlinf,wlsup
      common /sixs_ffu/ s(1501),wlinf,wlsup
      real sr(15,1501),wli(15),wls(15)
      integer iwa,l,i
c band 1 of MERIS (cw=412nm bw=9.98nm)
      DATA (SR(1,L),L=1,1501)/ 61*0.,
     A .0000, .0002, .6080, 1.0000, 1.0000, 1.0000, .3829, .0000, 
     A1432*0./

c band 2 of MERIS (cw=442nm bw=9.97nm)
      DATA (SR(2,L),L=1,1501)/ 73*0.,
     A .0000, .0002, .6396, 1.0000, 1.0000, .9990, .3484, .0000, 
     A1420*0./
c band 3 of MERIS (cw=489nm bw=9.97nm)
      DATA (SR(3,L),L=1,1501)/ 92*0.,
     A .0000, .0003, .7031, 1.0000, 1.0000, .9990, .2829, .0000,
     A1401*0./
c band 4 of MERIS (cw=509nm bw=9.97nm)
      DATA (SR(4,L),L=1,1501)/ 100*0.,
     A .0000, .0004, .7315, 1.0000, 1.0000, .9990, .2545, .0000, 
     A1393*0./ 
c band 5 of MERIS (cw=559nm bw=9.97nm)
      DATA (SR(5,L),L=1,1501)/ 120*0.,
     A .0000, .0007, .7968, 1.0000, 1.0000, .9990, .1922, .0000,
     A1373*0./ 
c band 6 of MERIS (cw=619nm bw=9.97nm)
      DATA (SR(6,L),L=1,1501)/ 144*0.,
     A .0000, .0013, .8530, 1.0000, 1.0000, .9980, .1400, .0000, 
     A1349*0./ 
c band 7 of MERIS (cw=664nm bw=9.98nm)
      DATA (SR(7,L),L=1,1501)/ 162*0.,
     A .0000, .0016, .8760, 1.0000, .9990, .9970, .1210, .0000,
     A1331*0./ 
c band 8 of MERIS (cw=681nm bw=7.49nm)
      DATA (SR(8,L),L=1,1501)/ 169*0.,
     A .0000, .0016, .8800, 1.0000, .9980, .1190, .0000,
     A1325*0./ 
c band 9 of MERIS (cw=708nm bw=9.99nm)
      DATA (SR(9,L),L=1,1501)/ 180*0.,
     A .0000, .1690, .9990, 1.0000, .9990, .8330, .0008, .0000, 
     A1313*0./ 
c band 10 of MERIS (cw=753nm bw=7.49nm)
      DATA (SR(10,L),L=1,1501)/ 198*0.,
     A .0000, .0011, .8680, 1.0000, .9990, .1360, .0000, 
     A1296*0./ 
c band 11 of MERIS (cw=760nm bw=3.74nm)
      DATA (SR(11,L),L=1,1501)/ 202*0.,
     A .0000, .1430, 1.0000, .1390, .0000, 
     A1294*0./ 
c band 12 of MERIS (cw=778nm bw=1.50nm)
      DATA (SR(12,L),L=1,1501)/ 207*0.,
     A .0000, .1340, .9990, 1.0000, .9990, .9990, .9990, .8750,
     A .0012, .0000, 
     A1284*0./ 
c band 13 of MERIS (cw=865nm bw=2.00nm)
      DATA (SR(13,L),L=1,1501)/ 240*0.,
     A .0000, .0002, .7683, 1.0000, .9990, .9980, .9980, .9970,
     A .9970, .9960, .2618, .0000, 
     A1249*0./ 
c band 14 of MERIS (cw=885nm bw=1.00nm)
      DATA (SR(14,L),L=1,1501)/ 250*0.,
     A .0000, .0002, .7294, 1.0000, 1.0000, .9990, .2877, .0000,
     A1243*0./ 
c band 15 of MERIS (cw=900nm bw=1.00nm)
      DATA (SR(15,L),L=1,1501)/ 256*0.,
     A .0000, .0001, .7029, 1.0000, .9990, .9980, .3152, .0000, 
     A1237*0./ 

c channel 1 lower and upper wavelength
      wli(1)=0.4025
      wls(1)=0.42
c channel 2 lower and upper wavelength
      wli(2)=0.4325
      wls(2)=0.45
c channel 3 lower and upper wavelength
      wli(3)=0.48
      wls(3)=0.4975
c channel 4 lower and upper wavelength
      wli(4)=0.5
      wls(4)=0.5175
c channel 5 lower and upper wavelength
      wli(5)=0.55
      wls(5)=0.5675
c channel 6 lower and upper wavelength
      wli(6)=0.61
      wls(6)=0.6275
c channel 7 lower and upper wavelength
      wli(7)=0.655
      wls(7)=0.6725
c channel 8 lower and upper wavelength
      wli(8)=0.6725
      wls(8)=0.6875
c channel 9 lower and upper wavelength
      wli(9)=0.7
      wls(9)=0.7175
c channel 10 lower and upper wavelength
      wli(10)=0.7450
      wls(10)=0.76
c channel 11 lower and upper wavelength
      wli(11)=0.755
      wls(11)=0.765
c channel 12 lower and upper wavelength
      wli(12)=0.7675
      wls(12)=0.79
c channel 13 lower and upper wavelength
      wli(13)=0.85
      wls(13)=0.8775
c channel 14 lower and upper wavelength
      wli(14)=0.875
      wls(14)=0.8925
c channel 15 lower and upper wavelength
      wli(15)=0.89
      wls(15)=0.9075


      do 1 i=1,1501
      s(i)=sr(iwa,i)
    1 continue

      wlinf=wli(iwa)
      wlsup=wls(iwa)
      return
      end  
        
