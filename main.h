//
//  main.h
//  3D_Hand
//
//  Created by maurilio di cicco on 08/04/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef _D_Hand_main_h
#define _D_Hand_main_h

#endif

#include <XnOS.h>
#include <XnCppWrapper.h>

#include <iostream>
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>
#include <iostream>
#include <fstream>
#include <ctime>
#include "ShaderLoader.cpp"

using namespace xn;

void initGL(int width, int height);
void menu(int op);
void keyboardDown(unsigned char key, int x, int y);
void keyboardUp(unsigned char key, int x, int y);
void keyboardSpecialDown(int k, int x, int y);
void keyboardSpecialUp(int k, int x, int y);
void reshape(int width, int height);
void mouseClick(int button, int state, int x, int y);
void mouseMotion(int x, int y);

void draw();
void draw2();

void draw3();
void reshapeKinect(int width, int height);

void idle();
void hand(float offsetx,float offsety,float offsetz);
void display2();
void DrawAxes(float length);
void finger(float offsetx,float offsety, float offsetz,float lunghezza_f,float articolazione,int angolo1,int angolo2,int angolo3);
void pollice_dito(float offsetx,float offsety, float offsetz,float lunghezza_f,float articolazione,int angolo1,int angolo2,int angolo3);
void pollice(float offsetx,float offsety, float offsetz,float lunghezza_f,float articolazione,int angolo1,int angolo2,int angolo3);
void fullhand(float posx, float posy, float posz, float ax, float ay, float az);
int kinect_fn(int index);
float absf(float num);


#define PARTICLENUM 5000
#define MAXITERATION 5000
#define VMAX 0.2f
#define LIMIT 90

#define PSO 1
#define SECONDARYWIN 1

#define DIMX 640
#define DIMY 480
#define MAX_DEPTH 1000


#define SAMPLE_XML_PATH "SamplesConfig.xml"


//ROBA PER I TEXEL


float* texMap= new float[DIMX*DIMY]; //memorizzo la depth map della mano generato con le openGL
float* kinectMap= new float[DIMX*DIMY]; //memorizzo la depth map della mano generato con le openGL
float* tmpMap= new float[DIMX*DIMY];

unsigned int texMapX = 0;
unsigned int texMapY = 0;


//ROBA KINECT
// Parameters for the K matrix of the kinect
static const double fx_d = 5.9421434211923247e+02;
static const double fy_d = 5.9104053696870778e+02;
static const double cx_d = 3.3930780975300314e+02;
static const double cy_d = 2.4273913761751615e+02;
// Variables used to get data from the kinect
Context g_context;
ScriptNode g_scriptNode;
DepthGenerator g_depth;
ImageGenerator g_image;
DepthMetaData g_depthMD;
ImageMetaData g_imageMD;


int PixelErrorSum; //differenza tra le depth maps

//Struttura dati delle particella, in questo caso demo è solo per la posa 3Dof della mano
//Dettaglio:
//conservo in x,y,z gli angoli generati casualmente
//conservo in error l'errore MINIMO trovato fin'ora
//conservo in best[3] la tripla x,y,z relativa all'errore MINIMO trovato fin'ora, all'update se il nuovo stato della particella
//  porta ad un valore della funzione di errore minore scarto quella che ho conservato e l'aggiorno con l'attuale in modo da utilizzare
//  la nuova tripla

struct particle {
    float x;
    float y;
    float z;
    float vx;
    float vy;
    float vz;
    float error;
    float best[3];
    int index;
};


particle solution; //MI SERVE SOLO PER DETERMINARE LA FUNZIONE DI ERRORE
particle bestparticle;
particle particlearray[PARTICLENUM]; //IL MIO STORMO
int iteration=0;

void particleinit(); //init delle particelle
void best_particle_so_far(); //ritorna la particella migliore dello stormo in modo da ricavare la componente sociale
void best_for_particle(particle *theparticle); //useless, da rimuovere
void update_particle(particle *theparticle); //aggiorna le componenti della singola particella
float compute_error(particle *theparticle);


float spin_x,spin_y,old_x,old_y,oold_x,oold_y=0;
int window2 = 0;
int window3 = 0;
