#include "testing.hxx"
#include "affine.hxx"

using namespace math;

namespace {
    constexpr float pi = 3.14159265358979323846F;
}

void transform_test_identity()
{
    const transform t{{0.0F, 0.0F, 0.0F},
                      {0.0F, 0.0F, 0.0F},
                      {1.0F, 1.0F, 1.0F}};

    EQ(t.model_matrix(), matrix4x4{});
}

void transform_test_translation()
{
    const transform t{{1.0F, 2.0F, 3.0F},
                      {0.0F, 0.0F, 0.0F},
                      {1.0F, 1.0F, 1.0F}};
    const vector4 p{0.0F, 0.0F, 0.0F, 1.0F};

    const vector4 result = t.model_matrix() * p;
    FLOAT_EQ(result.x(), 1.0F);
    FLOAT_EQ(result.y(), 2.0F);
    FLOAT_EQ(result.z(), 3.0F);
}

void transform_test_scaling()
{
    const transform t{{0.0F, 0.0F, 0.0F},
                      {0.0F, 0.0F, 0.0F},
                      {2.0F, 3.0F, 4.0F}};
    const vector4 p{1.0F, 1.0F, 1.0F, 1.0F};

    const vector4 result = t.model_matrix() * p;
    FLOAT_EQ(result.x(), 2.0F);
    FLOAT_EQ(result.y(), 3.0F);
    FLOAT_EQ(result.z(), 4.0F);
}

void transform_test_rotation()
{
    const transform t{{0.0F, 0.0F, 0.0F},
                      {0.0F, 0.0F, pi / 2.0F},
                      {1.0F, 1.0F, 1.0F}};
    const vector4 p{1.0F, 0.0F, 0.0F, 1.0F};

    const vector4 result = t.model_matrix() * p;
    FLOAT_EQ(result.x(), 0.0F);
    FLOAT_EQ(result.y(), 1.0F);
    FLOAT_EQ(result.z(), 0.0F);
}

void transform_test_trs_order()
{
    const transform t{{1.0F, 0.0F, 0.0F},
                      {0.0F, 0.0F, 0.0F},
                      {2.0F, 2.0F, 2.0F}};
    const vector4 p{1.0F, 1.0F, 1.0F, 1.0F};

    const vector4 result = t.model_matrix() * p;
    FLOAT_EQ(result.x(), 3.0F);
    FLOAT_EQ(result.y(), 2.0F);
    FLOAT_EQ(result.z(), 2.0F);
    const vector4 direction{1.0F, 1.0F, 1.0F, 0.0F};
    const vector4 dr = t.model_matrix() * direction;
    FLOAT_EQ(dr.x(), 2.0F);
    FLOAT_EQ(dr.y(), 2.0F);
    FLOAT_EQ(dr.z(), 2.0F);
}