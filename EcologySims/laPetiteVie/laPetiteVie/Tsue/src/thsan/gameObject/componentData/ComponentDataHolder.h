#ifndef COMPONENTDATAHOLDER_H
#define COMPONENTDATAHOLDER_H

#include "ComponentData.h"

class ComponentDataHolder
{
    public:
        ComponentDataHolder()
        :mask(0){
        };

        template<typename T>
        void setComponent(COMPONENT_DATA target, T& comp){
            ComponentData* curr = components[static_cast<unsigned int>(target)];
            if (curr)
                delete curr;

            components[static_cast<unsigned int>(target)] = static_cast<ComponentData*>(&comp);
            if(!hasComponent(target))
                mask += 1 << static_cast<unsigned int>(target);
        }

        template<typename T>
        void setComponent(COMPONENT_DATA target, T&& comp){
            ComponentData* curr = components[static_cast<unsigned int>(target)];
            if (curr)
                delete curr;

            T* temp = new T(std::move(comp));
            components[static_cast<unsigned int>(target)] = static_cast<ComponentData*>(temp);
            if(!hasComponent(target))
                mask += 1 << static_cast<unsigned int>(target);
        }

        template<typename T>
        T* getComponent(COMPONENT_DATA target){
            return static_cast<T*>(components[static_cast<unsigned int>(target)]);
        }

        bool hasComponent(COMPONENT_DATA target){
            unsigned int value = 1 << static_cast<unsigned int>(target);

            return (value & mask) != 0 ? true : false;
        }

        ~ComponentDataHolder() {
            for(int i = 0; i < 20; i++)
                if(components[i])
                    delete components[i];
        }

    private:
        ComponentData* components[20] = {nullptr};
        unsigned int mask{0};

};

/*

    // the registry class has to contain the component on stack

    template<typename T>
    concept Component = requires(T t) {
        { t.entityId } -> std::convertible_to<int>; // Checks if entityId exists and is convertible to int
    };

    using Entity = int;

    enum class ComponentType
    {
        Position = 0,
        Asteroid = 1,
        LAST = 2
    };

    // Define component structs
    struct Position {
        float x, y;
        int entityId;
    };

    struct Asteroid {
        float weight;
        int entityId;
    };


    //use variant and all the component has to be saved on the stack

    I would expect to have:
     
    std::array<Position, MAX> positions;
    std::array<Asteroids, MAX> asteroids;
    std::unordered_map<Entity, int> EntityIndex; //where the index is the position in the arrays, might need a bitmask for a potential fonction called "hasComponent(compenentType)"

    but I must absolutely never have garbage like: "std::vector<Position> getAllPosition()" this is not good long term. 
    ...

    Registry reg;
    Entity e = reg.createEntity();
    reg.setCompoenent(e, compenentType::position, Position{5.f, 3.f});
    reg.setCompoenent(e, compenentType::asteroid, Asteroid{...});

   std::vector<Position> positions = reg.getAll<Position>(); // this is O(1) should just return an existing vector and should pick the proper one with Template T

   for(Position p : positions)
   {
    if(p.x > 800.f)
    {
        Entity& e = reg.getEntity(p.EntityId); // this should also be O(1)
        Asteroid& a = reg.getCompoenent<Asteroid>(e);
        a.weight = 2.f;
    }
   }

   reg.remove(e);

*/

#endif // COMPONENTDATAHOLDER_H
