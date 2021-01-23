# Import libaries
import cs50
import re
import sys #to see the command line argument
import csv

# Check to ensure the corrct number of command line arguements
if len(sys.argv) != 3:
    print("Invalid files provided")

# Import the CSV with people's STR patterns into List of Dictionary
with open(sys.argv[1], 'r') as csvfile:
    # read in as a dictionary
    reader = csv.DictReader(csvfile)
    # Create a list to store the rows in
    people = []
    # Append each row to the list
    for row in reader:
        people.append(row)

# Import the STR patterns from the CSV
with open(sys.argv[1], 'r') as csvfile:
    # read in as a list
    reader = csv.reader(csvfile)
    # Create a list to store the rows in
    strtemp = []
    # Append first row to the list, with only the names of the STRs
    for row in reader:
        count = 0
        strtemp.append(row[1:])
        count = count + 1
        if count > 0:
            break

# Remove the STR's from list of list format, to a normal list
str = strtemp[0]

# Count the number of STRs
numstr = len(str)

#Import the DNA sequence into memory, from txt file
file = open(sys.argv[2], 'r')
sequence = file.read()

# Compute length of the DNA sequence
legth = len(sequence)

# Compute the number of STR repeats in the sequence
repeats = {}
for item in str:
    # Find all instances of the STR, including repeating ones
    instance = re.findall(r'(({})\2*)'.format(item), sequence)
    #Ensure there are no null values
    if instance == []:
        break
    else:
        minstance = max(instance)
    # keep the longest version
    tmplist = []
    for x in minstance:
        tmplist.append(int(len(x)/len(item)))
    maxseq = max(tmplist)
    #Append to repeats
    repeats.update({item: maxseq})

# Check against the people for an exact match on the STR counts
for person in people:
    # Counter to see if all STR counts match
    cnt = 0
    # Interate over all STRs
    for x in str:
        if int(person[x]) == int(repeats[x]):
            cnt = cnt + 1
    # Check if count matches for all STRs
    if cnt == numstr:
        print(person['name'])
        exit()
#If cannot find any matches
print("No match")
                
        