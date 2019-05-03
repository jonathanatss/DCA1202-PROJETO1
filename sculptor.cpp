#include <iostream>
#include <fstream>
#include "voxel_sculptor.h"

using namespace std;

Sculptor::Sculptor(int nx, int ny, int nz){
    if(nx<=0 || ny<=0 || nz<=0){
        nx = 0;
        ny = 0;
        nz = 0;
    }

    v = new Voxel**[nx];
    if(v == nullptr){
        cout << "Erro na alocação das linhas do Voxel" << endl;
        exit(0);
    }
    v[0] = new Voxel*[nx*ny];
    if(v[0] == nullptr){
        cout << "Erro na alocação das colunas do Voxel" << endl;
        exit(0);
    }
    v[0][0] = new Voxel[nx*ny*nz];
    if(v[0][0] == nullptr){
        cout << "Erro na alocação dos planos do Voxel" << endl;
        exit(0);
    }

    for(int i = 1; i<nx; i++){

        v[i] = v[i-1] + ny;
    }
    for(int i = 1; i<nx*ny; i++){
        v[0][i] = v[0][i-1] + nz;
    }
    for(int i=0;i<nx;i++){
        for(int j=0;j<ny;j++){
            for(int k=0;k<nz;k++){
                v[i][j][k].isOn = false;
            }
        }
    }
}

Sculptor::~Sculptor(){
    if(nx<=0 || ny<=0 || nz<=0){
        return;
    }
    delete v[0][0];
    delete v[0];
    delete v;
}

void Sculptor::setColor(float r, float g, float b, float alpha){
    if(r>1){
        r = 1;
    }
    else if(r<0){
        r = 0;
    }
    if(g>1){
        g = 1;
    }
    else if(g<0){
        g = 0;
    }
    if(b>1){
        b = 1;
    }
    else if(b<0){
        b = 0;
    }
    if(alpha>1){
        alpha = 1;
    }
    else if(alpha<0){
        alpha = 0;
    }
}

void Sculptor::putVoxel(int x, int y, int z){
    v[x][y][z].isOn = true;
    v[x][y][z].r = r;
    v[x][y][z].g = g;
    v[x][y][z].b = b;
    v[x][y][z].a = a;
}

void Sculptor::cutVoxel(int x, int y, int z){
    v[x][y][z].isOn = false;
}

void Sculptor::putBox(int x0, int x1, int y0, int y1, int z0, int z1){
    for(int i=x0;i<x1+1;i++){
        for(int j=y0;j<y1+1;j++){
            for(int k=z0;k<z1+1;k++){
                v[i][j][k].isOn = true;
                v[i][j][k].r = r;
                v[i][j][k].g = g;
                v[i][j][k].b = b;
                v[i][j][k].a = a;
            }
        }
    }
}

void Sculptor::cutBox(int x0, int x1, int y0, int y1, int z0, int z1){
    for(int i=x0;i<x1+1;i++){
        for(int j=y0;j<y1+1;j++){
            for(int k=z0;k<z1+1;k++){
                v[i][j][k].isOn = false;
            }
        }
    }
}

void Sculptor::putSphere(int xcenter, int ycenter, int zcenter, int radius){
    for(int i=0;i<xcenter;i++){
        for(int j=0;j<ycenter;j++){
            for(int k=0;k<zcenter;k++){
                if(((i-xcenter)*(i-xcenter) + (j-ycenter)*(j-ycenter) + (k-zcenter)*(k-zcenter)) <= radius*radius){
                    v[i][j][k].isOn = true;
                    v[i][j][k].r = r;
                    v[i][j][k].g = g;
                    v[i][j][k].b = b;
                    v[i][j][k].a = a;
                }
            }
        }
    }
}

void Sculptor::cutSphere(int xcenter, int ycenter, int zcenter, int radius){
    for(int i=0;i<xcenter;i++){
        for(int j=0;j<ycenter;j++){
            for(int k=0;k<zcenter;k++){
                if(((i-xcenter)*(i-xcenter) + (j-ycenter)*(j-ycenter) + (k-zcenter)*(k-zcenter)) <= radius*radius){
                    v[i][j][k].isOn = false;
                }
            }
        }
    }
}

void Sculptor::putEllipsoid(int xcenter, int ycenter, int zcenter, int rx, int ry, int rz){
    for(int i=0;i<rx;i++){
        for(int j=0;j<ry;j++){
            for(int k=0;k<rz;k++){
                if((((i-xcenter)*(i-xcenter))/(float)(rx*rx) + ((j-ycenter)*(j-ycenter))/(float)(ry*ry) + ((k-zcenter)*(k-zcenter)))/(float)(rz*rz) <= 1){
                    v[i][j][k].isOn = true;
                    v[i][j][k].r = r;
                    v[i][j][k].g = g;
                    v[i][j][k].b = b;
                    v[i][j][k].a = a;
                }
            }
        }
    }
}

void Sculptor::cutEllipsoid(int xcenter, int ycenter, int zcenter, int rx, int ry, int rz){
    for(int i=0;i<rx;i++){
        for(int j=0;j<ry;j++){
            for(int k=0;k<rz;k++){
                if((((i-xcenter)*(i-xcenter))/(float)(rx*rx) + ((j-ycenter)*(j-ycenter))/(float)(ry*ry) + ((k-zcenter)*(k-zcenter)))/(float)(rz*rz) <= 1){
                    v[i][j][k].isOn = false;
                }
            }
        }
    }
}

void Sculptor::writeOFF(string filename){
    ofstream outfile(filename);
    outfile << "OFF" << endl;
    int nv = 1, nvervox = 0;
    for(int i=0;i<nx;i++){
        for(int j=0;j<ny;j++){
            for(int k=0;k<nz;k++){
                if(v[i][j][k].isOn){
                    nv++;
                }
            }
       }
    }
    outfile << 8*nv << " " << 6*nv << " " << 0 << std::endl;
    for(int i=0;i<nx;i++){
        for(int j=0;j<ny;j++){
            for(int k=0;k<nz;k++){
                if(v[i][j][k].isOn){
                    outfile << -0.5+i << " " << 0.5+j << " " << -0.5+k << endl;
                    outfile << -0.5+i << " " << -0.5+j << " " << -0.5+k << endl;
                    outfile << 0.5+i << " " << -0.5+j << " " << -0.5+k << endl;
                    outfile << 0.5+i << " " << 0.5+j << " " << -0.5+k << endl;
                    outfile << -0.5+i << " " << 0.5+j << " " << 0.5+k << endl;
                    outfile << -0.5+i << " " << -0.5+j << " " << 0.5+k << endl;
                    outfile << 0.5+i << " " << -0.5+j << " " << 0.5+k << endl;
                    outfile << 0.5+i << " " << 0.5+j << " " << 0.5+k << endl;
                }
            }
        }
    }
    for(int i=0;i<nx;i++){
        for(int j=0;j<ny;j++){
            for(int k=0;k<nz;k++){
                if(v[i][j][k].isOn == 0){
                    outfile << "4" << nvervox+0 << " " << nvervox+3 << " " << nvervox+2 << " " << nvervox+1 << " " << v[i][j][k].r << " " << v[i][j][k].g << " " << v[i][j][k].b << " " << v[i][j][k].a << endl;
                    outfile << "4" << nvervox+4 << " " << nvervox+5 << " " << nvervox+6 << " " << nvervox+7 << " " << v[i][j][k].r << " " << v[i][j][k].g << " " << v[i][j][k].b << " " << v[i][j][k].a << endl;
                    outfile << "4" << nvervox+0 << " " << nvervox+1 << " " << nvervox+5 << " " << nvervox+4 << " " << v[i][j][k].r << " " << v[i][j][k].g << " " << v[i][j][k].b << " " << v[i][j][k].a << endl;
                    outfile << "4" << nvervox+0 << " " << nvervox+4 << " " << nvervox+7 << " " << nvervox+3 << " " << v[i][j][k].r << " " << v[i][j][k].g << " " << v[i][j][k].b << " " << v[i][j][k].a << endl;
                    outfile << "4" << nvervox+3 << " " << nvervox+7 << " " << nvervox+6 << " " << nvervox+2 << " " << v[i][j][k].r << " " << v[i][j][k].g << " " << v[i][j][k].b << " " << v[i][j][k].a << endl;
                    outfile << "4" << nvervox+1 << " " << nvervox+2 << " " << nvervox+6 << " " << nvervox+5 << " " << v[i][j][k].r << " " << v[i][j][k].g << " " << v[i][j][k].b << " " << v[i][j][k].a << endl;
                    nvervox++;
                }
            }
        }
    }
    outfile.close();
}
