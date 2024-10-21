#include "Scene.h"
#include <iostream>
#include <string>
#include "../ressourceManager/ShaderManager.h"
#include "../ressourceManager/MapDataManager.h"
#include "../config/ControlSetting.h"
#include "../gameObject/image/ImageRenderComponent.h"
#include "../gameObject/spriteAnimation/SpriteAnimationRenderComponent.h"
#include "../gameObject/terrain/TerrainInputComponent.h"
#include "../gameObject/terrain/TerrainPhysicComponent.h"
#include "../gameObject/terrain/TerrainRenderComponent.h"
#include "../util/helper.h"
#include "../graphic/RendererPseudo3D_Dynamic.h"
#include "../graphic/RendererPseudo3D_Fixed.h"
#include "../graphic/RendererPseudo3D_preRender.h"
#include <filesystem>

Scene::Scene(State* parent, std::string mapDataPath, VIEW_TYPE view_type)
{
    spawnerStaticGameObject     = std::make_unique<SpawnerStaticGameObject>(SpawnerStaticGameObject::PassKey(), this);
    serializerStaticGameObject  = std::make_unique<SerializerStaticGameObject>(SerializerStaticGameObject::PassKey(), this);
    CollisionEngine::bind(this);
    this->parent = parent;

    if (mapDataPath == "") {
        loadFromMapData("media/data/map/map0000.json", view_type);
    }else {
        loadFromMapData(mapDataPath, view_type);
    }

    auto window = parent->getParent();
    sf::Vector2i res = sf::Vector2i(window->getWidth(), window->getHeight());
    postProcess = std::make_shared<PostProcess>(sf::Vector2u(res));



    // Create the physics world with your settings 
}

void Scene::updateMapData(std::string curr_path) {

    //save exported render to .png
    curr_path.erase(curr_path.size() - 5, 5);
    getRenderImage(EXPORT_IMAGE_TYPE::COLOR).saveToFile(curr_path + "_color.png");
    getRenderImage(EXPORT_IMAGE_TYPE::DEPTH).saveToFile(curr_path + "_depth.png");
    getRenderImage(EXPORT_IMAGE_TYPE::NORMAL).saveToFile(curr_path + "_normal.png");
    getRenderImage(EXPORT_IMAGE_TYPE::POSITION_X).saveToFile(curr_path + "_position_x.png");
    getRenderImage(EXPORT_IMAGE_TYPE::POSITION_Y).saveToFile(curr_path + "_position_y.png");
    getRenderImage(EXPORT_IMAGE_TYPE::POSITION_Z).saveToFile(curr_path + "_position_z.png");
    getRenderImage(EXPORT_IMAGE_TYPE::DEBUG_NORMAL).saveToFile(curr_path + "_debug_normal.png");
    getRenderImage(EXPORT_IMAGE_TYPE::DEBUG_POSITION).saveToFile(curr_path + "_debug_position.png");
    getRenderImage(EXPORT_IMAGE_TYPE::DEBUG_DEPTH).saveToFile(curr_path + "_debug_depth.png");

    //save to .json
    *mapData = serializerStaticGameObject->serialize();
    RessourceManager::MapManager::save(curr_path + ".json", mapData);
}

void Scene::loadFromMapData(std::string mapDataPath, VIEW_TYPE view_type) {
    CollisionEngine::bind(this);

    mapData = RessourceManager::MapManager::get(mapDataPath);

    if (view_type == VIEW_TYPE::DYNAMIC_VIEW)
        rendererPseudo3D = std::make_unique<RendererPseudo3D_dynamic>(mapData->voxel_terrain.screen_width, mapData->voxel_terrain.screen_height);
    else if (view_type == VIEW_TYPE::FIXED_VIEW)
        rendererPseudo3D = std::make_unique<RendererPseudo3D_fixed>(mapData->voxel_terrain.screen_width, mapData->voxel_terrain.screen_height);
    else if (view_type == VIEW_TYPE::PRE_RENDER_VIEW) {
        std::string curr_path = mapDataPath.substr(0, mapDataPath.size() - std::string(".json").size());
        if (std::filesystem::exists(curr_path + "_color.png")) {
            rendererPseudo3D = std::make_unique<RendererPseudo3D_preRender>(
                mapData->voxel_terrain.screen_width,
                mapData->voxel_terrain.screen_height,
                curr_path + "_color.png",
                curr_path + "_depth.png",
                curr_path + "_position_x.png",
                curr_path + "_position_y.png",
                curr_path + "_position_z.png",
                curr_path + "_normal.png"
                );
        }
    }

    createTerrainGameObject();
    spawnerStaticGameObject->spawn(*mapData);
    
}

void Scene::updateGameObjectComponent(GameObject* go)
{
    Transform* transform = nullptr;
    HitBox* hitBox = nullptr;
    if (go->hasComponent(COMPONENT_DATA::TRANSFORM))
        transform = go->getComponent<Transform>(COMPONENT_DATA::TRANSFORM);
    if (go->hasComponent(COMPONENT_DATA::HITBOX))
        hitBox = go->getComponent<HitBox>(COMPONENT_DATA::HITBOX);

    /*
        collisionEngine can only have one pair of Transform and HitBox
        for any GameObject. Don't worry that much. Delete the hitBox in
        specific component on the onDelete if you want to remove a
        previous collision or something when you set a new component.
    */
    collisionEngine->add(go, transform, hitBox);
}

void Scene::setInput(GameObject* go, InputComponent* comp)
{
    go->setInput(comp, *this);
}

void Scene::setPhysic(GameObject* go, PhysicComponent* comp)
{
    go->setPhysic(comp, *this);
}

void Scene::setRender(GameObject* go, RenderComponent* comp)
{
    go->setRender(comp, *this);
}

void Scene::callOnDelete(InputComponent* ic)
{
    ic->onDelete(*this);
}

void Scene::callOnDelete(PhysicComponent* pc)
{
    pc->onDelete(*this);
}

void Scene::callOnDelete(RenderComponent* rc)
{
    rc->onDelete(*this);
}


void Scene::input(const sf::Time& dt, std::vector<Config::InputAction> inputActions)
{
    CollisionEngine::bind(this);

    for (int i = 0; i < m_inputComponents.size(); i++) {
        InputComponent* inputComp = m_inputComponents[i];
        if (inputComp) {
            if (inputComp->parent && inputComp->isActive())
                inputComp->update(*this, dt, inputActions);
        }
    }
}

void Scene::update(const sf::Time& dt){
    
    CollisionEngine::bind(this);

    for (int i = 0; i < m_children.size(); i++) {
        GameObject* go = m_children[i];
        if (go->hasScript()) {
            NativeScriptComponent* script = go->getScriptInstance();

            if (!script) {
                script = go->createScriptInstance();
                script->parent = go;
                script->init(*this);
            }

            if (script->isActive()) {
                script->update(*this, dt);
            }
        }
    }

    for (int i = 0; i < m_physicComponents.size(); i++) {
        PhysicComponent* physicComp = m_physicComponents[i];
        if (physicComp)
            if (physicComp->parent && physicComp->isActive())
                physicComp->update(*this, dt);
    }

}

void Scene::render(const sf::Time& dt) {
    CollisionEngine::bind(this);

    static bool first_render = true;

    rendererPseudo3D->clearSprites();

    for (int i = 0; i < m_renderComponents.size(); i++) {
        RenderComponent* renderComp = m_renderComponents[i];
        if (renderComp) 
            if (renderComp->parent && renderComp->isActive()) {
                renderComp->update(*this, dt, *m_pRenderTarget, *rendererPseudo3D);
                renderComp->callEvent(*this, dt);
            }
    }
    
    if(isRendererPseudo3Denable){
        rendererPseudo3D->directionalLight = directionalLight.getLightProperties();
        rendererPseudo3D->pointLights.resize(pointLights.size());
        for (int i = 0; i < pointLights.size(); i++) {
            rendererPseudo3D->pointLights[i] = pointLights[i]->getLightProperties();
        }
        rendererPseudo3D->time = dt.asSeconds();
        if (rendererPseudo3D->getViewType() == VIEW_TYPE::FIXED_VIEW)
        {
           //rendererPseudo3D->renderTerrain();
        }

        rendererPseudo3D->render();

        if (postProcess->isEnable()) {
            if (sf::Vector2f(postProcessInRenderTexture.getSize()) != this->getRenderResolution()) {
                sf::Vector2f res = this->getRenderResolution();
                postProcessInRenderTexture.create(static_cast<unsigned int>(res.x), static_cast<unsigned int>(res.y), parent->getParent()->getContextSettings());
            }
            postProcessInRenderTexture.clear(this->getDirectionalLightProperties().ambient);
            postProcessInRenderTexture.draw(*rendererPseudo3D);
            postProcessInRenderTexture.display();
            postProcess->setInputColor(postProcessInRenderTexture.getTexture());
            postProcess->setInputDepth(this->getDepthTexture());
            postProcess->setDrectionalLightAmbient(this->getDirectionalLightProperties().ambient);
            postProcess->update(dt);
            m_pRenderTarget->draw(*postProcess);
        }
        else
            m_pRenderTarget->draw(*rendererPseudo3D);
    }

}

void Scene::enablePseudo3DRendering()
{
    isRendererPseudo3Denable = true;
}

void Scene::disablePseudo3DRendering()
{
    isRendererPseudo3Denable = false;
}

const sf::Texture& Scene::getDepthTexture()
{
    if(rendererPseudo3D)
        return rendererPseudo3D->getDepthTexture();
}

VIEW_TYPE Scene::pseudo3DRendering_getViewType()
{
    return rendererPseudo3D->view_type;
}

sf::FloatRect Scene::querySpriteRectAssociateWith(const std::string& tag, bool normalize)
{
    sf::FloatRect rect{};
    if(rendererPseudo3D)
        rect = rendererPseudo3D->querySpriteRectAssociateWith(tag);

    if (normalize) {
       sf::Vector2u size = m_pRenderTarget->getSize();
       rect.left /= static_cast<float>(size.x);
       rect.top  /= static_cast<float>(size.y);
       rect.width  /= static_cast<float>(size.x);
       rect.height /= static_cast<float>(size.y);
    }
    return rect;
}

const std::vector<std::string>* Scene::getAllTags() const
{
    const std::vector<std::string>* tags = nullptr;
    if(rendererPseudo3D)
        tags = rendererPseudo3D->getAllTags();
    return tags;
}

sf::Vector2f Scene::getRenderResolution() const
{
    sf::Vector2f res(0.f, 0.f);
    if (rendererPseudo3D) {
        auto param = rendererPseudo3D->getParam();
        res.x = param.screen_width;
        res.y = param.screen_height;
    }

    return res;
}

float Scene::getRenderHeightOnPosition(sf::Vector2f position)
{
	return rendererPseudo3D->terrain->getRenderHeight(position);
}

float Scene::getPhysicalHeightOnPosition(sf::Vector2f position)
{
   return rendererPseudo3D->terrain->getPhysicalHeight(position);
}

sf::Vector3f Scene::getNormalOnPosition(sf::Vector2f position)
{
    sf::Vector3f normal = rendererPseudo3D->terrain->getNormal(position);
    return normal;
}

float Scene::getCollisionMaskOnPosition(sf::Vector2f position)
{
    return rendererPseudo3D->terrain->getCollisionMask(position);
}

void Scene::setTerrain(VoxelTerrain* terrain)
{
    this->terrain = terrain;
    rendererPseudo3D->setTerrain(terrain);
}


sf::Time Scene::getDeltaTime()
{
    return m_deltaTime;
}

void Scene::enableDirectionalLight()
{
    directionalLight.enable(false);
}

void Scene::disableDirectionalLight()
{
    directionalLight.enable(false);
}

void Scene::enableDebugRendering(Debug_rendering debug_rendering)
{
    rendererPseudo3D->enableDebugRendering(debug_rendering);
}

void Scene::disableDebugRendring()
{
    rendererPseudo3D->disableDebugRendring();
}

PointLight* Scene::createPointLight(sf::Vector3f position, float constant, float linear, float quadratic, sf::Color ambient, sf::Color diffuse, sf::Color specular)
{
	PointLight* pl = new PointLight(position);
	pointLights.push_back(pl);
	pl->enable(true);
	pl->setAmbientColor(ambient);
	pl->setDiffuseColor(diffuse);
	pl->setSpecularColor(specular);
	pl->setAttenuation(constant, linear, quadratic);

	return pl;
}

void Scene::deletePointLight(PointLight* pl)
{
    pointLights.erase(std::remove(pointLights.begin(), pointLights.end(), pl), pointLights.end());
    delete pl;
}

void Scene::setDirLight_direction(sf::Vector3f direction)
{
    directionalLight.setDirection(direction);
}

void Scene::setDirLight_ambient(sf::Color ambient)
{
    directionalLight.setAmbientColor(ambient);
}

void Scene::setDirLight_diffuse(sf::Color diffuse)
{
    directionalLight.setDiffuseColor(diffuse);
}

void Scene::setDirLight_specular(sf::Color specular)
{
    directionalLight.setSpecularColor(specular);
}

LightProperties Scene::getDirectionalLightProperties()
{
    return directionalLight.getLightProperties();
}


std::vector<GameObject*> Scene::get_children()
{
    return m_children;
}


GameObject* Scene::createGameObject(const std::string& name)
{
    CollisionEngine::bind(this);

    GameObject* go = new GameObject(*this, nullptr, nullptr, nullptr, name);
    m_children.emplace_back(go);
    m_childrenMap[name] = go;

    return go;
}

GameObject* Scene::createTerrainGameObject()
{
    CollisionEngine::bind(this);

    if (terrainGameObject)
        if (m_childrenMap.find(terrainGameObject->getName()) != m_childrenMap.end())
            delete_child(terrainGameObject->getName());

    auto& vm = mapData->voxel_terrain;
    terrainGameObject = createGameObject("terrain");
    setGameObjectInputComponent<TerrainInputComponent>(terrainGameObject);
    setGameObjectPhysicComponent<TerrainPhysicComponent>(terrainGameObject);
    setGameObjectRenderComponent<TerrainRenderComponent>(
        terrainGameObject,
        vm.terrain_collision_height_path,
        vm.terrain_color_path,
        vm.terrain_collision_height_path,
        vm.terrain_collision_mask_path
    );
    
    return terrainGameObject;
}

void Scene::addGameObject(GameObject* go)
{
    CollisionEngine::bind(this);

    std::string name = go->getName();
    std::unordered_map<std::string, GameObject*>::iterator child_location = m_childrenMap.find(name);
    if (child_location == m_childrenMap.end()) {
        m_children.emplace_back(go);
        m_childrenMap[name] = go;

        m_inputComponents.push_back(go->getInput());
        m_physicComponents.push_back(go->getPhysic());
        m_renderComponents.push_back(go->getRender());
    }
}

std::vector<PointLight*> Scene::getPointLights()
{
    return pointLights;
}

GameObject* Scene::get_child(const std::string& name)
{
	if(m_childrenMap.find(name) != m_childrenMap.end())
	    return m_childrenMap.at(name);
	else
	    return nullptr;
}

void Scene::delete_child(const std::string& name)
{
    std::unordered_map<std::string, GameObject*>::iterator child_location = m_childrenMap.find(name);
    if (child_location != m_childrenMap.end()) {
        GameObject* go = m_childrenMap[name];
        m_inputComponents.erase(
            std::remove_if(
                m_inputComponents.begin(), m_inputComponents.end(),
                [&go](InputComponent* ic) { return ic == go->getInput(); }),
            m_inputComponents.end()
        );

        m_physicComponents.erase(
            std::remove_if(
                m_physicComponents.begin(), m_physicComponents.end(),
                [&go](PhysicComponent* pc) { return pc == go->getPhysic(); }),
            m_physicComponents.end()
        );

        m_renderComponents.erase(
            std::remove_if(
                m_renderComponents.begin(), m_renderComponents.end(),
                [&go](RenderComponent* rc) { return rc == go->getRender(); }),
            m_renderComponents.end()
        );

        m_childrenMap[name]->onDelete(*this);
        collisionEngine->remove(m_childrenMap[name]);

        m_children.erase(
            std::remove_if(
                m_children.begin(), m_children.end(),
                [&name](GameObject* go) { return go->getName() == name; }),
            m_children.end()
        );

        delete m_childrenMap[name];
        m_childrenMap.erase(child_location);
    }
}

void Scene::delete_all_child()
{
    for (auto child : m_children) {
        child->onDelete(*this);
        collisionEngine->remove(child);
        delete child;
    }

    m_children.clear();
    m_childrenMap.clear();
}

bool Scene::has_child(const std::string& name)
{
    if (m_childrenMap.find(name) != m_childrenMap.end())
        return true;
    else
        return false;
}


State* Scene::getParent()
{
    return parent;
}

void Scene::setParent(State* parent)
{
    this->parent = parent;
}

sf::Image Scene::getRenderImage(EXPORT_IMAGE_TYPE export_image_type)
{
    return rendererPseudo3D->getImage(export_image_type);
}

std::shared_ptr<PostProcess> Scene::getPostProcess()
{
    return postProcess;
}

void Scene::addToGroups(GameObject* p_go, const std::initializer_list<std::string> groups)
{
    for (const std::string& gr_name : groups) {
        if (!p_go->has_group(gr_name)) {
            p_go->add_group(gr_name);
            groups_map[gr_name].push_back(p_go);
            map_for_groups_gameObject_index[gr_name][p_go] = groups_map[gr_name].end() - 1;
        }
    }
}

void Scene::removeFromGroups(GameObject* p_go, const std::initializer_list<std::string> groups)
{
    for (const std::string& gr_name : groups) {
        if (map_for_groups_gameObject_index.count(gr_name) > 0) {
            groups_map[gr_name].erase(map_for_groups_gameObject_index[gr_name][p_go]);
            p_go->remove_group(gr_name);
            map_for_groups_gameObject_index[gr_name].erase(p_go);
        }
    }
}

std::vector<GameObject*> Scene::getAllGameObjectFromGroup(const std::string& group_name)
{
    std::vector<GameObject*> gameObjects;
    if (groups_map.count(group_name) > 0)
        gameObjects = groups_map[group_name];
    return gameObjects;
}

std::vector<GameObject*> Scene::getAllGameObjectFromGroups(const std::initializer_list<std::string> groups)
{
    std::vector<GameObject*> gameObjects;
    for(std::string group_name: groups)
        if (groups_map.count(group_name) > 0) {
            std::vector<GameObject*> temp = groups_map[group_name];
            gameObjects.insert(std::end(gameObjects), std::begin(temp), std::end(temp));
        }
    return gameObjects;
}

void Scene::setRenderTarget(sf::RenderTarget* target)
{
    m_pRenderTarget = target;
}


Scene::~Scene(){
    for(auto& child: m_children)
        delete child; 
    
    for(auto& pl: pointLights)
        delete pl;

    m_children.clear();
}
