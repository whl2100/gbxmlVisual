#define _USE_MATH_DEFINES

#include <iostream>
#include <cmath>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <type_traits>
#include <vector>
#include <cstring>

class DataCsv{

public:
	DataCsv(std::string inputfile1,std::string inputfile2,std::string inputfile3);
	~DataCsv();
	std::vector<std::vector<double> > get_front_sonar_matrix();
	std::vector<std::vector<double> > get_front_sweep_matrix();
	std::vector<std::vector<double> > get_odom_matrix();

private:
	std::vector<std::vector<double> > front_sonar_matrix;
	std::vector<std::vector<double> > front_sweep_matrix;
	std::vector<std::vector<double> > odom_matrix;
};

std::vector<std::vector<double> > DataCsv::get_front_sonar_matrix(){


return front_sonar_matrix;
}

std::vector<std::vector<double> > DataCsv::get_front_sweep_matrix(){


return front_sweep_matrix;
}

std::vector<std::vector<double> > DataCsv::get_odom_matrix(){


return odom_matrix;
}



DataCsv::DataCsv(std::string inputfile1,std::string inputfile2,std::string inputfile3){

	const int MAX_CHARS_PER_LINE = 1024;

	const int MAX_TOKENS_PER_LINE = 40;

	const char* const DELIMITER = " ,";

	std::ifstream front_sonar_log;
	std::ifstream front_sweep_log;
	std::ifstream house_odom;

	char *token;
	char buf[MAX_CHARS_PER_LINE];

	front_sonar_log.open(inputfile1);
	if (!front_sonar_log.good()){exit;}
	front_sweep_log.open(inputfile2);
	if (!front_sweep_log.good()){exit;}
	house_odom.open(inputfile3);
	if (!house_odom.good()){exit;}

	while(!front_sonar_log.eof()){
		std::vector<double> numbers;
		front_sonar_log.getline(buf,MAX_CHARS_PER_LINE);
		for(token=strtok(buf,DELIMITER);token!=NULL;token=strtok(NULL,DELIMITER)){
			numbers.push_back(atof(token));
		}
		front_sonar_matrix.push_back(numbers);
		numbers.clear();
	}
	std::cout<<"Front Sonar Data Details"<< std::endl;
	std::cout <<"Number of observations: " <<front_sonar_matrix.size() << std::endl;
	std::cout <<"Number of fields per observation: " <<front_sonar_matrix[0].size() << std::endl;
	std::cout<<std::endl;

	while(!front_sweep_log.eof()){
		std::vector<double> numbers;
		front_sweep_log.getline(buf,MAX_CHARS_PER_LINE);
		for(token=strtok(buf,DELIMITER);token!=NULL;token=strtok(NULL,DELIMITER)){
			numbers.push_back(atof(token));
		}
		front_sweep_matrix.push_back(numbers);
		numbers.clear();
	}
	std::cout<<"Front Sweep Sonar Data Details"<< std::endl;
	std::cout <<"Number of observations: " <<front_sweep_matrix.size() << std::endl;
	std::cout <<"Number of fields per observation: " <<front_sweep_matrix[0].size() << std::endl;
	std::cout<<std::endl;

	while(!house_odom.eof()){
		std::vector<double> numbers;
		house_odom.getline(buf,MAX_CHARS_PER_LINE);
		for(token=strtok(buf,DELIMITER);token!=NULL;token=strtok(NULL,DELIMITER)){
			numbers.push_back(atof(token));
		}
		odom_matrix.push_back(numbers);
		numbers.clear();
	}
	std::cout<<"Odometry Data Details"<< std::endl;
	std::cout <<"Number of observations: " <<odom_matrix.size() << std::endl;
	std::cout <<"Number of fields per observation: " <<odom_matrix[0].size() << std::endl;
	std::cout<<std::endl;
}


DataCsv::~DataCsv(){

}