//
//  Hand.cpp
//  openni-hand-tracker
//
//  Created by maurilio di cicco on 22/05/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "Hand.h"

//-------------------------------------------------------------------------------------------------------------------------------------
float RandomFloat(float min, float max)
{
    // this  function assumes max > min, you may want 
    // more robust error checking for a non-debug build
    assert(max > min); 
    float random = ((float) rand()) / (float) RAND_MAX;
    
    // generate (in your case) a float between 0 and (4.5-.78)
    // then add .78, giving you a float between .78 and 4.5
    float range = max - min;  
    return (random*range) + min;
}

//-------------------------------------------------------------------------------------------------------------------------------------

Hand::Hand(float* stato)
{
    //STATO DI INIT
    
    for(int i=0;i<HAND_DIM;i++)
    {
        _hand[i]=stato[i];
    }
    
    //VINCOLI CINEMATICI
    {
    Constraints[0][0]=0;
    Constraints[1][0]=0;
    Constraints[2][0]=0;
        
    Constraints[3][0]=0;
    Constraints[4][0]=0;
    Constraints[5][0]=0;
    
    Constraints[6][0]=0;
    Constraints[7][0]=0;
    Constraints[8][0]=0;
    
    Constraints[9][0]=0;
    Constraints[10][0]=0;
    Constraints[11][0]=0;
    
    Constraints[12][0]=0;
    Constraints[13][0]=0;
    Constraints[14][0]=0;
    
    Constraints[15][0]=-90;
    Constraints[16][0]=-90;
    Constraints[17][0]=-90;
    
    Constraints[18][0]=-20;
    Constraints[19][0]=-20;
    Constraints[20][0]=-20;
    
    Constraints[0][1]=90;
    Constraints[1][1]=90;
    Constraints[2][1]=90;
    
    Constraints[3][1]=90;
    Constraints[4][1]=90;
    Constraints[5][1]=90;
    
    Constraints[6][1]=90;
    Constraints[7][1]=90;
    Constraints[8][1]=90;
    
    Constraints[9][1]=90;
    Constraints[10][1]=90;
    Constraints[11][1]=90;
    
    Constraints[12][1]=90;
    Constraints[13][1]=90;
    Constraints[14][1]=90;
    
    Constraints[15][1]=90;
    Constraints[16][1]=90;
    Constraints[17][1]=90;
    
    Constraints[18][1]=20;
    Constraints[19][1]=20;
    Constraints[20][1]=20;
    }
}

Hand::~Hand()
{
    
}

//GETTER
void Hand::getState(float* state)
{
    for(int i=0;i<HAND_DIM;i++)
    {
        *state=_hand[i];
        state++;
    }
}

//SETTER (SPANIEL)
/*
 
                                                                                                             
             ,''';:..`                                                                                      
           .+++'''';,;,`                                                                                    
          `'+';'';;;,,,;.                                                                                   
  ..,,,::',+';::;,,:;;;'+                                                                                   
 :,'+';;;';;#:,;+';''''';                                                                                   
 @++'';;;;::;++++++'';'',.                                                                                  
 ';++';;;::''#+####'''':';`                                                                                 
 `'';''';';;+#'+##;',+:;;';                                                                                 
  ;'';;+++';:+'##;';;':;+::,                                                                                
   ,#+''+####'+'''',;::'',;+`                                                                               
    ,+#++'####@##+;,',;:';:;+                                                                               
  ,``';;##:+++###'.';:,:'''''`                                                                              
   `,.:+'+''+++#@#;'',:;+''',+                                                                              
   `##';`;#+++#@##'+':,:'++:;,`                                                                             
    :++;#+'+####@#;#'+:+;'+';:,                                                                             
      .:;:.   @#@++;';,:##;+;;;.                                                                            
              ,#@+++';;;',++;,;':                                                                           
               +##+:':..++##'::+'',                                                                         
                +';:+:.,'##+';:+++';:,                                                                      
                 ';:',:,'+'+';'+++'''''.                                                                    
                 ;:;,:::,''+;+'''''''+++;.                                                                  
                  ';;,'#+':';:'''''+''++'''',`                                                              
                  +::'##''+::';;;;';'''+''''''';,`                                                          
                  :',@##+:;:';';;;::;''';'''''++'''':,.                                                     
                   ######''::,;';:::;;;;;;'''''+''+++++++';;::,,.```                                        
                   ,###+##++::;';:::::::;:::;;''+'+++++++++''+'''';;;'';:`                                  
                    #####+++'''';;,::;;,,,,:;;'''''+'''++++'+'+'''';;;;''':.                                
                    '##'++#+''+'+:;''';,:,,,,:';'''''+'''+++++''+''''';'+'';',                              
                    ,+';;+#'+#+'''''';:;::;;,:::;;;';'''''+''''''';;'';'++'';;:,                            
                    .+'::;'+'#'++++'':'';;;;:::::;:;:;::'';;;;;+;:;;;;;''''''',:'.                          
                   ;+'';+++++'++'++'';+'''';;;;;::;;;:':;;::;;;;;;;;::::;;';;';:,'.                         
                  `'''++++++'+'++++''++'';;;',;;;;;;'''';''++:;'''''';;:::;;;';';;+:                        
                  ;''+##+++'';+'+++'++++'::'''''':''''++++++++'+++#+++';;;::;;;:''++;                       
                 .'++####++':+++#++++#'+;;''''+++++++'+##+######+###+++'';:;:;:,'''+;',                     
                 `+######++;'++++++++#+;;'+'''++++++#++###############+++''+;:;:;;'+;':.                    
                 .+######+#+++##+++#+#+##+#++++#++++##########@########+++++':;;:;',+;;:                    
                 ,++#####+#++++++#+#+##@###+++####+#########@##++#+###++++++++'++''+#':;:                   
                 `+'#####++++++++#+##+#@#################@#@###########+#++++++++''''':''+.                 
                 .+'##@##++'+++++++#+#@####@################@#'++#++#####++++''+++''+;',:;:`                
                 ;';###+++''+'+++#+#+#@####@###+##########'+##+'++'++#####+++'+''+++'#+;:;;:                
                 +;'+#####+'+++++'+#+++##########@#####+#+'##++++++#+;;##++;;''++'''#+#;;:;;                
                 ''+;##+#+#'++++++##';''++#++@@##@####+#+++########@#++''+;'''++''''++#++'::`               
                 .+;+##+#+#'+++++++#;+'+;##@#####@#@####++#######@@@@##+'+##++++'''''++#+;;:`               
                  ''++#+##+''+++#++#@'++'############+#++########@@###########+''+''+'+,;,,;:;              
                  `;+++###+++#++++++@#+'+##@########++++#;+'#########@######+++''++++++',',;:;.             
                   `.''+####++#+++++###+'##########'+####++++#########@#####+++#+#';;;;+':;';;:,            
                     `'+###+.++#++++####''#+++#+##++#######+;++##############++'#':;::;';'';:';;.           
                      '+##++'++++++##++#'++++++###+########;;;'++#####@####+##+'+++'';:'':',;::;;           
                      :''##:+'#++#+#+++#+'+'+#+++##########++;:;'###'####''+#++++'''+';;;'';;:::;:          
                      `;++''+'''+'#+''+'++'+##+++#############'';####++##++'##''#;++++;:'',':;;::;;`        
                       ;''++++;'''';:+++#+++#+'#############+++++####++'##+##'++;+++++';';:;#';;;';:.       
                       ,''+;#';+,+':'+###+++##'+#+########+''+';+#####+#+##+'#+;+''#++';';';;:';;;;,;,      
                       .'+;'##:'+'';'##'#+'+++##+#####+#++';;+;,'+####+#'##++'+++'++++';;'+;;+;+':;',+:     
                       `;'++++''+#+;;;';++++++#++###++''++';:;,.,++#####++##+####''''+'+'+;;'';;;:;;;,+:`   
                        `'++'+'';+';,;;'++'+++#++++#+'':''+``..` +:+#############+''';+'';';++;';;;'';++:`  
                          ++++;:;'+':;,,+##+++++'':+'':;;`+````` `,'##############+''':'+;,'++''+''++';;::  
                          ;++++;''''+'','+++++++''::;;`.`.;..`    ,################+';;;';.`;''+++;+'+;+#;` 
                           +'++,;'++';';+++'+''''..`::    ;       ,#'#######+###++##+';:''   ;'+'+;':+';;`: 
                           ,++#+'+'';';:;+'''',;:.  ``    .       ,,+########:'##+'+++::::   `:+:;;:::`     
                            ++++'''+'':;::;;.;.,.`        `       .`+;++######'+###'+;':,;`   .,`.,`        
                            #+++++''+',';;:: :``                    '`#:'##########++;':::,`   `            
                            ++#++'++;'+''''` `                      ; ',,'###########'+:':::                
                            ###++';'';++;;;.                       ., :.:,'+#########+#;;+:;,.              
                            #+#++'';'''+'''.                      `.  ,.`.`####+########+;+:;:;.            
                            ###'+':+;;;';::`                         ``. ` +###++#++''###';':;:;;           
                            ###++'''+;:';.                           .   ` ;+###+#+#+,,+##''+;+;+           
                            ####+'';;:;;;`                                 :'######:..::'#++''++'           
                            @##+''';'+'+'`                                 .:+###+#  .,;,;#+':'+'           
                            ;##+';':'''+;`                                  .'##+#:  ``'` ++##+++`          
                            :#@#;;+;:;'':`                                   ,####     `` ;##+';+`          
                            :###;;':'''';`                                   ####`    `   ,:++'''           
                            ,##'';#;';;''.                                   ###+         . `++++`          
                            +##;';#':''''.                                  +###          `  ++''           
                            #+#';;#::;';,,                               ` :+##.             +'''           
                           `#++;:'+;;+::,.                            `;#++++##              +;';           
                        `'+###+;;'#;:';''                            .;+'+####.              +;':           
                        ;+#+##',:'++';+;;                            ++++####:               +:;:           
                       :++##+;;:'+.++;;;.                         ```,:;+@+;               .:';',           
                    ``.,;':,:,;;+',;':::                                                 .,::+:;,           
                         ,,:,.,:++;;'++;                                                `.'.,,,::           
                         .'::::,': .,.                                                  `:,:';:,            
                      `.........`                                                     `........`            
                                                                                                            

 
 
 */

void Hand::setState(float* state)
{    
    for(int i=0;i<HAND_DIM;i++)
    {
        _hand[i]=state[i];
    }
}


void Hand::draw()
{
    glPushMatrix();
    
    glTranslatef(_hand[posX], _hand[posY], _hand[posZ]); //12=posX 13=posY 14=posZ
    glRotatef(_hand[angoloX], 1, 0, 0);
    glRotatef(_hand[angoloY], 0, 1, 0);
    glRotatef(_hand[angoloZ], 0, 0, 1);
    
    
    glPushMatrix();
    glScalef(1.0,1.0,0.6);
    this->_finger(-1,0,-0.1,0.8f,0.2f,_hand[indice1],_hand[indice2],_hand[indice3]);
    glPopMatrix();
    
    glPushMatrix();
    glScalef(1.0,1.0,0.9);
    this->_finger(-0.5,0,0.1,0.8f,0.2f,_hand[medio1],_hand[medio2],_hand[medio3]);
    glPopMatrix();
    
    glPushMatrix();
    glScalef(1.0,1.0,0.8);
    this->_finger(0,0,0.1,0.8f,0.2f,_hand[anulare1],_hand[anulare2],_hand[anulare3]);
    glPopMatrix();
    
    glPushMatrix();
    glScalef(1.0,1.0,0.7);
    this->_finger(0.5,0,-0.1,0.8f,0.2f,_hand[mignolo1],_hand[mignolo2],_hand[mignolo3]);
    glPopMatrix();
    
    this->_thumb(0.35, -0.5, -0.3,0.4f,0.2f,_hand[pollice1],_hand[pollice2],_hand[pollice3]);
    this->_palm(-0.25,0,-1.3);
    
    glPopMatrix();
}

void Hand::_palm(float offsetx,float offsety,float offsetz)
{
    
    float lunghezza_palmo=1.0f;
    float larghezza_palmo=1.0f;
    float altezza_palmo=0.3f;   
    
    glPushMatrix();
    
    glTranslatef(offsetx, offsety, offsetz);
    
    glPushMatrix();
    glColor3f(1, 1, 0);
    GLUquadric* quad=  gluNewQuadric(); 
    glScalef(larghezza_palmo, altezza_palmo, 1.0f);
    gluCylinder(quad, 1, 1.0f, lunghezza_palmo, 20, 20);
    glPopMatrix();
    
    glPushMatrix();
    glColor3f(1, 0, 0);
    glScalef(larghezza_palmo, altezza_palmo, 0.3f);    
    glutSolidSphere(1, 20, 20);
    glPopMatrix();
    
    glPushMatrix();
    glColor3f(1, 0, 0);
    glTranslatef(0,0,lunghezza_palmo);
    glScalef(larghezza_palmo, altezza_palmo, 0.3f);    
    glutSolidSphere(1, 20, 20);
    glPopMatrix();
    
    glPopMatrix();
    
}

void Hand::_finger(float offsetx,float offsety, float offsetz,float lunghezza_f,float articolazione,int angolo1,int angolo2,int angolo3)
{
    float ratio=0.6f;
    
    //PRIMA ARTICOLAZIONE INDICE
    glPushMatrix();
    glColor3f(0, 1, 0);
    glTranslatef(offsetx, 0, offsetz);
    glutSolidSphere(articolazione,10,10);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0, 0, offsetz);
    glRotatef(angolo1, 1, 0, 0);
    glTranslatef(0, 0, -offsetz);
    
    //PRIMA FALANGE INDICE
    glPushMatrix();
    glColor3f(0, 0, 1);
    glTranslatef(offsetx, 0, offsetz);
    GLUquadric* ffi=  gluNewQuadric(); 
    gluCylinder(ffi, articolazione, articolazione, lunghezza_f, 20, 20);
    glPopMatrix();    
    
    //SECONDA ARTICOLAZIONE INDICE
    glPushMatrix();
    glColor3f(0, 1, 0);
    glTranslatef(offsetx, 0, offsetz+lunghezza_f);
    glutSolidSphere(articolazione,10,10);
    glPopMatrix();
    
    
    //SECONDA FALANGE INDICE
    glPushMatrix();
    glColor3f(0, 0, 1);
    glTranslatef(offsetx, 0, offsetz+lunghezza_f);
    glRotatef(angolo2, 1, 0, 0);
    GLUquadric* sfi=  gluNewQuadric(); 
    gluCylinder(sfi, articolazione, articolazione, lunghezza_f, 20, 20);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0,0,lunghezza_f+offsetz);
    glRotatef(angolo2, 1, 0, 0);
    glTranslatef(0,0,-lunghezza_f-offsetz);
    
    //TERZA ARTICOLAZIONE INDICE
    glPushMatrix();
    glColor3f(0, 1, 0);
    glTranslatef(offsetx, 0, offsetz+lunghezza_f+lunghezza_f);
    glutSolidSphere(articolazione,10,10);
    glPopMatrix();
    
    //TERZA FALANGE INDICE
    glPushMatrix();
    glColor3f(0, 0, 1);
    glTranslatef(offsetx, 0, offsetz+lunghezza_f+lunghezza_f);
    glRotatef(angolo3, 1, 0, 0);
    GLUquadric* tfi=  gluNewQuadric(); 
    gluCylinder(tfi, articolazione, articolazione, lunghezza_f*ratio, 20, 20);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0,0,lunghezza_f+lunghezza_f+offsetz);
    glRotatef(angolo3, 1, 0, 0);
    glTranslatef(0,0,-lunghezza_f-lunghezza_f-offsetz);
    
    //TAPPO INDICE
    glPushMatrix();
    glColor3f(0, 1, 0);
    glTranslatef(offsetx, 0, offsetz+lunghezza_f+lunghezza_f+lunghezza_f*ratio);
    glutSolidSphere(articolazione,10,10);
    glPopMatrix();
    glPopMatrix();
    
    glPopMatrix();
    
    glPopMatrix();
}

void Hand::_thumb(float offsetx,float offsety, float offsetz,float lunghezza_f,float articolazione,int angolo1,int angolo2,int angolo3)
{
    glPushMatrix();
    glColor3f(1, 0, 0);
    glTranslatef(0.5, -0.4, -0.8);
    glRotatef(30, 0, 1, 0);
    glScalef(0.4, 0.4, 0.6);
    glutSolidSphere(1, 10, 10);
    glPopMatrix();
    
    
    this->_thumbFinger(offsetx, offsety, offsetz,lunghezza_f,articolazione,angolo1,angolo2,angolo3);
}


void Hand::_thumbFinger(float offsetx,float offsety, float offsetz,float lunghezza_f,float articolazione,int angolo1,int angolo2,int angolo3)
{
    //PRIMA ARTICOLAZIONE INDICE
    glPushMatrix();
    
    glTranslatef(offsetx, offsety, offsetz);
    glRotatef(-90, 0, 0, 1);
    glTranslatef(-offsetx, -offsety, -offsetz);
    
    
    //PRIMA ARTICOLAZIONE INDICE
    glPushMatrix();
    glColor3f(0, 1, 0);
    glTranslatef(offsetx, 0, offsetz);
    glutSolidSphere(articolazione,10,10);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0, 0, offsetz);
    glRotatef(angolo1, 1, 0, 0);
    glTranslatef(0, 0, -offsetz);
    
    //PRIMA FALANGE INDICE
    glPushMatrix();
    glColor3f(0, 0, 1);
    glTranslatef(offsetx, 0, offsetz);
    GLUquadric* ffi=  gluNewQuadric(); 
    gluCylinder(ffi, articolazione, articolazione, lunghezza_f, 20, 20);
    glPopMatrix();    
    
    //SECONDA ARTICOLAZIONE INDICE
    glPushMatrix();
    glColor3f(0, 1, 0);
    glTranslatef(offsetx, 0, offsetz+lunghezza_f);
    glutSolidSphere(articolazione,10,10);
    glPopMatrix();
    
    
    //SECONDA FALANGE INDICE
    glPushMatrix();
    glColor3f(0, 0, 1);
    glTranslatef(offsetx, 0, offsetz+lunghezza_f);
    glRotatef(angolo2, 1, 0, 0);
    GLUquadric* sfi=  gluNewQuadric(); 
    gluCylinder(sfi, articolazione, articolazione, lunghezza_f, 20, 20);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0,0,lunghezza_f+offsetz);
    glRotatef(angolo2, 1, 0, 0);
    glTranslatef(0,0,-lunghezza_f-offsetz);
    
    //TERZA ARTICOLAZIONE INDICE
    glPushMatrix();
    glColor3f(0, 1, 0);
    glTranslatef(offsetx, 0, offsetz+lunghezza_f+lunghezza_f);
    glutSolidSphere(articolazione,10,10);
    glPopMatrix();
    
    
    glPopMatrix();
    
    glPopMatrix();
    
    
    glPopMatrix();
}
