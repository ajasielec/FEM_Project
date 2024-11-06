#pragma once

// all includes
#include "includes.h"
#include "grid.h"
#include "ElemUniv.h"
#include "Jakobian.h"

// functions
Grid readFromFile(const std::string& path);
void calculateH(Grid& grid);

