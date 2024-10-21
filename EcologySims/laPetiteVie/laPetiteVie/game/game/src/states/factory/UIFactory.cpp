#include "UIFactory.h"
#include "../choiceBox/ChoiceBoxState.h"
#include <thsan/ressourceManager/StateManager.h>
#include <thsan/ressourceManager/ThemeManager.h>

UIFactory::UIFactory(std::shared_ptr<State> current):
    current(current)
{

}

std::shared_ptr<State> UIFactory::createPlayerActionChoiceBox(GameObject& player, Scene& scene)
{
	// verification if player is the actual player

    if (player.getName() != "player" || !player.hasComponent(COMPONENT_DATA::PLAYER))
        return nullptr;

	// initialization of value required for the ChoiceBox
    std::shared_ptr<UI::ShaderUIparam> shader_param_ptr = RessourceManager::ThemeManager::get("media/data/theme/FF7.json");

    sf::FloatRect player_rect = scene.querySpriteRectAssociateWith("player", true);
    sf::Vector2f position = sf::Vector2f(player_rect.left + 0.10 + player_rect.width / 1.618f, player_rect.top - 0.10f);

	////////////////////////////////////
	//	Delcaration of label buttons  //
	////////////////////////////////////

	std::vector<UI::LabelButton*> bttns;
	UI::LabelButton* lbl_bttn_eat = new UI::LabelButton("lbl_bttn_action_player_eat", "media/font/Final_Fantasy_VII/Final_Fantasy_VII.ttf", sf::Vector2f(0.15, -0.15), "eat", 0.6f, sf::Color::White);
	UI::LabelButton* lbl_bttn_plant = new UI::LabelButton("lbl_bttn_action_player_plant", "media/font/Final_Fantasy_VII/Final_Fantasy_VII.ttf", sf::Vector2f(0.55, -0.15), "plant", 0.6f, sf::Color::White);
	UI::LabelButton* lbl_bttn_throw = new UI::LabelButton("lbl_bttn_action_player_throw", "media/font/Final_Fantasy_VII/Final_Fantasy_VII.ttf", sf::Vector2f(0.15, 0.15), "throw", 0.6f, sf::Color::White);
	UI::LabelButton* lbl_bttn_wear = new UI::LabelButton("lbl_bttn_action_player_wear", "media/font/Final_Fantasy_VII/Final_Fantasy_VII.ttf", sf::Vector2f(0.55, 0.15), "wear", 0.6f, sf::Color::White);
	
	///////////////////////////////////
	//	Set label buttons direction  //
	///////////////////////////////////

	lbl_bttn_eat->left = lbl_bttn_plant;
	lbl_bttn_eat->right = lbl_bttn_plant;
	lbl_bttn_eat->up = lbl_bttn_throw;
	lbl_bttn_eat->down = lbl_bttn_throw;

	lbl_bttn_plant->left = lbl_bttn_eat;
	lbl_bttn_plant->right = lbl_bttn_eat;
	lbl_bttn_plant->up = lbl_bttn_wear;
	lbl_bttn_plant->down = lbl_bttn_wear;

	lbl_bttn_throw->left = lbl_bttn_wear;
	lbl_bttn_throw->right = lbl_bttn_wear;
	lbl_bttn_throw->up = lbl_bttn_eat;
	lbl_bttn_throw->down = lbl_bttn_eat;

	lbl_bttn_wear->left = lbl_bttn_throw;
	lbl_bttn_wear->right = lbl_bttn_throw;
	lbl_bttn_wear->up = lbl_bttn_plant;
	lbl_bttn_wear->down = lbl_bttn_plant;

	///////////////////////////////////
	//	Connect label buttons event  //
	///////////////////////////////////

	// eat ////////////////////////////

	lbl_bttn_eat->connect(UI::Action::crossed, [lbl_bttn_eat]() {
		lbl_bttn_eat->setFontColor(sf::Color::White);
		});

	lbl_bttn_eat->connect(UI::Action::hover, [lbl_bttn_eat]() {
		if (lbl_bttn_eat)
			lbl_bttn_eat->setFontColor(sf::Color(255, 128, 0));
		});

	lbl_bttn_eat->connect(UI::Action::pressed, [&]() {

	});

	// plant ////////////////////////////

	lbl_bttn_plant->connect(UI::Action::crossed, [lbl_bttn_plant]() {
		lbl_bttn_plant->setFontColor(sf::Color::White);
	});

	lbl_bttn_plant->connect(UI::Action::hover, [lbl_bttn_plant]() {
		if (lbl_bttn_plant)
			lbl_bttn_plant->setFontColor(sf::Color::Magenta);
	});

	lbl_bttn_plant->connect(UI::Action::pressed, [&]() {
		// todo
	});

	// throw ////////////////////////////

	lbl_bttn_throw->connect(UI::Action::crossed, [lbl_bttn_throw]() {
		lbl_bttn_throw->setFontColor(sf::Color::White);
	});

	lbl_bttn_throw->connect(UI::Action::hover, [lbl_bttn_throw]() {
		if (lbl_bttn_throw)
			lbl_bttn_throw->setFontColor(sf::Color::Cyan);
	});

	lbl_bttn_throw->connect(UI::Action::pressed, [&]() {

	});

	// wear ////////////////////////////

	lbl_bttn_wear->connect(UI::Action::crossed, [lbl_bttn_wear]() {
		lbl_bttn_wear->setFontColor(sf::Color::White);
	});

	lbl_bttn_wear->connect(UI::Action::hover, [lbl_bttn_wear]() {
		if (lbl_bttn_wear)
			lbl_bttn_wear->setFontColor(sf::Color::Green);
	});

	lbl_bttn_wear->connect(UI::Action::pressed, [&]() {

	});

	///////////////////////////////////////
	//  pack label button for choiceBox  //
	///////////////////////////////////////

	bttns.push_back(lbl_bttn_eat);
	bttns.push_back(lbl_bttn_plant);
	bttns.push_back(lbl_bttn_throw);
	bttns.push_back(lbl_bttn_wear);

	///////////////////////////////////////
	//  create ChoiceBox for action      //
	///////////////////////////////////////
	// 
	//vite vite explanation: current scene is for UIFactory. actual current scene that current scene isn't the same that the one of UIFactory.
	std::shared_ptr<State> actual_curr = scene.getParent()->getParent()->peekState();
	std::shared_ptr<State> actionChoiceBox = RessourceManager::StateManager::create<ChoiceBoxState>(
		actual_curr->getParent(),
		actual_curr,
		position,
		sf::Vector2f(0.2, 0.10),
		*shader_param_ptr,
		bttns,
		sf::Color::White,
		true
	);
		
	return actionChoiceBox;
}

std::shared_ptr<State> UIFactory::createPlayerListChoiceBox(GameObject& player, Scene& scene)
{
	// verification if player is the actual player

	if (player.getName() != "player" || !player.hasComponent(COMPONENT_DATA::PLAYER))
		return nullptr;

	// initialization of value required for the item listbox
	std::shared_ptr<UI::ShaderUIparam> shader_param_ptr = RessourceManager::ThemeManager::get("media/data/theme/FF7.json");

	sf::FloatRect player_rect = scene.querySpriteRectAssociateWith("player", true);
	sf::Vector2f position = sf::Vector2f(player_rect.left + player_rect.width / 1.618f, player_rect.top - 0.05f);


	////////////////////////////////////
	//	Delcaration of label buttons  //
	////////////////////////////////////

	std::vector<UI::LabelButton*> bttns;

	const int I_MAX = 1;
	const int J_MAX = 6;

	std::array<UI::LabelButton*, I_MAX* J_MAX> lbl_bbtns{nullptr};
	{
		int i = 0.f;
		int j = 0.f;

		/*
		* 
		* TODO: à la place de loop 5 fois, lmao essaie d'avoir des actuels plants
		* 
		*/
		for (int k = 0; k < 5; k++) {

			UI::LabelButton* curr = new UI::LabelButton("lbl_bttn_plant_" + std::to_string(k),
				"media/font/Final_Fantasy_VII/Final_Fantasy_VII.ttf",
				sf::Vector2f(0.1f + i * 0.5f, j * 0.1f),
				"plant num " + std::to_string(k),
				0.25f,
				sf::Color::White
			);

			///////////////////////////////////
			//	Connect label buttons event  //
			///////////////////////////////////

			// eat ////////////////////////////

			curr->connect(UI::Action::crossed, [curr]() {
				curr->setFontColor(sf::Color::White);
				});

			curr->connect(UI::Action::hover, [curr]() {
				if (curr)
					curr->setFontColor(sf::Color(255, 128, 0));
				});

			curr->connect(UI::Action::pressed, [&]() {
				UIFactory factory(scene.getParent()->getParent()->peekState());
				std::shared_ptr<State> state = factory.createPlayerActionChoiceBox(player, scene);
				scene.getParent()->getParent()->changeState(state, true);
				});

			lbl_bbtns[i + j * I_MAX] = curr;
			i++;
			if (i >= I_MAX) {
				i = 0;
				j++;
			}
			if (j >= J_MAX)
				break;
		}
	}
	for (int i = 0; i < I_MAX; i++)
		for (int j = 0; j < J_MAX; j++) {
			UI::LabelButton* curr{ nullptr };
			UI::LabelButton* left{ nullptr };
			UI::LabelButton* right{ nullptr };
			UI::LabelButton* up{ nullptr };
			UI::LabelButton* down{ nullptr };

			curr = lbl_bbtns[i + j * I_MAX];

			//right
			if (i + 1 < I_MAX)
				right = lbl_bbtns[(i + 1) + j * I_MAX];
			else
				right = lbl_bbtns[0 + j * I_MAX];

			//left
			if (i > 0)
				left = lbl_bbtns[(i - 1) + j * I_MAX];
			else
				left = lbl_bbtns[(I_MAX - 1) + j * I_MAX];
			

			//down
			if (j + 1 < J_MAX)
				down = lbl_bbtns[i + (j + 1) * I_MAX];
			else
				down = lbl_bbtns[i + 0 * I_MAX];

			//up
			if (j > 0)
				up = lbl_bbtns[i + (j - 1) * I_MAX];
			else
				up = lbl_bbtns[i + (J_MAX - 1) * I_MAX];
			

			if (curr) {
				curr->up = up;
				curr->down = down;
				curr->left = left;
				curr->right = right;
			}
		}

	for(auto& bttn: lbl_bbtns)
		bttns.push_back(bttn);

	//////////////////////////////////////////////////////////////////////
	// create or retrieve and update the listchoiceBoxState if it exist //
	//////////////////////////////////////////////////////////////////////

	std::shared_ptr<ChoiceBoxState> listChoiceBox{ nullptr };

	if (RessourceManager::StateManager::there_exists(playerListChoiceBoxId))
	{
		listChoiceBox = RessourceManager::StateManager::get<ChoiceBoxState>(playerListChoiceBoxId);
		listChoiceBox->setChoices(bttns);
	}
	else{

		listChoiceBox = RessourceManager::StateManager::create<ChoiceBoxState>(
			current->getParent(),
			current,
			position,
			sf::Vector2f(0.2, 0.3),
			*shader_param_ptr,
			bttns,
			sf::Color::White
		);

		playerListChoiceBoxId = listChoiceBox->getId();
	}

	return listChoiceBox;
}
