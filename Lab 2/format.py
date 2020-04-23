

filename = "out.txt"

with open(filename) as f:
    for line in f:
        linedata = line.split('\n')
        if linedata[0] == "10000" or linedata[0] == "100000" or linedata[0] == "1000000" or linedata[0] == "10000000" or linedata[0] == "100000000":
        	#print("\n" + linedata[0] +", 1, 2, 5, 10, 20, 100\n")
        	print("")
        else:
        	s = linedata[0].replace("Time taken for the main part: ", "", 1)
       
        	if s != "1" and s != "2" and s != "5" and s != "10" and s != "20" and s != "100":
        		print(linedata[0].replace("Time taken for the main part: ", "", 1), end = ", ")










