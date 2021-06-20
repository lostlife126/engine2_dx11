#include "renderer.h"
#include <cmath>

void toViewport(Buffer<float>* zBuff, Vect3f* v)
{
	for (int i = 0; i < 3; i++)
	{
		v[i].x = (v[i].x + 1.0f) * 0.5f * zBuff->width + 0.5f;
		v[i].y = (v[i].y + 1.0f) * 0.5f * zBuff->height + 0.5f;
	}
}

void toBoundBox(Buffer<float>* zBuff, Vect3f* v, int& xmin, int& xmax, int& ymin, int& ymax)
{
	xmin = int(min(v[0].x, v[1].x, v[2].x));
	xmax = int(max(v[0].x, v[1].x, v[2].x));
	ymin = int(min(v[0].y, v[1].y, v[2].y));
	ymax = int(max(v[0].y, v[1].y, v[2].y));
	xmin = max(xmin, 0);
	xmax = min(xmax, zBuff->width-1);
	ymin = max(ymin, 0);
	ymax = min(ymax, zBuff->height - 1);
}

void packData(Vect3i& index, Vect3f* prim, std::vector<Vect3f>& vals)
{
	prim[0] = vals[index.data[0]];
	prim[1] = vals[index.data[1]];
	prim[2] = vals[index.data[2]];
}

void Renderer::drawObject(Object* obj)
{
	Mesh* mesh = obj->getMesh();
	std::vector<Vect3i>* vIndices = &mesh->vertexIndices;
	std::vector<Vect3i>* tIndices = &mesh->textureIndices;
	std::vector<Vect3i>* nIndices = &mesh->normalsIndices;
	std::vector<Vect3f>* fNormals = &mesh->normals;

	std::vector<Vect3f>* vertices = &mesh->vertices;
	std::vector<Vect3f>* texels = &mesh->texels;
	std::vector<Vect3f>* normals = &mesh->normals;
	int numFaces = mesh->numFaces;

	Matrix4 mv =  (camera->viewMatrix * (*obj->getMatrix()));
	Matrix4 mvp = camera->projectionMatrix * mv;

	for (int i = 0; i < numFaces; i++)
	{
		Vect3f triPrim[3], normalPrim[3], uvPrim[3], tangentPrim[3];

		Vect3i v = (*vIndices)[i];
		Vect3i n = (*nIndices)[i];
		Vect3i t = (*tIndices)[i];

		packData(v, triPrim, *vertices);
		packData(n, normalPrim, *normals);
		packData(t, uvPrim, *texels);

		for (int j = 0; j < 3; j++)
		{
			triPrim[j] = mvp.mul(triPrim[j]);
			triPrim[j].x /= triPrim[j].w;
			triPrim[j].y /= triPrim[j].w;
			triPrim[j].z /= triPrim[j].w;
		}
		rasterize(triPrim);
	}
	return;
}


void Renderer::rasterize(Vect3f* v)
{
	float area;
	Vect3f hw{ 1.0f / v[0].w, 1.0f / v[1].w, 1.0f / v[2].w };
	float depth;
	Vect3f e, e_row;
	toViewport(zBuff, v);
	
	area = edge(v[0], v[1], v[2]);
	if (area <= 0.0f)
		return;
	area = 1.0f / area;
	
	int xmin;
	int xmax;
	int ymin;
	int ymax;

	toBoundBox(zBuff, v, xmin, xmax, ymin, ymax);
	
	Vect3f zVals(v[0].z, v[1].z, v[2].z);

	float a01 = v[0].y - v[1].y, b01 = v[1].x - v[0].x;
	float a12 = v[1].y - v[2].y, b12 = v[2].x - v[1].x;
	float a20 = v[2].y - v[0].y, b20 = v[0].x - v[2].x;

	Vect3f point(((float)(xmin)), ((float)(ymin)), 0.0f);

	e_row.x = edge(v[1], v[2], point);
	e_row.y = edge(v[2], v[0], point);
	e_row.z = edge(v[0], v[1], point);
	
	for (int y = ymin; y < ymax; y++)
	{
		e.x = e_row.x;
		e.y = e_row.y;
		e.z = e_row.z;

		for (int x = xmin; x < xmax; x++)
		{
			if ((e.x >= 0.0f) && (e.y >= 0.0f) && (e.z >= 0.0f))
			{
				depth = (e * area).dot(zVals);
				if ((depth > (*zBuff)(y, x)) && (depth <= 1.0f))
				{
					(*zBuff)(y, x) = depth;
				}
			}
			e.x += a12;
			e.y += a20;
			e.z += a01;
		}
		e_row.x += b12;
		e_row.y += b20;
		e_row.z += b01;

	}
	
	
}