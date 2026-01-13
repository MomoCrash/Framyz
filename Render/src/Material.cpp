#include "Material.h"

Material::Material() : m_ambientColor(Color::White), m_diffuseColor(Color::White), m_diffuseTexture(Texture::Default),
                             m_specularColor({0.5, 0.5,0.5, 1}), m_specularTexture(Texture::Default), m_shininess(32) {
}

void Material::SetAmbientColor(Color _ambientColor) {
    m_ambientColor = _ambientColor;
}

void Material::SetDiffuseColor(Color _diffuseColor) {
    m_diffuseColor = _diffuseColor;
}

void Material::SetDiffuseTexture(Texture* _diffuseTexture) {
    m_diffuseTexture = _diffuseTexture;
}

void Material::SetSpecularColor(Color _specularColor) {
    m_specularColor = _specularColor;
}

void Material::SetSpecularTexture(Texture* _specularTexture) {
    m_specularTexture = _specularTexture;
}

void Material::SetShininess(float _shininess) {
    m_shininess = _shininess;
}

Color Material::GetAmbientColor() const {
    return m_ambientColor;
}

Color Material::GetDiffuseColor() const {
    return m_diffuseColor;
}

Texture* Material::GetDiffuseTexture() const {
    return m_diffuseTexture;
}

Color Material::GetSpecularColor() const {
    return m_specularColor;
}

Texture* Material::GetSpecularTexture() const {
    return m_specularTexture;
}

float Material::GetShininess() const {
    return m_shininess;
}

void Material::Use() const {
    m_diffuseTexture->Use(0);
    m_specularTexture->Use(1);
}

void Material::Shutdown() const {
    m_diffuseTexture->Shutdown(0);
    m_specularTexture->Shutdown(1);
}
