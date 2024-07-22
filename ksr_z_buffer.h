#pragma once

namespace KSR
{

// structure for zbuffer
typedef struct ZBUFFERV1_TYP
{
int attr;       // attributes of zbuffer
UCHAR *zbuffer; // ptr to storage
int width;      // width in zpixels
int height;     // height in zpixels
int sizeq;      // total size in QUADs
                // of zbuffer
} ZBUFFERV1, *ZBUFFERV1_PTR;


int Create_Zbuffer(ZBUFFERV1_PTR zb, // pointer to a zbuffer object
                   int width,      // width 
                   int height,     // height
                   int attr);       // attributes of zbuffer

int Delete_Zbuffer(ZBUFFERV1_PTR zb);

void Clear_Zbuffer(ZBUFFERV1_PTR zb, UINT data);

}