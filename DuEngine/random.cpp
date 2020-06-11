#include <chrono>
#include <random>
#include "random.hpp"

namespace Random{
	static std::mt19937 rangen((unsigned long int)std::chrono::high_resolution_clock::now().time_since_epoch().count());
	uint_fast32_t get(){
		return rangen();
	}
	float getf(){
		constexpr uint_fast32_t max = rangen.max();
		return (float)rangen()/max;
	}
}