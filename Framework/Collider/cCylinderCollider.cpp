#include "stdafx.h"
#include "cCylinderCollider.h"
#include "./Mesh/MeshUtility.h"
#include "./Graphic/ConstBuffer/cColliderBuffer.h"
#include "./Transform/sTransform.h"
#include "./Helper/cMath.h"

cCylinderCollider::cCylinderCollider(weak_ptr<sTransform> boneTransform,
									 D3DXVECTOR3 center,
									 float height, float radius)
	: cCollider(boneTransform, center)
	, _height(height)
	, _radius(radius)
{
	CreateVertices();
	CreateIndices();

	cMeshUtility<VertexC>::CreateVertexBuffer(&_mesh.VertexBuffer, _mesh.Vertices);

	cMeshUtility<VertexC>::CreateIndexBuffer(&_mesh.IndexBuffer, _mesh.Indices);

	_shader = cShader::Create(Shader + L"002_Collider.hlsl");
	_cbuffer = make_unique<cColliderBuffer>();
}

cCylinderCollider::~cCylinderCollider()
{
	SAFE_RELEASE(_mesh.VertexBuffer);
	SAFE_RELEASE(_mesh.IndexBuffer);
}

void cCylinderCollider::Update()
{
	cCollider::Update();
}

void cCylinderCollider::Render()
{
	cCollider::Render();

	cMeshUtility<VertexC>::Render(_mesh, D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
}

ContainmentType cCylinderCollider::ContainsRay(D3DXVECTOR3 position, D3DXVECTOR3 direction)
{
	return ContainmentType();
}

ContainmentType cCylinderCollider::ContainsPlane(D3DXVECTOR3 normal, float d)
{
	return ContainmentType();
}

ContainmentType cCylinderCollider::ContainsDot(D3DXVECTOR3 point)
{
	return ContainmentType();
}

ContainmentType cCylinderCollider::ContainsSphere(D3DXVECTOR3 center, float _radius)
{
	return ContainmentType();
}

ContainmentType cCylinderCollider::ContainsBox(D3DXVECTOR3 max, D3DXVECTOR3 min)
{
	return ContainmentType();
}

bool cCylinderCollider::IntersectsWithRay(D3DXVECTOR3 position, D3DXVECTOR3 direction)
{
	return false;
}

PlaneIntersectionType cCylinderCollider::IntersectsWithPlane(D3DXVECTOR3 normal, float d)
{
	return PlaneIntersectionType();
}

bool cCylinderCollider::IntersectsWithDot(D3DXVECTOR3 point)
{
	return false;
}

bool cCylinderCollider::IntersectsWithSphere(D3DXVECTOR3 center, float _radius)
{
	return false;
}

bool cCylinderCollider::IntersectsWithBox(D3DXVECTOR3 min, D3DXVECTOR3 max)
{
	return false;
}

void cCylinderCollider::CreateVertices()
{
	VertexC vertex;
	vertex.color = D3DXCOLOR{ 1, 1, 0, 1 };

	float rotY = 0;
	//원 2개
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 32; j++)
		{
			rotY = PI * 2 / 32 * j;
			if (i == 0)
				vertex.position = D3DXVECTOR3(sinf(rotY) * _radius, _height / 2, cosf(rotY) * _radius);
			else
				vertex.position = D3DXVECTOR3(sinf(rotY) * _radius, -_height / 2, cosf(rotY) * _radius);

			_mesh.Vertices.push_back(vertex);
		}
	}

	//반원 4개
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 32; j++)
		{
			if (j >= 17) break;

			rotY = PI * 2 / 32 * j;

			switch (i)
			{
				case 0:
					vertex.position = D3DXVECTOR3(0, cosf(rotY - PI / 2) / 2 + _radius, sinf(rotY - PI / 2) / 2);
					break;
				case 1:
					vertex.position = D3DXVECTOR3(sinf(rotY - PI / 2) / 2, cosf(rotY - PI / 2) / 2 + _radius, 0);
					break;
				case 2:
					vertex.position = D3DXVECTOR3(0, cosf(rotY + PI / 2) / 2 - _radius, sinf(rotY + PI / 2) / 2);
					break;
				case 3:
					vertex.position = D3DXVECTOR3(sinf(rotY + PI / 2) / 2, cosf(rotY + PI / 2) / 2 - _radius, 0);
					break;
			}

			_mesh.Vertices.push_back(vertex);
		}
	}

	//선 4개
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			if (j == 0) rotY = _height / 2;
			else rotY = -_height / 2;
			switch (i)
			{
				case 0: vertex.position = D3DXVECTOR3(0, rotY, _radius); break;
				case 1: vertex.position = D3DXVECTOR3(_radius, rotY, 0); break;
				case 2: vertex.position = D3DXVECTOR3(0, rotY, -_radius); break;
				case 3: vertex.position = D3DXVECTOR3(-_radius, rotY, 0); break;
			}
			_mesh.Vertices.push_back(vertex);
		}
	}
}

void cCylinderCollider::CreateIndices()
{
	int start = 0;
	int index = 64;
	//원 2개
	for (int i = start; i < index; i++, start++)
	{
		if (i % 32 == 31)
		{
			_mesh.Indices.push_back(i);
			_mesh.Indices.push_back(i - 31);
		}
		else
		{
			_mesh.Indices.push_back(i);
			_mesh.Indices.push_back(i + 1);
		}
	}

	//반원 4개
	index += 17 * 4;
	for (int i = start, j = 0; i < index; i++, j++, start++)
	{
		if (j % 17 == 16) continue;

		_mesh.Indices.push_back(i);
		_mesh.Indices.push_back(i + 1);
	}

	//선 4개
	index += 8;
	for (int i = start; i < index; i++)
		_mesh.Indices.push_back(i);
}

bool cCylinderCollider::IntersectsWithCylinder(sLine line, float radius)
{
	float dist2 = ClosestPtSegmentSegment(line);

	
	float radius2 = _radius * _scale.x + radius;

	return dist2 <= radius2 * radius2;
}

sLine cCylinderCollider::GetTransformedLine() const
{
	sLine line;
	line.start = { 0, _height * 0.5f, 0 };
	line.end = { 0, -_height * 0.5f, 0 };

	D3DXMATRIX world;
	D3DXVec3TransformCoord(&line.start, &line.start, &_world);
	D3DXVec3TransformCoord(&line.end, &line.end, &_world);

	return line;
}

float cCylinderCollider::ClosestPtSegmentSegment(sLine line2)
{
	auto line1 = GetTransformedLine();
	D3DXVECTOR3 d1 = line2.start - line1.start;
	D3DXVECTOR3 d2 = line2.end - line1.end;
	D3DXVECTOR3 r = line1.start - line1.end;
	float a = D3DXVec3Dot(&d1, &d1);
	float e = D3DXVec3Dot(&d2, &d2);
	float f = D3DXVec3Dot(&d2, &r);

	float s, t;
	D3DXVECTOR3 c1, c2;
	if (a <= 0.001f && e <= 0.001f)
	{
		s = t = 0.0f;
		c1 = line1.start;
		c2 = line1.end;
		return D3DXVec3Dot(&(c1 - c2), &(c1 - c2));
	}

	if (a <= 0.001f)
	{
		s = 0.0f;
		t = f / e;
		return cMath::Clamp(t, 0.0f, 1.0f);
	}
	else
	{
		float c = D3DXVec3Dot(&d1, &r);

		if (e <= 0.001f)
		{
			t = 0.0f;
			s = cMath::Clamp(-c / a, 0.0f, 1.0f);
		}
		else
		{
			float b = D3DXVec3Dot(&d1, &d2);
			float denom = a*e - b*b;

			if (denom != 0.0f)
			{
				s = cMath::Clamp((b*f - c*e) / denom, 0.0f, 1.0f);
			}
			else s = 0.0f;

			t = (b*s + f) / e;

			if (t < 0.0f)
			{
				t = 0.0f;
				s = cMath::Clamp(-c / a, 0.0f, 1.0f);
			}
			else if (t > 1.0f)
			{
				t = 1.0f;
				s = cMath::Clamp((b - c) / a, 0.0f, 1.0f);
			}
		}
	}

	c1 = line1.start + d1 * s;
	c2 = line1.end + d2 * t;
	return D3DXVec3Dot(&(c1 - c2), &(c1 - c2));
}