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
	grid.npc = 4;
	grid.calculateAllJakobians();
	grid.calculateAllShapeDerivatives();
	grid.calculateAllMatrixH();
}


int main() {

	// LAB 1 - READING GRID FROM FILE //
	//---------------------------------------------------------------------//
	// LAB 2 - GAUSS QUADRATURES METHOD
	//---------------------------------------------------------------------//
	// LAB 3 - JAKOBIAN / LAB 4, 5 - MATRIX H
	//---------------------------------------------------------------------//
	// LAB 6 - testing data from file
	//---------------------------------------------------------------------//
	// LAB7 - MATRIX H AGGREGATION
	//---------------------------------------------------------------------//
	// LAB 8 - BOUNDARY CONDITIONS
	//---------------------------------------------------------------------//
	// LAB 9 - CALCULATING AND AGREGATING VECTOP P
	//---------------------------------------------------------------------//
	// LAB 10 - calculating matrix C


	// grid 4x4 test
	std::cout << "GRID 4X4 TEST:" << std::endl;
	Grid grid4x4 = readFromFile("data\\Test1_4_4.txt");
	grid4x4.npc = 4;

	calculateH(grid4x4);

	GlobalSystemOfEquation SOE;
	aggregate(grid4x4, SOE);
	std::cout << "\nGLOBAL MATRIX C" << std::endl;
	SOE.displayMatrixC();
	std::cout << "\nGLOBAL VECTOR P" << std::endl;
	SOE.displayVectorP();
	std::cout << "\nGLOBAL MATRIX H" << std::endl;
	SOE.displayMatrixH();

	// solving system of equation
	SOE.solve(grid4x4);

	// mix grid 4x4 test
	std::cout << "MIX GRID 4X4 TEST:" << std::endl;
	Grid mix_grid = readFromFile("data\\Test2_4_4_MixGrid.txt");
	mix_grid.npc = 4;

	calculateH(mix_grid);

	GlobalSystemOfEquation SOE2;
	aggregate(mix_grid, SOE2);
	std::cout << "\nGLOBAL MATRIX C" << std::endl;
	SOE2.displayMatrixC();
	std::cout << "\nGLOBAL VECTOR P" << std::endl;
	SOE2.displayVectorP();
	std::cout << "\nGLOBAL MATRIX H" << std::endl;
	SOE2.displayMatrixH();

	// solving system of equation
	SOE2.solve(mix_grid);

	return 0;
}
