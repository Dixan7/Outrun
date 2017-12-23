#ifndef __LINE_H__
#define __LINE_H__

struct PointLine {
	float xCamera, yCamera, zCamera;
	float xWorld, yWorld, zWorld;
	float xScreen, yScreen, wScreen;
	float scale;
};

class Line
{
public:

	Line();
	~Line();

	void projection(PointLine &p, int cameraX, int cameraY, int cameraZ, float cameraD);

public:
	int index;
	PointLine p1, p2;
	PointLine p11, p21;
	bool light, mirror;
	float curve, distance;

};

#endif // __LINE_H__