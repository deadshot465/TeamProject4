#pragma once
#include <algorithm>
#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <string_view>
#include <utility>
#include <vector>
#include "Helper.h"

class Entity;
class Scene;

class Component {
public:
	Component();
	explicit Component(std::string_view name);
	Component(const Component& component);
	Component(Component&& component) noexcept;
	virtual ~Component() = default;

	Component& operator=(const Component& component);
	Component& operator=(Component&& component) noexcept;

protected:
	std::string mName = "";
	Entity* mEntity = nullptr;
	virtual void Initialize() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Render() = 0;

	friend class Entity;	
};

class Entity final
{
public:
	Entity();
	explicit Entity(std::string_view name);
	Entity(const Entity& entity) = delete;
	Entity(Entity&& entity) noexcept;
	~Entity();

	void Initialize();

	template <typename T>
	T* AddComponent();

	template <typename T>
	T* AddComponent(std::string_view name);

	template <typename T>
	T* AddComponent(std::unique_ptr<T>& component);

	template <typename T>
	T* GetComponent();

	template <typename T>
	T* GetComponent(std::string_view name);

	void RemoveComponent(std::string_view name);
	void AttachToScene(Scene* scene);
	void DetachFromScene();

	Entity& operator=(const Entity& entity) = delete;
	Entity& operator=(Entity&& entity) noexcept;

	glm::vec2 Position = {};
	bool IsRelative = false;
	glm::vec2 RelativePosition = {};
	Entity* Parent = nullptr;
	Scene* CurrentScene = nullptr;

private:
	std::vector<std::unique_ptr<Component>> mComponents =
		std::vector<std::unique_ptr<Component>>();
	std::string mName = "";

	void Update(float deltaTime);
	void Render();

	friend class Component;
	friend class Scene;
};

class Scene
{
public:
	Scene();
	explicit Scene(std::string_view name);
	Scene(const Scene& scene) = delete;
	Scene(Scene&& scene) noexcept;
	virtual ~Scene();
	virtual void Initialize();
	virtual void Update(float deltaTime);
	virtual void Render();

	Entity* AddEntity();
	Entity* AddEntity(std::string_view name);
	Entity* AddEntity(Entity entity);
	Entity* GetEntity(std::string_view name);
	void RemoveEntity(std::string_view name);

	Scene& operator=(const Scene& scene) = delete;
	Scene& operator=(Scene&& scene) noexcept;

	bool IsInitialized() const noexcept;

protected:
	std::vector<Entity> mEntities = std::vector<Entity>();
	std::string mName = "";
	bool mInitialized = false;

	friend class Entity;
};

template<typename T>
inline T* Entity::AddComponent()
{
	auto& obj = mComponents.emplace_back(std::make_unique<T>());
	obj->mEntity = this;
	return dynamic_cast<T*>(obj.get());
}

template<typename T>
inline T* Entity::AddComponent(std::string_view name)
{
	auto& obj = mComponents.emplace_back(std::make_unique<T>(name));
	obj->mEntity = this;
	return dynamic_cast<T*>(obj.get());
}

template<typename T>
inline T* Entity::AddComponent(std::unique_ptr<T>& component)
{
	auto& obj = mComponents.emplace_back(std::move(component));
	obj->mEntity = this;
	return dynamic_cast<T*>(obj.get());
}

template<typename T>
inline T* Entity::GetComponent()
{
	for (auto& component : mComponents) {
		T* derived = dynamic_cast<T*>(component.get());
		if (derived) return derived;
	}
	return nullptr;
}

template<typename T>
inline T* Entity::GetComponent(std::string_view name)
{
	auto iter = std::find_if(mComponents.begin(), mComponents.end(),
		[&name](const std::unique_ptr<Component>& component) {
			return component->mName == name.data();
		});

	return dynamic_cast<T*>(&(*iter));
}
