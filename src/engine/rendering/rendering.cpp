/*
** Agartha-Software, 2023
** Cevy
** File description:
** Rendering
*/

#include "rendering.hpp"
#include "Color.hpp"
#include "Line.hpp"
#include "Transform.hpp"
#include "cevy.hpp"
#include "raylib.hpp"
#include <raylib.h>
#include <raymath.h>

using namespace cevy::engine;
using namespace cevy;

void render_lines(ecs::Query<option<Position>, Line, option<cevy::engine::Color>> lines) {
  for (auto [opt_pos, lines, opt_color] : lines) {
    const Position &pos = opt_pos.value_or(Position(0., 0., 0.));
    const cevy::engine::Color &col = opt_color.value_or(cevy::engine::Color(0., 255., 60));

    DrawCylinderEx(lines.start + pos, lines.end + pos, 0.1, 0.1, 4, (::Color)col);
  }
}

static void render_model(const Model& model, Matrix transform)
{
  transform = MatrixMultiply(model.transform, transform);
  for (int i = 0; i < model.meshCount; ++i) {
    DrawMesh(model.meshes[i], model.materials[model.meshMaterial[i]], transform);
  }
}

void render_models(ecs::Query<option<Position>, option<engine::Transform>, Handle<engine::Mesh>,
                              option<Handle<Diffuse>>, option<engine::Color>>
                       models) {
  for (auto [opt_pos, opt_tm, mesh, opt_diffuse, opt_color] : models) {
    const Position &pos = opt_pos.value_or(Position(0., 0., 0.));
    const cevy::engine::Transform &tm = opt_tm.value_or(cevy::engine::Transform(pos));
    auto &handle = mesh.get();
    ::Color ray_color;
    if (opt_color) {
      ray_color = opt_color.value();
    } else
      ray_color = ::WHITE;
    if (opt_diffuse) {
      SetMaterialTexture(handle.mesh.materials, MATERIAL_MAP_DIFFUSE,
                         opt_diffuse.value().get().texture);
    }
    render_model(handle.mesh, tm);
    if (opt_diffuse) {
      handle.mesh.materialCount = 1;
      handle.mesh.materials = (Material *)RL_CALLOC(handle.mesh.materialCount, sizeof(Material));
      handle.mesh.materials[0] = LoadMaterialDefault();
    }
  }
}
