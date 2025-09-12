//********************************************************************
//  Programmer: Erik Bowers			Student ID: 
//  Contact:    
//  Course:     C867
//  Creation date: 10/21/2020
//  Last updated : 10/22/2020
//
//	File name: student.cpp
//	Purpose:   cpp file for the student class.
//
//	Constructors:
//               student()
//					default constructor. Builds Student object with empty data.
//
//               student(string,string,string,string,int,vector<int>,DegreeProgram)
//				    Builds Student object with given input.
//
//  Functions:
//				Accessors:
//						  getStudentID()
//										Returns (string) studentID
//						  getFirstName()
//										Returns (string) firstName
//					      getLastName()
//										Returns (string) lastName
//						  getEmailAddress()
//                                      Returns (string) emailAddress
//                        getStudentAge()
//									    Returns (int) studentAge
//						  getDaysToCompleteCourse()
//										Returns (vector<int>) daysToCompleteCourse
//                        getDegreeProgram()
//                                      Returns (DegreeProgram) degreeProgram
//				Mutators:
//						  setStudentID()
//										Sets (string) studentID
//						  setFirstName()
//										Sets (string) firstName
//					      setLastName()
//										Sets (string) lastName
//						  setEmailAddress()
//                                      Sets (string) emailAddress
//                        setStudentAge()
//									    Sets (int) studentAge
//						  setDaysToCompleteCourse()
//										Sets (vector<int>) daysToCompleteCourse
//                        setDegreeProgram()
//                                      Sets (DegreeProgram) degreeProgram
//
//				print()
//						Prints a Student's data in a readable format
//********************************************************************
#include <iostream>
#include <vector>
#include "student.h"
using namespace std;


student::student() {
	studentID = "";
	firstName = "";
	lastName = "";
	emailAddress = "";
	studentAge = 0;
	daysToCompleteCourse;
	degree;
}

//constructor for student
student::student(string studentID, string newFirstName, string newLastName, string newEmailAddress, int newAge, vector<int> newDaysToComopleteCourse, DegreeProgram degreeProgram){
	this->studentID = studentID;
	firstName = newFirstName;
	lastName = newLastName;
	emailAddress = newEmailAddress;
	studentAge = newAge;
	daysToCompleteCourse = newDaysToComopleteCourse;
	degree = degreeProgram;
}//end constructor for student

//Accessors for student variables

//Accessor for studentID
string student::getStudentID() {
	return studentID;
}//end function getStudentID()

//Accessor for firstName
string student::getFirstName() {
	return firstName;
}//end function getFirstName()

//Accessor for lastName
string student::getLastName() {
	return lastName;
}//end function getLastName()

//Accessor for emailAddress
string student::getEmailAddress() {
	return emailAddress;
}//end function getEmailAddress()

//Accessor for studentAge
int student::getStudentAge() {
	return studentAge;
}//end function getStudentAge()

//Accessor for vector<int> daysToCompleteCourse
vector<int> student::getDaysToCompleteCourse() {
	return daysToCompleteCourse;
}//end function getDaysToCompleteCourse()

//Accessor for degree
DegreeProgram student::getDegreeProgram() {
	return degree;
}//end function getDegreeProgram()

//Mutators for student variables

//mutator for studentID
void student::setStudentID(string newStudentID) {
	studentID = newStudentID;
}//end function setStudentID(string)

//mutator for firstName
void student::setFirstName(string newFirstName) {
	firstName = newFirstName;
}//end function setFirstName(string)

//mutator for lastName
void student::setLastName(string newLastName) {
	lastName = newLastName;
}//end function setLastName(string)

//mutator for emailAddress
void student::setEmailAddress(string newEmailAddress) {
	emailAddress = newEmailAddress;
}//end function setEmailAddress(string)

//mutator for studentAge
void student::setStudentAge(int newAge) {
	studentAge = newAge;
}//end function setStudentAge(int)

//mutator for vector<int> daysToCompleteCourse
void student::setDaysToCompleteCourse(vector<int> newDaysToCompleteCourse) {
	daysToCompleteCourse = newDaysToCompleteCourse;
}//end function setDaysToCompleteCourse(vector<int>)

//mutator for degree
void student::setDegreeProgram(DegreeProgram newDegree) {
	degree = newDegree;
}//end function setDegreeProgram(DegreeProgram)

//functions for student that are not accessors/mutators/constructors

//print student data. Uses cout
void student::print() {
	int i = 0, numberOfCourses = 0;
	cout << studentID << "\tFirst Name: " << firstName << "\tLast Name: " << lastName;
	cout << "\tAge: " << studentAge << " " << "\tdaysInCourse: {";
	numberOfCourses = daysToCompleteCourse.size();
	for (i = 0; i < numberOfCourses - 1; i++) {
		cout << daysToCompleteCourse.at(i) << ", ";
	}
	cout << daysToCompleteCourse.at(i) << "} ";
	cout << "\tDegree Program: ";
	if (degree == SECURITY) {
		cout << "Security. ";
	}
	else if (degree == NETWORK) {
		cout << "Network. ";
	}
	else if (degree == SOFTWARE) {
		cout << "Software. ";
	}
	else {
		cout << "Degree not found. ";
	}
	cout << endl;
}//end function print()
//end student functions