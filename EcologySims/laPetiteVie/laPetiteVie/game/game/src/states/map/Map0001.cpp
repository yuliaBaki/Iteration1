#include "Map0001.h"

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
#include "../../gameObjects/bar/BarRenderComponent.h"

Map0001::Map0001(Game* parent) :
	MapState(parent)
{

}

void Map0001::init()
{
	m_scene = new Scene(this, "media/data/map/peak/peak.json", VIEW_TYPE::DYNAMIC_VIEW);

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

	GameObject* camera = m_scene->createGameObject("camera");
	m_scene->setGameObjectInputComponent<CameraInputComponent>(camera);
	m_scene->setGameObjectPhysicComponent<FollowCameraPhysicComponent>(camera, player);
	m_scene->setGameObjectRenderComponent<CameraRenderComponent>(camera);

	class CameraController : public NativeScriptComponent {

		void init(Scene& scene) override {
			CameraData* cd = parent->getComponent<CameraData>(COMPONENT_DATA::CAMERA);

			cd->offset.y = 25.f;
			cd->angle = 0.f;
			cd->offset_distance = 100.f;
			cd->isPolar = true;
		};
		void update(Scene& scene, const sf::Time& dt) override {

		};

		void onDelete(Scene& scene) override {

		};
	};

	camera->setNativeScript<CameraController>();

	std::shared_ptr<PostProcess> pp = m_scene->getPostProcess();
	pp->enable();
	pp->snowfall_param.bg_layers = 2.f;
	pp->snowfall_param.fg_layers = 1.f;
	pp->snowfall_param.layer_gap = 1.f;
	pp->snowfall_param.size_rate = 0.1f;
	pp->snowfall_param.speed = sf::Glsl::Vec2(-2.f, -2.f);
	pp->snowfall_param.wind_direction = sf::Glsl::Vec2(-0.2, -0.4);

	pp->letterbox_param.bar_screen_ratio_top = 0.4f;
	pp->letterbox_param.bar_screen_ratio_bottom = 0.1f;

	pp->letterbox_param.fading_range_top = 0.2f;
	pp->letterbox_param.fading_range_bottom = 0.2f;

	pp->letterbox_param.type = PostProcess::Letterbox_param::Type::smoothstep;
	pp->letterbox_param.top_bar_color = sf::Color::Red;
	pp->letterbox_param.bottom_bar_color = sf::Color::White;

	pp->letterbox_param.blend = PostProcess::Blend::additive;

	pp->enable_effect(PostProcess::Effect::snowfall);
	pp->bind_effect_to_layer(PostProcess::Effect::snowfall, 0);

	pp->enable_effect(PostProcess::Effect::letterbox);
	pp->bind_effect_to_layer(PostProcess::Effect::snowfall, 1);


	m_scene->setDirLight_ambient(sf::Color(10, 0, 10));
	m_scene->setDirLight_diffuse(sf::Color(255, 80, 160));

	GameObject* bars = m_scene->createGameObject("bars");
	m_scene->setGameObjectRenderComponent<BarRenderComponent>(bars);
}

void Map0001::handleEvent(sf::Event event)
{
}

void Map0001::input(const sf::Time& deltaTime, std::vector<Config::InputAction> inputActions)
{
	m_scene->input(deltaTime, inputActions);
}

void Map0001::update(const sf::Time& deltaTime)
{
	m_scene->update(deltaTime);
	static float t = 0;
	t += deltaTime.asSeconds();

}

void Map0001::draw(sf::RenderTarget& target, const sf::Time& deltaTime)
{
	m_scene->setRenderTarget(&target);
	LightProperties dir = m_scene->getDirectionalLightProperties();
	target.clear(dir.ambient);
	m_scene->render(deltaTime);
}
