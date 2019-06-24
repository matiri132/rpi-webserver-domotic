# -*- coding: utf-8 -*-
import sys
sys.path = ["/var/www/lampctrlsv/pythonctrl"] + sys.path

from modules import lamp as l

def application(environ, start_response): 
    peticion = environ['REQUEST_URI']
    if peticion.startswith('/status'):
        output = l.status(0)  + "<br>" + l.status(1)+ "<br>" + l.status(2) + "<br>" + l.status(3) + "<br>"
    elif peticion.startswith('/lamp'):
        lampm = peticion[5]
	if( ord(lampm)<48 or ord(lampm)>52):
            output = ("Valor de lampara incorrecto: <br>  Utilice /lampX tal que X valga 1 , 2, 3 o 4 ")
        else:
            if(lampm == '1'):
                l.modstatus(0)
            elif(lampm == '2'):
                l.modstatus(1)
            elif(lampm == '3'):
                l.modstatus(2)
            elif(lampm == '4'):
                l.modstatus(3)
            output = ("Se ha modificado el estado de L" + lampm)
    else:
        output = l.default_page()

    start_response('200 OK', [('Content-Type', 'text/html; charset=utf-8')]) 
    # Retorno el contenido HTML 
    return output





