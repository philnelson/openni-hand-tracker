/*
 * particle.h
 *
 *  Created on: Oct 1, 2012
 *      Author: malcom
 */
#include <stdio.h>

#ifndef PARTICLE_H_
#define PARTICLE_H_

#define partX		0
#define partY		1
#define partZ		2
#define partrot	X	3
#define partrotY	4
#define partrotZ	5
#define partDIM		6




class particle {
public:
	particle();
	virtual ~particle();

	int posa[6];
	int posaBest[6];
	int vposa[6];

	int punti_esterni;
	int errore_posa;
	int errore_posaBest;
};

#endif /* PARTICLE_H_ */
