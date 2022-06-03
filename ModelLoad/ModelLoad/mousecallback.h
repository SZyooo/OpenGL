#pragma once
#include<utility>
#include<string>
namespace callback
{
	typedef std::pair<double,double> dx_dy;
	class MouseCallback {
	public:
		virtual void run(const dx_dy&) = 0;
		MouseCallback(std::string name) :_name(name){}
		std::string Name() const { return _name; }
	private:
		std::string _name;
	};
};