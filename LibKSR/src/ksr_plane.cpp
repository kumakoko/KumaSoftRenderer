#include "ksr_plane.h"

namespace KSR
{
    void PLANE3D_Init(PLANE3D_PTR plane, POINT3D_PTR p0,VECTOR3D_PTR normal, int normalize)
    {
        // this function initializes a 3d plane

        // copy the point
        POINT3D_COPY(&plane->p0, p0);

        // if normalize is 1 then the normal is made into a unit vector
        if (!normalize)
            VECTOR3D_COPY(&plane->n, normal);
        else
        {
            // make normal into unit vector
            VECTOR3D_Normalize(normal, &plane->n);
        } // end else

    } // end PLANE3D_Init
}