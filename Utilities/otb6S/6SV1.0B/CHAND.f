	subroutine chand (xphi,xmuv,xmus,xtau
     s			,xrray)
c input parameters: xphi,xmus,xmuv,xtau
c xphi: azimuthal difference between sun and observation (xphi=0,
c in backscattering) and expressed in degree (0.:360.)
c xmus: cosine of the sun zenith angle
c xmuv: cosine of the observation zenith angle
c xtau: molecular optical depth
c output parameter: xrray : molecular reflectance (0.:1.)
c constant : xdep: depolarization factor (0.0279)
	real xdep,pl(10)
	real fs0,fs1,fs2
	real as0(10),as1(2),as2(2)
        real xphi,xmus,fac,xmuv,xtau,xrray,pi,phios,xcosf1,xcosf2
        real xcosf3,xbeta2,xfd,xph1,xph2,xph3,xitm, xp1, xp2, xp3
        real cfonc1,cfonc2,cfonc3,xlntau,xitot1,xitot2,xitot3
        integer i
	data (as0(i),i=1,10) /.33243832,-6.777104e-02,.16285370
     s	,1.577425e-03,-.30924818,-1.240906e-02,-.10324388
     s	,3.241678e-02,.11493334,-3.503695e-02/
	data (as1(i),i=1,2) /.19666292, -5.439061e-02/
	data (as2(i),i=1,2) /.14545937,-2.910845e-02/
	pi=3.1415927
	fac=pi/180.
	phios=180.-xphi
	xcosf1=1.
	xcosf2=cos(phios*fac)
	xcosf3=cos(2*phios*fac)
	xbeta2=0.5
	xdep=0.0279
	xfd=xdep/(2-xdep)
	xfd=(1-xfd)/(1+2*xfd)
	xph1=1+(3*xmus*xmus-1)*(3*xmuv*xmuv-1)*xfd/8.
	xph2=-xmus*xmuv*sqrt(1-xmus*xmus)*sqrt(1-xmuv*xmuv)
	xph2=xph2*xfd*xbeta2*1.5
	xph3=(1-xmus*xmus)*(1-xmuv*xmuv)
	xph3=xph3*xfd*xbeta2*0.375
	xitm=(1-exp(-xtau*(1/xmus+1/xmuv)))*xmus/(4*(xmus+xmuv))
	xp1=xph1*xitm
	xp2=xph2*xitm
	xp3=xph3*xitm
	xitm=(1-exp(-xtau/xmus))*(1-exp(-xtau/xmuv))
	cfonc1=xph1*xitm
	cfonc2=xph2*xitm
	cfonc3=xph3*xitm
	xlntau=log(xtau)
	pl(1)=1.
	pl(2)=xlntau
	pl(3)=xmus+xmuv
	pl(4)=xlntau*pl(3)
	pl(5)=xmus*xmuv
	pl(6)=xlntau*pl(5)
	pl(7)=xmus*xmus+xmuv*xmuv
	pl(8)=xlntau*pl(7)
	pl(9)=xmus*xmus*xmuv*xmuv
	pl(10)=xlntau*pl(9)
	fs0=0.
	do i=1,10
	fs0=fs0+pl(i)*as0(i)
	enddo
	fs1=pl(1)*as1(1)+pl(2)*as1(2)
	fs2=pl(1)*as2(1)+pl(2)*as2(2)
	xitot1=xp1+cfonc1*fs0*xmus
	xitot2=xp2+cfonc2*fs1*xmus
	xitot3=xp3+cfonc3*fs2*xmus
	xrray=xitot1*xcosf1
	xrray=xrray+xitot2*xcosf2*2
	xrray=xrray+xitot3*xcosf3*2
	xrray=xrray/xmus
	return
	end
