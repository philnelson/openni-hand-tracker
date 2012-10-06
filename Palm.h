/*
 * Palm.h
 *
 *  Created on: Oct 1, 2012
 *      Author: malcom
 */

#ifndef PALM_H_
#define PALM_H_

#include <iostream>
#include <assert.h>
#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include <ctime>
#include <cmath>

#define PALM_DIM		6
#define PALM_VERTEX		13

#define PALMx    	  	0
#define PALMy    	  	1
#define PALMz   	  	2
#define PALMrotx      	3
#define PALMroty      	4
#define PALMrotz      	5

using namespace std;

class Palm {
public:
	Palm();
	virtual ~Palm();
	void Draw();
	void Update();

	float posa[PALM_DIM];
	float punti[PALM_VERTEX][3];
	float puntiProiettati[PALM_VERTEX][3];
	float teta;
	float raggio;
	float fi;
	float pimezz;
};

#endif /* PALM_H_ */
