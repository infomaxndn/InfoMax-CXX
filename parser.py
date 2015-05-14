import sys

items = []

filename = str(sys.argv[1])
# print filename

with open(filename) as f:
    for line in f:
    	if "D: " in line:
    		common = 0
    		words = line.strip().split('/')
    		for word in words:
    			# if "found " not in word: 
    				if word in items:
    					common = common + 1
    				else:
    					items.append(word)
    					break            
    		print (str(common))
