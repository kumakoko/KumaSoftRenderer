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

        } // end for poly

    } // end Reset_OBJECT4DV1

    void Transform_OBJECT4DV1(OBJECT4DV1_PTR obj, MATRIX4X4_PTR mt, int coord_select, int transform_basis) // flags if vector orientation
        // should be transformed too
    {
        // this function simply transforms all of the vertices in the local or trans
        // array by the sent matrix

        // what coordinates should be transformed?
        switch (coord_select)
        {
        case TRANSFORM_LOCAL_ONLY:
        {
            // transform each local/model vertex of the object mesh in place
            for (int vertex = 0; vertex < obj->num_vertices; vertex++)
            {
                POINT4D presult; // hold result of each transformation

                // transform point
                Mat_Mul_VECTOR4D_4X4(&obj->vlist_local[vertex], mt, &presult);

                // store result back
                VECTOR4D_COPY(&obj->vlist_local[vertex], &presult);
            } // end for index
        } break;

        case TRANSFORM_TRANS_ONLY:
        {
            // transform each "transformed" vertex of the object mesh in place
            // remember, the idea of the vlist_trans[] array is to accumulate
            // transformations
            for (int vertex = 0; vertex < obj->num_vertices; vertex++)
            {
                POINT4D presult; // hold result of each transformation

                // transform point
                Mat_Mul_VECTOR4D_4X4(&obj->vlist_trans[vertex], mt, &presult);

                // store result back
                VECTOR4D_COPY(&obj->vlist_trans[vertex], &presult);
            } // end for index

        } break;

        case TRANSFORM_LOCAL_TO_TRANS:
        {
            // transform each local/model vertex of the object mesh and store result
            // in "transformed" vertex list
            for (int vertex = 0; vertex < obj->num_vertices; vertex++)
            {
                POINT4D presult; // hold result of each transformation

                // transform point
                Mat_Mul_VECTOR4D_4X4(&obj->vlist_local[vertex], mt, &obj->vlist_trans[vertex]);

            } // end for index
        } break;

        default: break;

        } // end switch

        // finally, test if transform should be applied to orientation basis
        // hopefully this is a rotation, otherwise the basis will get corrupted
        if (transform_basis)
        {
            // now rotate orientation basis for object
            VECTOR4D vresult; // use to rotate each orientation vector axis

            // rotate ux of basis
            Mat_Mul_VECTOR4D_4X4(&obj->ux, mt, &vresult);
            VECTOR4D_COPY(&obj->ux, &vresult);

            // rotate uy of basis
            Mat_Mul_VECTOR4D_4X4(&obj->uy, mt, &vresult);
            VECTOR4D_COPY(&obj->uy, &vresult);

            // rotate uz of basis
            Mat_Mul_VECTOR4D_4X4(&obj->uz, mt, &vresult);
            VECTOR4D_COPY(&obj->uz, &vresult);
        } // end if

    } // end Transform_OBJECT4DV1

    void Model_To_World_OBJECT4DV1(OBJECT4DV1_PTR obj, int coord_select)
    {
        // NOTE: Not matrix based
        // this function converts the local model coordinates of the
        // sent object into world coordinates, the results are stored
        // in the transformed vertex list (vlist_trans) within the object

        // interate thru vertex list and transform all the model/local 
        // coords to world coords by translating the vertex list by
        // the amount world_pos and storing the results in vlist_trans[]

        if (coord_select == TRANSFORM_LOCAL_TO_TRANS)
        {
            for (int vertex = 0; vertex < obj->num_vertices; vertex++)
            {
                // translate vertex
                VECTOR4D_Add(&obj->vlist_local[vertex], &obj->world_pos, &obj->vlist_trans[vertex]);
            } // end for vertex
        } // end if local
        else
        { // TRANSFORM_TRANS_ONLY
            for (int vertex = 0; vertex < obj->num_vertices; vertex++)
            {
                // translate vertex
                VECTOR4D_Add(&obj->vlist_trans[vertex], &obj->world_pos, &obj->vlist_trans[vertex]);
            } // end for vertex
        } // end else trans

    } // end Model_To_World_OBJECT4DV1

    void Remove_Backfaces_OBJECT4DV1(OBJECT4DV1_PTR obj, CAM4DV1_PTR cam)
    {
        // NOTE: this is not a matrix based function
        // this function removes the backfaces from an object's
        // polygon mesh, the function does this based on the vertex
        // data in vlist_trans along with the camera position (only)
        // note that only the backface state is set in each polygon

        // test if the object is culled
        if (obj->state & OBJECT4DV1_STATE_CULLED)
            return;

        // process each poly in mesh
        for (int poly = 0; poly < obj->num_polys; poly++)
        {
            // acquire polygon
            POLY4DV1_PTR curr_poly = &obj->plist[poly];

            // is this polygon valid?
            // test this polygon if and only if it's not clipped, not culled,
            // active, and visible and not 2 sided. Note we test for backface in the event that
            // a previous call might have already determined this, so why work
            // harder!
            if (!(curr_poly->state & POLY4DV1_STATE_ACTIVE) ||
                (curr_poly->state & POLY4DV1_STATE_CLIPPED) ||
                (curr_poly->attr & POLY4DV1_ATTR_2SIDED) ||
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

            // we need to compute the normal of this polygon face, and recall
            // that the vertices are in cw order, u = p0->p1, v=p0->p2, n=uxv
            VECTOR4D u, v, n;

            // build u, v
            VECTOR4D_Build(&obj->vlist_trans[vindex_0], &obj->vlist_trans[vindex_1], &u);
            VECTOR4D_Build(&obj->vlist_trans[vindex_0], &obj->vlist_trans[vindex_2], &v);

            // compute cross product
            VECTOR4D_Cross(&u, &v, &n);

            // now create eye vector to viewpoint
            VECTOR4D view;
            VECTOR4D_Build(&obj->vlist_trans[vindex_0], &cam->pos, &view);

            // and finally, compute the dot product
            float dp = VECTOR4D_Dot(&n, &view);

            // if the sign is > 0 then visible, 0 = scathing, < 0 invisible
            if (dp <= 0.0f)
                SET_BIT(curr_poly->state, POLY4DV1_STATE_BACKFACE);

        } // end for poly

    } // end Remove_Backfaces_OBJECT4DV1

    void World_To_Camera_OBJECT4DV1(OBJECT4DV1_PTR obj, CAM4DV1_PTR cam)
    {
        // NOTE: this is a matrix based function
        // this function transforms the world coordinates of an object
        // into camera coordinates, based on the sent camera matrix
        // but it totally disregards the polygons themselves,
        // it only works on the vertices in the vlist_trans[] list
        // this is one way to do it, you might instead transform
        // the global list of polygons in the render list since you 
        // are guaranteed that those polys represent geometry that 
        // has passed thru backfaces culling (if any)

        // transform each vertex in the object to camera coordinates
        // assumes the object has already been transformed to world
        // coordinates and the result is in vlist_trans[]
        for (int vertex = 0; vertex < obj->num_vertices; vertex++)
        {
            // transform the vertex by the mcam matrix within the camera
            // it better be valid!
            POINT4D presult; // hold result of each transformation

            // transform point
            Mat_Mul_VECTOR4D_4X4(&obj->vlist_trans[vertex], &cam->mcam, &presult);

            // store result back
            VECTOR4D_COPY(&obj->vlist_trans[vertex], &presult);
        } // end for vertex

    } // end World_To_Camera_OBJECT4DV1

    void Camera_To_Perspective_OBJECT4DV1(OBJECT4DV1_PTR obj, CAM4DV1_PTR cam)
    {
        // NOTE: this is not a matrix based function
        // this function transforms the camera coordinates of an object
        // into perspective coordinates, based on the 
        // sent camera object, but it totally disregards the polygons themselves,
        // it only works on the vertices in the vlist_trans[] list
        // this is one way to do it, you might instead transform
        // the global list of polygons in the render list since you 
        // are guaranteed that those polys represent geometry that 
        // has passed thru backfaces culling (if any)
        // finally this function is really for experimental reasons only
        // you would probably never let an object stay intact this far down
        // the pipeline, since it's probably that there's only a single polygon
        // that is visible! But this function has to transform the whole mesh!

        // transform each vertex in the object to perspective coordinates
        // assumes the object has already been transformed to camera
        // coordinates and the result is in vlist_trans[]
        for (int vertex = 0; vertex < obj->num_vertices; vertex++)
        {
            float z = obj->vlist_trans[vertex].z;

            // transform the vertex by the view parameters in the camera
            obj->vlist_trans[vertex].x = cam->view_dist * obj->vlist_trans[vertex].x / z;
            obj->vlist_trans[vertex].y = cam->view_dist * obj->vlist_trans[vertex].y * cam->aspect_ratio / z;
            // z = z, so no change

            // not that we are NOT dividing by the homogenous w coordinate since
            // we are not using a matrix operation for this version of the function 

        } // end for vertex

    } // end Camera_To_Perspective_OBJECT4DV1

    void Camera_To_Perspective_Screen_OBJECT4DV1(OBJECT4DV1_PTR obj, CAM4DV1_PTR cam)
    {
        // NOTE: this is not a matrix based function
        // this function transforms the camera coordinates of an object
        // into Screen scaled perspective coordinates, based on the 
        // sent camera object, that is, view_dist_h and view_dist_v 
        // should be set to cause the desired (width X height)
        // projection of the vertices, but the function totally 
        // disregards the polygons themselves,
        // it only works on the vertices in the vlist_trans[] list
        // this is one way to do it, you might instead transform
        // the global list of polygons in the render list since you 
        // are guaranteed that those polys represent geometry that 
        // has passed thru backfaces culling (if any)
        // finally this function is really for experimental reasons only
        // you would probably never let an object stay intact this far down
        // the pipeline, since it's probably that there's only a single polygon
        // that is visible! But this function has to transform the whole mesh!
        // finally, the function also inverts the y axis, so the coordinates
        // generated from this function ARE screen coordinates and ready for
        // rendering

        float alpha = (0.5 * cam->viewport_width - 0.5);
        float beta = (0.5 * cam->viewport_height - 0.5);

        // transform each vertex in the object to perspective screen coordinates
        // assumes the object has already been transformed to camera
        // coordinates and the result is in vlist_trans[]
        for (int vertex = 0; vertex < obj->num_vertices; vertex++)
        {
            float z = obj->vlist_trans[vertex].z;

            // transform the vertex by the view parameters in the camera
            obj->vlist_trans[vertex].x = cam->view_dist * obj->vlist_trans[vertex].x / z;
            obj->vlist_trans[vertex].y = cam->view_dist * obj->vlist_trans[vertex].y / z;
            // z = z, so no change

            // not that we are NOT dividing by the homogenous w coordinate since
            // we are not using a matrix operation for this version of the function 

            // now the coordinates are in the range x:(-viewport_width/2 to viewport_width/2)
            // and y:(-viewport_height/2 to viewport_height/2), thus we need a translation and
            // since the y-axis is inverted, we need to invert y to complete the screen 
            // transform:
            obj->vlist_trans[vertex].x = obj->vlist_trans[vertex].x + alpha;
            obj->vlist_trans[vertex].y = -obj->vlist_trans[vertex].y + beta;

        } // end for vertex

    } // end Camera_To_Perspective_Screen_OBJECT4DV1

    void Perspective_To_Screen_OBJECT4DV1(OBJECT4DV1_PTR obj, CAM4DV1_PTR cam)
    {
        // NOTE: this is not a matrix based function
        // this function transforms the perspective coordinates of an object
        // into screen coordinates, based on the sent viewport info
        // but it totally disregards the polygons themselves,
        // it only works on the vertices in the vlist_trans[] list
        // this is one way to do it, you might instead transform
        // the global list of polygons in the render list since you 
        // are guaranteed that those polys represent geometry that 
        // has passed thru backfaces culling (if any)
        // finally this function is really for experimental reasons only
        // you would probably never let an object stay intact this far down
        // the pipeline, since it's probably that there's only a single polygon
        // that is visible! But this function has to transform the whole mesh!
        // this function would be called after a perspective
        // projection was performed on the object

        // transform each vertex in the object to screen coordinates
        // assumes the object has already been transformed to perspective
        // coordinates and the result is in vlist_trans[]

        float alpha = (0.5 * cam->viewport_width - 0.5);
        float beta = (0.5 * cam->viewport_height - 0.5);

        for (int vertex = 0; vertex < obj->num_vertices; vertex++)
        {
            // assumes the vertex is in perspective normalized coords from -1 to 1
            // on each axis, simple scale them to viewport and invert y axis and project
            // to screen

            // transform the vertex by the view parameters in the camera
            obj->vlist_trans[vertex].x = alpha + alpha * obj->vlist_trans[vertex].x;
            obj->vlist_trans[vertex].y = beta - beta * obj->vlist_trans[vertex].y;

        } // end for vertex

    } // end Perspective_To_Screen_OBJECT4DV1

    void Draw_OBJECT4DV1_Wire16(OBJECT4DV1_PTR obj,uint8_t* video_buffer, int lpitch)
    {
        // this function renders an object to the screen in wireframe, 
        // 16 bit mode, it has no regard at all about hidden surface removal, 
        // etc. the function only exists as an easy way to render an object 
        // without converting it into polygons, the function assumes all 
        // coordinates are screen coordinates, but will perform 2D clipping

        // iterate thru the poly list of the object and simply draw
        // each polygon
        for (int poly = 0; poly < obj->num_polys; poly++)
        {
            // render this polygon if and only if it's not clipped, not culled,
            // active, and visible, note however the concecpt of "backface" is 
            // irrelevant in a wire frame engine though
            if (!(obj->plist[poly].state & POLY4DV1_STATE_ACTIVE) ||
                (obj->plist[poly].state & POLY4DV1_STATE_CLIPPED) ||
                (obj->plist[poly].state & POLY4DV1_STATE_BACKFACE))
                continue; // move onto next poly

            // extract vertex indices into master list, rember the polygons are 
            // NOT self contained, but based on the vertex list stored in the object
            // itself
            int vindex_0 = obj->plist[poly].vert[0];
            int vindex_1 = obj->plist[poly].vert[1];
            int vindex_2 = obj->plist[poly].vert[2];

            // draw the lines now
            Draw_Clip_Line16(obj->vlist_trans[vindex_0].x, obj->vlist_trans[vindex_0].y,
                obj->vlist_trans[vindex_1].x, obj->vlist_trans[vindex_1].y,
                obj->plist[poly].color,
                video_buffer, lpitch);

            Draw_Clip_Line16(obj->vlist_trans[vindex_1].x, obj->vlist_trans[vindex_1].y,
                obj->vlist_trans[vindex_2].x, obj->vlist_trans[vindex_2].y,
                obj->plist[poly].color,
                video_buffer, lpitch);

            Draw_Clip_Line16(obj->vlist_trans[vindex_2].x, obj->vlist_trans[vindex_2].y,
                obj->vlist_trans[vindex_0].x, obj->vlist_trans[vindex_0].y,
                obj->plist[poly].color,
                video_buffer, lpitch);

            // track rendering stats
#ifdef DEBUG_ON
            debug_polys_rendered_per_frame++;
#endif


        } // end for poly

    } // end Draw_OBJECT4DV1_Wire16

    int Cull_OBJECT4DV1(OBJECT4DV1_PTR obj,  // object to cull
        CAM4DV1_PTR cam,     // camera to cull relative to
        int cull_flags)     // clipping planes to consider
    {
        // NOTE: is matrix based
        // this function culls an entire object from the viewing
        // frustrum by using the sent camera information and object
        // the cull_flags determine what axes culling should take place
        // x, y, z or all which is controlled by ORing the flags
        // together
        // if the object is culled its state is modified thats all
        // this function assumes that both the camera and the object
        // are valid!

        // step 1: transform the center of the object's bounding
        // sphere into camera space

        POINT4D sphere_pos; // hold result of transforming center of bounding sphere

        // transform point
        Mat_Mul_VECTOR4D_4X4(&obj->world_pos, &cam->mcam, &sphere_pos);

        // step 2:  based on culling flags remove the object
        if (cull_flags & CULL_OBJECT_Z_PLANE)
        {
            // cull only based on z clipping planes

            // test far plane
            if (((sphere_pos.z - obj->max_radius) > cam->far_clip_z) ||
                ((sphere_pos.z + obj->max_radius) < cam->near_clip_z))
            {
                SET_BIT(obj->state, OBJECT4DV1_STATE_CULLED);
                return(1);
            } // end if

        } // end if

        if (cull_flags & CULL_OBJECT_X_PLANE)
        {
            // cull only based on x clipping planes
            // we could use plane equations, but simple similar triangles
            // is easier since this is really a 2D problem
            // if the view volume is 90 degrees the the problem is trivial
            // buts lets assume its not

            // test the the right and left clipping planes against the leftmost and rightmost
            // points of the bounding sphere
            float z_test = (0.5) * cam->viewplane_width * sphere_pos.z / cam->view_dist;

            if (((sphere_pos.x - obj->max_radius) > z_test) || // right side
                ((sphere_pos.x + obj->max_radius) < -z_test))  // left side, note sign change
            {
                SET_BIT(obj->state, OBJECT4DV1_STATE_CULLED);
                return(1);
            } // end if
        } // end if

        if (cull_flags & CULL_OBJECT_Y_PLANE)
        {
            // cull only based on y clipping planes
            // we could use plane equations, but simple similar triangles
            // is easier since this is really a 2D problem
            // if the view volume is 90 degrees the the problem is trivial
            // buts lets assume its not

            // test the the top and bottom clipping planes against the bottommost and topmost
            // points of the bounding sphere
            float z_test = (0.5) * cam->viewplane_height * sphere_pos.z / cam->view_dist;

            if (((sphere_pos.y - obj->max_radius) > z_test) || // top side
                ((sphere_pos.y + obj->max_radius) < -z_test))  // bottom side, note sign change
            {
                SET_BIT(obj->state, OBJECT4DV1_STATE_CULLED);
                return(1);
            } // end if

        } // end if

        // return failure to cull
        return(0);

    } // end Cull_OBJECT4DV1
}