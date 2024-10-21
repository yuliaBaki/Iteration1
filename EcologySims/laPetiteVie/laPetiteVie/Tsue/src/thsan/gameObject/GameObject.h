#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "componentData/ComponentDataHolder.h"
#include "component/InputComponent.h"
#include "component/PhysicComponent.h"
#include "component/RenderComponent.h"
#include "component/NativeScriptComponent.h"
#include "../scene/Scene.h"

#include <memory>

class InputComponent;
class PhysicComponent;
class RenderComponent;
class NativeScriptComponent;
class Scene;

class  GameObject
{
    public:
        GameObject(Scene &scene, InputComponent* inputComp, PhysicComponent* physicComp, RenderComponent* renderComp, std::string name = "None");

        GameObject(const GameObject& other);
        ~GameObject();

        void updateInput(Scene &scene, const sf::Time& delta, std::vector<Config::InputAction> inputActions);
        void updatePhysic(Scene &scene, const sf::Time& delta);
        void updateRender(Scene& scene, const sf::Time& delta, sf::RenderTarget& target, RendererPseudo3D& rendererPseudo3D);
        void updateNativeScript(Scene& scene, const sf::Time& delta);

        void onDelete(Scene& scene);

        bool hasComponent(COMPONENT_DATA type);
        void setComponent(COMPONENT_DATA target, ComponentData* comp);

        template<typename T>
        T* getComponent(COMPONENT_DATA type){
            return m_componentDataHolder.getComponent<T>(type);
        };

        template<typename T>
        T getCopyComponent(COMPONENT_DATA type) {
            return *(m_componentDataHolder.getComponent<T>(type));
        };

        std::string getName();
        void setName(std::string name);
        GameObject* getParent();

        Transform getWorldTransform();

        bool hasChild(GameObject* go);
        void addChild(GameObject* go);
        GameObject* getChild(const std::string& name);
        std::vector<GameObject*> getChildren();
        void removeChild(const std::string& name);
        void removeAllChild(std::string name);
        void setParent(GameObject* go);

        InputComponent* getInput();
        PhysicComponent* getPhysic();
        RenderComponent* getRender();


        bool isInputActive();
        void disableInput();
        void enableInput();

        bool isPhysicActive();
        void disablePhysic();
        void enablePhysic();

        bool isRenderActive();
        void disableRender();
        void enableRender();

        bool isNativeScriptActive();
        void disableNativeScriptRender();
        void enableNativeScriptRender();

        NativeScriptComponent* (*InstantiateScript)();

        bool hasScript();
        NativeScriptComponent* createScriptInstance();
        NativeScriptComponent* getScriptInstance();


        bool hasRenderComponent();
        void addOnSpriteAnimationRenderEvent(const std::string& sprite_path, const std::string& animation, float at, bool call_once, std::function<void(GameObject&, Scene&, const sf::Time&)> func);
        
        /*
        * this methode will set an event on every single sprite with this animation
        * much slower than specifying the proper sprite_path.
        * It atleast avoid some coupling with the rendering.
        */
        void addOnSpriteAnimationRenderEvent(const std::string& animation, float at, bool call_once, std::function<void(GameObject&, Scene&, const sf::Time&)> func);
        
        template<typename T>
        void setNativeScript() {
            InstantiateScript = []() { return static_cast<NativeScriptComponent*>(new T()); };
            ownScript = true;
        }

        std::vector<std::string> getGroupName();
        bool has_group(const std::string& name);

    private:

        void setInput(InputComponent* inputComp, Scene& scene);
        void setPhysic(PhysicComponent* physicComp, Scene& scene);
        void setRender(RenderComponent* renderComp, Scene& scene);

        void add_group(const std::string& name);
        void remove_group(const std::string& name);

        std::vector<std::string> groups;
        std::unordered_map<std::string, std::vector<std::string>::iterator> groups_index;

        static uint32_t s_IDcounter;
        uint32_t m_ID;
        std::string m_name;
        bool ownScript{false};

        InputComponent* m_inputComponent;
        PhysicComponent* m_physicComponent;
        RenderComponent* m_renderComponent;

        NativeScriptComponent* m_nativeScripComponent;
        ComponentDataHolder m_componentDataHolder;
        Transform* transform;

        GameObject* m_parent;
        GameObject* childs;
        std::vector<GameObject*> m_children;
        std::unordered_map<std::string, GameObject*> m_childrenMap;

        friend class Scene;
};

#endif // GAMEOBJECT_H
