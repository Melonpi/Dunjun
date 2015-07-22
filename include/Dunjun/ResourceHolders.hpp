#ifndef DUNJUN_RESOURCEHOLDERS_HPP
#define DUNJUN_RESOURCEHOLDERS_HPP

#include <Dunjun/ResourceHolder.hpp>
#include <Dunjun/Core/String.hpp>
#include <Dunjun/Graphics.hpp>
#include <Dunjun/Graphics/Texture.hpp>

#include <string>

namespace Dunjun
{/*
class TextureHolder : public ResourceHolder<Texture, std::string>
{
public:
    bool insertFromFile(const std::string& id,
                        const std::string& filename,
                        TextureFilter minMagFilter = TextureFilter::Linear,
                        TextureWrapMode wrapMode = TextureWrapMode::ClampToEdge)
    {
        Texture tex = loadTextureFromFile(
            std::string{BaseDirectory::Textures + filename}.c_str(),
            minMagFilter,
            wrapMode);

        bool t = true;

        if (!tex.handle || !tex.width || !tex.height)
            t = false;

        if (t)
            insert(id, std::unique_ptr<Texture>{new Texture{tex}});
        return t;
    }
};*/

class ShaderHolder : public ResourceHolder<ShaderProgram, std::string>
{
public:
	bool insertFromFile(const std::string& id,
	                    const char* vertexFilename,
	                    const char* fragmentFilename,
	                    bool bindDefaultAttribLocation = true)
	{
		std::unique_ptr<ShaderProgram> shaders = make_unique<ShaderProgram>();
		if (!shaders->attachShaderFromFile(ShaderType::Vertex, vertexFilename))
		{
			std::cerr << shaders->errorLog << std::endl;

			return false;
		}
		if (!shaders->attachShaderFromFile(ShaderType::Fragment,
		                                   fragmentFilename))
		{
			std::cerr << shaders->errorLog << std::endl;

			return false;
		}

		if (bindDefaultAttribLocation)
		{
			shaders->bindAttribLocation((s32)AttribLocation::Position,
			                            "a_position");
			shaders->bindAttribLocation((s32)AttribLocation::TexCoord,
			                            "a_texCoord");
			shaders->bindAttribLocation((s32)AttribLocation::Color,
			                            "a_color");
			shaders->bindAttribLocation((s32)AttribLocation::Normal,
			                            "a_normal");
		}

		if (!shaders->link())
		{
			std::cerr << shaders->errorLog << std::endl;

			return false;
		}

		insert(id, std::move(shaders));

		return true;
	}
};

using MeshHolder     = ResourceHolder<Mesh, std::string>;
using MaterialHolder = ResourceHolder<Material, std::string>;

// extern TextureHolder g_textureHolder;
extern ShaderHolder g_shaderHolder;
extern MeshHolder g_meshHolder;
extern MaterialHolder g_materialHolder;
} // namespace Dunjun

#endif
