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

Hand::Hand(handState stato)
{
    //STATO DI INIT
    
    _hand=stato;
    
    //VINCOLI CINEMATICI
    {
    _constraints.MIN_angolo1_indice=0;
    _constraints.MIN_angolo2_indice=0;
    _constraints.MIN_angolo3_indice=0;
    
    _constraints.MIN_angolo1_medio=0;
    _constraints.MIN_angolo2_medio=0;
    _constraints.MIN_angolo3_medio=0;
    
    _constraints.MIN_angolo1_anulare=0;
    _constraints.MIN_angolo2_anulare=0;
    _constraints.MIN_angolo3_anulare=0;
    
    _constraints.MIN_angolo1_mignolo=0;
    _constraints.MIN_angolo2_mignolo=0;
    _constraints.MIN_angolo3_mignolo=0;
    
    _constraints.MIN_angolo1_pollice=0;
    _constraints.MIN_angolo2_pollice=0;
    _constraints.MIN_angolo3_pollice=0;
    
    _constraints.MIN_rotX=-90;
    _constraints.MIN_rotY=-90;
    _constraints.MIN_rotZ=-90;
    
    _constraints.MIN_posX=-2;
    _constraints.MIN_posY=-2;
    _constraints.MIN_posZ=-2;
    
    _constraints.MAX_angolo1_indice=90;
    _constraints.MAX_angolo2_indice=90;
    _constraints.MAX_angolo3_indice=90;
    
    _constraints.MAX_angolo1_medio=90;
    _constraints.MAX_angolo2_medio=90;
    _constraints.MAX_angolo3_medio=90;
    
    _constraints.MAX_angolo1_anulare=90;
    _constraints.MAX_angolo2_anulare=90;
    _constraints.MAX_angolo3_anulare=90;
    
    _constraints.MAX_angolo1_mignolo=90;
    _constraints.MAX_angolo2_mignolo=90;
    _constraints.MAX_angolo3_mignolo=90;
    
    _constraints.MAX_angolo1_pollice=90;
    _constraints.MAX_angolo2_pollice=90;
    _constraints.MAX_angolo3_pollice=90;
    
    _constraints.MAX_rotX=90;
    _constraints.MAX_rotY=90;
    _constraints.MAX_rotZ=90;
    
    _constraints.MAX_posX=2;
    _constraints.MAX_posY=2;
    _constraints.MAX_posZ=2;
    }
}

Hand::~Hand()
{
    
}

//GETTER
void Hand::getState(handState* state)
{
    state=&_hand;
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
void Hand::setState(handState state)
{
    _hand=state;
}


void Hand::randomUpdate(bool FLOAT)
{
    
    if(!FLOAT)
    {
        _hand.angolo1_indice=(rand()%(int)(_constraints.MAX_angolo1_indice-_constraints.MIN_angolo1_indice));
        _hand.angolo2_indice=(rand()%(int)(_constraints.MAX_angolo2_indice-_constraints.MIN_angolo2_indice));
        _hand.angolo3_indice=(rand()%(int)(_constraints.MAX_angolo3_indice-_constraints.MIN_angolo3_indice));
        
        _hand.angolo1_medio=(rand()%(int)(_constraints.MAX_angolo1_medio-_constraints.MIN_angolo1_medio));
        _hand.angolo2_medio=(rand()%(int)(_constraints.MAX_angolo2_medio-_constraints.MIN_angolo2_medio));
        _hand.angolo3_medio=(rand()%(int)(_constraints.MAX_angolo3_medio-_constraints.MIN_angolo3_medio));
        
        _hand.angolo1_anulare=(rand()%(int)(_constraints.MAX_angolo1_anulare-_constraints.MIN_angolo1_indice));
        _hand.angolo2_anulare=(rand()%(int)(_constraints.MAX_angolo2_anulare-_constraints.MIN_angolo2_anulare));
        _hand.angolo3_anulare=(rand()%(int)(_constraints.MAX_angolo3_anulare-_constraints.MIN_angolo3_anulare));
        
        _hand.angolo1_mignolo=(rand()%(int)(_constraints.MAX_angolo1_mignolo-_constraints.MIN_angolo1_mignolo));
        _hand.angolo2_mignolo=(rand()%(int)(_constraints.MAX_angolo2_mignolo-_constraints.MIN_angolo2_mignolo));
        _hand.angolo3_mignolo=(rand()%(int)(_constraints.MAX_angolo3_mignolo-_constraints.MIN_angolo3_mignolo));
        
        _hand.angolo1_pollice=(rand()%(int)(_constraints.MAX_angolo1_pollice-_constraints.MIN_angolo1_pollice));
        _hand.angolo2_pollice=(rand()%(int)(_constraints.MAX_angolo2_pollice-_constraints.MIN_angolo2_pollice));
        _hand.angolo3_pollice=(rand()%(int)(_constraints.MAX_angolo3_pollice-_constraints.MIN_angolo3_pollice));
        
        _hand.rotX=(rand()%(int)(_constraints.MAX_rotX-_constraints.MIN_rotX));
        _hand.rotY=(rand()%(int)(_constraints.MAX_rotY-_constraints.MIN_rotY));
        _hand.rotZ=(rand()%(int)(_constraints.MAX_rotZ-_constraints.MIN_rotZ));
        
        _hand.posX=(rand()%(int)(_constraints.MAX_rotX-_constraints.MIN_rotX));
        _hand.posY=(rand()%(int)(_constraints.MAX_rotY-_constraints.MIN_rotY));
        _hand.posZ=(rand()%(int)(_constraints.MAX_rotZ-_constraints.MIN_rotZ));
    }
    
    
    if(FLOAT)
    {
        std::cout<<"NOT YET"<<std::endl;        
    }

}

void Hand::draw()
{
    glPushMatrix();
    
    glTranslatef(_hand.posX, _hand.posY, _hand.posZ);
    glRotatef(_hand.rotX, 1, 0, 0);
    glRotatef(_hand.rotY, 0, 1, 0);
    glRotatef(_hand.rotZ, 0, 0, 1);    
    
    
    glPushMatrix();
    glScalef(1.0,1.0,0.6);
    this->_finger(-1,0,-0.1,0.8f,0.2f,_hand.angolo1_indice,_hand.angolo2_indice,_hand.angolo3_indice);
    glPopMatrix();
    
    glPushMatrix();
    glScalef(1.0,1.0,0.9);
    this->_finger(-0.5,0,0.1,0.8f,0.2f,_hand.angolo1_medio,_hand.angolo2_medio,_hand.angolo3_medio);
    glPopMatrix();
    
    glPushMatrix();
    glScalef(1.0,1.0,0.8);
    this->_finger(0,0,0.1,0.8f,0.2f,_hand.angolo1_anulare,_hand.angolo2_anulare,_hand.angolo3_anulare);
    glPopMatrix();
    
    glPushMatrix();
    glScalef(1.0,1.0,0.7);
    this->_finger(0.5,0,-0.1,0.8f,0.2f,_hand.angolo1_mignolo,_hand.angolo2_mignolo,_hand.angolo3_mignolo);
    glPopMatrix();
    
    this->_thumb(0.35, -0.5, -0.3,0.4f,0.2f,_hand.angolo1_pollice,_hand.angolo2_pollice,_hand.angolo3_pollice);
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
