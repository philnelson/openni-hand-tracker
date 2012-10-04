/*
 * Palm.cpp
 *
 *  Created on: Oct 1, 2012
 *      Author: malcom
 */

#include "Palm.h"
#include "bullet/LinearMath/btQuaternion.h"
#include "bullet/LinearMath/btMatrix3x3.h"

Palm::Palm() {

	this->fi=0;
	this->teta=3.14/6;
	this->raggio=50;

	this->pimezz=3.14/2;

	for(int i=0;i<PALM_DIM;i++)
	{
		posa[i]=0;
	}

	//PUNTO 1
	punti[0][0]=punti[0][1]=punti[0][2]=0;
	//PUNTO 2
	int n=1;
	for(n=1;n<6;n++)
	{
		punti[n][0]=punti[0][0]+raggio*sin(pimezz-n*teta)*cos(fi);
		punti[n][1]=punti[0][1]+raggio*cos(pimezz-n*teta);
		punti[n][2]=punti[0][2]+raggio*sin(pimezz-n*teta)*sin(fi);
	}
	punti[6][0]=punti[4][0];
	punti[6][1]=-punti[4][1];
	punti[6][2]=punti[4][2];

	punti[7][0]=-punti[6][0];
	punti[7][1]=punti[6][1];
	punti[7][2]=punti[6][2];

	for(int i=0;i<8;i++)
	{
		for(int j=0;j<3;j++)
		{
			puntiProiettati[i][j]=punti[i][j];
		}
	}

}

Palm::~Palm() {
	// TODO Auto-generated destructor stub
}

void Palm::Draw() {


	glPointSize(5.0f);
	glBegin(GL_POINTS);
	glColor3f(1.0f,0,0);
	glVertex3f(puntiProiettati[0][0],puntiProiettati[0][1],puntiProiettati[0][2]);
	glColor3f(0.0f,1.00,0);
	for(int i=1;i<8;i++)
		glVertex3f(puntiProiettati[i][0],puntiProiettati[i][1],puntiProiettati[i][2]);
	glEnd();

}

void Palm::Update()
{
	int debug=0;
	int T[3]={posa[PALMx],posa[PALMy],posa[PALMz]};

	if(debug)
		printf("Costruisco la matrice R con i quaternioni\n");
	if(debug)
		printf("[%d %d %d] \n",posa[PALMroty],posa[PALMrotx],posa[PALMrotz]);



	btQuaternion q((float)posa[PALMroty],(float)posa[PALMrotx],(float)posa[PALMrotz]);
	btMatrix3x3 R(q);

	if(debug)
	{
		printf("T[%d %d %d] \n",T[0],T[1],T[2]);
		printf("R\n");
		printf("[%d %d %d] \n",(int)R[0][0],(int)R[0][1],(int)R[0][2]);
		printf("[%d %d %d] \n",(int)R[1][0],(int)R[1][1],(int)R[1][2]);
		printf("[%d %d %d] \n",(int)R[2][0],(int)R[2][1],(int)R[2][2]);
	}

	for(int i=0;i<8;i++)
	{
		this->puntiProiettati[i][0] = (punti[i][0]*R[0][0]+punti[i][1]*R[0][1]+punti[i][2]*R[0][2]) + T[0];
		this->puntiProiettati[i][1] = (punti[i][0]*R[1][0]+punti[i][1]*R[1][1]+punti[i][2]*R[1][2]) + T[1];
		this->puntiProiettati[i][2] = (punti[i][0]*R[2][0]+punti[i][1]*R[2][1]+punti[i][2]*R[2][2]) + T[2];

		//printf("[%d][%f \t%f \t%f]\n",i,this->puntiProiettati[i][0],this->puntiProiettati[i][1],this->puntiProiettati[i][2]);
	}

}


