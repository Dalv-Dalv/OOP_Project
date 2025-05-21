#pragma once
#include "OreInfo.h"
#include "../../EventSystem/ActionEvent.h"

class IOreContainer {
public:
	virtual ~IOreContainer() {};
	virtual void AddOres(OreInfo& ores) = 0;

	virtual OreInfo GetContents() const = 0;
};
