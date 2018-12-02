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
#include <graphics\mesh.h>
#include <graphics\scene.h>
#include "Collision.h"


// FRAMEWORK FORWARD DECLARATIONS
namespace gef
{
	class Platform;
	class SpriteRenderer;
	class Font;
	class Renderer3D;
	class Mesh;
	class Scene;
	class InputManager;
}

class Mesh_Render
{
public:
	Mesh_Render();
	~Mesh_Render();

	// Initalize 
	void RenderInit(PrimitiveBuilder* primitive_builder_, gef::Platform* platform_);
	// Detect Marker
	void DetectMarker();
	// Update
	void RenderUpdate(gef::InputManager* input_manager_, int& score);

	// Render Target
	void TargetRender(gef::Renderer3D* renderer_3D);
	// Render Projectile
	void ProjectileRender(gef::Renderer3D* renderer_3D);

	gef::Mesh* GetMesh(gef::Scene* scene, gef::Platform* platform_);

	// Target mesh
	gef::MeshInstance* target_mesh_[6];
	// Projectile mesh
	gef::MeshInstance* projectile_mesh_;

	// Scene for model loading
	gef::Scene* target_scene_;
	// 
	gef::Mesh* target_mesh_model;


	// The amount of targets
	int target_amount_;

	// Has the marker(s) been found
	float is_found;

	// Transform Matrix
	gef::Matrix44 transform_matrix_; 
	// To store all of the markeer transfroms
	gef::Matrix44 stored_marker_transform_[6];
	
	// For moving the projectile
	float move_projectile_;
	// Check for button press
	bool button_pressed_;



	
	
};
#endif