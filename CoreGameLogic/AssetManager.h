#pragma once
#include <string>
#include <unordered_map>

#include "raylib.h"

class AssetManager {
private:
	static std::unordered_map<std::string, Texture2D> textures;
	static std::unordered_map<std::string, Shader> shaders;
	static std::unordered_map<std::string, Sound> sounds;
	AssetManager() = default;

public:
	static const Texture2D& LoadTexture(const std::string& path);
	static const Shader& LoadShader(const std::string& path);
	static const Sound& LoadSound(const std::string& path);

	static void UnloadAll();
};
