import os
import subprocess
import math
import matplotlib.pyplot as plt

def get_time(file_name):
  with open(file_name, 'rb') as f:
    try:  # catch OSError in case of a one line file 
        f.seek(-2, os.SEEK_END)
        while f.read(1) != b'\n':
            f.seek(-2, os.SEEK_CUR)
    except OSError:
        f.seek(0)
    last_line = f.readline().decode()
  return float(last_line)

def biggest_time(matrix_dir):
  time = 0
  for f in os.listdir(matrix_dir):
    file_time = get_time(matrix_dir + f)
    if file_time > time:
      time = file_time
  return time

abs_path = os.path.abspath(".") + "/"
aux_prog = abs_path + "bin/auxiliar"      # Programa auxiliar
seq_prog = abs_path + "bin/sequencial"    # Programa Sequencial
thr_prog = abs_path + "bin/threads"       # Programa Threads
pro_prog = abs_path + "bin/processos"     # Programa Processos
mat1 = abs_path + "mat/matriz1.txt"       # Matriz de entrada 1
mat2 = abs_path + "mat/matriz2.txt"       # Matriz de entrada 2
thr_mat = abs_path + "mat/threads/"       # Pasta de resultados do programa Threads
pro_mat = abs_path + "mat/processos/"     # Pasta de resultados do programa Processos
seq_mat = abs_path +"mat/sequencial.txt"  # Local do arquivo resultado do programa Sequencial

mat_size = []
seq_time = []
pro_time = []
thr_time = []
n = 8       

current_time = 0
iteration = 0
                    ### IMPORTANTE ###
# Apague qualquer arquivo txt anterior antes de rodar o programa (sendo feito pela chamada system abaixo)#

while current_time < 120.0:
  #Alterar esses caminhos para onde ficam os resultados de seus respectivos programas, irá apagar tudo dentro deles
  os.system("rm -f ./mat/threads/* ./mat/processos/*") 

  mat_size.append(100 * (2**iteration))
  seq_time.append(0)
  pro_time.append(0)
  thr_time.append(0)
  
  size = mat_size[iteration]
  size_str = str(size)
  p = str(math.ceil(size * size / n))

  auxiliar = subprocess.Popen([aux_prog, size_str, size_str, size_str, size_str])
  auxiliar.wait()
  
  #Realiza as 10 multiplicações por matriz, pode fazer os programas demorarem muito, diminua se necessário
  for i in range(10):
    sequencial = subprocess.Popen([seq_prog, mat1, mat2])
    sequencial.wait()
    process = subprocess.Popen([pro_prog, mat1, mat2, p])
    process.wait()
    threads = subprocess.Popen([thr_prog, mat1, mat2, p])
    threads.wait()
    
    seq_time[iteration] += get_time(seq_mat)
    thr_time[iteration] += biggest_time(thr_mat)
    pro_time[iteration] += biggest_time(pro_mat)    
  
  seq_time[iteration] = round(seq_time[iteration] / 10, 8)
  thr_time[iteration] = round(thr_time[iteration] / 10, 8)
  pro_time[iteration] = round(pro_time[iteration] / 10, 8)

  current_time = seq_time[iteration]
  iteration += 1

  print("Tamanho:    ", mat_size)
  print("Sequencial: ", seq_time)
  print("Threads:    ", thr_time)
  print("Processos:  ", pro_time, "\n")

with open("resultados.txt", "w") as f:
   f.write(str(mat_size) + "\n")
   f.write(str(seq_time) + "\n")
   f.write(str(thr_time) + "\n")
   f.write(str(pro_time))

plt.xticks(mat_size, rotation=30)
plt.xlabel("Tamanho da matriz (X * X)")
plt.ylabel("Tempo (em segundos)")
plt.plot(mat_size, seq_time, color='red', label='Sequencial')
plt.plot(mat_size, thr_time, color='green', label='Threads')
plt.plot(mat_size, pro_time, color='blue', label='Processos')
plt.grid(True)
plt.legend()
plt.savefig("./grafico.png")