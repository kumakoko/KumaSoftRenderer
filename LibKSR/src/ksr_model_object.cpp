#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "fmt/format.h"
#include "ksr_model_object.h"
#include "ksr_error.h"
#include "ksr_constants.h"
#include "ksr_color.h"

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
        //sscanf(token_string, "%s %d %d", obj->name, &obj->num_vertices, &obj->num_polys);

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
  /*          sscanf(token_string, "%f %f %f", &obj->vlist_local[vertex].x,
                &obj->vlist_local[vertex].y,
                &obj->vlist_local[vertex].z);
            obj->vlist_local[vertex].w = 1;*/

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
            /*
            sscanf(token_string, "%s %d %d %d %d", tmp_string,
                &poly_num_verts, // should always be 3
                &obj->plist[poly].vert[0],
                &obj->plist[poly].vert[1],
                &obj->plist[poly].vert[2]);
            */


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

}