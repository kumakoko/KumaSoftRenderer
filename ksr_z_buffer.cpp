#include "ksr_z_buffer.h"

namespace KSR
{
	int Create_Zbuffer(ZBUFFERV1_PTR zb, // pointer to a zbuffer object
                   int width,      // width 
                   int height,     // height
                   int attr)       // attributes of zbuffer
{
// this function creates a zbuffer with the sent width, height, 
// and bytes per word, in fact the the zbuffer is totally linear
// but this function is nice so we can create it with some
// intelligence

// is this a valid object
if (!zb) 
   return 0;

// is there any memory already allocated
if (zb->zbuffer)
   free(zb->zbuffer);

// set fields
zb->width  = width;
zb->height = height;
zb->attr   = attr;

// what size zbuffer 16/32 bit?
if (attr & ZBUFFER_ATTR_16BIT)
   {
   // compute size in quads
   zb->sizeq = width*height/2;

   // allocate memory
   if ((zb->zbuffer = (UCHAR *)malloc(width * height * sizeof(SHORT))))
      return 1;
   else
      return 0;

   } // end if
else
if (attr & ZBUFFER_ATTR_32BIT)
   {
   // compute size in quads
   zb->sizeq = width*height;

   // allocate memory
   if ((zb->zbuffer = (UCHAR *)malloc(width * height * sizeof(INT))))
      return 1;
   else
      return 0;
   } // end if
else
   return 0;

} // end Create_Zbuffer

////////////////////////////////////////////////////////////////

int Delete_Zbuffer(ZBUFFERV1_PTR zb)
{
// this function deletes and frees the memory of the zbuffer

// test for valid object
if (zb)
   {
   // delete memory and zero object
   if (zb->zbuffer)
      free(zb->zbuffer);
   
   // clear memory
   memset((void *)zb,0, sizeof(ZBUFFERV1));

   return 1;

   } // end if
else
   return 0;

} // end Delete_Zbuffer

/////////////////////////////////////////////////////////////////

void Clear_Zbuffer(ZBUFFERV1_PTR zb, UINT data)
{
// this function clears/sets the zbuffer to a value, the filling
// is ALWAYS performed in QUADS, thus if your zbuffer is a 16-bit
// zbuffer then you must build a quad that has two values each
// the 16-bit value you want to fill with, otherwise just send 
// the fill value casted to a UINT

Mem_Set_QUAD((void *)zb->zbuffer, data, zb->sizeq); 

} // end Clear_Zbuffer
}