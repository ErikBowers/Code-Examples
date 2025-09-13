# Erik Bowers 
# WGU June 2021 for C950
# Created: 6/30/2021    Created By: Erik Bowers
# Last updated: 8/1/2021        Last updated by: Erik Bowers

# address class
class Address:
    def __init__(self, ID, address, zip_code):
        self.ID = int(ID)
        self.address = address
        self.zip_code = zip_code

    def __str__(self):
        return f"{self.ID}: {self.address}, {self.zip_code}"

    def getID(self):
        return self.ID

    def getFullAddress(self):
        return f"{self.address} {self.zip_code}"



