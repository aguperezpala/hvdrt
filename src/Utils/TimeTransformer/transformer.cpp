/* Toma un archivo .agu y transforma los tiempos a 0 tomando como primer
 * (el valor 0) el primer valor de la lista
 */


#include <iostream>
#include <string>
#include <fstream>


int main(int argc, char **args)
{
	if(argc < 2) {
		std::cout << "Error falta archivo a transformar\n";
		return -1;
	}

  	std::ifstream ifs ( args[1], std::ifstream::in );
	std::ofstream ofs ( "out.agu", std::ofstream::out );

	double t,v;
	std::string aux;
	char buff[501];

	// removemos las primeros 2 valores"
	ifs.getline(buff, 500);
	std::cout << "first line: " << buff << std::endl;

	double ft;
	char c;
	ifs >> 	ft >> c >> v;
	ofs << 0.0 << "," << v << std::endl;
	while (ifs.good()){
		ifs >> 	t >> c >> v;
		ofs << t - ft << "," << v << std::endl;	
	}

	ifs.close();
	ofs.close();

	return 0;
}

	 
