# Erik Bowers 
# WGU June 2021 for C950
# Created: 6/30/2021    Created By: Erik Bowers
# Last updated: 8/11/2021        Last updated by: Erik Bowers

# hash table
# This is the self-adjusting data structure for the assignment
# This hash table allows for a faster look-up speed than searching an entire list
# for a package. By using a chaining hash table the program can handle a larger
# number of packages than the base 40 without any change and only a minor decrease in
# the speed of the search as a short list must be searched after finding the correct bucket.
# The space usage for the chaining hash table will only increase as more packages are added.
# When this happens, the lists of the buckets will grow to fit the increased number of packages.
# If there are too many collisions and the lists of each bucket grow too long, it is possible to
# increase the size of the main list by creating the hash table with a larger initial capacity thus
# reducing the size of the lists at each bucket. 
# The number of trucks changing would not change the look-up time or space usage of the hash table.
# The number of cities changing would only change the look-up time or space usage of the hash table
# if it was also increasing the number of packages. The hash table uses the ID of a package for sorting,
# so the city it's being sent to doesn't factor in to either look-up or storage space.
# Two other data structures that would have worked in this asignment would have been a direct hash table
# where the package ID corresponds directly to a bucket, or a linear-probing hash table where a collision would
# lead to walking down the hash table until an empty bucket was found (and searching would require walking
# through the list until the item is found or a bucket that has been empty since the start is found).
# Python's dictionary, a type of hash table built-in to Python, would have also worked well.
class chaining_hash_table:
    def __init__(self, initial_capacity):
        self.table = []
        for i in range(initial_capacity):
            self.table.append([])

    def insert(self, package, packageID):
        bucket = hash(packageID) % len(self.table)
        bucket_list = self.table[bucket]
        bucket_list.append(package)

        # the key is the ID
    def search(self, key):
        bucket = hash(key) % len(self.table)
        bucket_list = self.table[bucket]
        for i in range(len(bucket_list)):
            if(bucket_list[i] is not None and key == bucket_list[i].getID()):
                return bucket_list[i]

        # the key is the ID
    def remove(self, key):
        bucket = hash(key) % len(self.table)
        bucket_list = self.table[bucket]
        for i in range(len(bucket_list)):
            if(bucket_list[i] is not None and bucket_list[i].getID() == key):
                bucket_list[i] = None

    # For testing and checking the hash table
    def printAll(self):
        for i in range(len(self.table)):
            for j in range(len(self.table[i])):
                #print(i, j)
                if self.table[i][j] is not None:
                    print (self.table[i][j])

    # updating a package in the hash table
    def update(self, package):
        self.remove(package.getID())
        self.insert(package, package.getID())





