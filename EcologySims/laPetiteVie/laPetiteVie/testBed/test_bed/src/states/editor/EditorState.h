#pragma once
#include <Thsan.h>

#include "imGuIZMOquat.h"
#include <iostream>
#include <SFML/Graphics/Shader.hpp>
#include <unordered_set>
#include <thsan/ressourceManager/MapDataManager.h>
#include <imfilebrowser.h>
#include <thsan/gameObject/GameObjectFactory.h>

class EditorState : public State
{
public:
    EditorState(Game* parent);
    std::string getType() override { return "editor"; } 
    void init() override;
    void handleEvent(sf::Event event) override;
    void input(const sf::Time& deltaTime, std::vector<Config::InputAction> inputActions) override;
    void update(const sf::Time& deltaTime) override;
    void draw(sf::RenderTarget& target, const sf::Time& deltaTime) override;

    ~EditorState() = default;

private:
    unsigned int ID;
    void menuBar();
    void edit();
    void listInGameAddedAsset();
    void terrainSource();
    void terrainEdit();
    void lightEdit();
    void translationView();
    void assetExplorer(const sf::Time& delta);
    void imageExplorer(const sf::Time& delta);
    void spriteAnimationExplorer(const sf::Time& delta);
    void LightExplorer(int width);
    void createTreelistInGameAddedAsset(GameObject* gameObject, ImGuiTreeNodeFlags node_flags);

    std::vector<std::string> loadImageFromFolder(const std::string& folderPath);
    std::vector<std::string> loadSpriteFromFolder(const std::string& folderPath);

    struct FolderNode {
        std::vector<FolderNode> child;
        std::string path;
    };
    void CreateFolderTreeRecursivelyFromGameObject(GameObject* gameObject, std::unordered_map<int, std::string> key_to_name, int* n, ImGuiTreeNodeFlags node_flags);
    FolderNode CreateFolderTreeRecursivelyFromFolder(const std::string& folderPath);
    void createTreeFromFolderNode(FolderNode folderNode);

    void gameObjectAssetEdit();
    void saveAs();
    void validateCurrPath();
    void load_scene(std::string path);

    sf::View gameView;
    sf::RenderTexture texture;
    sf::Texture frame_texture;
    sf::RenderWindow* window;
    Scene* scene{ nullptr };
    sf::Sprite spriteForPP;

    float color_ambient[3] = { 0.f, 0.f, 0.f };
    float color_diffuse[3] = { 0.f, 0.f, 0.f };
    float color_specular[3] = { 0.f, 0.f, 0.f };
    float color_key[4]     = { 0.f, 0.f, 0.f, 0.f };
    int screenSize[2] = { 800,600 };

    float elevation{ 0 };
    sf::Vector2i screen_size;
    vgm::Vec3 dir_light;

    sf::Shader shader_pp;
    sf::Sprite cursor;

    sf::Sprite height_map;
    sf::Sprite color_map;
    sf::Sprite mask_map;
    sf::Sprite terrain_map;

    std::shared_ptr<sf::Texture> terrain_texture;
    std::shared_ptr<sf::Texture> height_texture;
    std::shared_ptr<sf::Texture> color_texture;
    std::shared_ptr<sf::Texture> mask_texture;

    std::unordered_set<std::string> emissionMaps;
    std::unordered_set<std::string> depthMaps;
    std::vector<GameObject*> addedGameAssets;

    bool multiSelectionActivated;

    GameObject* selectedGameObject;
    std::vector<GameObject*> multipleSelectedGameObject;

    PointLight* selectedPointLight;

    std::string selectedFolderPath;

    //selected point light 
    float position[3] = { 0.f, 0.f, 0.f };
    float linear_att = 1.8;
    float quad_att = 75;
    float color_ambient_pl[3] = { 0.f, 0.f, 0.f };
    float color_diffuse_pl[3] = { 0.f, 0.f, 0.f };
    float color_specular_pl[3] = { 0.f, 0.f, 0.f };

    bool isFlipX = false;
    bool isFlipY = false;
    bool isFocused = true;
    float listInGameAddedAsset_end_y_pos{ 0.f };
    data::MapData mapData;

    std::string curr_path;
    ImGui::FileBrowser fileDialogOpen;
    ImGui::FileBrowser fileDialogOpenImagePath;
    ImGui::FileBrowser fileDialogSaveAs;

    std::vector<std::string> image_path;
    std::vector<std::string> sprite_path;

    GameObjectFactory* gameObjectFactory{ nullptr };
    float terrain_size;
};

