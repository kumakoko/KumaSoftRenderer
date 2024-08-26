#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "fmt/format.h"
#include "ksr_model_object.h"
#include "ksr_error.h"
#include "ksr_constants.h"
#include "ksr_color.h"
#include "ksr_transform.h"
#include "ksr_shape_drawing.h"

namespace KSR
{

    static char* Get_Line_PLG(char* buffer, int maxlength, FILE* fp)
    {
        // this little helper function simply read past comments 
        // and blank lines in a PLG file and always returns full 
        // lines with something on them on nullptr if the file is empty

        int index = 0;  // general index
        int length = 0; // general length

        // enter into parsing loop
        while (1)
        {
            // read the next line
            if (!fgets(buffer, maxlength, fp))
                return(nullptr);

            // kill the whitespace
            for (length = strlen(buffer), index = 0; isspace(buffer[index]); index++);

            // test if this was a blank line or a comment
            if (index >= length || buffer[index] == '#')
                continue;

            // at this point we have a good line
            return(&buffer[index]);
        } // end while

    } // end Get_Line_PLG

    float Compute_OBJECT4DV1_Radius(OBJECT4DV1_PTR obj)
    {
        // this function computes the average and maximum radius for 
        // sent object and opdates the object data

        // reset incase there's any residue
        obj->avg_radius = 0;
        obj->max_radius = 0;

        // loop thru and compute radius
        for (int vertex = 0; vertex < obj->num_vertices; vertex++)
        {
            // update the average and maximum radius
            float dist_to_vertex =
                sqrt(obj->vlist_local[vertex].x * obj->vlist_local[vertex].x +
                    obj->vlist_local[vertex].y * obj->vlist_local[vertex].y +
                    obj->vlist_local[vertex].z * obj->vlist_local[vertex].z);

            // accumulate total radius
            obj->avg_radius += dist_to_vertex;

            // update maximum radius   
            if (dist_to_vertex > obj->max_radius)
                obj->max_radius = dist_to_vertex;

        } // end for vertex

    // finallize average radius computation
        obj->avg_radius /= obj->num_vertices;

        // return max radius
        return(obj->max_radius);

    } // end Compute_OBJECT4DV1_Radius

    bool Load_OBJECT4DV1_PLG(OBJECT4DV1_PTR obj, // pointer to object
        const char* filename,     // filename of plg file
        VECTOR4D_PTR scale,     // initial scaling factors
        VECTOR4D_PTR pos,       // initial position
        VECTOR4D_PTR rot)       // initial rotations
    {
        // this function loads a plg object in off disk, additionally
        // it allows the caller to scale, position, and rotate the object
        // to save extra calls later for non-dynamic objects

        FILE* fp;          // file pointer
        char buffer[256];  // working buffer

        char* token_string;  // pointer to actual token text, ready for parsing

        // file format review, note types at end of each description
        // # this is a comment

        // # object descriptor
        // object_name_string num_verts_int num_polys_int

        // # vertex list
        // x0_float y0_float z0_float
        // x1_float y1_float z1_float
        // x2_float y2_float z2_float
        // .
        // .
        // xn_float yn_float zn_float
        //
        // # polygon list
        // surface_description_ushort num_verts_int v0_index_int v1_index_int ..  vn_index_int
        // .
        // .
        // surface_description_ushort num_verts_int v0_index_int v1_index_int ..  vn_index_int

        // lets keep it simple and assume one element per line
        // hence we have to find the object descriptor, read it in, then the
        // vertex list and read it in, and finally the polygon list -- simple :)

        // Step 1: clear out the object and initialize it a bit
        memset(obj, 0, sizeof(OBJECT4DV1));

        // set state of object to active and visible
        obj->state = OBJECT4DV1_STATE_ACTIVE | OBJECT4DV1_STATE_VISIBLE;

        // set position of object
        obj->world_pos.x = pos->x;
        obj->world_pos.y = pos->y;
        obj->world_pos.z = pos->z;
        obj->world_pos.w = pos->w;

        // Step 2: open the file for reading
        if (!(fp = fopen(filename, "r")))
        {
            throw Error(fmt::format("Couldn't open PLG file {}", filename), __FILE__, __LINE__);
            return false;
        } // end if

     // Step 3: get the first token string which should be the object descriptor
        if (!(token_string = Get_Line_PLG(buffer, 255, fp)))
        {
            throw Error(fmt::format("PLG file error with file {} (object descriptor invalid).", filename), __FILE__, __LINE__);
            return false;
        } // end if

        //Write_Error("Object Descriptor: %s", token_string);

        // parse out the info object
        sscanf(token_string, "%s %d %d", obj->name, &obj->num_vertices, &obj->num_polys);

        // Step 4: load the vertex list
        for (int vertex = 0; vertex < obj->num_vertices; vertex++)
        {
            // get the next vertex
            if (!(token_string = Get_Line_PLG(buffer, 255, fp)))
            {
                throw Error(fmt::format("PLG file error with file {} (vertex list invalid).", filename), __FILE__, __LINE__);
                return false;
            } // end if

      // parse out vertex
            sscanf(token_string, "%f %f %f", &obj->vlist_local[vertex].x,
                &obj->vlist_local[vertex].y,
                &obj->vlist_local[vertex].z);
            obj->vlist_local[vertex].w = 1;

            // scale vertices
            obj->vlist_local[vertex].x *= scale->x;
            obj->vlist_local[vertex].y *= scale->y;
            obj->vlist_local[vertex].z *= scale->z;

            /*
            Write_Error("\nVertex %d = %f, %f, %f, %f", vertex,
                obj->vlist_local[vertex].x,
                obj->vlist_local[vertex].y,
                obj->vlist_local[vertex].z,
                obj->vlist_local[vertex].w);*/

        } // end for vertex

    // compute average and max radius
        Compute_OBJECT4DV1_Radius(obj);

        /*
        Write_Error("\nObject average radius = %f, max radius = %f",
            obj->avg_radius, obj->max_radius);*/

        int poly_surface_desc = 0; // PLG/PLX surface descriptor
        int poly_num_verts = 0; // number of vertices for current poly (always 3)
        char tmp_string[8];        // temp string to hold surface descriptor in and
        // test if it need to be converted from hex

// Step 5: load the polygon list
        for (int poly = 0; poly < obj->num_polys; poly++)
        {
            // get the next polygon descriptor
            if (!(token_string = Get_Line_PLG(buffer, 255, fp)))
            {
                //   Write_Error("PLG file error with file %s (polygon descriptor invalid).", filename);
                throw Error(fmt::format("PLG file error with file {} (polygon descriptor invalid).", filename), __FILE__, __LINE__);
                return false;
            } // end if

            //Write_Error("\nPolygon %d:", poly);

            // each vertex list MUST have 3 vertices since we made this a rule that all models
            // must be constructed of triangles
            // read in surface descriptor, number of vertices, and vertex list

            sscanf(token_string, "%s %d %d %d %d", tmp_string,
                &poly_num_verts, // should always be 3
                &obj->plist[poly].vert[0],
                &obj->plist[poly].vert[1],
                &obj->plist[poly].vert[2]);



            // since we are allowing the surface descriptor to be in hex format
            // with a leading "0x" we need to test for it
            if (tmp_string[0] == '0' && toupper(tmp_string[1]) == 'X')
                sscanf(tmp_string, "%x", &poly_surface_desc);
            else
                poly_surface_desc = atoi(tmp_string);

            // point polygon vertex list to object's vertex list
            // note that this is redundant since the polylist is contained
            // within the object in this case and its up to the user to select
            // whether the local or transformed vertex list is used when building up
            // polygon geometry, might be a better idea to set to nullptr in the context
            // of polygons that are part of an object
            obj->plist[poly].vlist = obj->vlist_local;

            /*
            Write_Error("\nSurface Desc = 0x%.4x, num_verts = %d, vert_indices [%d, %d, %d]",
                poly_surface_desc,
                poly_num_verts,
                obj->plist[poly].vert[0],
                obj->plist[poly].vert[1],
                obj->plist[poly].vert[2]);
            */

            // now we that we have the vertex list and we have entered the polygon
            // vertex index data into the polygon itself, now let's analyze the surface
            // descriptor and set the fields for the polygon based on the description

            // extract out each field of data from the surface descriptor
            // first let's get the single/double sided stuff out of the way
            if ((poly_surface_desc & PLX_2SIDED_FLAG))
            {
                SET_BIT(obj->plist[poly].attr, POLY4DV1_ATTR_2SIDED);
                // Write_Error("\n2 sided.");
            } // end if
            else
            {
                // one sided
               // Write_Error("\n1 sided.");
            } // end else

         // now let's set the color type and color
            if ((poly_surface_desc & PLX_COLOR_MODE_RGB_FLAG))
            {
                // this is an RGB 4.4.4 surface
                SET_BIT(obj->plist[poly].attr, POLY4DV1_ATTR_RGB16);

                // now extract color and copy into polygon color
                // field in proper 16-bit format 
                // 0x0RGB is the format, 4 bits per pixel 
                int red = ((poly_surface_desc & 0x0f00) >> 8);
                int green = ((poly_surface_desc & 0x00f0) >> 4);
                int blue = (poly_surface_desc & 0x000f);

                // although the data is always in 4.4.4 format, the graphics card
                // is either 5.5.5 or 5.6.5, but our virtual color system translates
                // 8.8.8 into 5.5.5 or 5.6.5 for us, but we have to first scale all
                // these 4.4.4 values into 8.8.8
                obj->plist[poly].color = RGB16Bit(red * 16, green * 16, blue * 16);
                //Write_Error("\nRGB color = [%d, %d, %d]", red, green, blue);
            } // end if
            else
            {
                // this is an 8-bit color indexed surface
                SET_BIT(obj->plist[poly].attr, POLY4DV1_ATTR_8BITCOLOR);

                // and simple extract the last 8 bits and that's the color index
                obj->plist[poly].color = (poly_surface_desc & 0x00ff);

                //  Write_Error("\n8-bit color index = %d", obj->plist[poly].color);

            } // end else

        // handle shading mode
            int shade_mode = (poly_surface_desc & PLX_SHADE_MODE_MASK);

            // set polygon shading mode
            switch (shade_mode)
            {
            case PLX_SHADE_MODE_PURE_FLAG:
            {
                SET_BIT(obj->plist[poly].attr, POLY4DV1_ATTR_SHADE_MODE_PURE);
                //Write_Error("\nShade mode = pure");
            }
            break;

            case PLX_SHADE_MODE_FLAT_FLAG:
            {
                SET_BIT(obj->plist[poly].attr, POLY4DV1_ATTR_SHADE_MODE_FLAT);
                //Write_Error("\nShade mode = flat");

            } break;

            case PLX_SHADE_MODE_GOURAUD_FLAG:
            {
                SET_BIT(obj->plist[poly].attr, POLY4DV1_ATTR_SHADE_MODE_GOURAUD);
                //Write_Error("\nShade mode = gouraud");
            } break;

            case PLX_SHADE_MODE_PHONG_FLAG:
            {
                SET_BIT(obj->plist[poly].attr, POLY4DV1_ATTR_SHADE_MODE_PHONG);
                //Write_Error("\nShade mode = phong");
            }
            break;

            default:
                break;
            } // end switch

       // finally set the polygon to active
            obj->plist[poly].state = POLY4DV1_STATE_ACTIVE;

        } // end for poly

    // close the file
        fclose(fp);

        // return success
        return true;

    } // end Load_OBJECT4DV1_PLG

    void Reset_OBJECT4DV1(OBJECT4DV1_PTR obj)
    {
        // this function resets the sent object and redies it for 
        // transformations, basically just resets the culled, clipped and
        // backface flags, but here's where you would add stuff
        // to ready any object for the pipeline
        // the object is valid, let's rip it apart polygon by polygon

        // reset object's culled flag
        RESET_BIT(obj->state, OBJECT4DV1_STATE_CULLED);

        // now the clipped and backface flags for the polygons 
        for (int poly = 0; poly < obj->num_polys; poly++)
        {
            // acquire polygon
            POLY4DV1_PTR curr_poly = &obj->plist[poly];

            // first is this polygon even visible?
            if (!(curr_poly->state & POLY4DV1_STATE_ACTIVE))
                continue; // move onto next poly

            // reset clipped and backface flags
            RESET_BIT(curr_poly->state, POLY4DV1_STATE_CLIPPED);
            RESET_BIT(curr_poly->state, POLY4DV1_STATE_BACKFACE);

        }
    }

    void Transform_OBJECT4DV1(OBJECT4DV1_PTR obj, MATRIX4X4_PTR mt, TransformControlFlag coord_select, bool transform_basis)
    {
        if (TRANSFORM_LOCAL_ONLY == coord_select)
        {
            // 遍历多边形中的三个顶点，对这些存储在vlist数组成员变量的顶点坐标，使用mt矩阵，进行变换，然后再存回到vlist数组中去
            for (int vertex = 0; vertex < obj->num_vertices; vertex++)
            {
                POINT4D presult;
                Mat_Mul_VECTOR4D_4X4(&obj->vlist_local[vertex], mt, &presult);
                VECTOR4D_COPY(&obj->vlist_local[vertex], &presult);
            }
        }
        else if (TRANSFORM_TRANS_ONLY == coord_select)
        {
            // 遍历多边形中的三个顶点，对这些存储在vlist_trans数组成员变量的顶点坐标，使用mt矩阵，进行变换，然后再存回到vlist_trans数组中去
            for (int vertex = 0; vertex < obj->num_vertices; vertex++)
            {
                POINT4D presult;
                Mat_Mul_VECTOR4D_4X4(&obj->vlist_trans[vertex], mt, &presult);
                VECTOR4D_COPY(&obj->vlist_trans[vertex], &presult);
            }
        }
        else if (TRANSFORM_LOCAL_TO_TRANS == coord_select)
        {
            // 遍历多边形中的三个顶点，对这些存储在vlist_local数组成员变量的顶点坐标，使用mt矩阵，进行变换，然后再存回到vlist_trans数组中去
            for (int vertex = 0; vertex < obj->num_vertices; vertex++)
            {
                Mat_Mul_VECTOR4D_4X4(&obj->vlist_local[vertex], mt, &obj->vlist_trans[vertex]);
            }
        }

        // 最后必须要检查下是否对model object自身的朝向向量进行变换，不变换的话，一旦model
        // object发生了旋转，其朝向向量就不再有效了
        if (transform_basis)
        {
            VECTOR4D vresult;

            // 根据算出来的旋转矩阵，把自身的x轴也旋转一下
            Mat_Mul_VECTOR4D_4X4(&obj->ux, mt, &vresult);
            VECTOR4D_COPY(&obj->ux, &vresult);

            // 把自身的y轴也旋转一下
            Mat_Mul_VECTOR4D_4X4(&obj->uy, mt, &vresult);
            VECTOR4D_COPY(&obj->uy, &vresult);

            // 把自身的z轴也旋转
            Mat_Mul_VECTOR4D_4X4(&obj->uz, mt, &vresult);
            VECTOR4D_COPY(&obj->uz, &vresult);
        }
    }

    // 可参见Model_To_World_RENDERLIST4DV1函数
    void Model_To_World_OBJECT4DV1(OBJECT4DV1_PTR obj, int coord_select)
    {
        if (coord_select == TRANSFORM_LOCAL_TO_TRANS)
        {
            for (int vertex = 0; vertex < obj->num_vertices; vertex++)
            {
                VECTOR4D_Add(&obj->vlist_local[vertex], &obj->world_pos, &obj->vlist_trans[vertex]);
            }
        }
        else
        {
            // TRANSFORM_TRANS_ONLY
            for (int vertex = 0; vertex < obj->num_vertices; vertex++)
            {
                VECTOR4D_Add(&obj->vlist_trans[vertex], &obj->world_pos, &obj->vlist_trans[vertex]);
            }
        }
    }

    void Remove_Backfaces_OBJECT4DV1(OBJECT4DV1_PTR obj, CAM4DV1_PTR cam)
    {
        // 如果本model object已经被拣选则直接返回
        if (obj->state & OBJECT4DV1_STATE_CULLED)
            return;

        // 单独处理每一个网格中的多边形
        for (int poly = 0; poly < obj->num_polys; poly++)
        {
            POLY4DV1_PTR curr_poly = &obj->plist[poly];

            // 如果本多边形处于不活跃状态，或者处于被裁剪状态，或者它是双面的，或者它是背面的
            if (!(curr_poly->state & POLY4DV1_STATE_ACTIVE) ||
                (curr_poly->state & POLY4DV1_STATE_CLIPPED) ||
                (curr_poly->attr & POLY4DV1_ATTR_2SIDED) ||
                (curr_poly->state & POLY4DV1_STATE_BACKFACE))
                continue; // move onto next poly

            // 获取顶点列表中的顶点索引，因为多边形结构体里面并没有直接包含了构成该多边形的顶点
            // 而是由多边形顶点索引组成。所以首先得取出顶点索引，然后在顶点列表中取得
            int vindex_0 = curr_poly->vert[0];
            int vindex_1 = curr_poly->vert[1];
            int vindex_2 = curr_poly->vert[2];

            // 使用vlist_trans列表进行背面消除，因为背面消除操作只能在顶点被转换为世界坐标后才能进行
            // 需要计算多边形的面法线，顶点是按顺时针方向排列的，u=p0->p1 v=p0->p2,n=uxv 即向量u叉乘v

            VECTOR4D u, v, n;

            // 计算u和v向量
            VECTOR4D_Build(&obj->vlist_trans[vindex_0], &obj->vlist_trans[vindex_1], &u);
            VECTOR4D_Build(&obj->vlist_trans[vindex_0], &obj->vlist_trans[vindex_2], &v);

            // 计算叉积
            VECTOR4D_Cross(&u, &v, &n);

            // 创建一个方向向量view，起始点被观察的那个顶点，终点是摄像机位置点
            VECTOR4D view;
            VECTOR4D_Build(&obj->vlist_trans[vindex_0], &cam->pos, &view);

            // 计算面法线n和方向向量view的点积，小于0者表示多边形背向摄像机，
            // 刚好等于0表示法线和view刚好垂直，大于0表示正面朝向摄像机，可见
            float dp = VECTOR4D_Dot(&n, &view);

            if (dp <= 0.0f)
                SET_BIT(curr_poly->state, POLY4DV1_STATE_BACKFACE);
        }
    }

    // 可参见World_To_Camera_RENDERLIST4DV1函数
    void World_To_Camera_OBJECT4DV1(OBJECT4DV1_PTR obj, CAM4DV1_PTR cam)
    {
        for (int vertex = 0; vertex < obj->num_vertices; vertex++)
        {
            POINT4D presult;
            Mat_Mul_VECTOR4D_4X4(&obj->vlist_trans[vertex], &cam->mcam, &presult);
            VECTOR4D_COPY(&obj->vlist_trans[vertex], &presult);
        }
    }

    // 可参见Camera_To_Perspective_RENDERLIST4DV1函数
    void Camera_To_Perspective_OBJECT4DV1(OBJECT4DV1_PTR obj, CAM4DV1_PTR cam)
    {
        for (int vertex = 0; vertex < obj->num_vertices; vertex++)
        {
            float z = obj->vlist_trans[vertex].z;
            obj->vlist_trans[vertex].x = cam->view_dist * obj->vlist_trans[vertex].x / z;
            obj->vlist_trans[vertex].y = cam->view_dist * obj->vlist_trans[vertex].y * cam->aspect_ratio / z;
        }
    }

    // 可参见Camera_To_Perspective_Screen_RENDERLIST4DV1函数
    void Camera_To_Perspective_Screen_OBJECT4DV1(OBJECT4DV1_PTR obj, CAM4DV1_PTR cam)
    {
        float alpha = 0.5f * cam->viewport_width - 0.5f;
        float beta = 0.5f * cam->viewport_height - 0.5f;

        for (int vertex = 0; vertex < obj->num_vertices; vertex++)
        {
            float z = obj->vlist_trans[vertex].z;
            obj->vlist_trans[vertex].x = cam->view_dist * obj->vlist_trans[vertex].x / z;
            obj->vlist_trans[vertex].y = cam->view_dist * obj->vlist_trans[vertex].y / z;
            obj->vlist_trans[vertex].x = obj->vlist_trans[vertex].x + alpha;
            obj->vlist_trans[vertex].y = -obj->vlist_trans[vertex].y + beta;
        }
    }

    // 可参见Perspective_To_Screen_RENDERLIST4DV1函数
    void Perspective_To_Screen_OBJECT4DV1(OBJECT4DV1_PTR obj, CAM4DV1_PTR cam)
    {
        float alpha = 0.5f * cam->viewport_width - 0.5f;
        float beta = 0.5f * cam->viewport_height - 0.5f;

        for (int vertex = 0; vertex < obj->num_vertices; vertex++)
        {
            obj->vlist_trans[vertex].x = alpha + alpha * obj->vlist_trans[vertex].x;
            obj->vlist_trans[vertex].y = beta - beta * obj->vlist_trans[vertex].y;
        }
    }

    void Draw_OBJECT4DV1_Wire16(OBJECT4DV1_PTR obj, uint8_t* video_buffer, int pitch)
    {
        for (int poly = 0; poly < obj->num_polys; poly++)
        {
            if (!(obj->plist[poly].state & POLY4DV1_STATE_ACTIVE) ||
                (obj->plist[poly].state & POLY4DV1_STATE_CLIPPED) ||
                (obj->plist[poly].state & POLY4DV1_STATE_BACKFACE))
                continue;

            int vindex_0 = obj->plist[poly].vert[0];
            int vindex_1 = obj->plist[poly].vert[1];
            int vindex_2 = obj->plist[poly].vert[2];

            Draw_Clip_Line16(
                static_cast<int>(obj->vlist_trans[vindex_0].x),
                static_cast<int>(obj->vlist_trans[vindex_0].y),
                static_cast<int>(obj->vlist_trans[vindex_1].x),
                static_cast<int>(obj->vlist_trans[vindex_1].y),
                obj->plist[poly].color, video_buffer, pitch);

            Draw_Clip_Line16(
                static_cast<int>(obj->vlist_trans[vindex_1].x),
                static_cast<int>(obj->vlist_trans[vindex_1].y),
                static_cast<int>(obj->vlist_trans[vindex_2].x),
                static_cast<int>(obj->vlist_trans[vindex_2].y),
                obj->plist[poly].color, video_buffer, pitch);

            Draw_Clip_Line16(
                static_cast<int>(obj->vlist_trans[vindex_2].x),
                static_cast<int>(obj->vlist_trans[vindex_2].y),
                static_cast<int>(obj->vlist_trans[vindex_0].x),
                static_cast<int>(obj->vlist_trans[vindex_0].y),
                obj->plist[poly].color, video_buffer, pitch);

#ifdef DEBUG_ON
            debug_polys_rendered_per_frame++;
#endif
        }
    }

    bool Cull_OBJECT4DV1(OBJECT4DV1_PTR obj, CAM4DV1_PTR cam, int cull_flags)
    {
        // 首先将物体的包围球球心变换到观察空间
        POINT4D sphere_pos;
        Mat_Mul_VECTOR4D_4X4(&obj->world_pos, &cam->mcam, &sphere_pos);

        // 根据给定的剔除标志对物体执行剔除操作
        if (cull_flags & CULL_OBJECT_Z_PLANE)
        {
            // 根据摄像机的远近裁剪面进行裁剪，如果该model object的球心坐标z分量，加上
            // 最大包围半径，都还小于近裁剪面。或者球心坐标z分量减去最大包围半径都大于远
            // 裁剪面。就表示肯定整个model都超出视截体。这两种条件一旦符合，立即剔除
            if (sphere_pos.z - obj->max_radius > cam->far_clip_z ||
                sphere_pos.z + obj->max_radius < cam->near_clip_z)
            {
                SET_BIT(obj->state, OBJECT4DV1_STATE_CULLED); // 设置本object为被剔除
                return true;
            }
        }

        if (cull_flags & CULL_OBJECT_X_PLANE)
        {
            // 使用视截体右裁剪面和左裁剪面，检测object包围球上最左边和最右边的点
            float z_test = 0.5f * cam->viewplane_width * sphere_pos.z / cam->view_dist;

            if (sphere_pos.x - obj->max_radius > z_test || // 最右边
                sphere_pos.x + obj->max_radius < -z_test)   // 最左边
            {
                SET_BIT(obj->state, OBJECT4DV1_STATE_CULLED);
                return true;
            }
        }

        if (cull_flags & CULL_OBJECT_Y_PLANE)
        {
            // 使用视截体上裁剪面和下裁剪面，检测object包围球上最下边和最上边的点
            float z_test = 0.5f * cam->viewplane_height * sphere_pos.z / cam->view_dist;

            if (sphere_pos.y - obj->max_radius > z_test || // 包围球最下边的点，是不是都超过视截体上裁剪面
                sphere_pos.y + obj->max_radius < -z_test)   // 包围球最上边的点，是不是都超过视截体下裁剪面
            {
                SET_BIT(obj->state, OBJECT4DV1_STATE_CULLED);
                return true;
            }
        }

        return false;
    }

    bool OBJECT4DV2_TYP::Set_Frame(int frame)
    {
        if (nullptr == this)
            return false;

        // 检查本model objec是不是多帧的，不是的话就不用设置
        if (!(this->attr & OBJECT4DV2_ATTR_MULTI_FRAME))
            return false;

        // 将指针指向当前帧数据，检查参数frame是否越界
        if (frame < 0)
        {
            frame = 0;
        }
        else
        {
            if (frame >= this->num_frames)
                frame = this->num_frames - 1;
        }

        // 设置当前帧
        this->curr_frame = frame;

        // 让指针指向顶点列表中相对应的帧
        this->vlist_local = &(this->head_vlist_local[frame * this->num_vertices]);
        this->vlist_trans = &(this->head_vlist_trans[frame * this->num_vertices]);
        return true;

    } // end Set_Frame
}