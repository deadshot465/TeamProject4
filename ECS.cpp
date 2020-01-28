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
	: mName(component.mName), mEntity(component.mEntity)
{
}

Component::Component(Component&& component) noexcept
	: mName(std::move(component.mName)), mEntity(std::move(component.mEntity))
{
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

	std::swap(mName, component.mName);
	std::swap(mEntity, component.mEntity);

	return *this;
}

Entity::Entity() : mName(GetRandomString(7))
{
}

Entity::Entity(std::string_view name) : mName(name)
{
}

Entity::Entity(Entity&& entity) noexcept
	: mName(std::move(entity.mName)), CurrentScene(std::move(entity.CurrentScene)),
	mComponents(std::move(entity.mComponents))
{
	entity.mName = "";
	entity.CurrentScene = nullptr;
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
	mComponents.erase(std::remove_if(mComponents.begin(), mComponents.end(),
		[&](const std::unique_ptr<Component>& component) {
			return component->mName == name.data();
		}), mComponents.end());
}

void Entity::AttachToScene(Scene* scene)
{
	CurrentScene = scene;
}

void Entity::DetachFromScene()
{
	CurrentScene = nullptr;
}

const std::string& Entity::GetName() const noexcept
{
	return mName;
}

Entity& Entity::operator=(Entity&& entity) noexcept
{
	if (&entity == this) return *this;
	if (!mComponents.empty()) mComponents.clear();

	std::swap(mName, entity.mName);
	std::swap(CurrentScene, entity.CurrentScene);
	mComponents = std::move(entity.mComponents);

	entity.mComponents.clear();

	return *this;
}

void Entity::Update(float deltaTime)
{
	if (Parent)
	{
		Position = Parent->Position + RelativePosition;
	}

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

Scene::Scene() : mName(GetRandomString(5)), mInitialized(false)
{
}

Scene::Scene(std::string_view name) : mName(name), mInitialized(false)
{
}

Scene::Scene(Scene&& scene) noexcept
	: mName(std::move(scene.mName)), mInitialized(std::move(scene.mInitialized)),
	mEntities(std::move(scene.mEntities))
{
	scene.mName = "";
	scene.mInitialized = false;
	scene.mEntities.clear();
}

Scene::~Scene()
{
	mEntities.clear();
}

void Scene::Initialize()
{
	for (auto& entity : mEntities)
		entity->Initialize();
}

void Scene::Update(float deltaTime)
{
	for (auto& entity : mEntities)
		entity->Update(deltaTime);
}

void Scene::Render()
{
	for (auto& entity : mEntities)
		entity->Render();
}

Entity* Scene::AddEntity()
{
	mEntities.emplace_back(std::make_unique<Entity>());
	auto entity = mEntities.rbegin();
	entity->get()->CurrentScene = this;
	return entity->get();
}

Entity* Scene::AddEntity(std::string_view name)
{
	mEntities.emplace_back(std::make_unique<Entity>(name));
	auto entity = mEntities.rbegin();
	entity->get()->CurrentScene = this;
	return entity->get();
}

Entity* Scene::AddEntity(std::unique_ptr<Entity>& entity)
{
	mEntities.emplace_back(std::move(entity));
	auto _entity = mEntities.rbegin();
	_entity->get()->CurrentScene = this;
	return _entity->get();
}

Entity* Scene::GetEntity(std::string_view name)
{
	auto iter = std::find_if(mEntities.begin(), mEntities.end(),
		[&](const std::unique_ptr<Entity>& entity) {
			return entity->mName == name.data();
		});

	return iter->get();
}

void Scene::RemoveEntity(std::string_view name)
{
	auto iter = std::remove_if(mEntities.begin(), mEntities.end(),
		[&](const std::unique_ptr<Entity>& entity) {
			return entity->mName == name.data();
		});

	mEntities.erase(iter, mEntities.end());
}

Scene& Scene::operator=(Scene&& scene) noexcept
{
	if (&scene == this) return *this;
	if (!mEntities.empty()) mEntities.clear();

	std::swap(mName, scene.mName);
	mEntities = std::move(scene.mEntities);
	scene.mEntities.clear();

	return *this;
}
