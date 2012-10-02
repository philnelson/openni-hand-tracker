/*
 * particle.cpp
 *
 *  Created on: Oct 1, 2012
 *      Author: malcom
 */

#include "particle.h"

particle::particle() {

	for(int i=0;i<partDIM;i++)
	{
		this->posa[i]=this->posaBest[i]=this->vposa[i]=0;
	}


	this->errore_posa=999;
	this->errore_posaBest=999;



}

particle::~particle() {
	// TODO Auto-generated destructor stub
}

