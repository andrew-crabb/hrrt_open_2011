/*! \file const.h
    \brief This file contains some constants used at different places.
    \author Frank Kehren (frank.kehren@cpspet.com)
    \date 2003/11/17 initial version
 */

#ifndef _CONST_H
#define _CONST_H

/*- constants ---------------------------------------------------------------*/

                                          // don't modify the following 2 lines
                                                         /*! revision number */
const unsigned long int revnum=27501; // 275.01 major.minor

#ifdef M_PI
#undef M_PI
#endif
#ifdef M_PI_2
#undef M_PI_2
#endif

#ifdef XEON_HYPERTHREADING_BUG
                                 /*! offset between stacks of different threads
                                     This is a multiple of CACHE_LINE_SIZE and
                                     larger than the stack used by a thread. */
const unsigned long int STACK_OFFSET=1024;
           /*! size of a cache line on Pentium 4 and Xeon CPUs is 64 Byte; each
               read fetches a cache sector consisting of two adjacent lines */
const unsigned short int CACHE_LINE_SIZE=128;
#endif

const double M_PI=3.1415926535897932384626433832795029;       /*!< \f$\pi\f$ */
                                                     /*! \f$\frac{\pi}{2}\f$ */
const double M_PI_2=1.5707963267948966192313216916397514;

const signed short int CLOCKWISE=1;      /*!< clockwise rotation of detector */
                                   /*! counterclockwise rotation of detector */
const signed short int CTR_CLOCKWISE=-1;
const float EPS=1.0e-6f;                                    /*!< small value */

                                            /*! corrections flag: normalized */
const unsigned long int CORR_Normalized=0x0001;
                       /*! corrections flag: measured attenuation correction */
const unsigned long int CORR_Measured_Attenuation_Correction=0x0002;
                     /*! corrections flag: calculated attenuation correction */
const unsigned long int CORR_Calculated_Attenuation_Correction=0x0004;
                                 /*! corrections flag: 2d scatter correction */
const unsigned long int CORR_2D_scatter_correction=0x0008;
                                 /*! corrections flag: 3d scatter correction */
const unsigned long int CORR_3D_scatter_correction=0x0010;
                                 /*! corrections flag: radial arc correction */
const unsigned long int CORR_Radial_Arc_correction=0x0020;
                                  /*! corrections flag: axial arc correction */
const unsigned long int CORR_Axial_Arc_correction=0x0040;
                                      /*! corrections flag: decay correction */
const unsigned long int CORR_Decay_correction=0x0080;
                               /*! corrections flag: frame-length correction */
const unsigned long int CORR_FrameLen_correction=0x0100;
                                     /*! corrections flag: fourier rebinning */
const unsigned long int CORR_FORE=0x0200;
                                /*! corrections flag: single-slice rebinning */
const unsigned long int CORR_SSRB=0x0400;
const unsigned long int CORR_SEG0=0x0800;   /*!< corrections flag: segment 0 */
                                     /*! corrections flag: randoms smoothing */
const unsigned long int CORR_Randoms_Smoothing=0x1000;
                                   /*! corrections flag: randoms subtraction */
const unsigned long int CORR_Randoms_Subtraction=0x2000;
                                              /*! corrections flag: untilted */
const unsigned long int CORR_untilted=0x4000;
                            /*! corrections flag: smoothing in x/y-direction */
const unsigned long int CORR_XYSmoothing=0x8000;
                              /*! corrections flag: smoothing in z-direction */
const unsigned long int CORR_ZSmoothing=0x10000;

#endif

