//
//  Header.h
//  openni-hand-tracker
//
//  Created by maurilio di cicco on 22/05/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef hand_h
#define hand_h

#include <XnOS.h>
#include <XnCppWrapper.h>

#include <iostream>
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>
#include <iostream>
#include <fstream>
#include <ctime>
#include "ShaderLoader.cpp"

struct handState
{
    float angolo1_indice;
    float angolo2_indice;
    float angolo3_indice;
    
    float angolo1_medio;
    float angolo2_medio;
    float angolo3_medio;
    
    float angolo1_anulare;
    float angolo2_anulare;
    float angolo3_anulare;
    
    float angolo1_mignolo;
    float angolo2_mignolo;
    float angolo3_mignolo;
    
    float angolo1_pollice;
    float angolo2_pollice;
    float angolo3_pollice;
    
    float rotX;
    float rotY;
    float rotZ;
    
    float posX;
    float posY;
    float posZ;
};

struct ConstraintsStruct
{
    
    float MIN_angolo1_indice;
    float MIN_angolo2_indice;
    float MIN_angolo3_indice;
    
    float MIN_angolo1_medio;
    float MIN_angolo2_medio;
    float MIN_angolo3_medio;
    
    float MIN_angolo1_anulare;
    float MIN_angolo2_anulare;
    float MIN_angolo3_anulare;
    
    float MIN_angolo1_mignolo;
    float MIN_angolo2_mignolo;
    float MIN_angolo3_mignolo;
    
    float MIN_angolo1_pollice;
    float MIN_angolo2_pollice;
    float MIN_angolo3_pollice;
    
    float MIN_rotX;
    float MIN_rotY;
    float MIN_rotZ;
    
    float MIN_posX;
    float MIN_posY;
    float MIN_posZ;
    
    float MAX_angolo1_indice;
    float MAX_angolo2_indice;
    float MAX_angolo3_indice;
    
    float MAX_angolo1_medio;
    float MAX_angolo2_medio;
    float MAX_angolo3_medio;
    
    float MAX_angolo1_anulare;
    float MAX_angolo2_anulare;
    float MAX_angolo3_anulare;
    
    float MAX_angolo1_mignolo;
    float MAX_angolo2_mignolo;
    float MAX_angolo3_mignolo;
    
    float MAX_angolo1_pollice;
    float MAX_angolo2_pollice;
    float MAX_angolo3_pollice;
    
    float MAX_rotX;
    float MAX_rotY;
    float MAX_rotZ;
    
    float MAX_posX;
    float MAX_posY;
    float MAX_posZ;

    
};

class Hand
{
    
public:
    
    //constructor
    Hand(handState stato);      
    //destructor
    ~Hand();     
    //draw routine 
    void draw(); 
    
    void getState(handState* state);
    void setState(handState state);
    
    void randomUpdate(bool FLOAT);
    
private:
    
    //WHOLE HAND POSITION IN THE SPACE
    float _x;
    float _y;
    float _z;
    
    
    //Represents the state of all the fingers
    handState _hand;
    ConstraintsStruct _constraints;
    
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
