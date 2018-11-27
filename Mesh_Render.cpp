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

Mesh_Render::Mesh_Render()
{
	
}


Mesh_Render::~Mesh_Render()
{
}


// Init()
void Mesh_Render::RenderInit(PrimitiveBuilder* primitive_builder_)
{
	
	// Initulize mirror amount and beam amount
	mirror_amount_ = 4;
	beam_amount_ = 5;

	// Set trsansfor mamtirx to identity
	transform_matrix_.SetIdentity();
	//  set scale of transform_matrix
	transform_matrix_.Scale(gef::Vector4(0.059f, 0.059f, 0.059f));

	// For each mirror
	for (int i = 0; i < mirror_amount_; i++)
	{
		// Initlize mirror_mesh_ to default cube mesh
		mirror_mesh_[i] = new gef::MeshInstance();
		mirror_mesh_[i]->set_mesh(primitive_builder_->GetDefaultCubeMesh());
	}

	// For  each beam
	for (int i = 0; i < beam_amount_; i++)
	{
		// Initlize laser_beam_mesh_ to default cube mesh
		laser_beam_mesh_[i] = new gef::MeshInstance();
		laser_beam_mesh_[i]->set_mesh(primitive_builder_->GetDefaultCubeMesh());
	}
}

// Update()
void Mesh_Render::RenderUpdate()
{
	// To store all of the markeer transfroms
	gef::Matrix44 stored_marker_transform_[5];

	// Set is found to false
	is_found = false;

	// For  each mirror
	for (int i = 0; i < mirror_amount_; i++)
	{
		if (sampleIsMarkerFound(i))
		{
			gef::Matrix44 marker_transform_;
			sampleGetTransform(i, &marker_transform_);

			// Marker is found
			is_found = true;

			// Set marker transform to  display cube on marker
			stored_marker_transform_[i] = transform_matrix_ * marker_transform_;
			// Set mirror mesh transform
			mirror_mesh_[i]->set_transform(stored_marker_transform_[i]);
		}
		else
		{
			gef::Matrix44 start_transform_matrix_;
			gef::Vector4 start_position;
			start_position.set_value(100.0f, 100.0f, 100.0f);
			start_transform_matrix_.SetTranslation(start_position);
			// Set start posiitons for meshses
			mirror_mesh_[i]->set_transform(start_transform_matrix_);
			//laser_beam_mesh_->set_transform(start_transform_matrix_);
		}


	}

	// For  each beam
	for (int i = 0; i < beam_amount_; i++)
	{
		// If sample is not found
		if (!sampleIsMarkerFound)
		{
			gef::Matrix44 start_transform_matrix_;
			gef::Vector4 start_position;
			start_position.set_value(100.0f, 100.0f, 100.0f);
			start_transform_matrix_.SetTranslation(start_position);

			laser_beam_mesh_[i]->set_transform(start_transform_matrix_);
		}
	}

	// Offset
	gef::Matrix44 offset_matrix_, offset_scale_, offset_rotation_, offset_translation_;
	// Beam
	gef::Matrix44  beam_transform_matrix_;
	gef::Vector4 beam_scale_transfrom_;

	// Distance
	float v_distance, h_distance, distance;
	// Angle
	float angle;
	// Midpoint
	gef::Vector4 midpoint[5];

	// For each marker
	for (int i = 0; i < beam_amount_; i++)
	{
		int j;

		if (i < 5)
		{
			j = i + 1;
		}

		// Calculate the  verticle distance
		v_distance = stored_marker_transform_[j].GetTranslation().y() - stored_marker_transform_[i].GetTranslation().y();
		// Calculate the  horizontal  distance
		h_distance = stored_marker_transform_[j].GetTranslation().x() - stored_marker_transform_[i].GetTranslation().x();
		// Calculate distance
		distance = sqrt(pow(v_distance, 2) + pow(h_distance, 2));
		// Calculate the angle
		angle = atan(h_distance / v_distance);





		// Calculate the midpoint
		midpoint[i] = (stored_marker_transform_[i].GetTranslation() + stored_marker_transform_[j].GetTranslation() / 2);


		// Set value
		beam_scale_transfrom_.set_value(0.059f, distance, 0.059f);
		// Set offset scale to identity
		offset_scale_.SetIdentity();
		// Set scale offset
		offset_scale_.Scale(beam_scale_transfrom_);

		// Set offset rotation to identity
		offset_rotation_.SetIdentity();
		// Set offset roation
		offset_rotation_.RotationZ(-angle);

		// Set translation to identity
		offset_translation_.SetIdentity();
		// Set offset translation
		offset_translation_.SetTranslation(midpoint[i]);

		// Set offst matrix to identity
		offset_matrix_.SetIdentity();
		// Set offset matrix
		offset_matrix_ = offset_scale_ * offset_rotation_ * offset_translation_;

		// New arker matrix
		gef::Matrix44 marker_matrix_;
		// Set marker mtrix to identity
		marker_matrix_.SetIdentity();

		// Set tranlation of marker matrixto the same as stored marker
		marker_matrix_.SetTranslation(stored_marker_transform_[i].GetTranslation());


		// Set beam transform to identity
		beam_transform_matrix_.SetIdentity();
		// Set beam matrix
		beam_transform_matrix_ = offset_matrix_ * marker_matrix_;

		// Set beam transform
		laser_beam_mesh_[i]->set_transform(beam_transform_matrix_);
	}
}

// For rendering the mirrors
void Mesh_Render::MirrorRender(gef::Renderer3D* renderer_3D)
{
	// For each mirror
	for (int i = 0; i < mirror_amount_; i++)
	{
		// Draw mirrors
		renderer_3D->DrawMesh(*mirror_mesh_[i]);
	}
	
}

// For rendering the beam
void Mesh_Render::BeamRender(gef::Renderer3D* renderer_3D)
{

	// For  each beam
	for (int i = 0; i < beam_amount_; i++)
	{
		// Draw beam
		renderer_3D->DrawMesh(*laser_beam_mesh_[i]);
	}
}

// For rendering the laser
void Mesh_Render::LaserRender()
{

}

// For rendering the goal
void Mesh_Render::GoalRender()
{

}