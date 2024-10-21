#pragma once

#include <SFML/Window/Event.hpp>
#include <vector>
#include <map>

namespace Config {

	enum InputState
	{
		isPressed,
		isPressedNoRepeat,
		isReleased,
		isDoubleClick,
		JoystickAxisUp,
		JoystickAxisDown,
		JoystickAxisRight,
		JoystickAxisLeft
	};

	enum  InputAction
	{
		up,
		down,
		left,
		right,
		rotateLeft,
		rotateRight,
		start,
		select,
		run,
		stopRun,
		jump,
		back,
		action,
		item,
		plant,
		aim,
		stopAim
	};

	struct  InputCode {
		sf::Keyboard::Key KeyCode;
		unsigned int buttonCode;
		sf::Joystick::Axis axisCode;
		float direction;
	};
	namespace{
		struct  InputCodeState {
			InputCode inputCode;
			InputState inputState;
		};
	}
	class  ControlSetting {
	public:
		ControlSetting() = default;
		~ControlSetting() = default;
		void add(sf::Keyboard::Key key, InputState inputState, InputAction inputAction);
		void remove(sf::Keyboard::Key key, InputState inputState, InputAction inputAction);
		void handleInput(sf::Event event);
		void updateInput();
		std::vector<InputAction> getInput();
	private:
		std::map<std::pair<sf::Keyboard::Key, InputState>, std::vector<InputAction>> inputMap;
		std::vector<InputAction> values;
		//std::map<sf::Keyboard::Key, bool> keyReleasedCheker;
		std::map<std::pair<sf::Keyboard::Key, InputState>, bool> inputActive;

	};


}