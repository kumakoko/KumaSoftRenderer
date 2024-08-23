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
#include "ksr_constants.h"

namespace KSR
{
    // P358
    void Reset_RENDERLIST4DV1(RENDERLIST4DV1_PTR render_list)
    {
        // 这里用了render list的num_polys成员来跟踪渲染列表包含的多边形数目，因此将其直接硬编码为0
        // 这个方案比较粗暴，如果需要使得render list更加通用则需要采用更健壮的方案，并将其与多边形指针列表的关联切断
        render_list->num_polys = 0;
    }

    int Insert_POLYF4DV1_RENDERLIST4DV1(RENDERLIST4DV1_PTR render_list, POLYF4DV1_PTR poly)
    {
        // inserts the sent polyface POLYF4DV1 into the render list

        // step 0: are we full?
        if (render_list->num_polys >= RENDERLIST4DV1_MAX_POLYS)
            return(0);

        // step 1: copy polygon into next opening in polygon render list

        // point pointer to polygon structure
        render_list->poly_ptrs[render_list->num_polys] = &render_list->poly_data[render_list->num_polys];

        // copy face right into array, thats it
        memcpy((void*)&render_list->poly_data[render_list->num_polys], (void*)poly, sizeof(POLYF4DV1));

        // now the polygon is loaded into the next free array position, but
        // we need to fix up the links
        // test if this is the first entry
        if (render_list->num_polys == 0)
        {
            // set pointers to null, could loop them around though to self
            render_list->poly_data[0].next = nullptr;
            render_list->poly_data[0].prev = nullptr;
        } // end if
        else
        {
            // first set this node to point to previous node and next node (null)
            render_list->poly_data[render_list->num_polys].next = nullptr;
            render_list->poly_data[render_list->num_polys].prev =
                &render_list->poly_data[render_list->num_polys - 1];

            // now set previous node to point to this node
            render_list->poly_data[render_list->num_polys - 1].next =
                &render_list->poly_data[render_list->num_polys];
        } // end else

     // increment number of polys in list
        render_list->num_polys++;

        // return successful insertion
        return(1);

    } // end Insert_POLYF4DV1_RENDERLIST4DV1

    //P461
    bool Insert_OBJECT4DV1_RENDERLIST4DV12(RENDERLIST4DV1_PTR rend_list,OBJECT4DV1_PTR obj,int insert_local,int lighting_on)
    {
        // 将物体转换为一个多边形面列表
        // 然后将可见的，活动，未被剔除的，未被裁剪掉的多边形插入到渲染列表中
        // 参数insert_local 指定使用顶点列表vlist_local还是vlist_trans
        // 如果insert_local为1，将把未变换的原始物体插入到渲染列表中
        // 该参数的默认值为0，即只插入只是奥经过了局部坐标到世界坐标变换的物体
        // 最后一个参数指出之前是否执行了光照计算，光照计算生成的颜色值被存储在前16位中
        // 如果light_on = 1，则将多边形插入到渲染列表时，将使用这种颜色覆盖多边形原来的颜色

        unsigned int base_color; // save base color of polygon

        // 该物体不处于活动状态被删除掉或者不可见吗
        if (!(obj->state & OBJECT4DV1_STATE_ACTIVE) || (obj->state & OBJECT4DV1_STATE_CULLED) || !(obj->state & OBJECT4DV1_STATE_VISIBLE))
            return 0;

        // 提取本object包含的多边形
        for (int poly = 0; poly < obj->num_polys; poly++)
        {
            POLY4DV1_PTR curr_poly = &obj->plist[poly];

            // 多边形不可见，多边形不活跃，多边形背向摄像机的话，不渲染，处理下一个
            if (!(curr_poly->state & POLY4DV1_STATE_ACTIVE) || 
                curr_poly->state & POLY4DV1_STATE_CLIPPED ||
                curr_poly->state & POLY4DV1_STATE_BACKFACE)
                continue;

            // 如果要使用局部坐标，改变多边形指向的顶点列表，首先要保存原来的指针
            POINT4D_PTR vlist_old = curr_poly->vlist;

            if (insert_local)
                curr_poly->vlist = obj->vlist_local;
            else
                curr_poly->vlist = obj->vlist_trans;

            // 判断是否需要使用前16位中的颜色覆盖原来的颜色
            if (lighting_on == 1)
            {
                base_color = static_cast<uint32_t>(curr_poly->color);
                curr_poly->color = static_cast<int32_t>(base_color >> 16);
            }

            // 插入多边形，如果插入失败的话恢复顶点指针并返回false
            if (!Insert_POLY4DV1_RENDERLIST4DV1(rend_list, curr_poly))
            {
                curr_poly->vlist = vlist_old;
                return false;
            }

            // 判断是否需要使用前16位中的颜色覆盖原来的颜色
            if (lighting_on == 1)
            {
                curr_poly->color = static_cast<uint32_t>(base_color & 0xffff);
            }

            // 恢复顶点列表指针
            curr_poly->vlist = vlist_old;
        }

        return true;
    }

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

    void Transform_RENDERLIST4DV1(RENDERLIST4DV1_PTR render_list, MATRIX4X4_PTR mat_transform, TransformControlFlag coord_select)
    {
        if (TRANSFORM_LOCAL_ONLY == coord_select) // 查看ksr_transform.h中的TransformControlFlag::TRANSFORM_LOCAL_ONLY枚举值的说明
        {
            for (int poly = 0; poly < render_list->num_polys; poly++) // 遍历render list中的每一个多边形
            {
                POLYF4DV1_PTR current_polygon = render_list->poly_ptrs[poly];
                if (!_Polygon4DV1NeedToRender(current_polygon))
                    continue; //当前这个多边形不满足渲染条件，跳过，检查下一个多边形

                // 遍历多边形中的三个顶点，对这些存储在vlist数组成员变量的顶点坐标，使用mt矩阵，进行变换，然后再存回到vlist数组中去
                for (int vertex = 0; vertex < 3; vertex++)
                {
                    POINT4D presult;
                    Mat_Mul_VECTOR4D_4X4(&current_polygon->vlist[vertex], mat_transform, &presult);
                    VECTOR4D_COPY(&current_polygon->vlist[vertex], &presult);
                }
            }
        }
        else if (TRANSFORM_TRANS_ONLY == coord_select) // 查看ksr_transform.h中的TransformControlFlag::TRANSFORM_TRANS_ONLY枚举值的说明
        {
            for (int poly = 0; poly < render_list->num_polys; poly++) // 遍历render list中的每一个多边形
            {
                POLYF4DV1_PTR current_polygon = render_list->poly_ptrs[poly];
                if (!_Polygon4DV1NeedToRender(current_polygon))
                    continue; // 当前这个多边形不满足渲染条件，跳过，检查下一个多边形

                // 遍历多边形中的三个顶点，对这些存储在tvlist数组成员变量的顶点坐标，使用mt矩阵，进行变换，然后再存回到tvlist数组中去
                for (int vertex = 0; vertex < 3; vertex++)
                {
                    POINT4D presult;
                    Mat_Mul_VECTOR4D_4X4(&current_polygon->tvlist[vertex], mat_transform, &presult);
                    VECTOR4D_COPY(&current_polygon->tvlist[vertex], &presult);
                }
            }
        }
        else if (TRANSFORM_LOCAL_TO_TRANS == coord_select) // 查看ksr_transform.h中的TransformControlFlag::TRANSFORM_LOCAL_TO_TRANS枚举值的说明
        {
            for (int poly = 0; poly < render_list->num_polys; poly++)
            {
                POLYF4DV1_PTR current_polygon = render_list->poly_ptrs[poly];
                if (!_Polygon4DV1NeedToRender(current_polygon))
                    continue; //当前这个多边形不满足渲染条件，跳过，检查下一个多边形

                // 遍历多边形中的三个顶点，对这些存储在vlist数组成员变量的顶点坐标，使用mt矩阵，进行变换，然后存到tvlist数组中去
                for (int vertex = 0; vertex < 3; vertex++)
                {
                    Mat_Mul_VECTOR4D_4X4(&current_polygon->vlist[vertex], mat_transform, &current_polygon->tvlist[vertex]);
                }
            }
        }
    }

    void Model_To_World_RENDERLIST4DV1(RENDERLIST4DV1_PTR render_list, POINT4D_PTR world_pos, TransformControlFlag coord_select)
    {
        if (coord_select == TRANSFORM_LOCAL_TO_TRANS)
        {
            for (int poly = 0; poly < render_list->num_polys; poly++)
            {
                POLYF4DV1_PTR current_polygon = render_list->poly_ptrs[poly];
                if (!_Polygon4DV1NeedToRender(current_polygon))
                    continue; //当前这个多边形不满足渲染条件，跳过，检查下一个多边形

                for (int vertex = 0; vertex < 3; vertex++) // 将vlist中的顶点平移，得出的值，存到tvlist中
                {
                    VECTOR4D_Add(&current_polygon->vlist[vertex], world_pos, &current_polygon->tvlist[vertex]);
                }
            }
        }
        else // TRANSFORM_TRANS_ONLY 和 TRANSFORM_LOCAL_ONLY 都一样，是直接操作tvlist
        {
            for (int poly = 0; poly < render_list->num_polys; poly++)
            {
                POLYF4DV1_PTR current_polygon = render_list->poly_ptrs[poly];
                if (!_Polygon4DV1NeedToRender(current_polygon))
                    continue; //当前这个多边形不满足渲染条件，跳过，检查下一个多边形

                for (int vertex = 0; vertex < 3; vertex++) // 将tvlist中的顶点平移，得出的指，原样存回到tvlist中
                {
                    VECTOR4D_Add(&current_polygon->tvlist[vertex], world_pos, &current_polygon->tvlist[vertex]);
                }
            }
        }
    }

    void World_To_Camera_RENDERLIST4DV1(RENDERLIST4DV1_PTR render_list, CAM4DV1_PTR camera)
    {
        for (int poly = 0; poly < render_list->num_polys; poly++)
        {
            POLYF4DV1_PTR current_polygon = render_list->poly_ptrs[poly];

            if (!_Polygon4DV1NeedToRender(current_polygon))
                continue; //当前这个多边形不满足渲染条件，跳过，检查下一个多边形

            for (int vertex = 0; vertex < 3; vertex++)
            {
                // 当下tvlist中的顶点数据是基于世界坐标系的。世界坐标系下的顶点位置数据，乘以
                // 观察变换矩阵（相机变换矩阵），即相机的mcam。即将其位置数据转变到观察空间下。
                // 再把经过变换的顶点位置数据回存到tvlist成员变量
                POINT4D presult;
                Mat_Mul_VECTOR4D_4X4(&current_polygon->tvlist[vertex], &camera->mcam, &presult);
                VECTOR4D_COPY(&current_polygon->tvlist[vertex], &presult);
            }
        }
    }

    void Camera_To_Perspective_RENDERLIST4DV1(RENDERLIST4DV1_PTR render_list, CAM4DV1_PTR camera)
    {
        for (int poly = 0; poly < render_list->num_polys; poly++)
        {
            POLYF4DV1_PTR current_polygon = render_list->poly_ptrs[poly];

            if (!_Polygon4DV1NeedToRender(current_polygon))
                continue; //当前这个多边形不满足渲染条件，跳过，检查下一个多边形

            for (int vertex = 0; vertex < 3; vertex++)
            {
                float z = current_polygon->tvlist[vertex].z; // z坐标不变因为没使用变换矩阵，所以不用把分量除以w
                current_polygon->tvlist[vertex].x = camera->view_dist * current_polygon->tvlist[vertex].x / z;
                current_polygon->tvlist[vertex].y = camera->view_dist * current_polygon->tvlist[vertex].y * camera->aspect_ratio / z;
            }
        }
    }

    //p404
    void Camera_To_Perspective_Screen_RENDERLIST4DV1(RENDERLIST4DV1_PTR render_list, CAM4DV1_PTR camera)
    {
        // 将render list中的每个多边形，从透视坐标变换为屏幕坐标。
        // 这里假设已经对render list中的多边形执行过归一化变换了，结果存储在tvlist[]成员中。
        for (int poly = 0; poly < render_list->num_polys; poly++)
        {
            // “当前多边形” 拿到当前多边形
            POLYF4DV1_PTR current_polygon = render_list->poly_ptrs[poly];

            // 当且仅当“当前多边形”没被裁剪掉，没被剔除掉，处于活动状态，且没有背向摄像机，才执行
            // 渲染，否则跳过到下一个
            if (!_Polygon4DV1NeedToRender(current_polygon))
                continue;

            float alpha = 0.5f * camera->viewport_width - 0.5f;
            float beta = 0.5f * camera->viewport_height - 0.5f;

            // 需要渲染当前多边形，则对其进行变换
            for (int vertex = 0; vertex < 3; vertex++)
            {
                // 顶点的透视坐标已经是归一化的了，取值范围是-1到1，对坐标进行缩放，并反转y轴
                float z = current_polygon->tvlist[vertex].z;

                // 根据相机的视口高宽对顶点进行变换。在此变换中z保持不变
                current_polygon->tvlist[vertex].x = camera->view_dist * current_polygon->tvlist[vertex].x / z;
                current_polygon->tvlist[vertex].y = camera->view_dist * current_polygon->tvlist[vertex].y / z;

                // 因为本函数没使用矩阵，所以不需要对xy进行除以w的操作
                // x,y的取值范围如下：
                // x:(-viewport_width/2 -> viewport_width/2)
                // y:(-viewport_height/2 -> viewport_height/2)
                // 接下来需要对坐标进行平移并翻转y轴，这是因为屏幕坐标的原点和轴朝向和投影空间的有所不同
                current_polygon->tvlist[vertex].x = current_polygon->tvlist[vertex].x + alpha;
                current_polygon->tvlist[vertex].y = -current_polygon->tvlist[vertex].y + beta;
            }
        }
    }

    void Perspective_To_Screen_RENDERLIST4DV1(RENDERLIST4DV1_PTR render_list, CAM4DV1_PTR camera)
    {
        for (int poly = 0; poly < render_list->num_polys; poly++)
        {
            POLYF4DV1_PTR current_polygon = render_list->poly_ptrs[poly];

            if (!_Polygon4DV1NeedToRender(current_polygon))
                continue; //当前这个多边形不满足渲染条件，跳过，检查下一个多边形

            float alpha = (0.5f * camera->viewport_width - 0.5f);
            float beta = (0.5f * camera->viewport_height - 0.5f);

            for (int vertex = 0; vertex < 3; vertex++)
            {
                // 因为透视坐标基于归一化的裁剪空间（投影空间）中了，取值范围是-1到1，所以在这里对坐标进行缩放，并反转y轴
                current_polygon->tvlist[vertex].x = alpha + alpha * current_polygon->tvlist[vertex].x;
                current_polygon->tvlist[vertex].y = beta - beta * current_polygon->tvlist[vertex].y;
            }
        }
    }

    void Draw_RENDERLIST4DV1_Wire16(RENDERLIST4DV1_PTR render_list, uint8_t* video_buffer, int pitch)
    {
        for (int poly = 0; poly < render_list->num_polys; poly++)
        {
            POLYF4DV1_PTR current_polygon = render_list->poly_ptrs[poly];

            if (!_Polygon4DV1NeedToRender(current_polygon))
                continue; //当前这个多边形不满足渲染条件，跳过，检查下一个多边形

            // 调用画线函数，绘制多边形的每一条边。函数内部会做裁剪，在屏幕外或者窗口外的多边形都会被剔除掉
            Draw_Clip_Line16(
                static_cast<int>(current_polygon->tvlist[0].x),
                static_cast<int>(current_polygon->tvlist[0].y),
                static_cast<int>(current_polygon->tvlist[1].x),
                static_cast<int>(current_polygon->tvlist[1].y),
                current_polygon->color,video_buffer, pitch);

            Draw_Clip_Line16(
                static_cast<int>(current_polygon->tvlist[1].x),
                static_cast<int>(current_polygon->tvlist[1].y),
                static_cast<int>(current_polygon->tvlist[2].x),
                static_cast<int>(current_polygon->tvlist[2].y),
                current_polygon->color,video_buffer, pitch);

            Draw_Clip_Line16(
                static_cast<int>(current_polygon->tvlist[2].x),
                static_cast<int>(current_polygon->tvlist[2].y),
                static_cast<int>(current_polygon->tvlist[0].x),
                static_cast<int>(current_polygon->tvlist[0].y),
                current_polygon->color,video_buffer, pitch);
#ifdef DEBUG_ON
            debug_polys_rendered_per_frame++;
#endif
        } // end for poly

    } // end Draw_RENDERLIST4DV1_Wire

    int Insert_POLY4DV1_RENDERLIST4DV1(RENDERLIST4DV1_PTR rend_list,POLY4DV1_PTR poly)
    {
        // converts the sent POLY4DV1 into a FACE4DV1 and inserts it
        // into the render list

        // step 0: are we full?
        if (rend_list->num_polys >= RENDERLIST4DV1_MAX_POLYS)
            return(0);

        // step 1: copy polygon into next opening in polygon render list

        // point pointer to polygon structure
        rend_list->poly_ptrs[rend_list->num_polys] = &rend_list->poly_data[rend_list->num_polys];

        // copy fields
        rend_list->poly_data[rend_list->num_polys].state = poly->state;
        rend_list->poly_data[rend_list->num_polys].attr = poly->attr;
        rend_list->poly_data[rend_list->num_polys].color = poly->color;

        // now copy vertices, be careful! later put a loop, but for now
        // know there are 3 vertices always!
        VECTOR4D_COPY(&rend_list->poly_data[rend_list->num_polys].tvlist[0],
            &poly->vlist[poly->vert[0]]);

        VECTOR4D_COPY(&rend_list->poly_data[rend_list->num_polys].tvlist[1],
            &poly->vlist[poly->vert[1]]);

        VECTOR4D_COPY(&rend_list->poly_data[rend_list->num_polys].tvlist[2],
            &poly->vlist[poly->vert[2]]);

        // and copy into local vertices too
        VECTOR4D_COPY(&rend_list->poly_data[rend_list->num_polys].vlist[0],
            &poly->vlist[poly->vert[0]]);

        VECTOR4D_COPY(&rend_list->poly_data[rend_list->num_polys].vlist[1],
            &poly->vlist[poly->vert[1]]);

        VECTOR4D_COPY(&rend_list->poly_data[rend_list->num_polys].vlist[2],
            &poly->vlist[poly->vert[2]]);

        // now the polygon is loaded into the next free array position, but
        // we need to fix up the links

        // test if this is the first entry
        if (rend_list->num_polys == 0)
        {
            // set pointers to null, could loop them around though to self
            rend_list->poly_data[0].next = NULL;
            rend_list->poly_data[0].prev = NULL;
        } // end if
        else
        {
            // first set this node to point to previous node and next node (null)
            rend_list->poly_data[rend_list->num_polys].next = NULL;
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

    } // end Insert_POLY4DV1_RENDERLIST4DV1

}