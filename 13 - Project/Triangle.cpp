#include "Triangle.h"
#include <QMatrix4x4>

QVector4D multiply(const QMatrix4x4& matrix, const QVector4D& vector)
{
    QVector4D vec;
    vec.setX(matrix(0, 0) * vector.x() + matrix(0, 1) * vector.y() + matrix(0, 2) * vector.z() + matrix(0, 3) * vector.w());
    vec.setY(matrix(1, 0) * vector.x() + matrix(1, 1) * vector.y() + matrix(1, 2) * vector.z() + matrix(1, 3) * vector.w());
    vec.setZ(matrix(2, 0) * vector.x() + matrix(2, 1) * vector.y() + matrix(2, 2) * vector.z() + matrix(2, 3) * vector.w());
    vec.setW(matrix(3, 0) * vector.x() + matrix(3, 1) * vector.y() + matrix(3, 2) * vector.z() + matrix(3, 3) * vector.w());
    return vec;
}
Triangle::Triangle()
{

}

Triangle::Triangle(QVector3D v1, QVector3D v2, QVector3D v3)
{
    _vertices[0] = v1;
    _vertices[1] = v2;
    _vertices[2] = v3;
}

void Triangle::applyTransforms(const QVector3D& pos, const QVector3D& scale,
                               const QVector3D& rotation, const QVector3D& shearing)
{
    float sinx = sinf(rotation.x());
    float siny = sinf(rotation.y());
    float sinz = sinf(rotation.z());
    float cosx = cosf(rotation.x());
    float cosy = cosf(rotation.y());
    float cosz = cosf(rotation.z());

    for (int i = 0; i < 3; i++)
    {
        QVector3D* v3d = &_vertices[i];
        _transformedVertices[i] = QVector4D(v3d->x(),
                                            v3d->y(),
                                            v3d->z(),
                                            1.0f);
    }

    QMatrix4x4 translation(1.0f, 0.0f, 0.0f, pos.x(),
                           0.0f, 1.0f, 0.0f, pos.y(),
                           0.0f, 0.0f, 1.0f, pos.z(),
                           0.0f, 0.0f, 0.0f, 1.0f);

    QMatrix4x4 scaleM(scale.x(), 0.0f, 0.0f, 0.0f,
                      0.0f, scale.y(), 0.0f, 0.0f,
                      0.0f, 0.0f, scale.z(), 0.0f,
                      0.0f, 0.0f, 0.0f, 1.0f);

    QMatrix4x4 rotationX(1.0f, 0.0f, 0.0f, 0.0f,
                         0.0f, cosx, -sinx, 0.0f,
                         0.0f, sinx, cosx, 0.0f,
                         0.0f, 0.0f, 0.0f, 1.0f);

    QMatrix4x4 rotationY(cosy, 0.0f, siny, 0.0f,
                         0.0f, 1.0f, 0.0f, 0.0f,
                         -siny, 0.0f, cosy, 0.0f,
                         0.0f, 0.0f, 0.0f, 1.0f);

    QMatrix4x4 rotationZ(cosz, -sinz, 0.0f, 0.0f,
                         sinz, cosz, 0.0f, 0.0f,
                         0.0f, 0.0f, 1.0f, 0.0f,
                         0.0f, 0.0f, 0.0f, 1.0f);

    QMatrix4x4 shearX(1.0f, 0.0f, 0.0f, 0.0f,
                      shearing.y(), 1.0f, 0.0f, 0.0f,
                      shearing.z(), 0.0f, 1.0f, 0.0f,
                      0.0f, 0.0f, 0.0f, 1.0f);

    QMatrix4x4 shearY(1.0f, shearing.x(), 0.0f, 0.0f,
                      0.0f, 1.0f, 0.0f, 0.0f,
                      0.0f, shearing.z(), 1.0f, 0.0f,
                      0.0f, 0.0f, 0.0f, 1.0f);

    QMatrix4x4 shearZ(1.0f, 0.0f, shearing.x(), 0.0f,
                      0.0f, 1.0f, shearing.y(), 0.0f,
                      0.0f, 0.0f, 1.0f, 0.0f,
                      0.0f, 0.0f, 0.0f, 1.0f);

    for (int i = 0; i < 3; i++)
    {
        _transformedVertices[i] = multiply(scaleM, _transformedVertices[i]);
        _transformedVertices[i] = multiply(shearX, _transformedVertices[i]);
        _transformedVertices[i] = multiply(shearY, _transformedVertices[i]);
        _transformedVertices[i] = multiply(shearZ, _transformedVertices[i]);
        _transformedVertices[i] = multiply(rotationX, _transformedVertices[i]);
        _transformedVertices[i] = multiply(rotationY, _transformedVertices[i]);
        _transformedVertices[i] = multiply(rotationZ, _transformedVertices[i]);
        _transformedVertices[i] = multiply(translation, _transformedVertices[i]);
    }
}
