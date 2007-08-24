      subroutine enviro (difr,difa,r,palt,xmuv,
     a                   fra,fae,fr)
      real difr, difa, r, palt
      real fae,fra,fr,fae0,fra0,xmuv,xlnv,a0,b0,a1,b1
      real zmin,zmax,xcfr1,xcfr2,xcfa1,xcfa2,xcfa3
      real alt(16),cfr1(16),cfr2(16),cfa1(16),cfa2(16),cfa3(16)
      integer i
      data (alt(i),i=1,16) /0.5,1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,
     s                      10.0,12.0,14.0,16.0,18.0,20.0,60.0/
      data (cfr1(i),i=1,16) /0.730,0.710,0.656,0.606,0.560,0.516,0.473,
     s 0.433,0.395,0.323,0.258,0.209,0.171,0.142,0.122,0.070/
      data (cfr2(i),i=1,16) /2.8,1.51,0.845,0.634,0.524,0.465,0.429,
     s 0.405,0.390,0.386,0.409,0.445,0.488,0.545,0.608,0.868/
      data (cfa1(i),i=1,16) /0.239,0.396,0.588,0.626,0.612,0.505,0.454,
     s 0.448,0.444,0.445,0.444,0.448,0.448,0.448,0.448,0.448/
      data (cfa2(i),i=1,16) /1.40,1.20,1.02,0.86,0.74,0.56,0.46,0.42,
     s 0.38,0.34,0.3,0.28,0.27,0.27,0.27,0.27/
      data (cfa3(i),i=1,16) /9.17,6.26,5.48,5.16,4.74,3.65,3.24,3.15,
     s 3.07,2.97,2.88,2.83,2.83,2.83,2.83,2.83/
c
c     calculation of the environmental function for
c     rayleigh and aerosols contribution.
c
c     this calculation have been done for nadir observation
c     and are corrected of the effect of the view zenith angle.
c
      a0=1.3347
      b0=0.57757
      a1=-1.479
      b1=-1.5275

      if (palt.ge.60.) then
      fae0=1-0.448*exp(-r*0.27)-0.552*exp(-r*2.83)
      fra0=1-0.930*exp(-r*0.080)-0.070*exp(-r*1.100)
      else
      i=0
 10   i=i+1
      if (palt.ge.alt(i)) goto 10
      if ((i.gt.1).and.(i.le.16)) then
         zmin=alt(i-1)
         zmax=alt(i)
         xcfr1=cfr1(i-1)+(cfr1(i)-cfr1(i-1))*(palt-zmin)/(zmax-zmin)
         xcfr2=cfr2(i-1)+(cfr2(i)-cfr2(i-1))*(palt-zmin)/(zmax-zmin)
         xcfa1=cfa1(i-1)+(cfa1(i)-cfa1(i-1))*(palt-zmin)/(zmax-zmin)
         xcfa2=cfa2(i-1)+(cfa2(i)-cfa2(i-1))*(palt-zmin)/(zmax-zmin)
         xcfa3=cfa3(i-1)+(cfa3(i)-cfa3(i-1))*(palt-zmin)/(zmax-zmin)
         endif
      if (i.eq.1) then
         xcfr1=cfr1(1)
         xcfr2=cfr2(1)
         xcfa1=cfa1(1)
         xcfa2=cfa2(1)
         xcfa3=cfa3(1)
         endif
      fra0=1.-xcfr1*exp(-r*xcfr2)-(1.-xcfr1)*exp(-r*0.08)
      fae0=1.-xcfa1*exp(-r*xcfa2)-(1.-xcfa1)*exp(-r*xcfa3)
      endif
c correction of the effect of the view zenith angle
      xlnv=log(xmuv)
      fra=fra0*(xlnv*(1-fra0)+1)
      fae=fae0*((1+a0*xlnv+b0*xlnv*xlnv)+fae0*(a1*xlnv+b1*xlnv*xlnv)+
     sfae0*fae0*((-a1-a0)*xlnv+(-b1-b0)*xlnv*xlnv))
c
      if ((difa+difr).gt.1.E-03) then
         fr=(fae*difa+fra*difr)/(difa+difr)
         else
         fr=1.
         endif
      return
      end
