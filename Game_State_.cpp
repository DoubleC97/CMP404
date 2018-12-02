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

void Game_State_::GameInit(PrimitiveBuilder* primitive_builder_, gef::Platform* platform_)
{
	
	// Start Game
	start_game_ = false;

	// Game State menu
	game_state = MENU;

	if (game_state == MENU)
	{
		// Init menu
		menu_state_manager_.MenuInit(start_game_, platform_);

	}

	/*if (game_state == WIN)
	{*/
		// Load sprite for win screen
		win_texture_ = CreateTexturefromPNG("Win.png", *platform_);
	//}

	// Set trsansfor mamtirx to identity
	transform_matrix.SetIdentity();
	//  set scale of transform_matrix
	transform_matrix.Scale(gef::Vector4(0.059f, 0.059f, 0.059f));

	// Set marker amount 6
	marker_amount_ = 6;

	// Set score to 0
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
	mesh_render_manager_.RenderInit(primitive_builder_, platform_);

	// reset marker tracking
	AppData* dat = sampleUpdateBegin();
	smartTrackingReset();
	sampleUpdateEnd(dat);
}

void Game_State_::GameUpdate(gef::InputManager* input_manager_, float& timer)
{
	
	AppData* dat = sampleUpdateBegin();

	// Check if game has started
	if (start_game_ == true)
	{
		// Change state to gme
		game_state = GAME;
	}
	// Check if state is menu
	if (game_state == MENU)
	{
		// Call menu update
		menu_state_manager_.MenuUpdate(input_manager_, start_game_);
	}

	// Check timer has reached 0 
	if (timer <= 0)
	{
		// Change satte to win
		game_state = WIN;
	}

	// Check if state is game
	if (game_state == GAME || game_state == WIN)
	{
		// use the tracking library to try and find markers
		smartUpdate(dat->currentImage);

		// Set scale matrix to idenity
		scale_matrix.SetIdentity();
		// Set scale of matrix
		scale_matrix.Scale(projection_scale);
		// Set world matrix to identity
		world_transform_matrix.SetIdentity();
		// Set scale of world matrix
		world_transform_matrix.Scale(scale_vector);

		// Call mesh render update
		mesh_render_manager_.RenderUpdate(input_manager_, score);

		// read input devices
		if (input_manager_)
		{
			input_manager_->Update();

			// controller input
			const gef::SonyController* controller_manager = input_manager_->controller_input()->GetController(0);
			// Check if start has been pressed
			if (controller_manager->buttons_pressed() & gef_SONY_CTRL_START)
			{
				// Return to menu state
				game_state = MENU;
				// Game has not started
				start_game_ = false;
				// Reset score
				score = 0;
				// Reset timer
				timer = 30.0f;

			}

			// Check if triangle has been pressed to reset the game
			if (controller_manager->buttons_pressed() & gef_SONY_CTRL_TRIANGLE)
			{
				// Reset score
				score = 0;
				// Reset timer
				timer = 30.0f;

			}

			// Check if slelct has been pressed
			if (controller_manager->buttons_pressed() & gef_SONY_CTRL_SELECT)
			{
				// Exit applicaiotn
				exit(0); 
			}

		}
	}

	sampleUpdateEnd(dat);
}

void Game_State_::GameRender(gef::Renderer3D* renderer_3D_, gef::SpriteRenderer* sprite_renderer_, gef::Platform& platform_, gef::Font* font_, float fps_, float& timer)
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
		// End render 3D
		renderer_3D_->End();

		// Render overlay
		RenderOverlay(sprite_renderer_,  &platform_, font_, fps_, timer);

		sampleRenderEnd();
	}


	// Check if game state is win
	if (game_state == WIN)
	{

		// Begin sprit render
		sprite_renderer_->Begin();
		// Set win texture
		win_sprite_.set_texture(win_texture_);
		// Set postion
		win_sprite_.set_position(gef::Vector4(platform_.width() / 2, platform_.height() / 2, 0.0f, 0.0f));
		// Set height
		win_sprite_.set_height(platform_.height());
		// Set width
		win_sprite_.set_width(platform_.width());
		// Draw sprite
		sprite_renderer_->DrawSprite(win_sprite_);
		// End sprite rendeer
		sprite_renderer_->End();
	}
}

gef::Texture* Game_State_::CreateTexturefromPNG(const char* PNG_file, gef::Platform& platform_)
{
	gef::PNGLoader PNG_loader_;
	gef::ImageData image_data_;
	gef::Texture* texture_ = NULL;

	// Load image data form PNG
	PNG_loader_.Load(PNG_file, platform_, image_data_);
	// If the image data is valid then create a texture from it
	if (image_data_.image() != NULL)
	{
		texture_ = gef::Texture::Create(platform_, image_data_);
	}

	// return a texture
	return texture_;
}

void Game_State_::RenderOverlay(gef::SpriteRenderer* sprite_renderer_, gef::Platform* platform_, gef::Font* font_, float fps_, float& timer)
{
	//
	// render 2d hud on top
	//
	gef::Matrix44 proj_matrix2d;

	proj_matrix2d = platform_->OrthographicFrustum(0.0f, platform_->width(), 0.0f, platform_->height(), -1.0f, 1.0f);
	sprite_renderer_->set_projection_matrix(proj_matrix2d);
	sprite_renderer_->Begin(false);
	DrawHUD(sprite_renderer_, font_, fps_, timer);
	sprite_renderer_->End();
}


void Game_State_::DrawHUD(gef::SpriteRenderer* sprite_renderer_, gef::Font* font_, float fps_, float& timer)
{
	if (font_)
	{
		font_->RenderText(sprite_renderer_, gef::Vector4(850.0f, 510.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "FPS: %.1f", fps_);
		// If foud display marker is found
		if (is_found == true)
		{
			font_->RenderText(sprite_renderer_, gef::Vector4(10.0f, 510.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "Marker is found");
		}
		else // Else display marker is not found
		{
			font_->RenderText(sprite_renderer_, gef::Vector4(10.0f,110.0, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "Marker is not found");
			font_->RenderText(sprite_renderer_, gef::Vector4(10.0f, 130.0, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "If this continues try moving closer ");
			font_->RenderText(sprite_renderer_, gef::Vector4(10.0f, 150.0, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "further away form the makrer, or try restarting the game ");
		}

		font_->RenderText(sprite_renderer_, gef::Vector4(10.0f, 210.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "To restart game press Triangle");
		font_->RenderText(sprite_renderer_, gef::Vector4(10.0f, 230.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "To quit game press select");
		font_->RenderText(sprite_renderer_, gef::Vector4(10.0f, 250.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "To return to menu press start");

		font_->RenderText(sprite_renderer_, gef::Vector4(850.0f, 110.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "Score: %i", score);
		font_->RenderText(sprite_renderer_, gef::Vector4(850.0f, 90.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "Time: %f", timer);

		// If game has been won
		if (game_state == WIN)
		{
			// Display final score
			font_->RenderText(sprite_renderer_, gef::Vector4(10.0f, 210.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "%i", score);
		}
	}
}

