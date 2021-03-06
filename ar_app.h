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
#include "Game_State_.h"

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

	class gef::Renderer3D* renderer_3d_;
	PrimitiveBuilder* primitive_builder_;

	float fps_;
	// Timer
	float timer;

};


#endif // _RENDER_TARGET_APP_H