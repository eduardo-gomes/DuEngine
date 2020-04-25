#pragma once
#ifndef _MAIN
#include "colision.cpp"
#endif
int colide(const vector_quad_text &a, const vector_quad_text&b){
	const position &bPos = a.pos, &aPos = b.pos;
	const double &bWidth = a.vector[3*4],
				 &aWidth = b.vector[3*4],
				 &bHeight = a.vector[1*4 + 1],
				 &aHeight = b.vector[1*4 + 1];
	if(
		aPos.x < bPos.x + bWidth &&
		aPos.x + aWidth > bPos.x &&
		aPos.y < bPos.y + bHeight &&
		aPos.y + aHeight > bPos.y){
		if (aPos.x < bPos.x + bWidth)
			return 0b11;
		else
			return 0b01;
	}
	return 0;
}
int colide(const vector_quad_text &a, const vector_with_text&b){
	const position &aPos = a.pos;
	const double &aWidth = a.vector[3*4],
				 &aHeight = a.vector[1 * 4 + 1];
	int has_colided = 0;
	for(long unsigned int nQuad= 0; nQuad < b.cords.size()/(4*4); ++nQuad){
		const double &bX = b.cords[nQuad * 4 * 4],
					 &bY = b.cords[nQuad * 4 * 4 + 1],
					 &bWidth = b.cords[nQuad * 4 * 4 + 4 * 3],
					 &bHeight = b.cords[nQuad * 4 * 4 + 4 * 1 + 1];
		if( //Test if there isn't a gap betwin any the two quads
			aPos.x < bX + (bWidth - bX) &&
			aPos.x + aWidth > bX &&
			aPos.y < bY + (bHeight - bY) &&
			aPos.y + aHeight > bY){
				has_colided |= 1;
				if(aPos.x < bX + (bWidth - bX)) has_colided |= 0b11;
		}
	}
	return has_colided;
}
position colision_push(const vector_quad_text &a, const vector_with_text&b){
	const position &aPos = a.pos;
	const double &aWidth = a.vector[3*4],
				 &aHeight = a.vector[1 * 4 + 1];
	for(long unsigned int nQuad= 0; nQuad < b.cords.size()/(4*4); ++nQuad){
		const double &bX = b.cords[nQuad * 4 * 4],
					 &bY = b.cords[nQuad * 4 * 4 + 1],
					 &bWidth = b.cords[nQuad * 4 * 4 + 4 * 3],
					 &bHeight = b.cords[nQuad * 4 * 4 + 4 * 1 + 1],

					 overlap_up = bY + (bHeight - bY) - aPos.y,	//+y
					 overlap_dw = bY - (aPos.y + aHeight),		//-y
					 overlap_lf = bX - (aPos.x + aWidth),		//-x
					 overlap_rt = bX + (bWidth - bX) - aPos.x;	//+x
		if( aPos.x < bX + (bWidth - bX) && 	//left
			aPos.x + aWidth > bX &&			//right
			aPos.y < bY + (bHeight - bY) &&	//up
			aPos.y + aHeight > bY){			//down
			if(overlap_up > 0.0) return {0.0, overlap_up};
			if(overlap_dw < 0.0) return {0.0, overlap_dw};
			if(overlap_rt > 0.0) return {overlap_rt, 0.0};
			if(overlap_lf < 0.0) return {overlap_lf, 0.0};
		}
	}
	return {0.0, 0.0};
}
position colision_push(const vector_quad_text &a, const vector_quad_text &b) {
	const position &bPos = b.pos, &aPos = a.pos;
	const double &bWidth = b.vector[3*4],
				 &aWidth = a.vector[3*4],
				 &bHeight = b.vector[1*4 + 1],
				 &aHeight = a.vector[1*4 + 1],

				 overlap_up = bPos.y + bHeight - aPos.y,	//+y
				 overlap_dw = bPos.y - (aPos.y + aHeight),	//-y
				 overlap_lf = bPos.x - (aPos.x + aWidth),	//-x
				 overlap_rt = bPos.x + bWidth - aPos.x;		//+x
	if(
		aPos.x < bPos.x + bWidth &&
		aPos.x + aWidth > bPos.x &&
		aPos.y < bPos.y + bHeight &&
		aPos.y + aHeight > bPos.y){
			const double retx = abs(overlap_rt) > abs(overlap_lf) ? overlap_lf : overlap_rt, rety = abs(overlap_up) > abs(overlap_dw) ? overlap_dw : overlap_up;
			return abs(rety) > abs(retx) ? position(retx * 1.01, 0.0) : position(0.0, rety * 1.01);
	}
	return {0.0, 0.0};
}
std::vector<position> colision_push_vec_result;
void colision_push_vec(const vector_quad_text &a, const vector_with_text&b){
	colision_push_vec_result.clear();
	const position &aPos = a.pos;
	const double &aWidth = a.vector[3*4],
				 &aHeight = a.vector[1 * 4 + 1];
	for(long unsigned int nQuad= 0; nQuad < b.cords.size()/(4*4); ++nQuad){
		const double &bX = b.cords[nQuad * 4 * 4],
					 &bY = b.cords[nQuad * 4 * 4 + 1],
					 &bWidth = b.cords[nQuad * 4 * 4 + 4 * 3],
					 &bHeight = b.cords[nQuad * 4 * 4 + 4 * 1 + 1],

					 overlap_up = bY + (bHeight - bY) - aPos.y,	//+y
					 overlap_dw = bY - (aPos.y + aHeight),		//-y
					 overlap_lf = bX - (aPos.x + aWidth),		//-x
					 overlap_rt = bX + (bWidth - bX) - aPos.x;	//+x
		if( aPos.x < bX + (bWidth - bX) && 	//left
			aPos.x + aWidth > bX &&			//right
			aPos.y < bY + (bHeight - bY) &&	//up
			aPos.y + aHeight > bY){			//down
			/*if(overlap_up > 0.0) return {0.0, overlap_up};
			if(overlap_dw < 0.0) return {0.0, overlap_dw};
			if(overlap_rt > 0.0) return {overlap_rt, 0.0};
			if(overlap_lf < 0.0) return {overlap_lf, 0.0};*/
			const double retx = abs(overlap_rt) > abs(overlap_lf) ? overlap_lf : overlap_rt, rety = abs(overlap_up) > abs(overlap_dw) ? overlap_dw : overlap_up;
			colision_push_vec_result.push_back(abs(rety) > abs(retx) ? position(retx * 1.01, 0.0) : position(0.0, rety * 1.01));
		}
	}
}
void colision_push_vec(const vector_quad_text &a, const vector_quad_text &b) {
	const position &bPos = b.pos, &aPos = a.pos;
	const double &bWidth = b.vector[3*4],
				 &aWidth = a.vector[3*4],
				 &bHeight = b.vector[1*4 + 1],
				 &aHeight = a.vector[1*4 + 1],

				 overlap_up = bPos.y + bHeight - aPos.y,	//+y
				 overlap_dw = bPos.y - (aPos.y + aHeight),	//-y
				 overlap_lf = bPos.x - (aPos.x + aWidth),	//-x
				 overlap_rt = bPos.x + bWidth - aPos.x;		//+x
	if(
		aPos.x < bPos.x + bWidth &&
		aPos.x + aWidth > bPos.x &&
		aPos.y < bPos.y + bHeight &&
		aPos.y + aHeight > bPos.y){
			const double retx = abs(overlap_rt) > abs(overlap_lf) ? overlap_lf : overlap_rt, rety = abs(overlap_up) > abs(overlap_dw) ? overlap_dw : overlap_up;
			colision_push_vec_result.push_back(abs(rety) > abs(retx) ? position(retx * 1.01, 0.0) : position(0.0, rety * 1.01));
	}
}

extern std::vector<vector_with_text *> colision_static;
extern std::vector<vector_quad_text *> colision_static_quad;
extern entidade pers;
const vector_quad_text &persElement = pers.getElement();


int colide_all(){
	int out = 0;
	for(auto x : colision_static)
		out |= colide(persElement, *x);
	for(auto x : colision_static_quad)
		out |= colide(persElement, *x);
	return out;
}
position push_colision_calc(){
	position ret;
	for(auto x : colision_static)
		colision_push_vec(persElement, *x);
	for(auto x : colision_static_quad)
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
		if(has_colided && 0b10)
			pers.onhorizontalColision();
		pers.onColision();
	}else pers.noColision();

	pers.apply_jump();
	pers.gravity();
	has_colided = colide_all();
	if (has_colided) {  //gravity colision
		if (has_colided && 0b10)
			pers.onhorizontalColision();
		pers.onColision();
	} else pers.noColision();

	static position push_colision;
	has_colided = 0;
	/*push_colision = colision_push(pers.getElement(), quad);
	if(push_colision.x != 0.0 || push_colision.y != 0.0){//normal colision
		pers.move(push_colision);
		if(push_colision.y > 0.0)
			pers.onhorizontalColision();
		has_colided = 1;
	}*/
	push_colision = push_colision_calc();
	if(push_colision.x != 0.0 || push_colision.y != 0.0){//normal colision
		pers.move(push_colision);
		if(push_colision.y > 0.0)
			pers.onhorizontalColision();
		has_colided = 1;
	}
	/*if(!has_colided)*/ pers.noColision();
}