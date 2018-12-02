#ifndef _GAME_STATE__H
#define _GAME_STATE__H

#include <system/application.h>
#include <iostream>
#include <platform/vita/graphics/texture_vita.h>
#include <sony_sample_framework.h>
#include <input\input_manager.h>
#include <input\sony_controller_input_manager.h>
#include "primitive_builder.h"
#include <graphics/sprite.h>
#include "Menu_State.h"
#include "Mesh_Render.h"

// Vita AR includes
#include <camera.h>
#include <gxm.h>
#include <motion.h>
#include <libdbg.h>
#include <libsmart.h>

// Math
#include <math.h>

using std::pow;
using std::sqrt;


// FRAMEWORK FORWARD DECLARATIONS
namespace gef
{
	class Platform;
	class SpriteRenderer;
	class Font;
	class Renderer3D;
	class Mesh;
	class Scene;
	class RenderTarget;
	class TextureVita;
	class InputManager;
}

class Game_State_
{
public:
	Game_State_();
	~Game_State_();

	void GameInit(PrimitiveBuilder* primitive_builder_, gef::Platform* platform_);
	void GameUpdate(gef::InputManager* input_manager_);
	void GameRender(gef::Renderer3D* renderer_3, gef::SpriteRenderer* sprite_renderer_, gef::Platform& platform_, gef::Font* font_);
	void RenderOverlay(gef::SpriteRenderer* sprite_renderer_, gef::Platform* platform_, gef::Font* font_);
	void DrawHUD(gef::SpriteRenderer* sprite_renderer_, gef::Font* font_);


	
	// Varibales for AR projection and mesh
	gef::Matrix44 ortho_projection_matrix;
	gef::Matrix44 projection_matrix;
	gef::Matrix44 view_matrix;
	gef::Sprite draw_camera_feed_;
	gef::TextureVita camera_texture_map_;
	// Matrixs and vectors for AR scale
	gef::Matrix44 transform_matrix;
	gef::Vector4 scale_vector;
	gef::Matrix44 world_transform_matrix;
	gef::Matrix44 scale_matrix;
	gef::Vector4 projection_scale;

	float camera_image_aspect_ratio_;
	float display_aspect_ratio_;
	float virticle_image_scale_factor_;


	// Has the marker been found
	bool is_found;
	// Game has started
	bool start_game_;
	// amount of markers
	float marker_amount_;
	// Score
	int score;

	// Game states
	enum Game_State
	{
		MENU,
		GAME,
		EXIT
	};

	// Game state
	Game_State game_state;


	

	
}; 
#endif

