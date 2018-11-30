#ifndef _MENU_STATE_H
#define _MENU_STATE_H

#include <system/application.h>
#include <iostream>;
#include <platform/vita/graphics/texture_vita.h>
#include <sony_sample_framework.h>
#include <input\input_manager.h>
#include <input\sony_controller_input_manager.h>

using namespace std;
using std::string;

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

	void MenuInit(bool& start_game_);
	void MenuUpdate(gef::InputManager* input_manager_, bool& start_game_);
	

}; 
#endif

