#include "Menu_State.h"



Menu_State::Menu_State()
{
}


Menu_State::~Menu_State()
{
}

// Init()
void Menu_State::MenuInit(bool& start_game_)
{
	start_game_ = false;
}
// Update()
void Menu_State::MenuUpdate(gef::InputManager* input_manager_, bool& start_game_)
{
	// read input devices
	if (input_manager_)
	{
		input_manager_->Update();

		// controller input
		const gef::SonyController* controller_manager = input_manager_->controller_input()->GetController(0);
		// Check if start has been pressed
		if (controller_manager->buttons_pressed() & gef_SONY_CTRL_START)
		{
			// Start game is true
			start_game_ = true;
		}
		// Check if slelct has been pressed
		if (controller_manager->buttons_pressed() & gef_SONY_CTRL_SELECT)
		{
			// Exit applicaiotn
		}

	}
}
