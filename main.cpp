//********************************************************************
//  Programmer: Erik Bowers			Student ID: 
//  Contact:    
//  Course:     C867
//  Creation date: 10/21/2020
//  Last updated : 10/23/2020
//
//	File name: main.cpp
//	Purpose:   main file for final project.
//             Creates a roster of Students, calls functions
//             as outlined in program requirements.
//********************************************************************

#include <iostream>
#include <string>
#include <vector>
#include "degree.h"
#include "student.h"
#include "roster.h"
using namespace std;

int main() {
	cout << "C867 Scripting and Programming - Applications. C++ used. Student ID: **********. Name: Erik Bowers." << endl; //required header

	//input data
	const string studentData[] = {
		"A1,John,Smith,John1989@gm ail.com,20,30,35,40,SECURITY",
		"A2,Suzan,Erickson,Erickson_1990@gmailcom,19,50,30,40,NETWORK",
		"A3,Jack,Napoli,The_lawyer99yahoo.com,19,20,40,33,SOFTWARE",
		"A4,Erin,Black,Erin.black@comcast.net,22,50,58,40,SECURITY",
		"A5,Erik,Bowers,example@my.wgu.edu,30,36,21,0,SOFTWARE"
	};//end studentData[string]

	//parse each string, breaking it into bits and add everything as a Student in classRoster
	roster* classRoster = new roster;
	for (int i = 0; i < ARRAY_SIZE; i++) { //ARRAY_SIZE a global variable set in roster.cpp
		string tempStudentID, tempFirstName, tempLastName, tempEmail, tempDegreeAsString;
		int tempAge, tempCourse1, tempCourse2, tempCourse3;
		DegreeProgram tempDegree = SECURITY;
		string tempString = studentData[i];
		int frontParsePosition = 0, rearParsePosition = 0;

			//find a comma or the end of the string
			//take everything from start of search to before comma, add it to temp variable
			//repeat until end of string is reached, ending while loop with data parsed and put into variables
			rearParsePosition = tempString.find_first_of(",");
			tempStudentID = tempString.substr(frontParsePosition, (rearParsePosition) - frontParsePosition);

			frontParsePosition = rearParsePosition + 1;
			rearParsePosition = tempString.find_first_of(",", frontParsePosition);
			tempFirstName = tempString.substr(frontParsePosition, (rearParsePosition) - frontParsePosition);

			frontParsePosition = rearParsePosition + 1;
			rearParsePosition = tempString.find_first_of(",", frontParsePosition);
			tempLastName = tempString.substr(frontParsePosition, (rearParsePosition) - frontParsePosition);

			frontParsePosition = rearParsePosition + 1;
			rearParsePosition = tempString.find_first_of(",", frontParsePosition);
			tempEmail = tempString.substr(frontParsePosition, (rearParsePosition) - frontParsePosition);

			frontParsePosition = rearParsePosition + 1;
			rearParsePosition = tempString.find_first_of(",", frontParsePosition);
			tempAge = stoi((tempString.substr(frontParsePosition, (rearParsePosition) - frontParsePosition)),nullptr,10);

			frontParsePosition = rearParsePosition + 1;
			rearParsePosition = tempString.find_first_of(",", frontParsePosition);
			tempCourse1 = stoi((tempString.substr(frontParsePosition, (rearParsePosition) - frontParsePosition)), nullptr, 10);
			
			frontParsePosition = rearParsePosition + 1;
			rearParsePosition = tempString.find_first_of(",", frontParsePosition);
			tempCourse2 = stoi((tempString.substr(frontParsePosition, (rearParsePosition) - frontParsePosition)), nullptr, 10);

			frontParsePosition = rearParsePosition + 1;
			rearParsePosition = tempString.find_first_of(",", frontParsePosition);
			tempCourse3 = stoi((tempString.substr(frontParsePosition, (rearParsePosition) - frontParsePosition)), nullptr, 10);

			frontParsePosition = rearParsePosition + 1;
			rearParsePosition = tempString.size() - frontParsePosition;
			tempDegreeAsString = tempString.substr(frontParsePosition, rearParsePosition);
			if (tempDegreeAsString == "SOFTWARE") {
				tempDegree = DegreeProgram::SOFTWARE;
			}
			else if (tempDegreeAsString == "SECURITY") {
				tempDegree = DegreeProgram::SECURITY;
			}
			else if (tempDegreeAsString == "NETWORK") {
				tempDegree = DegreeProgram::NETWORK;
			}

			classRoster->add(tempStudentID, tempFirstName, tempLastName, tempEmail, tempAge, tempCourse1, tempCourse2, tempCourse3, tempDegree);
	}//end for loop for parsing studentData[]

	classRoster->printAll();
	classRoster->printInvalidEmails();

	for (int i = 0; i < ARRAY_SIZE; i++) {
		classRoster->printAverageDaysInCourse(classRoster->classRosterArray[i]->getStudentID());
	}

	classRoster->printByDegreeProgram(DegreeProgram::SOFTWARE);
	classRoster->remove("A3");
	classRoster->printAll();
	classRoster->remove("A3");

	delete classRoster;
}//end main