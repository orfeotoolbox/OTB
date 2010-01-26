      subroutine equivwl(iinf,isup,step,wlmoy)

      common /sixs_ffu/s(1501),wlinf,wlsup
      real step,wlmoy,s,wlinf,wlsup,seb,wlwave,sbor,wl,swl,coef
      integer iinf,isup,l

      seb=0.
      wlwave=0.

      do 50 l=iinf,isup
        sbor=s(l)
        if(l.eq.iinf.or.l.eq.isup) sbor=sbor*0.5
        wl=.25+(l-1)*step
        call solirr(wl,swl)
        coef=sbor*step*swl
        seb=seb+coef
        wlwave=wlwave+wl*coef
  50  continue
      wlmoy=wlwave/seb
      return
      end
