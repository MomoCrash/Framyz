#pragma once

#include "Color.h"
#include "Shader.h"
#include "Texture.h"

class Material {
    Color m_ambientColor;
    Color m_diffuseColor;
    Texture* m_diffuseTexture;
    Color m_specularColor;
    Texture* m_specularTexture;
    float m_shininess;

public:
    Material();
    void SetAmbientColor(Color _ambientColor);
    void SetDiffuseColor(Color _diffuseColor);
    void SetDiffuseTexture(Texture* _diffuseTexture);
    void SetSpecularColor(Color _specularColor);
    void SetSpecularTexture(Texture* _specularTexture);
    void SetShininess(float _shininess);

    Color GetAmbientColor() const;
    Color GetDiffuseColor() const;
    Texture* GetDiffuseTexture() const;
    Color GetSpecularColor() const;
    Texture* GetSpecularTexture() const;
    float GetShininess() const;

    void Use() const;
    void Shutdown() const;

    friend Shader;
};

#endif //MINIGL_MATERIAL_H