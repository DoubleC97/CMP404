#ifndef _MENU_STATE_H
#define _MENU_STATE_H

#include <system/application.h>
#include <iostream>
#include <platform/vita/graphics/texture_vita.h>
#include <sony_sample_framework.h>
#include <input\input_manager.h>
#include <input\sony_controller_input_manager.h>
#include <graphics\sprite.h>
#include <graphics/sprite_renderer.h>
#include <system/platform.h>
#include <assets/png_loader.h>
#include <graphics/image_data.h>
#include <system/platform.h>

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

class Menu_State
{
public:
	Menu_State();
	~Menu_State();

	void MenuInit(bool& start_game_, gef::Platform* platform_);
	void MenuUpdate(gef::InputManager* input_manager_, bool& start_game_);
	void MenuRender(gef::SpriteRenderer* sprite_renderer_, gef::Platform* platform_);

private:

	// Menu PNG
	gef::Texture* CreateTexturefromPNG(const char* PNG_file, gef::Platform& platform_);
	gef::Texture* menu_texture_;
	gef::Sprite menu_sprite_;

	

}; 
#endif

