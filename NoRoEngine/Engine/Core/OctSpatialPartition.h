#ifndef OCTSPATIALPARTITION_H
#define OCTSPATIALPARTITION_H

#include "../Math/Ray.h"
#include "../Rendering//3D/GameObject.h"

#define CHILDREN_NUMBER 8
using namespace glm;
enum class OctChildren {
	//T = top, L = left, F = front
	OCT_TLF,
	OCT_BLF,
	OCT_TRF,
	OCT_BRF,
	//R = Rear , B = Bottom, R = Right
	OCT_TLR,
	OCT_BLR,
	OCT_TRR,
	OCT_BRR
};

class OctNode
{
public:
	OctNode(vec3 position_, float size_, OctNode * parent_);
	~OctNode();

	void OnDestroy();
	void Octify(int depth_);
	OctNode * GetParent();
	OctNode * getChild(OctChildren childrenPosition_);
	void AddCollisionObject(GameObject * obj_);
	int GetObjectCount() const;
	bool isLeaf() const;
	BoundingBox * GetBoundingBox() const;
	int GetChildCount() const;

private:
	friend class OctSpatialPartition;
	BoundingBox * octBounds;
	OctNode * parent;
	OctNode * children[CHILDREN_NUMBER];
	vector<GameObject*> objectList;
	float size;
	static int childNum;


};

class OctSpatialPartition 
{
public:
	OctSpatialPartition(float worldSize_);
	~OctSpatialPartition();
	void OnDestroy();
	void AddObject(GameObject * obj_);
	GameObject * GetCollision(Ray ray_);

private:
	OctNode * root;
	vector<OctNode*> rayIntersectionList;
	void AddObjectToCell(OctNode * cell_, GameObject *obj_);
	void PrepareCollisionQuery(OctNode * cell_, Ray ray_);

};
#endif // !OCTSPATIALPARTITION_H


