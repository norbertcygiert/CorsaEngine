#pragma once
#include "TriangleList.h"
#include <DirectXMath.h>
#include "Math.h"
namespace dx = DirectX;
class Sphere {
public:
	template<class T>
	static TriangleList<T> makeTesselated(int latDiv, int longDiv)
	{
		assert(latDiv >= 3);
		assert(longDiv >= 3);

		constexpr float radius = 1.0f;
		const auto base = dx::XMVectorSet(0.0f, 0.0f, radius, 0.0f);
		const float lattitudeAngle = PI / latDiv;
		const float longitudeAngle = 2.0f * PI / longDiv;

		std::vector<T> vertices;
		for (int iLat = 1; iLat < latDiv; iLat++) {
			const auto latBase = dx::XMVector3Transform( base, dx::XMMatrixRotationX(lattitudeAngle * iLat) );
			for (int iLong = 0; iLong < longDiv; iLong++) {
				vertices.emplace_back();
				auto v = dx::XMVector3Transform( latBase, dx::XMMatrixRotationZ(longitudeAngle * iLong) );
				dx::XMStoreFloat3(&vertices.back().pos, v);
			}
		}

		// Add the cap vertices
		const auto iNorthPole = (unsigned short)vertices.size();
		vertices.emplace_back();
		dx::XMStoreFloat3(&vertices.back().pos, base);
		const auto iSouthPole = (unsigned short)vertices.size();
		vertices.emplace_back();
		dx::XMStoreFloat3(&vertices.back().pos, dx::XMVectorNegate(base));

		const auto calculatedIndex = [latDiv, longDiv](unsigned short iLat, unsigned short iLong) { return iLat * longDiv + iLong; };
		std::vector<unsigned short> indices;
		for (unsigned short iLat = 0; iLat < latDiv - 2; iLat++) {
			for (unsigned short iLong = 0; iLong < longDiv - 1; iLong++) {
				indices.push_back(calculatedIndex(iLat, iLong));
				indices.push_back(calculatedIndex(iLat + 1, iLong));
				indices.push_back(calculatedIndex(iLat, iLong + 1));
				indices.push_back(calculatedIndex(iLat, iLong + 1));
				indices.push_back(calculatedIndex(iLat + 1, iLong));
				indices.push_back(calculatedIndex(iLat + 1, iLong + 1));
			}
			// wrap band
			indices.push_back(calculatedIndex(iLat, longDiv - 1));
			indices.push_back(calculatedIndex(iLat + 1, longDiv - 1));
			indices.push_back(calculatedIndex(iLat, 0));
			indices.push_back(calculatedIndex(iLat, 0));
			indices.push_back(calculatedIndex(iLat + 1, longDiv - 1));
			indices.push_back(calculatedIndex(iLat + 1, 0));
		}

		// cap fans
		for (unsigned short iLong = 0; iLong < longDiv - 1; iLong++)
		{
			// north
			indices.push_back(iNorthPole);
			indices.push_back(calculatedIndex(0, iLong));
			indices.push_back(calculatedIndex(0, iLong + 1));
			// south
			indices.push_back(calculatedIndex(latDiv - 2, iLong + 1));
			indices.push_back(calculatedIndex(latDiv - 2, iLong));
			indices.push_back(iSouthPole);
		}
		// Wrap the triangles
		// From north
		indices.push_back(iNorthPole);
		indices.push_back(calculatedIndex(0, longDiv - 1));
		indices.push_back(calculatedIndex(0, 0));
		// From south
		indices.push_back(calculatedIndex(latDiv - 2, 0));
		indices.push_back(calculatedIndex(latDiv - 2, longDiv - 1));
		indices.push_back(iSouthPole);

		return { std::move(vertices),std::move(indices) };
	}
	template<class T>
	static TriangleList<T> make() {
		return makeTesselated<T>(12, 24);
	}
};