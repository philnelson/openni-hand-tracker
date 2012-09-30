//
//  Header.h
//  openni-hand-tracker
//
//  Created by maurilio di cicco on 22/05/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef hand_h
#define hand_h

#define HAND_DIM    21

#define indice1     0
#define indice2     1
#define indice3     2
#define medio1      3
#define medio2      4
#define medio3      5
#define anulare1    6
#define anulare2    7
#define anulare3    8
#define mignolo1    9
#define mignolo2    10
#define mignolo3    11
#define pollice1    12
#define pollice2    13
#define pollice3    14
#define angoloX     15
#define angoloY     16
#define angoloZ     17
#define posX        18
#define posY        19
#define posZ        20

#include <iostream> 
#include <assert.h>
#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include <ctime>




//
//struct ConstraintsStruct
//{
//    
//    float MIN_angolo1_indice;
//    float MIN_angolo2_indice;
//    float MIN_angolo3_indice;
//    
//    float MIN_angolo1_medio;
//    float MIN_angolo2_medio;
//    float MIN_angolo3_medio;
//    
//    float MIN_angolo1_anulare;
//    float MIN_angolo2_anulare;
//    float MIN_angolo3_anulare;
//    
//    float MIN_angolo1_mignolo;
//    float MIN_angolo2_mignolo;
//    float MIN_angolo3_mignolo;
//    
//    float MIN_angolo1_pollice;
//    float MIN_angolo2_pollice;
//    float MIN_angolo3_pollice;
//    
//    float MIN_rotX;
//    float MIN_rotY;
//    float MIN_rotZ;
//    
//    float MIN_posX;
//    float MIN_posY;
//    float MIN_posZ;
//    
//    float MAX_angolo1_indice;
//    float MAX_angolo2_indice;
//    float MAX_angolo3_indice;
//    
//    float MAX_angolo1_medio;
//    float MAX_angolo2_medio;
//    float MAX_angolo3_medio;
//    
//    float MAX_angolo1_anulare;
//    float MAX_angolo2_anulare;
//    float MAX_angolo3_anulare;
//    
//    float MAX_angolo1_mignolo;
//    float MAX_angolo2_mignolo;
//    float MAX_angolo3_mignolo;
//    
//    float MAX_angolo1_pollice;
//    float MAX_angolo2_pollice;
//    float MAX_angolo3_pollice;
//    
//    float MAX_rotX;
//    float MAX_rotY;
//    float MAX_rotZ;
//    
//    float MAX_posX;
//    float MAX_posY;
//    float MAX_posZ;
//
//    
//};

class Hand
{
    
public:
    
    //constructor
    Hand(float* stato);
    //destructor
    ~Hand();     
    //draw routine 
    void draw(); 
    
    void getState(float* state);
    void setState(float* state);
    
    void randomUpdate(bool FLOAT);
    
    float Constraints[HAND_DIM][2];

private:
    
    //WHOLE HAND POSITION IN THE SPACE
    
//    float _x;
//    float _y;
//    float _z;
    
    
    //Represents the state of all the fingers
    float _hand[HAND_DIM];
    
    
    //Draws a finger placed in the <0+offX,0+offY,0+offZ>, of a certain lenght with some angle
    void _finger(float offsetx,float offsety, float offsetz,
                 float lunghezza_f,float articolazione,
                 int angolo1,int angolo2,int angolo3);
    
    //Draws the thumb base
    void _thumb(float offsetx,float offsety, float offsetz,
                float lunghezza_f,float articolazione,
                int angolo1,int angolo2,int angolo3);

    //Draws the palm
    void _palm(float offsetx,float offsety,float offsetz);
    
    //Draws the thumb finger placed in the <0+offX,0+offY,0+offZ>, of a certain lenght with some angle    
    void _thumbFinger(float offsetx,float offsety, float offsetz,
                      float lunghezza_f,float articolazione,
                      int angolo1,int angolo2,int angolo3);
        
     
};

#endif
