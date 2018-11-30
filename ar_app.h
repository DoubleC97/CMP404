#ifndef _RENDER_TARGET_APP_H
#define _RENDER_TARGET_APP_H

#include <system/application.h>
#include <graphics/sprite.h>
#include <maths/vector2.h>
#include <vector>
#include <graphics/mesh_instance.h>
#include <platform/vita/graphics/texture_vita.h>
#include "primitive_builder.h"
#include "Collision.h"
#include "Mesh_Render.h"
#include "Menu_State.h"

// Vita AR includes
#include <camera.h>
#include <gxm.h>
#include <motion.h>
#include <libdbg.h>
#include <libsmart.h>

// FRAMEWORK FORWARD DECLARATIONS
namespace gef
{
	class Platform;
	class SpriteRenderer;
	class Font;
	class Renderer3D;
	class Mesh;
	class RenderTarget;
	class TextureVita;
	class InputManager;
}


class ARApp : public gef::Application
{
public:
	ARApp(gef::Platform& platform);
	void Init();
	void CleanUp();
	bool Update(float frame_time);
	void Render();
private:
	void InitFont();
	void CleanUpFont();
	void DrawHUD();

	void RenderOverlay();
	void SetupLights();

	gef::InputManager* input_manager_;

	gef::SpriteRenderer* sprite_renderer_;
	gef::Font* font_;

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

	float fps_;
	float camera_image_aspect_ratio_;
	float display_aspect_ratio_; 
	float virticle_image_scale_factor_;

	class gef::Renderer3D* renderer_3d_;
	PrimitiveBuilder* primitive_builder_;
	
	bool is_found;
	float marker_amount_;
;
	// Temp untilrender class is working
	gef::MeshInstance mirror_cube_[5];
	gef::MeshInstance beam_mesh_;

	// Score
	int score;

	enum Game_State
	{
		MENU,
		GAME,
		EXIT
	};

	Game_State game_state;
	// Game has started
	bool start_game_;

};


#endif // _RENDER_TARGET_APP_H