#include "RubikCube.h"
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>

RubikCube::RubikCube()
{
	int ai = EDGE_SIZE * EDGE_SIZE, aj = EDGE_SIZE;
	for (int i = 0; i < EDGE_SIZE; i++)
	{
		for (int j = 0; j < EDGE_SIZE; j++)
		{
			for (int k = 0; k < EDGE_SIZE; k++)
			{
				miniCubes[i][j][k] = k + aj * j + ai * i;
			}
		}
	}
	for (int i = 0; i < CUBE_FACES; i++)
	{
		this->rotetedFaces[i] = 0.0;
	}
}
Face RubikCube::getFace(char face)
{
	switch (face)
	{
	case 'U':
		return getUpFace();
		break;
	case 'D':
		return getDownFace();
		break;
	case 'F':
		return getFrontFace();
		break;
	case 'B':
		return getBackFace();
		break;
	case 'L':
		return getLeftFace();
		break;
	case 'R':
		return getRightFace();
		break;
	default:
		return Face();
		break;
	}
}
void RubikCube::setFace(Face newFace, char face)
{
	switch (face)
	{
	case 'U':
		setUpFace(newFace);
		break;
	case 'D':
		setDownFace(newFace);
		break;
	case 'F':
		setFrontFace(newFace);
		break;
	case 'B':
		setBackFace(newFace);
		break;
	case 'L':
		setLeftFace(newFace);
		break;
	case 'R':
		setRightFace(newFace);
		break;
	default:
		break;
	}
}

void RubikCube::printData() {
	for (int i = 0; i < EDGE_SIZE;i++) {
		for (int j = 0; j < EDGE_SIZE;j++) {
			for (int k = 0; k < EDGE_SIZE;k++) {
				int loc = 9 * i + 3 * j + k;
				std::cout << "Location " << loc << " : idx of shape at loc: " << miniCubes[i][j][k] << std::endl;
			}
		}
	}
}

Face RubikCube::rotateFace(const Face& face, double angle, bool flip)
{
	auto newFace = face;
	for (int i = 0; i < EDGE_SIZE; i++)
	{
		for (int j = 0; j < EDGE_SIZE; j++)
		{
			std::vector<int> rotatedCoords = findRotatedLocation(i, j, M_PI / 2, flip);
			int rotated_i = rotatedCoords[0];
			int rotated_j = rotatedCoords[1];
			newFace[rotated_i][rotated_j] = face[i][j];
		}
	}
	return newFace;
}

bool RubikCube::isAbleToRot(char face) const
{
	switch (face)
	{
	case 'U':
	case 'D':
		return this->rotetedFaces[LEFT] == 0 && this->rotetedFaces[RIGHT] == 0
			&& this->rotetedFaces[FRONT] == 0 && this->rotetedFaces[BACK] == 0;
		break;
	case 'L':
	case 'R':
		return this->rotetedFaces[UP] == 0 && this->rotetedFaces[DOWN] == 0
			&& this->rotetedFaces[FRONT] == 0 && this->rotetedFaces[BACK] == 0;
		break;
	case 'F':
	case 'B':
		return this->rotetedFaces[LEFT] == 0 && this->rotetedFaces[RIGHT] == 0
			&& this->rotetedFaces[UP] == 0 && this->rotetedFaces[DOWN] == 0;
		break;
	default:
		break;
	}
	return true;
}

void RubikCube::NTYDegRot(int face, bool ckw)
{
	
	switch (face)
	{
	case UP:
		setUpFace(rotateFace(getUpFace(), 90, ckw? true:false));//true
		break;
	case DOWN:
		setDownFace(rotateFace(getDownFace(), 90, ckw ? true:false));//true
		break;
	case FRONT:
		setFrontFace(rotateFace(getFrontFace(), 90, ckw ? false: true));//false
		break;
	case BACK:
		setBackFace(rotateFace(getBackFace(), 90, ckw ? true : false));//true
		break;
	case LEFT:
		setLeftFace(rotateFace(getLeftFace(), 90, ckw ? false : true));//false
		break;
	case RIGHT:
		setRightFace(rotateFace(getRightFace(), 90, ckw ? true : false));//true
		break;
	default:
		break;
	}
}


Face RubikCube::rotateFace(Face face, bool ckw)
{
	int t1;
	/*[
	* [(02),(12),(22)],
	* [(01),(11),(21)],
	* [(00),(10),(20)]
	* ]
	*/
	if (ckw)
	{
		//tips
		t1 = face[0][2];
		face[0][2] = face[0][0];
		face[0][0] = face[2][0];
		face[2][0] = face[2][2];
		face[2][2] = t1;
		//plus
		t1 = face[1][2];
		face[1][2] = face[0][1];
		face[0][1] = face[1][0];
		face[1][0] = face[2][1];
		face[2][1] = t1;
	}
	else {
		//tips
		t1 = face[2][0];
		face[2][0] = face[0][0];
		face[0][0] = face[0][2];
		face[0][2] = face[2][2];
		face[2][0] = t1;
		//plus
		t1 = face[1][0];
		face[1][0] = face[0][1];
		face[0][1] = face[1][2];
		face[1][2] = face[2][1];
		face[2][1] = t1;
	}
	return face;
}

void RubikCube::updateFaceState(int face, double deg, bool ckw)
{
	this->rotetedFaces[face] += deg;
	while (this->rotetedFaces[face] >= 90)
	{
		this->rotetedFaces[face] -= 90;
		NTYDegRot(face, ckw);

	}
	while (this->rotetedFaces[face] <= -90)
	{
		this->rotetedFaces[face] += 90;
		NTYDegRot(face, ckw);
	}
}


Face RubikCube::getUpFace()
{
	std::cout << "Cubes orientation before rotating Up-Face:" << std::endl;
	printData();

	Face up = Face //holds the indices in the shape vector in game - that are on the face!
	(EDGE_SIZE, std::vector<int>(EDGE_SIZE));
	int j = 2;
	for (int i = 0; i < EDGE_SIZE; i++)
	{
		for (int k = 0; k < EDGE_SIZE; k++)
		{
			up[i][k] = miniCubes[i][j][k];
		}
	}
	return up;
}
void RubikCube::setUpFace(const Face& up)
{
	int j = 2;
	for (int i = 0; i < EDGE_SIZE; i++)
	{
		for (int k = 0; k < EDGE_SIZE; k++)
		{
			miniCubes[i][j][k] = up[i][k];
		}
	}
}
Face RubikCube::getDownFace()
{
	Face down = Face(EDGE_SIZE, std::vector<int>(EDGE_SIZE));
	int j = 0;
	for (int i = 0; i < EDGE_SIZE; i++)
	{
		for (int k = 0; k < EDGE_SIZE; k++)
		{
			down[i][k] = miniCubes[i][j][k];
		}
	}
	return down;
}
void RubikCube::setDownFace(const Face& down)
{
	int j = 0;
	for (int i = 0; i < EDGE_SIZE; i++)
	{
		for (int k = 0; k < EDGE_SIZE; k++)
		{
			miniCubes[i][j][k] = down[i][k];
		}
	}
}
Face RubikCube::getFrontFace()
{
	Face front = Face(EDGE_SIZE, std::vector<int>(EDGE_SIZE));
	int k = 0;
	for (int i = 0; i < EDGE_SIZE; i++)
	{
		for (int j = 0; j < EDGE_SIZE; j++)
		{
			front[i][j] = miniCubes[i][j][k];
		}
	}
	return front;
}
void RubikCube::setFrontFace(const Face& front)
{
	int k = 0;
	for (int i = 0; i < EDGE_SIZE; i++)
	{
		for (int j = 0; j < EDGE_SIZE; j++)
		{
			miniCubes[i][j][k] = front[i][j];
		}
	}
}
Face RubikCube::getBackFace()
{
	Face back = Face(EDGE_SIZE, std::vector<int>(EDGE_SIZE));
	int k = 2;
	for (int i = 0; i < EDGE_SIZE; i++)
	{
		for (int j = 0; j < EDGE_SIZE; j++)
		{
			back[i][j] = miniCubes[i][j][k];
		}
	}
	return back;
}
void RubikCube::setBackFace(const Face& back)
{
	int k = 2;
	for (int i = 0; i < EDGE_SIZE; i++)
	{
		for (int j = 0; j < EDGE_SIZE; j++)
		{
			miniCubes[i][j][k] = back[i][j];
		}
	}
}
Face RubikCube::getLeftFace()
{
	Face left = Face
	(EDGE_SIZE, std::vector<int>(EDGE_SIZE));
	int i = 0;
	for (int j = 0; j < EDGE_SIZE; j++)
	{
		for (int k = 0; k < EDGE_SIZE; k++)
		{
			left[j][k] = miniCubes[i][j][k];
		}
	}
	return left;
}
void RubikCube::setLeftFace(const Face& left)
{
	int i = 0;
	for (int j = 0; j < EDGE_SIZE; j++)
	{
		for (int k = 0; k < EDGE_SIZE; k++)
		{
			miniCubes[i][j][k] = left[j][k];
		}
	}
}
Face RubikCube::getRightFace()
{
	Face right = Face
	(EDGE_SIZE, std::vector<int>(EDGE_SIZE));
	int i = 2;
	for (int j = 0; j < EDGE_SIZE; j++)
	{
		for (int k = 0; k < EDGE_SIZE; k++)
		{
			right[j][k] = miniCubes[i][j][k];
		}
	}
	return right;
}
void RubikCube::setRightFace(const Face& right)
{
	int i = 2;
	for (int j = 0; j < EDGE_SIZE; j++)
	{
		for (int k = 0; k < EDGE_SIZE; k++)
		{
			miniCubes[i][j][k] = right[j][k];
		}
	}
}

std::vector<int> RubikCube::findRotatedLocation(int u, int v, float angle, bool shouldFlip) { //first translate by -1 in the two axes, then rotate - then translate back to our original coords system
	int inverseFactor = shouldFlip != 0 ? -1 : 1;
	angle = M_PI_2; //pi/2
	int xTranslate = -1;
	int yTranslate = -1;
	double sin_angle = sin(angle);
	double cos_angle = cos(angle);
	sin_angle = 1;
	cos_angle = 0;
	u = u + xTranslate;
	v = v + xTranslate;

	//int new_u = (u * cos_angle - sin_angle * v) - xTranslate;
	//int new_v = (u * sin_angle - v * cos_angle) - yTranslate;
	int new_u = (inverseFactor * -v) - xTranslate;
	int new_v = (inverseFactor * u) - yTranslate;

	std::vector<int> newCoords;
	newCoords.push_back(new_u);
	newCoords.push_back(new_v);
	return newCoords;

}



