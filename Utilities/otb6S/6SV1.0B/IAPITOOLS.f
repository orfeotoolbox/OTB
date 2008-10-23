        real function Ro_1 (Theta_i,Phi_i,Theta_e,Phi_e)
        common /gauss_m/xgm (20),wgm (20),n
        real xgm,wgm
        integer n
        real G_f,Geo,h,gamma_f
        common /p/xLt,Rl,Tl,Rs,c,ild
        real xLt,Rl,Tl,Rs,c
        integer ild
        common /Ro/Ro_1_c,Ro_1_s,Ro_mult
        real Ro_1_c,Ro_1_s,Ro_mult
        real Theta_i,Phi_i,Theta_e,Phi_e,xmui,xmu,xtmu
        real Gi,Ge,Ki,Ke,xLi
        real xmm,xrm
        real xL
        double precision xdb
        integer j
c
        xtmu=1.E-05
        xmui=abs(cos(Theta_i))
        xmu=cos(Theta_e)
        if (abs(xmu).lt.xtmu) xmu=xtmu
c
        Gi=G_f (Theta_i)
        Ge=G_f (Theta_e)
c
        Ki=Gi/xmui
        Ke=Ge/xmu
c
        xmm=0.5*(xLt+0.)
        xrm=0.5*(xLt-0.)
        Ro_1_c=0.
        xLi=c/Geo (Theta_i,Phi_i,Theta_e,Phi_e) 
        do j=1,n
          xL=xmm+xrm*xgm(j)
          xdb=(Ki+Ke*h(xL,xLi))*dble(xL)
          if (abs(xdb).lt.1.E-30) xdb=0.
          if (xdb.le.20) Ro_1_c=Ro_1_c+wgm(j)*xrm*dexp(-xdb)
        enddo
c
        Ro_1_c=Ro_1_c*Gamma_f (Theta_i,Phi_i,
     +                         Theta_e,Phi_e)/xmui/xmu
c
        xdb=(Ki+Ke*h (xLt,xLi))*dble(xLt)
        if (abs(xdb).lt.1.E-30) xdb=0.
        if (xdb.le.20) Ro_1_s=Rs*dexp(-xdb)
c
        Ro_1=Ro_1_c+Ro_1_s
c
        return
        end
c
        real function Gamma_f (Theta_p,Phi_p,Theta,Phi)
        parameter (Pi=3.141592653589793)
        common /p/xLt,Rl,Tl,Rs,c,ild
        real xLt,Rl,Tl,Rs,c,gl
        integer ild
        common /gauss_m/xgm (20),wgm (20),n
        real xgm,wgm
        integer n
        real Theta_p,Phi_p,Theta,Phi
        real xmm,xrm,xt
        real ymm,yrm,yt
        real dp,dpp,f
        real sum
        integer i,j
c
        xmm=0.5*(Pi/2.+0.)
        xrm=0.5*(Pi/2.-0.)
        ymm=0.5*(2.*Pi+0.)
        yrm=0.5*(2.*Pi-0.)
        Gamma_f = 0.
        do j=1,n
          xt=xmm+xrm*xgm (j)
          sum=0.
          do i=1,n
            yt=ymm+yrm*xgm (i)
            dpp = cos (Theta_p)*cos (xt)+
     +            sin (Theta_p)*sin (xt)*cos (Phi_p-yt)
            dp = cos (Theta)*cos (xt)+
     +           sin (Theta)*sin (xt)*cos (Phi-yt)
C correction when porting code to HP730
            if (dp*dpp.lt.0.) then
              f=Rl*abs (dp)/Pi
              else
                  f=Tl*abs (dp)/Pi
            endif
C end of correction
            sum=sum+wgm (i)*xrm*gl (xt)*f*abs (dpp)
          enddo
          Gamma_f=Gamma_f+wgm (j)*yrm*sum
        enddo
        Gamma_f=Gamma_f/2.
        return
        end
c
        real function Geo (Theta_i,Phi_i,Theta_e,Phi_e) 
        real Theta_i,Phi_i,Theta_e,Phi_e
        Geo=sqrt (abs (tan (Theta_i)**2+
     +                 tan (Theta_e)**2-
     +                 2.*tan (Theta_i)*tan (Theta_e)*
     +                    cos (Phi_i-Phi_e)))
        if (Geo.lt.1.e-35) Geo=1.e-35
        return
        end
c
        real function h (xL,xLi)
        parameter (Pi=3.141592653589793)
        real xL,xLi
        if (xL.lt.xLi) then
          h=(1.-4./3./Pi)/xLi*xL
          else
              h=1.-4./3./Pi*xLi/xL
        endif
        return
        end
c
        real function G_f (Theta)
        parameter (Pi=3.141592653589793)
        common /p/xLt,Rl,Tl,Rs,c,ild
        real xLt,Rl,Tl,Rs,c,psi,gl
        integer ild
        common /gauss_m/xgm (20),wgm (20),n
        real xgm,wgm
        integer n
        real Theta
        real xmm,xrm,xt
        integer j
c
        xmm=0.5*(Pi/2.+0.)
        xrm=0.5*(Pi/2.-0.)
        G_f = 0.
        do j=1,n
          xt=xmm+xrm*xgm (j)
          G_f=G_f+wgm (j)*xrm*Psi (Theta,xt)*gl (xt)
        enddo
        return
        end
c
        real function Psi (Theta,xt)
        parameter (Pi=3.141592653589793)
        common /p/xLt,Rl,Tl,Rs,c,ild
        real xLt,Rl,Tl,Rs,c
        integer ild
        real Theta,xt
        real cpt,pt
        real xmu,smu
c
        xmu=cos (xt)
        smu=sin (xt)
        if (xmu.eq.1.) then
          Psi=cos (Theta)
          else
              if (sin (Theta).eq.0.) then
                Psi=xmu
                else
                    if (smu.eq.0.) then
                      cpt=0.
                      else
                          cpt=1.*xmu/smu*cos (Theta)/
     +                                   sin (Theta)
                    endif
                    if (abs (cpt).gt.1.) then
                      Psi=xmu*cos (Theta)
                      else
                          pt=acos (-cpt)
                          Psi=xmu*cos (Theta)*(2./Pi*pt-1.)+
     +                       2./Pi*smu*sin (Theta)*sin (pt)
                    endif
              endif
        endif
        Psi=abs (Psi)
        return
        end
c
        real function gl (Theta)
        parameter (Pi=3.141592653589793)
        common /ld/a_ld,b_ld,c_ld,d_ld
        real a_ld,b_ld,c_ld,d_ld
        real Theta 
c
        gl=a_ld+
     +     b_ld*cos (2.*Theta)+
     +     c_ld*cos (4.*Theta)+
     +     d_ld*sin (Theta)
        return
        end
c
        subroutine gauleg(x1,x2,x,w,n)
        integer n
        real x1,x2,x (n),w (n)
        double precision eps
        parameter (eps=3.d-14)
        integer i,j,m
        double precision p1,p2,p3,pp,xl,xm,z,z1
        m=(n+1)/2
        xm=0.5d00*(x2+x1)
        xl=0.5d00*(x2-x1)
        do i=1,m
          z=cos(3.141592654d00*(i-.25d00)/(n+.5d00))
1         continue
            p1=1.d00
            p2=0.d00
            do j=1,n
              p3=p2
              p2=p1
              p1=((2.d00*j-1.d00)*z*p2-(j-1.d00)*p3)/j
            enddo
            pp=n*(z*p1-p2)/(z*z-1.d00)
            z1=z
            z=z1-p1/pp
          if(abs(z-z1).gt.eps)go to 1
          x(i)=real (xm-xl*z)
          x(n+1-i)=real (xm+xl*z)
          w(i)=real (2.d00*xl/((1.d00-z*z)*pp*pp))
          w(n+1-i)=w(i)
        enddo
        return
        end
c
        subroutine solve (Theta_i)
        parameter (Pi=3.141592653589793)
        parameter (m=20)
        common /gauss_m/xgm (20),wgm (20),n
        real xgm,wgm,g_f
        integer n
        common /p/xLt,Rl,Tl,Rs,c,ild
        real xLt,Rl,Tl,Rs,c
        integer ild
        common /Ro/Ro_1_c,Ro_1_s,Ro_mult
        real Ro_1_c,Ro_1_s,Ro_mult
        real Theta_i,xmui,Gi
        double precision xdb
        common /l/dL,xL
        real dL,xL
        real xI0t,xI1t,xImt
        real xI (m+1,20)
        real Q0d (m),Q0u (m),Q1 (m),S (m),xIf (m+1,20)
        real G (20)
        integer j,k,l
        real xmm,xrm,xmu
        real sum
        integer nc
        real Epsilon
c
        Epsilon=1.e-4
c
        xmui=abs (cos (Theta_i))
c
        dL=xLt/float (m)
c
c - Computation of G-functions
c
        Gi=G_f (Theta_i)
        xmm=0.5*(1.+(-1.))
        xrm=0.5*(1.-(-1.))
        do j=1,n
          xmu=xmm+xrm*xgm (j)
          G (j)=G_f (acos (xmu))
        enddo
c
c - Initialisation of S (k) & xIf (k,j)
c
        do k=1,m
          S (k)=0.
          do j=1,n
            xIf (k,j)=0.
          enddo
        enddo
c
c - Computation of Q0d (k) & Q0u (k) <- first collision source
c
c - (down)
c
        do k=1,m
          xL=(k-.5)*dL
          xdb=Gi/xmui*dble(xL)
          if (abs(xdb).lt.1.E-30) xdb=0.
          if (xdb.lt.20) Q0d(k)=(Rl+Tl)/2.*Gi*dexp(-xdb)
        enddo
c
c - (up)
c       
        xdb=Gi/xmui*dble(xLt)
        if (abs(xdb).lt.1.E-30) xdb=0.
        if (xdb.lt.20) xI0t=2.*Rs*xmui*dexp(-xdb)
        do k=m,1,-1
          xL=(k-.5)*dL
          sum=0.
          do j=n/2+1,n
            xmu=xmm+xrm*xgm (j)
            xdb=dble(G(j))/dble(xmu)*(xLt-xL)
            if (abs(xdb).lt.1.E-30) xdb=0.
            if (xdb.lt.20) sum=sum+wgm(j)*xrm*xI0t*(Rl+Tl)/2.*G(j)*
     +                      dexp(-xdb)
          enddo
          Q0u (k)=sum
        enddo
c
c - Computation of xI (k,j) <- single scattering
c
c - Initialisation of xI (k,j)
c
        do k=1,m+1
          do j=1,n/2
            xI (k,j)=0.
          enddo
        enddo
c
c - (down)
c
        do k=1,m
          do j=1,n/2
            xmu=xmm+xrm*xgm (j)
            xI (k+1,j)=(Q0d (k)-
     +                  xI (k,j)*(G (j)/2.+xmu/dL))/
     +                           (G (j)/2.-xmu/dL)
          enddo
        enddo
c
c - (boundary condition)
c
        xI1t=0.
        do j=1,n/2
          xmu=xmm+xrm*xgm (j)
          xI1t=xI1t+wgm (j)*xrm*2.*Rs*abs (xmu)*xI (m+1,j)
        enddo
c
        do j=n/2+1,n
          xI (m+1,j)=0.
        enddo
c
c - (up)
c
        do k=m,1,-1
          do j=n/2+1,n
            xmu=xmm+xrm*xgm (j)
            xI (k,j)=(Q0d (k)-
     +                xI (k+1,j)*(G (j)/2.-xmu/dL))/
     +                           (G (j)/2.+xmu/dL)
          enddo
        enddo
c
c - Computation of Q1 (k) <- second collision source
c
        do k=1,m
          sum=0.
          do j=1,n
            sum=sum+wgm (j)*xrm*(Rl+Tl)/2.*G (j)*
     +              (xI (k+1,j)+xI (k,j))/2.
          enddo
          Q1 (k)=sum
        enddo
c
c - Computation of xI (k,j) <- multiple scattering
c
c - Initialisation of xI (k,j)
c
        do k=1,m+1
          do j=1,n/2
            xI (k,j)=0.
          enddo
        enddo
        l=0
 1      l=l+1
c
c - (down)
c
        do k=1,m
          do j=1,n/2
            xmu=xmm+xrm*xgm (j)
            xI (k+1,j)=(S (k)+Q0u (k)+Q1 (k)-
     +                  xI (k,j)*(G (j)/2.+xmu/dL))/
     +                           (G (j)/2.-xmu/dL)
          enddo
        enddo
c
c - (boundary condition)
c
        xImt=0.
        do j=1,n/2
          xmu=xmm+xrm*xgm (j)
          xImt=xImt+wgm (j)*xrm*2.*Rs*abs (xmu)*xI (m+1,j)
        enddo
        do j=n/2+1,n
          xI (m+1,j)=xImt+xI1t
        enddo
c
c - (up)
c
        do k=m,1,-1
          do j=n/2+1,n
            xmu=xmm+xrm*xgm (j)
            xI (k,j)=(S (k)+Q0u (k)+Q1 (k)-
     +                xI (k+1,j)*(G (j)/2.-xmu/dL))/
     +                           (G (j)/2.+xmu/dL)
          enddo
        enddo
c
c - End test
c
        nc=0
        do k=1,m+1
          do j=1,n
            if (abs (xIf (k,j)-xI (k,j)).lt.Epsilon) nc=nc+1
            xIf (k,j)=xI (k,j)
          enddo
        enddo
        if ((l.lt.50).and.(nc.ne.(m+1)*n)) then
c
c - Computation of S (k) <- distributed source
c
          do k=1,m
            sum=0.
            do j=1,n
              sum=sum+wgm (j)*xrm*(Rl+Tl)/2.*G (j)*
     +                            (xI (k+1,j)+xI (k,j))/2.
            enddo
            S (k)=sum
          enddo
          goto 1
        endif
c
c - Computation of Ro_mult
c
        sum=0.
        do j=n/2+1,n
          xmu=xmm+xrm*xgm (j)
          sum=sum+wgm (j)*xrm*xI (1,j)*xmu/xmui
        enddo
        Ro_mult=sum
        return
        end


        subroutine lad
        parameter (Pi=3.141592653589793)
        common /p/xLt,Rl,Tl,Rs,c,ild
        real xLt,Rl,Tl,Rs,c
        integer ild
        common /ld/a_ld,b_ld,c_ld,d_ld
        real a_ld,b_ld,c_ld,d_ld
c
        if (ild.eq.1) then
          a_ld=2./Pi
          b_ld=2./Pi
          c_ld=0.
          d_ld=0.
          else
              if (ild.eq.2) then
                a_ld=2./Pi
                b_ld=-2./Pi
                c_ld=0.
                d_ld=0.
                else
                    if (ild.eq.3) then
                      a_ld=2./Pi
                      b_ld=0.
                      c_ld=-2./Pi
                      d_ld=0.
                      else
                          if (ild.eq.4) then
                            a_ld=2./Pi
                            b_ld=0.
                            c_ld=2./Pi
                            d_ld=0.
                            else
                                a_ld=0.
                                b_ld=0.
                                c_ld=0.
                                d_ld=1.
                          endif
                    endif
              endif
        endif
        return
        end

