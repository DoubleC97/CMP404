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



// Game state manager
Game_State_ game_state_manager;
// Menu state render  manager
Menu_State menu_render_manager;

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

	// Call game sttae init
	game_state_manager.GameInit(primitive_builder_, &platform_);

	// Set timer to 60.0f;
	timer = 30.0f;
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

	// Chck if game has started
	if (game_state_manager.start_game_ == true)
	{
		// Remove time
		timer -= frame_time;
	}

	// Call to game update
	game_state_manager.GameUpdate(input_manager_, timer);

	

	return true;
}

void ARApp::Render()
{

	// Check if the game has started
	if (game_state_manager.start_game_ == false)
	{
		// Call to menu render
		menu_render_manager.MenuRender(sprite_renderer_, &platform_); 
	}
	// Check if the game has started
	if (game_state_manager.start_game_ == true)
	{
		// Call to game render
		game_state_manager.GameRender(renderer_3d_, sprite_renderer_, platform_, font_, fps_, timer);
	}

}


void ARApp::RenderOverlay()
{
	// Call to render overlay
	game_state_manager.RenderOverlay(sprite_renderer_, &platform_, font_, fps_, timer);
}


void ARApp::InitFont()
{
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
	// Call to draw HUD
	game_state_manager.DrawHUD(sprite_renderer_, font_, fps_, timer);
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
