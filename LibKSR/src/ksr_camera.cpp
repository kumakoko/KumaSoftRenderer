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

#include <cmath>
#include "ksr_camera.h"
#include "ksr_constants.h"
#include "ksr_vector.h"
#include "ksr_matrix.h"

namespace KSR
{
    void Init_CAM4DV1(CAM4DV1_PTR camera, CameraModelType camera_attribute, POINT4D_PTR camera_pos, VECTOR4D_PTR camera_direction, POINT4D_PTR camera_target, float near_clip_z, float far_clip_z, float fov, float viewport_width, float viewport_height)
    {
        camera->attr = camera_attribute;

        VECTOR4D_COPY(&camera->pos, camera_pos); // 记录下相机的位置和观察方向
        VECTOR4D_COPY(&camera->dir, camera_direction);

        // for UVN 设置UVN模式下的相机的三个方向向量
        VECTOR4D_INITXYZ(&camera->u, 1, 0, 0);  // 设置为+x轴方向
        VECTOR4D_INITXYZ(&camera->v, 0, 1, 0);  // 设置为+y轴方向
        VECTOR4D_INITXYZ(&camera->n, 0, 0, 1);  // 设置为+z轴方向        

        if (camera_target != nullptr)
            VECTOR4D_COPY(&camera->target, camera_target); // UVN相机的目标位置
        else
            VECTOR4D_ZERO(&camera->target);

        camera->near_clip_z = near_clip_z;     // 记录下远近视截面
        camera->far_clip_z = far_clip_z;
        camera->viewport_width = viewport_width;   // 记录下视口的高宽值，中心点值，宽高比
        camera->viewport_height = viewport_height;
        camera->viewport_center_x = (viewport_width - 1) / 2;
        camera->viewport_center_y = (viewport_height - 1) / 2;
        camera->aspect_ratio = static_cast<float>(viewport_width) / static_cast<float>(viewport_height);
        camera->fov = fov; // 记录下FOV值
        camera->viewplane_width = 2.0f;
        camera->viewplane_height = 2.0f / camera->aspect_ratio;

        // 设置M2V V2P P2S矩阵为单位矩阵
        MAT_IDENTITY_4X4(&camera->mcam);
        MAT_IDENTITY_4X4(&camera->mper);
        MAT_IDENTITY_4X4(&camera->mscr);

        // 根据视平面大小和FOV计算出焦距（focal）
        float tan_fov_div2 = tanf(DEG_TO_RAD(fov / 2.0f));
        camera->view_dist = 0.5f * (camera->viewplane_width) * tan_fov_div2;

        // 如果FOV恰好为90度时，计算视截体的裁剪面会取巧得多
        if (fov == 90.0f)
        {
            // 开始建立视截体的裁剪面，首先指定裁剪面上的一个点和法线，使用点法式面
            POINT3D plane_origin;  // 面原点
            VECTOR3D plane_normal; // 面法线
            VECTOR3D_INITXYZ(&plane_origin, 0, 0, 0);

            /*
            当FOV为90度时右视截体裁剪面的法线的示意图
             \   | A (1,0,1)
              \  |  /\
               \ | /  \
                \|/    \ 这条便是c的法线AB,所以为(1,0,-1)
                 O------B
                (0,0,0)（1,0,0)
            */
            // 右视截体裁剪面，因为FOV是90度，所以右视截体裁剪面的法线
            VECTOR3D_INITXYZ(&plane_normal, 1.0f, 0.0f, -1.0f);
            PLANE3D_Init(&camera->rt_clip_plane, &plane_origin, &plane_normal, 1);

            // 左视截体裁剪面
            VECTOR3D_INITXYZ(&plane_normal, -1.0f, 0.0f, -1.0f);
            PLANE3D_Init(&camera->lt_clip_plane, &plane_origin, &plane_normal, 1);

            // 上视截体裁剪面
            VECTOR3D_INITXYZ(&plane_normal, 0.0f, 1.0f, -1.0f);
            PLANE3D_Init(&camera->tp_clip_plane, &plane_origin, &plane_normal, 1);

            // 下视截体裁剪面
            VECTOR3D_INITXYZ(&plane_normal, 0.0f, -1.0f, -1.0f);
            PLANE3D_Init(&camera->bt_clip_plane, &plane_origin, &plane_normal, 1);
        }
        else
        {
            POINT3D plane_origin;  // 面原点
            VECTOR3D plane_normal; // 面法线
            VECTOR3D_INITXYZ(&plane_origin, 0, 0, 0);

            // 首先计算右视截体裁剪面在平面xz和平面yz上的2D投影的向量，然后计算与这两个向量都垂直的向量，便是右视截体裁剪面的面法线
            VECTOR3D_INITXYZ(&plane_normal, camera->view_dist, 0, -camera->viewplane_width / 2.0f);
            PLANE3D_Init(&camera->rt_clip_plane, &plane_origin, &plane_normal, 1);

            // 左视截体裁剪面，因为和右视截体裁剪面关于z轴对称的，所以只需要把左视截体裁剪面法线的x分量取负数即得
            VECTOR3D_INITXYZ(&plane_normal, -camera->view_dist, 0, -camera->viewplane_width / 2.0f);
            PLANE3D_Init(&camera->lt_clip_plane, &plane_origin, &plane_normal, 1);

            // 上视截体裁剪面和下视截体裁剪面都按此法构建
            VECTOR3D_INITXYZ(&plane_normal, 0, camera->view_dist, -camera->viewplane_width / 2.0f);
            PLANE3D_Init(&camera->tp_clip_plane, &plane_origin, &plane_normal, 1);

            VECTOR3D_INITXYZ(&plane_normal, 0, -camera->view_dist, -camera->viewplane_width / 2.0f);
            PLANE3D_Init(&camera->bt_clip_plane, &plane_origin, &plane_normal, 1);
        }
    }

    void Build_CAM4DV1_Matrix_Euler(CAM4DV1_PTR cam, CameraRotationSequences cam_rot_seq)
    {
        /*
        根据欧拉角度计算相机变换矩阵
        即相机平移矩阵的逆矩阵乘以相机绕yxz轴的旋转矩阵的逆矩阵
        Mcam = mt(-1) * my(-1) * mx(-1) * mz(-1)
        采用何种旋转顺序可以任意，xyz，xzy，zyx，等等
        */
        MATRIX4X4 mt_inv;  // 相机平移矩阵的逆矩阵
        MATRIX4X4 mx_inv;  // 相机绕x轴的旋转矩阵的逆矩阵
        MATRIX4X4 my_inv;  // 相机绕y轴的旋转矩阵的逆矩阵
        MATRIX4X4 mz_inv;  // 相机绕z轴的旋转矩阵的逆矩阵
        MATRIX4X4 mrot;    // 所有逆旋转矩阵的乘积
        MATRIX4X4 mtmp;    // 临时中间矩阵，计算使用

        // 第1步：根据相机的平移位置算出相机平移矩阵的逆矩阵
        // 这是因为，如果相机把自身的坐标变换到世界坐标用到的平移矩阵为M，
        // 那么把世界空间下的物体变换到相机空间下的平移矩阵自然就是M的逆矩阵
        Mat_Init_4X4(&mt_inv,
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            -cam->pos.x, -cam->pos.y, -cam->pos.z, 1);

        // 要计算旋转矩阵的逆矩阵，可以将它转置，也可以将每个旋转角度取负来求得
        // 从Direction成员变量中，取出三个方向的欧拉角
        float theta_x = cam->dir.x;
        float theta_y = cam->dir.y;
        float theta_z = cam->dir.z;

        // 根据欧拉角计算出xyz轴三个方向的旋转矩阵的逆矩阵，每个轴的欧拉角分别求出正余弦值
        float cos_theta = cosf(theta_x);  // cos(-x) == cos(x)
        float sin_theta = -sinf(theta_x); // sin(-x) == -sin(x)

        // x轴上的旋转矩阵的逆矩阵
        Mat_Init_4X4(&mx_inv,
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, cos_theta, sin_theta, 0.0f,
            0.0f, -sin_theta, cos_theta, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f);

        cos_theta = cosf(theta_y);  // cos(-x) == cos(x)
        sin_theta = -sinf(theta_y); // sin(-x) == -sin(x)

        // y轴上的旋转矩阵的逆矩阵
        Mat_Init_4X4(&my_inv,
            cos_theta, 0.0f, -sin_theta, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            sin_theta, 0.0f, cos_theta, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f);

        cos_theta = cosf(theta_z);  // cos(-x) == cos(x)
        sin_theta = -sinf(theta_z); // sin(-x) == -sin(x)

        Mat_Init_4X4(&mz_inv,
            cos_theta, sin_theta, 0.0f, 0.0f,
            -sin_theta, cos_theta, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f);

        // 根据指定的旋转顺序构建旋转矩阵
        switch (cam_rot_seq)
        {
        case CAM_ROT_SEQ_XYZ:
            Mat_Mul_4X4(&mx_inv, &my_inv, &mtmp);
            Mat_Mul_4X4(&mtmp, &mz_inv, &mrot);
            break;
        case CAM_ROT_SEQ_YXZ:
            Mat_Mul_4X4(&my_inv, &mx_inv, &mtmp);
            Mat_Mul_4X4(&mtmp, &mz_inv, &mrot);
            break;
        case CAM_ROT_SEQ_XZY:
            Mat_Mul_4X4(&mx_inv, &mz_inv, &mtmp);
            Mat_Mul_4X4(&mtmp, &my_inv, &mrot);
            break;
        case CAM_ROT_SEQ_YZX:
            Mat_Mul_4X4(&my_inv, &mz_inv, &mtmp);
            Mat_Mul_4X4(&mtmp, &mx_inv, &mrot);
            break;
        case CAM_ROT_SEQ_ZYX:
            Mat_Mul_4X4(&mz_inv, &my_inv, &mtmp);
            Mat_Mul_4X4(&mtmp, &mx_inv, &mrot);
            break;
        case CAM_ROT_SEQ_ZXY:
            Mat_Mul_4X4(&mz_inv, &mx_inv, &mtmp);
            Mat_Mul_4X4(&mtmp, &my_inv, &mrot);
            break;
        default:
            break;
        }

        // now mrot holds the concatenated product of inverse rotation matrices
        // multiply the inverse translation matrix against it and store in the 
        // camera objects' camera transform matrix we are done!
        // 用mrot（逆旋转矩阵），乘以逆平移矩阵
        Mat_Mul_4X4(&mt_inv, &mrot, &cam->mcam);
    }

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
            mt_uvn;  // the final uvn matrix


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