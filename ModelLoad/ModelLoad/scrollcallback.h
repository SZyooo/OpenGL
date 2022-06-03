#pragma once
#include<string>
namespace callback {
	class ScrollCallback {
	public:
		ScrollCallback(std::string name)
			:_name(name) {}
		void virtual run(float vd, float hd) = 0;
		const std::string Name() const { return _name; }
	private:
		std::string _name;
	};
};