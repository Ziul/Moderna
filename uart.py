import serial		# metodos para a serial
import os			# metodos para SO

"""
import signal		# pegando sinais como em C

def signal_handler(signal, frame):
    print 'You pressed Ctrl+C!'
    sys.exit(0)

signal.signal(signal.SIGINT, signal_handler)
"""


f=file('./saida.txt','w')

print "Inicializando serial..."
ser = serial.Serial('/dev/ttyACM1',9600)
try:
	ser.open()
except SerialException:
	print "Nao consegui abrir a porta!"

print "Aberta?"
print ser.isOpen()
print "Legivel?" 
print ser.readable()
ser.flush()
#limpa tela
os.system('clear')
ser.flushInput()
print "Aperte Ctrl+C para terminar a leitura\n\n"
try:
	while True:
#		msg = ser.readline()
		msg = ser.read()
		print msg 
		f.write(msg)
except KeyboardInterrupt:
	print "\rFim da leitura!"	
finally:
	ser.close()

f.write('\n')

print "Fechando arquivo \"saida.txt\""
f.close()
print "Serial fechada"
