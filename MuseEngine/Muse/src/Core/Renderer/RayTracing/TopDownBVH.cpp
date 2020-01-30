#include "MusePCH.h"

#include "TopDownBVH.h"
#include "SceneObject.h"
#include "BoundingVolume.h"
#include "Box.h"
#include "Shape.h"

TopDownBVH::TopDownBVH(Scene & scene)
	: BVH(scene) { }

TopDownBVH::~TopDownBVH() { }

void TopDownBVH::ConstructHierarchy(std::vector<SceneObject*> & sceneObjects)
{
	boundingVolumes.clear();

	glm::vec3 minBounds;
	glm::vec3 maxBounds;
	GetWorldMinMaxBounds(minBounds, maxBounds, sceneObjects);
	glm::vec3 range = maxBounds - minBounds;
	glm::vec3 center = (minBounds + maxBounds) / 2;

	Box* box = new Box(center, range);
	BoundingVolume* boundingVolume = new BoundingVolume(*box);
	boundingVolumes.push_back(boundingVolume);

	if (sceneObjects.size() > 2)
	{
		Split(*boundingVolume, sceneObjects);
	}
	else
	{
		for (SceneObject* sceneObject : sceneObjects)
		{
			boundingVolume->childrenSceneObjects.push_back(sceneObject);
		}
	}

}

void TopDownBVH::Split(BoundingVolume & parentBoundingVolume, std::vector<SceneObject*> & sceneObjectsToGroup) const
{
	//Start with placing two bounding volumes inside the parent bounding volume, that split the space evenly.
	glm::vec3 parentBoundingVolumePosition = parentBoundingVolume.boundingVolumeShape.GetTransform().GetPosition();
	glm::vec3 parentBoundingVolumeScale = parentBoundingVolume.boundingVolumeShape.GetTransform().GetScale();

	glm::vec3 box1Position;
	glm::vec3 box2Position;
	glm::vec3 boxScale;

	if (parentBoundingVolumeScale.x >= parentBoundingVolumeScale.y && 
		parentBoundingVolumeScale.x >= parentBoundingVolumeScale.z)
	{
		boxScale = glm::vec3(parentBoundingVolumeScale.x / 2, parentBoundingVolumeScale.y, parentBoundingVolumeScale.z);
		box1Position = glm::vec3(parentBoundingVolumePosition.x - parentBoundingVolumeScale.x / 4, parentBoundingVolumePosition.y, parentBoundingVolumePosition.z);
		box2Position = glm::vec3(parentBoundingVolumePosition. + parentBoundingVolumeScale.x / 4, parentBoundingVolumePosition.y, parentBoundingVolumePosition.z);
	}
	else if (parentBoundingVolumeScale.y >= parentBoundingVolumeScale.x && 
		parentBoundingVolumeScale.y >= parentBoundingVolumeScale.z)
	{
		boxScale = glm::vec3(parentBoundingVolumeScale.x, parentBoundingVolumeScale.y / 2, parentBoundingVolumeScale.z);
		box1Position = glm::vec3(parentBoundingVolumePosition.x, parentBoundingVolumePosition.y - parentBoundingVolumeScale.y / 4, parentBoundingVolumePosition.z);
		box2Position = glm::vec3(parentBoundingVolumePosition.x, parentBoundingVolumePosition.y + parentBoundingVolumeScale.y / 4, parentBoundingVolumePosition.z);
	}
	else
	{
		boxScale = glm::vec3(parentBoundingVolumeScale.x, parentBoundingVolumeScale.y, parentBoundingVolumeScale.z / 2);
		box1Position = glm::vec3(parentBoundingVolumePosition.x, parentBoundingVolumePosition.y, parentBoundingVolumePosition.z - parentBoundingVolumeScale.z / 4);
		box2Position = glm::vec3(parentBoundingVolumePosition.x, parentBoundingVolumePosition.y, parentBoundingVolumePosition.z + parentBoundingVolumeScale.z / 4);
	}

	//Sort the sceneObjects over the two new boundubg volumes box spaces, and the parent boundubg volume.
	std::vector<SceneObject*> sceneObjectsWithinbox1;
	std::vector<SceneObject*> sceneObjectsWithinbox2;

	glm::vec3 halfBoxScale = boxScale / 2.f;
	glm::vec3 box1MinBounds = box1Position - halfBoxScale;
	glm::vec3 box1MaxBounds = box1Position + halfBoxScale;

	glm::vec3 box2MinBounds = box2Position - halfBoxScale;
	glm::vec3 box2MaxBounds = box2Position + halfBoxScale;

	for (SceneObject* sceneObject : sceneObjectsToGroup)
	{
		bool overlapWithBoundingVolume1 = CheckShapeOverlap(sceneObject->GetShape(), box1MinBounds, box1MaxBounds);
		bool overlapWithBoundingVolume2 = CheckShapeOverlap(sceneObject->GetShape(), box2MinBounds, box2MaxBounds);

		assert(overlapWithBoundingVolume1 || overlapWithBoundingVolume2);

		if (overlapWithBoundingVolume1 && overlapWithBoundingVolume2)
		{
			parentBoundingVolume.childrenSceneObjects.push_back(sceneObject);
		}
		else
		{
			if (overlapWithBoundingVolume1)
			{
				sceneObjectsWithinbox1.push_back(sceneObject);
			}
			else
			{
				sceneObjectsWithinbox2.push_back(sceneObject);
			}
		}
	}

	//Check if the new boundings boxes should be created, and if so create them.
	//If the bounding boxes have more then one SceneObject as child, split them as well.
	if (sceneObjectsWithinbox1.size() > 2)
	{
		BoundingVolume& newBoundingVolume = *new BoundingVolume(*new Box(box1Position, boxScale));
		parentBoundingVolume.childrenBoundingVolumes.push_back(&newBoundingVolume);
		Split(newBoundingVolume, sceneObjectsWithinbox1);
	}
	else 
	{
		for (SceneObject* sceneObject : sceneObjectsWithinbox1)
		{
			parentBoundingVolume.childrenSceneObjects.push_back(sceneObject);
		}
	}

	if (sceneObjectsWithinbox2.size() > 2)
	{
		BoundingVolume& newBoundingVolume = *new BoundingVolume(*new Box(box2Position, boxScale));
		parentBoundingVolume.childrenBoundingVolumes.push_back(&newBoundingVolume);
		Split(newBoundingVolume, sceneObjectsWithinbox2);
	}
	else
	{
		for (SceneObject* sceneObject : sceneObjectsWithinbox2)
		{
			parentBoundingVolume.childrenSceneObjects.push_back(sceneObject);
		}
	}

}

bool TopDownBVH::CheckShapeOverlap(const Shape & shape, const glm::vec3 & minBound, const glm::vec3 & maxBound) const
{
	glm::vec3 shapeMinBound;
	glm::vec3 shapeMaxBound;

	shape.GetMinMaxBounds(shapeMinBound, shapeMaxBound);

	return (shapeMaxBound.x >= minBound.x && maxBound.x >= shapeMinBound.x) &&
		(shapeMaxBound.y >= minBound.y && maxBound.y >= shapeMinBound.y) &&
		(shapeMaxBound.z >= minBound.z && maxBound.z >= shapeMinBound.z);
}

void TopDownBVH::GetWorldMinMaxBounds(glm::vec3 & minBound, glm::vec3 & maxBound, const std::vector<SceneObject*> & sceneObjects) const
{
	glm::vec3 sceneObjectMinBound;
	glm::vec3 sceneObjectMaxBound;

	minBound = glm::vec3(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
	maxBound = glm::vec3(std::numeric_limits<float>::min(), std::numeric_limits<float>::min(), std::numeric_limits<float>::min());

	for (SceneObject* sceneObject : sceneObjects)
	{
		sceneObject->GetShape().GetMinMaxBounds(sceneObjectMinBound, sceneObjectMaxBound);

		minBound.x = std::min(sceneObjectMinBound.x, minBound.x);
		minBound.y = std::min(sceneObjectMinBound.y, minBound.y);
		minBound.z = std::min(sceneObjectMinBound.z, minBound.z);

		maxBound.x = std::max(sceneObjectMaxBound.x, maxBound.x);
		maxBound.y = std::max(sceneObjectMaxBound.y, maxBound.y);
		maxBound.z = std::max(sceneObjectMaxBound.z, maxBound.z);
	}
}
