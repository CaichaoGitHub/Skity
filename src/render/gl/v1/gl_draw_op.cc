#include "src/render/gl/v1/gl_draw_op.hpp"

#include "glm/gtc/matrix_transform.hpp"
#include "src/render/gl/v1/draw/gl_clear_stencil_op.hpp"
#include "src/render/gl/v1/draw/gl_draw_debug_line_op.hpp"
#include "src/render/gl/v1/draw/gl_fill_color_op.hpp"
#include "src/render/gl/v1/draw/gl_fill_gradient_op.hpp"
#include "src/render/gl/v1/draw/gl_fill_texture_op.hpp"
#include "src/render/gl/v1/draw/gl_stencil_op.hpp"
#include "src/render/gl/gl_shader.hpp"

namespace skity {

void GLDrawOp::Draw(glm::mat4 const& mvp, bool has_clip) {
  OnBeforeDraw(has_clip);

  if (shader_) {
    shader_->SetMVPMatrix(mvp);
  }

  OnDraw(has_clip);
  OnAfterDraw(has_clip);
}

void GLDrawOp::OnBeforeDraw(bool has_clip) {
  if (shader_) {
    shader_->Bind();
  }
}

void GLDrawOp::OnAfterDraw(bool has_clip) {
  if (shader_) {
//    shader_->UnBind();
  }
}

void GLDrawOp::Init() { OnInit(); }

void GLDrawOpBuilder::UpdateStencilShader(StencilShader* shader) {
  stencil_shader = shader;
}

void GLDrawOpBuilder::UpdateColorShader(ColorShader* shader) {
  color_shader = shader;
}

void GLDrawOpBuilder::UpdateGradientShader(GLGradientShader* shader) {
  gradient_shader = shader;
}

void GLDrawOpBuilder::UpdateTextureShader(GLTextureShader* shader) {
  texture_shader = shader;
}

void GLDrawOpBuilder::UpdateMesh(GLMesh* mesh) { gl_mesh = mesh; }

void GLDrawOpBuilder::UpdateFrontStart(uint32_t value) { front_start = value; }

void GLDrawOpBuilder::UpdateFrontCount(uint32_t value) { front_count = value; }

void GLDrawOpBuilder::UpdateBackStart(uint32_t value) { back_start = value; }

void GLDrawOpBuilder::UpdateBackCount(uint32_t value) { back_count = value; }

std::unique_ptr<GLDrawOp> GLDrawOpBuilder::CreateStencilOp(float stroke_width,
                                                           bool positive) {
  auto op = std::make_unique<GLStencilDrawOp>(
      front_start, front_count, back_start, back_count, stencil_shader, gl_mesh,
      positive);
  op->Init();
  op->SetStrokeWidth(stroke_width);

  return op;
}

std::unique_ptr<GLDrawOp> GLDrawOpBuilder::CreateColorOp(float r, float g,
                                                         float b, float a) {
  return CreateColorOpAA(r, g, b, a, 0, 0);
}

std::unique_ptr<GLDrawOp> GLDrawOpBuilder::CreateColorOpAA(float r, float g,
                                                           float b, float a,
                                                           uint32_t aa_start,
                                                           uint32_t aa_count) {
  auto op = std::make_unique<GLFillColorOp>(front_start, front_count,
                                            back_start, back_count, aa_start,
                                            aa_count, color_shader, gl_mesh);
  op->Init();
  op->SetColor(r, g, b, a);

  return op;
}

std::unique_ptr<GLDrawOp> GLDrawOpBuilder::CreateClearStencilOp() {
  auto op = std::make_unique<GLClearStencilOp>();
  op->Init();
  return op;
}

std::unique_ptr<GLDrawOp> GLDrawOpBuilder::CreateDebugLineOp() {
  auto op = std::make_unique<GLDrawDebugLineOp>(
      front_start, front_count, back_start, back_count, color_shader, gl_mesh);

  op->Init();

  return op;
}

std::unique_ptr<GLDrawOp> GLDrawOpBuilder::CreateGradientOp(
    Shader::GradientInfo* info, Shader::GradientType type) {
  return CreateGradientOpAA(info, type, 0, 0);
}

std::unique_ptr<GLDrawOp> GLDrawOpBuilder::CreateGradientOpAA(
    Shader::GradientInfo* info, Shader::GradientType type, uint32_t aa_start,
    uint32_t aa_count) {
  auto op = std::make_unique<GLFillGradientOp>(
      front_start, front_count, back_start, back_count, aa_start, aa_count,
      gradient_shader, gl_mesh);

  op->Init();

  op->SetGradientType(type);
  op->SetColors(info->colors);
  op->SetStops(info->color_offsets);
  op->SetPoints(info->point[0], info->point[1]);
  op->SetRadius(info->radius[0], info->radius[1]);
  op->SetLocalMatrix(info->local_matrix);
  op->SetGradientFlag(info->gradientFlags);

  return op;
}

std::unique_ptr<GLDrawOp> GLDrawOpBuilder::CreateTextureOp(
    const GLTexture* texture, Point const& p1, Point const& p2) {
  return CreateTextureOpAA(texture, p1, p2, 0, 0);
}

std::unique_ptr<GLDrawOp> GLDrawOpBuilder::CreateTextureOpAA(
    const GLTexture* texture, Point const& p1, Point const& p2,
    uint32_t aa_start, uint32_t aa_count) {
  auto op = std::make_unique<GLFillTextureOp>(
      front_start, front_count, back_start, back_count, aa_start, aa_count,
      texture_shader, gl_mesh);

  op->Init();
  op->SetBounds(p1, p2);
  // currently, not use this matrix
  op->SetLocalMatrix(glm::identity<Matrix>());
  op->SetTexture(texture);

  return op;
}

}  // namespace skity