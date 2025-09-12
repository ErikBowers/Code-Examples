//********************************************************************
//  Programmer: Erik Bowers			Student ID: 
//  Contact:	
//  Course:		C867
//  Creation date: 10/21/2020
//  Last updated : 10/22/2020
//
//	File name: roster.cpp
//	Purpose:   cpp file for the roster class.
//			   Creates and holds an array of pointers to
//			   Student objects.
//			   Functions for manipulating Student data
//             contained in the roster.
//
//  Functions:
//            add
//				 Adds a Student to the roster with given data.
//            remove
//               Removes a Student from the roster with given ID.
//               If no student with ID is found prints that none
//               was found.
//            printAll
//               For every Student in roster, calls their print function
//            printAverageDaysInCourse
//               For Student with given ID, prints the average days
//               in a course given their days in 3 courses.
//            printInvalidEmails
//               For each Student in roster, checks that their email
//               address is valid. A valid email contains exactly one '@',
//               contains at least one '.', and contains no ' '.
//               If an email is not valid, the email is printed.
//            printByDegreeProgram
//               Using given enumerated datatype DegreeProgram, calls
//               a Student's print function if that Student's DegreeProgram
//               matches input
//********************************************************************

#include <iostream>
#include "roster.h"
#include "student.h"
using namespace std;

int i = 0;

//roster functions

//add new student to roster
void roster::add(string studentID, string firstName, string lastName, string emailAddress, int age, int daysInCourse1, int daysInCourse2, int daysInCourse3, DegreeProgram degreeProgram){
	vector<int> daysToCompleteCourse = { daysInCourse1, daysInCourse2, daysInCourse3 };
	student* newStudent = nullptr;
	 newStudent = new student(studentID, firstName, lastName, emailAddress, age, daysToCompleteCourse, degreeProgram);
	classRosterArray[roster::currentArrayElement] = newStudent;
	roster::currentArrayElement++;
}//end function add(string,string,string,int,int,int,int,DegreeProgram)

//remove student from roster using ID number
void roster::remove(string studentID) {
	bool wasRemoved = false;
	for (i = 0; i < roster::currentArrayElement - 1; i++) {
		if (classRosterArray[i]->getStudentID() == studentID) {
			classRosterArray[i] = nullptr;
			for (int j = i; j < roster::currentArrayElement - 1; j++) {
				classRosterArray[j] = classRosterArray[j + 1];
			}
			wasRemoved = true;
			roster::currentArrayElement--;
			break;
		}
	}//end for loop
	if (wasRemoved == false) {
		cout << "No student found with ID '" << studentID << "'." << endl;
	}
}//end function remove(string)

//call student's print function for each student in roster
void roster::printAll() {
	for (i = 0; i < roster::currentArrayElement; i++) {
		classRosterArray[i]->print();
	}//end for loop
}//end function printAll()

//use cout to print average days in course using studentID and daysInCourse array
void roster::printAverageDaysInCourse(string studentID) {
	int averageDaysInCourse = 0;
	vector<int> daysInCourse;
	for (i = 0; i < roster::currentArrayElement; i++) {
		if (classRosterArray[i]->getStudentID() == studentID) {
			daysInCourse = classRosterArray[i]->getDaysToCompleteCourse();
			for (unsigned int j = 0; j < daysInCourse.size(); j++) {
				averageDaysInCourse += daysInCourse.at(j);
			}//end inner for loop
			averageDaysInCourse /= daysInCourse.size();
		}
	}//end outer for loop
	cout << "Average days per course for student '" << studentID << "': " << averageDaysInCourse << "." << endl;
}//end function printAverageDaysInCourse(string)

//iterate through roster and print any invalid email address
void roster::printInvalidEmails() {
	//a valid email address has one '@' and at least one '.'
	//a valid email address cannot contain a space character
	string tempEmail;
	bool isValid;
	for (i = 0; i < roster::currentArrayElement; i++) {
		isValid = true;
		tempEmail = classRosterArray[i]->getEmailAddress();
		if (tempEmail.find_first_of(".") != string::npos) {
			//This block used to detect if more than one period is used
			//if (tempEmail.find_first_of(".") != tempEmail.find_last_of(".")) {
			//	//it must contain two or more periods
			//	isValid = false;
			//}
		}
		else {
			//it contains no period
			isValid = false;
		}

		if (tempEmail.find_first_of("@") != string::npos) {
			if (tempEmail.find_first_of("@") != tempEmail.find_last_of("@")) {
				//it must contain two or more at signs
				isValid = false;
			}
		}
		else {
			//it contains no at sign
			isValid = false;
		}

		if (tempEmail.find_first_of(" ") != string::npos) {
			//it contains at least one space
			isValid = false;
		}

		if(isValid == false){
			//email was invalid, print it
			cout << tempEmail << endl;
		}
	}//end for loop
}//end function printInvalidEmails()

//print student information from roster if their degree program matches input
void roster::printByDegreeProgram(DegreeProgram degreeProgram) {
	for (i = 0; i < roster::currentArrayElement; i++) {
		if (classRosterArray[i]->getDegreeProgram() == degreeProgram) {
			classRosterArray[i]->print();
		}
	}//end for loop
}//end function printByDegreeProgram(DegreeProgram)
