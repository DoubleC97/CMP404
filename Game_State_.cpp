#include "Game_State_.h"
#include <system/platform.h>
#include <graphics/sprite_renderer.h>
#include <graphics/texture.h>
#include <graphics/mesh.h>
#include <graphics/primitive.h>
#include <assets/png_loader.h>
#include <graphics/image_data.h>
#include <graphics/font.h>
#include <input/touch_input_manager.h>
#include <maths/vector2.h>
#include <input/input_manager.h>
#include <maths/math_utils.h>
#include <graphics/renderer_3d.h>
#include <graphics/render_target.h>
#include <sony_sample_framework.h>
#include <sony_tracking.h>

// Menu state manager
Menu_State menu_state_manager_;
// Mesh render manager
Mesh_Render mesh_render_manager_;

Game_State_::Game_State_()
{
}


Game_State_::~Game_State_()
{
}

void Game_State_::GameInit(PrimitiveBuilder* primitive_builder_)
{
	
	// Start Game
	start_game_ = false;

	// Game State menu
	game_state = MENU;

	if (game_state == MENU)
	{
		// Init menu
		menu_state_manager_.MenuInit(start_game_);

	}

	// Set trsansfor mamtirx to identity
	transform_matrix.SetIdentity();
	//  set scale of transform_matrix
	transform_matrix.Scale(gef::Vector4(0.059f, 0.059f, 0.059f));

	// Initialise variables
	marker_amount_ = 5;

	// Score
	score = 0;

	// initialise sony framework
	sampleInitialize();
	smartInitialize();

	// Orthographic projection matrix
	ortho_projection_matrix.OrthographicFrustumGL(-1, 1, -1, 1, -1, 1);
	// Calculate CIAR
	camera_image_aspect_ratio_ = (float)CAMERA_WIDTH / (float)CAMERA_HEIGHT;
	// Calculate Ddisplay aspect ratio
	display_aspect_ratio_ = (float)DISPLAY_WIDTH / (float)DISPLAY_HEIGHT;
	// Calculate virtual image scale factor
	virticle_image_scale_factor_ = display_aspect_ratio_ / camera_image_aspect_ratio_;

	// Set postion of sprite to the back of the projection volume
	draw_camera_feed_.set_position(gef::Vector4(DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2, 1));
	draw_camera_feed_.set_width(DISPLAY_WIDTH);
	draw_camera_feed_.set_height(virticle_image_scale_factor_ * DISPLAY_HEIGHT);

	// Apply texture to sprite
	draw_camera_feed_.set_texture(&camera_texture_map_);

	// Set up scale vector
	scale_vector.set_value(0.059f, 0.059f*virticle_image_scale_factor_, 0.059f);
	// Set projection scale
	projection_scale.set_value(1, virticle_image_scale_factor_, 1);

	// Initulize mesh renderer
	mesh_render_manager_.RenderInit(primitive_builder_);

	// reset marker tracking
	AppData* dat = sampleUpdateBegin();
	smartTrackingReset();
	sampleUpdateEnd(dat);
}

void Game_State_::GameUpdate(gef::InputManager* input_manager_)
{
	
	AppData* dat = sampleUpdateBegin();

	if (start_game_ == true)
	{
		game_state = GAME;
	}

	if (game_state == MENU)
	{
		//
		menu_state_manager_.MenuUpdate(input_manager_, start_game_);
	}


	if (game_state == GAME)
	{
		// use the tracking library to try and find markers
		smartUpdate(dat->currentImage);

		// Set scale matrix to idenity
		scale_matrix.SetIdentity();
		// Set scale of matrix
		scale_matrix.Scale(projection_scale);
		// Set world matrix to identity
		world_transform_matrix.SetIdentity();
		// Set scale of worl matrix
		world_transform_matrix.Scale(scale_vector);

		// Call mesh render update
		mesh_render_manager_.RenderUpdate(input_manager_, score);

	}

	sampleUpdateEnd(dat);
}

void Game_State_::GameRender(gef::Renderer3D* renderer_3D_, gef::SpriteRenderer* sprite_renderer_, gef::Platform& platform_, gef::Font* font_)
{
	AppData* dat = sampleRenderBegin();
	if (game_state == GAME)
	{
		sprite_renderer_->Begin(true);

		// DRAW CAMERA FEED SPRITE HERE

		// Check there is data present for the camera image before trying to draw it
		if (dat->currentImage)
		{
			camera_texture_map_.set_texture(dat->currentImage->tex_yuv);
			sprite_renderer_->DrawSprite(draw_camera_feed_);
		}


		sprite_renderer_->End();

		//
		// Render 3D scene
		//

		// SET VIEW AND PROJECTION MATRIX HERE
		// Caluclate projection matrix
		projection_matrix.PerspectiveFovGL(SCE_SMART_IMAGE_FOV, camera_image_aspect_ratio_, PROJECTION_ZNEAR, PROJECTION_ZFAR);
		// Set view matrix to identity matrix
		view_matrix.SetIdentity();

		// Scale the projection matrix
		projection_matrix = projection_matrix * scale_matrix;
		// Projectionmatrix
		renderer_3D_->set_projection_matrix(projection_matrix);
		// Veiw matrix
		renderer_3D_->set_view_matrix(view_matrix);

		// Begin rendering 3D meshes, don't clear the frame buffer
		renderer_3D_->Begin(false);

		// Render target
		mesh_render_manager_.TargetRender(renderer_3D_);
		// Render projectile
		mesh_render_manager_.ProjectileRender(renderer_3D_);

		renderer_3D_->End();

		RenderOverlay(sprite_renderer_,  &platform_, font_);

		sampleRenderEnd();
	}
}



void Game_State_::RenderOverlay(gef::SpriteRenderer* sprite_renderer_, gef::Platform* platform_, gef::Font* font_)
{
	//
	// render 2d hud on top
	//
	gef::Matrix44 proj_matrix2d;

	proj_matrix2d = platform_->OrthographicFrustum(0.0f, platform_->width(), 0.0f, platform_->height(), -1.0f, 1.0f);
	sprite_renderer_->set_projection_matrix(proj_matrix2d);
	sprite_renderer_->Begin(false);
	DrawHUD(sprite_renderer_, font_);
	sprite_renderer_->End();
}

void Game_State_::GameInitFont(gef::Font* font_, gef::Platform& platform_)
{
	font_ = new gef::Font(platform_);
	font_->Load("comic_sans");
}

void Game_State_::DrawHUD(gef::SpriteRenderer* sprite_renderer_, gef::Font* font_)
{
	if (font_)
	{
		//font_->RenderText(sprite_renderer_, gef::Vector4(850.0f, 510.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "FPS: %.1f", fps_);
		// If foud display marker is found
		if (is_found)
		{
			font_->RenderText(sprite_renderer_, gef::Vector4(10.0f, 510.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "Marker is found");
		}
		else // Else display marker is not found
		{
			font_->RenderText(sprite_renderer_, gef::Vector4(10.0f, 510.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "Marker is not found");
		}

		font_->RenderText(sprite_renderer_, gef::Vector4(850.0f, 110.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "Score: %i", score);
	}
}
