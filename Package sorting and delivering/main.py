# Erik Bowers 
# WGU June 2021 for C950
# Created: 6/30/2021    Created By: Erik Bowers
# Last updated: 8/11/2021        Last updated by: Erik Bowers

import sys
import csv
import datetime
from datetime import timedelta
from package import Package
from address import Address
from chainingHashTable import chaining_hash_table

# fill a list with all the addresses
addressBook = []
addressBook.append(Address(0, "4001 South 700 East", "84107"))
addressBook.append(Address(1, "1060 Dalton Ave S", "84104"))
addressBook.append(Address(2, "1330 2100 S", "84106"))
addressBook.append(Address(3, "1488 4800 S", "84123"))
addressBook.append(Address(4, "177 W Price Ave", "84115"))
addressBook.append(Address(5, "195 W Oakland Ave", "84115"))
addressBook.append(Address(6, "2010 W 500 S", "84104"))
addressBook.append(Address(7, "2300 Parkway Blvd", "84119"))
addressBook.append(Address(8, "233 Canyon Rd", "84103"))
addressBook.append(Address(9, "2530 S 500 E", "84106"))
addressBook.append(Address(10, "2600 Taylorsville Blvd", "84118"))
addressBook.append(Address(11, "2835 Main St", "84115"))
addressBook.append(Address(12, "300 State St", "84103"))
addressBook.append(Address(13, "3060 Lester St", "84119"))
addressBook.append(Address(14, "3148 S 1100 W", "84119"))
addressBook.append(Address(15, "3365 S 900 W", "84119"))
addressBook.append(Address(16, "3575 W Valley Central Station bus Loop", "84119"))
addressBook.append(Address(17, "3595 Main St", "84115"))
addressBook.append(Address(18, "380 W 2880 S", "84115"))
addressBook.append(Address(19, "410 S State St", "84111"))
addressBook.append(Address(20, "4300 S 1300 E", "84117"))
addressBook.append(Address(21, "4580 S 2300 E", "84117"))
addressBook.append(Address(22, "5025 State St", "84107"))
addressBook.append(Address(23, "5100 South 2700 West", "84118"))
addressBook.append(Address(24, "5383 S 900 East #104", "84117"))
addressBook.append(Address(25, "600 E 900 South", "84105"))
addressBook.append(Address(26, "6351 South 900 East", "84121"))

#for i in range(len(addressBook)):
#    print(addressBook[i].getFullAddress())

packageTable = chaining_hash_table(60)
# add in all 40 packages to the hash table
# load these packages from a csv file
# how to use csv for Python all from https://docs.python.org/3/library/csv.html
with open('WGUPS Package File.csv', newline='') as packagecsvfile:
    packagereader = csv.reader(packagecsvfile, delimiter=',', quotechar='|')
    for row in packagereader:
        # print for testing
        # print(', '.join(row))
        # add each piece to a package object
        tempPackage = Package(int(row[0]),row[1],row[2],row[3],row[4],row[5],float(row[6]),row[7])
        # then add that to the hash table
        packageTable.insert(tempPackage, tempPackage.getID())

# load all of the address data from the .csv file into a 2D list
# first, create the 2D list and fill everything with zero
distances = []
for i in range(50):
    distances.append([])
    for j in range(50):
        distances[i].append(0)
# now, loop through each cell, adding in a list
with open('WGUPS Distance Table.csv', newline='') as distancecsvfile:
    distancereader = csv.reader(distancecsvfile, delimiter=",", quotechar='|')
    i = 0
    for row in distancereader:
        #print(row)
        j = 0
        #print(i, j)
        for j in range(len(row)):
            if(row[j] != ""):
                distances[i][j] = row[j]
            else:
                distances[i][j] = 0
        i = i + 1
    # all the cells are either zero or filled with the value from the csv file
    # for searching, that zero will tell the algorithm to swap destonation and current location and search again. If it's still a zero than that package goes to the current address.

# print out everything for testing
#i = 0
#j = 0
#for i in range(50):
#    for j in range(50):
#      print (i, j, distances[i][j])

# some of these packages are not yet at the hub
# update package 6
tempPackage = packageTable.search(6)
tempPackage.setLocation("en route to hub")
packageTable.remove(6)
packageTable.insert(tempPackage, tempPackage.getID())
# update package 25
tempPackage = packageTable.search(25)
tempPackage.setLocation("en route to hub")
packageTable.remove(25)
packageTable.insert(tempPackage, tempPackage.getID())
# update package 28
tempPackage = packageTable.search(28)
tempPackage.setLocation("en route to hub")
packageTable.remove(28)
packageTable.insert(tempPackage, tempPackage.getID())
# update package 32
tempPackage = packageTable.search(32)
tempPackage.setLocation("en route to hub")
packageTable.remove(32)
packageTable.insert(tempPackage, tempPackage.getID())
# With that complete, it's possible to start adding them onto the trucks

truck1 = []
truck2 = []
truck3 = []

# Truck1 holds packages at the hub that have a deadline of 9 or 10:30 am or go to the same address as one that does
truck1.append(14)
truck1.append(15)
truck1.append(16)
truck1.append(34)
truck1.append(20)
truck1.append(21)
truck1.append(13)
truck1.append(39)
truck1.append(40)
truck1.append(4)
#10 packages, so 6 slots remaining

# Truck2 holds the packages that are delayed and won't end up at the hub until 9:05am
# as well as some more due at 10:30 and packages that have the same address as a package that has a 10:30am time
truck2.append(30)
truck2.append(8)
truck2.append(9)
truck2.append(29)
truck2.append(7)
truck2.append(37)
truck2.append(38)
truck2.append(5)
truck2.append(1)
truck2.append(6)
truck2.append(25)
truck2.append(26)
truck2.append(31)
truck2.append(32)
#14 packages, so 2 slots remaining

#the remaining 16 all fit on truck 3, and are all EOD delivery times
truck3.append(2)
truck3.append(3)
truck3.append(10)
truck3.append(11)
truck3.append(12)
truck3.append(17)
truck3.append(18)
truck3.append(19)
truck3.append(22)
truck3.append(23)
truck3.append(24)
truck3.append(27)
truck3.append(28)
truck3.append(33)
truck3.append(35)
truck3.append(36)

# With the trucks all loaded, sort them
# The sorting algorithm used is Nearest Neighbor
# Search entire list, comparing package at i with the rest of the list to find shortest distance, swapping if shorter distance is found
# Then, using the previous location, increase i by one and compare everything remaining in the list
# Sort truck1
for i in range(len(truck1)):
    j = i + 1
    while j < len(truck1):
        #get current location and compare to possible destinations        
        if(i < 1):
            truckLocation = 0 #still at the hub
        else:
            truckAddress = packageTable.search(truck1[i-1]).getSearchingAddress()
            for k in range(len(addressBook)):
                if truckAddress == addressBook[k].getFullAddress():
                    truckLocation = addressBook[k].getID()

        package1address = packageTable.search(truck1[i]).getSearchingAddress()
        for k in range(len(addressBook)):
            if package1address == addressBook[k].getFullAddress():
                destination1 = addressBook[k].getID()
        package2address = packageTable.search(truck1[j]).getSearchingAddress()
        for k in range(len(addressBook)):
            if package2address == addressBook[k].getFullAddress():
                destination2 = addressBook[k].getID()
        #with the two possible destinations, find which is shorter
        distance1 = distances[truckLocation][destination1]
        #print(destination1, ": distance1: ", distance1)
        if not distance1:
            distance1 = distances[destination1][truckLocation]
            #print("was zero, check reverse. Distance1: ", distance1)

        distance2 = distances[truckLocation][destination2]
        #print(destination2, ": distance2: ", distance2)
        if not distance2:
            distance2 = distances[destination2][truckLocation]
            #print("was zero, check reverse. Distance2: ", distance2)

        if float(distance2) < float(distance1):
            #print(distance2, " is less than ", distance1)
            tempID = truck1[i]
            truck1[i] = truck1[j]
            truck1[j] = tempID
        j = j + 1
    #end while loop
#end for loop

# sort truck2
for i in range(len(truck2)):
    j = i + 1
    while j < len(truck2):
        #get current location and compare to possible destinations        
        if(i < 1):
            truckLocation = 0 #still at the hub
        else:
            truckAddress = packageTable.search(truck2[i-1]).getSearchingAddress()
            for k in range(len(addressBook)):
                if truckAddress == addressBook[k].getFullAddress():
                    truckLocation = addressBook[k].getID()

        package1address = packageTable.search(truck2[i]).getSearchingAddress()
        for k in range(len(addressBook)):
            if package1address == addressBook[k].getFullAddress():
                destination1 = addressBook[k].getID()
        package2address = packageTable.search(truck2[j]).getSearchingAddress()
        for k in range(len(addressBook)):
            if package2address == addressBook[k].getFullAddress():
                destination2 = addressBook[k].getID()
        #with the two possible destinations, find which is shorter
        distance1 = distances[truckLocation][destination1]
        if not distance1:
            distance1 = distances[destination1][truckLocation]

        distance2 = distances[truckLocation][destination2]
        if not distance2:
            distance2 = distances[destination2][truckLocation]

        if distance2 <= distance1:
            tempID = truck2[i]
            truck2[i] = truck2[j]
            truck2[j] = tempID
        j = j + 1
    #end while loop
#end for loop

# Sort truck3
for i in range(len(truck3)):
    j = i + 1
    while j < len(truck3):
        #get current location and compare to possible destinations        
        if(i < 1):
            truckLocation = 0 #still at the hub
        else:
            #truckLocation = truck3[i-1]
            truckAddress = packageTable.search(truck3[i-1]).getSearchingAddress()
            for k in range(len(addressBook)):
                if truckAddress == addressBook[k].getFullAddress():
                    truckLocation = addressBook[k].getID()

        package1address = packageTable.search(truck3[i]).getSearchingAddress()
        for k in range(len(addressBook)):
            if package1address == addressBook[k].getFullAddress():
                destination1 = addressBook[k].getID()
        package2address = packageTable.search(truck3[j]).getSearchingAddress()
        for k in range(len(addressBook)):
            if package2address == addressBook[k].getFullAddress():
                destination2 = addressBook[k].getID()
        #with the two possible destinations, find which is shorter
        distance1 = distances[truckLocation][destination1]
        if not distance1:
            distance1 = distances[destination1][truckLocation]

        distance2 = distances[truckLocation][destination2]
        if not distance2:
            distance2 = distances[destination2][truckLocation]

        if distance2 <= distance1:
            tempID = truck3[i]
            truck3[i] = truck3[j]
            truck3[j] = tempID
        j = j + 1
    #end while loop
#end for loop

print ("0 - quit")
print ("1 - print all package data")
print ("2 - print all packages by truck")
print ("3 - run and print all delivery data")
userSelection = int(input())
while(userSelection != 0):
    if(userSelection == 1):
        packageTable.printAll()
    elif(userSelection == 2):
        print("")
        print("Truck 1 package IDs")
        for i in range(len(truck1)):
            print (truck1[i])
        print("")
        print("Truck 2 package IDs")
        for i in range(len(truck2)):
            print (truck2[i])
        print("")
        print("Truck 3 package IDs")
        for i in range(len(truck3)):
            print (truck3[i])
        print("")
    elif(userSelection == 3):
        # Usage of datetime from https://docs.python.org/3/library/datetime.html
        truckSpeed = 18 #miles per hour, constant speed of trucks.
        # TODO get the current date and use that in place of the dummy date
        currentTime = datetime.datetime(2000,10,10,8,0,0,0) # a dummy date of 10/10/2000 since Python won't work on just time values
                                    # That python needs a full date for time calculations learned from:
                                    # https://stackoverflow.com/questions/100210/what-is-the-standard-way-to-add-n-seconds-to-datetime-time-in-python
        truckLocation = 0
        totalDistance = 0
        truck1TotalDistance = 0
        truck2TotalDistance = 0
        truck3TotalDistance = 0
        checkTime1 = datetime.datetime(2000,10,10, 9,0,0,0)
        time1checked = False
        checkTime2 = datetime.datetime(2000,10,10,10,0,0,0)
        time2checked = False
        checkTime3 = datetime.datetime(2000,10,10,13,0,0,0)
        time3checked = False

        #update all packages on truck1 to be en route
        for i in range(len(truck1)):
            tempPackage = packageTable.search(truck1[i])
            tempPackage.setLocation("en route")
            packageTable.update(tempPackage)

        for i in range(len(truck1)):
            packageAddress = packageTable.search(truck1[i]).getSearchingAddress()
            #print(packageAddress, packageTable.search(truck1[i]).getID())
            for j in range(len(addressBook)):
                if packageAddress == addressBook[j].getFullAddress():
                    truckDestination = addressBook[j].getID()
                    #print("address found: ", truckLocation, truckDestination)
            tempDistance = float(distances[truckLocation][truckDestination])
            if not tempDistance:
                tempDistance = float(distances[truckDestination][truckLocation])
            #print("temp distance: ", tempDistance)
            truck1TotalDistance = truck1TotalDistance + tempDistance           
            additionalTime = float(tempDistance / truckSpeed) #this computes time in hours
            currentTime = currentTime + datetime.timedelta(hours = additionalTime)
            #print("Package delivered at ", currentTime.time())
            #check to see if the time is after 9am, print table if it is
            if time1checked is False and currentTime >= checkTime1:
                print("9 am time check:")
                packageTable.printAll()
                time1checked = True
            if time2checked is False and currentTime >= checkTime2:
                print("10 am time check:")
                packageTable.printAll()
                time2checked = True
            if time3checked is False and currentTime >= checkTime3:
                print("1 pm time check:")
                packageTable.printAll()
                time3checked = True
            # update the package in the packageTable to show that it has been delivered
            tempPackage = packageTable.search(truck1[i])
            tempPackage.setDelivered(True)
            tempPackage.setLocation("delivered")
            tempPackage.setTimeDelivered(currentTime)
            packageTable.remove(tempPackage.getID())
            packageTable.insert(tempPackage, tempPackage.getID())
            #print(packageAddress)
            #print("Distance traveled: ", totalDistance)
            #print("")
            truckLocation = truckDestination
        #packages delivered, return to the hub
        tempDistance = float(distances[truckLocation][0])
        if not tempDistance:
            tempDistance = float(distances[0][truckLocation])
        truck1TotalDistance = truck1TotalDistance + tempDistance
        additionalTime = float(tempDistance / truckSpeed)
        currentTime = currentTime + datetime.timedelta(hours = additionalTime)
        truck1FinishTime = currentTime

        truckLocation = 0
        currentTime = datetime.datetime(2000,10,10,9,6,0,0) #truck 2 leaves as soon as the delayed packages arrive at the hub at 9:05am
        # update all other packages on truck1 to be en route
        for i in range(len(truck2)):
            tempPackage = packageTable.search(truck2[i])
            tempPackage.setLocation("en route")
            packageTable.update(tempPackage)


        for i in range(len(truck2)):
            packageAddress = packageTable.search(truck2[i]).getSearchingAddress()
            #print(packageAddress, packageTable.search(truck2[i]).getID())
            for j in range(len(addressBook)):
                if packageAddress == addressBook[j].getFullAddress():
                    truckDestination = addressBook[j].getID()
                    #print("address found: ", truckLocation, truckDestination)
            tempDistance = float(distances[truckLocation][truckDestination])
            if not tempDistance:
                tempDistance = float(distances[truckDestination][truckLocation])
            #print("temp distance: ", tempDistance)
            truck2TotalDistance = truck2TotalDistance + tempDistance           
            #math to calculate delivery time
            additionalTime = float(tempDistance / truckSpeed) #this computes time in hours
            currentTime = currentTime + datetime.timedelta(hours = additionalTime)
            if time1checked is False and currentTime >= checkTime1:
                print("9 am time check:")
                packageTable.printAll()
                time1checked = True
            if time2checked is False and currentTime >= checkTime2:
                print("10 am time check:")
                packageTable.printAll()
                time2checked = True
            if time3checked is False and currentTime >= checkTime3:
                print("1 pm time check:")
                packageTable.printAll()
                time3checked = True
            #print("Package delivered at ", currentTime.time())
            #update the package in the packageTable to show that it has been delivered
            tempPackage = packageTable.search(truck2[i])
            tempPackage.setDelivered(True)
            tempPackage.setLocation("delivered")
            tempPackage.setTimeDelivered(currentTime)
            packageTable.remove(tempPackage.getID())
            packageTable.insert(tempPackage, tempPackage.getID())
            #print(packageAddress)
            #print("Distance traveled: ", totalDistance)
            #print("")
            truckLocation = truckDestination
        #packages delivered, return to the hub
        tempDistance = float(distances[truckLocation][0])
        if not tempDistance:
            tempDistance = float(distances[0][truckLocation])
        truck2TotalDistance = truck2TotalDistance + tempDistance
        additionalTime = float(tempDistance / truckSpeed)
        currentTime = currentTime + datetime.timedelta(hours = additionalTime)
        truck2FinishTime = currentTime

        currentTime = truck1FinishTime
        truckLocation = 0
        # update all packages on truck3 to be en route
        for i in range(len(truck3)):
            tempPackage = packageTable.search(truck3[i])
            tempPackage.setLocation("en route")
            packageTable.update(tempPackage)
        for i in range(len(truck3)):
            packageAddress = packageTable.search(truck3[i]).getSearchingAddress()
            #print(packageAddress, packageTable.search(truck2[i]).getID())
            for j in range(len(addressBook)):
                if packageAddress == addressBook[j].getFullAddress():
                    truckDestination = addressBook[j].getID()
                    #print("address found: ", truckLocation, truckDestination)
            tempDistance = float(distances[truckLocation][truckDestination])
            if not tempDistance:
                tempDistance = float(distances[truckDestination][truckLocation])
            #print("temp distance: ", tempDistance)
            truck3TotalDistance = truck3TotalDistance + tempDistance           
            # math to calculate delivery time
            additionalTime = float(tempDistance / truckSpeed) # this computes time in hours
            currentTime = currentTime + datetime.timedelta(hours = additionalTime)
            if time1checked is False and currentTime >= checkTime1:
                print("9 am time check:")
                packageTable.printAll()
                time1checked = True
            if time2checked is False and currentTime >= checkTime2:
                print("10 am time check:")
                packageTable.printAll()
                time2checked = True
            if time3checked is False and currentTime >= checkTime3:
                print("1 pm time check:")
                packageTable.printAll()
                time3checked = True
            #print("Package delivered at ", currentTime.time())
            # update the package in the packageTable to show that it has been delivered
            tempPackage = packageTable.search(truck3[i])
            tempPackage.setDelivered(True)
            tempPackage.setLocation("delivered")
            tempPackage.setTimeDelivered(currentTime)
            packageTable.remove(tempPackage.getID())
            packageTable.insert(tempPackage, tempPackage.getID())
            #print(packageAddress)
            #print("Distance traveled: ", totalDistance)
            #print("")
            truckLocation = truckDestination
        #packages delivered, return to the hub
        tempDistance = float(distances[truckLocation][0])
        if not tempDistance:
            tempDistance = float(distances[0][truckLocation])
        truck3TotalDistance = truck3TotalDistance + tempDistance
        additionalTime = float(tempDistance / truckSpeed)
        currentTime = currentTime + datetime.timedelta(hours = additionalTime)
        truck3FinishTime = currentTime

        # calculate total distance driven by all of the trucks, and then print all of the required data
        totalDistance = truck1TotalDistance + truck2TotalDistance + truck3TotalDistance
        print("")
        print("Run finished, truck one back at the hub at " + str(truck1FinishTime.time()) + ", with a total of " + str(truck1TotalDistance) + " miles driven.")
        print("Run finished, truck two back at the hub at " + str(truck2FinishTime.time()) + ", with a total of " + str(truck2TotalDistance) + " miles driven.")
        print("Run finished, truck three back at the hub at " + str(truck3FinishTime.time()) + ", with a total of " + str(truck3TotalDistance) + " miles driven.")
        print("Total distance driven by all trucks: ", totalDistance)
        print("")
    
    # With selection complete, reprint menu and get new input from user
    print ("0 - quit")
    print ("1 - print all package data")
    print ("2 - print all packages by truck")
    print ("3 - run and print all delivery data")
    userSelection = int(input())