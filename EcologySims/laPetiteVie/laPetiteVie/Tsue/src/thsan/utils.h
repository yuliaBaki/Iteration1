#pragma once
#include <vector>
#include <fstream>
#include <streambuf>
#include <iostream>
#include <sstream>

namespace Utils{
	template<typename T>
	std::vector<T> convertParamsToVector(std::string stringParams) {
		std::vector<T> params;
		std::stringstream ss(stringParams);
		for (T i; ss >> i;) {
			params.push_back(i);
			if (ss.peek() == ',')
				ss.ignore();
		}

		return params;
	}
}