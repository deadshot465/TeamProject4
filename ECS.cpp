#include "ECS.h"

Component::Component()
	: mName(GetRandomString(10))
{
}

Component::Component(std::string_view name)
	: mName(name)
{
}

Component::Component(const Component& component)
{
	mName = component.mName;
	mEntity = component.mEntity;
}

Component::Component(Component&& component) noexcept
{
	mName = component.mName;
	mEntity = component.mEntity;

	component.mName.clear();
	component.mEntity = nullptr;
}

Component& Component::operator=(const Component& component)
{
	if (&component == this) return *this;

	mName = component.mName;
	mEntity = component.mEntity;

	return *this;
}

Component& Component::operator=(Component&& component) noexcept
{
	if (&component == this) return *this;

	mName = component.mName;
	mEntity = component.mEntity;

	component.mName.clear();
	component.mEntity = nullptr;

	return *this;
}

Entity::Entity() : mName(GetRandomString(7))
{
}

Entity::Entity(std::string_view name) : mName(name)
{
}

Entity::Entity(Entity&& entity) noexcept
{
	mName = entity.mName;
	mScene = entity.mScene;
	std::move(entity.mComponents.begin(), entity.mComponents.end(), mComponents.begin());

	entity.mName = "";
	entity.mScene = nullptr;
	entity.mComponents.clear();
}

Entity::~Entity()
{
	mComponents.clear();
}

void Entity::Initialize()
{
	for (auto& component : mComponents) {
		component->Initialize();
	}
}

void Entity::RemoveComponent(std::string_view name)
{
	std::remove_if(mComponents.begin(), mComponents.end(),
		[&](const std::unique_ptr<Component>& component) {
			return component->mName == name.data();
		});
}

void Entity::AttachToScene(Scene* scene)
{
	mScene = scene;
}

void Entity::DetachFromScene()
{
	mScene = nullptr;
}

Entity& Entity::operator=(Entity&& entity) noexcept
{
	if (&entity == this) return *this;
	if (!mComponents.empty()) mComponents.clear();

	mName = entity.mName;
	mScene = entity.mScene;
	std::move(entity.mComponents.begin(), entity.mComponents.end(), mComponents.begin());

	entity.mName = "";
	entity.mScene = nullptr;
	entity.mComponents.clear();

	return *this;
}

void Entity::Update(float deltaTime)
{
	for (auto& component : mComponents) {
		component->Update(deltaTime);
	}
}

void Entity::Render()
{
	for (auto& component : mComponents) {
		component->Render();
	}
}

Scene::Scene() : mName(GetRandomString(5))
{
}

Scene::Scene(std::string_view name) : mName(name)
{
}

Scene::Scene(Scene&& scene) noexcept
{
	mName = scene.mName;
	std::move(scene.mEntities.begin(), scene.mEntities.end(), mEntities.begin());

	scene.mName = "";
	scene.mEntities.clear();
}

Scene::~Scene()
{
	mEntities.clear();
}

void Scene::Update(float deltaTime)
{
	for (auto& entity : mEntities)
		entity.Update(deltaTime);
}

void Scene::Render()
{
	for (auto& entity : mEntities)
		entity.Render();
}

Entity* Scene::AddEntity()
{
	Entity& entity = mEntities.emplace_back(Entity());
	entity.mScene = this;
	return &entity;
}

Entity* Scene::AddEntity(std::string_view name)
{
	Entity& entity = mEntities.emplace_back(Entity(name));
	entity.mScene = this;
	return &entity;
}

Entity* Scene::AddEntity(Entity entity)
{
	Entity& _entity = mEntities.emplace_back(std::move(entity));
	_entity.mScene = this;
	return &_entity;
}

Entity* Scene::GetEntity(std::string_view name)
{
	auto iter = std::find_if(mEntities.begin(), mEntities.end(),
		[&](const Entity& entity) {
			return entity.mName == name.data();
		});

	return &(*iter);
}

void Scene::RemoveEntity(std::string_view name)
{
	std::remove_if(mEntities.begin(), mEntities.end(),
		[&](const Entity& entity) {
			return entity.mName == name.data();
		});
}

Scene& Scene::operator=(Scene&& scene) noexcept
{
	if (&scene == this) return *this;
	if (!mEntities.empty()) mEntities.clear();

	mName = scene.mName;
	std::move(scene.mEntities.begin(), scene.mEntities.end(), mEntities.begin());

	scene.mName = "";
	scene.mEntities.clear();

	return *this;
}

bool Scene::IsInitialized() const noexcept
{
	return mInitialized;
}
