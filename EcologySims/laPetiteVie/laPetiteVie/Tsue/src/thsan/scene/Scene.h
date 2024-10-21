#pragma once

#include <vector>
#include <memory>
#include <unordered_map>
#include <SFML/System/Time.hpp>
#include "SpawnerStaticGameObject.h"
#include "SerializerStaticGameObject.h"
#include "../gameObject/GameObject.h"
#include "../graphic/light/DirectionalLight.h"
#include "../graphic/light/PointLight.h"
#include "../graphic/RendererPseudo3D.h"
#include "../graphic/PostProcess.h"
#include "../state/State.h"
#include "../ressourceManager/MapDataManager.h"
#include "../CollisionEngine/CollisionEngine.h"


class GameObject;
class InputComponent;
class PhysicComponent;
class RenderComponent;

class SpawnerStaticGameObject;
class SerializerStaticGameObject;
class CollisionEngine;

class Scene
{
    public:
        Scene(State* parent, std::string MapDatapath = "", VIEW_TYPE view_type = VIEW_TYPE::DYNAMIC_VIEW);
        virtual ~Scene();
        Scene (const Scene& scene) = delete;

        void updateMapData(std::string curr_path);

        void input(const sf::Time& dt, std::vector<Config::InputAction> inputActions);
        void update(const sf::Time& dt);
        void render(const sf::Time& dt);

        //terrain related methode
        float getRenderHeightOnPosition(sf::Vector2f position);
        float getPhysicalHeightOnPosition(sf::Vector2f position);
        sf::Vector3f getNormalOnPosition(sf::Vector2f position);
        float getCollisionMaskOnPosition(sf::Vector2f position);
        void setTerrain(VoxelTerrain *terrain);

        //render related methode
        void setRenderTarget(sf::RenderTarget* target);
        VIEW_TYPE pseudo3DRendering_getViewType();
        sf::FloatRect querySpriteRectAssociateWith(const std::string& tag, bool normalize = false);
        const std::vector<std::string>* getAllTags() const;
        sf::Vector2f getRenderResolution() const;
        void enablePseudo3DRendering();
        void disablePseudo3DRendering();

        const sf::Texture& getDepthTexture();

        //light related method
		PointLight* createPointLight(sf::Vector3f position = sf::Vector3f(0.f, 0.f, 0.f), float constant = 1.0, float linear = 0.7, float quadratic = 1.8, sf::Color ambient = sf::Color::White, sf::Color diffuse = sf::Color::White, sf::Color specular = sf::Color::White);
        void deletePointLight(PointLight* pl);
        void setDirLight_direction(sf::Vector3f direction);
        void setDirLight_ambient(sf::Color ambient);
        void setDirLight_diffuse(sf::Color diffuse);
        void setDirLight_specular(sf::Color specular);

        void enableDirectionalLight();
        void disableDirectionalLight();

        void enableDebugRendering(Debug_rendering debug_rendering);
        void disableDebugRendring();

        LightProperties getDirectionalLightProperties();

        //scene graph related method
        std::vector<GameObject*> get_children();
        GameObject* get_child(const std::string& name);
        void delete_child(const std::string& name);
        void delete_all_child();
        bool has_child(const std::string& name);

        sf::Time getDeltaTime();
        GameObject* createGameObject(const std::string& name);

        template<typename T, typename... Args>
        void setGameObjectInputComponent(GameObject* go, Args... args) {
            static_assert(std::is_base_of<InputComponent, T>::value, "T must derive from InputComponent");

            InputComponent* inputComp = new T(args...);
            setInput(go, inputComp);

            if (m_inputComponentsMap.count(go) > 0){
                int position_in_vector = m_inputComponentsMap[go];
                InputComponent* prev_inputComp = m_inputComponents[position_in_vector];
                m_inputComponents[position_in_vector] = inputComp;
                callOnDelete(prev_inputComp);
                delete prev_inputComp;
            }
            else {
                m_inputComponents.push_back(inputComp);
                m_inputComponentsMap[go] = m_inputComponents.size() - 1;
            }

            updateGameObjectComponent(go);
        }

        template<typename T, typename... Args>
        void setGameObjectPhysicComponent(GameObject* go, Args... args) {
            static_assert(std::is_base_of<PhysicComponent, T>::value, "T must derive from PhysicComponent");

            PhysicComponent* physicComp = new T(args...);
            setPhysic(go, physicComp);

            if (m_physicComponentsMap.count(go) > 0) {
                int position_in_vector = m_physicComponentsMap[go];
                PhysicComponent* prev_physicComp = m_physicComponents[position_in_vector];
                m_physicComponents[position_in_vector] = physicComp;
                callOnDelete(prev_physicComp);
                delete prev_physicComp;
            }
            else {
                m_physicComponents.push_back(physicComp);
                m_physicComponentsMap[go] = m_physicComponents.size() - 1;
            }

            updateGameObjectComponent(go);
        }

        template<typename T, typename... Args>
        void setGameObjectRenderComponent(GameObject* go, Args... args) {
            static_assert(std::is_base_of<RenderComponent, T>::value, "T must derive from RenderComponent");

            RenderComponent* renderComp = new T(args...);
            setRender(go, renderComp);

            if (m_renderComponentsMap.count(go) > 0) {
                int position_in_vector = m_renderComponentsMap[go];
                RenderComponent* prev_renderComp = m_renderComponents[position_in_vector];
                m_renderComponents[position_in_vector] = renderComp;
                callOnDelete(prev_renderComp);
                delete prev_renderComp;
            }
            else {
                m_renderComponents.push_back(renderComp);
                m_renderComponentsMap[go] = m_renderComponents.size() - 1;
            }

            updateGameObjectComponent(go);
        }
        
        template<typename T, typename... Args>
        void setGameObjectInputComponent(const std::string& name, Args... args) {
            setGameObjectInputComponent<T>(m_childrenMap[name], args...);
        }

        template<typename T, typename... Args>
        void setGameObjectPhysicComponent(const std::string& name, Args... args) {
            setGameObjectPhysicComponent<T>(m_childrenMap[name], args...);
        }

        template<typename T, typename... Args>
        void setGameObjectRenderComponent(const std::string& name, Args... args) {
            setGameObjectRenderComponent<T>(m_childrenMap[name], args...);
        }

        template <typename T>
        void setGameObjectNativeScriptComponent(const std::string& name) {
            setGameObjectNativeScriptComponent<T>(m_childrenMap[name]);
        }

        GameObject* createTerrainGameObject();
        void addGameObject(GameObject* go);
        std::vector<PointLight*> getPointLights();
        State* getParent();
        void setParent(State* parent);

        sf::Image getRenderImage(EXPORT_IMAGE_TYPE export_image_type);

        std::shared_ptr<PostProcess> getPostProcess();

        void addToGroups(GameObject* p_go, const std::initializer_list<std::string> groups);
        void removeFromGroups(GameObject* p_go, const std::initializer_list<std::string> groups);
        std::vector<GameObject*> getAllGameObjectFromGroup(const std::string& group_name);
        std::vector<GameObject*> getAllGameObjectFromGroups(const std::initializer_list<std::string> groups);

    private:
        void loadFromMapData(std::string mapDataPath, VIEW_TYPE view_type);
        void updateGameObjectComponent(GameObject* go);
        void setInput(GameObject* go, InputComponent* comp);
        void setPhysic(GameObject* go, PhysicComponent* comp);
        void setRender(GameObject* go, RenderComponent* comp);

        void callOnDelete(InputComponent* ic);
        void callOnDelete(PhysicComponent* pc);
        void callOnDelete(RenderComponent* rc);

        std::vector<std::string> to_delete_names;

        bool isRendererPseudo3Denable;
        std::unique_ptr<RendererPseudo3D> rendererPseudo3D{nullptr};

        DirectionalLight directionalLight;
		std::vector<PointLight*> pointLights;
        std::vector<GameObject*> m_children;
        std::unordered_map<std::string, std::vector<GameObject*>> groups_map;
        std::unordered_map<std::string, std::unordered_map<GameObject*, std::vector<GameObject*>::iterator>> map_for_groups_gameObject_index;

        std::vector<InputComponent*> m_inputComponents;
        std::vector<PhysicComponent*> m_physicComponents;
        std::vector<RenderComponent*> m_renderComponents;

        std::unordered_map<GameObject*, int> m_inputComponentsMap;
        std::unordered_map<GameObject*, int> m_physicComponentsMap;
        std::unordered_map<GameObject*, int> m_renderComponentsMap;
        std::unordered_map<GameObject*, int> m_nativeScriptComponentsMap;

        std::unordered_map<std::string, GameObject*> m_childrenMap;

        State* parent;
        sf::Time m_deltaTime;
        sf::RenderTarget* m_pRenderTarget;
        VoxelTerrain* terrain;

        GameObject* terrainGameObject{nullptr};
        std::shared_ptr<data::MapData> mapData;

        std::unique_ptr<SpawnerStaticGameObject> spawnerStaticGameObject;
        std::unique_ptr<SerializerStaticGameObject> serializerStaticGameObject;
        std::unique_ptr<CollisionEngine> collisionEngine;

        std::shared_ptr<PostProcess> postProcess;
        sf::RenderTexture postProcessInRenderTexture;

};
