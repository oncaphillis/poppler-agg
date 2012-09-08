//----------------------------------------------------------------------------
// Anti-Grain Geometry (AGG) - Version 2.5
// A high quality rendering engine for C++
// Copyright (C) 2002-2006 Maxim Shemanarev
// Contact: mcseem@antigrain.com
//          mcseemagg@yahoo.com
//          http://antigrain.com
// 
// AGG is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
// 
// AGG is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with AGG; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, 
// MA 02110-1301, USA.
//----------------------------------------------------------------------------
//
// Adaptation for high precision colors has been sponsored by 
// Liberty Technology Systems, Inc., visit http://lib-sys.com
//
// Liberty Technology Systems, Inc. is the provider of
// PostScript and PDF technology for software developers.
// 
//----------------------------------------------------------------------------
// Contact: mcseem@antigrain.com
//          mcseemagg@yahoo.com
//          http://www.antigrain.com
//----------------------------------------------------------------------------

#ifndef AGG_COLOR_CMYK_INCLUDED
#define AGG_COLOR_CMYK_INCLUDED

#include <math.h>
#include "agg_color_rgba.h"

namespace agg
{
    // Supported byte orders for RGB and RGBA pixel formats
    //=======================================================================
    
    struct order_cmyk  { enum cmyk_e  { C=0, M=1, Y=2, K=3, cmyk_tag }; };       //----order_rgb
    
    //====================================================================rgba
    struct cmyk
    {
        typedef double value_type;

        double c;
        double m;
        double y;
        double k;
        double a;
        
        //--------------------------------------------------------------------
        cmyk() 
        {
        }

        //--------------------------------------------------------------------
        cmyk(double c_, double m_, double y_, double k_,double a_=1.0) :           
            c(c_), m(m_), y(y_), k(k_),a(a_) 
        {
        }
        
        //--------------------------------------------------------------------
        cmyk(const cmyk & c, double a_) : 
            c(c.c), m(c.m), y(c.y), k(c.k), a(a_) 
        {
        }

        //--------------------------------------------------------------------
        void clear()
        {
            c = m = y = k = a = 0;
        }

        //--------------------------------------------------------------------
        const cmyk & transparent()
        {
            a = 0.0;
            return *this;
        }

        //--------------------------------------------------------------------
        const cmyk & opacity(double a_)
        {
            if(a_ < 0.0) 
                a_ = 0.0;
            if(a_ > 1.0) 
                a_ = 1.0;
            a = a_;
            return *this;
        }

        //--------------------------------------------------------------------
        double opacity() const
        {
            return a;
        }

        //--------------------------------------------------------------------
        const cmyk & premultiply()
        {
            c *= a;
            m *= a;
            y *= a;
            k *= a;
            return *this;
        }

        //--------------------------------------------------------------------
        const cmyk &  premultiply(double a_)
        {
            if(a <= 0.0 || a_ <= 0.0)
            {
                c = m = y = k = a = 0.0;
                return *this;
            }
            a_ /= a;
            c *= a_;
            m *= a_;
            y *= a_;
            k *= a_;

            a  = a_;
            return *this;
        }

        //--------------------------------------------------------------------
        const cmyk & demultiply()
        {
            if(a == 0)
            {
                c = m = y = k = 0;
                return *this;
            }
            double a_ = 1.0 / a;
            c *= a_;
            m *= a_;
            y *= a_;
            k *= a_;
            return *this;
        }
    

        //--------------------------------------------------------------------
        cmyk gradient(cmyk cl, double l) const
        {
            cmyk ret;
            ret.c = c + (cl.c - c) * l;
            ret.m = m + (cl.m - m) * l;
            ret.y = y + (cl.y - y) * l;
            ret.k = k + (cl.k - k) * l;
            ret.a = a + (cl.a - a) * l;
            return ret;
        }
        
        cmyk to_cmy() const
        {
            cmyk cl;

            cl.c = ( c * ( 1.0 - k ) + k );
            cl.m = ( m * ( 1.0 - k ) + k );
            cl.y = ( y * ( 1.0 - k ) + k );
            cl.k = 0.0;
            cl.a = a;

            return cl;
        }

        rgba to_rgb() const
        {
            rgba r;
            cmyk cl = to_cmy();
            r.r = ( 1.0 - cl.c );
            r.g = ( 1.0 - cl.m );
            r.b = ( 1.0 - cl.y );
            r.a = a;

            return r;
        }

        //--------------------------------------------------------------------
        static cmyk no_color() { return cmyk(0,0,0,0,0); }
        
        //--------------------------------------------------------------------
        static cmyk from_wavelength(double wl, double gamma = 1.0);
        
        //--------------------------------------------------------------------
        explicit cmyk(double wavelen, double gamma=1.0)
        {
            *this = from_wavelength(wavelen, gamma);
        }

        static const cmyk black;
        static const cmyk white;
        static const cmyk cyan;
        static const cmyk yellow;
        static const cmyk magenta;
    };

    //----------------------------------------------------------------rgba_pre
    inline cmyk cmyk_pre(double c, double m, double y, double k,double a=1.0)
    {
        return cmyk(c, m, y, k, a).premultiply();
    }
    inline cmyk cmyk_pre(const cmyk & c)
    {
        return cmyk(c).premultiply();
    }
    inline cmyk cmyk_pre(const cmyk & c, double a)
    {
        return cmyk(c, a).premultiply();
    }

#if 0
    //------------------------------------------------------------------------
    inline cmyk cmyk::from_wavelength(double wl, double gamma)
    {
        cmyk t(0.0, 0.0, 0.0, 0.0);

        if(wl >= 380.0 && wl <= 440.0)
        {
            t.r = -1.0 * (wl - 440.0) / (440.0 - 380.0);
            t.b = 1.0;
        }
        else 
        if(wl >= 440.0 && wl <= 490.0)
        {
            t.g = (wl - 440.0) / (490.0 - 440.0);
            t.b = 1.0;
        }
        else
        if(wl >= 490.0 && wl <= 510.0)
        {
            t.g = 1.0;
            t.b = -1.0 * (wl - 510.0) / (510.0 - 490.0);
        }
        else
        if(wl >= 510.0 && wl <= 580.0)
        {
            t.r = (wl - 510.0) / (580.0 - 510.0);
            t.g = 1.0;
        }
        else
        if(wl >= 580.0 && wl <= 645.0)
        {
            t.r = 1.0;
            t.g = -1.0 * (wl - 645.0) / (645.0 - 580.0);
        }
        else
        if(wl >= 645.0 && wl <= 780.0)
        {
            t.r = 1.0;
        }

        double s = 1.0;
        if(wl > 700.0)       s = 0.3 + 0.7 * (780.0 - wl) / (780.0 - 700.0);
        else if(wl <  420.0) s = 0.3 + 0.7 * (wl - 380.0) / (420.0 - 380.0);

        t.r = pow(t.r * s, gamma);
        t.g = pow(t.g * s, gamma);
        t.b = pow(t.b * s, gamma);
        return t;
    }
#endif


    
    //===================================================================cmyka8

    struct cmyka8
    {
        typedef int8u  value_type;
        typedef int32u calc_type;
        typedef int32  long_type;
        enum base_scale_e
        {
            base_shift = 8,
            base_scale = 1 << base_shift,
            base_mask  = base_scale - 1
        };
        typedef cmyka8 self_type;


        value_type c;
        value_type m;
        value_type y;
        value_type k;
        value_type a;

        //--------------------------------------------------------------------
        cmyka8() 
        {
        }

        //--------------------------------------------------------------------
        cmyka8(unsigned c_, unsigned m_, unsigned y_, unsigned k_,unsigned a_=base_mask) :
            c(value_type(c_)), 
            m(value_type(m_)), 
            y(value_type(y_)), 
            k(value_type(k_)), 
            a(value_type(a_)) {}

        //--------------------------------------------------------------------

        cmyka8(const cmyk & cl, double a_) :
            c((value_type)uround(cl.c * double(base_mask))), 
            m((value_type)uround(cl.m * double(base_mask))), 
            y((value_type)uround(cl.y * double(base_mask))), 
            k((value_type)uround(cl.k * double(base_mask))), 
            a((value_type)uround(a_   * double(base_mask))) 
        {
        }

        //--------------------------------------------------------------------
        cmyka8(const self_type& cl, unsigned a_) :
            c(cl.c), 
            m(cl.m), 
            y(cl.y), 
            k(cl.k),
            a(value_type(a_)) 
        {
        }

        //--------------------------------------------------------------------
        cmyka8(const cmyk & cl) :
            c((value_type)uround(cl.c * double(base_mask))), 
            m((value_type)uround(cl.m * double(base_mask))), 
            y((value_type)uround(cl.y * double(base_mask))), 
            k((value_type)uround(cl.k * double(base_mask))), 
            a((value_type)uround(cl.a * double(base_mask))) {}

        //--------------------------------------------------------------------
        void clear()
        {
            c = m = y = k = a = 0;
        }
        
        //--------------------------------------------------------------------
        const self_type& transparent()
        {
            a = 0;
            return *this;
        }

        //--------------------------------------------------------------------
        const self_type& opacity(double a_)
        {
            if(a_ < 0.0) a_ = 0.0;
            if(a_ > 1.0) a_ = 1.0;
            a = (value_type)uround(a_ * double(base_mask));
            return *this;
        }

        //--------------------------------------------------------------------
        double opacity() const
        {
            return double(a) / double(base_mask);
        }

        //--------------------------------------------------------------------
        AGG_INLINE const self_type& premultiply()
        {
            if(a == base_mask) 
            {
                return *this;
            }
            if(a == 0)
            {
                c = m = y = k = 0;
                return *this;
            }

            c = value_type((calc_type(c) * a) >> base_shift);
            m = value_type((calc_type(m) * a) >> base_shift);
            y = value_type((calc_type(y) * a) >> base_shift);
            k = value_type((calc_type(k) * a) >> base_shift);

            return *this;
        }

        //--------------------------------------------------------------------
        AGG_INLINE const self_type& premultiply(unsigned a_)
        {
            if(a == base_mask && a_ >= base_mask) 
                return *this;
            if(a == 0 || a_ == 0)
            {
                c = m = y = k = a = 0;
                return *this;
            }

            calc_type c_ = (calc_type(c) * a_) / a;
            calc_type m_ = (calc_type(m) * a_) / a;
            calc_type y_ = (calc_type(y) * a_) / a;
            calc_type k_ = (calc_type(k) * a_) / a;

            c = value_type((c_ > a_) ? a_ : c_);
            m = value_type((m_ > a_) ? a_ : m_);
            y = value_type((y_ > a_) ? a_ : y_);
            k = value_type((k_ > a_) ? a_ : k_);
            a = value_type(a_);

            return *this;
        }

        //--------------------------------------------------------------------
        AGG_INLINE const self_type& demultiply()
        {
            if(a == base_mask) return *this;
            if(a == 0)
            {
                c = m = y = k = 0;
                return *this;
            }
            calc_type c_ = (calc_type(c) * base_mask) / a;
            calc_type m_ = (calc_type(m) * base_mask) / a;
            calc_type y_ = (calc_type(y) * base_mask) / a;
            calc_type k_ = (calc_type(k) * base_mask) / a;
            c = value_type((c_ > calc_type(base_mask)) ? calc_type(base_mask) : c_);
            m = value_type((m_ > calc_type(base_mask)) ? calc_type(base_mask) : m_);
            y = value_type((y_ > calc_type(base_mask)) ? calc_type(base_mask) : y_);
            k = value_type((k_ > calc_type(base_mask)) ? calc_type(base_mask) : k_);
            return *this;
        }

        //--------------------------------------------------------------------
        AGG_INLINE self_type gradient(const self_type& cl, double l) const
        {
            self_type ret;
            calc_type ik = uround(l * base_scale);
            ret.c = value_type(calc_type(c) + (((calc_type(cl.c) - c) * ik) >> base_shift));
            ret.m = value_type(calc_type(m) + (((calc_type(cl.m) - m) * ik) >> base_shift));
            ret.y = value_type(calc_type(y) + (((calc_type(cl.y) - y) * ik) >> base_shift));
            ret.k = value_type(calc_type(k) + (((calc_type(cl.k) - k) * ik) >> base_shift));
            ret.a = value_type(calc_type(a) + (((calc_type(cl.a) - a) * ik) >> base_shift));
            return ret;
        }

        //--------------------------------------------------------------------
        AGG_INLINE void add(const self_type& cl, unsigned cover)
        {
            calc_type cc, cm, cy, ck, ca;
            if(cover == cover_mask)
            {
                if(cl.a == base_mask) 
                {
                    *this = cl;
                }
                else
                {
                    cc = c + cl.c; c = (cc > calc_type(base_mask)) ? calc_type(base_mask) : cc;
                    cm = m + cl.m; m = (cm > calc_type(base_mask)) ? calc_type(base_mask) : cm;
                    cy = y + cl.y; y = (cy > calc_type(base_mask)) ? calc_type(base_mask) : cy;
                    ck = k + cl.k; k = (ck > calc_type(base_mask)) ? calc_type(base_mask) : ck;
                    ca = a + cl.a; a = (ca > calc_type(base_mask)) ? calc_type(base_mask) : ca;
                }
            }
            else
            {
                cc = c + ((cl.c * cover + cover_mask / 2) >> cover_shift);
                cm = m + ((cl.m * cover + cover_mask / 2) >> cover_shift);
                cy = y + ((cl.y * cover + cover_mask / 2) >> cover_shift);
                ck = k + ((cl.k * cover + cover_mask / 2) >> cover_shift);
                ca = a + ((cl.a * cover + cover_mask / 2) >> cover_shift);

                c = (cc > calc_type(base_mask)) ? calc_type(base_mask) : cc;
                m = (cm > calc_type(base_mask)) ? calc_type(base_mask) : cm;
                y = (cy > calc_type(base_mask)) ? calc_type(base_mask) : cy;
                k = (ck > calc_type(base_mask)) ? calc_type(base_mask) : ck;
                a = (ca > calc_type(base_mask)) ? calc_type(base_mask) : ca;
            }
        }

        //--------------------------------------------------------------------
        template<class GammaLUT>
        AGG_INLINE void apply_gamma_dir(const GammaLUT& gamma)
        {
            c = gamma.dir(c);
            m = gamma.dir(m);
            y = gamma.dir(y);
            k = gamma.dir(k);
        }

        //--------------------------------------------------------------------
        template<class GammaLUT>
        AGG_INLINE void apply_gamma_inv(const GammaLUT& gamma)
        {
            c = gamma.inv(c);
            m = gamma.inv(m);
            y = gamma.inv(y);
            k = gamma.inv(k);
        }

        //--------------------------------------------------------------------
        static self_type no_color() { return self_type(0,0,0,0); }

        //--------------------------------------------------------------------
        static self_type from_wavelength(double wl, double gamma = 1.0)
        {
            return self_type(cmyk::from_wavelength(wl, gamma));
        }

        static const self_type black;
        static const self_type white;
        static const self_type cyan;
        static const self_type magenta;
        static const self_type yellow;
    };
    
    //-------------------------------------------------------------cmyka8_pre
    inline cmyka8 cmyka8_pre(unsigned c, unsigned m, unsigned y, unsigned k, 
                             unsigned a = cmyka8::base_mask)
    {
        return cmyka8(c,m,y,k,a).premultiply();
    }
    inline cmyka8 cmyka8_pre(const cmyka8 & cl)
    {
        return cmyka8(cl).premultiply();
    }
    inline cmyka8 cmyka8_pre(const cmyka8 & cl, unsigned a)
    {
        return cmyka8(cl,a).premultiply();
    }
    inline cmyka8 cmaka8_pre(const cmyk & cl)
    {
        return  cmyka8(cl).premultiply();
    }
    inline cmyka8 cmyka8_pre(const cmyk & cl, double a)
    {
        return cmyka8(cl,a).premultiply();
    }

    inline cmyk to_cmyk(const cmyka8 & cl)
    {
        return cmyk( (double) cl.c/255.0,
                     (double) cl.m/255.0,
                     (double) cl.y/255.0,
                     (double) cl.k/255.0,
                     (double) cl.a/255.0);
    }
#if 0
    //-------------------------------------------------------------rgb8_packed
    inline cmyka8 cmyka8_packed(unsigned v)
    {
        return rgba8((v >> 16) & 0xFF, (v >> 8) & 0xFF, v & 0xFF);
    }

    //-------------------------------------------------------------bgr8_packed
    inline rgba8 bgr8_packed(unsigned v)
    {
        return rgba8(v & 0xFF, (v >> 8) & 0xFF, (v >> 16) & 0xFF);
    }

    //------------------------------------------------------------argb8_packed
    inline rgba8 argb8_packed(unsigned v)
    {
        return rgba8((v >> 16) & 0xFF, (v >> 8) & 0xFF, v & 0xFF, v >> 24);
    }

    //---------------------------------------------------------rgba8_gamma_dir
    template<class GammaLUT>
    rgba8 rgba8_gamma_dir(rgba8 c, const GammaLUT& gamma)
    {
        return rgba8(gamma.dir(c.r), gamma.dir(c.g), gamma.dir(c.b), c.a);
    }

    //---------------------------------------------------------rgba8_gamma_inv
    template<class GammaLUT>
    rgba8 rgba8_gamma_inv(rgba8 c, const GammaLUT& gamma)
    {
        return rgba8(gamma.inv(c.r), gamma.inv(c.g), gamma.inv(c.b), c.a);
    }



    //==================================================================rgba16
    struct rgba16
    {
        typedef int16u value_type;
        typedef int32u calc_type;
        typedef int64  long_type;
        enum base_scale_e
        {
            base_shift = 16,
            base_scale = 1 << base_shift,
            base_mask  = base_scale - 1
        };
        typedef rgba16 self_type;

        value_type r;
        value_type g;
        value_type b;
        value_type a;

        //--------------------------------------------------------------------
        rgba16() {}

        //--------------------------------------------------------------------
        rgba16(unsigned r_, unsigned g_, unsigned b_, unsigned a_=base_mask) :
            r(value_type(r_)), 
            g(value_type(g_)), 
            b(value_type(b_)), 
            a(value_type(a_)) {}

        //--------------------------------------------------------------------
        rgba16(const self_type& c, unsigned a_) :
            r(c.r), g(c.g), b(c.b), a(value_type(a_)) {}

        //--------------------------------------------------------------------
        rgba16(const rgba& c) :
            r((value_type)uround(c.r * double(base_mask))), 
            g((value_type)uround(c.g * double(base_mask))), 
            b((value_type)uround(c.b * double(base_mask))), 
            a((value_type)uround(c.a * double(base_mask))) {}

        //--------------------------------------------------------------------
        rgba16(const rgba& c, double a_) :
            r((value_type)uround(c.r * double(base_mask))), 
            g((value_type)uround(c.g * double(base_mask))), 
            b((value_type)uround(c.b * double(base_mask))), 
            a((value_type)uround(a_  * double(base_mask))) {}

        //--------------------------------------------------------------------
        rgba16(const rgba8& c) :
            r(value_type((value_type(c.r) << 8) | c.r)), 
            g(value_type((value_type(c.g) << 8) | c.g)), 
            b(value_type((value_type(c.b) << 8) | c.b)), 
            a(value_type((value_type(c.a) << 8) | c.a)) {}

        //--------------------------------------------------------------------
        rgba16(const rgba8& c, unsigned a_) :
            r(value_type((value_type(c.r) << 8) | c.r)), 
            g(value_type((value_type(c.g) << 8) | c.g)), 
            b(value_type((value_type(c.b) << 8) | c.b)), 
            a(value_type((             a_ << 8) | c.a)) {}

        //--------------------------------------------------------------------
        void clear()
        {
            r = g = b = a = 0;
        }
        
        //--------------------------------------------------------------------
        const self_type& transparent()
        {
            a = 0;
            return *this;
        }

        //--------------------------------------------------------------------
        AGG_INLINE const self_type& opacity(double a_)
        {
            if(a_ < 0.0) a_ = 0.0;
            if(a_ > 1.0) a_ = 1.0;
            a = (value_type)uround(a_ * double(base_mask));
            return *this;
        }

        //--------------------------------------------------------------------
        double opacity() const
        {
            return double(a) / double(base_mask);
        }

        //--------------------------------------------------------------------
        AGG_INLINE const self_type& premultiply()
        {
            if(a == base_mask) return *this;
            if(a == 0)
            {
                r = g = b = 0;
                return *this;
            }
            r = value_type((calc_type(r) * a) >> base_shift);
            g = value_type((calc_type(g) * a) >> base_shift);
            b = value_type((calc_type(b) * a) >> base_shift);
            return *this;
        }

        //--------------------------------------------------------------------
        AGG_INLINE const self_type& premultiply(unsigned a_)
        {
            if(a == base_mask && a_ >= base_mask) return *this;
            if(a == 0 || a_ == 0)
            {
                r = g = b = a = 0;
                return *this;
            }
            calc_type r_ = (calc_type(r) * a_) / a;
            calc_type g_ = (calc_type(g) * a_) / a;
            calc_type b_ = (calc_type(b) * a_) / a;
            r = value_type((r_ > a_) ? a_ : r_);
            g = value_type((g_ > a_) ? a_ : g_);
            b = value_type((b_ > a_) ? a_ : b_);
            a = value_type(a_);
            return *this;
        }

        //--------------------------------------------------------------------
        AGG_INLINE const self_type& demultiply()
        {
            if(a == base_mask) return *this;
            if(a == 0)
            {
                r = g = b = 0;
                return *this;
            }
            calc_type r_ = (calc_type(r) * base_mask) / a;
            calc_type g_ = (calc_type(g) * base_mask) / a;
            calc_type b_ = (calc_type(b) * base_mask) / a;
            r = value_type((r_ > calc_type(base_mask)) ? calc_type(base_mask) : r_);
            g = value_type((g_ > calc_type(base_mask)) ? calc_type(base_mask) : g_);
            b = value_type((b_ > calc_type(base_mask)) ? calc_type(base_mask) : b_);
            return *this;
        }

        //--------------------------------------------------------------------
        AGG_INLINE self_type gradient(const self_type& c, double k) const
        {
            self_type ret;
            calc_type ik = uround(k * base_scale);
            ret.r = value_type(calc_type(r) + (((calc_type(c.r) - r) * ik) >> base_shift));
            ret.g = value_type(calc_type(g) + (((calc_type(c.g) - g) * ik) >> base_shift));
            ret.b = value_type(calc_type(b) + (((calc_type(c.b) - b) * ik) >> base_shift));
            ret.a = value_type(calc_type(a) + (((calc_type(c.a) - a) * ik) >> base_shift));
            return ret;
        }

        //--------------------------------------------------------------------
        AGG_INLINE void add(const self_type& c, unsigned cover)
        {
            calc_type cr, cg, cb, ca;
            if(cover == cover_mask)
            {
                if(c.a == base_mask) 
                {
                    *this = c;
                }
                else
                {
                    cr = r + c.r; r = (cr > calc_type(base_mask)) ? calc_type(base_mask) : cr;
                    cg = g + c.g; g = (cg > calc_type(base_mask)) ? calc_type(base_mask) : cg;
                    cb = b + c.b; b = (cb > calc_type(base_mask)) ? calc_type(base_mask) : cb;
                    ca = a + c.a; a = (ca > calc_type(base_mask)) ? calc_type(base_mask) : ca;
                }
            }
            else
            {
                cr = r + ((c.r * cover + cover_mask) >> cover_shift);
                cg = g + ((c.g * cover + cover_mask) >> cover_shift);
                cb = b + ((c.b * cover + cover_mask) >> cover_shift);
                ca = a + ((c.a * cover + cover_mask) >> cover_shift);
                r = (cr > calc_type(base_mask)) ? calc_type(base_mask) : cr;
                g = (cg > calc_type(base_mask)) ? calc_type(base_mask) : cg;
                b = (cb > calc_type(base_mask)) ? calc_type(base_mask) : cb;
                a = (ca > calc_type(base_mask)) ? calc_type(base_mask) : ca;
            }
        }

        //--------------------------------------------------------------------
        template<class GammaLUT>
        AGG_INLINE void apply_gamma_dir(const GammaLUT& gamma)
        {
            r = gamma.dir(r);
            g = gamma.dir(g);
            b = gamma.dir(b);
        }

        //--------------------------------------------------------------------
        template<class GammaLUT>
        AGG_INLINE void apply_gamma_inv(const GammaLUT& gamma)
        {
            r = gamma.inv(r);
            g = gamma.inv(g);
            b = gamma.inv(b);
        }

        //--------------------------------------------------------------------
        static self_type no_color() { return self_type(0,0,0,0); }

        //--------------------------------------------------------------------
        static self_type from_wavelength(double wl, double gamma = 1.0)
        {
            return self_type(rgba::from_wavelength(wl, gamma));
        }
    };



    //--------------------------------------------------------------rgba16_pre
    inline rgba16 rgba16_pre(unsigned r, unsigned g, unsigned b, 
                             unsigned a = rgba16::base_mask)
    {
        return rgba16(r,g,b,a).premultiply();
    }
    inline rgba16 rgba16_pre(const rgba16& c, unsigned a)
    {
        return rgba16(c,a).premultiply();
    }
    inline rgba16 rgba16_pre(const rgba& c)
    {
        return rgba16(c).premultiply();
    }
    inline rgba16 rgba16_pre(const rgba& c, double a)
    {
        return rgba16(c,a).premultiply();
    }
    inline rgba16 rgba16_pre(const rgba8& c)
    {
        return rgba16(c).premultiply();
    }
    inline rgba16 rgba16_pre(const rgba8& c, unsigned a)
    {
        return rgba16(c,a).premultiply();
    }


    //------------------------------------------------------rgba16_gamma_dir
    template<class GammaLUT>
    rgba16 rgba16_gamma_dir(rgba16 c, const GammaLUT& gamma)
    {
        return rgba16(gamma.dir(c.r), gamma.dir(c.g), gamma.dir(c.b), c.a);
    }

    //------------------------------------------------------rgba16_gamma_inv
    template<class GammaLUT>
    rgba16 rgba16_gamma_inv(rgba16 c, const GammaLUT& gamma)
    {
        return rgba16(gamma.inv(c.r), gamma.inv(c.g), gamma.inv(c.b), c.a);
    }
#endif // 0

}



#endif
