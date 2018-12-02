#include "Menu_State.h"



Menu_State::Menu_State()
{
}


Menu_State::~Menu_State()
{
}

// Init()
void Menu_State::MenuInit(bool& start_game_, gef::Platform* platform_)
{
	// Set start game to false
	start_game_ = false;

	// Load texture
	menu_texture_ = CreateTexturefromPNG("Menu.png", *platform_);
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


void Menu_State::MenuRender(gef::SpriteRenderer* sprite_renderer_, gef::Platform* platform_)
{
	// Begin sprit render
	sprite_renderer_->Begin();
	// Set menu texture
	menu_sprite_.set_texture(menu_texture_);
	// Set postion
	menu_sprite_.set_position(gef::Vector4(platform_->width() / 2, platform_->height() / 2, 0.0f, 0.0f));
	// Set height
	menu_sprite_.set_height(platform_->height());
	// Set width
	menu_sprite_.set_width(platform_->width());
	// Draw sprite
	sprite_renderer_->DrawSprite(menu_sprite_);
	// End sprite rendeer
	sprite_renderer_->End();
}

gef::Texture* Menu_State::CreateTexturefromPNG(const char* PNG_file, gef::Platform& platform_)
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
