#include "Mesh_Render.h"
#include "graphics\mesh_instance.h"
#include "graphics\mesh.h"

#include <graphics/renderer_3d.h>
#include <graphics/render_target.h>
#include <graphics/sprite_renderer.h>

#include <assets/png_loader.h>
#include <graphics/texture.h>
#include <graphics/image_data.h>
#include <graphics/primitive.h>

#include <sony_sample_framework.h>
#include "math.h" 
#include <input\input_manager.h>
#include <input\sony_controller_input_manager.h>

Mesh_Render::Mesh_Render()
{
	
}


Mesh_Render::~Mesh_Render()
{
}

// Colliusion manger
Collision collision_manager_;

// Init()
void Mesh_Render::RenderInit(PrimitiveBuilder* primitive_builder_, gef::Platform* platform_)
{
	
	// Initulize target amount and projectile amount
	target_amount_ = 6;
	// Initulize move projectile for firing
	move_projectile_ = -0.5;
	// Initulaize button pressed
	button_pressed_ = false;
	//
	// Collision variables
	collision_manager_.is_colliding = false;

	// Set trsansfor mamtirx to identity
	transform_matrix_.SetIdentity();
	//  set scale of transform_matrix
	transform_matrix_.Scale(gef::Vector4(0.059f, 0.059f, 0.059f));

	// Target scene initulize
	target_scene_ = new gef::Scene();

	// Load in target sceneform file
	target_scene_->ReadSceneFromFile(*platform_, "target.scn");

	// Get mesh
	target_mesh_model = GetMesh(target_scene_, platform_);

	// For each target
	for (int i = 0; i < target_amount_; i++)
	{
		// Initlize target_mesh_ to target mesh modle
		target_mesh_[i] = new gef::MeshInstance();
		target_mesh_[i]->set_mesh(target_mesh_model);
	}

	// Initlize laser_projectile_mesh_ to default cube mesh
	projectile_mesh_ = new gef::MeshInstance();
	projectile_mesh_->set_mesh(primitive_builder_->GetDefaultCubeMesh());
	

}

void Mesh_Render::DetectMarker()
{

	// For  each target
	for (int i = 0; i < target_amount_; i++)
	{
		if (sampleIsMarkerFound(i))
		{
			gef::Matrix44 marker_transform_;
			sampleGetTransform(i, &marker_transform_);

			// Marker is found
			is_found = true;

			// Set marker transform to  display cube on marker
			stored_marker_transform_[i] = transform_matrix_ * marker_transform_;
			// Set target mesh transform
			target_mesh_[i]->set_transform(stored_marker_transform_[i]);
		}
		else
		{
			// Set up start position for meshes so they are off screen when markers havent been found
			gef::Matrix44 start_transform_matrix_;
			gef::Vector4 start_position;
			start_position.set_value(100.0f, 100.0f, 100.0f);
			start_transform_matrix_.SetTranslation(start_position);
			// Set start posiitons for targets
			target_mesh_[i]->set_transform(start_transform_matrix_);
			// Set start position for projectile
			projectile_mesh_->set_transform(start_transform_matrix_);
		}
	}

}

// Update()
void Mesh_Render::RenderUpdate(gef::InputManager* input_manager_, int& score)
{
	
	// Set is found to false
	is_found = false;

	// Find mnarkers
	DetectMarker();

	gef::Matrix44 projectile_transform_matrix_;
	gef::Vector4 projectile_scale_, projectile_position_;
	
	// Set Scale value
	projectile_scale_.set_value(0.059f, 0.059f, 0.059f);
	// Set position value
	projectile_position_.set_value(0.0f, 0.0f, move_projectile_);
	// Set scale transform
	projectile_transform_matrix_.Scale(projectile_scale_);
	// Set translation
	projectile_transform_matrix_.SetTranslation(projectile_position_);
	// Set projectile transform
	projectile_mesh_->set_transform(projectile_transform_matrix_);


	// For each target
	for (int i = 0; i < target_amount_; i++)
	{
		// Calll colliisons
		collision_manager_.AABBtoAABB(target_mesh_[i], projectile_mesh_);

		// Check if collision has happned
		if (collision_manager_.is_colliding == true && button_pressed_ == true)
		{
			// Set button pressed to false
			button_pressed_ = false;
			// Restet move projectile
			move_projectile_ = -0.5;
			// Add to score
			score++;

			break;
		}
	}


	// Check if button is pressed
	if (button_pressed_ == true)
	{
		// Add -0.05 to move projectile
		move_projectile_ += -0.05f;
	}

	// Check if projectile has reach furthest distance
	if (move_projectile_ <= -1.5f)
	{
		// Set button pressed to false
		button_pressed_ = false;
		// Restet move projectile
		move_projectile_ = -0.5;
	}

	// read input devices
	if (input_manager_)
	{
		input_manager_->Update();

		// controller input
		const gef::SonyController* controller_manager = input_manager_->controller_input()->GetController(0);
		if (controller_manager->buttons_pressed() & gef_SONY_CTRL_CROSS)
		{
			// Check if button has already been pressed
			if (button_pressed_ == false)
			{
				// Set button pressed to true
				button_pressed_ = true;
			}
		}

	}
}

// For rendering the targets
void Mesh_Render::TargetRender(gef::Renderer3D* renderer_3D)
{
	// For each target
	for (int i = 0; i < target_amount_; i++)
	{
		// Draw targets
		renderer_3D->DrawMesh(*target_mesh_[i]);
	}
	
}

// For rendering the projectile
void Mesh_Render::ProjectileRender(gef::Renderer3D* renderer_3D)
{
	// Render projectile
	renderer_3D->DrawMesh(*projectile_mesh_);
}


gef::Mesh* Mesh_Render::GetMesh(gef::Scene* scene, gef::Platform* platform_)
{
	gef::Mesh* mesh_ = NULL;

	// If scene
	if (scene)
	{
		// Check if ther eis scene data
		if (scene->mesh_data.size() > 0)
		{
			// Create a mesh from data
			mesh_ = scene->CreateMesh(*platform_, scene->mesh_data.front());
		}
	}

	// Return mesh
	return mesh_;
}

