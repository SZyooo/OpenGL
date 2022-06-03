#pragma once
#include<string>
namespace callback {
	class KeyCallback {
	public:
		KeyCallback(std::string name)
			:_name(name) {}
		void virtual run(int key) = 0;
		const std::string Name() const { return _name; }
	private:
		std::string _name;
	};
};