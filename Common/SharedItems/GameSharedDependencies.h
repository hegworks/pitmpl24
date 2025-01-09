#pragma once

/*
* REF: this class has been written using the help of claude.ai
* But I completely know what is going on.
*/

#include <stdexcept>
#include <string>
#include <typeindex>
#include <unordered_map>

class GameSharedDependencies
{
public:
	template<typename T>
	static void Set(T* service)
	{
		auto typeIdx = std::type_index(typeid(T));
		if(m_dependencies.find(typeIdx) != m_dependencies.end())
		{
			throw std::runtime_error(std::string("Dependency of type ") + typeid(T).name() + " is already set");
		}
		m_dependencies[typeIdx] = service;
	}

	template<typename T>
	static T* Get()
	{
		auto typeIdx = std::type_index(typeid(T));
		auto it = m_dependencies.find(typeIdx);
		if(it == m_dependencies.end())
		{
			throw std::runtime_error(std::string("Dependency of type ") + typeid(T).name() + " is not set");
		}
		return static_cast<T*>(it->second);
	}

	template<typename T>
	static void Remove()
	{
		auto typeIdx = std::type_index(typeid(T));
		auto it = m_dependencies.find(typeIdx);
		if(it == m_dependencies.end())
		{
			throw std::runtime_error(std::string("Dependency of type ") + typeid(T).name() + " is not set");
		}
		m_dependencies.erase(it);
	}

private:
	static inline std::unordered_map<std::type_index, void*> m_dependencies;
};
