#pragma once
#define EDGE_SIZE 3
#define CUBE_FACES 6
#include <vector>
typedef std::vector<std::vector<int>> Face;
enum FACES
{
	FRONT=0, BACK=1, UP=2, DOWN=3, LEFT=4, RIGHT=5
};
class RubikCube
{
public:
	RubikCube();
	Face getFace(char face);//get face tag: 'U','L','R','B','F','D'
	Face rotateFace(Face face, bool ckw);// ckw = clock-wise
	void updateFaceState(int face, double deg, bool ckw);
	void printData();
	void setFace(Face newFace, char face);
	Face rotateFace(const Face& face, double angle, bool flip);
	bool isAbleToRot(char face) const;
	void NTYDegRot(int face, bool ckw);//ninety degree rotation
private:
	int miniCubes[EDGE_SIZE][EDGE_SIZE][EDGE_SIZE];
	Face getUpFace();
	void setUpFace(const Face& up);
	Face getDownFace();
	void setDownFace(const Face& down);
	Face getFrontFace();
	void setFrontFace(const Face& front);
	Face getBackFace();
	void setBackFace(const Face& back);
	Face getLeftFace();
	void setLeftFace(const Face& left);
	Face getRightFace();
	void setRightFace(const Face& right);

	std::vector<int> findRotatedLocation(int u, int v, float angle, bool shouldFlip); //find rotated location of vector: (u,v) after rotation of angle degrees about the u,v plane
	//the rotation is counter-clockwise so negative angle corresponds to clockwise-rotation
	double rotetedFaces[CUBE_FACES];

};


