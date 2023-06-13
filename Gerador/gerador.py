import re
import subprocess
import os

def leInput(filename):
    input = open(filename, 'r')

    frames=0
    esferas=[]
    luzes=[]
    dicAtual={}
    dentroEsfera=False
    dentroLuz=False
    for line in input:
        
        if re.search("^FRAMES", line):
            frames = int(line.split("=")[1])
        elif re.search("^ESFERA", line):
            dentroEsfera=True
        elif re.search("^COR", line):
            temp = line.split("=")[1].split(" ")
            dicAtual["cor"]={"r": float(temp[0]),"g": float(temp[1]), "b": float(temp[2])}
        elif re.search("^RAIO", line):
            temp = line.split("=")[1]
            dicAtual["raio"]= float(temp)
        elif re.search("^POSICAOINICIAL", line):
            temp = line.split("=")[1].split(" ")
            dicAtual["posicao_inicial"]={"x": float(temp[0]), "y": float(temp[1]), "z": float(temp[2])}
        elif re.search("^POSICAOFINAL", line):
            temp = line.split("=")[1].split(" ")
            dicAtual["posicao_final"]={"x": float(temp[0]), "y": float(temp[1]), "z": float(temp[2])}
        elif re.search("^ENDESFERA", line):
            if dentroEsfera:
                esferas.append(dicAtual)
                dicAtual={}
            dentroEsfera=False
        elif re.search("^LUZ", line):
            dentroLuz=True
        elif re.search("^ENDLUZ", line):
            if dentroLuz:
                luzes.append(dicAtual)
                dicAtual={}
            dentroLuz=False

    input.close()
    return {
        "esferas": esferas,
        "luzes": luzes,
        "frames": frames
    }

def executar_programa_c(filename):
    try:
        # Executa o programa C compilado (a.out)
        subprocess.run(filename, shell=True)
    except FileNotFoundError:
        print("Arquivo 'a.out' não encontrado.")
    except subprocess.CalledProcessError:
        print("O programa C retornou um erro.")

def geraOutput(nome_programa, filename, frames, esferas, luzes):
    for i in range(frames):
        output = open(filename, 'w')
        content = ""
        for esfera in esferas:
            
            x = esfera["posicao_final"]["x"] * (i)/(frames-1) + esfera["posicao_inicial"]["x"] * ((frames-1)-i)/(frames-1)
            y = esfera["posicao_final"]["y"] * (i)/(frames-1) + esfera["posicao_inicial"]["y"] * ((frames-1)-i)/(frames-1)
            z = esfera["posicao_final"]["z"] * (i)/(frames-1) + esfera["posicao_inicial"]["z"] * ((frames-1)-i)/(frames-1)
            raio = esfera["raio"]
            r = esfera["cor"]["r"]
            g = esfera["cor"]["g"]
            b = esfera["cor"]["b"]
            content+="ESFERA\n"
            content+=f"{x} {y} {z}\n"
            content+=f"{raio} {r} {g} {b}\n\n"

        for luz in luzes:
            x = luz["posicao_final"]["x"] * (i)/(frames-1) + luz["posicao_inicial"]["x"] * ((frames-1)-i)/(frames-1)
            y = luz["posicao_final"]["y"] * (i)/(frames-1) + luz["posicao_inicial"]["y"] * ((frames-1)-i)/(frames-1)
            z = luz["posicao_final"]["z"] * (i)/(frames-1) + luz["posicao_inicial"]["z"] * ((frames-1)-i)/(frames-1)
            content+="PONTO DE LUZ\n"
            content+=f"{x} {y} {z}\n\n"
        output.write(content)
        output.close()
        executar_programa_c(nome_programa)
        os.rename("./imagem.ppm", f"./imagem{i+1}.ppm")



dados = leInput('input.txt')
geraOutput('./RayCast', 'cena.txt', dados["frames"], dados["esferas"], dados["luzes"])