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


	//---------------------------------------------------------------------//
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


	//---------------------------------------------------------------------//
	// LAB 3 - JAKOBIAN / LAB 4, 5 - MATRIX H

	// // test element
	// // creating nodes
	// Node node1(1, 0, 0);
	// Node node2(2, 0.025, 0);
	// Node node3(3, 0.025, 0.025);
	// Node node4(4, 0, 0.025);
	//
	// // creating	an element
	// int nodes_id[4] = {1,2,3,4};
	// Element element0(1, nodes_id);
	//
	// // completing a grid with nodes and element
	// Grid mygrid;
	// mygrid.elements.push_back(element0);
	// mygrid.nodes.push_back(node1);
	// mygrid.nodes.push_back(node2);
	// mygrid.nodes.push_back(node3);
	// mygrid.nodes.push_back(node4);
	// mygrid.npc = 4;
	// mygrid.nodes_number = 4;
	// mygrid.elements_number = 1;
	// mygrid.globalData["Conductivity"] = 30;
	//
	// // displaying nodes and element
	// // mygrid.displayNodes();
	// // mygrid.displayElements();
	//
	// // completing tables and displaying them
	// // ElemUniv elem_univ_4pc(mygrid.npc);
	// // elem_univ_4pc.display();
	//
	// // calculating jakobians for every element
	// mygrid.calculateAllJakobians();
	// // mygrid.elements[0].display();
	//
	// // calculating shape derivatives
	// mygrid.calculateAllShapeDerivatives();
	// // mygrid.elements[0].display_dN_dx();
	// // mygrid.elements[0].display_dN_dy();
	//
	// // calculating H matrix
	// mygrid.calculateAllMatrixH();
	// std::cout << "\nTEST 0:\nMatrix H:" << std::endl;
	// mygrid.elements[0].display_H();
	//
	// // TEST 1 - for 9 npc
	// Element element1(2, nodes_id);
	// mygrid.elements.push_back(element1);
	// mygrid.npc = 9;
	// mygrid.calculateAllJakobians();
	// mygrid.calculateAllShapeDerivatives();
	// mygrid.calculateAllMatrixH();
	// std::cout << "\nTEST 1:\nMatrix H:" << std::endl;
	// mygrid.elements[1].display_H();
	// mygrid.elements[1].display_dN_dx();
	// mygrid.elements[1].display_dN_dy();
	//
	// // TEST 2 - not square element
	// Node node(5, 0.01, -0.01);
	// nodes_id[0] = 5;
	// Element element2(3, 	nodes_id);
	// mygrid.elements.push_back(element2);
	// mygrid.nodes.push_back(node);
	// mygrid.npc = 4;
	// mygrid.nodes_number ++;
	// mygrid.elements_number ++;
	// mygrid.calculateAllJakobians();
	// mygrid.calculateAllShapeDerivatives();
	// mygrid.calculateAllMatrixH();
	// std::cout << "\nTEST 2:\nMatrix H:" << std::endl;
	// mygrid.elements[2].display_H();


	//---------------------------------------------------------------------//
	// LAB 6 - testing grids from file
	// reading grid from file
	// std::cout << "\n\nTEST - grid 4x4:";
	// Grid grid4x4 = readFromFile("grids\\Test1_4_4.txt");
	//
	// std::cout << std::fixed << std::setprecision(10);
	// calculateH(grid4x4);

	// std::cout << "\nH Matrixes:";
	// grid4x4.displayAllMatrixH();

	// std::cout << "\n\nTEST - migrid 4x4:";
	// Grid grid2 = readFromFile("grids\\Test3_31_31_kwadrat.txt");
	// grid2.displayNodes();
	// grid2.displayElements();
	//
	//
	// std::cout << std::fixed << std::setprecision(10);
	//
	// calculateH(grid2);
	//
	// // printing h matrix
	// std::cout << "\nH Matrixes:";
	// grid2.displayAllMatrixH();


	//---------------------------------------------------------------------//
	// LAB7 - MATRIX H AGGREGATION

	// TEST 1
	// std::cout << "\n\nTEST - grid 4x4:";
	// Grid grid4x4 = readFromFile("grids\\Test1_4_4.txt");
	// calculateH(grid4x4);
	//
	// GlobalSystemOfEquation global_system_of_equation;
	// aggregateMatrixH(grid4x4, global_system_of_equation);
	//
	// // diplay global h
	// std::cout << std::fixed << std::setprecision(3);
	// std::cout << "\n\nGLOBAL H:" << std::endl;
	// global_system_of_equation.displayMatrixH();
	//
	//
	// // TEST 2
	// std::cout << "\n\nTEST - mix grid 4x4:";
	// Grid mix_grid4x4 = readFromFile("grids\\Test2_4_4_MixGrid.txt");
	// calculateH(mix_grid4x4);
	//
	// aggregateMatrixH(mix_grid4x4, global_system_of_equation);
	//
	// // diplay global h
	// std::cout << std::fixed << std::setprecision(3);
	// std::cout << "\n\nGLOBAL H:" << std::endl;
	// global_system_of_equation.displayMatrixH();

	//---------------------------------------------------------------------//
	// LAB 8 - przeczytanie wezlow z pliku - dodac bool do node (1 - jeste brzegowy, 0 - nie jest na brzegu)
	// warunek brzegowy gdy 2 wezly na brzegu (z flaga 1)
	// wyznaczenie w strukturze elementu drugiego schematu calkowania ktory dotyczy TYLKO calkowania na powierzchni
	// calka w 1 i 2 punktcie i zsumowac
	// wyznacznik z pitagorasa

	// read BC from file added
	// Grid grid4x4 = readFromFile("grids\\Test1_4_4.txt");
	// for (int i = 0; i < grid4x4.nodes_number; i++) {
	// 	std::cout << grid4x4.nodes[i].id << " - BC: " << grid4x4.nodes[i].BC << std::endl;
	// }
	//
	// calculateH(grid4x4);
	//
	// // displaying all H
	// grid4x4.displayAllMatrixH();

	// test element
	// std::cout << "TEST ELEMENT:" << std::endl;
	// Node node1(1, 0, 0, 1);
	// Node node2(2, 0.025, 0, 1);
	// Node node3(3, 0.025, 0.025, 1);
	// Node node4(4, 0, 0.025, 1);
	//
	// // creating	an element
	// int nodes_id[4] = {3,4,1,2};
	// Element element0(1, nodes_id);
	//
	// // completing a grid with nodes and element
	// Grid mygrid;
	// mygrid.elements.push_back(element0);
	// mygrid.nodes.push_back(node1);
	// mygrid.nodes.push_back(node2);
	// mygrid.nodes.push_back(node3);
	// mygrid.nodes.push_back(node4);
	// mygrid.npc = 4;
	// mygrid.nodes_number = 4;
	// mygrid.elements_number = 1;
	// mygrid.globalData["Conductivity"] = 25;
	// mygrid.globalData["Alfa"] = 25;
	//
	// // displaying nodes and element
	// mygrid.displayNodes();
	// mygrid.displayElements();
	//
	// // calculating H matrix
	// calculateH(mygrid);
	//
	// mygrid.displayAllMatrixH();

	// // grid 4x4 test
	// std::cout << "GRID 4X4 TEST:" << std::endl;
	// Grid grid4x4 = readFromFile("grids\\Test1_4_4.txt");
	// grid4x4.npc = 4;
	//
	// // grid4x4.displayNodes();
	// // grid4x4.displayElements();
	//
	// calculateH(grid4x4);
	//
	// // displaying all H
	// // grid4x4.displayAllMatrixH();
	//
	// // H agregation
	// GlobalSystemOfEquation global_system_of_equation;
	// aggregateMatrixH(grid4x4, global_system_of_equation);
	// aggregateVectorP(grid4x4, global_system_of_equation);
	//
	// // diplay global h
	// std::cout << std::fixed << std::setprecision(3);
	// std::cout << "\n\nGLOBAL H:" << std::endl;
	// global_system_of_equation.displayMatrixH();
	//
	// // display global vector P
	// std::cout << std::fixed << std::setprecision(3);
	// std::cout << "\n\nVECTOR P:" << std::endl;
	// global_system_of_equation.displayVectorP();
	//
	// // solve equation
	// global_system_of_equation.solve();
	// std::cout << "\n\nVECTOR T:" << std::endl;
	// global_system_of_equation.displayVectorT();

	//---------------------------------------------------------------------//
	// LAB 9 - calculating vector P (1xliczba wezlow) and its agregation
	// dodac wektor globalny P do struktury rozw. ukladu rownan
	// dodac metode do rozwiazywania ukladu rownac (jakis gauss) - raczej nie zadziala to co mam
	// stworzyc wektor P[4] w struct elementu
	// policzy wektory P dla kazdego elementu
	// zagregowac
	// rozwiazac uklad rownan [H]{t} = - {P}


	// std::cout << "\n\nMIX GRID 4X4 TEST:" << std::endl;
	// Grid mix_grid = readFromFile("grids\\Test2_4_4_MixGrid.txt");
	// mix_grid.npc = 4;
	//
	// calculateH(mix_grid);
	//
	// // displaying all vectors P
	// // std::cout << "\n\nVECTORS P:" << std::endl;
	// // for (auto& element : mix_grid.elements) {
	// // 	element.display_P();
	// // }
	//
	// // H and P agregation
	// aggregateMatrixH(mix_grid, global_system_of_equation);
	// aggregateVectorP(mix_grid, global_system_of_equation);
	//
	// // diplay global h
	// std::cout << std::fixed << std::setprecision(3);
	// std::cout << "\n\nGLOBAL H:" << std::endl;
	// global_system_of_equation.displayMatrixH();
	//
	// // display global vector P
	// std::cout << std::fixed << std::setprecision(3);
	// std::cout << "\n\nVECTOR P:" << std::endl;
	// global_system_of_equation.displayVectorP();
	//
	// // solve equation
	// global_system_of_equation.solve();
	// std::cout << "\n\nVECTOR T:" << std::endl;
	// global_system_of_equation.displayVectorT();

	//---------------------------------------------------------------------//
	// LAB 10 - calculating matrix C
	//1. w elem policzyc wartosci funkcji ksztaltu w punktach calkowania
	// robimy macierze C w kazdym punkcie i sumujemy
	// bez petli po czasie
	// w strukt wezla dodac temperature
	// czytajac z pliku temp poczatkowej przypisujemy temp poczatkowa do kazdego wezla
	// rozwiazujemy uklad rownan
	// delta tau to step time w pliku
	// t0 podmieniamy na t1 w wezlach (w 1 iteracji itd)
	// iteraCJe = sim time/step time

	// grid 4x4 test
	std::cout << "GRID 4X4 TEST:" << std::endl;
	Grid grid4x4 = readFromFile("grids\\Test1_4_4.txt");
	grid4x4.npc = 4;

	// grid4x4.displayNodes();
	// grid4x4.displayElements();

	calculateH(grid4x4);

	for (auto& element : grid4x4.elements) {
		//element.display_dN_dy();
		// element.display_C();
	}

	// agregation
	GlobalSystemOfEquation SOE;
	aggregate(grid4x4, SOE);
	std::cout << "\nGLOBAL MATRIX C" << std::endl;
	SOE.displayMatrixC();
	std::cout << "\nGLOBAL VECTOR P" << std::endl;
	SOE.displayVectorP();
	std::cout << "\nGLOBAL MATRIX H" << std::endl;
	SOE.displayMatrixH();

	for (auto& element : grid4x4.elements) {
		element.display_P();
	}

	// solving system of equation
	SOE.solve(grid4x4);
	std::cout << "\nGLOBAL VECTOR T" << std::endl;
	SOE.displayVectorT();

	return 0;
}
