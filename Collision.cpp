#include "Collision.h"
#include "graphics\mesh_instance.h"
#include "graphics\mesh.h"

Collision::Collision()
{
}


Collision::~Collision()
{
}

void Collision::AABBtoAABB(gef::MeshInstance* first_mesh_, gef::MeshInstance* other_mesh_)
{
	// Get min and max of ball 1 x pos
	left_side_ = first_mesh_->mesh()->aabb().Transform(first_mesh_->transform()).min_vtx().x();
	right_side_ = first_mesh_->mesh()->aabb().Transform(first_mesh_->transform()).max_vtx().x();
	// Get min and max of ball 2 x pos
	left_side2_ = other_mesh_->mesh()->aabb().Transform(other_mesh_->transform()).min_vtx().x();
	right_side2_ = other_mesh_->mesh()->aabb().Transform(other_mesh_->transform()).max_vtx().x();
	// Get min and max of ball 1 y pos
	bottom = first_mesh_->mesh()->aabb().Transform(first_mesh_->transform()).min_vtx().y();
	top = first_mesh_->mesh()->aabb().Transform(first_mesh_->transform()).max_vtx().y();
	// Get min and max of ball 2 y pos
	bottom2 = other_mesh_->mesh()->aabb().Transform(other_mesh_->transform()).min_vtx().y();
	top2 = other_mesh_->mesh()->aabb().Transform(other_mesh_->transform()).max_vtx().y();
	// Get min and max of ball 1 z pos
	back_ = first_mesh_->mesh()->aabb().Transform(first_mesh_->transform()).min_vtx().z();
	forward_ = first_mesh_->mesh()->aabb().Transform(first_mesh_->transform()).max_vtx().z();
	// Get min and max of ball 2 z pos
	back2_ = other_mesh_->mesh()->aabb().Transform(other_mesh_->transform()).min_vtx().z();
	forward2_ = other_mesh_->mesh()->aabb().Transform(other_mesh_->transform()).max_vtx().z();


	// Check if left
	if (left_side_ < right_side2_ && right_side_ > left_side2_)
	{
		// Check if down
		if (bottom < top2 && top > bottom2)
		{
			// Check if back
			if (back_ < forward2_ && forward_ > back2_)
			{
				// Set is colliding to true
				is_colliding = true;

			}
			else
			{
				// Set is colliding to false
				is_colliding = false;
			}
		}
		else
		{
			// Set is colliding to false
			is_colliding = false;
		}

	}
	else
	{
		// Set is colliding to false
		is_colliding = false;
	}

}
