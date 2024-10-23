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

	// completing tables
	grid.npc = 4;
	ElemUniv elem_univ(grid.npc);
	// elem_univ.display();

	// assigning jakobian to all elements
	for (int i = 0; i < grid.elements_number; i++) {
		Jakobian result = calculateJakobian(i+1, grid, elem_univ);
		grid.elements[i].jakobian = result;
		grid.elements[i].jakobians = calculateJakobiansOfElement(i+1, grid, elem_univ);
	}

	return grid;
}

// function that can calculate all jakobians in one element
std::vector<Jakobian> calculateJakobiansOfElement(int element_id, Grid grid, ElemUniv e) {

	std::vector<Jakobian> result;

	// taking an element
	const Element element = grid.findElementById(element_id);

	// taking nodes of given element
	std::vector<Node> nodes (4);
	for (int i = 0; i < 4; i++) {
		nodes[i] = grid.findNodeById(element.node_id[i]);
	}

	//calculating jakobian for each npc
	for (int i = 0; i < 4; i++) {
		Jakobian jakobian;
		jakobian.calculateJakobian(e, nodes, i);
		jakobian.calculateInverse();
		result.push_back(jakobian);
	}

	return result;
}

Jakobian calculateJakobian(int element_id, Grid grid, ElemUniv e) {
	Jakobian result;

	// taking an element
	const Element element = grid.findElementById(element_id);

	// taking nodes of given element
	std::vector<Node> nodes (4);
	for (int i = 0; i < 4; i++) {
		nodes[i] = grid.findNodeById(element.node_id[i]);
	}

	// calculating Jakobian
	result.calculateJakobian(e, nodes, 0);
	result.calculateInverse();
	return result;
}



int main() {

	// LAB 1 - READING GRID FROM FILE //

	// grid 1
	// Grid grid4x4 = readFromFile("grids\\Test1_4_4.txt");
	//
	// std::cout << std::fixed << std::setprecision(10);
	//
	// std::cout << "Test 1 - 4x4 Grid:" << std::endl;
	// // grid4x4.displayGlobalData();
	// grid4x4.displayNodes();
	// grid4x4.displayElements();
	//
	// // grid 2
	// Grid mix_grid4x4 = readFromFile("grids\\Test2_4_4_MixGrid.txt");
	//
	// std::cout << "\n\nTest 2 - 4x4 MixGrid:" << std::endl;
	// mix_grid4x4.displayNodes();
	// mix_grid4x4.displayElements();
	//
	// // grid 3
	// Grid grid31x31 = readFromFile("grids\\Test3_31_31_kwadrat.txt");
	//
	// std::cout << "\n\nTest 3 - 31x31 Grid:" << std::endl;
	// grid31x31.displayNodes();
	// grid31x31.displayElements();
	//


	// // LAB 2 - GAUSS QUADRATURES METHOD
	//
	// std::cout << "\n\nIntegration using Gauss quadratures method." << std::endl;
	//
	// std::cout << std::fixed << std::setprecision(15);
	//
	// // integral of f(x) = 5x^2 + 3x + 6
	// std::cout << "\nf(x) = 5x^2 + 3x + 6\n";
	// std::cout << "\tNodes nr:\tResult:\n";
	// auto f = [](double x) {return 5 * x * x + 3 * x + 6; };
	//
	// for (int n = 1; n <= 4; n++) {
	// 	std::cout << "\t\t" << n << ": \t" << Quadrature(n).solveQuadratures(f) << std::endl;
	// }
	// // std::cout << "\tAccurate result: " << 15.333333333333333333333333333333 << std::endl;
	//
	// // integral of = 5x^2 + 3x + 6
	// std::cout << "\nf(x) = 5x^2y^2 + 3xy + 6\n";
	// std::cout << "\tNodes nr:\tResult:\n";
	// auto f_xy = [](double x, double y) {return 5 * x * x * y * y + 3 * x * y + 6; };
	//
	// for (int n = 1; n <= 4; n++) {
	// 	std::cout << "\t\t" << n << ": \t" << Quadrature(n).solveQuadratures(f_xy) << std::endl;
	// }
	// // std::cout << "\tAccurate result: " << 26.222222222222222222222222222222 << std::endl;

	// LAB 3 - JAKOBIAN

	// test element
	// creating nodes
	std::cout << "TESTING JAKOBIAN FOR ONE ELEMENT:" << std::endl;
	Node node1(1, 0, 0);
	Node node2(2, 0.025, 0);
	Node node3(3, 0.025, 0.025);
	Node node4(4, 0, 0.025);

	// creating	an element
	int nodes_id[4] = {1,2,3,4};
	Element element(1, nodes_id);

	// completing a grid with nodes and element
	Grid mygrid;
	mygrid.elements.push_back(element);
	mygrid.nodes.push_back(node1);
	mygrid.nodes.push_back(node2);
	mygrid.nodes.push_back(node3);
	mygrid.nodes.push_back(node4);
	mygrid.npc = 4;
	mygrid.nodes_number = 4;
	mygrid.elements_number = 1;

	// displaying nodes and element
	mygrid.displayNodes();
	mygrid.displayElements();

	// completing tables
	ElemUniv elem_univ(mygrid.npc);
	elem_univ.display();

	// calculating jakobians for whole element
	std::vector<Jakobian> jakobians = calculateJakobiansOfElement(1, mygrid, elem_univ);
	mygrid.elements[0].jakobian = jakobians[0];
	mygrid.elements[0].jakobians = jakobians;

	std::cout << std::endl;
	for (int i = 0; i < mygrid.npc; i++) {
		// std::cout << "\nJakobian in pc" << i+1 << ":" << std::endl;
		// jakobians[i].displayJakobian();
	}


	// LAB 4 - H MATRIX

	// creating dN/dx and dN/dy tables for test elem
	double dN_dx = 0;
	double dN_dy = 0;
	Element elem = mygrid.findElementById(1);

	for (int i = 0; i < mygrid.npc; i++) {
		std::vector<double> x_row;
		std::vector<double> y_row;

		// uzupelnianie tabel dla pc_i
		for (int j = 0; j < 4; j++) {
			dN_dx = elem.jakobians[i].J1[0][0] * elem_univ.dN_dE[i][j] + elem.jakobians[i].J1[0][1] * elem_univ.dN_dn[i][j];
			dN_dy = elem.jakobians[i].J1[1][0] * elem_univ.dN_dE[i][j] + elem.jakobians[i].J1[1][1] * elem_univ.dN_dn[i][j];
			x_row.push_back(dN_dx);
			y_row.push_back(dN_dy);
		}
		elem.dN_dx.push_back(x_row);
		elem.dN_dy.push_back(y_row);
	}
	elem.display_dN_dx();
	elem.display_dN_dy();

	// uzupelnianie macierzy H
	std::vector<std::vector<double>> H(4, std::vector<double>(4, 0.0));
	for (int i = 0; i < mygrid.npc; i++) {

		std::vector<double> dNdx = elem.dN_dx[i];
		std::vector<double> dNdy = elem.dN_dy[i];

		// obliczanie elementow macierzy dla calego elementu
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				H[j][k] += elem.jakobians[i].detJ * 30 * (dNdx[j] * dNdx[k] + dNdy[j] * dNdy[k]);
			}
		}
	}
	elem.H = H;

	std::cout << "\nMacierz H:\n";
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			std::cout << elem.H[i][j] << " ";
		}
		std::cout << "\n";
	}





	// testing jakobian for grid from file
	// reading grid from file
	// std::cout << "\n\nJAKOBIAN TEST - grid 4x4:";
	// Grid grid4x4 = readFromFile("grids\\Test1_4_4.txt");
	//
	// std::cout << std::fixed << std::setprecision(10);
	//
	// //  displaying jakobians of each element
	// for (int i = 0; i < grid4x4.elements_number; i++) {
	// 	std::cout << "\n";
	// 	grid4x4.elements[i].display();
	// 	grid4x4.elements[i].jakobian.displayJakobian();
	// }

	// jakobians for each node in 1 element
	// for (int i = 0; i < grid4x4.nodes_number; i++) {
	// 	std::cout << "\n";
	// 	grid4x4.elements[i].jakobians[0].displayJakobian();
	// }


	// czy jakobian ma byc taki sam dla kazdego punktu w elemencie?
	// JAKOBIAN TAKI SAM W KAZDYM ELEMENCIE?


	return 0;
}
