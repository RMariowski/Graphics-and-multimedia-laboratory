#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <QVector2D>
#include <QVector3D>
#include <QVector4D>

class Triangle
{
public:
    Triangle();

    Triangle(QVector3D v1, QVector3D v2, QVector3D v3);

    void applyTransforms(const QVector3D& pos, const QVector3D& scale,
                         const QVector3D& rotation, const QVector3D& shearing);

    QVector3D getVertex(int i) const {
        return _vertices[i];
    }

    QVector2D getTexcoords(int i) const {
        return _texcoords[i];
    }

    QImage* getTexture() const {
        return _texture;
    }

    QVector4D getTransformedVertex(int i) const {
        return _transformedVertices[i];
    }

    void setVertex(int i, const QVector3D& v) {
        _vertices[i] = v;
    }

    void setTexcoord(int i, const QVector2D& t) {
        _texcoords[i] = t;
    }

    void setTexture(QImage* texture) {
        _texture = texture;
    }

private:
    QVector3D _vertices[3];
    QVector2D _texcoords[3];
    QImage* _texture;
    QVector4D _transformedVertices[3];
};

#endif // TRIANGLE_H
