      subroutine discre(ta,ha,tr,hr,it,nt,yy,dd,ppp2,ppp1,
     s     zx)
      double precision ta,ha,tr,hr,yy,dd,ppp2,ppp1,zx,dt,ti
      double precision y1,y2,y3,x2,xd,delta,ecart
      double precision xx
      integer it,nt
      if (ha.ge.7.) then
          call print_error
     s    ('check aerosol measurements or plane altitude')
          return
          endif
      if (it.eq.0) then
         dt=1.e-17
         else
         dt=2.*(ta+tr-yy)/(nt-it+1.)
      endif
  99  dt=dt/2.
      ti=yy+dt
      y1=ppp2
      y3=ppp1
  706 y2=(y1+y3)*0.5
      xx=-y2/ha
      if (xx.lt.-18) then
         x2=tr*exp(-y2/hr)
         else
         x2=ta*dexp(xx)+tr*exp(-y2/hr)
         endif
      xd=abs(ti-x2)
      if(xd.lt.0.00001) go to 705
      if(ti-x2) 701,703,703
  701 y3=y2
      go to 706
  703 y1=y2
      go to 706
  705 zx=y2
      delta=1./(1.+ta*hr/tr/ha*exp((zx-ppp1)*(1./hr-1./ha)))
      ecart=0
      if(dd.ne.0) ecart=abs((dd-delta)/dd)
      if((ecart.gt.0.75).and.(it.ne.0)) go to 99
      return
      end
