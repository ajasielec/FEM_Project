#include "main.h"

// function that reads from desired file
Grid readFromFile(const std::string& path) {

	Grid grid;
	std::ifstream myfile("" + path);

	if (!myfile.good()) {
		std::cerr << "Cannot open a file." << std::endl;
		return grid;
	}

	std::string line;

	// reading simulation data
	while (std::getline(myfile, line)) {
		std::istringstream iss(line);	//creates object from line
		std::string key;
		int value;

		if (iss >> key >> value) {
			if (key == "SimulationTime" || key == "SimulationStepTime" || key == "Conductivity" ||
				key == "Alfa" || key == "Tot" || key == "InitialTemp" || key == "Density" || key == "SpecificHeat") {
				grid.globalData[key] = value;
			}
		}

		if (key == "SpecificHeat") {
			break;
		}
	}

	// read number of nodes and elements
	while (std::getline(myfile, line)) {
		std::istringstream iss(line);
		std::string key;
		std::string key2;
		int value;

		if (iss >> key >> key2 >> value) {
			if (key == "Nodes") {
				grid.nodes_number = value;
			}
			else if (key == "Elements") {
				grid.elements_number = value;
			}
		}

		if (line == "*Node") {
			break;
		}
	}

	// reading nodes
	while (std::getline(myfile, line) && line != "*Element") {
		std::istringstream iss(line);
		Node node;
		if (char comma; iss >> node.id >> comma >> node.x >> comma >> node.y) {
			grid.nodes.push_back(node);
		}
		if (node.id == grid.nodes_number) {
			break;
		}
	}

	// reading elements
	while (std::getline(myfile, line) && line != "*BC") {
		std::istringstream iss(line);
		Element elem{};
		char comma;
		if (iss >> elem.id >> comma >> elem.node_id[0] >> comma >> elem.node_id[1] >> comma >> elem.node_id[2] >> comma >> elem.node_id[3]) {
			grid.elements.push_back(elem);
		}
	}

	// reading BC to set
	std::set<int> bcNodes;
	while (std::getline(myfile, line)) {
		std::istringstream iss(line);
		int nodeId;
		while (iss >> nodeId) {
			bcNodes.insert(nodeId);
			if(iss.peek() == ',') iss.ignore();
		}
	}

	// set the BC flag for nodes and start temperature
	for (auto& node : grid.nodes) {
		if (bcNodes.find(node.id) != bcNodes.end()) {
			node.BC = true;
		} else {
			node.BC = false;
		}

		node.temp = grid.globalData["InitialTemp"];
	}

	return grid;
}

void calculateH(Grid &grid) {
	grid.calculateAllJakobians();
	grid.calculateAllShapeDerivatives();
	grid.calculateAllMatrixH();
}


int main() {
	//------------------- TEST 1: grid 4x4 ---------------------//
	std::cout << "\nGRID 4X4 TEST:" << std::endl;
	Grid grid4x4 = readFromFile("data\\Test1_4_4.txt");
	grid4x4.npc = 16;

	calculateH(grid4x4);

	GlobalSystemOfEquation SOE;
	aggregate(grid4x4, SOE);

	// solving system of equation
	SOE.solve(grid4x4);

	//------------------- TEST 2: mix grid 4x4 ---------------------//
	std::cout << "\nMIX GRID 4X4 TEST:" << std::endl;
	Grid mix_grid = readFromFile("data\\Test2_4_4_MixGrid.txt");
	mix_grid.npc = 16;

	calculateH(mix_grid);

	GlobalSystemOfEquation SOE2;
	aggregate(mix_grid, SOE2);

	// solving system of equation
	SOE2.solve(mix_grid);

	//------------------- TEST 3: grid 31x31 ---------------------//
	std::cout << "\nGRID 31X31 TEST:" << std::endl;
	Grid grid_31x31 = readFromFile("data\\Test3_31_31_kwadrat.txt");
	grid_31x31.npc = 16;

	calculateH(grid_31x31);

	GlobalSystemOfEquation SOE3;
	aggregate(grid_31x31, SOE3);

	// solving system of equation
	SOE3.solve(grid_31x31);


	return 0;
}
