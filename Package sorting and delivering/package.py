# Erik Bowers 
# WGU June 2021 for C950
# Created: 6/30/2021    Created By: Erik Bowers
# Last updated: 8/11/2021        Last updated by: Erik Bowers

#package class
class Package:
    def __init__(self, ID, address, city, state, zip_code, delivery_deadline, weight, notes):
        self.ID = ID
        self.address = address
        self.city = city
        self.state = state
        self.zip_code = zip_code
        self.delivery_deadline = delivery_deadline
        self.weight = weight
        self.notes = notes
        self.delivered = False
        self.location = "hub"
        self.timeDelivered = None

    
    def __str__(self):
       # return "%\t %, %, % %\n%, % kilos. %" % (self.ID, self.address, self.city, self.state, self.zip_code, self.delivery_deadline, self.weight, self.notes)
       # return f"{self.ID}: {self.address} {self.city}, {self.state} {self.zip_code}\nCurrent location: {self.location}\nDeliver by: {self.delivery_deadline}\nDelivered: {self.getTimeDelivered()}\n{self.weight} kilos. '{self.notes}'"
       #let's return a smaller bit of info for better parsing
       return f"{self.ID}: {self.location}. Deliver by: {self.delivery_deadline}. Delivered: {(self.getTimeDelivered())}"

   # accessors
    def getID(self):
       return self.ID

    def getAddress(self):
       return self.address

    def getCity(self):
       return self.city

    def getState(self):
        return self.state

    def getZipCode(self):
        return self.zip_code

    def getSearchingAddress(self):
        return f"{self.address} {self.zip_code}"

    def getDeliveryDeadline(self):
        return self.delivery_deadline

    def getWeight(self):
        return self.weight

    def getNotes(self):
        return self.notes

    def isDelivered(self):
        return self.delivered

    def getLocation(self):
        return self.location
    
    def getTimeDelivered(self):
        if(self.delivered == False):
            return "Not yet delivered"
        else:
            return self.timeDelivered


    # mutators
    def setAddress(self, newAddress):
        self.address = newAddress

    def setDelivered(self, status):
        self.delivered = status

    def setLocation(self, newLocation):
        self.location = newLocation

    def setTimeDelivered(self, newTimeDelivered):
        self.timeDelivered = newTimeDelivered.time()





    

