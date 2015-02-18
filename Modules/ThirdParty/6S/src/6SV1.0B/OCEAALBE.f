      subroutine oceaalbe(pws,paw,xsal,pcl,pwl,
     s           brdfalbe)
C INPUT:  pws=speed of wind (in m/s)
C         paw=azim. of sun - azim. of wind (in deg.)
C	  xsal=salinity (in ppt)
C	  pcl=pigment concentration (in mg.m-3)
C         pwl=wavelength of the computation (in micrometer)
C OUTPUT: brdfalbe=the spherical albedo of the ocean 
C
      real Ref(39)
      real pwl,azw,pcl,wl,wspd,C,pws,brdfalbe,w,wlp,paw
      real ref_i,rwc,rw,rogalbe,a,rwb,xsal
      real nr,ni
      integer iwl

c effective reflectance of the whitecaps (Koepke, 1984)
      data Ref/
     &0.220,0.220,0.220,0.220,0.220,0.220,0.215,0.210,0.200,0.190,
     &0.175,0.155,0.130,0.080,0.100,0.105,0.100,0.080,0.045,0.055,
     &0.065,0.060,0.055,0.040,0.000,0.000,0.000,0.000,0.000,0.000,
     &0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000/
C conversion of parameter
      C=pcl
      wspd=pws
      azw=paw
      wl=pwl

C COMPUTE WHITECAPS REFLECTANCE (LAMBERTIAN)
      W=2.95e-06*(wspd**3.52)
      iwl=1+int((wl-0.2)/0.1)
      wlp=0.5+(iwl-1)*0.1
      Ref_i=ref(iwl+1)+(wl-wlp)/0.1*(ref(iwl)-ref(iwl+1))
      Rwc=W*Ref_i
C COMPUTE WATER REFRACTION INDEX
      call indwat(wl,xsal,nr,ni)
C COMPUTE BACKSCATTERED REFLECTANCE FROM THE SEA WATER (LAMBERTIAN)
C  water reflectance below the sea surface
      call morcasiwat(wl,C,Rw)
C SUNGLINT spherical albedo
      call glitalbe(wspd,nr,ni,azw,rogalbe)
C  water reflectance above the sea surface, (albedo re=0.485)
C albedo is a=re is taken from table 2 of Austin,1974,The remote sensing
C of spectral radiance from below the ocean surface, in Optical
C Aspects of Oceanography (N.G. Jerlov and E. Steeman Nielsen,Eds),
C Academic,London,pp. 317-344
      a=0.485
      Rwb=(1.-Rogalbe)*(1.-a)*Rw/(1-a*Rw)
C SPHERICAL ALBEDO OF SEA WATER
      brdfalbe=Rwc+(1-W)*Rogalbe+(1-Rwc)*Rwb
      return
      end
