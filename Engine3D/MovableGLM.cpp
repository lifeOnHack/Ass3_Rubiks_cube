#include "MovableGLM.h"
#include <stdio.h>
#include "../res/includes/glm/gtc/matrix_transform.hpp"
#include "../res/includes/glm/gtc/quaternion.hpp"
#include <iostream>

static void printMat(const glm::mat4 mat)
{
	printf(" matrix: \n");
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			printf("%f ", mat[j][i]);
		printf("\n");
	}
}

MovableGLM::MovableGLM()
{
	ZeroTrans();
}

glm::mat4 MovableGLM::MakeTrans(const glm::mat4 &prevTransformations) const
{
	return prevTransformations * MakeTrans();
}

glm::mat4 MovableGLM::MakeTrans() const
{
	return rot * trans * scl;
}

void MovableGLM::MyTranslate(glm::vec3 delta,int mode)
{
	trans = glm::translate(trans,delta);
}

void  MovableGLM::MyRotate(float angle,const glm::vec3 &vec,int mode)
{
	std::cout << "Updating Rotation matrix: ";
	std::cout << "rotVec= (" << vec.x << " , " << vec.y << " , " << vec.z << " ,)" <<std::endl;

	/*
	glm::mat4 currentRot = glm::rotate(glm::mat4(1), angle, vec); //desired : rot * currentRot
	
	/rot = currentRot * rot;
	glm::vec3 vec1 =glm::mat3(glm::transpose(rot)) * vec;
	//std::cout << "RotatedVec= (x=" << vec1.x << ", y=" << vec1.y << " ," << vec1.z << ")" << std::endl;
	
	*/
	glm::vec3 vec1 = glm::mat3(glm::transpose(rot)) * vec;
	glm::mat4 currentRot = glm::rotate(glm::mat4(1), angle, vec1); //desired : rot * currentRot

	rot = glm::rotate(rot, angle, vec1);
	  //rot = currentRot * rot ;
}
	
void  MovableGLM::MyScale(glm::vec3 scale)
{
	scl = glm::scale(scl,scale);
}

void MovableGLM::ZeroTrans()
{
	trans = glm::mat4(1);
	rot = glm::mat4(1);
	scl = glm::mat4(1);
}