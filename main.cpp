
#include "main.h"

using namespace xn;

int kinectCNT;
int shaderCNT;

//NOTA: TUTTE LE DICHIARAZIONI SONO NEL .h

#pragma MARK WINDOW 1

void reshape(int width, int height) {
    
   	GLfloat fieldOfView = 45.0f;
	glViewport (0, 0, (GLsizei) width, (GLsizei) height);	
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fieldOfView, (GLfloat) width/(GLfloat) height, 0.1, 500.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
    
}

void draw() {
    
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
    gluLookAt(0, 0, -(1000*0.8/1000),		
              0, 0, 1,		
              0, 1, 0);	    

    
    glRotatef(spin_y, 1.0, 0.0, 0.0);
    glRotatef(spin_x, 0.0, 1.0, 0.0);


    DrawAxes(10);
    
    {
    const GLchar *vertex_shader[] = {
        "void main(void) {\n",
        "    gl_Position = ftransform();\n",
        "    gl_FrontColor = gl_Color;\n",
        "}"
    };
    
    const GLchar *color_shader[] = {
        "void main() {\n",
        "   float f =  gl_FragCoord.w;\n",
        "   gl_FragColor = vec4(f, f, f, 1.0);\n",
        "}"
    };
    
    static shader_prog prog(vertex_shader, color_shader);
    //prog(); 
    }
    
    glPushMatrix();
    glScalef(0.15, 0.15, 0.15);
    fullhand(0,0,0,bestparticle.x,bestparticle.y,bestparticle.z);
    glPopMatrix();
    
    {

      glReadPixels(0, 0, DIMX, DIMY, GL_DEPTH_COMPONENT, GL_FLOAT, texMap); 

    }
    
    //std::cout << "SHADER: "<<shaderCNT<< "       KINECT: "<<kinectCNT<<"        DIFF: "<<shaderCNT-kinectCNT <<std::endl;
    
    glFlush();
    glutSwapBuffers();
    glutPostRedisplay();
}

#pragma MARK WINDOW 2
//DISEGNO LA SECONDA FINESTRA (QUELLA DELLO SHADER)
void draw2(){
    
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
    
    glOrtho(0.0, 5, 0.0, 5, -1.0, 1.0);
    
    glBindTexture (GL_TEXTURE_2D, 13);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);       
    
    shaderCNT=0;
    
    for(int i=0;i<DIMX*DIMY;i++)
    {
        if(texMap[i]==1)
        {
         texMap[i]=0;
        }
        else
        {
            shaderCNT++;
        }
        texMap[i]=texMap[i];
    }
    
    /*
    std::cout<<"CENTRO ALTO  : "<<texMap[DIMX*(DIMY-1)+DIMX/2] << std::endl;
    std::cout<<"CENTRO BASS0 : "<<texMap[DIMX/2] << std::endl;
    */
    
    //-------TEST     
    
    
    int acc=0;
    
    for(int incX=DIMY;incX>0;incX--)
    {
        for(int incY=1;incY<=DIMX;incY++,acc++)
        {
            float tmp=texMap[acc];
            texArranged[( (incX-1) *DIMX)+incY ]=tmp;
            //std::cout << acc <<std::endl;
        }
        //std::cout << "->" <<std::endl;
    }
    
    /*
    std::cout<<"arr UP: "<<texArranged[DIMX/2] << std::endl;
    std::cout<<"arr DW: "<<texArranged[DIMX*(DIMY-1)+DIMX/2] << std::endl;
    
    
    std::cout<<"tex UP: "<< texMap[DIMX*(DIMY-1)+DIMX/2]<< std::endl;
    std::cout<<"tex DW: "<< texMap[DIMX/2] << std::endl;
    */
    
    //-------TEST
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, DIMX, DIMY, 0, GL_DEPTH_COMPONENT, GL_FLOAT, (GLvoid*)texArranged);
    
    glColor4f(1, 1, 1,1);
    glEnable(GL_TEXTURE_2D);

    glBindTexture (GL_TEXTURE_2D, 13);
    glBegin (GL_QUADS);
    glTexCoord2f (0.0, 0.0);
    glVertex2f (0.0, 0.0);
        glTexCoord2f (1, 0.0);
    glVertex2f (5.0, 0.0);
        glTexCoord2f (1.f, 1.f);
    glVertex2f (5.0, 5.0);
        glTexCoord2f (0.0, 1);
    glVertex2f (0.0, 5.0);
    glEnd ();    
    glDisable(GL_TEXTURE_2D);
    
    
    //PUNTO ROSSO DI COORDINATE (WIDTH/2,0)
    glPointSize(2.f);    
    glBegin(GL_POINTS);
    glColor3f(1.f, 0.f, 0.f);    

    glVertex3f(2.5f,0.f,0.f);    
    glEnd();
    

    
    
    
    glFlush();
    glutSwapBuffers();
    glutPostRedisplay();
}

#pragma MARK WINDOW 3
//DISEGNO LA TERZA FINESTRA (QUELLA DEL KINECT)
void draw3()
{
	XnStatus rc = XN_STATUS_OK;
	rc = g_context.WaitAnyUpdateAll();
    
    /*
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
    gluLookAt(0, 0, -(MAX_DEPTH*0.2/1000),0, 0, 1,0, 1, 0);	

    
    glPushMatrix();
    glTranslatef(0.0, 0.0, -(MAX_DEPTH/2000));
    DrawAxes(2.0);
    glPopMatrix();
    */
	if (rc != XN_STATUS_OK) {
		glutSwapBuffers();
		return;
	}
    
	g_depth.GetMetaData(g_depthMD);
	
	 
    const XnDepthPixel* pDepth = g_depthMD.Data();
    
    /*
    glEnable(GL_POINT_SMOOTH);
    glBegin(GL_POINTS);
    */
    
    XnDouble d;
    XnDouble color;
    XnDouble xc;
    XnDouble yc;
    XnDouble zc;    
    
    int knct=0;
    kinectCNT=0;
    
    for (XnUInt y = 0; y < g_depthMD.YRes(); y++)
	{
        
		for (XnUInt x = 0; x < g_depthMD.XRes(); x++, pDepth++,knct++)
		{
            
            // Take the distance data for the actual pixel
            d = *pDepth;
            
            kinectMap[knct]=0;
            if(d != 0 && d<800) 
            {
                kinectMap[knct]=1;
                /*
                color = (1.0-(1.0/d));
                
                xc = (((float)x - cx_d)*(d/1000))/(fx_d);
                yc = -(((float)y - cy_d)*(d/1000))/(fy_d);
                zc = d/1000;
                glColor3f(color, color, color);
                glVertex3f(xc, yc, zc-(MAX_DEPTH*0.8/2000));
                
                kinectCNT++;
                */
                
            }
                        
        }
        
        
    }
    
    //glReadPixels(0, 0, DIMX, DIMY, GL_RGB, GL_FLOAT, kinectMap); 
    
    /*
    std::cout<<"k CENTRO ALTO  : "<<pDepth[DIMX/2] << std::endl;
    std::cout<<"k CENTRO BASS0 : "<<pDepth[DIMX*(DIMY-1)+DIMX/2] << std::endl;
    */
    
    int errfun=0;
    
    for(int i=0;i<DIMX*DIMY;i++)
    {
        if(kinectMap[i]!=0 && texArranged[i]!=0) errfun++;
    }
          
    std::cout<< "ERR: "<<errfun<<std::endl;
    
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
    
    glOrtho(0.0, 5, 0.0, 5, -1.0, 1.0);
    
    glBindTexture (GL_TEXTURE_2D, 13);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);    
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, DIMX, DIMY, 0, GL_DEPTH_COMPONENT, GL_FLOAT, (GLvoid*)kinectMap);
    
    glColor4f(1, 1, 1,1);
    glEnable(GL_TEXTURE_2D);
    
    glBindTexture (GL_TEXTURE_2D, 13);
    glBegin (GL_QUADS);
    glTexCoord2f (0.0, 0.0);
    glVertex2f (0.0, 0.0);
    glTexCoord2f (1, 0.0);
    glVertex2f (5.0, 0.0);
    glTexCoord2f (1.f, 1.f);
    glVertex2f (5.0, 5.0);
    glTexCoord2f (0.0, 1);
    glVertex2f (0.0, 5.0);
    glEnd ();    
    glDisable(GL_TEXTURE_2D);
    
    
    glEnd();
    glutSwapBuffers();
    glutPostRedisplay();
    
    
    /*
    int tmpMapcnt=0;
    
    for(int i=DIMY-1;i>=0;i--)
    {
        for(int j=0;j<=DIMX-1;j++)
        {
            tmpMap[tmpMapcnt]=texMap[i*j+j];
            tmpMapcnt++;
        }
    }
    */
    /*
    PixelErrorSum=0;
    for( int i=0;i<DIMX*DIMY;i++)
    {
        if(kinectMap[i]>=500 & kinectMap[i]<=700 & tmpMap[i]!=0) PixelErrorSum++;
    }
    */
    //std::cout<<"CNT: "<<PixelErrorSum<<std::endl;
    
    
    //PSO
    
    
    if(PSO)
    {
        
        int iteration=0;
        
        while(absf(bestparticle.error)>0.001f && iteration<=MAXITERATION)
        {
            for(int i=0;i<PARTICLENUM;i++)
            {
                update_particle(&particlearray[i]);
                best_for_particle(&particlearray[i]);
                
            }
            
            best_particle_so_far();
            
            //std::cout<<"Minor errore in questa iterazione "<<bestparticle.error<<std::endl;
            //std::cout<<"VALORI "<<bestparticle.x<<"  "<<bestparticle.y<<"  "<<bestparticle.z<<std::endl;
            iteration++;
        }
        
        std::cout<<"Soluzione raggiunta dopo "<<iteration<<" iterazioni con "<< PARTICLENUM <<" particelle"<<std::endl;
        std::cout<<"Soluzione "<<bestparticle.x<<" "<<bestparticle.y<<" "<<bestparticle.z<<std::endl;
        
    }
    
    
}

void reshapeKinect(int width, int height) 
{
	GLfloat fieldOfView = 45.0f;
	glViewport (0, 0, (GLsizei) width, (GLsizei) height);	
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fieldOfView, (GLfloat) width/(GLfloat) height, 0.1, 500.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
    
}





#pragma MARK MAIN

int main(int argc, char** argv) {
    
    srand ((unsigned int)time(NULL));
    
    kinect_fn(0);
    
    
    if(PSO)
    {
        
        int iteration=0;
        particleinit();
        
        while(absf(bestparticle.error)>0.001f && iteration<=MAXITERATION)
        {
            for(int i=0;i<PARTICLENUM;i++)
            {
                update_particle(&particlearray[i]);
                best_for_particle(&particlearray[i]);
                
            }
            
            best_particle_so_far();
            {
            //std::cout<<"Minor errore in questa iterazione "<<bestparticle.error<<std::endl;
            //std::cout<<"VALORI "<<bestparticle.x<<"  "<<bestparticle.y<<"  "<<bestparticle.z<<std::endl;
            }
            iteration++;
        }
        
        std::cout<<"Soluzione raggiunta dopo "<<iteration<<" iterazioni con "<< PARTICLENUM <<" particelle"<<std::endl;
        std::cout<<"Soluzione "<<bestparticle.x<<" "<<bestparticle.y<<" "<<bestparticle.z<<std::endl;
    
    }
    
    
    glutInit(&argc, argv);
    
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Hand view (master)");
    
    glutKeyboardFunc(keyboardDown);
    glutKeyboardUpFunc(keyboardUp);
    glutSpecialFunc(keyboardSpecialDown);
    glutSpecialUpFunc(keyboardSpecialUp);
    glutMouseFunc(mouseClick);
    glutMotionFunc(mouseMotion);
    glutReshapeFunc(reshape);
    glutDisplayFunc(draw);  
    glutIdleFunc(idle);
    glutIgnoreKeyRepeat(true); // ignore keys held down
        
    initGL(640, 480);
    
    
    {
    window2 = glutCreateWindow("Texel View (slave)");
    glutPositionWindow(740,100);
    //glutReshapeFunc(reshape);
    glutDisplayFunc(draw2);  
    glutIdleFunc(idle);
    initGL(640, 480);
    }
     
    
    
    window3 = glutCreateWindow("Kinect View");
    glutPositionWindow(50,580);
    //glutReshapeFunc(reshapeKinect);
    glutDisplayFunc(draw3);  
    glutIdleFunc(idle);
    initGL(640, 480);
    
    //right click menu
    {
    // create a sub menu 
    int subMenu = glutCreateMenu(menu);
    glutAddMenuEntry("Do nothing", 0);
    glutAddMenuEntry("Really Quit", 'q');
    
    // create main "right click" menu
    glutCreateMenu(menu);
    glutAddSubMenu("Sub Menu", subMenu);
    glutAddMenuEntry("Quit", 'q');
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    }
    
    
    glutMainLoop();
    return 0;
}


#pragma MARK PSO
void particleinit()
{
    for(int i=0;i<PARTICLENUM;i++)
    {
        
        
        particlearray[i].x=rand()%180-90; //inizializzo le posizioni a random
        particlearray[i].y=rand()%180-90;
        particlearray[i].z=rand()%180-90;  
        particlearray[i].index=i;
        
        particlearray[i].vx=0; //inizializzo le velocità a zero
        particlearray[i].vy=0;
        particlearray[i].vz=0;        
        
        particlearray[i].error=compute_error(&particlearray[i]); //calcolo la funzione di errore per la particella appena istanziata
        best_for_particle(&particlearray[i]); //Scelgo la posizione migliore raggiunta nella storia della particella

    }
        best_particle_so_far(); //scelgo la particella migliore tra quelle esistenti in questa iterazione salvando lo stato e l'errore
        
}

void best_particle_so_far()
{
    float currentmin=99999;
    int index=99999;
    
    for(int i=0;i<PARTICLENUM;i++)
    {
        if(abs(particlearray[i].error)<abs(currentmin))
        {
            currentmin=particlearray[i].error;
            index=i;
        }
        
    }
    
    bestparticle.x=particlearray[index].x;
    bestparticle.y=particlearray[index].y;
    bestparticle.z=particlearray[index].z;
    
    bestparticle.index=particlearray[index].index;
    
    bestparticle.error=currentmin;
    
    /*
    std::cout<<"La particella migliora è la [ "<<index<<" ] "<<"con una funzione di errore di: "<<compute_error(&particlearray[index])<<std::endl;
     */
}

void best_for_particle(particle *theparticle)
{
    float current_error=theparticle->error;
    
    float  new_error=compute_error(theparticle);
    
    //std::cout<<"OLD ERROR: "<<current_error<<" NEW ERROR: "<<new_error<<std::endl;
    
    if(new_error<=current_error)
    {
        
        theparticle->best[0]=theparticle->x;
        theparticle->best[1]=theparticle->y;
        theparticle->best[2]=theparticle->z; 
        theparticle->error=new_error;
        
        //std::cout<<"["<<theparticle->index<<"] error updated (old= "<<current_error<<" new= "<<new_error<<")"<<std::endl;    
    }
    
    else
    {
    //std::cout<<"["<<theparticle->index<<"] error NOT updated "<<std::endl;    
    }
    
    //std::cout<<"["<<theparticle->index<<"] = "<<theparticle->error<<std::endl;
    
    
}

void update_particle(particle *theparticle)
{
    float c1=2.8;
    float c2=1.3;
    float r1=(float)(rand()%1001)/1000;
    float r2=(float)(rand()%1001)/1000;
    
    //FORMULA
    //v(t+1)=v(t)+c1*R1(best_pos-current_pos)+c2*R2(global_best_pos-current_pos)
    //x(t+1)=x(t)+v(t+1);
    
    
    //std::cout<<"PARTICELLA ["<<theparticle->index<<"]"<<std::endl;
    //std::cout<<"OLD : "<<theparticle->x<<" # "<<theparticle->y<<" # "<<theparticle->z<<std::endl;
    //std::cout<<"BEST : "<<theparticle->best[0]<<" # "<<theparticle->best[1]<<" # "<<theparticle->best[2]<<std::endl;
    //std::cout<<"GLOBALBEST : "<<bestparticle.x<<" # "<<bestparticle.y<<" # "<<bestparticle.z<<std::endl;
    
    
    theparticle->vx=theparticle->vx+c1*r1*(theparticle->best[0]-theparticle->x)+c2*r2*(bestparticle.x-theparticle->x);
    theparticle->vy=theparticle->vy+c1*r1*(theparticle->best[1]-theparticle->y)+c2*r2*(bestparticle.y-theparticle->y);
    theparticle->vz=theparticle->vz+c1*r1*(theparticle->best[2]-theparticle->z)+c2*r2*(bestparticle.z-theparticle->z);  
    
    if(abs(theparticle->vx)>=VMAX){theparticle->vx=(theparticle->vx/absf(theparticle->vx))*VMAX;}; //recupero il segno e imposto la velocità massima
    if(abs(theparticle->vy)>=VMAX){theparticle->vy=(theparticle->vy/absf(theparticle->vy))*VMAX;}; //recupero il segno e imposto la velocità massima
    if(abs(theparticle->vz)>=VMAX){theparticle->vz=(theparticle->vz/absf(theparticle->vz))*VMAX;}; //recupero il segno e imposto la velocità massima    

    
    //std::cout<<"V : "<<theparticle->vx<<" # "<<theparticle->vy<<" # "<<theparticle->vz<<std::endl;
    
    theparticle->x=theparticle->x+theparticle->vx;
    theparticle->y=theparticle->y+theparticle->vy;
    theparticle->z=theparticle->z+theparticle->vz;
    
    if(abs(theparticle->x)>=LIMIT) {theparticle->x=theparticle->x/absf(theparticle->x)*LIMIT;};
    if(abs(theparticle->y)>=LIMIT) {theparticle->y=theparticle->y/absf(theparticle->y)*LIMIT;};
    if(abs(theparticle->z)>=LIMIT) {theparticle->z=theparticle->z/absf(theparticle->z)*LIMIT;};    
    
    //std::cout<<"NEW : "<<theparticle->x<<" # "<<theparticle->y<<" # "<<theparticle->z<<std::endl<<std::endl;;

}

float compute_error(particle *theparticle)
{
    /*
    //float err=(solution.x-theparticle->x+solution.y-theparticle->y+solution.z-theparticle->z)/3;
    float err= (    powf((solution.x-theparticle->x),2)+ 
                    powf((solution.y-theparticle->y),2)+
                    powf((solution.z-theparticle->z),2)
                ) *0.33333f;
    //std::cout<<"errore computato ="<<err<<std::endl;
    return err;
    */
    
    
    
    PixelErrorSum=0;
    for( int i=0;i<DIMX*DIMY;i++)
    {
        if(kinectMap[i]>=500 & kinectMap[i]<=700 & tmpMap[i]!=0) PixelErrorSum++;
    }
    
    
    return PixelErrorSum;
    
}

#pragma MARK HAND

void fullhand(float posx, float posy, float posz, float ax, float ay, float az)
{
    glPushMatrix();
    
        glTranslatef(posx, posy, posx);
        glRotatef(ax, 1, 0, 0);
        glRotatef(ay, 0, 1, 0);
        glRotatef(az, 0, 0, 1);    

    
        glPushMatrix();
        glScalef(1.0,1.0,0.6);
        finger(-1,0,-0.1,0.8f,0.2f,0,20,20);
        glPopMatrix();
        
        glPushMatrix();
        glScalef(1.0,1.0,0.9);
        finger(-0.5,0,0.1,0.8f,0.2f,0,20,20);
        glPopMatrix();
        
        glPushMatrix();
        glScalef(1.0,1.0,0.8);
        finger(0,0,0.1,0.8f,0.2f,0,20,20);
        glPopMatrix();
        
        glPushMatrix();
        glScalef(1.0,1.0,0.7);
        finger(0.5,0,-0.1,0.8f,0.2f,0,20,20);
        glPopMatrix();
        
        pollice(0.35, -0.5, -0.3,0.4f,0.2f,-30,40,40);
        hand(-0.25,0,-1.3);
    
    glPopMatrix();
}

void hand(float offsetx,float offsety,float offsetz)
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

void finger(float offsetx,float offsety, float offsetz,float lunghezza_f,float articolazione,int angolo1,int angolo2,int angolo3)
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

void pollice(float offsetx,float offsety, float offsetz,float lunghezza_f,float articolazione,int angolo1,int angolo2,int angolo3)
{
    glPushMatrix();
    glColor3f(1, 0, 0);
    glTranslatef(0.5, -0.4, -0.8);
    glRotatef(30, 0, 1, 0);
    glScalef(0.4, 0.4, 0.6);
    glutSolidSphere(1, 10, 10);
    glPopMatrix();
    
    
    pollice_dito(offsetx, offsety, offsetz,lunghezza_f,articolazione,angolo1,angolo2,angolo3);
}


void pollice_dito(float offsetx,float offsety, float offsetz,float lunghezza_f,float articolazione,int angolo1,int angolo2,int angolo3)
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

#pragma MARK USER INTERACTION FUNCTIONS

void menu(int op) {
    
    switch(op) {
        case 'Q':
        case 'q':
            exit(0);
    }
}

void keyboardDown(unsigned char key, int x, int y) {
    
    switch(key) {
        case 'Q':
        case 'q':
        case  27:   // ESC
            exit(0);
    }
}

void keyboardUp(unsigned char key, int x, int y) {
    
}

void keyboardSpecialDown(int k, int x, int y) {
    
}

void keyboardSpecialUp(int k, int x, int y) {
    
}

void mouseClick(int button, int state, int x, int y) 
{
	
    old_x = x;
    old_y = y;
    if(state==GLUT_UP)
    {
        oold_x = spin_x;
        oold_y = spin_y;
    }
    glutPostRedisplay();
    
}

void mouseMotion(int x, int y) 
{
    
    spin_x += x-old_x;
    spin_y += old_y-y;
    old_x = x;
    old_y = y;
    glutPostRedisplay();
    
}

#pragma MARK OPENGL STUFF

void DrawAxes(float length){
    /*
     glPushMatrix();
     glScalef(length, length, length);
     glLineWidth(2.f);
     glBegin(GL_LINES);
     // x red
     glColor3f(1.f, 0.f, 0.f);
     glVertex3f(0.f, 0.f, 0.f);
     glVertex3f(0.5f, 0.f, 0.f);
     // y green
     glColor3f(0.f, 1.f, 0.f);
     glVertex3f(0.f, 0.f, 0.f);
     glVertex3f(0.f, 0.5f, 0.f);
     // z blue
     glColor3f(0.f, 0.f, 1.f);
     glVertex3f(0.f, 0.f, 0.f);
     glVertex3f(0.f, 0.f, 0.5f);
     glEnd();
     glLineWidth(1.f);
     glPopMatrix();
     */
}

void idle() { 
    
}

void initGL(int width, int height) {
    
    GLfloat light_ambient_0[] = {0.65, 0.65, 0.65, 1.0};
	GLfloat light_diffuse_0[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat light_specular_0[] = {1.0, 1.0, 1.0, 1.0};	
	GLfloat light_position_0[] = {5.0, 5.0, 0.0, 0.0};
	glLightfv (GL_LIGHT0, GL_AMBIENT,	light_ambient_0);
	glLightfv (GL_LIGHT0, GL_DIFFUSE,	light_diffuse_0);
	glLightfv (GL_LIGHT0, GL_SPECULAR,	light_specular_0);
	glLightfv (GL_LIGHT0, GL_POSITION,	light_position_0);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
    
}

float absf(float num)
{
    if(num>0) return num;
    else return -num;
}

#pragma MARK KINECT INIT

int kinect_fn(int ID){
    XnStatus rc;
	EnumerationErrors errors;
    rc = g_context.InitFromXmlFile(SAMPLE_XML_PATH, g_scriptNode, &errors);
	if (rc == XN_STATUS_NO_NODE_PRESENT)
	{
		XnChar strError[1024];
		errors.ToString(strError, 1024);
		printf("%s\n", strError);
		exit(1);
	}
	else if (rc != XN_STATUS_OK)
	{
		printf("Open failed: %s\n", xnGetStatusString(rc));
		exit(1);
	}
	rc = g_context.FindExistingNode(XN_NODE_TYPE_DEPTH, g_depth);
	if (rc != XN_STATUS_OK)
	{
		printf("No depth node exists! Check your XML.");
		exit(1);
	}
    
    rc = g_context.FindExistingNode(XN_NODE_TYPE_IMAGE, g_image);
	if (rc != XN_STATUS_OK)
	{
		printf("No image node exists! Check your XML.");
		return 1;
	}
    
    
	g_depth.GetMetaData(g_depthMD);
    g_image.GetMetaData(g_imageMD);
    
    return 0;
}

