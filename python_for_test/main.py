import random
import math

def main():
    file = open("../pytest/weights.in", 'w')
    
    file.write("86 40 12\n")
    for _ in range(40):
        for _ in range(86):
            file.write(str((math.floor(random.uniform(0.0,1.0)*100))/100))
            file.write(" ")
        file.write("\n")
        
    for _ in range(12):
        for _ in range(40):
             file.write(str((math.floor(random.uniform(0.0,1.0)*100))/100))
             file.write(" ")
        file.write("\n")
    file.close()
    
    
    patterns = open("../pytest/patterns.in", "w")
    
    patterns.write("20 86 1\n")
    for _ in range(20):
        for _ in range(86):
            patterns.write(str((math.floor(random.uniform(0.0,1.0)*100))/100))
            patterns.write(" ")
        patterns.write("\n")
    patterns.close()
    
    correct = open("../pytest/correct.in", "w")
    
    correct.write("20 12\n")
    for _ in range(20):
        for _ in range(12):
            correct.write(str((math.floor(random.uniform(0.0,1.0)*100))/100))
            correct.write(" ")
        correct.write("\n")
    correct.close()
    
main()