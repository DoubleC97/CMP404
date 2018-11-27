#ifndef _COLLISIONS_H
#define _COLLISIONS_H

#include <system/application.h>
#include <graphics/sprite.h>
#include <maths/vector2.h>
#include <maths/vector4.h>
#include <maths/matrix44.h>
#include <vector>
#include <graphics/mesh_instance.h>
#include "primitive_builder.h"
#include "maths/quaternion.h"
#include "ar_app.h"

// FRAMEWORK FORWARD DECLARATIONS
namespace gef
{
	class Platform;
	class SpriteRenderer;
	class Font;
	class Renderer3D;
	class Mesh;
	class InputManager;
	class Scene;
}

class Collision
{
public:
	Collision();
	~Collision();

	void SpheretoSphere(gef::MeshInstance* first_mesh_, gef::MeshInstance* other_mesh_);
	void AABBtoAABB(gef::MeshInstance* first_mesh_, gef::MeshInstance* other_mesh_);

	gef::Matrix44 other_mesh_transform_matrix;;
	gef::Vector4 position_vector, position_vector2;
	gef::Vector4 scale_vector, scale_vector2;

	float distance_circle_centre_;
	float positionX1, positionX2, positionY1, positionY2, positionZ1, positionZ2;
	float sum_of_radius;
	float left_side_, right_side_, left_side2_, right_side2_;
	float bottom, top, bottom2, top2;
	float back_, forward_, back2_, forward2_;

	bool is_colliding;
};
#endif

