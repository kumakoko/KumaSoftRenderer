/*********************************************************************************************
MIT License

Copyright (c) 2024 kumakoko www.xionggf.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*********************************************************************************************/
#include "ksr_light.h"
#include "ksr_model_object.h"
#include "ksr_constants.h"
#include "ksr_surface.h"

namespace KSR
{
    void Reset_Lights_LIGHTV1(void)
    {
        // 重置所有的光源，将当前的光源数归零
        static int first_time = 1;
        memset(lights, 0, MAX_LIGHTS * sizeof(LIGHTV1));
        num_lights = 0;
        first_time = 0;
    }

    int Init_Light_LIGHTV1(int index, LightOnOffState state, LightType light_type, RGBAV1 ambient,
        RGBAV1 diffuse, RGBAV1 specular, POINT4D_PTR position, VECTOR4D_PTR direction,
        float kc, float kl, float kq, float spot_inner, float spot_outer, float pf)
    {
        // 根据传入的参数初始化光源，调用该函数时，为确保创建的光源有效，将不需要的参数值设置为0
        if (index < 0 || index >= MAX_LIGHTS)
            return 0;

        // 初始化该光源
        lights[index].state = state;
        lights[index].id = index;
        lights[index].attr = light_type;
        lights[index].c_ambient = ambient; 
        lights[index].c_diffuse = diffuse; 
        lights[index].c_specular = specular;
        lights[index].kc = kc;
        lights[index].kl = kl;
        lights[index].kq = kq;

        if (nullptr != position)
        {
            VECTOR4D_COPY(&lights[index].pos, position);
        }

        if (nullptr != direction)
        {
            VECTOR4D_COPY(&lights[index].dir, direction);
            VECTOR4D_Normalize(&lights[index].dir); // 注意要单位化它
        }

        lights[index].spot_inner = spot_inner;
        lights[index].spot_outer = spot_outer;
        lights[index].pf = pf;

        return index;
    }

    int Light_OBJECT4DV1_World16(OBJECT4DV1_PTR obj,  // object to process
        CAM4DV1_PTR cam,     // camera position
        LIGHTV1_PTR lights,  // light list (might have more than one)
        int max_lights)      // maximum lights in list
    {
        // 16-bit version of function
        // function lights an object based on the sent lights and camera. the function supports
        // constant/pure shading (emmisive), flat shading with ambient, infinite, point lights, and spot lights
        // note that this lighting function is rather brute force and simply follows the math, however
        // there are some clever integer operations that are used in scale 256 rather than going to floating
        // point, but why? floating point and ints are the same speed, HOWEVER, the conversion to and from floating
        // point can be cycle intensive, so if you can keep your calcs in ints then you can gain some speed
        // also note, type 1 spot lights are simply point lights with direction, the "cone" is more of a function
        // of the falloff due to attenuation, but they still look like spot lights
        // type 2 spot lights are implemented with the intensity having a dot product relationship with the
        // angle from the surface point to the light direction just like in the optimized model, but the pf term
        // that is used for a concentration control must be 1,2,3,.... integral and non-fractional


        unsigned int r_base, g_base, b_base,  // base color being lit
            r_sum, g_sum, b_sum,   // sum of lighting process over all lights
            shaded_color;            // final color

        float dp,     // dot product 
            dist,   // distance from light to surface
            i,      // general intensities
            nl,     // length of normal
            atten;  // attenuation computations

        // test if the object is culled
        if (!(obj->state & OBJECT4DV1_STATE_ACTIVE) ||
            (obj->state & OBJECT4DV1_STATE_CULLED) ||
            !(obj->state & OBJECT4DV1_STATE_VISIBLE))
            return(0);

        // process each poly in mesh
        for (int poly = 0; poly < obj->num_polys; poly++)
        {
            // acquire polygon
            POLY4DV1_PTR curr_poly = &obj->plist[poly];

            // is this polygon valid?
            // test this polygon if and only if it's not clipped, not culled,
            // active, and visible. Note we test for backface in the event that
            // a previous call might have already determined this, so why work
            // harder!
            if (!(curr_poly->state & POLY4DV1_STATE_ACTIVE) ||
                (curr_poly->state & POLY4DV1_STATE_CLIPPED) ||
                (curr_poly->state & POLY4DV1_STATE_BACKFACE))
                continue; // move onto next poly

            // extract vertex indices into master list, rember the polygons are 
            // NOT self contained, but based on the vertex list stored in the object
            // itself
            int vindex_0 = curr_poly->vert[0];
            int vindex_1 = curr_poly->vert[1];
            int vindex_2 = curr_poly->vert[2];

            // we will use the transformed polygon vertex list since the backface removal
            // only makes sense at the world coord stage further of the pipeline 

            // test the lighting mode of the polygon (use flat for flat, gouraud))
            if (curr_poly->attr & POLY4DV1_ATTR_SHADE_MODE_FLAT || curr_poly->attr & POLY4DV1_ATTR_SHADE_MODE_GOURAUD)
            {
                // step 1: extract the base color out in RGB mode
                if (dd_pixel_format == DD_PIXEL_FORMAT565)
                {
                    _RGB565FROM16BIT(curr_poly->color, &r_base, &g_base, &b_base);

                    // scale to 8 bit 
                    r_base <<= 3;
                    g_base <<= 2;
                    b_base <<= 3;
                } // end if
                else
                {
                    _RGB555FROM16BIT(curr_poly->color, &r_base, &g_base, &b_base);

                    // scale to 8 bit 
                    r_base <<= 3;
                    g_base <<= 3;
                    b_base <<= 3;
                } // end if

             // initialize color sum
                r_sum = 0;
                g_sum = 0;
                b_sum = 0;

                // loop thru lights
                for (int curr_light = 0; curr_light < max_lights; curr_light++)
                {
                    // is this light active
                    if (lights[curr_light].state == LIGHTV1_STATE_OFF)
                        continue;

                    // what kind of light are we dealing with
                    if (lights[curr_light].attr & LIGHTV1_ATTR_AMBIENT)
                    {
                        // simply multiply each channel against the color of the 
                        // polygon then divide by 256 to scale back to 0..255
                        // use a shift in real life!!! >> 8
                        r_sum += ((lights[curr_light].c_ambient.r * r_base) / 256);
                        g_sum += ((lights[curr_light].c_ambient.g * g_base) / 256);
                        b_sum += ((lights[curr_light].c_ambient.b * b_base) / 256);

                        // there better only be one ambient light!

                    } // end if
                    else
                        if (lights[curr_light].attr & LIGHTV1_ATTR_INFINITE)
                        {
                            // infinite lighting, we need the surface normal, and the direction
                            // of the light source

                            // we need to compute the normal of this polygon face, and recall
                            // that the vertices are in cw order, u=p0->p1, v=p0->p2, n=uxv
                            VECTOR4D u, v, n;

                            // build u, v
                            VECTOR4D_Build(&obj->vlist_trans[vindex_0], &obj->vlist_trans[vindex_1], &u);
                            VECTOR4D_Build(&obj->vlist_trans[vindex_0], &obj->vlist_trans[vindex_2], &v);

                            // compute cross product
                            VECTOR4D_Cross(&u, &v, &n);

                            // at this point, we are almost ready, but we have to normalize the normal vector!
                            // this is a key optimization we can make later, we can pre-compute the length of all polygon
                            // normals, so this step can be optimized
                            // compute length of normal
                            nl = VECTOR4D_Length_Fast(&n);

                            // ok, recalling the lighting model for infinite lights
                            // I(d)dir = I0dir * Cldir
                            // and for the diffuse model
                            // Itotald =   Rsdiffuse*Idiffuse * (n . l)
                            // so we basically need to multiple it all together
                            // notice the scaling by 128, I want to avoid floating point calculations, not because they 
                            // are slower, but the conversion to and from cost cycles

                            dp = VECTOR4D_Dot(&n, &lights[curr_light].dir);

                            // only add light if dp > 0
                            if (dp > 0)
                            {
                                i = 128 * dp / nl;
                                r_sum += (lights[curr_light].c_diffuse.r * r_base * i) / (256 * 128);
                                g_sum += (lights[curr_light].c_diffuse.g * g_base * i) / (256 * 128);
                                b_sum += (lights[curr_light].c_diffuse.b * b_base * i) / (256 * 128);
                            } // end if

                        } // end if infinite light
                        else
                            if (lights[curr_light].attr & LIGHTV1_ATTR_POINT)
                            {
                                // perform point light computations
                                // light model for point light is once again:
                                //              I0point * Clpoint
                                //  I(d)point = ___________________
                                //              kc +  kl*d + kq*d2              
                                //
                                //  Where d = |p - s|
                                // thus it's almost identical to the infinite light, but attenuates as a function
                                // of distance from the point source to the surface point being lit

                                // we need to compute the normal of this polygon face, and recall
                                // that the vertices are in cw order, u=p0->p1, v=p0->p2, n=uxv
                                VECTOR4D u, v, n, l;

                                // build u, v
                                VECTOR4D_Build(&obj->vlist_trans[vindex_0], &obj->vlist_trans[vindex_1], &u);
                                VECTOR4D_Build(&obj->vlist_trans[vindex_0], &obj->vlist_trans[vindex_2], &v);

                                // compute cross product
                                VECTOR4D_Cross(&u, &v, &n);

                                // at this point, we are almost ready, but we have to normalize the normal vector!
                                // this is a key optimization we can make later, we can pre-compute the length of all polygon
                                // normals, so this step can be optimized
                                // compute length of normal
                                nl = VECTOR4D_Length_Fast(&n);

                                // compute vector from surface to light
                                VECTOR4D_Build(&obj->vlist_trans[vindex_0], &lights[curr_light].pos, &l);

                                // compute distance and attenuation
                                dist = VECTOR4D_Length_Fast(&l);

                                // and for the diffuse model
                                // Itotald =   Rsdiffuse*Idiffuse * (n . l)
                                // so we basically need to multiple it all together
                                // notice the scaling by 128, I want to avoid floating point calculations, not because they 
                                // are slower, but the conversion to and from cost cycles
                                dp = VECTOR4D_Dot(&n, &l);

                                // only add light if dp > 0
                                if (dp > 0)
                                {
                                    atten = (lights[curr_light].kc + lights[curr_light].kl * dist + lights[curr_light].kq * dist * dist);

                                    i = 128 * dp / (nl * dist * atten);

                                    r_sum += (lights[curr_light].c_diffuse.r * r_base * i) / (256 * 128);
                                    g_sum += (lights[curr_light].c_diffuse.g * g_base * i) / (256 * 128);
                                    b_sum += (lights[curr_light].c_diffuse.b * b_base * i) / (256 * 128);
                                } // end if

                            } // end if point
                            else
                                if (lights[curr_light].attr & LIGHTV1_ATTR_SPOTLIGHT1)
                                {
                                    // perform spotlight/point computations simplified model that uses
                                    // point light WITH a direction to simulate a spotlight
                                    // light model for point light is once again:
                                    //              I0point * Clpoint
                                    //  I(d)point = ___________________
                                    //              kc +  kl*d + kq*d2              
                                    //
                                    //  Where d = |p - s|
                                    // thus it's almost identical to the infinite light, but attenuates as a function
                                    // of distance from the point source to the surface point being lit

                                    // we need to compute the normal of this polygon face, and recall
                                    // that the vertices are in cw order, u=p0->p1, v=p0->p2, n=uxv
                                    VECTOR4D u, v, n, l;

                                    // build u, v
                                    VECTOR4D_Build(&obj->vlist_trans[vindex_0], &obj->vlist_trans[vindex_1], &u);
                                    VECTOR4D_Build(&obj->vlist_trans[vindex_0], &obj->vlist_trans[vindex_2], &v);

                                    // compute cross product (we need -n, so do vxu)
                                    VECTOR4D_Cross(&v, &u, &n);

                                    // at this point, we are almost ready, but we have to normalize the normal vector!
                                    // this is a key optimization we can make later, we can pre-compute the length of all polygon
                                    // normals, so this step can be optimized
                                    // compute length of normal
                                    nl = VECTOR4D_Length_Fast(&n);

                                    // compute vector from surface to light
                                    VECTOR4D_Build(&obj->vlist_trans[vindex_0], &lights[curr_light].pos, &l);

                                    // compute distance and attenuation
                                    dist = VECTOR4D_Length_Fast(&l);

                                    // and for the diffuse model
                                    // Itotald =   Rsdiffuse*Idiffuse * (n . l)
                                    // so we basically need to multiple it all together
                                    // notice the scaling by 128, I want to avoid floating point calculations, not because they 
                                    // are slower, but the conversion to and from cost cycles

                                    // note that I use the direction of the light here rather than a the vector to the light
                                    // thus we are taking orientation into account which is similar to the spotlight model
                                    dp = VECTOR4D_Dot(&n, &lights[curr_light].dir);

                                    // only add light if dp > 0
                                    if (dp > 0)
                                    {
                                        atten = (lights[curr_light].kc + lights[curr_light].kl * dist + lights[curr_light].kq * dist * dist);

                                        i = 128 * dp / (nl * atten);

                                        r_sum += (lights[curr_light].c_diffuse.r * r_base * i) / (256 * 128);
                                        g_sum += (lights[curr_light].c_diffuse.g * g_base * i) / (256 * 128);
                                        b_sum += (lights[curr_light].c_diffuse.b * b_base * i) / (256 * 128);
                                    } // end if

                                } // end if spotlight1
                                else
                                    if (lights[curr_light].attr & LIGHTV1_ATTR_SPOTLIGHT2) // simple version
                                    {
                                        // perform spot light computations
                                        // light model for spot light simple version is once again:
                                        //         	     I0spotlight * Clspotlight * MAX( (l . s), 0)^pf                     
                                        // I(d)spotlight = __________________________________________      
                                        //               		 kc + kl*d + kq*d2        
                                        // Where d = |p - s|, and pf = power factor

                                        // thus it's almost identical to the point, but has the extra term in the numerator
                                        // relating the angle between the light source and the point on the surface

                                        // we need to compute the normal of this polygon face, and recall
                                        // that the vertices are in cw order, u=p0->p1, v=p0->p2, n=uxv
                                        VECTOR4D u, v, n, d, s;

                                        // build u, v
                                        VECTOR4D_Build(&obj->vlist_trans[vindex_0], &obj->vlist_trans[vindex_1], &u);
                                        VECTOR4D_Build(&obj->vlist_trans[vindex_0], &obj->vlist_trans[vindex_2], &v);

                                        // compute cross product (v x u, to invert n)
                                        VECTOR4D_Cross(&v, &u, &n);

                                        // at this point, we are almost ready, but we have to normalize the normal vector!
                                        // this is a key optimization we can make later, we can pre-compute the length of all polygon
                                        // normals, so this step can be optimized
                                        // compute length of normal
                                        nl = VECTOR4D_Length_Fast(&n);

                                        // and for the diffuse model
                                        // Itotald =   Rsdiffuse*Idiffuse * (n . l)
                                        // so we basically need to multiple it all together
                                        // notice the scaling by 128, I want to avoid floating point calculations, not because they 
                                        // are slower, but the conversion to and from cost cycles
                                        dp = VECTOR4D_Dot(&n, &lights[curr_light].dir);

                                        // only add light if dp > 0
                                        if (dp > 0)
                                        {
                                            // compute vector from light to surface (different from l which IS the light dir)
                                            VECTOR4D_Build(&lights[curr_light].pos, &obj->vlist_trans[vindex_0], &s);

                                            // compute length of s (distance to light source) to normalize s for lighting calc
                                            dist = VECTOR4D_Length_Fast(&s);

                                            // compute spot light term (s . l)
                                            float dpsl = VECTOR4D_Dot(&s, &lights[curr_light].dir) / dist;

                                            // proceed only if term is positive
                                            if (dpsl > 0)
                                            {
                                                // compute attenuation
                                                atten = (lights[curr_light].kc + lights[curr_light].kl * dist + lights[curr_light].kq * dist * dist);

                                                // for speed reasons, pf exponents that are less that 1.0 are out of the question, and exponents
                                                // must be integral
                                                float dpsl_exp = dpsl;

                                                // exponentiate for positive integral powers
                                                for (int e_index = 1; e_index < (int)lights[curr_light].pf; e_index++)
                                                    dpsl_exp *= dpsl;

                                                // now dpsl_exp holds (dpsl)^pf power which is of course (s . l)^pf 

                                                i = 128 * dp * dpsl_exp / (nl * atten);

                                                r_sum += (lights[curr_light].c_diffuse.r * r_base * i) / (256 * 128);
                                                g_sum += (lights[curr_light].c_diffuse.g * g_base * i) / (256 * 128);
                                                b_sum += (lights[curr_light].c_diffuse.b * b_base * i) / (256 * 128);

                                            } // end if

                                        } // end if

                                    } // end if spot light

                } // end for light

            // make sure colors aren't out of range
                if (r_sum > 255) r_sum = 255;
                if (g_sum > 255) g_sum = 255;
                if (b_sum > 255) b_sum = 255;

                // write the color
                shaded_color = RGB16Bit(r_sum, g_sum, b_sum);
                curr_poly->color = (int)((shaded_color << 16) | curr_poly->color);

            } // end if
            else // assume POLY4DV1_ATTR_SHADE_MODE_CONSTANT
            {
                // emmisive shading only, copy base color into upper 16-bits
                // without any change
                curr_poly->color = (int)((curr_poly->color << 16) | curr_poly->color);
            } // end if

        } // end for poly

    // return success
        return(1);

    } // end Light_OBJECT4DV1_World16
}