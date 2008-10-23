      subroutine morcasiwat(wl,C,R2)
C Spectral diffuse attenuation coefficient of Case I Waters as Predicted 
C by MOREL within the spectral range 400-700nm (1988, Journal of Geophysical 
C Research, Vol.93, No C9, pp 10749-10768)
C
C input parameters:	wl wavelength (IN MICROMETERS)
C			C  pigment concentration
C output parameter:	R2  reflectance of water
C
C According Morel,1988, we use:
C
C Kd	spectral value of the attenuation coefficient for 
C	 downwelling irradiance
C	 with: Kd=Kw+Xc*C**e
C Kw	spectral value of the diffuse attenuation coefficient 
C	 for pure oceanic water
C Xc, e	spectral coefficients to compute the diffuse attenuation 
C	 coefficient for pigment
C bb	total backscattering coefficient
C	 with: bb=0.5*bw+bbt*b
C bw	spectral value of the molecular scattering coefficient of water
C bbt,b	parameters to compute the scattering coefficients of pigments
C
C R2	reflectance of water below the surface
C	 with: R2=(0.33/u)*(bb/Kd)	where u is depending of R2
C
      real Kw,Kd
      real tKw(61),tXc(61),te(61),tbw(61)
      real wl,c,r2,xc,e,bw,bb,b,bbt,u1,r1,u2,err
      integer iwl

      data tKw/0.0209,0.0200,0.0196,0.0189,0.0183,
     & 0.0182,0.0171,0.0170,0.0168,0.0166,
     & 0.0168,0.0170,0.0173,0.0174,0.0175,
     & 0.0184,0.0194,0.0203,0.0217,0.0240,
     & 0.0271,0.0320,0.0384,0.0445,0.0490,
     & 0.0505,0.0518,0.0543,0.0568,0.0615,
     & 0.0640,0.0640,0.0717,0.0762,0.0807,
     & 0.0940,0.1070,0.1280,0.1570,0.2000,
     & 0.2530,0.2790,0.2960,0.3030,0.3100,
     & 0.3150,0.3200,0.3250,0.3300,0.3400,
     & 0.3500,0.3700,0.4050,0.4180,0.4300,
     & 0.4400,0.4500,0.4700,0.5000,0.5500,
     & 0.6500/
      data tXc/0.1100,0.1110,0.1125,0.1135,0.1126,
     & 0.1104,0.1078,0.1065,0.1041,0.0996,
     & 0.0971,0.0939,0.0896,0.0859,0.0823,
     & 0.0788,0.0746,0.0726,0.0690,0.0660,
     & 0.0636,0.0600,0.0578,0.0540,0.0498,
     & 0.0475,0.0467,0.0450,0.0440,0.0426,
     & 0.0410,0.0400,0.0390,0.0375,0.0360,
     & 0.0340,0.0330,0.0328,0.0325,0.0330,
     & 0.0340,0.0350,0.0360,0.0375,0.0385,
     & 0.0400,0.0420,0.0430,0.0440,0.0445,
     & 0.0450,0.0460,0.0475,0.0490,0.0515,
     & 0.0520,0.0505,0.0440,0.0390,0.0340,
     & 0.0300/
      data te/0.668,0.672,0.680,0.687,0.693,
     & 0.701,0.707,0.708,0.707,0.704,
     & 0.701,0.699,0.700,0.703,0.703,
     & 0.703,0.703,0.704,0.702,0.700,
     & 0.700,0.695,0.690,0.685,0.680,
     & 0.675,0.670,0.665,0.660,0.655,
     & 0.650,0.645,0.640,0.630,0.623,
     & 0.615,0.610,0.614,0.618,0.622,
     & 0.626,0.630,0.634,0.638,0.642,
     & 0.647,0.653,0.658,0.663,0.667,
     & 0.672,0.677,0.682,0.687,0.695,
     & 0.697,0.693,0.665,0.640,0.620,
     & 0.600/
      data tbw/0.0076,0.0072,0.0068,0.0064,0.0061,
     & 0.0058,0.0055,0.0052,0.0049,0.0047,
     & 0.0045,0.0043,0.0041,0.0039,0.0037,
     & 0.0036,0.0034,0.0033,0.0031,0.0030,
     & 0.0029,0.0027,0.0026,0.0025,0.0024,
     & 0.0023,0.0022,0.0022,0.0021,0.0020,
     & 0.0019,0.0018,0.0018,0.0017,0.0017,
     & 0.0016,0.0016,0.0015,0.0015,0.0014,
     & 0.0014,0.0013,0.0013,0.0012,0.0012,
     & 0.0011,0.0011,0.0010,0.0010,0.0010,
     & 0.0010,0.0009,0.0008,0.0008,0.0008,
     & 0.0007,0.0007,0.0007,0.0007,0.0007,
     & 0.0007/
      if (wl.lt.0.400.or.wl.gt.0.700)then
	R2=0.000
	goto 60
      endif

      iwl=1+nint((wl-0.400)/0.005)
      Kw=tKw(iwl)
      Xc=tXc(iwl)
      e=te(iwl)
      bw=tbw(iwl)
C
      if (abs(C).lt.0.0001)then
         bb=0.5*bw
         Kd=Kw
      else
         b=0.30*C**0.62
         bbt=0.002+0.02*(0.5-0.25*alog10(C))*0.550/wl
         bb=0.5*bw+bbt*b
         Kd=Kw+Xc*C**e
      endif

      u1=0.75
      R1=0.33*bb/u1/Kd

 50   u2=0.90*(1.-R1)/(1.+2.25*R1)
      R2=0.33*bb/u2/Kd
      err=abs((R2-R1)/R2)
      if (err.lt.0.0001)goto 60
      R1=R2
      goto 50
 60   return
      end
C
       subroutine indwat(wl,xsal,nr,ni)
C
C input parameters:  wl=wavelength (in micrometers)
C                    xsal=salinity (in ppt), if xsal<0 then 34.3ppt by default
C output parameters: nr=index of refraction of sea water
C                    ni=extinction coefficient of sea water
C
       real twl(62),tnr(62),tni(62)
       real nr,ni,wl,xwl,yr,yi,nrc,nic,xsal
       integer i
C Indices of refraction for pure water from Hale and Querry, 
C Applied Optique, March 1973, Vol. 12,  No. 3, pp. 555-563
       data twl/
     S  0.250,0.275,0.300,0.325,0.345,0.375,0.400,0.425,0.445,0.475,
     S  0.500,0.525,0.550,0.575,0.600,0.625,0.650,0.675,0.700,0.725,
     S  0.750,0.775,0.800,0.825,0.850,0.875,0.900,0.925,0.950,0.975,
     S  1.000,1.200,1.400,1.600,1.800,2.000,2.200,2.400,2.600,2.650,
     S  2.700,2.750,2.800,2.850,2.900,2.950,3.000,3.050,3.100,3.150,
     S  3.200,3.250,3.300,3.350,3.400,3.450,3.500,3.600,3.700,3.800,
     S  3.900,4.000/
        data tnr/
     S  1.362,1.354,1.349,1.346,1.343,1.341,1.339,1.338,1.337,1.336,
     S  1.335,1.334,1.333,1.333,1.332,1.332,1.331,1.331,1.331,1.330,
     S  1.330,1.330,1.329,1.329,1.329,1.328,1.328,1.328,1.327,1.327,
     S  1.327,1.324,1.321,1.317,1.312,1.306,1.296,1.279,1.242,1.219,
     S  1.188,1.157,1.142,1.149,1.201,1.292,1.371,1.426,1.467,1.483,
     S  1.478,1.467,1.450,1.432,1.420,1.410,1.400,1.385,1.374,1.364,
     S  1.357,1.351/
        data tni/
     S  3.35E-08,2.35E-08,1.60E-08,1.08E-08,6.50E-09,
     S  3.50E-09,1.86E-09,1.30E-09,1.02E-09,9.35E-10,
     S  1.00E-09,1.32E-09,1.96E-09,3.60E-09,1.09E-08,
     S  1.39E-08,1.64E-08,2.23E-08,3.35E-08,9.15E-08,
     S  1.56E-07,1.48E-07,1.25E-07,1.82E-07,2.93E-07,
     S  3.91E-07,4.86E-07,1.06E-06,2.93E-06,3.48E-06,
     S  2.89E-06,9.89E-06,1.38E-04,8.55E-05,1.15E-04,
     S  1.10E-03,2.89E-04,9.56E-04,3.17E-03,6.70E-03,
     S  1.90E-02,5.90E-02,1.15E-01,1.85E-01,2.68E-01,
     S  2.98E-01,2.72E-01,2.40E-01,1.92E-01,1.35E-01,
     S  9.24E-02,6.10E-02,3.68E-02,2.61E-02,1.95E-02,
     S  1.32E-02,9.40E-03,5.15E-03,3.60E-03,3.40E-03,
     S  3.80E-03,4.60E-03/
        i=2
 10     if (wl.lt.twl(i)) goto 20
        if (i.lt.62) then
           i=i+1
           goto 10
           endif
 20     xwl=twl(i)-twl(i-1)        
        yr=tnr(i)-tnr(i-1)        
        yi=tni(i)-tni(i-1)        
        nr=tnr(i-1)+(wl-twl(i-1))*yr/xwl
        ni=tni(i-1)+(wl-twl(i-1))*yi/xwl
c 
c Correction to be applied to the index of refraction and to the extinction 
c coefficients of the pure water to obtain the ocean water one (see for 
c example Friedman). By default, a typical sea water is assumed 
c (Salinity=34.3ppt, Chlorinity=19ppt) as reported by Sverdrup. 
c In that case there is no correction for the extinction coefficient between 
c 0.25 and 4 microns. For the index of refraction, a correction of +0.006 
c has to be applied (McLellan). For a chlorinity of 19.0ppt the correction 
c is a linear function of the salt concentration. Then, in 6S users are able 
c to enter the salt concentration (in ppt).
c REFERENCES:
c Friedman D., Applied Optics, 1969, Vol.8, No.10, pp.2073-2078.
c McLellan H.J., Elements of physical Oceanography, Pergamon Press, Inc.,
c        New-York, 1965, p 129.
c Sverdrup H.V. et al., The Oceans (Prentice-Hall, Inc., Englewood Cliffs,
c        N.J., 1942, p 173.

        nrc=0.006
        nic=0.000
        nr=nr+nrc*(xsal/34.3)
	ni=ni+nic*(xsal/34.3)
        return
        end
C
      subroutine sunglint(wspd,nr,ni,azw,ts,tv,fi,rog)
C input parameters:   wspd=speed of the wind (in m/s)
C                     nr=index of refraction of the sea water
C                     ni=extinction coefficient of the sea water
c                     azw=azim. of the sun - azim. of the wind (in deg.)
C                     ts=solar zenith angle (in deg.)
C                     tv=view zenith angle (in deg.)
C                     fi=relative azimuth (sun-satellite)
C output parameters:  rog=reflectance of the sun glint
C
      real pi,fac
      real wspd,nr,ni,ts,tv,fi,rog,azw,phw
      real cs,cv,ss,sv,phi,zx,zy,tantilt,tilt,proba,xe,xn,xe2,xn2
      real coef,cos2chi,coschi,sinchi
      real r1,sigmaC,sigmaU,C21,C03,C40,C04,C22
      pi=atan(1.)*4.
      fac=pi/180.
      phw=azw*fac
      cs=cos(ts*fac)
      cv=cos(tv*fac)
      ss=sin(ts*fac)
      sv=sin(tv*fac)
      phi=fi*fac
      Zx=-sv*sin(phi)/(cs+cv)
      Zy=(ss+sv*cos(phi))/(cs+cv)
      tantilt=sqrt(zx*zx+zy*zy)
      
      tilt=atan(tantilt)
C      write(6,*) "tantilt ",tantilt
C      write(6,*) "tilt " ,tilt
c  Anisotropic Gaussian distribution
c    phw=phi_sun-phi_wind
      sigmaC=0.003+0.00192*wspd
      sigmaU=0.00316*wspd
      C21=0.01-0.0086*wspd
      C03=0.04-0.033*wspd
      C40=0.40
      C22=0.12
      C04=0.23
      xe=(cos(phw)*Zx+sin(phw)*Zy)/sqrt(SigmaC)
      xn=(-sin(phw)*Zx+cos(phw)*Zy)/sqrt(SigmaU)
      xe2=xe*xe
      xn2=xn*xn
      coef=1-C21/2.*(xe2-1)*xn-C03/6.*(xn2-3)*xn
      coef=coef+c40/24.*(xe2*xe2-6*xe2+3)
      coef=coef+C04/24.*(xn2*xn2-6*xn2+3)
      coef=coef+C22/4.*(xe2-1)*(xn2-1)
      proba=coef/2./pi/sqrt(sigmaU)/sqrt(sigmaC)*exp(-(xe2+xn2)/2.)
C      write(6,*) "probaglit:",proba
C      write(6,*) "coef glit:",coef
C      write(6,*) "tilt glit:",tilt
C      write(6,*) "phw glit:",phw
c Compute Fresnel's coefficient R1
      cos2chi=cv*cs+sv*ss*cos(phi)
      if (cos2chi.gt.1.0)cos2chi=0.99999999999
      if (cos2chi.lt.-1.0)cos2chi=-0.99999999999
      coschi=sqrt(0.5*(1+cos2chi))
      sinchi=sqrt(0.5*(1-cos2chi))
      if (coschi.ge.1.0)coschi=0.99999999
      if (coschi.le.-1.0)coschi=-0.999999
       if (sinchi.gt.1.0)sinchi=0.9999999
      if (sinchi.lt.-1.0)sinchi=-0.999999
    
      Call Fresnel(nr,ni,coschi,sinchi,R1)
C Compute Reflectance of the sun glint
      Rog=pi*R1*proba/4./cs/cv/(cos(tilt)**4)
C      write(6,*) "ROg ",Rog,R1,proba
      return
      end
C
C
      Subroutine Fresnel(nr,ni,coschi,sinchi,R1)
C
C to compute the Fresnel's coefficient of reflection (see for
C example M. Born and E. Wolf, Principles of Optics, Pergamon Press, fifth
C edition, 1975, pp 628
C input parameters: nr=index of refraction of the sea water
C                   ni=extinction coefficient of the sea water
C                   coschi & sinchi=cosine and sine of the incident radiation 
C                                   with respect of the wave facet normal.
C output parameter: R1=Fresnel's coefficient for reflection
C
      real nr,ni,a1,a2,u,v,Rr2,Rl2,b1,b2,R1,coschi,sinchi
c absolute value for a1 to get v=0 when ni=0
      a1=abs(nr*nr-ni*ni-sinchi*sinchi)
      a2=sqrt((nr*nr-ni*ni-sinchi*sinchi)**2.+4*nr*nr*ni*ni)
      u=sqrt(0.5*abs(a1+a2))
      v=sqrt(0.5*abs(-a1+a2))
      Rr2=((coschi-u)**2+v*v)/((coschi+u)**2+v*v)
      b1=(nr*nr-ni*ni)*coschi
      b2=2*nr*ni*coschi
      Rl2=((b1-u)**2+(b2+v)**2)/((b1+u)**2+(b2-v)**2)
      R1=(Rr2+Rl2)/2.
C      write(6,*) "fresnel ", R1,u,v,a1,a2
      return
      end
C
C
      subroutine glitalbe(wspd,nr,ni,azw,rge)
C
C To compute the spherical albedo of the sea water. See for example
C Masuda et al., Remote Sens. Environ., 24, 313-329, 1988.
C 
C input parameters: wsp=wind of speed
C                   nr=index of refraction of the sea water
C                   ni=extinction coefficient of the sea water
C                   azw=azim. of sun - azim. of wind (in deg.)
C output parameter: rge=spherical albedo of the sun glint
C
      real nr,ni,azw,phw,rge,q,wspd,prefl,proba,pr,pp,pi,fac
      real sigma,sigmaC,sigmaU,C21,C03,C40,C04,C22
      real costt,hta,htb,hfa,cotb,cota,cofa,diff,coef
      real phin,cosphin,sinphin,costet,tet,sintet
      real costetn,sintetn,tantetn,coschi,sinchi
      real zx,zy,xe,xn,xe2,xn2,fonc0,pond,r1
      integer nta,nfa,ntb,km,i,j

      pi=atan(1.)*4.
      fac=pi/180.
      sigma=0.003+0.00512*wspd
      sigmaC=0.003+0.00192*wspd
      sigmaU=0.00316*wspd
      C21=0.01-0.0086*wspd
      C03=0.04-0.033*wspd
      C40=0.40
      C22=0.12
      C04=0.23
C costt to minimize the time of the computation
c     integration between 1 and costt instead of 1 and 0
      q=50
      costt=1./sqrt(1+q*sigma/4.)
      phw=azw*fac

      prefl=0.
      proba=0.

      ntb=31
      htb=1./float(ntb-1)
c loops on the zenith angle of the emitted radiation
      do km=1,ntb
        costet=(km-1)*htb
	if (costet.lt.0.99999999) then
           tet=acos(costet)
	   else
	   tet=0.0
	   endif
        sintet=sin(tet)
	tet=tet/fac
C	write(6,*) "sintet ",sintet,tet,costet
c Simpson's rules for the angle of the emitted radiation teta
        cotb=2.
        diff=abs(km/2-km/2.)
        if (diff.lt.0.00001)cotb=4.
        if (km.eq.1.or.km.eq.ntb)cotb=1.0
c  loops step for phiN and tetaN (N is the facet unit normal vector)
        if (tet.lt.91)nta=801
        if (tet.lt.81)nta=301
        if (tet.lt.75)nta=101
        if (tet.lt.65)nta=31
        nfa=nta
        hta=(1.-costt)/float(nta-1)
        hfa=pi/float(nfa-1)
c loops on phiN (azimuth angle of the facet normal vector)
        pr=0.
        pp=0.
        do i=1,nfa
         phin=(i-1)*hfa
         cosphin=cos(phin)
         sinphin=sin(phin)
c  Simpson's rules for phin
         cofa=2.
         diff=abs(i/2-i/2.)
         if (diff.lt.0.00001)cofa=4.
         if (i.eq.1.or.i.eq.nfa)cofa=1.0
c loops on tetaN (zenith angle of the facet normal vector)
         do j=1,nta
          costetn=costt+(j-1)*hta
          sintetn=sqrt(abs(1.-costetn*costetn))
          tantetn=sintetn/costetn
c  Simpson's rules for tetaN
          cota=2.
          diff=abs(j/2-j/2.)
          if (diff.lt.0.00001)cota=4.
          if (j.eq.1.or.j.eq.nta)cota=1.0
c Fresnel's reflection coefficient R1
          coschi=costet*costetn+sintet*sintetn*cosphin
C       write(6,*)" coschi ",coschi,sintet,sintetn,cosphin
          if (coschi*coschi.gt.1.0)coschi=0.99999999999
          sinchi=sqrt(1-coschi*coschi)
          if (coschi.lt.0.0)then
            r1=0.
            cota=0.
          else
            Call Fresnel(nr,ni,coschi,sinchi,r1)
          endif
c  Anisotropic Gaussian distribution for wave facets slopes
          Zx=-tantetn*cosphin
          Zy=-tantetn*sinphin
          xe=(cos(phw)*Zx+sin(phw)*Zy)/sqrt(SigmaC)
          xn=(-sin(phw)*Zx+cos(phw)*Zy)/sqrt(SigmaU)
          xe2=xe*xe
          xn2=xn*xn
          coef=1-C21/2.*(xe2-1)*xn-C03/6.*(xn2-3)*xn
          coef=coef+c40/24.*(xe2*xe2-6*xe2+3)
          coef=coef+C04/24.*(xn2*xn2-6*xn2+3)
          coef=coef+C22/4.*(xe2-1)*(xn2-1)
          fonc0=0.5*coschi*coef*exp(-(xe2+xn2)/2.)/(costetn**4)
          pr=pr+r1*fonc0*cofa*cota*cotb
          pp=pp+fonc0*cofa*cota*cotb
C       write(6,*) coef,coschi,xe2 ,xn2," pr ",pr," pp ",pp
         enddo
        enddo
c
        pond=2.*hta*hfa*htb/pi/sqrt(sigmaC)/sqrt(sigmaU)/3./3./3.
C       write(6,*) "pond ",pond," pr ",pr," pp ",pp
        prefl=prefl+pr*pond
        proba=proba+pp*pond
      enddo
C      write(6,*) "proba ",proba," prefl ",prefl
      rge=prefl/proba
      return
      end
