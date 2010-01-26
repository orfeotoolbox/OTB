      subroutine aero_prof (ta,piz,tr,hr,nt,xmus,
     s   h,ch,ydel,xdel,altc)    

      include "paramdef.inc"
      double precision xdel(0:nt),ydel(0:nt),ch(0:nt),h(0:nt)
      double precision altc(0:nt),ta,piz,tr,hr,xmus
      double precision dz,z_up,dtau_ray,dtau_aer,dtau,dtau_OS

      real alt_z,taer_z,taer55_z,ssa_aer					 

      integer j,i,nt,num_z
      common /aeroprof/ num_z,alt_z(0:nt_p_max),
     &taer_z(0:nt_p_max),taer55_z(0:nt_p_max)     
      


c If the maximum aerosol height is less than 300 km, one additional
c layer is added above with the aerosol optical thickness equal to 0.

      
      if (alt_z(0).lt.300) then
       taer_z(0)=0.0
       num_z=num_z+1
       do i=0,num_z-1
        alt_z(num_z-i)=alt_z(num_z-i-1)
        taer_z(num_z-i)=taer_z(num_z-i-1)
       enddo
      endif
      
      alt_z(0)=300
      ssa_aer=piz


c The atmosphere is divided into nt layers with the same
c (molecular + aerosol) optical thickness.

      dtau_OS=(tr+ta)/nt

      i=0
      dz=0.0001
      h(0)=0.0
      altc(0)=300.0
      z_up=alt_z(0)
      ch(0)=0.5
      ydel(0)=1.0
      xdel(0)=0.0
      j=1
      n=1
      dtau_aer=0.0

 11   i=i+1

      z=alt_z(0)-dz*i
      dtau_ray=tr*(exp(-z/hr)-exp(-z_up/hr))

      dtau_aer=dtau_aer+taer_z(n)*dz/(alt_z(n-1)-alt_z(n))
      if (z.lt.alt_z(n)) n=n+1

      dtau=dtau_ray+dtau_aer
      if (dtau.ge.dtau_OS) then
        altc(j)=z
        h(j)=h(j-1)+dtau
        ch(j)=exp(-h(j)/xmus)/2
        xdel(j)=dtau_aer*ssa_aer/dtau ! aerosol portion in the j-th layer
        ydel(j)=dtau_ray/dtau         ! molecular portion in the j-th layer
c        write(6,*)j,z,dtau_ray,dtau_aer,dtau,(ta+tr)/nt
        j=j+1
        z_up=z
        dtau_aer=0.0
      endif
      if(z.gt.0) goto 11

       altc(nt)=0
       h(nt)=tr+ta
       ch(nt)=exp(-h(nt)/xmus)/2
       xdel(nt)=dtau_aer*ssa_aer/dtau
       ydel(nt)=dtau_ray/dtau

c  checking
c       do j=0,nt
c      write(6,*)j,altc(j),h(j),h(j)-h(j-1)
c       enddo


      return
      end
