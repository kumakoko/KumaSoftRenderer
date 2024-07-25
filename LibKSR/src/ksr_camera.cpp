#include <cmath>
#include "ksr_camera.h"
#include "ksr_constants.h"
#include "ksr_vector.h"
#include "ksr_matrix.h"

namespace KSR
{
    void Init_CAM4DV1(CAM4DV1_PTR cam, CameraModelType cam_attr, POINT4D_PTR cam_pos, VECTOR4D_PTR cam_dir, POINT4D_PTR cam_target, float near_clip_z, float far_clip_z, float fov, float viewport_width, float viewport_height)
    {
        cam->attr = cam_attr;              // camera attributes

        VECTOR4D_COPY(&cam->pos, cam_pos); // 记录下相机的位置和观察方向
        VECTOR4D_COPY(&cam->dir, cam_dir);

        // for UVN 设置UVN模式下的相机的三个方向向量
        VECTOR4D_INITXYZ(&cam->u, 1, 0, 0);  // set to +x
        VECTOR4D_INITXYZ(&cam->v, 0, 1, 0);  // set to +y
        VECTOR4D_INITXYZ(&cam->n, 0, 0, 1);  // set to +z        

        if (cam_target != nullptr)
            VECTOR4D_COPY(&cam->target, cam_target); // UVN target
        else
            VECTOR4D_ZERO(&cam->target);

        cam->near_clip_z = near_clip_z;     // 记录下远近视截面
        cam->far_clip_z = far_clip_z;
        cam->viewport_width = viewport_width;   // 记录下视口的高宽值，中心点值，宽高比
        cam->viewport_height = viewport_height;
        cam->viewport_center_x = (viewport_width - 1) / 2;
        cam->viewport_center_y = (viewport_height - 1) / 2;
        cam->aspect_ratio = static_cast<float>(viewport_width) / static_cast<float>(viewport_height);
        cam->fov = fov; // 记录下FOV值
        cam->viewplane_width = 2.0f; // set the viewplane dimensions up, they will be 2 x (2/ar)
        cam->viewplane_height = 2.0f / cam->aspect_ratio;
        // 记录下M2V V2P P2S矩阵
        MAT_IDENTITY_4X4(&cam->mcam);
        MAT_IDENTITY_4X4(&cam->mper);
        MAT_IDENTITY_4X4(&cam->mscr);



        // now we know fov and we know the viewplane dimensions plug into formula and
        // solve for view distance parameters
        float tan_fov_div2 = tanf(DEG_TO_RAD(fov / 2));

        cam->view_dist = 0.5f * (cam->viewplane_width) * tan_fov_div2;

        // test for 90 fov first since it's easy :)
        if (fov == 90.0f)
        {
            // set up the clipping planes -- easy for 90 degrees!
            POINT3D pt_origin; // point on the plane
            VECTOR3D_INITXYZ(&pt_origin, 0, 0, 0);

            VECTOR3D vn; // normal to plane

            // right clipping plane 
            VECTOR3D_INITXYZ(&vn, 1, 0, -1); // x=z plane
            PLANE3D_Init(&cam->rt_clip_plane, &pt_origin, &vn, 1);

            // left clipping plane
            VECTOR3D_INITXYZ(&vn, -1, 0, -1); // -x=z plane
            PLANE3D_Init(&cam->lt_clip_plane, &pt_origin, &vn, 1);

            // top clipping plane
            VECTOR3D_INITXYZ(&vn, 0, 1, -1); // y=z plane
            PLANE3D_Init(&cam->tp_clip_plane, &pt_origin, &vn, 1);

            // bottom clipping plane
            VECTOR3D_INITXYZ(&vn, 0, -1, -1); // -y=z plane
            PLANE3D_Init(&cam->bt_clip_plane, &pt_origin, &vn, 1);
        } // end if d=1
        else
        {
            // now compute clipping planes yuck!
            POINT3D pt_origin; // point on the plane
            VECTOR3D_INITXYZ(&pt_origin, 0, 0, 0);

            VECTOR3D vn; // normal to plane

            // since we don't have a 90 fov, computing the normals
            // are a bit tricky, there are a number of geometric constructions
            // that solve the problem, but I'm going to solve for the
            // vectors that represent the 2D projections of the frustrum planes
            // on the x-z and y-z planes and then find perpendiculars to them

            // right clipping plane, check the math on graph paper 
            VECTOR3D_INITXYZ(&vn, cam->view_dist, 0, -cam->viewplane_width / 2.0f);
            PLANE3D_Init(&cam->rt_clip_plane, &pt_origin, &vn, 1);

            // left clipping plane, we can simply reflect the right normal about
            // the z axis since the planes are symetric about the z axis
            // thus invert x only
            VECTOR3D_INITXYZ(&vn, -cam->view_dist, 0, -cam->viewplane_width / 2.0f);
            PLANE3D_Init(&cam->lt_clip_plane, &pt_origin, &vn, 1);

            // top clipping plane, same construction
            VECTOR3D_INITXYZ(&vn, 0, cam->view_dist, -cam->viewplane_width / 2.0f);
            PLANE3D_Init(&cam->tp_clip_plane, &pt_origin, &vn, 1);

            // bottom clipping plane, same inversion
            VECTOR3D_INITXYZ(&vn, 0, -cam->view_dist, -cam->viewplane_width / 2.0f);
            PLANE3D_Init(&cam->bt_clip_plane, &pt_origin, &vn, 1);
        } // end else

    } // end Init_CAM4DV1

    void Build_CAM4DV1_Matrix_Euler(CAM4DV1_PTR cam, CameraRotationSequences cam_rot_seq)
    {
        // this creates a camera matrix based on Euler angles 
        // and stores it in the sent camera object
        // if you recall from chapter 6 to create the camera matrix
        // we need to create a transformation matrix that looks like:

        // Mcam = mt(-1) * my(-1) * mx(-1) * mz(-1)
        // that is the inverse of the camera translation matrix mutilplied
        // by the inverses of yxz, in that order, however, the order of
        // the rotation matrices is really up to you, so we aren't going
        // to force any order, thus its programmable based on the value
        // of cam_rot_seq which can be any value CAM_ROT_SEQ_XYZ where 
        // XYZ can be in any order, YXZ, ZXY, etc.

        MATRIX4X4 mt_inv,  // inverse camera translation matrix
            mx_inv,  // inverse camera x axis rotation matrix
            my_inv,  // inverse camera y axis rotation matrix
            mz_inv,  // inverse camera z axis rotation matrix
            mrot,    // concatenated inverse rotation matrices
            mtmp;    // temporary working matrix


        // step 1: create the inverse translation matrix for the camera
        // position
        Mat_Init_4X4(&mt_inv, 1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            -cam->pos.x, -cam->pos.y, -cam->pos.z, 1);

        // step 2: create the inverse rotation sequence for the camera
        // rember either the transpose of the normal rotation matrix or
        // plugging negative values into each of the rotations will result
        // in an inverse matrix

        // first compute all 3 rotation matrices

        // extract out euler angles
        float theta_x = cam->dir.x;
        float theta_y = cam->dir.y;
        float theta_z = cam->dir.z;

        // compute the sine and cosine of the angle x
        float cos_theta = cosf(theta_x);  // no change since cos(-x) = cos(x)
        float sin_theta = -sinf(theta_x); // sin(-x) = -sin(x)

        // set the matrix up 
        Mat_Init_4X4(&mx_inv, 1, 0, 0, 0,
            0, cos_theta, sin_theta, 0,
            0, -sin_theta, cos_theta, 0,
            0, 0, 0, 1);

        // compute the sine and cosine of the angle y
        cos_theta = cosf(theta_y);  // no change since cos(-x) = cos(x)
        sin_theta = -sinf(theta_y); // sin(-x) = -sin(x)

        // set the matrix up 
        Mat_Init_4X4(&my_inv, cos_theta, 0, -sin_theta, 0,
            0, 1, 0, 0,
            sin_theta, 0, cos_theta, 0,
            0, 0, 0, 1);

        // compute the sine and cosine of the angle z
        cos_theta = cosf(theta_z);  // no change since cos(-x) = cos(x)
        sin_theta = -sinf(theta_z); // sin(-x) = -sin(x)

        // set the matrix up 
        Mat_Init_4X4(&mz_inv, cos_theta, sin_theta, 0, 0,
            -sin_theta, cos_theta, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1);

        // now compute inverse camera rotation sequence
        switch (cam_rot_seq)
        {
        case CAM_ROT_SEQ_XYZ:
        {
            Mat_Mul_4X4(&mx_inv, &my_inv, &mtmp);
            Mat_Mul_4X4(&mtmp, &mz_inv, &mrot);
        } break;

        case CAM_ROT_SEQ_YXZ:
        {
            Mat_Mul_4X4(&my_inv, &mx_inv, &mtmp);
            Mat_Mul_4X4(&mtmp, &mz_inv, &mrot);
        } break;

        case CAM_ROT_SEQ_XZY:
        {
            Mat_Mul_4X4(&mx_inv, &mz_inv, &mtmp);
            Mat_Mul_4X4(&mtmp, &my_inv, &mrot);
        } break;

        case CAM_ROT_SEQ_YZX:
        {
            Mat_Mul_4X4(&my_inv, &mz_inv, &mtmp);
            Mat_Mul_4X4(&mtmp, &mx_inv, &mrot);
        } break;

        case CAM_ROT_SEQ_ZYX:
        {
            Mat_Mul_4X4(&mz_inv, &my_inv, &mtmp);
            Mat_Mul_4X4(&mtmp, &mx_inv, &mrot);
        } break;

        case CAM_ROT_SEQ_ZXY:
        {
            Mat_Mul_4X4(&mz_inv, &mx_inv, &mtmp);
            Mat_Mul_4X4(&mtmp, &my_inv, &mrot);

        } break;

        default: break;
        } // end switch

  // now mrot holds the concatenated product of inverse rotation matrices
  // multiply the inverse translation matrix against it and store in the 
  // camera objects' camera transform matrix we are done!
        Mat_Mul_4X4(&mt_inv, &mrot, &cam->mcam);

    } // end Build_CAM4DV1_Matrix_Euler

    void Build_CAM4DV1_Matrix_UVN(CAM4DV1_PTR cam, int mode)
    {
        // this creates a camera matrix based on a look at vector n,
        // look up vector v, and a look right (or left) u
        // and stores it in the sent camera object, all values are
        // extracted out of the camera object itself
        // mode selects how uvn is computed
        // UVN_MODE_SIMPLE - low level simple model, use the target and view reference point
        // UVN_MODE_SPHERICAL - spherical mode, the x,y components will be used as the
        //     elevation and heading of the view vector respectively
        //     along with the view reference point as the position
        //     as usual

        MATRIX4X4 mt_inv,  // inverse camera translation matrix
            mt_uvn,  // the final uvn matrix
            mtmp;    // temporary working matrix

        // step 1: create the inverse translation matrix for the camera
        // position
        Mat_Init_4X4(&mt_inv, 1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            -cam->pos.x, -cam->pos.y, -cam->pos.z, 1);


        // step 2: determine how the target point will be computed
        if (mode == UVN_MODE_SPHERICAL)
        {
            // use spherical construction
            // target needs to be recomputed

            // extract elevation and heading 
            float phi = cam->dir.x; // elevation
            float theta = cam->dir.y; // heading

            // compute trig functions once
            float sin_phi = sinf(phi);
            float cos_phi = cosf(phi);

            float sin_theta = sinf(theta);
            float cos_theta = cosf(theta);

            // now compute the target point on a unit sphere x,y,z
            cam->target.x = -1 * sin_phi * sin_theta;
            cam->target.y = 1 * cos_phi;
            cam->target.z = 1 * sin_phi * cos_theta;
        } // end else

     // at this point, we have the view reference point, the target and that's
     // all we need to recompute u,v,n
     // Step 1: n = <target position - view reference point>
        VECTOR4D_Build(&cam->pos, &cam->target, &cam->n);

        // Step 2: Let v = <0,1,0>
        VECTOR4D_INITXYZ(&cam->v, 0, 1, 0);

        // Step 3: u = (v x n)
        VECTOR4D_Cross(&cam->v, &cam->n, &cam->u);

        // Step 4: v = (n x u)
        VECTOR4D_Cross(&cam->n, &cam->u, &cam->v);

        // Step 5: normalize all vectors
        VECTOR4D_Normalize(&cam->u);
        VECTOR4D_Normalize(&cam->v);
        VECTOR4D_Normalize(&cam->n);


        // build the UVN matrix by placing u,v,n as the columns of the matrix
        Mat_Init_4X4(&mt_uvn, cam->u.x, cam->v.x, cam->n.x, 0,
            cam->u.y, cam->v.y, cam->n.y, 0,
            cam->u.z, cam->v.z, cam->n.z, 0,
            0, 0, 0, 1);

        // now multiply the translation matrix and the uvn matrix and store in the 
        // final camera matrix mcam
        Mat_Mul_4X4(&mt_inv, &mt_uvn, &cam->mcam);

    } // end Build_CAM4DV1_Matrix_UVN
}