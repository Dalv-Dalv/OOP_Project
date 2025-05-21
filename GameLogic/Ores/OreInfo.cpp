#include "OreInfo.h"

#include <iostream>

void OreInfo::AddOreFromMap(float oreType, float oreValue) {
	int i = round(oreType * 10);
	i--;
	if(i < 0 || i > 8) return;
	quantities[i] += oreValue;
}

float& OreInfo::operator[](int index) {
	return quantities[index];
}


