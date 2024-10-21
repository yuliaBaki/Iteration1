#include "Map0000.h"

#include "../../gameObjects/camera/CameraInputComponent.h"
#include "../../gameObjects/camera/FollowCameraPhysicComponent.h"
#include "../../gameObjects/camera/CameraRenderComponent.h"

#include "../../gameObjects/player/PlayerInputComponent.h"
#include "../../gameObjects/player/PlayerPhysicComponent.h"
#include "../../gameObjects/player/PlayerSpriteRenderComponent.h"

#include "../../gameObjects/NPC/SimpleDialogNPCInputComponent.h"
#include "../../gameObjects/NPC/FixedOnGroundPhysicComponent.h"
#include "../../gameObjects/NPC/OrangeCuteRenderComponent.h"

#include "../dialogue/DialogueState.h"
#include "../choiceBox/ChoiceBoxState.h"

#include <thsan/state/transition/TransitionState.h>
#include <thsan/state/transition/transition/Transition.h>
#include <thsan/ressourceManager/StateManager.h>
#include <thsan/ressourceManager/ThemeManager.h>
#include <thsan/util/helper.h>
#include "Map0001.h"


Map0000::Map0000(Game* parent):
	MapState(parent)
{

}

void Map0000::init()
{
	m_scene = new Scene(this, "media/data/map/winter/winter.json", VIEW_TYPE::FIXED_VIEW);

	GameObject* player = m_scene->createGameObject("player");
	m_scene->setGameObjectInputComponent<PlayerInputComponent>(player);
	m_scene->setGameObjectPhysicComponent<PlayerPhysicComponent>(player);
	m_scene->setGameObjectRenderComponent<PlayerSpriteRenderComponent>(player);

	GameObject* orange = m_scene->createGameObject("orangeCute_npc");
	m_scene->setGameObjectInputComponent<SimpleDialogNPCInputComponent>(orange);
	m_scene->setGameObjectPhysicComponent<FixedOnGroundPhysicComponent>(orange);
	m_scene->setGameObjectRenderComponent<OrangeCuteRenderComponent>(orange);

	
	auto tr = orange->getComponent<Transform>(COMPONENT_DATA::TRANSFORM);
	tr->position.x -= 28.f;
	
	/*
	GameObject* go = scene.createGameObject("camera");
	scene.setGameObjectInputComponent<CameraInputComponent>(go);
	scene.setGameObjectPhysicComponent<FollowCameraPhysicComponent>(go, player);
	scene.setGameObjectRenderComponent<CameraRenderComponent>(go);

	class CameraController : public NativeScriptComponent {

		void init(Scene& scene) override {
			CameraData* cd = parent->getComponent<CameraData>(COMPONENT_DATA::CAMERA);

			cd->offset.y = 15.f;
			cd->angle = 0.f;
			cd->offset_distance = 70.f;
			cd->isPolar = true;
		};
		void update(Scene& scene, const sf::Time& dt) override {

		};

		void onDelete(Scene& scene) override {

		};
	};

	go->setNativeScript<CameraController>();
	*/
	UI::ShaderUIparam shader_param;
	shader_param.borderSize[0] = 0.25f;
	shader_param.borderSize[1] = 0.25f;
	shader_param.borderSize[2] = 0.25f;
	shader_param.borderSize[3] = 0.25f;
	shader_param.colorBg[0] = sf::Color(0, 0, 168);
	shader_param.colorBg[1] = sf::Color(0, 0, 128);
	shader_param.colorBg[2] = sf::Color::Black;
	shader_param.colorBg[3] = sf::Color(0, 0, 128);
	shader_param.colorBorder = sf::Color(220, 220, 220);
	shader_param.colorBorderBorders = sf::Color(255,0,0, 128);
	shader_param.cornerRadius[0] = 0.05f;
	shader_param.cornerRadius[1] = 0.05f;
	shader_param.cornerRadius[2] = 0.05f;
	shader_param.cornerRadius[3] = 0.05f;
	shader_param.type = UI::Type::fill;
	shader_param.enableImage = true;
	shader_param.p_texture = RessourceManager::TextureManager::get("media/image/1024/terrain.png");


	std::shared_ptr<UI::ShaderUIparam> shader_param_ptr = RessourceManager::ThemeManager::get("media/data/theme/FF7.json");

	std::shared_ptr<State> curr_state = RessourceManager::StateManager::get<State>(this->getId());

	std::shared_ptr<DialogueState> dialogue = RessourceManager::StateManager::create<DialogueState>(m_parent, curr_state, sf::Vector2f(0.15f, 0.35f), sf::Vector2f(0.4, 0.2), "media/data/dialogue/test.txt", *shader_param_ptr, sf::Color::Red, 0.3f, 0.0f);
	
	std::vector<UI::LabelButton*> bttns;
	UI::LabelButton* lbl_bttn_1 = new UI::LabelButton("lbl_bttn_1", "media/font/Final_Fantasy_VII/Final_Fantasy_VII.ttf", sf::Vector2f(0.15, -0.35), "oui", 1.0f, sf::Color::White);
	UI::LabelButton* lbl_bttn_2 = new UI::LabelButton("lbl_bttn_2", "media/font/Final_Fantasy_VII/Final_Fantasy_VII.ttf", sf::Vector2f(0.55, -0.35), "non", 1.0f, sf::Color::White);
	lbl_bttn_1->left = lbl_bttn_2;
	lbl_bttn_1->right = lbl_bttn_2;

	lbl_bttn_2->left = lbl_bttn_1;
	lbl_bttn_2->right = lbl_bttn_1;

	lbl_bttn_2->connect(UI::Action::pressed, [&]() {m_scene->createPointLight(sf::Vector3f(100.f, 100.f, 75.f)); });
	lbl_bttn_1->connect(UI::Action::crossed, [lbl_bttn_1]() {lbl_bttn_1->setFontColor(sf::Color::White); });

	lbl_bttn_1->connect(UI::Action::hover, [lbl_bttn_1]() {
		if(lbl_bttn_1)
			lbl_bttn_1->setFontColor(sf::Color::Cyan);
	});


	lbl_bttn_1->connect(UI::Action::pressed, [&]() {
		std::shared_ptr<State> peek = this->getParent()->peekState();
		
		sf::Vector2u resolution = sf::Vector2u(this->getParent()->getWidth(), this->getParent()->getHeight());
		//PostProcess::Letterbox_param::Bar, sf::Color, Ease, InterpolationType
		std::unique_ptr<Transition> transition = std::make_unique<Transition>(
			sf::seconds(1.0f),
			sf::seconds(2.0f),
			Transition::Type::fade_to_color,
			Transition::Type::fade_to_color,
			Transition::Fade_type::step,
			Transition::Fade_type::step,
			sf::Color::Black,
			sf::Color::Cyan,
			sf::Color::Black,
			sf::Color::Black
		);

		auto current = RessourceManager::StateManager::get<State>(this->getId());
		std::shared_ptr<TransitionState<Map0001>> interpolate = RessourceManager::StateManager::create_transition<Map0001>(this->getParent(), current, transition);
		//auto interpolate = RessourceManager::StateManager::create_transition(this->getParent(), current, hxhstate, transition);
		this->getParent()->changeState(interpolate, true);
	});

	lbl_bttn_2->connect(UI::Action::crossed, [lbl_bttn_2]() {lbl_bttn_2->setFontColor(sf::Color::White); });
	lbl_bttn_2->connect(UI::Action::hover, [lbl_bttn_2]() {lbl_bttn_2->setFontColor(sf::Color::Red); });

	bttns.push_back(lbl_bttn_1);
	bttns.push_back(lbl_bttn_2);
	
	dialogue->addChoiceBox(bttns, 1, sf::Vector2f(0.4f, 0.5f), sf::Vector2f(0.2, 0.1));
	m_parent->changeState(dialogue, true);

	std::shared_ptr<PostProcess> pp = m_scene->getPostProcess();
	pp->enable();
	pp->snowfall_param.bg_layers = 2.f;
	pp->snowfall_param.fg_layers = 2.f;
	pp->snowfall_param.layer_gap = 1.f;
	pp->snowfall_param.size_rate = 0.1f;
	pp->snowfall_param.wind_direction = sf::Glsl::Vec2(0.2, 0.4);

	pp->enable_effect(PostProcess::Effect::snowfall);
	pp->bind_effect_to_layer(PostProcess::Effect::snowfall, 0);

	m_scene->setDirLight_ambient(sf::Color::Black);
}

void Map0000::handleEvent(sf::Event event)
{
}

void Map0000::input(const sf::Time& deltaTime, std::vector<Config::InputAction> inputActions)
{
	m_scene->input(deltaTime, inputActions);
}

void Map0000::update(const sf::Time& deltaTime)
{
	m_scene->update(deltaTime);
	static float t = 0;
	t += deltaTime.asSeconds();

}

void Map0000::draw(sf::RenderTarget& target, const sf::Time& deltaTime)
{
	m_scene->setRenderTarget(&target);
	LightProperties dir = m_scene->getDirectionalLightProperties();
	target.clear(dir.ambient);
	m_scene->render(deltaTime);
}
