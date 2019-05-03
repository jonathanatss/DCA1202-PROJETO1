#include <iostream>
#include "voxel_sculptor.h"
#include "sculptor.cpp"

using namespace std;

int main()
{
    Sculptor paint(2,2,2);
    //paint.setColor(1,1,1,1);
    //paint.putBox(1,1,1,1,1,1);
    //paint.cutBox(1,1,1,1,1,1);
    //paint.putSphere(1,1,1,1);
    //paint.cutSphere(1,1,1,1);
    //paint.putEllipsoid(1,1,1,1,1,1);
    //paint.cutEllipsoid(1,1,1,1,1,1);
    paint.writeOFF("C:\PROJETODCA1202\teste.off");

    return 0;
}


