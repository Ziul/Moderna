#!/usr/bin/python
import serial		# metodos para a serial
import os			# metodos para SO
import subprocess	# para o gnuplot
import sys			# para ter acesso aos parametros

"""
import signal		# pegando sinais como em C

def signal_handler(signal, frame):
    print 'You pressed Ctrl+C!'
    sys.exit(0)

signal.signal(signal.SIGINT, signal_handler)
#port = raw_input("Entre com a operacao desejada: ")
"""

try:
	ser = serial.Serial(sys.argv[1],9600)
	ser.open()
except IndexError:
	print "Passe uma porta existente com o parametro!"
	print "\teg.: python " + sys.argv[0] + " /dev/ttyUSB0"
	print "\teg.: ./" + sys.argv[0] + " /dev/ttyUSB0"
	sys.exit(0)

if ser.isOpen() & ser.readable():
	f=file('./saida.txt','w')
else:
	print "Porta nao acessivel"
	sys.exit(0)

#limpa tela e buffer
os.system('clear')
ser.flushInput()
print "Aperte Ctrl+C para terminar a leitura\n\n"
while True:
	try:
#		msg = ser.read()
		msg = ser.readline()
		print msg 
	except KeyboardInterrupt:
		print "\rFim da leitura!"
		break	
	finally:
		f.write(msg)
ser.close()

#f.write('\n')

print "Fechando arquivo \"saida.txt\""
f.close()
print "Serial fechada. Abrindo o GNUPLOT"

proc = subprocess.Popen(['gnuplot','-p'], 
                        shell=True,
                        stdin=subprocess.PIPE,
                        )
                        
proc.stdin.write("plot 'saida.txt' with lines\n")
proc.stdin.write("pause mouse\n")
proc.stdin.write("quit\n")
