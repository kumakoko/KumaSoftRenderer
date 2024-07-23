#pragma once

#include "ksr_matrix.h"
#include "ksr_vector.h"
#include "ksr_plane.h"

namespace KSR
{
    typedef struct CAM4DV1_TYP
    {
        int state;      // state of camera
        int attr;       // camera attributes

        POINT4D pos;    // world position of camera used by both camera models

        VECTOR4D dir;   // angles or look at direction of camera for simple 
        // euler camera models, elevation and heading for
        // uvn model

        VECTOR4D u;     // extra vectors to track the camera orientation
        VECTOR4D v;     // for more complex UVN camera model
        VECTOR4D n;

        VECTOR4D target; // look at target

        float view_dist;  // focal length 

        float fov;          // field of view for both horizontal and vertical axes

        // 3d clipping planes
        // if view volume is NOT 90 degree then general 3d clipping
        // must be employed
        float near_clip_z;     // near z=constant clipping plane
        float far_clip_z;      // far z=constant clipping plane

        PLANE3D rt_clip_plane;  // the right clipping plane
        PLANE3D lt_clip_plane;  // the left clipping plane
        PLANE3D tp_clip_plane;  // the top clipping plane
        PLANE3D bt_clip_plane;  // the bottom clipping plane                        

        float viewplane_width;     // width and height of view plane to project onto
        float viewplane_height;    // usually 2x2 for normalized projection or 
        // the exact same size as the viewport or screen window

// remember screen and viewport are synonomous 
        float viewport_width;     // size of screen/viewport
        float viewport_height;
        float viewport_center_x;  // center of view port (final image destination)
        float viewport_center_y;

        // aspect ratio
        float aspect_ratio;

        // these matrices are not necessarily needed based on the method of
        // transformation, for example, a manual perspective or screen transform
        // and or a concatenated perspective/screen, however, having these 
        // matrices give us more flexibility         

        MATRIX4X4 mcam;   // storage for the world to camera transform matrix
        MATRIX4X4 mper;   // storage for the camera to perspective transform matrix
        MATRIX4X4 mscr;   // storage for the perspective to screen transform matrix

    } CAM4DV1, * CAM4DV1_PTR;


    void Init_CAM4DV1(CAM4DV1_PTR cam,       // the camera object
        int cam_attr,          // attributes
        POINT4D_PTR cam_pos,   // initial camera position
        VECTOR4D_PTR cam_dir,  // initial camera angles
        POINT4D_PTR cam_target, // UVN target
        float near_clip_z,     // near and far clipping planes
        float far_clip_z,
        float fov,             // field of view in degrees
        float viewport_width,  // size of final screen viewport
        float viewport_height);
}
