#!/usr/bin/python
# -*- coding:utf-8 -*-

from scipy.interpolate import interp1d
import numpy as np
import matplotlib.pyplot as plt
from math import *
import subprocess	# para subprocessos (gnuplot)


def lagrange(points):
	def P(x):
		total = 0
		n = len(points)
		for i in xrange(n):
			xi, yi = points[i]
			
			def g(i, n):
				
				tot_mul = 1
				for j in xrange(n):
					if i == j:
						continue
					xj, yj = points[j]
					tot_mul *= (x - xj) / float(xi - xj)
					
				return tot_mul 
 
			total += yi * g(i, n)
		return total
	return P

class dados(object):
	x=[]
	y=[]
	file=''
	def __init__(self,path):
		self.file=file(path,'r')
		self.get()
		
	def get(self):
		i=0
		while 1:
			msg=self.file.readline()
			if not msg: break
			self.x.append(eval(msg.split(" ")[0]))
			self.y.append(eval(msg.split(" ")[1]))
			i+=1
		del self.x[-1]
		del self.y[-1]
		
def main():
	d=dados("./saida_green_dupla.txt")
	
	fd=file("./saida.txt","w")
	n=len(d.x)
	for i in xrange(n):
		fd.write("%.3f %.3f\n" % (d.x[i]/164.0, d.y[i]/75.0))
	
	fd.close()
	
#	plt.plot(d.x,d.y)
#	plt.show()

	proc = subprocess.Popen(['gnuplot','-p'], 
		                    shell=True,
		                    stdin=subprocess.PIPE,
		                    )
		                    
	#Margem segura para visualizar os dados
	proc.stdin.write("set table\n")
	proc.stdin.write("set output \"graph.table\" \n")
	proc.stdin.write("set format \"%.5f\"\n")
	proc.stdin.write("plot 'saida.txt'\n")
	proc.stdin.write("quit\n")
	proc.wait()
	#proc.terminate()
			
if __name__ == "__main__":
    main()
