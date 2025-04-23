#pragma once
#include "TriangleList.h"
#include <DirectXMath.h>
#include <initializer_list>
namespace dx = DirectX;

class Cube {
public:
	template<class T>
	static TriangleList<T> make() {
		constexpr float side = .5f;

		std::vector<T> vertices(8);
		vertices[0].pos = { -side,-side,-side };
		vertices[1].pos = { side,-side,-side };
		vertices[2].pos = { -side,side,-side };
		vertices[3].pos = { side,side,-side };
		vertices[4].pos = { -side,-side,side };
		vertices[5].pos = { side,-side,side };
		vertices[6].pos = { -side,side,side };
		vertices[7].pos = { side,side,side };
		return {
			std::move(vertices),
			{
				0,2,1, 2,3,1,
				1,3,5, 3,7,5,
				2,6,3, 3,6,7,
				4,5,7, 4,7,6,
				0,4,2, 2,4,6,
				0,1,4, 1,5,4
			}
		};
	}


	//We need more vertices, because of the fact that if we have an image (texture) of an unwrapped cube, and we fold it, 
	//then three vertices with different textures will be in the same place in terms of coordinates, 
	//but they still need to have different numbers.
	/*		
	 		3##4
			#  #
			#  #
		1###	####|#### <- If this cube is folded then vertices 1 and 3 will meet, but their respective walls may have different colors 
		#			    #
		2###    ####|####
			#  #
			#  #
			5##6
	*/
	template<class T>
	static TriangleList<T> makeTextured() {
		namespace dx = DirectX;

		constexpr float side = .5f;
		
		std::vector<T> vertices(14);

		vertices[0].pos = { -side,-side,-side };
		vertices[0].tex = { 2.0f / 3.0f,0.0f };
		vertices[1].pos = { side,-side,-side };
		vertices[1].tex = { 1.0f / 3.0f,0.0f};
		vertices[2].pos = { -side,side,-side };
		vertices[2].tex = { 2.0f / 3.0f,.25f};
		vertices[3].pos = { side,side,-side };
		vertices[3].tex = { 1.0f / 3.0f,.25f };
		vertices[4].pos = { -side,-side,side };
		vertices[4].tex = { 2.0f / 3.0f,.75f};
		vertices[5].pos = { side,-side,side };
		vertices[5].tex = { 1.0f / 3.0f,.75f};
		vertices[6].pos = { -side,side,side };
		vertices[6].tex = { 2.0f / 3.0f, .5f };
		vertices[7].pos = { side,side,side };
		vertices[7].tex = { 1.0f / 3.0f, .5f};
		vertices[8].pos = { -side,-side,-side };
		vertices[8].tex = { 2.0f / 3.0f, 1.0f };
		vertices[9].pos = { side,-side,-side };
		vertices[9].tex = { 1.0f / 3.0f, 1.0f };
		vertices[10].pos = { -side,-side,-side };
		vertices[10].tex = { 1.0f, .25f };
		vertices[11].pos = { -side,-side,side };
		vertices[11].tex = { 1.0f, .5f };
		vertices[12].pos = { side,-side,-side };
		vertices[12].tex = { 0.0f, .25f };
		vertices[13].pos = { side,-side,side };
		vertices[13].tex = { 0.0f, .5f };

		return {
			std::move(vertices),{
				0,2,1,   2,3,1,
				4,8,5,   5,8,9,
				2,6,3,   3,6,7,
				4,5,7,   4,7,6,
				2,10,11, 2,11,6,
				12,3,7,  12,7,13
			}
		};
	}
};