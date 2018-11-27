#ifndef _MESH_RENDER_H
#define _MESH_RENDER_H

#include <system/application.h>
#include <graphics/sprite.h>
#include <maths/vector2.h>
#include <maths/vector4.h>
#include <maths/matrix44.h>
#include <vector>
#include <graphics/mesh_instance.h>
#include "primitive_builder.h"
#include <platform/vita/graphics/texture_vita.h>
#include <system/application.h>


// FRAMEWORK FORWARD DECLARATIONS
namespace gef
{
	class Platform;
	class SpriteRenderer;
	class Font;
	class Renderer3D;
	class Mesh;
	class InputManager;
}

class Mesh_Render
{
public:
	Mesh_Render();
	~Mesh_Render();

	// Initalize 
	void RenderInit(PrimitiveBuilder* primitive_builder_);
	// Update
	void RenderUpdate();
	// Render Mirrors
	void MirrorRender(gef::Renderer3D* renderer_3D);
	// Render Beam
	void BeamRender(gef::Renderer3D* renderer_3D);
	// Render Goal
	void GoalRender();
	// Render laser
	void LaserRender();

	// Mirror Mesh
	gef::MeshInstance* mirror_mesh_[4];
	// Beam Mesh
	gef::MeshInstance* laser_beam_mesh_[5];
	// Goal Mesh
	// Laser Mesh

	int mirror_amount_, beam_amount_;
	float is_found;
	gef::Matrix44 transform_matrix_; 

	
	
};
#endif