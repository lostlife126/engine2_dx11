#include "renderer.h"
#include <cmath>

void packData(Vect3i& index, Vect3f* prim, std::vector<Vect3f>& vals)
{
	prim[0] = vals[index.data[0]];
	prim[1] = vals[index.data[1]];
	prim[2] = vals[index.data[2]];
}

void Renderer::drawObject(Object* obj)
{
	Mesh* mesh = obj->getMesh();
	Texture* tex = obj->getTexture();
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
	std::cout << "Rasterize: " << mesh->caption << " numFaces = " << numFaces << std::endl;

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
		}
		for (int j = 0; j < 3; j++)
		{
			float wr = 1.0f / triPrim[j].w;
			triPrim[j].x *= wr;
			triPrim[j].y *= wr;
			triPrim[j].z *= wr;
		}
		rasterize(triPrim, uvPrim, tex);
	}
	return;
}

void Renderer::rasterize(Vect3f* v, Vect3f* uv, Texture* texture)
{
	float area;
	Vect3f hw{ 1.0f / v[0].w, 1.0f / v[1].w, 1.0f / v[2].w };
	float depth;
	Vect3f e, e_row;

	// to viewport
	for (int i = 0; i < 3; i++)
	{
		v[i].x = (v[i].x + 1.0f) * 0.5f * zBuff->width + 0.5f;
		v[i].y = (v[i].y + 1.0f) * 0.5f * zBuff->height + 0.5f;
	}
	
	//area = edge(v[0], v[1], v[2]);
	area = edge(v[1], v[0], v[2]);
	if (area <= 0.0f)
		return;
	area = 1.0f / area;
	//if ((v[0].w > 2.0f) || (v[1].w > 2.0f) || (v[2].w > 2.0f))
	//	return;

	// to bound box
	int xmin, xmax, ymin, ymax;
	xmin = floor(min(v[0].x, v[1].x, v[2].x));
	xmax = ceil(max(v[0].x, v[1].x, v[2].x));
	ymin = floor(min(v[0].y, v[1].y, v[2].y));
	ymax = ceil(max(v[0].y, v[1].y, v[2].y));
	xmin = max(xmin, 0);
	xmax = min(xmax, zBuff->width - 1);
	ymin = max(ymin, 0);
	ymax = min(ymax, zBuff->height - 1);
	
	Vect3f zVals(v[0].z, v[1].z, v[2].z);

	//float a01 = v[0].y - v[1].y, b01 = v[1].x - v[0].x;
	//float a12 = v[1].y - v[2].y, b12 = v[2].x - v[1].x;
	//float a20 = v[2].y - v[0].y, b20 = v[0].x - v[2].x;

	float a01 = v[1].y - v[0].y, b01 = v[0].x - v[1].x;
	float a12 = v[2].y - v[1].y, b12 = v[1].x - v[2].x;
	float a20 = v[0].y - v[2].y, b20 = v[2].x - v[0].x;

	Vect3f point(((float)(xmin)), ((float)(ymin)), 0.0f);

	//e_row.x = edge(v[1], v[2], point);
	//e_row.y = edge(v[2], v[0], point);
	//e_row.z = edge(v[0], v[1], point);

	e_row.x = edge(v[1], point, v[2]);
	e_row.y = edge(v[2], point, v[0]);
	e_row.z = edge(v[0], point, v[1]);
	
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
					Vect3f uvect(uv[0].x, uv[1].x, uv[2].x);
					Vect3f vvect(uv[0].y, uv[1].y, uv[2].y);
					float u_ = (e * area).dot(uvect);
					float v_ = (e * area).dot(vvect);
					(*pixBuff)(y, x) = texture->getPixel(u_, v_);
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