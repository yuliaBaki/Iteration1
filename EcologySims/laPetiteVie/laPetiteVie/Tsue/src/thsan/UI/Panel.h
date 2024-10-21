#pragma once
#include <string>
#include <vector>
#include "../ressourceManager/ShaderManager.h"
#include <SFML/System/Vector2.hpp>
#include "Widget.h"
#include "Button.h"


namespace UI {
	enum class Type {
		none = 0,
		fill = 1
	};

	struct ShaderUIparam {
		Type type;
		bool enableImage;
		float speed;
		float density;
		float cornerRadius[4];
		float borderSize[4];
		int iteration;
		sf::Color colorBg[4];
		sf::Color colorBorder;
		sf::Color colorBorderBorders;
		sf::Vector3f direction;
		std::shared_ptr<sf::Texture> p_texture;
	};

	class Panel : public Widget
	{
	public:
		Panel(const std::string& name, sf::Vector2f position, sf::Vector2f size, bool lockRatio = false);
		~Panel();


		virtual void update(const sf::Time& deltaTime) override;

		void ActAsATransparentContainerForPosition();
		void DoNotActAsATransparentContainerForPosition();
		bool isTransparentContainerForPosition();

		void setColorBg(unsigned int i, sf::Color color);
		sf::Color getColorBg(unsigned int i);

		void setColorBorder(sf::Color color);
		sf::Color getColorBorder();

		void setColorBorderBorders(sf::Color color);
		sf::Color getColorBorderBorders();

		void setCornerRadius(unsigned int corner, float radius);
		float getCornerRadius(unsigned int corner);

		void setBorderSize(unsigned int corner, float size);
		float getBorderSize(unsigned int corner);

		void setBgTexture(std::string path);
		std::shared_ptr<sf::Texture> getBgTexture();
		void enableBgTexture();
		void disableBgTexture();
		bool isBgTextureEnable();

		void setType(Type theme);
		void setThemeParam(sf::Vector3f direction, float speed, float density, int iteration);

		void setShaderParams(UI::ShaderUIparam param);
		UI::ShaderUIparam getShaderParams();

	protected:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		bool transparentContainer{false};
		ShaderUIparam shaderParam;

		std::vector<Button*> buttons;
		Button* selection{nullptr};

		sf::VertexArray m_vertices;
		const std::shared_ptr<sf::Shader> p_shader = RessourceManager::ShaderManager::get("media/shader/UI/UI.vert", "media/shader/UI/UI.frag");
	};

}