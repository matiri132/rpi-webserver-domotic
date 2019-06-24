def status(i):
    content = getProc()
    output = textolamp(content , i)
    return output

def getProc():
    spath = "/proc/lampctrl"
    f = open(spath, "r")
    proccont = f.read()
    f.close()
    return proccont

def textolamp(valores , i):

    out = ""
    if valores[i] == '1':
        out += "Lampara L" + str(i+1) + ": Encendida\n"
    else:
        out += "Lampara L" + str(i+1) + ": Apagada\n"
    return out


def modstatus(i):
    
    cont = getProc()
    j = 0
    out = ""

    while j<4:
        if j == i:
            if cont[i] == '1':
                out += '0'
            else:
                out += '1'
        else:
            out += cont[j]
        j += 1

    spath = "/proc/lampctrl"
    f = open(spath , "w")
    f.write(out)
    f.close()
    return 0
    


def default_page():
    # Genero la salida HTML a mostrar al usuario 
    out = "<b> Hola Pipol <b>"
  # Retorno el contenido HTML 
    return out



