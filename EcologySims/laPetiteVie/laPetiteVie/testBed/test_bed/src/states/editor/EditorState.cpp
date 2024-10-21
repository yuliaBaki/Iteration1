#include "EditorState.h"

#include "imgui.h"
#include "imgui-SFML.h"

#include <string>
#include <iostream>
#include <filesystem>
#include <thsan/gameObject/image/ImageRenderComponent.h>
#include <thsan/gameObject/spriteAnimation/SpriteAnimationRenderComponent.h>
#include <thsan/gameObject/pointLight/PointLightRenderComponent.h>
#include <thsan/util/helper.h>
#include <thsan/vendors/nlohmann/json.hpp>
#include <imfilebrowser.h>

#include "../../gameObjects/point/PointPhysicComponent.h"

using json = nlohmann::json;


namespace fs = std::filesystem;


namespace ImGui
{
    static auto vector_getter = [](void* vec, int idx, const char** out_text)
    {
        auto& vector = *static_cast<std::vector<std::string>*>(vec);
        if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
        *out_text = vector.at(idx).c_str();
        return true;
    };

    bool Combo(const char* label, int* currIndex, std::vector<std::string>& values)
    {
        if (values.empty()) { return false; }
        return Combo(label, currIndex, vector_getter,
            static_cast<void*>(&values), values.size());
    }

    bool ListBox(const char* label, int* currIndex, std::vector<std::string>& values)
    {
        if (values.empty()) { return false; }
        return ListBox(label, currIndex, vector_getter,
            static_cast<void*>(&values), values.size());
    }

}


EditorState::EditorState(Game* parent) :
    State(parent),
    ID(0),
    fileDialogOpen(ImGuiFileBrowserFlags_CreateNewDir),
    fileDialogOpenImagePath(ImGuiFileBrowserFlags_CreateNewDir),
    fileDialogSaveAs(ImGuiFileBrowserFlags_EnterNewFilename | ImGuiFileBrowserFlags_CreateNewDir)
{
    curr_path = "";
    window = &parent->getWindow();
    screenSize[0] = 1024;
    screenSize[1] = 768;
    texture.create(screenSize[0], screenSize[1], parent->getContextSettings());

    spriteForPP.setTexture(texture.getTexture());
    spriteForPP.setTextureRect(sf::IntRect(0, screenSize[1], screenSize[0], -screenSize[1]));
    shader_pp.loadFromFile("media/shader/postProcess/postprocess.vert", "media/shader/postProcess/postprocess.frag");

    ImGui::SFML::Init(parent->getWindow());

    gameView.setSize(sf::Vector2f(window->getSize()));
    gameView.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));

    multiSelectionActivated = false;
    multipleSelectedGameObject.clear();

    sf::Image img;
    img.loadFromFile("media/image/terrain.png");
    terrain_size = img.getSize().x;
}


void EditorState::init()
{

}

void EditorState::handleEvent(sf::Event event)
{
    ImGui::SFML::ProcessEvent(event);

    float scroll_sensitivity = 10.f;
    if (event.type == sf::Event::MouseWheelScrolled)
    {
        if (event.type == event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
            gameView.zoom(1 - (event.mouseWheelScroll.delta / scroll_sensitivity));
    }

    if (event.type == sf::Event::LostFocus)
        isFocused = false;
    if (event.type == sf::Event::GainedFocus)
        isFocused = true;
}

void EditorState::input(const sf::Time& deltaTime, std::vector<Config::InputAction> inputActions) {
    if (scene && isFocused)
        scene->input(deltaTime, inputActions);
}

void EditorState::update(const sf::Time& deltaTime)
{
    static float temps = 0;
    temps += deltaTime.asSeconds();

    if (scene)
        scene->update(deltaTime);
    //bottom
}

void EditorState::draw(sf::RenderTarget& target, const sf::Time& deltaTime)
{
    ImGui::SFML::Update(m_parent->getWindow(), deltaTime);

    menuBar();
    if (scene) {
        edit();
        listInGameAddedAsset();
        //terrainSource();
        translationView();
        assetExplorer(deltaTime);
    }

    window->setView(gameView);
    target.clear();

    if (scene) {

        LightProperties dir = scene->getDirectionalLightProperties();
        texture.clear(dir.ambient);
        scene->render(deltaTime);

        sf::Shader::bind(&shader_pp);
        const sf::Texture var = scene->getDepthTexture();
        shader_pp.setUniform("texture_depth", var);

        target.draw(spriteForPP);
        sf::Shader::bind(0);

    }
    ImGui::SFML::Render(m_parent->getWindow());

}

void EditorState::saveAs() {
    fileDialogSaveAs.SetTitle("Save As...");
    fileDialogSaveAs.SetTypeFilters({ ".json" });
    fileDialogSaveAs.Open();
}



void EditorState::validateCurrPath() {
    if (curr_path.size() >= 5) {
        if (curr_path.substr(curr_path.size() - 5, curr_path.size()) != ".json")
            curr_path += ".json";
    }
    else
        curr_path += ".json";
}

void EditorState::load_scene(std::string path)
{
    if(scene)
        delete scene;
    if (gameObjectFactory)
        delete gameObjectFactory;
    
    std::cout << this;

    emissionMaps.clear();
    depthMaps.clear();
    addedGameAssets.clear();
    selectedGameObject = nullptr;

    scene = new Scene(this, path);
    scene->setRenderTarget(&texture);
    gameObjectFactory = new GameObjectFactory(scene);

    //scene->setDirLight_ambient(sf::Color(40, 100, 200));
    //scene->setDirLight_diffuse((sf::Color(128, 80, 10)));
    scene->enableDirectionalLight();

    LightProperties light = scene->getDirectionalLightProperties();

    color_ambient[0] = light.ambient.r / 255.0;
    color_ambient[1] = light.ambient.g / 255.0;
    color_ambient[2] = light.ambient.b / 255.0;

    color_diffuse[0] = light.diffuse.r / 255.0;
    color_diffuse[1] = light.diffuse.g / 255.0;
    color_diffuse[2] = light.diffuse.b / 255.0;

    dir_light[0] = light.direction.x;
    dir_light[1] = light.direction.y;
    dir_light[2] = light.direction.z;

    //ajouter les éléments de la scene
    for (GameObject* go : scene->get_children()) {
        if (go->getName() != "terrain" && go->getName() != "player") {
            addedGameAssets.push_back(go);
            ID++;
        }
    }

    for (PointLight* pl : scene->getPointLights()) {
        selectedGameObject = gameObjectFactory->createPointLight("pointLight" + std::to_string(ID++), pl);
        addedGameAssets.push_back(selectedGameObject);
    }

    VoxelTerrainData* voxelTerrainData = scene->get_child("terrain")->getComponent<VoxelTerrainData>(COMPONENT_DATA::VOXEL_TERRAIN);

    terrain_texture = RessourceManager::TextureManager::get(voxelTerrainData->height_map_path);
    height_texture = RessourceManager::TextureManager::get(voxelTerrainData->collision_height_map_path);
    color_texture = RessourceManager::TextureManager::get(voxelTerrainData->color_map_path);
    mask_texture = RessourceManager::TextureManager::get(voxelTerrainData->collision_mask_map_path);

}

void EditorState::menuBar()
{
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoCollapse;
    window_flags |= ImGuiWindowFlags_NoTitleBar;

    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(m_parent->getWidth(), 15), ImGuiCond_Once);


    if (ImGui::BeginMainMenuBar()) {

        if (ImGui::BeginMenu("File"))
        {

            //new -- uses the default.json
            if (ImGui::MenuItem("New")) {
                curr_path = "default.json";
                if (curr_path != "") {
                    validateCurrPath();
                    std::cout << curr_path << std::endl;
                    load_scene(curr_path);
                }
            }

            if (ImGui::MenuItem("Open")) {
                fileDialogOpen.SetTitle("Open...");
                fileDialogOpen.SetTypeFilters({ ".json" });
                fileDialogOpen.Open();
            }

            // make sure to gray these if there is currently no file openned at the moment
            if (curr_path == "")
            {
                ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
                ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
            }

            //save
            if (ImGui::MenuItem("Save")) {
                if (curr_path == "default.json")
                    saveAs();

                if (curr_path != "default.json") {
                    scene->updateMapData(curr_path);

                }
            }
            //save as
            if (ImGui::MenuItem("Save As..."))
                saveAs();

            //ends the graying of stuff
            if (curr_path == "")
            {
                ImGui::PopItemFlag();
                ImGui::PopStyleVar();
            }

            ImGui::EndMenu();



        }

        //draw
        fileDialogSaveAs.Display();
        fileDialogOpen.Display();

        //if saveAs is done
        if (fileDialogSaveAs.HasSelected())
        {

            curr_path = fileDialogSaveAs.GetSelected().string();
            if (curr_path != "") {
                validateCurrPath();
                scene->updateMapData(curr_path);
            }
            fileDialogSaveAs.ClearSelected();
        }

        //if open is done
        if (fileDialogOpen.HasSelected())
        {
            curr_path = fileDialogOpen.GetSelected().string();
            if (curr_path != "") {
                validateCurrPath();
                std::cout << curr_path << std::endl;
                load_scene(curr_path);
            }
            fileDialogOpen.ClearSelected();
        }

        if (scene)
            if (ImGui::BeginMenu("Debug mode")) {
                if (ImGui::MenuItem("normal"))
                    scene->enableDebugRendering(Debug_rendering::debug_normal);
                if (ImGui::MenuItem("position"))
                    scene->enableDebugRendering(Debug_rendering::debug_position);
                if (ImGui::MenuItem("normalized scale"))
                    scene->enableDebugRendering(Debug_rendering::debug_normalized_scale);
                if (ImGui::MenuItem("position and normalized scale"))
                    scene->enableDebugRendering(Debug_rendering::debug_position_normalized_scale);
                if (ImGui::MenuItem("depth"))
                    scene->enableDebugRendering(Debug_rendering::debug_depth);
                if (ImGui::MenuItem("disable"))
                    scene->enableDebugRendering(Debug_rendering::none);
                ImGui::EndMenu();

            }

        ImGui::EndMainMenuBar();
    }

}

void EditorState::edit()
{

    ImGuiWindowFlags window_flags = 0;

    float selection_height = 400;

    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoCollapse;

    ImGui::SetNextWindowPos(ImVec2(m_parent->getWidth() - 300, 18), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(300, m_parent->getHeight() - selection_height), ImGuiCond_Always);

    ImGui::Begin("Edit", nullptr, window_flags); // begin window

    terrainEdit();
    lightEdit();


    ImGui::End();


    ImGui::SetNextWindowPos(ImVec2(m_parent->getWidth() - 300, m_parent->getHeight() - selection_height + 15), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(300, selection_height), ImGuiCond_Always);

    ImGui::Begin("selection Edit", nullptr, window_flags); // begin window
    gameObjectAssetEdit();

    //retrieve object from scene
    VoxelTerrainData* voxelTerrainData = scene->get_child("terrain")->getComponent<VoxelTerrainData>(COMPONENT_DATA::VOXEL_TERRAIN);

    ImGui::End();

}

void EditorState::terrainEdit()
{

    //retrieve object from scene
    VoxelTerrainData* voxelTerrainData = scene->get_child("terrain")->getComponent<VoxelTerrainData>(COMPONENT_DATA::VOXEL_TERRAIN);


    ImGui::DragFloat("scale height", &(voxelTerrainData->scale_height), 1.0f, 0.0f, 1024.0f);


    if (ImGui::CollapsingHeader("terrain edit")) {

        ImGui::DragFloat("rotate", &(voxelTerrainData->angle), 0.01f, 0.0f, 2 * 3.14159265358979f);
        ImGui::DragFloat("horizon", &(voxelTerrainData->horizon), 1.0f, -m_parent->getHeight(), m_parent->getHeight());
        ImGui::DragFloat("LOD start at", &(voxelTerrainData->LOD_start_at), 1.0f, 0.0f, 1024.0f);
        ImGui::DragFloat("LOD", &(voxelTerrainData->LOD), 0.01f, 0.0f, 4.0f);
        ImGui::DragInt("FOV", &(voxelTerrainData->FOV), 1, 0, 4096);
        ImGui::DragFloat3("position", &(voxelTerrainData->position.x), 1, -terrain_size / 2, terrain_size / 2);

        if (ImGui::DragFloat("terrain render Distance", &(voxelTerrainData->terrain_render_distance), 1.0f, 0.0f, terrain_size)) {
            voxelTerrainData->sprite_render_distance = voxelTerrainData->terrain_render_distance;
        }

        if (ImGui::DragInt2("resolution", screenSize, 1, 1, 4096)) {
            if (screenSize[0] > 4096)
                screenSize[0] = 4096;
            if (screenSize[1] > 4096)
                screenSize[1] = 4096;
            voxelTerrainData->screen_width = (float)screenSize[0];
            voxelTerrainData->screen_height = (float)screenSize[1];

            texture.create(screenSize[0], screenSize[1], m_parent->getContextSettings());
            spriteForPP.setTextureRect(sf::IntRect(0, screenSize[1], screenSize[0], -screenSize[1]));
        }
    }

}

void  EditorState::CreateFolderTreeRecursivelyFromGameObject(GameObject* gameObject, std::unordered_map<int, std::string> key_to_name, int* n, ImGuiTreeNodeFlags node_flags) {

    for (auto child : gameObject->getChildren()) {
        std::string name = child->getName();
        if (ImGui::TreeNodeEx(name.c_str(), node_flags)) {
            if (ImGui::IsItemClicked())
                selectedGameObject = child;
            key_to_name[*n + 1] = child->getName();
            (*n)++;
            CreateFolderTreeRecursivelyFromGameObject(child, key_to_name, n, node_flags);
            ImGui::TreePop();
        }


    }

}

void EditorState::listInGameAddedAsset()
{
    ImGuiWindowFlags window_flags = 0;

    float height = 217;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoCollapse;
    listInGameAddedAsset_end_y_pos = m_parent->getHeight() - height;
    ImGui::SetNextWindowPos(ImVec2(0, 18), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(200, m_parent->getHeight() - height), ImGuiCond_Always);

    ImGui::Begin("list added GameObjects", nullptr, window_flags); // begin window


    static bool selection[999] = { false };
    static std::unordered_map<int, bool*> multiCurrSelected;
    static bool* currSelected = &selection[998];
    int i = 0;

    for (auto gameObject : addedGameAssets) {
        std::string name = gameObject->getName();
        if (ImGui::Selectable(name.c_str(), &selection[i])) {

            if (ImGui::GetIO().KeyCtrl) {
                multiSelectionActivated = true;
                if(currSelected)
                    if(currSelected != &selection[i])
                        *currSelected = false;
                currSelected = nullptr;
                multiCurrSelected[i] = &selection[i];

                if(*multiCurrSelected[i])
                    multipleSelectedGameObject.push_back(gameObject);
                else if (multiCurrSelected.find(i) != multiCurrSelected.end()){
                    *multiCurrSelected[i] = false;
                    
                    auto go_iter = find(multipleSelectedGameObject.begin(), multipleSelectedGameObject.end(), gameObject);
                    if (go_iter != multipleSelectedGameObject.end())
                        multipleSelectedGameObject.erase(go_iter);
                }
            }else {
                multiSelectionActivated = false;
                for (auto selected : multiCurrSelected)
                    *selected.second = false;

                if(multiCurrSelected.size() > 0)
                    multiCurrSelected.clear();

                if(multipleSelectedGameObject.size() > 0)
                    multipleSelectedGameObject.clear();

                if(currSelected)
                    *currSelected = false;
                currSelected = &selection[i];
                
                selectedGameObject = gameObject;
            }

        }
        i++;
    }

    ImGui::End();






    //TODO: scene graph

    /*
    ImGuiWindowFlags window_flags = 0;

    float height = 217;


    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoCollapse;

    ImGui::SetNextWindowPos(ImVec2(0, 18), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(200, m_parent->getHeight() - height), ImGuiCond_Always);

    ImGui::Begin("list added GameObjects", nullptr, window_flags); // begin window

    ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanAvailWidth;


    static bool selection[999] = { false };
    static bool* currSelected = &selection[998];

    std::string from = "";
    std::string to = "";

    static std::unordered_map<int, std::string> key_to_name;

    int n = 0;

    for (GameObject* go : addedGameAssets) {

        std::string name = go->getName();
        if (go->getParent() == nullptr)
            if (ImGui::TreeNodeEx(name.c_str(), node_flags)) {
                if (ImGui::IsItemClicked())
                    selectedGameObject = go;
                key_to_name[n + 1] = go->getName();
                n++;
                CreateFolderTreeRecursivelyFromGameObject(go, key_to_name, &n, node_flags);

                ImGui::TreePop();
            }

        if (ImGui::BeginDragDropSource()) {
            std::cout << go << std::endl;
            ImGui::SetDragDropPayload("game_asset_tree", &n, sizeof(int));
            ImGui::Text("%s", go->getName().c_str());
            ImGui::EndDragDropSource();
        }

        if (ImGui::BeginDragDropTarget()) {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("game_asset_tree")) {
                IM_ASSERT(payload->DataSize == sizeof(int));
                from = key_to_name[*(const int*)payload->Data];
                to = go->getName();

                std::cout << "dropped: " << from << " on " << to << std::endl;

            }

            ImGui::EndDragDropTarget();
        }

    }
    ImGui::End();

    if (to != "" && from != "") {
        scene->get_child(to)->addChild(scene->get_child(from));
        key_to_name.clear();
    }
    */
}

void EditorState::terrainSource()
{
    float height = 200.f;
    ImGuiWindowFlags window_flags = 0;

    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoCollapse;

    ImGui::SetNextWindowPos(ImVec2(0, listInGameAddedAsset_end_y_pos + 18), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(200, height), ImGuiCond_Always);

    ImGui::Begin("terrain source", nullptr, window_flags); // begin window
    static bool isTerrainPathClicked = false;
    static bool isHeightPathClicked = false;
    static bool isColorPathClicked = false;
    static bool isMaskPathClicked = false;

    VoxelTerrainData* voxelTerrainData = scene->get_child("terrain")->getComponent<VoxelTerrainData>(COMPONENT_DATA::VOXEL_TERRAIN);

    terrain_texture = RessourceManager::TextureManager::get(voxelTerrainData->height_map_path);
    height_texture = RessourceManager::TextureManager::get(voxelTerrainData->collision_height_map_path);
    color_texture = RessourceManager::TextureManager::get(voxelTerrainData->color_map_path);
    mask_texture = RessourceManager::TextureManager::get(voxelTerrainData->collision_mask_map_path);

    /*
    ImGui::Image(height_map, sf::Vector2f(height / 2.2f, height / 2.2f));
    ImGui::SameLine();
    ImGui::Selectable(fs_path.filename().string().c_str(), &selected[selectedCounter]);
    ImGui::SameLine(); ImGui::Text(image_resolition.c_str());
    */

    if (ImGui::Button("terrain...")) {
        isTerrainPathClicked = true;
        fileDialogOpenImagePath.SetTitle("Open image...");
        fileDialogOpenImagePath.SetTypeFilters({ ".png", ".jpg" });
        fileDialogOpenImagePath.Open();
    }

    if (ImGui::Button("height...")) {
        isHeightPathClicked = true;
        fileDialogOpenImagePath.SetTitle("Open image...");
        fileDialogOpenImagePath.SetTypeFilters({ ".png", ".jpg" });
        fileDialogOpenImagePath.Open();
    }

    if (ImGui::Button("color...")) {
        isColorPathClicked = true;
        fileDialogOpenImagePath.SetTitle("Open image...");
        fileDialogOpenImagePath.SetTypeFilters({ ".png", ".jpg" });
        fileDialogOpenImagePath.Open();
    }

    if (ImGui::Button("mask...")) {
        isMaskPathClicked = true;
        fileDialogOpenImagePath.SetTitle("Open image...");
        fileDialogOpenImagePath.SetTypeFilters({ ".png", ".jpg" });
        fileDialogOpenImagePath.Open();
    }


    fileDialogOpenImagePath.Display();

    if (fileDialogOpenImagePath.HasSelected()) {
        curr_path = fileDialogOpenImagePath.GetSelected().string();
        if (curr_path != "") {
            if (isHeightPathClicked) {
                voxelTerrainData->collision_height_map_path = curr_path;
                isHeightPathClicked = false;
            }            
            else if (isTerrainPathClicked) {
                voxelTerrainData->height_map_path = curr_path;
                isTerrainPathClicked = false;
            }            
            
            else if (isColorPathClicked) {
                voxelTerrainData->color_map_path = curr_path;
                isColorPathClicked = false;
            }            
            else if (isMaskPathClicked) {
                voxelTerrainData->collision_mask_map_path = curr_path;
                isMaskPathClicked = false;
            }

            std::cout << curr_path << std::endl;
        }
        fileDialogOpenImagePath.ClearSelected();
    }

    ImGui::End();
}

void EditorState::lightEdit()
{

    //retrieve object from scene
    VoxelTerrainData* voxelTerrainData = scene->get_child("terrain")->getComponent<VoxelTerrainData>(COMPONENT_DATA::VOXEL_TERRAIN);

    if (ImGui::CollapsingHeader("light edit")) {


        if (ImGui::ColorEdit3("ambient color", color_ambient)) {
            // this code gets called if color value changes, so
            // the background color is upgraded automatically!
            scene->setDirLight_ambient(sf::Color(
                static_cast<sf::Uint8>(color_ambient[0] * 255.f),
                static_cast<sf::Uint8>(color_ambient[1] * 255.f),
                static_cast<sf::Uint8>(color_ambient[2] * 255.f)
            ));
        }

        if (ImGui::ColorEdit3("diffuse color", color_diffuse)) {
            // this code gets called if color value changes, so
            // the background color is upgraded automatically!
            scene->setDirLight_diffuse(sf::Color(
                static_cast<sf::Uint8>(color_diffuse[0] * 255.f),
                static_cast<sf::Uint8>(color_diffuse[1] * 255.f),
                static_cast<sf::Uint8>(color_diffuse[2] * 255.f)
            ));

        }

        if (ImGui::ColorEdit3("specular color", color_specular)) {
            // this code gets called if color value changes, so
            // the background color is upgraded automatically!
            scene->setDirLight_specular(sf::Color(
                static_cast<sf::Uint8>(color_specular[0] * 255.f),
                static_cast<sf::Uint8>(color_specular[1] * 255.f),
                static_cast<sf::Uint8>(color_specular[2] * 255.f)
            ));

        }

        ImGui::DragFloat("shininess", &(voxelTerrainData->shininess), 1.f, 1.f, 256.f);

        ImGui::DragFloat2("blurr normal factor", &(voxelTerrainData->normal_blurr.x), 0.01, 0.1, 10);
        ImGui::SliderInt("blurr normal iter", &(voxelTerrainData->normal_blur_iteration), 0, 20);

        static int item_current = 0;
        static std::vector<std::string> items = { "none", "losange", "square", "triangle", "wall"};

        item_current = voxelTerrainData->blur_type;

        if(ImGui::Combo("combo", &item_current, items))
            voxelTerrainData->blur_type = item_current;

        if (ImGui::gizmo3D("##gizmo1", dir_light, 100, imguiGizmo::modeDirection))
            scene->setDirLight_direction(sf::Vector3f(dir_light.x, dir_light.y, dir_light.z));

        std::string guizmo_dir = "< " + std::to_string(dir_light.x) + ", " + std::to_string(dir_light.y) + ", " + std::to_string(dir_light.z) + " >";
        ImGui::LabelText("gizmo position", guizmo_dir.c_str());
    }



}

void EditorState::translationView()
{
    static sf::Vector2i mouse_prev;
    static bool first_tick = true;
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !ImGui::IsAnyItemHovered() && !ImGui::IsAnyItemActive() && isFocused)
    {
        sf::Vector2i mouse_pos = sf::Mouse::getPosition();

        if (first_tick) {
            first_tick = false;
        }
        else {
            sf::Vector2i mouvement = mouse_prev - mouse_pos;
            gameView.move(mouvement.x, mouvement.y);
        }

        mouse_prev = mouse_pos;
    }
    else {
        first_tick = true;
    }



}


std::vector<std::string> excludedExtensions = {
    ".exe",
    ".dll",
    ".bat",
    ".ini",
    ".layout",
    ".ilk",
    ".lib",
    ".pdb",
};

void EditorState::createTreeFromFolderNode(FolderNode folderNode) {
    std::filesystem::path folder_path = folderNode.path;
    ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
    if (selectedFolderPath == folderNode.path)
        node_flags |= ImGuiTreeNodeFlags_Selected;

    if (ImGui::TreeNodeEx(folder_path.filename().string().c_str(), node_flags)) {
        if (ImGui::IsItemClicked())
            selectedFolderPath = folderNode.path;

        for (FolderNode& node : folderNode.child)
            createTreeFromFolderNode(node);

        ImGui::TreePop();
    }
}

void EditorState::assetExplorer(const sf::Time& delta)
{
    ImGuiWindowFlags window_flags = 0;

    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoCollapse;
    window_flags |= ImGuiWindowFlags_NoScrollWithMouse;

    const int height = 200;

    ImGui::SetNextWindowPos(ImVec2(0, m_parent->getHeight() - height), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(m_parent->getWidth() - 300, height), ImGuiCond_Always);

    ImGui::Begin("asset", nullptr, window_flags); // begin window


    ImGui::Separator();

    window_flags |= ImGuiWindowFlags_HorizontalScrollbar;

    {
        ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
        ImGui::BeginChild("ChildL", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.2f, height * 0.8), false, window_flags);

        FolderNode folder = CreateFolderTreeRecursivelyFromFolder("media\\image");
        createTreeFromFolderNode(folder);

        ImGui::EndChild();
        ImGui::PopStyleVar();
    }
    ImGui::SameLine();

    ImGui::BeginChild("ChildR", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.78f, height * 0.8), true, window_flags);

    static int show = 0;
    ImGui::RadioButton("image", &show, 0); ImGui::SameLine();
    ImGui::RadioButton("sprite", &show, 1); ImGui::SameLine();
    ImGui::RadioButton("light", &show, 2);
    ImGui::Separator();

    ImGui::BeginChild("ChildRB", ImVec2(ImGui::GetWindowContentRegionWidth(), 0), false, window_flags);

    if (show == 0)
        imageExplorer(delta);

    else if (show == 1)
        spriteAnimationExplorer(delta);
    else if (show == 2)
        LightExplorer(m_parent->getWidth() - 300);
    ImGui::EndChild();

    ImGui::EndChild();

    ImGui::End();

}

void EditorState::imageExplorer(const sf::Time& delta)
{
    const int imgSize = 30;

    static sf::Time timer = sf::seconds(0);
    timer += delta;

    if (timer.asSeconds() > 1) {
        image_path = loadImageFromFolder(selectedFolderPath);
        timer = sf::seconds(0);
    }

    static bool selected[99] = { false };

    int selectedCounter = 0;
    for (auto path : image_path) {

        std::filesystem::path fs_path = path;
        sf::Vector2u size = RessourceManager::TextureManager::get(path)->getSize();
        std::string image_resolition = "size: " + std::to_string(size.x) + "," + std::to_string(size.y);

        ImGui::Image(*RessourceManager::TextureManager::get(path), sf::Vector2f(imgSize, imgSize));
        ImGui::SameLine();
        ImGui::Selectable(fs_path.filename().string().c_str(), &selected[selectedCounter]);
        ImGui::SameLine(); ImGui::Text(image_resolition.c_str());
        selectedCounter++;

        if (selected[selectedCounter - 1]) {
            fs::path emissionPath = path.substr(0, path.size() - 4) + "emission.png";
            fs::path depthPath = path.substr(0, path.size() - 4) + "depth.png";

            std::string emission = fs::exists(emissionPath) ? emissionPath.string() : "";
            std::string depth = fs::exists(depthPath) ? depthPath.string() : "";

            std::string name = fs_path.filename().string();
            name = name.substr(0, name.size() - 4) + "_" + std::to_string(ID++);

            selectedGameObject = gameObjectFactory->createStaticImage(name, path, emission, depth);

            addedGameAssets.push_back(selectedGameObject);

            selected[selectedCounter - 1] = false;
        }


    }
}


void EditorState::gameObjectAssetEdit()
{
    static sf::Vector3f multi_position = sf::Vector3f();
    static sf::Vector2f multi_scale = sf::Vector2f();
    static float multi_rotation = 0.f;

    if (multiSelectionActivated && multipleSelectedGameObject.size() > 0) {

        sf::Vector3f prev_multi_position = multi_position;
        sf::Vector2f prev_multi_scale = multi_scale;
        float prev_multi_rotation = multi_rotation;

        ImGui::DragFloat3(": position", &(multi_position.x), 1, -512, 1024);
        ImGui::DragFloat(": rotation", &(multi_rotation), 1, -360, 360);
        ImGui::DragFloat(": scale", &(multi_scale.x), 0.05, -1, 10);

        for (auto go : multipleSelectedGameObject) {
            Transform* transform = go->getComponent<Transform>(COMPONENT_DATA::TRANSFORM);
            transform->position -= glm::vec3(prev_multi_position.x, prev_multi_position.y, prev_multi_position.z);
            transform->scale -= glm::vec2(prev_multi_scale.x, prev_multi_scale.y);
            transform->rotation -= prev_multi_rotation;

            transform->position += glm::vec3(multi_position.x, multi_position.y, multi_position.z);
            transform->scale += glm::vec2(multi_scale.x, multi_scale.y);
            transform->rotation += multi_rotation;

            if (go->hasComponent(COMPONENT_DATA::LIGHT)) {
                LightData* ld = go->getComponent<LightData>(COMPONENT_DATA::LIGHT);
                ld->lightProperties.position -= prev_multi_position;
                ld->lightProperties.position += multi_position;
            }
        }

    }else if (selectedGameObject) {
        multi_position = sf::Vector3f();
        multi_scale = sf::Vector2f();
        multi_rotation = 0.f;

        std::string name = selectedGameObject->getName();
        char* buf = &name[0];

        if (ImGui::InputText(": name", buf, 64))
            selectedGameObject->setName(std::string(buf));

        if (selectedGameObject->hasComponent(COMPONENT_DATA::LIGHT)) {
            LightData* ld = selectedGameObject->getComponent<LightData>(COMPONENT_DATA::LIGHT);

            color_ambient_pl[0] = ld->lightProperties.ambient.r / 255.0;
            color_ambient_pl[1] = ld->lightProperties.ambient.g / 255.0;
            color_ambient_pl[2] = ld->lightProperties.ambient.b / 255.0;

            color_diffuse_pl[0] = ld->lightProperties.diffuse.r / 255.0;
            color_diffuse_pl[1] = ld->lightProperties.diffuse.g / 255.0;
            color_diffuse_pl[2] = ld->lightProperties.diffuse.b / 255.0;

            color_specular_pl[0] = ld->lightProperties.specular.r / 255.0;
            color_specular_pl[1] = ld->lightProperties.specular.g / 255.0;
            color_specular_pl[2] = ld->lightProperties.specular.b / 255.0;

            ImGui::DragFloat3(": position", &(ld->lightProperties.position.x), 1, -512, 1024);

            if (ImGui::ColorEdit3("ambient color", color_ambient_pl)) {
                ld->lightProperties.ambient = sf::Color(
                    static_cast<sf::Uint8>(color_ambient_pl[0] * 255.f),
                    static_cast<sf::Uint8>(color_ambient_pl[1] * 255.f),
                    static_cast<sf::Uint8>(color_ambient_pl[2] * 255.f)
                );
            }

            if (ImGui::ColorEdit3("diffuse color", color_diffuse_pl)) {
                ld->lightProperties.diffuse = sf::Color(
                    static_cast<sf::Uint8>(color_diffuse_pl[0] * 255.f),
                    static_cast<sf::Uint8>(color_diffuse_pl[1] * 255.f),
                    static_cast<sf::Uint8>(color_diffuse_pl[2] * 255.f)
                );
            }

            if (ImGui::ColorEdit3("specular color", color_specular_pl)) {
                ld->lightProperties.specular = sf::Color(
                    static_cast<sf::Uint8>(color_specular_pl[0] * 255.f),
                    static_cast<sf::Uint8>(color_specular_pl[1] * 255.f),
                    static_cast<sf::Uint8>(color_specular_pl[2] * 255.f)
                );
            }


            ImGui::DragFloat("constant factor", &(ld->lightProperties.attenuationConstant), 0.1, 0.00, 500);
            ImGui::DragFloat("linear factor", &(ld->lightProperties.attenuationLinear), 0.1, 0.00, 500);
            ImGui::DragFloat("quad factor", &(ld->lightProperties.attenuationQuadratic), 0.1, 0.00, 500);


        }
        else {
            Transform* transform = selectedGameObject->getComponent<Transform>(COMPONENT_DATA::TRANSFORM);
            ImGui::DragFloat3(": position", &(transform->position.x), 1, -512, 1024);
            ImGui::DragFloat(": rotation", &(transform->rotation), 1, -360, 360);
            ImGui::DragFloat(": scale", &(transform->scale.x), 0.05, 0, 10);
        }


        if (selectedGameObject->hasComponent(COMPONENT_DATA::EDITABLE_IMAGE)) {
            EditableImageData* editableImageData = selectedGameObject->getComponent<EditableImageData>(COMPONENT_DATA::EDITABLE_IMAGE);

            if (editableImageData->depth_path != "")
                ImGui::DragFloat("depth factor", &editableImageData->depth_factor, 0.1, 0.001, 1024);

            ImGui::Checkbox(": flip X", &editableImageData->flipX);
            ImGui::Checkbox(": flip Y", &editableImageData->flipY);

            ImGui::DragInt("shininess", &editableImageData->shininess, 1, 1, 256);
        }

        if (selectedGameObject->hasComponent(COMPONENT_DATA::EDITABLE_SPRITE_ANIMATION)) {
            EditableSpriteAnimationData* e = selectedGameObject->getComponent<EditableSpriteAnimationData>(COMPONENT_DATA::EDITABLE_SPRITE_ANIMATION);

            if (e->has_depth)
                ImGui::DragFloat("depth factor", &e->depth_factor, 0.1, 0.001, 1024);


            ImGui::DragInt("shininess", &e->shininess, 1, 0, 256);

            ImGui::Checkbox(": flip X", &e->flipX);
            ImGui::Checkbox(": flip Y", &e->flipY);
            color_key[0] = e->key_color.r / 255.0;
            color_key[1] = e->key_color.g / 255.0;
            color_key[2] = e->key_color.b / 255.0;
            color_key[3] = e->key_color.a / 255.0;
            if (ImGui::ColorEdit4(": key color", color_key)) {
                e->key_color = sf::Color(
                    static_cast<sf::Uint8>(color_key[0] * 255),
                    static_cast<sf::Uint8>(color_key[1] * 255),
                    static_cast<sf::Uint8>(color_key[2] * 255),
                    static_cast<sf::Uint8>(color_key[3] * 255)
                );
            }

            ImGui::LabelText(": animation", "test");


            static int item_current = 0;
            std::vector<const char*> items;
            for (int i = 0; i < e->list_of_animations.size(); i++)
                items.push_back(e->list_of_animations[i].c_str());

            auto it = std::find(e->list_of_animations.begin(), e->list_of_animations.end(), e->current_animation);
            if (it != e->list_of_animations.end())
                item_current = it - e->list_of_animations.begin();
            else
                item_current = 0;

            ImGui::Combo("combo", &item_current, e->list_of_animations);
            e->current_animation = e->list_of_animations[item_current];

        }

        if (ImGui::Button("Delete")) {
            addedGameAssets.erase(std::find(addedGameAssets.begin(), addedGameAssets.end(), selectedGameObject));
            scene->delete_child(selectedGameObject->getName());
            selectedGameObject = nullptr;
        }


    }
}

void EditorState::spriteAnimationExplorer(const sf::Time& delta)
{
    const int imgSize = 30;
    const int iconSize = 100;

    static sf::Time timer = sf::seconds(0);
    timer += delta;

    if (timer.asSeconds() > 0.5) {
        sprite_path = loadSpriteFromFolder(selectedFolderPath);
        timer = sf::seconds(0);
    }
    static bool selected[99] = { false };

    int selectedCounter = 0;


    int onLineElemCounter = 0;
    for (auto& path : sprite_path) {
        std::cout << path << std::endl;
        std::filesystem::path fs_path = path;

        auto textureIcon = RessourceManager::TextureManager::get(path.substr(0, path.size() - 5) + ".png");
        sf::Sprite icon(*textureIcon);
        icon.setTextureRect(sf::IntRect(0, 0, iconSize, iconSize));

        sf::Vector2u size = textureIcon->getSize();
        std::string image_resolition = "size: " + std::to_string(size.x) + "," + std::to_string(size.y);

        ImGui::Image(icon, sf::Vector2f(imgSize, imgSize));
        ImGui::SameLine();
        ImGui::Selectable(fs_path.filename().string().c_str(), &selected[selectedCounter]);
        ImGui::SameLine(); ImGui::Text(image_resolition.c_str());
        selectedCounter++;

        if (selected[selectedCounter - 1]) {
            fs::path emissionPath = path.substr(0, path.size() - 5) + "emission.png";
            fs::path depthPath = path.substr(0, path.size() - 5) + "depth.png";

            std::string name = fs_path.filename().string();
            name = name.substr(0, name.size() - 4) + "_" + std::to_string(ID++);
            selectedGameObject = scene->createGameObject(name);
            scene->setGameObjectRenderComponent<SpriteAnimationRenderComponent>(name, path, fs::exists(emissionPath), fs::exists(depthPath));
            addedGameAssets.push_back(selectedGameObject);

            selected[selectedCounter - 1] = false;
        }

    }
}

void EditorState::LightExplorer(int width) {
    const int imgSize = 50;
    static std::array<std::string, 1> light_types = { "Point Light" };

    for (std::string light_type : light_types) {


        if (ImGui::Selectable(light_type.c_str())) {
            std::string name = "pointLight" + std::to_string(ID++);
            selectedGameObject = scene->createGameObject(name);
            scene->setGameObjectRenderComponent<PointLightRenderComponent>(name, sf::Vector3f(0, 0, 0), 1.0, 4.5 / (0.5 * 0.5), 150 / (0.5 * 0.5), sf::Color::Yellow, sf::Color::Yellow);
            addedGameAssets.push_back(selectedGameObject);
        }

    }
}

EditorState::FolderNode EditorState::CreateFolderTreeRecursivelyFromFolder(const std::string& folderPath)
{
    FolderNode folder;
    folder.path = folderPath;

    std::string path = folder.path;
    for (const auto& entry : fs::directory_iterator(path)) {

        if (entry.is_directory()) {
            FolderNode child_folder;

            child_folder.path = entry.path().string();
            folder.child.push_back(CreateFolderTreeRecursivelyFromFolder(child_folder.path));
        }
    }

    return folder;
}

std::vector<std::string> EditorState::loadImageFromFolder(const std::string& folderPath)
{
    std::vector<std::string> imagePaths;
    if (folderPath == "")
        return imagePaths;
    fs::path path_image = folderPath;
    std::string path = path_image.string();
    for (const auto& entry : fs::directory_iterator(path)) {

        if (!entry.is_directory()) {

            std::string filename = entry.path().filename().string();
            std::string full_path = entry.path().string();
            size_t length = filename.size();


            std::transform(filename.begin(), filename.end(), filename.begin(), [](unsigned char c) {
                return std::tolower(c);
            });

            if (entry.path().extension() == ".png") {
                bool valid = true;

                //ignore if emission
                if (length >= 12) {
                    if (filename.substr(length - 12, 12) == "emission.png") {
                        emissionMaps.insert(entry.path().string());
                        valid = false;
                    }
                }
                //ignore id depth
                if (length >= 9) {
                    if (filename.substr(length - 9, 9) == "depth.png") {
                        depthMaps.insert(entry.path().string());
                        valid = false;
                    }
                }

                if (fs::exists(full_path.substr(0, full_path.size() - 4) + ".json"))
                    valid = false;

                //just take it
                if (valid)
                    imagePaths.push_back(entry.path().string());
            }
        }
    }

    return imagePaths;
}

std::vector<std::string> EditorState::loadSpriteFromFolder(const std::string& folderPath) {
    std::vector<std::string> spritePaths;
    if (folderPath == "")
        return spritePaths;
    fs::path path_sprite = folderPath;
    std::string path = path_sprite.string();
    for (const auto& entry : fs::directory_iterator(path)) {

        if (!entry.is_directory()) {

            std::string filename = entry.path().filename().string();
            size_t length = filename.size();

            std::transform(filename.begin(), filename.end(), filename.begin(), [](unsigned char c) {
                return std::tolower(c);
            });

            if (entry.path().extension() == ".json" && fs::exists(entry.path().string().substr(0, entry.path().string().size() - 5) + ".png"))
            {
                spritePaths.push_back(entry.path().string());
            }

        }
    }

    return spritePaths;
}
