#pragma once
#include<unordered_map>
#include <SFML/Graphics.hpp>
#include <concepts>
#include <optional>
#include <functional>
namespace ECS {
	using Entity = std::uint32_t;


	//Component
	struct Transform
	{
		sf::Vector2f position;
		sf::Vector2f scale;
		sf::FloatRect bounds = sf::FloatRect();
	};
	 
	struct SpriteData 
	{
		sf::Texture texture;
	
		sf::IntRect textureRect;
		bool useTextureRect = false;
		sf::Sprite* sprite = nullptr;
	};

	struct Personality
	{
		bool canMove;
	};
	struct Depth
	{
		float offsetY = 0.f;
	};

	struct Collider {
		sf::FloatRect bounds;
		bool isStatic = true;
	};

	template <typename T>
	concept  ComponentConcept = std::is_same_v<T, Collider> || std::is_same_v<T, Depth> ||std::is_same_v<T, Transform>||std::is_same_v<T, SpriteData> || std::is_same_v<T, Personality>;

	class World {
	public:
		static World& get_instance() {
			static World instance;
			return instance;
		}

		Entity createEntity() {
			return nextEntity++;
		}

		template <ComponentConcept Component, typename ... Args>
		Component& add_component(Entity e, Args&&...args) {
			auto& storage = get_storage<Component>();
			return storage.add_component(e, std::forward<Args>(args)...);
		}
		template <ComponentConcept Component>
		std::optional<std::reference_wrapper<Component>> get_component(Entity e) {
			auto& storage = get_storage<Component>();
			return storage.get_component(e);
		}
		template <ComponentConcept Component>
		auto get_component_map() {
			return get_storage<Component>().get_map();
		}
	private:
		World() = default;
	private:
		template <ComponentConcept Component>
		class Storage {
		public:
			template <typename ... Args>
			Component& add_component(Entity e,Args&&...args) {
				auto [it, inserted] = m_storage.emplace(e, Component {std::forward<Args>(args)...});
				return it->second;
			}

			std::optional<std::reference_wrapper<Component>> get_component(Entity e) {
				auto it = m_storage.find(e);
				if(it == m_storage.end()) {
					return std::nullopt;
				}
				return it->second;
			}

			auto& get_map() {
				return m_storage;
			}

		private:
			std::unordered_map<Entity, Component> m_storage;
		};

		template <ComponentConcept Component>
		Storage<Component>& get_storage() {
			static Storage<Component> storage;
			return storage;
		}
	private:
		Entity nextEntity = 1;
		

	};
};