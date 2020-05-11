#pragma once
#ifndef _MAIN
#include "colision.cpp"
#endif
namespace phy{
	const double gravity = 0.020,
				 jumpVel = 0.30,
				 moveVel = 0.075;
}

int colide(const vertex_with_text &a, const vertex_with_text&b){
	//const position &aPos = a.pos;
	int has_colided = 0;
	for(long unsigned int nQuada= 0; nQuada < a.cords.size()/(4*4); ++nQuada){
		const double &aX = a.cords[nQuada * 4 * 4] + a.pos.x,
					 &aY = a.cords[nQuada * 4 * 4 + 1] + a.pos.y,
					 &aWidth = a.cords[nQuada * 4 * 4 + 4 * 1],
					 &aHeight = a.cords[nQuada * 4 * 4 + 4 * 3 + 1];
		for(long unsigned int nQuad= 0; nQuad < b.cords.size()/(4*4); ++nQuad){
			const double &bX = b.cords[nQuad * 4 * 4] + b.pos.x,
						&bY = b.cords[nQuad * 4 * 4 + 1] + b.pos.y,
						&bWidth = b.cords[nQuad * 4 * 4 + 4 * 1],
						&bHeight = b.cords[nQuad * 4 * 4 + 4 * 3 + 1];
			if( //Test if there isn't a gap betwin any the two quads
				aX < bX + (bWidth - bX) &&
				aX + aWidth > bX &&
				aY < bY + (bHeight - bY) &&
				aY + aHeight > bY){
					has_colided |= 1;
					if(aX < bX + (bWidth - bX) && (aX + aWidth > bX)) has_colided |= 0b10;
			}
		}
	}
	return has_colided;
}

std::vector<position> colision_push_vec_result;
void colision_push_vec(const vertex_with_text &a, const vertex_with_text&b){
	for(long unsigned int nQuada= 0; nQuada < a.cords.size()/(4*4); ++nQuada){
		const double &aX = a.cords[nQuada * 4 * 4] + a.pos.x,
					 &aY = a.cords[nQuada * 4 * 4 + 1] + a.pos.y,
					 &aWidth = a.cords[nQuada * 4 * 4 + 4 * 1],
					 &aHeight = a.cords[nQuada * 4 * 4 + 4 * 3 + 1];
		for(long unsigned int nQuad= 0; nQuad < b.cords.size()/(4*4); ++nQuad){
			const double &bX = b.cords[nQuad * 4 * 4] + b.pos.x,
						&bY = b.cords[nQuad * 4 * 4 + 1] + b.pos.y,
						&bWidth = b.cords[nQuad * 4 * 4 + 4 * 1] - bX + b.pos.x,
						&bHeight = b.cords[nQuad * 4 * 4 + 4 * 3 + 1] - bY + b.pos.y,

						overlap_up = bY + bHeight - aY,		//+y
						overlap_dw = bY - (aY + aHeight),	//-y
						overlap_lf = bX - (aX + aWidth),	//-x
						overlap_rt = bX + bWidth - aX;		//+x
			if( aX < bX + bWidth &&	//left
				aX + aWidth > bX &&	//right
				aY < bY + bHeight &&//up
				aY + aHeight > bY){	//down
				const double retx = abs(overlap_rt) > abs(overlap_lf) ? overlap_lf : overlap_rt, rety = abs(overlap_up) > abs(overlap_dw) ? overlap_dw : overlap_up;
				colision_push_vec_result.push_back(abs(rety) > abs(retx) ? position(retx * 1.01, 0.0) : position(0.0, rety * 1.01));
			}
		}
	}
}

extern std::vector<vertex_with_text *> colision_static;
extern player &pers;

int colide_all(){//if quad colide 0b01 if colide with the foor 0b1x
	int out = 0;
	for(auto x : colision_static)
		out |= colide(pers.getElement(), *x);
	return out;
}
position push_colision_calc(){
	position ret;
	const vertex_with_text &persElement = pers.getElement();
	colision_push_vec_result.clear();
	for(auto x : colision_static)
		colision_push_vec(persElement, *x);
	for(auto r : colision_push_vec_result){
		ret.x = abs(ret.x) > abs(r.x) ? ret.x : r.x;
		ret.y = abs(ret.y) > abs(r.y) ? ret.y : r.y;
	}
	return ret;
}
void physics(){
	static int has_colided = 0;
	has_colided = colide_all();
	if(has_colided){//walk colision
		//if(has_colided && 0b10)
			//pers.onhorizontalColision();
		pers.onColision();
		//std::cout << "Walk colision" << std::endl;
	}else pers.noColision();

	pers.apply_jump();
	has_colided = colide_all();
	if (has_colided && 0b10) {  //gravity colision
		//if (has_colided && 0b10)
			pers.onhorizontalColision();
		pers.onColision();
		//std::cout << "Gravity colision" << std::endl;
	}else{
		pers.gravity();
		pers.noColision();
	}
	static position push_colision;
	has_colided = 0;
	push_colision = push_colision_calc();
	if(push_colision.x != 0.0 || push_colision.y != 0.0){//normal colision
		pers.move(push_colision);
		if(push_colision.y > 0.0)
			pers.onhorizontalColision();
		has_colided = 1;
		//std::cout << "Normal colision" << std::endl;
	}
	/*if(!has_colided)*/ pers.noColision();
}