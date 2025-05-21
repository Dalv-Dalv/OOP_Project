#include "AssetManager.h"
#include <raylib.h>
#include <rlgl.h>
#include <filesystem>
#include <iostream>

std::unordered_map<std::string, Texture2D> AssetManager::textures;
std::unordered_map<std::string, Shader> AssetManager::shaders;
std::unordered_map<std::string, Sound> AssetManager::sounds;


const Texture2D& AssetManager::LoadTexture(const std::string& path) {
	auto it = textures.find(path);
	if (it != textures.end()) return it->second;

	Texture2D tex = ::LoadTexture(path.c_str());
	textures[path] = tex;
	return textures[path];
}

const Shader& AssetManager::LoadShader(const std::string& path) {
	auto it = shaders.find(path);
	if (it != shaders.end()) return it->second;

	Shader shader = ::LoadShader(0, path.c_str());
	shaders[path] = shader;
	return shaders[path];
}

const Sound& AssetManager::LoadSound(const std::string& path) {
	auto it = sounds.find(path);
	if (it != sounds.end()) return it->second;

	Sound sound = ::LoadSound(path.c_str());
	sounds[path] = sound;
	return sounds[path];
}


void AssetManager::UnloadAll() {
	for (auto& [_, tex] : textures)
		UnloadTexture(tex);
	for (auto& [_, shader] : shaders)
		UnloadShader(shader);
}


