#include "Panel.h"
#include "../ressourceManager/FontManager.h"
#include "../ressourceManager/TextureManager.h"


UI::Panel::Panel(const std::string& name, sf::Vector2f position, sf::Vector2f size, bool lockRatio)
:Widget(name, position, size, lockRatio)
{
	m_vertices.resize(4);
	m_vertices.setPrimitiveType(sf::Quads);

	m_vertices[0].position = sf::Vector2f(position.x, position.y);
	m_vertices[1].position = sf::Vector2f(position.x + size.x, position.y);
	m_vertices[2].position = sf::Vector2f(position.x + size.x, position.y + size.y);
	m_vertices[3].position = sf::Vector2f(position.x, position.y + size.y);

	m_vertices[0].texCoords = sf::Vector2f(0.0, 0.0);
	m_vertices[1].texCoords = sf::Vector2f(1.0, 0.0);
	m_vertices[2].texCoords = sf::Vector2f(1.0, 1.0);
	m_vertices[3].texCoords = sf::Vector2f(0.0, 1.0);

	m_vertices[0].color = shaderParam.colorBg[0];
	m_vertices[1].color = shaderParam.colorBg[1];
	m_vertices[2].color = shaderParam.colorBg[2];
	m_vertices[3].color = shaderParam.colorBg[3];

	elapsedTime = sf::seconds(0);

	this->position = position;
	this->size = size;
	this->origin = sf::Vector2f(0, 0);
	this->lockRatio = lockRatio;
}

UI::Panel::~Panel()
{
	for (Button* b : buttons)
		delete b;

	buttons.clear();
}



void UI::Panel::setColorBg(unsigned int i, sf::Color color)
{
	shaderParam.colorBg[i] = color;
}

sf::Color UI::Panel::getColorBg(unsigned int i)
{
	return shaderParam.colorBg[i];
}

void UI::Panel::setColorBorder(sf::Color color)
{
	shaderParam.colorBorder = color;
}

sf::Color UI::Panel::getColorBorder()
{
	return shaderParam.colorBorder;
}

void UI::Panel::setColorBorderBorders(sf::Color color)
{
	shaderParam.colorBorderBorders = color;
}

sf::Color UI::Panel::getColorBorderBorders()
{
	return shaderParam.colorBorderBorders;
}


void UI::Panel::setCornerRadius(unsigned int corner, float radius)
{
	if (corner >= 0 && corner <= 3)
		shaderParam.cornerRadius[corner] = radius;
}

float UI::Panel::getCornerRadius(unsigned int corner)
{
	if (corner >= 0 && corner <= 3)
		return shaderParam.cornerRadius[corner];
}

void UI::Panel::setBorderSize(unsigned int border, float size)
{
	if (border >= 0 && border <= 3)
		shaderParam.borderSize[border] = size;
}

float UI::Panel::getBorderSize(unsigned int border)
{
	if (border >= 0 && border <= 3)
		return shaderParam.borderSize[border];
}

void UI::Panel::setBgTexture(std::string path)
{
	shaderParam.p_texture = RessourceManager::TextureManager::get(path);
}

std::shared_ptr<sf::Texture> UI::Panel::getBgTexture()
{
	return shaderParam.p_texture;
}

void UI::Panel::enableBgTexture()
{
	shaderParam.enableImage = true;
}

void UI::Panel::disableBgTexture()
{
	shaderParam.enableImage = false;
}

bool UI::Panel::isBgTextureEnable()
{
	return shaderParam.enableImage;
}



void UI::Panel::setThemeParam(sf::Vector3f direction, float speed, float density, int iteration)
{
	this->shaderParam.direction = direction;
	this->shaderParam.speed = speed;
	this->shaderParam.density = density;
	this->shaderParam.iteration = iteration;
}

void UI::Panel::setShaderParams(UI::ShaderUIparam param)
{
	shaderParam = param;
}

UI::ShaderUIparam UI::Panel::getShaderParams()
{
	return shaderParam;
}

void UI::Panel::update(const sf::Time& deltaTime)
{
	calculateDimenstion();

	sf::Vector2f currPosition = origin + sf::Vector2f(position.x * screenSize.x, position.y * screenSize.y);

	sf::Vector2f currSize = sf::Vector2f(size.x * screenSize.x, size.y * screenSize.y);

	m_vertices[0].position = sf::Vector2f(currPosition.x, currPosition.y);
	m_vertices[1].position = sf::Vector2f(currPosition.x + currSize.x, currPosition.y);
	m_vertices[2].position = sf::Vector2f(currPosition.x + currSize.x, currPosition.y + currSize.y);
	m_vertices[3].position = sf::Vector2f(currPosition.x, currPosition.y + currSize.y);

	m_vertices[0].color = shaderParam.colorBg[0];
	m_vertices[1].color = shaderParam.colorBg[1];
	m_vertices[2].color = shaderParam.colorBg[2];
	m_vertices[3].color = shaderParam.colorBg[3];

	elapsedTime += deltaTime;
	for (Widget* widget : childs)
		widget->update(deltaTime);
}

void UI::Panel::ActAsATransparentContainerForPosition()
{
	transparentContainer = true;
}

void UI::Panel::DoNotActAsATransparentContainerForPosition()
{
	transparentContainer = false;
}

bool UI::Panel::isTransparentContainerForPosition()
{
	return transparentContainer;
}


void UI::Panel::setType(Type type)
{
	this->shaderParam.type = type;
}


void UI::Panel::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (!hidden) {
		p_shader->setUniform("time", elapsedTime.asSeconds());
		p_shader->setUniform("size", getGlobalSize());

		p_shader->setUniform("radius_top_left", shaderParam.cornerRadius[0]);
		p_shader->setUniform("radius_top_right", shaderParam.cornerRadius[1]);
		p_shader->setUniform("radius_bottom_left", shaderParam.cornerRadius[2]);
		p_shader->setUniform("radius_bottom_right", shaderParam.cornerRadius[3]);

		p_shader->setUniform("color_border", sf::Glsl::Vec4(shaderParam.colorBorder));
		p_shader->setUniform("color_borders_border", sf::Glsl::Vec4(shaderParam.colorBorderBorders));

		p_shader->setUniform("top_border_size", shaderParam.borderSize[0]);
		p_shader->setUniform("bottom_border_size", shaderParam.borderSize[1]);
		p_shader->setUniform("left_border_size", shaderParam.borderSize[2]);
		p_shader->setUniform("right_border_size", shaderParam.borderSize[3]);

		p_shader->setUniform("origin", origin);
		p_shader->setUniform("width", static_cast<float>(screenSize.x));
		p_shader->setUniform("height", static_cast<float>(screenSize.y));
		p_shader->setUniform("theme", static_cast<int>(shaderParam.type));

		p_shader->setUniform("texture", sf::Shader::CurrentTexture);

		p_shader->setUniform("image", *(shaderParam.p_texture));
		p_shader->setUniform("enable_image", shaderParam.enableImage);

		p_shader->setUniform("speed", shaderParam.speed);
		p_shader->setUniform("direction", shaderParam.direction);
		p_shader->setUniform("density", shaderParam.density);
		p_shader->setUniform("iteration", shaderParam.iteration);

		p_shader->setUniform("theme", static_cast<int>(shaderParam.type));

		if (!transparentContainer)
			target.draw(m_vertices, &*p_shader);

		for (Widget* widget : childs) {
			if (widget)
				target.draw(*widget);
		}
	}
}

