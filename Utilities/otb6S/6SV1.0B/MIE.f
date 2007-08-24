      subroutine mie(iaer,wldis,ex,sc,asy,ipol)

c - to vary the number of quadratures
      include "paramdef.inc"
      integer nquad
      common /num_quad/ nquad
      real ph,qh,uh
      common /sixs_aerbas/ ph(20,nqmax_p),qh(20,nqmax_p),uh(20,nqmax_p)
      real cgaus_S(nqmax_p), pdgs_S(nqmax_p)
      integer nbmu, nbmu_2
      real cosang(nqmax_p),weight(nqmax_p)
c - to vary the number of quadratures


      double precision nr,ext(20,4),sca(20,4),np(4),p1(20,4,nqmax_p)  
      double precision q1(20,4,nqmax_p),u1(20,4,nqmax_p)   
      double precision p11(nqmax_p),q11(nqmax_p),u11(nqmax_p)
      double precision pi,r,rmind,rmaxd,r0,alpha,dr,xndpr2,Qext,Qsca
      double precision rlogpas
      real ex(4,20),sc(4,20),asy(4,20),wldis(20)
      real rmax,rmin,rn,ri,x1,x2,x3,rsunph,nrsunph
      real asy_n,asy_d,cij
      integer icp,i,j,l,k,iaer,irsunph,ipol
      double precision arg,ldexp
      
      common /mie_in/ rmax,rmin,icp,rn(20,4),ri(20,4),x1(4),x2(4),
     s x3(4),cij(4),irsunph,rsunph(50),nrsunph(50)

      real sigm, vi(4)


      ldexp=-300.
      pi=4.D+00*datan(1.D+00)
      rlogpas=0.011
      nbmu=nquad 
      
      do i=1,icp
        np(i)=0.D+00
        do l=1,20
          ex(i,l)=0.0
          sc(i,l)=0.0
          asy(i,l)=0.0
          ext(l,i)=0.D+00
          sca(l,i)=0.D+00
          do k=1,nbmu
            p1(l,i,k)=0.D+00                         
            q1(l,i,k)=0.D+00                         
            u1(l,i,k)=0.D+00                         
          enddo
        enddo
      enddo
      rmaxd=dble(rmax)
      rmind=dble(rmin)
      do k=1,nbmu
       q11(k)=0.D+00
       u11(k)=0.D+00
      enddo
      
c - calculation of gauss points
      nbmu=nquad 
      nbmu_2=(nbmu-3)/2
      call gauss(-1.,1.,cosang,weight,nbmu-3)
      cgaus_S(1)=-1.0
      pdgs_S(1)=0.0
      do j=1,nbmu_2
        cgaus_S(j+1)=cosang(j)
        pdgs_S(j+1)=weight(j)
      enddo 
      cgaus_S(nbmu_2+2)=0.
      pdgs_S(nbmu_2+2)=0.
      do j=nbmu_2+1,nbmu-3
        cgaus_S(j+2)=cosang(j)
        pdgs_S(j+2)=weight(j)
      enddo
      cgaus_S(nbmu)=1.0
      pdgs_S(nbmu)=0.
c - calculation of gauss points      

c LOOPS ON THE NUMBER OF PARTICLE TYPE (4 max)
      do 600 i=1,icp
       r=rmind
       dr=r*(10**rlogpas-1.D+00)

 
 123   continue
c LOOPS ON THE RADIUS OF THE PARTICLE     

c call for the size distribution nr. For our computation, we need dn/dr for
c all functions except for sun-photometer inputs, for which we need dV/dlog(r)

       goto(300,301,302,303)iaer-7

c --- Mixture of particles (Log-Normal distribution functions, up to 5)

 300    nr=DLOG10(1.D+00*r/x1(i))*DLOG10(1.D+00*r/x1(i))
        nr=nr/(DLOG10(1.D+00*x2(i))*DLOG10(1.D+00*x2(i)))
        nr=DEXP(-nr/2.D+00)
        nr=nr/dsqrt(2.D+00*pi)/DLOG(10.D+00)/r/DLOG10(1.D+00*x2(i))
        goto 399

c --- Modified Gamma distribution function
 301   r0=1.00D+00   
       arg=-x2(i)*((r/r0)**x3(i))
       if (arg.gt.ldexp) then
          nr=((r/r0)**x1(i))*DEXP(arg)
          else
          nr=0.
          endif
       goto 399

C --- Junge power-law function
 302   r0=0.1000D+00
       nr= r0**(-x1(i))
       IF(r.GT.r0 ) nr= r**(-x1(i))
       goto 399
c
c --- from sun photometer
 303    nr=0.D+00
	do 299 j=2,irsunph
	if ((r-rsunph(j)).lt.0.000001)then
         nr=(r-rsunph(j-1))/(rsunph(j)-rsunph(j-1))
         nr=nrsunph(j-1)+nr*(nrsunph(j)-nrsunph(j-1))
	 goto 398
	endif
 299   continue

c The Mie's calculations have to be called several times (min=2, max=10 for
c each type of particle): at wavelengths bounding the range of the selected
c wavelengths,and at 0.550 microns to normalized the extinction coefficient 
c (if it's not in the selected range of wavelengths).

 398   nr=nr*3./(pi*r*r*r*r*4.)
 399   continue
       xndpr2=nr*dr*pi*(r**2.D+00)
c relatif number of particle for each type of particle (has to be equal to 1)
       np(i)=np(i)+nr*dr
       
        do l=1,20
	 alpha=2.D+00*pi*r/wldis(l)
         call EXSCPHASE(alpha,rn(l,i),ri(l,i),Qext,Qsca,
     &     p11,q11,u11,ipol,cgaus_S,pdgs_S)
         ext(l,i)=ext(l,i)+xndpr2*Qext
         sca(l,i)=sca(l,i)+xndpr2*Qsca

c phase function for each type of particle
         do k=1,nbmu
          p1(l,i,k)=p1(l,i,k)+4.D+00*p11(k)*xndpr2
	  if (ipol.ne.0)then
            q1(l,i,k)=q1(l,i,k)+4.D+00*q11(k)*xndpr2
            u1(l,i,k)=u1(l,i,k)+4.D+00*u11(k)*xndpr2
	  endif
         enddo
       enddo
  599  continue

       vi(i)=vi(i)+r*r*r*nr*dr

       r=r+dr
       dr=r*(10**rlogpas-1.D+00)
       if(r.ge.rmaxd) goto 600
       goto 123
  600 continue

     
c NOW WE MIXT THE DIFFERENT TYPES OF PARTICLE
c Computation of the scattering and extinction coefficients.
c The coefficients will be normalized at 0.550 micron.

       sigm=0.
       do i=1,icp
       vi(i)=4*pi*vi(i)/3
       sigm=sigm+cij(i)/vi(i)
       enddo

c     cij coefficients calculation
         do j=1,icp
         cij(j)=(cij(j)/vi(j)/sigm)
         enddo 

      do l=1,20
       do i=1,icp
         ex(1,l)=ex(1,l)+cij(i)*real(ext(l,i))
         sc(1,l)=sc(1,l)+cij(i)*real(sca(l,i))
       enddo
      enddo

c computation of the phase functions and the asymetry coefficient
c of the mixture of particles
      do l=1,20
        asy_n=0.
        asy_d=0.
        do k=1,nbmu
          ph(l,k)=0.
          do i=1,icp
           ph(l,k)=ph(l,k)+real(cij(i)*p1(l,i,k))
          enddo
          ph(l,k)=ph(l,k)/sc(1,l)
	  if (ipol.ne.0)then
	    qh(l,k)=0.
	    uh(l,k)=0.
	    do i=1,icp
              qh(l,k)=qh(l,k)+real(cij(i)*q1(l,i,k))
              uh(l,k)=uh(l,k)+real(cij(i)*u1(l,i,k))
	    enddo
            qh(l,k)=qh(l,k)/sc(1,l)
            uh(l,k)=uh(l,k)/sc(1,l)
	    xmug=180/pi*acos(cgaus_S(k))
	  endif
	  asy_n=asy_n+cgaus_S(k)*ph(l,k)*pdgs_S(k)
	  asy_d=asy_d+ph(l,k)*pdgs_S(k)
        enddo
	  asy(1,l)=asy_n/asy_d
      enddo

      do i=1,icp
       do l=1,20
        ex(i,l)=ex(i,l)/np(i)
        sc(i,l)=sc(i,l)/np(i) 
       enddo
      enddo

      return
      END                                                                       
C***************************************************************************
C Using the Mie's theory, this subroutine computes the scattering and 
C extinction efficiency factors (usually written Qsca and Qext) and it also 
C computes the scattering intensity efficiency
      subroutine EXSCPHASE(X,nr,ni,Qext,Qsca,p11,q11,u11,ipol,cgaus_S,
     & pdgs_S)
      parameter (nser=10000000)      
      double precision Ren,Imn,X,Y,Up,XnumRDnY,XnumIDnY
      double precision XdenDnY,coxj,Qsca,Qext,xJonH,XdenGNX
      double precision Xnum1An,Xnum2An,XdenAn,Xden1An,Xden2An,RAnb,IAnb
      double precision Xnum1Bn,Xnum2Bn,XdenBn,Xden1Bn,Xden2Bn,RBnb,IBnb
      double precision xmud,xpond,RS1,RS2,IS1,IS2,co_n,test
      double precision xj(0:nser),xy(-1:nser),Rn(0:nser)
      double precision IDnY(0:nser),RDnX(0:nser),RDnY(0:nser)
      double precision IGnX(0:nser),RGnX(0:nser)
      double precision RAn(0:nser),IAn(0:nser),RBn(0:nser),IBn(0:nser)
      double precision TAUn(0:nser),PIn(0:nser)
      real nr,ni
      integer N,Np,mu,mub,mu1,mu2,k,nbmu,j

c - to vary the number of quadratures
      include "paramdef.inc"
      integer nquad
      common /num_quad/ nquad
      double precision p11(nqmax_p),q11(nqmax_p),u11(nqmax_p)
      real cgaus_S(nqmax_p), pdgs_S(nqmax_p)
c - to vary the number of quadratures      

      nbmu=nquad      

      Ren=nr/(nr*nr+ni*ni)
      Imn=ni/(nr*nr+ni*ni)


cjcc
      Y=X*sqrt(nr*nr+ni*ni)
cjcc



c ---Identification of the greater order of computation (=mu)
c    as defined by F.J. Corbato, J. Assoc. Computing Machinery, 1959,
c    6, 366-375
cjcc  N=int(0.5D+00*(-1.D+00+dsqrt(1.D+00+4.D+00*X*X)))+1
      N=int(0.5D+00*(-1.D+00+dsqrt(1.D+00+4.D+00*Y*Y)))+1
C In certain this value of N above leads to a value of Up~1 and big problems
c     N=int(0.5D+00*(-1.D+00+dsqrt(1.D+00+4.D+00*Y*Y)))+3
      
cjcc
      if (N.eq.1)N=2

      mu2=1000000
      Np=N
cjcc
c     Up=2.D+00*X/(2.D+00*Np+1.D+00)
      Up=2.D+00*Y/(2.D+00*Np+1.D+00)
cjcc
      mu1=int(Np+30.*(0.10+0.35*Up*(2-Up*Up)/2./(1-Up)))
cjcc
c     Np=int(X-0.5D+00+dsqrt(30.*0.35*X))
      Np=int(Y-0.5D+00+dsqrt(30.*0.35*Y))
cjcc
      if (Np.gt.N)then
cjcc
c      Up=2.D+00*X/(2.D+00*Np+1.D+00)
       Up=2.D+00*Y/(2.D+00*Np+1.D+00)
cjcc
       mu2=int(Np+30.*(0.10+0.35*Up*(2-Up*Up)/2./(1-Up)))
      endif
      mu=min0(mu1,mu2)

      if (mu.ge.nser) then
         write(6,*) " Error, nser is too small, mu is equal to : ",mu
         Stop
         endif


      if (mu.le.0) then
         write(6,*) " Error, mu is too small, mu is equal to : ",mu
         Stop
         endif

c --- Identification of the transition line. Below this line the Bessel 
c     function j behaves as oscillating functions. Above the behavior 
c     becomes monotonic. We start at a order greater than this transition 
c     line (order max=mu) because a downward recursion is called for.
      Rn(mu)=0.D+00
      k=mu+1
 149  continue
      k=k-1
      xj(k)=0.D+00
      Rn(k-1)=X/(2.D+00*k+1.D+00-X*Rn(k))
      if (k.eq.2)then
	  mub=mu
	  xj(mub+1)=0.D+00
	  xj(mub)=1.D+00
	  goto 150
      endif
      if (Rn(k-1).gt.1.D+00)then
	  mub=k-1
	  xj(mub+1)=Rn(mub)
	  xj(mub)=1.D+00
	  goto 150
      endif
      goto 149
 150  continue

      do k=mub,1,-1
	xj(k-1)=(2.D+00*k+1.D+00)*xj(k)/X-xj(k+1)
      enddo
      coxj=(xj(0)-X*xj(1))*dcos(X)+X*xj(0)*sin(X)

c --- Computation Dn(alpha) and Dn(alpha*m) (cf MIE's theory) 
c     downward recursion    - real and imaginary parts
      RDnY(mu)=0.D+00
      IDnY(mu)=0.D+00
      RDnX(mu)=0.D+00
      do k=mu,1,-1
	 RDnX(k-1)=k/X-1.D+00/(RDnX(k)+k/X)
	 XnumRDnY=RDnY(k)+Ren*k/X
	 XnumIDnY=IDnY(k)+Imn*k/X
	 XdenDnY=XnumRDnY*XnumRDnY+XnumIDnY*XnumIDnY
	 RDnY(k-1)=k*Ren/X-XnumRDnY/XdenDnY
	 IDnY(k-1)=k*Imn/X+XnumIDnY/XdenDnY

      enddo 

c --- Initialization of the upward recursions
      xy(-1)=dsin(x)/x
      xy(0)=-dcos(x)/x
      RGnX(0)=0.D+00
      IGnX(0)=-1.D+00
      Qsca=0.D+00
      Qext=0.D+00
      do k=1,mu
	 if (k.le.mub)then
	   xj(k)=xj(k)/coxj
	 else
	   xj(k)=Rn(k-1)*xj(k-1)
	 endif

c --- Computation of bessel's function y(alpha)
	 xy(k)=(2.D+00*k-1.D+00)*xy(k-1)/X-xy(k-2)
	 xJonH=xj(k)/(xj(k)*xj(k)+xy(k)*xy(k))

c --- Computation of Gn(alpha), Real and Imaginary part
         XdenGNX=(RGnX(k-1)-k/X)**2.D+00+IGnX(k-1)*IGnX(k-1)
	 RGnX(k)=(k/X-RGnX(k-1))/XdenGNX-k/X
	 IGnX(k)=IGnX(k-1)/XdenGNX

c --- Computation of An(alpha) and Bn(alpha), Real and Imaginary part
	 Xnum1An=RDnY(k)-nr*RDnX(k)
	 Xnum2An=IDnY(k)+ni*RDnX(k)
	 Xden1An=RDnY(k)-nr*RGnX(k)-ni*IGnX(k)
	 Xden2An=IDnY(k)+ni*RGnX(k)-nr*IGnX(k)
	 XdenAn=Xden1An*Xden1An+Xden2An*Xden2An
	 RAnb=(Xnum1An*Xden1An+Xnum2An*Xden2An)/XdenAn
	 IAnb=(-Xnum1An*Xden2An+Xnum2An*Xden1An)/XdenAn
	 RAn(k)=xJonH*(xj(k)*RAnb-xy(k)*IAnb)
	 IAn(k)=xJonH*(xy(k)*RAnb+xj(k)*IAnb)

	 Xnum1Bn=nr*RDnY(k)+ni*IDnY(k)-RDnX(k)
	 Xnum2Bn=nr*IDnY(k)-ni*RDnY(k)
	 Xden1Bn=nr*RDnY(k)+ni*IDnY(k)-RGnX(k)
	 Xden2Bn=nr*IDnY(k)-ni*RDnY(k)-IGnX(k)
	 XdenBn=Xden1Bn*Xden1Bn+Xden2Bn*Xden2Bn
	 RBnb=(Xnum1Bn*Xden1Bn+Xnum2Bn*Xden2Bn)/XdenBn
	 IBnb=(-Xnum1Bn*Xden2Bn+Xnum2Bn*Xden1Bn)/XdenBn
	 RBn(k)=xJonH*(xj(k)*RBnb-xy(k)*IBnb)
	 IBn(k)=xJonH*(xy(k)*RBnb+xj(k)*IBnb)

c ---Criterion on the recursion formulas as defined by D. Deirmendjian 
c    et al., J. Opt. Soc. Am., 1961, 51, 6, 620-633
 	 test=(RAn(k)**2.+IAn(k)**2.+RBn(k)**2.+IBn(k)**2.)/k
 	 if (test.lt.1.0D-14)then
           mu=k
           if (mu.le.0) then
            write(6,*) "attention mu <0 ",mu
            endif
           goto 400
         endif
c --- Computation of the scattering and extinction efficiency factor
         xpond=2.D+00/X/X*(2.D+00*k+1)
         Qsca=Qsca+xpond*(RAn(k)**2.+IAn(k)**2.+RBn(k)**2.+IBn(k)**2.)
         Qext=Qext+xpond*(RAn(k)+RBn(k))

      enddo
 400  continue

c --- Computation of the amplitude functions S1 and S2 (cf MIE's theory)
c     defined by PIn, TAUn, An and Bn with PIn and TAUn related to the 
c     Legendre polynomials.
      do j=1,nbmu
	 xmud=cgaus_S(j)
	 RS1=0.D+00
	 RS2=0.D+00
	 IS1=0.D+00
	 IS2=0.D+00
	 PIn(0)=0.D+00
	 PIn(1)=1.D+00
	 TAUn(1)=xmud
	 do k=1,mu
          co_n=(2.D+00*k+1.D+00)/k/(k+1.D+00)
	  RS1=RS1+co_n*(RAn(k)*PIn(k)+RBn(k)*TAUn(k))
	  RS2=RS2+co_n*(RAn(k)*TAUn(k)+RBn(k)*PIn(k))
	  IS1=IS1+co_n*(IAn(k)*PIn(k)+IBn(k)*TAUn(k))
	  IS2=IS2+co_n*(IAn(k)*TAUn(k)+IBn(k)*PIn(k))
          PIn(k+1)=((2.D+00*k+1)*xmud*PIn(k)-(k+1.D+00)*PIn(k-1))/k
          TAUn(k+1)=(k+1.D+00)*xmud*PIn(k+1)-(k+2.D+00)*PIn(k)
         enddo
C --- Computation of the scattering intensity efficiency
         p11(j)=(RS1*RS1+IS1*IS1+RS2*RS2+IS2*IS2)/X/X/2.D+00
	 if (ipol.ne.0)then
           q11(j)=(RS2*RS2+IS2*IS2-RS1*RS1-IS1*IS1)/X/X/2.D+00
           u11(j)=(2.D+00*RS2*RS1+2.D+00*IS2*IS1)/X/X/2.D+00
	 endif
      enddo
               
      return
      end
