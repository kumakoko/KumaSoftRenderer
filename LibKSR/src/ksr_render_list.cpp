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

#include "ksr_render_list.h"
#include "ksr_transform.h"
#include "ksr_shape_drawing.h"

namespace KSR
{
    void Reset_RENDERLIST4DV1(RENDERLIST4DV1_PTR rend_list)
    {
        // this function intializes and resets the sent render list and
        // redies it for polygons/faces to be inserted into it
        // note that the render list in this version is composed
        // of an array FACE4DV1 pointer objects, you call this
        // function each frame

        // since we are tracking the number of polys in the
        // list via num_polys we can set it to 0
        // but later we will want a more robust scheme if
        // we generalize the linked list more and disconnect
        // it from the polygon pointer list
        rend_list->num_polys = 0; // that was hard!

    }  // Reset_RENDERLIST4DV1


    int Insert_POLYF4DV1_RENDERLIST4DV1(RENDERLIST4DV1_PTR rend_list, POLYF4DV1_PTR poly)
    {
        // inserts the sent polyface POLYF4DV1 into the render list

        // step 0: are we full?
        if (rend_list->num_polys >= RENDERLIST4DV1_MAX_POLYS)
            return(0);

        // step 1: copy polygon into next opening in polygon render list

        // point pointer to polygon structure
        rend_list->poly_ptrs[rend_list->num_polys] = &rend_list->poly_data[rend_list->num_polys];

        // copy face right into array, thats it
        memcpy((void*)&rend_list->poly_data[rend_list->num_polys], (void*)poly, sizeof(POLYF4DV1));

        // now the polygon is loaded into the next free array position, but
        // we need to fix up the links
        // test if this is the first entry
        if (rend_list->num_polys == 0)
        {
            // set pointers to null, could loop them around though to self
            rend_list->poly_data[0].next = nullptr;
            rend_list->poly_data[0].prev = nullptr;
        } // end if
        else
        {
            // first set this node to point to previous node and next node (null)
            rend_list->poly_data[rend_list->num_polys].next = nullptr;
            rend_list->poly_data[rend_list->num_polys].prev =
                &rend_list->poly_data[rend_list->num_polys - 1];

            // now set previous node to point to this node
            rend_list->poly_data[rend_list->num_polys - 1].next =
                &rend_list->poly_data[rend_list->num_polys];
        } // end else

     // increment number of polys in list
        rend_list->num_polys++;

        // return successful insertion
        return(1);

    } // end Insert_POLYF4DV1_RENDERLIST4DV1

    /**************************************************************************************
     判断给定的多边形polygon要不要被渲染
     * @name: _Polygon4DV1NeedToRender
     * @return: bool 要true不要false
     * @param: POLYF4DV1* polygon  待检查的多边形对象指针
     *************************************************************************************/
    static bool _Polygon4DV1NeedToRender(POLYF4DV1* polygon)
    {
        return nullptr != polygon &&                        // 这个多边形必须不为null
            polygon->state & POLY4DV1_STATE_ACTIVE &&       // 这个多边形必须为活跃状态
            !(polygon->state & POLY4DV1_STATE_CLIPPED) &&   // 这个多边形必须没有被裁剪掉
            !(polygon->state & POLY4DV1_STATE_BACKFACE);    // 这个多边形必须没有背对着摄像机
    }

    void Transform_RENDERLIST4DV1(RENDERLIST4DV1_PTR rend_list, MATRIX4X4_PTR mt, TransformControlFlag coord_select)
    {
        if (TRANSFORM_LOCAL_ONLY == coord_select) // 查看ksr_transform.h中的TransformControlFlag::TRANSFORM_LOCAL_ONLY枚举值的说明
        {
            for (int poly = 0; poly < rend_list->num_polys; poly++) // 遍历render list中的每一个多边形
            {
                POLYF4DV1_PTR curr_poly = rend_list->poly_ptrs[poly];
                if (!_Polygon4DV1NeedToRender(curr_poly))
                    continue; //当前这个多边形不满足渲染条件，跳过，检查下一个多边形

                // 遍历多边形中的三个顶点，对这些存储在vlist数组成员变量的顶点坐标，使用mt矩阵，进行变换，然后再存回到vlist数组中去
                for (int vertex = 0; vertex < 3; vertex++)
                {
                    POINT4D presult;
                    Mat_Mul_VECTOR4D_4X4(&curr_poly->vlist[vertex], mt, &presult);
                    VECTOR4D_COPY(&curr_poly->vlist[vertex], &presult);
                }
            }
        }
        else if (TRANSFORM_TRANS_ONLY == coord_select) // 查看ksr_transform.h中的TransformControlFlag::TRANSFORM_TRANS_ONLY枚举值的说明
        {
            for (int poly = 0; poly < rend_list->num_polys; poly++) // 遍历render list中的每一个多边形
            {
                POLYF4DV1_PTR curr_poly = rend_list->poly_ptrs[poly];
                if (!_Polygon4DV1NeedToRender(curr_poly))
                    continue; // 当前这个多边形不满足渲染条件，跳过，检查下一个多边形

                // 遍历多边形中的三个顶点，对这些存储在tvlist数组成员变量的顶点坐标，使用mt矩阵，进行变换，然后再存回到tvlist数组中去
                for (int vertex = 0; vertex < 3; vertex++)
                {
                    POINT4D presult;
                    Mat_Mul_VECTOR4D_4X4(&curr_poly->tvlist[vertex], mt, &presult);
                    VECTOR4D_COPY(&curr_poly->tvlist[vertex], &presult);
                }
            }
        }
        else if (TRANSFORM_LOCAL_TO_TRANS == coord_select) // 查看ksr_transform.h中的TransformControlFlag::TRANSFORM_LOCAL_TO_TRANS枚举值的说明
        {
            for (int poly = 0; poly < rend_list->num_polys; poly++)
            {
                POLYF4DV1_PTR curr_poly = rend_list->poly_ptrs[poly];
                if (!_Polygon4DV1NeedToRender(curr_poly))
                    continue; //当前这个多边形不满足渲染条件，跳过，检查下一个多边形

                // 遍历多边形中的三个顶点，对这些存储在vlist数组成员变量的顶点坐标，使用mt矩阵，进行变换，然后存到tvlist数组中去
                for (int vertex = 0; vertex < 3; vertex++)
                {
                    Mat_Mul_VECTOR4D_4X4(&curr_poly->vlist[vertex], mt, &curr_poly->tvlist[vertex]);
                }
            }
        }
    }

    void Model_To_World_RENDERLIST4DV1(RENDERLIST4DV1_PTR rend_list, POINT4D_PTR world_pos, TransformControlFlag coord_select)
    {
        if (coord_select == TRANSFORM_LOCAL_TO_TRANS)
        {
            for (int poly = 0; poly < rend_list->num_polys; poly++)
            {
                POLYF4DV1_PTR curr_poly = rend_list->poly_ptrs[poly];
                if (!_Polygon4DV1NeedToRender(curr_poly))
                    continue; //当前这个多边形不满足渲染条件，跳过，检查下一个多边形

                for (int vertex = 0; vertex < 3; vertex++) // 将vlist中的顶点平移，得出的值，存到tvlist中
                {
                    VECTOR4D_Add(&curr_poly->vlist[vertex], world_pos, &curr_poly->tvlist[vertex]);
                }
            }
        }
        else // TRANSFORM_TRANS_ONLY 和 TRANSFORM_LOCAL_ONLY 都一样，是直接操作tvlist
        {
            for (int poly = 0; poly < rend_list->num_polys; poly++)
            {
                POLYF4DV1_PTR curr_poly = rend_list->poly_ptrs[poly];
                if (!_Polygon4DV1NeedToRender(curr_poly))
                    continue; //当前这个多边形不满足渲染条件，跳过，检查下一个多边形

                for (int vertex = 0; vertex < 3; vertex++) // 将tvlist中的顶点平移，得出的指，原样存回到tvlist中
                {
                    VECTOR4D_Add(&curr_poly->tvlist[vertex], world_pos, &curr_poly->tvlist[vertex]);
                }
            }
        }
    }


    void World_To_Camera_RENDERLIST4DV1(RENDERLIST4DV1_PTR rend_list, CAM4DV1_PTR cam)
    {
        // NOTE: this is a matrix based function
        // this function transforms each polygon in the global render list
        // to camera coordinates based on the sent camera transform matrix
        // you would use this function instead of the object based function
        // if you decided earlier in the pipeline to turn each object into 
        // a list of polygons and then add them to the global render list
        // the conversion of an object into polygons probably would have
        // happened after object culling, local transforms, local to world
        // and backface culling, so the minimum number of polygons from
        // each object are in the list, note that the function assumes
        // that at LEAST the local to world transform has been called
        // and the polygon data is in the transformed list tvlist of
        // the POLYF4DV1 object

        // transform each polygon in the render list into camera coordinates
        // assumes the render list has already been transformed to world
        // coordinates and the result is in tvlist[] of each polygon object

        for (int poly = 0; poly < rend_list->num_polys; poly++)
        {
            // acquire current polygon
            POLYF4DV1_PTR curr_poly = rend_list->poly_ptrs[poly];
            if (!_Polygon4DV1NeedToRender(curr_poly))
                continue; //当前这个多边形不满足渲染条件，跳过，检查下一个多边形

            // all good, let's transform 
            for (int vertex = 0; vertex < 3; vertex++)
            {
                // transform the vertex by the mcam matrix within the camera
                // it better be valid!
                POINT4D presult; // hold result of each transformation

                // transform point
                Mat_Mul_VECTOR4D_4X4(&curr_poly->tvlist[vertex], &cam->mcam, &presult);

                // store result back
                VECTOR4D_COPY(&curr_poly->tvlist[vertex], &presult);
            } // end for vertex

        } // end for poly

    } // end World_To_Camera_RENDERLIST4DV1

    ///////////////////////////////////////////////////////////////

    void Camera_To_Perspective_RENDERLIST4DV1(RENDERLIST4DV1_PTR rend_list,
        CAM4DV1_PTR cam)
    {
        // NOTE: this is not a matrix based function
        // this function transforms each polygon in the global render list
        // into perspective coordinates, based on the 
        // sent camera object, 
        // you would use this function instead of the object based function
        // if you decided earlier in the pipeline to turn each object into 
        // a list of polygons and then add them to the global render list

        // transform each polygon in the render list into camera coordinates
        // assumes the render list has already been transformed to world
        // coordinates and the result is in tvlist[] of each polygon object

        for (int poly = 0; poly < rend_list->num_polys; poly++)
        {
            // acquire current polygon
            POLYF4DV1_PTR curr_poly = rend_list->poly_ptrs[poly];

            // is this polygon valid?
            // transform this polygon if and only if it's not clipped, not culled,
            // active, and visible, note however the concept of "backface" is 
            // irrelevant in a wire frame engine though
            if ((curr_poly == nullptr) || !(curr_poly->state & POLY4DV1_STATE_ACTIVE) ||
                (curr_poly->state & POLY4DV1_STATE_CLIPPED) ||
                (curr_poly->state & POLY4DV1_STATE_BACKFACE))
                continue; // move onto next poly

            // all good, let's transform 
            for (int vertex = 0; vertex < 3; vertex++)
            {
                float z = curr_poly->tvlist[vertex].z;

                // transform the vertex by the view parameters in the camera
                curr_poly->tvlist[vertex].x = cam->view_dist * curr_poly->tvlist[vertex].x / z;
                curr_poly->tvlist[vertex].y = cam->view_dist * curr_poly->tvlist[vertex].y * cam->aspect_ratio / z;
                // z = z, so no change

                // not that we are NOT dividing by the homogenous w coordinate since
                // we are not using a matrix operation for this version of the function 

            } // end for vertex

        } // end for poly

    } // end Camera_To_Perspective_RENDERLIST4DV1

    ////////////////////////////////////////////////////////////////

    void Camera_To_Perspective_Screen_RENDERLIST4DV1(RENDERLIST4DV1_PTR rend_list,
        CAM4DV1_PTR cam)
    {
        // NOTE: this is not a matrix based function
        // this function transforms the camera coordinates of an object
        // into Screen scaled perspective coordinates, based on the 
        // sent camera object, that is, view_dist_h and view_dist_v 
        // should be set to cause the desired (viewport_width X viewport_height)
        // it only works on the vertices in the tvlist[] list
        // finally, the function also inverts the y axis, so the coordinates
        // generated from this function ARE screen coordinates and ready for
        // rendering

        // transform each polygon in the render list to perspective screen 
        // coordinates assumes the render list has already been transformed 
        // to camera coordinates and the result is in tvlist[]
        for (int poly = 0; poly < rend_list->num_polys; poly++)
        {
            // acquire current polygon
            POLYF4DV1_PTR curr_poly = rend_list->poly_ptrs[poly];

            // is this polygon valid?
            // transform this polygon if and only if it's not clipped, not culled,
            // active, and visible, note however the concept of "backface" is 
            // irrelevant in a wire frame engine though
            if ((curr_poly == nullptr) || !(curr_poly->state & POLY4DV1_STATE_ACTIVE) ||
                (curr_poly->state & POLY4DV1_STATE_CLIPPED) ||
                (curr_poly->state & POLY4DV1_STATE_BACKFACE))
                continue; // move onto next poly

            float alpha = (0.5f * cam->viewport_width - 0.5f);
            float beta = (0.5f * cam->viewport_height - 0.5f);

            // all good, let's transform 
            for (int vertex = 0; vertex < 3; vertex++)
            {
                float z = curr_poly->tvlist[vertex].z;

                // transform the vertex by the view parameters in the camera
                curr_poly->tvlist[vertex].x = cam->view_dist * curr_poly->tvlist[vertex].x / z;
                curr_poly->tvlist[vertex].y = cam->view_dist * curr_poly->tvlist[vertex].y / z;
                // z = z, so no change

                // not that we are NOT dividing by the homogenous w coordinate since
                // we are not using a matrix operation for this version of the function 

                // now the coordinates are in the range x:(-viewport_width/2 to viewport_width/2)
                // and y:(-viewport_height/2 to viewport_height/2), thus we need a translation and
                // since the y-axis is inverted, we need to invert y to complete the screen 
                // transform:
                curr_poly->tvlist[vertex].x = curr_poly->tvlist[vertex].x + alpha;
                curr_poly->tvlist[vertex].y = -curr_poly->tvlist[vertex].y + beta;

            } // end for vertex

        } // end for poly

    } // end Camera_To_Perspective_Screen_RENDERLIST4DV1

    //////////////////////////////////////////////////////////////

    void Perspective_To_Screen_RENDERLIST4DV1(RENDERLIST4DV1_PTR rend_list,
        CAM4DV1_PTR cam)
    {
        // NOTE: this is not a matrix based function
        // this function transforms the perspective coordinates of the render
        // list into screen coordinates, based on the sent viewport in the camera
        // assuming that the viewplane coordinates were normalized
        // you would use this function instead of the object based function
        // if you decided earlier in the pipeline to turn each object into 
        // a list of polygons and then add them to the global render list
        // you would only call this function if you previously performed
        // a normalized perspective transform

        // transform each polygon in the render list from perspective to screen 
        // coordinates assumes the render list has already been transformed 
        // to normalized perspective coordinates and the result is in tvlist[]
        for (int poly = 0; poly < rend_list->num_polys; poly++)
        {
            // acquire current polygon
            POLYF4DV1_PTR curr_poly = rend_list->poly_ptrs[poly];

            // is this polygon valid?
            // transform this polygon if and only if it's not clipped, not culled,
            // active, and visible, note however the concept of "backface" is 
            // irrelevant in a wire frame engine though
            if ((curr_poly == nullptr) || !(curr_poly->state & POLY4DV1_STATE_ACTIVE) ||
                (curr_poly->state & POLY4DV1_STATE_CLIPPED) ||
                (curr_poly->state & POLY4DV1_STATE_BACKFACE))
                continue; // move onto next poly

            float alpha = (0.5f * cam->viewport_width - 0.5f);
            float beta = (0.5f * cam->viewport_height - 0.5f);

            // all good, let's transform 
            for (int vertex = 0; vertex < 3; vertex++)
            {
                // the vertex is in perspective normalized coords from -1 to 1
                // on each axis, simple scale them and invert y axis and project
                // to screen

                // transform the vertex by the view parameters in the camera
                curr_poly->tvlist[vertex].x = alpha + alpha * curr_poly->tvlist[vertex].x;
                curr_poly->tvlist[vertex].y = beta - beta * curr_poly->tvlist[vertex].y;
            } // end for vertex

        } // end for poly

    } // end Perspective_To_Screen_RENDERLIST4DV1


    void Draw_RENDERLIST4DV1_Wire16(RENDERLIST4DV1_PTR rend_list, uint8_t* video_buffer, int lpitch)
    {
        // this function "executes" the render list or in other words
        // draws all the faces in the list in wire frame 16bit mode
        // note there is no need to sort wire frame polygons, but 
        // later we will need to, so hidden surfaces stay hidden
        // also, we leave it to the function to determine the bitdepth
        // and call the correct rasterizer

        // at this point, all we have is a list of polygons and it's time
        // to draw them
        for (int poly = 0; poly < rend_list->num_polys; poly++)
        {
            // render this polygon if and only if it's not clipped, not culled,
            // active, and visible, note however the concecpt of "backface" is 
            // irrelevant in a wire frame engine though
            if (!(rend_list->poly_ptrs[poly]->state & POLY4DV1_STATE_ACTIVE) ||
                (rend_list->poly_ptrs[poly]->state & POLY4DV1_STATE_CLIPPED) ||
                (rend_list->poly_ptrs[poly]->state & POLY4DV1_STATE_BACKFACE))
                continue; // move onto next poly

            // draw the triangle edge one, note that clipping was already set up
            // by 2D initialization, so line clipper will clip all polys out
            // of the 2D screen/window boundary
            Draw_Clip_Line16(
                (int)rend_list->poly_ptrs[poly]->tvlist[0].x,
                (int)rend_list->poly_ptrs[poly]->tvlist[0].y,
                (int)rend_list->poly_ptrs[poly]->tvlist[1].x,
                (int)rend_list->poly_ptrs[poly]->tvlist[1].y,
                rend_list->poly_ptrs[poly]->color,
                video_buffer, lpitch);

            Draw_Clip_Line16(
                (int)rend_list->poly_ptrs[poly]->tvlist[1].x,
                (int)rend_list->poly_ptrs[poly]->tvlist[1].y,
                (int)rend_list->poly_ptrs[poly]->tvlist[2].x,
                (int)rend_list->poly_ptrs[poly]->tvlist[2].y,
                rend_list->poly_ptrs[poly]->color,
                video_buffer, lpitch);

            Draw_Clip_Line16(
                (int)rend_list->poly_ptrs[poly]->tvlist[2].x,
                (int)rend_list->poly_ptrs[poly]->tvlist[2].y,
                (int)rend_list->poly_ptrs[poly]->tvlist[0].x,
                (int)rend_list->poly_ptrs[poly]->tvlist[0].y,
                rend_list->poly_ptrs[poly]->color,
                video_buffer, lpitch);

            // track rendering stats
#ifdef DEBUG_ON
            debug_polys_rendered_per_frame++;
#endif

        } // end for poly

    } // end Draw_RENDERLIST4DV1_Wire

}