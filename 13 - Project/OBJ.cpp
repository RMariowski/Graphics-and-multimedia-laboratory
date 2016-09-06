#include "OBJ.h"
#include <QDebug>
#include <QFile>
#include <QTemporaryFile>
#include <QDir>

OBJ::OBJ()
{

}

OBJ::~OBJ()
{
    clear();
}

bool OBJ::load(const QString& basePath, const QString& file)
{
    clear();

    QFile qf(basePath + file);
    if (!qf.open(QFile::ReadOnly | QFile::Text) || !qf.isOpen())
    {
        qDebug() << "Cannot open an obj file!";
        return false;
    }

    QTextStream qts(&qf);
    Object* currentObject = 0;
    int line = 0;
    while (!qts.atEnd())
    {
        line++;
        //qDebug() << line;

        QString line = qts.readLine();
        QStringList fields = line.split(" ").filter(QRegExp("^\\S+$"));
        if (fields.size() == 0)
            continue;

        QString lineID = fields.at(0);

        if (lineID == "v")
        {
            QVector3D v(fields.at(1).toFloat(), fields.at(2).toFloat(), fields.at(3).toFloat());
            _vertices.push_back(v);
        }
        else if (lineID == "vt")
        {
            float u = fields.at(1).toFloat();
            float v = fields.at(2).toFloat();
            if (u < 0.0f || u > 1.0f)
            {
                qDebug() << "Unsupported obj";
                return false;
            }
            else if (v < 0.0f || v > 1.0f)
            {
                qDebug() << "Unsupported obj";
                return false;
            }

            QVector2D vt(u, v);
            _vts.push_back(vt);
        }
        else if (lineID == "f")
        {
            QStringList faceFields1 = fields.at(1).split("/");
            QStringList faceFields2 = fields.at(2).split("/");
            QStringList faceFields3 = fields.at(3).split("/");

            Triangle triangle;
            QVector3D v0 = _vertices[faceFields1.at(0).toInt() - 1];
            QVector3D v1 = _vertices[faceFields2.at(0).toInt() - 1];
            QVector3D v2 = _vertices[faceFields3.at(0).toInt() - 1];

            triangle.setVertex(0, v0);
            triangle.setVertex(1, v1);
            triangle.setVertex(2, v2);

            QVector2D vt0 = _vts[faceFields1.at(1).toInt() - 1];
            QVector2D vt1 = _vts[faceFields2.at(1).toInt() - 1];
            QVector2D vt2 = _vts[faceFields3.at(1).toInt() - 1];

            triangle.setTexcoord(0, vt0);
            triangle.setTexcoord(1, vt1);
            triangle.setTexcoord(2, vt2);

            triangle.setTexture(currentObject->material->texture);

            _triangles.push_back(triangle);
        }
        else if (lineID == "o")
        {
            Object object;
            object.name = fields.at(1);
            _objects.push_back(object);

            currentObject = &_objects[_objects.size() - 1];
        }
        else if (lineID == "g" && currentObject == 0)
        {
            Object object;
            object.name = "global";
            _objects.push_back(object);

            currentObject = &_objects[_objects.size() - 1];
        }
        else if (lineID == "usemtl")
        {
            QString key = fields.at(1);
            if (_materials.find(key) == _materials.end())
                return false;
            currentObject->material = &_materials[key];

        }
        else if (lineID == "mtllib")
        {
            bool result = loadMaterialFile(basePath, fields.at(1));
            if (!result)
                return false;
        }
    }

    qf.close();

    calculateModelSize();

    return true;
}

bool OBJ::loadMaterialFile(const QString& basePath, const QString& file)
{
    QFile qf(basePath + file);
    if (!qf.open(QFile::ReadOnly | QFile::Text) || !qf.isOpen())
    {
        qDebug() << "Cannot open a mtl file!";
        return false;
    }

    QTextStream qts(&qf);

    Material* currentMaterial = 0;
    while (!qts.atEnd())
    {
        QString line = qts.readLine();
        QStringList fields = line.split(" ").filter(QRegExp("^\\S+$"));
        if (fields.size() == 0)
            continue;

        QString lineID = fields.at(0);
        if (lineID == "newmtl")
        {
            Material material;
            QString key = fields.at(1);
            _materials[key] = material;
            currentMaterial = &_materials[key];
        }
        else if (lineID == "map_Kd")
        {
            currentMaterial->textureName = fields.at(1);

            QString texPath = basePath + currentMaterial->textureName;
            QImage* tex = new QImage(texPath);
            if (tex->isNull())
            {
                qDebug() << "Error while loading texture:" << texPath;
                return false;
            }

            _textures.push_back(tex);
            currentMaterial->texture = _textures[_textures.size() - 1];

        }
    }

    qf.close();
    return true;
}

void OBJ::calculateModelSize()
{
    float minX = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::min();
    float minY = std::numeric_limits<float>::max();
    float maxY = std::numeric_limits<float>::min();
    float minZ = std::numeric_limits<float>::max();
    float maxZ = std::numeric_limits<float>::min();

    for (size_t i = 0; i < _triangles.size(); i++)
    {
        Triangle* triangle = &_triangles[i];
        for (int j = 0; j < 3; j++)
        {
            QVector3D v3d = triangle->getVertex(j);
            if (v3d.x() < minX) minX = v3d.x();
            if (v3d.x() > maxX) maxX = v3d.x();
            if (v3d.y() < minY) minY = v3d.y();
            if (v3d.y() > maxY) maxY = v3d.y();
            if (v3d.z() < minZ) minZ = v3d.z();
            if (v3d.z() > maxZ) maxZ = v3d.z();
        }
    }

    _modelSize.setX(maxX + abs(minX));
    _modelSize.setY(maxY + abs(minY));
    _modelSize.setZ(maxZ + abs(minZ));
}

void OBJ::clear()
{
    for (int i = 0; i < _textures.size(); i++)
        delete _textures[i];
    _textures.clear();

    _vertices.clear();
    _vts.clear();
    _triangles.clear();
    _materials.clear();
    _objects.clear();
}
