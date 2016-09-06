#ifndef OBJ_H
#define OBJ_H

#include <QImage>
#include <QMap>

#include "Triangle.h"

struct Material
{
    QString textureName;
    QImage* texture;
};

struct Object
{
    QString name;
    Material* material;
};

class OBJ
{
public:
    OBJ();

    virtual ~OBJ();

    bool load(const QString& basePath, const QString& file);

    std::vector<Triangle>& getTriangles() {
        return _triangles;
    }

    QVector3D getModelSize() const {
        return _modelSize;
    }

private:
    bool loadMaterialFile(const QString& basePath, const QString& file);

    void calculateModelSize();

    void clear();

private:
    QList<Object> _objects;
    QList<QVector3D> _vertices;
    QList<QVector2D> _vts;
    QMap<QString, Material> _materials;
    QList<QImage*> _textures;
    std::vector<Triangle> _triangles;

    QVector3D _modelSize;
};

#endif // OBJ_H
