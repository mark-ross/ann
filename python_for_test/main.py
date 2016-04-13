import random
import math

def main():
    file = open("../pytest_austin/weights.in", 'w')
    
    file.write("480000 120000 1920000\n")
    for _ in range(1200000):
        for _ in range(480000):
            file.write(str(random.randrange(0,255)))
            file.write(" ")
        file.write("\n")
        
    for _ in range(1920000):
        for _ in range(1200000):
             file.write(str(random.randrange(0,255)))
             file.write(" ")
        file.write("\n")
    file.close()
    
    
    patterns = open("../pytest_austin/patterns.in", "w")
    
    patterns.write("5 480000 255\n")
    for _ in range(5):
        for _ in range(86):
            patterns.write(str(random.randrange(0,255)))
            patterns.write(" ")
        patterns.write("\n")
    patterns.close()
    
    correct = open("../pytest_austin/correct.in", "w")
    
    correct.write("5 1920000\n")
    for _ in range(5):
        for _ in range(1920000):
            correct.write(str(random.randrange(0,10)))
            correct.write(" ")
        correct.write("\n")
    correct.close()
    
main()