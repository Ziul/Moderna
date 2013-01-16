#!/usr/bin/python
# -*- coding:utf-8 -*-

import serial		# metodos para a serial
import os			# metodos para SO
import subprocess	# para subprocessos (gnuplot)
import sys			# para ter acesso aos parametros
import curses		# para a biblioteca curses

"""
import signal		# pegando sinais como em C

def signal_handler(signal, frame):
    print 'You pressed Ctrl+C!'
    sys.exit(0)

signal.signal(signal.SIGINT, signal_handler)
"""

try:
	ser = serial.Serial(sys.argv[1],9600)
	ser.open()
except IndexError:
	print "Passe uma porta existente com o parametro!"
	print "\teg.: python " + sys.argv[0] + " /dev/ttyUSB0"
	print "\teg.: ./" + sys.argv[0] + " /dev/ttyUSB0"
	sys.exit(0)
except IOError as e:
    print("({})".format(e))
    sys.exit(0)
    raise

if ser.isOpen() & ser.readable():
	f=file('./saida.txt','w')
	fx=file('./trabalhada.dot','w')
	fx.write("(0,0) " )
	print "Porta aberta!"

else:
	print "Porta não acessível"
	sys.exit(0)

vsel=0
l_vsel=1
teste=True
#limpa tela e buffer
#os.system('clear')
ser.flushInput()

stdscr = curses.initscr()
curses.noecho()

#print "Aperte Ctrl+C para terminar a leitura\n\n"
stdscr.addstr (0,0,"Aperte Ctrl+C para terminar a leitura\n\n",curses.A_BOLD)
stdscr.addstr (1,0,"!")
while teste:
	try:
		msg = ser.readline()
		#print  msg,
		stdscr.addstr (3,0,msg,curses.A_REVERSE)
		stdscr.refresh()
	except KeyboardInterrupt:
		teste=False
		break	
	
	finally:
		try:
			f.write(msg)
			if (l_vsel == eval(msg.split(" ")[0]) ):
				l_vsel=l_vsel+1
			else:
				raise
			vsel = eval(msg.split(" ")[0])/10.0
			fx.write("-- (%.2f,%.2f) " % (vsel,  eval(msg.split(" ")[1])/100.0  ) )
			if (vsel > 5):
				stdscr.addstr (1,0,"  ")
			else:
				stdscr.addstr (1,0,"!")
			stdscr.refresh()
		except:
			ser.close()
			ser.open()
			f=open('./saida.txt','w')
			fx=open('./trabalhada.txt','w')
			try:
				if len(msg) > 1:
#					print "\rFalha na sincronização!\n\rFoi capturado contudo inválido \t" + msg
					stdscr.addstr (1,0,"!")
#					stdscr.refresh()
			except:
				print "\rNão foi capturado nada!"
				curses.nocbreak(); stdscr.keypad(0); curses.echo()
				curses.endwin()
				ser.close()
				f.close()
				fx.close()
				sys.exit(0)			


stdscr.addstr (5,0, "\r   \nFim da leitura!")
ser.close()

stdscr.addstr (6,0, "Fechando arquivo \"saida.txt\"")
f.close()
fx.close()
stdscr.addstr (7,0, "Serial fechada. Abrindo o GNUPLOT")
stdscr.refresh()

proc = subprocess.Popen(['gnuplot','-p'], 
                        shell=True,
                        stdin=subprocess.PIPE,
                        )
                        
#Margem segura para visualizar os dados
proc.stdin.write("set yrange [-1:1300]\n")
proc.stdin.write("plot 'saida.txt' with lines\n")
proc.stdin.write("pause mouse\n")

stdscr.addstr (8,0, " Clique na figura para fecha-la ",curses.A_BOLD)
stdscr.addstr (25,25, " ")
stdscr.refresh()

#proc.stdin.write("set terminal latex\n")
#proc.stdin.write("unset key\n")
#proc.stdin.write("set output \"graph.tex\" \n")

proc.stdin.write("set table\n")
proc.stdin.write("set output \"graph.table\" \n")
proc.stdin.write("set format \"%.5f\"\n")
proc.stdin.write("plot 'saida.txt'\n")
proc.stdin.write("quit\n")
proc.wait()
#proc.terminate()

stdscr.addstr (9,0, "\n\nGráfico gerado. \nSaindo\n")
stdscr.refresh()

curses.nocbreak(); stdscr.keypad(0); curses.echo()
curses.endwin()		

print "Programa concluído com sucesso"

