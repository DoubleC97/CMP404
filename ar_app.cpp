#include "ar_app.h"
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

// Math
#include <math.h>

using std::pow;
using std::sqrt;


// Mesh render manager
//Mesh_Render mesh_render_manager_;
// Menu manager
//Menu_State menu_state_manager_;

//
Game_State_ game_state_manager;

ARApp::ARApp(gef::Platform& platform) :
	Application(platform),
	input_manager_(NULL),
	sprite_renderer_(NULL),
	font_(NULL),
	renderer_3d_(NULL),
	primitive_builder_(NULL)
{
}

void ARApp::Init()
{
	input_manager_ = gef::InputManager::Create(platform_);
	sprite_renderer_ = gef::SpriteRenderer::Create(platform_);
	renderer_3d_ = gef::Renderer3D::Create(platform_);
	primitive_builder_ = new PrimitiveBuilder(platform_);

	InitFont();
	SetupLights();

	game_state_manager.GameInit(primitive_builder_);

	// Game state
	//game_state = MENU;

	/*if (game_state == MENU)
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
}*/
}

void ARApp::CleanUp()
{
	delete primitive_builder_;
	primitive_builder_ = NULL;

	smartRelease();
	sampleRelease();

	CleanUpFont();
	delete sprite_renderer_;
	sprite_renderer_ = NULL;

	delete renderer_3d_;
	renderer_3d_ = NULL;

	delete input_manager_;
	input_manager_ = NULL;

}

bool ARApp::Update(float frame_time)
{

	
	fps_ = 1.0f / frame_time;

	input_manager_->Update();

	//AppData* dat = sampleUpdateBegin();

	game_state_manager.GameUpdate(input_manager_);

	/*if (start_game_ == true)
	{
		game_state = GAME;
	}

	if (game_state == MENU)
	{
		//
		//menu_state_manager_.MenuUpdate(input_manager_, start_game_);
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
		//mesh_render_manager_.RenderUpdate(input_manager_, score);

	}*/

	//sampleUpdateEnd(dat);

	return true;
}

void ARApp::Render()
{
	
	game_state_manager.GameRender(renderer_3d_, sprite_renderer_, platform_, font_);

	/*AppData* dat = sampleRenderBegin();
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
		renderer_3d_->set_projection_matrix(projection_matrix);
		// Veiw matrix
		renderer_3d_->set_view_matrix(view_matrix);

		// Begin rendering 3D meshes, don't clear the frame buffer
		renderer_3d_->Begin(false);

		// Render mirror
		//mesh_render_manager_.TargetRender(renderer_3d_);
		// Render beam
		//mesh_render_manager_.ProjectileRender(renderer_3d_);

		renderer_3d_->End();

		RenderOverlay();

		sampleRenderEnd();
	}*/
}


void ARApp::RenderOverlay()
{
	//
	// render 2d hud on top
	//
	/*gef::Matrix44 proj_matrix2d;

	proj_matrix2d = platform_.OrthographicFrustum(0.0f, platform_.width(), 0.0f, platform_.height(), -1.0f, 1.0f);
	sprite_renderer_->set_projection_matrix(proj_matrix2d);
	sprite_renderer_->Begin(false);
	DrawHUD();
	sprite_renderer_->End();*/

	game_state_manager.RenderOverlay(sprite_renderer_, &platform_, font_);
}


void ARApp::InitFont()
{

	//game_state_manager.GameInitFont(font_, platform_);
	font_ = new gef::Font(platform_);
	font_->Load("comic_sans");
}

void ARApp::CleanUpFont()
{
	delete font_;
	font_ = NULL;
}

void ARApp::DrawHUD()
{
	
	/*if(font_)
	{
		font_->RenderText(sprite_renderer_, gef::Vector4(850.0f, 510.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "FPS: %.1f", fps_);
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
	}*/

	game_state_manager.DrawHUD(sprite_renderer_, font_);
}

void ARApp::SetupLights()
{
	gef::PointLight default_point_light;
	default_point_light.set_colour(gef::Colour(0.7f, 0.7f, 1.0f, 1.0f));
	default_point_light.set_position(gef::Vector4(-300.0f, -500.0f, 100.0f));

	gef::Default3DShaderData& default_shader_data = renderer_3d_->default_shader_data();
	default_shader_data.set_ambient_light_colour(gef::Colour(0.5f, 0.5f, 0.5f, 1.0f));
	default_shader_data.AddPointLight(default_point_light);
}
