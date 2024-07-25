#include "render.h"
#include "ksr_surface.h"

/*


// 初始化这个唯一的待渲染三角形
static void _InitSinglePolygon()
{
    poly1.state = POLY4DV1_STATE_ACTIVE;
    poly1.attr = 0;
    poly1.color = 0x00FF00FF;//RGB16Bit(0, 255, 0);

    poly1.vlist[0].x = 0.0f;
    poly1.vlist[0].y = 50.0f;
    poly1.vlist[0].z = 0.0f;
    poly1.vlist[0].w = 1.0f;

    poly1.vlist[1].x = 50.0f;
    poly1.vlist[1].y = -50.0;
    poly1.vlist[1].z = 0.0f;
    poly1.vlist[1].w = 1.0;

    poly1.vlist[2].x = -50.0f;
    poly1.vlist[2].y = -50.0f;
    poly1.vlist[2].z = 0.0f;
    poly1.vlist[2].w = 1.0f;

    poly1.next = poly1.prev = nullptr;
}

int Game_Init(void* parms)
{
    _InitSinglePolygon();

    // initialize the camera with 90 FOV, normalized coordinates
    KSR::Init_CAM4DV1(&cam,      // the camera object
        KSR::CAM_MODEL_EULER, // euler camera model
        &cam_pos,  // initial camera position
        &cam_dir,  // initial camera angles
        nullptr,      // no initial target
        50.0f,      // near and far clipping planes
        500.0f,
        90.0f,      // field of view in degrees
        static_cast<float>(WINDOW_WIDTH),   // size of final screen viewport
        static_cast<float>(WINDOW_HEIGHT));

    // return success
    return 1;

} // end Game_Init


int Game_Main(void* parms)
{
    // this is the workhorse of your game it will be called
    // continuously in real-time this is like main() in C
    // all the calls for you game go here!

    static MATRIX4X4 mrot; // general rotation matrix
    static float ang_y = 0;      // rotation angle

  //  int index; // looping var

    // start the timing clock
 //   Start_Clock();

    // clear the drawing surface 
    DDraw_Fill_Surface(KSR::lpddsback, 0);

    // read keyboard and other devices here
  //  DInput_Read_Keyboard();

    // game logic here...

    // initialize the renderlist
    Reset_RENDERLIST4DV1(&rend_list);

    // insert polygon into the renderlist
    Insert_POLYF4DV1_RENDERLIST4DV1(&rend_list, &poly1);

    // generate rotation matrix around y axis
    Build_XYZ_Rotation_MATRIX4X4(0, ang_y, 0, &mrot);

    // rotate polygon slowly
    if (++ang_y >= 360.0) 
        ang_y = 0;

    // rotate the local coords of single polygon in renderlist
    KSR::Transform_RENDERLIST4DV1(&rend_list, &mrot, KSR::TRANSFORM_LOCAL_ONLY);

    // perform local/model to world transform
    Model_To_World_RENDERLIST4DV1(&rend_list, &poly1_pos);

    // generate camera matrix
    Build_CAM4DV1_Matrix_Euler(&cam, CAM_ROT_SEQ_ZYX);

    // apply world to camera transform
    World_To_Camera_RENDERLIST4DV1(&rend_list, &cam);

    // apply camera to perspective transformation
    Camera_To_Perspective_RENDERLIST4DV1(&rend_list, &cam);

    // apply screen transform
    Perspective_To_Screen_RENDERLIST4DV1(&rend_list, &cam);

    // draw instructions
 //   Draw_Text_GDI("Press ESC to exit.", 0, 0, RGB(0, 255, 0), lpddsback);

    // lock the back buffer
    DDraw_Lock_Back_Surface();

    // render the polygon list
 //   Draw_RENDERLIST4DV1_Wire16(&rend_list, back_buffer, back_lpitch);

    // unlock the back buffer
 //   DDraw_Unlock_Back_Surface();

    // flip the surfaces
 //   DDraw_Flip();

    // sync to 30ish fps
//    Wait_Clock(30);

    // check of user is trying to exit
 //   if (KEY_DOWN(VK_ESCAPE) || keyboard_state[DIK_ESCAPE])
 //   {
 //       PostMessage(main_window_handle, WM_DESTROY, 0, 0);

 //   } // end if

// return success
    return(1);

} // end Game_Main

*/