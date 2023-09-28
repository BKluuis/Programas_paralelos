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

def biggest_time(matrixes_dir):
  time = 0
  for f in os.listdir(matrixes_dir):
    file_time = get_time(matrixes_dir + f)
    if file_time > time:
      time = file_time
  return time

abs_path = os.path.abspath(".") + "/"
aux_prog = abs_path + "bin/auxiliar"
seq_prog = abs_path + "bin/sequencial"
thr_prog = abs_path + "bin/threads"
pro_prog = abs_path + "bin/processos"
mat1 = abs_path + "mat/matriz1.txt"
mat2 = abs_path + "mat/matriz2.txt"
thr_mat = abs_path + "mat/threads/"
pro_mat = abs_path + "mat/processos/"
seq_mat = abs_path +"mat/sequencial.txt"

mat_size = 3200
seq_time = [0.0027364, 0.0218146, 0.2011387, 1.7670146, 34.2133383, 300.9980317]
pro_time = [10.13858430, 12.16924630, 16.5207936, 21.69551780, 27.97308970, 29.82755050, 45.11391290, 92.76491390]
thr_time = [277.6083801, 259.8555603, 262.909552, 230.0875367, 205.7950623, 174.3021973, 139.0866028, 127.0888306]
n = [32,24,18,10,8,6,4,2]

iteration = 0
                    ### IMPORTANTE ###
# Apague qualquer arquivo txt anterior antes de rodar o programa #

for i in exec:
  os.system("rm -f ./mat/threads/* ./mat/processos/*")

  pro_time.append(0)
  # thr_time.append(0)
  
  p = str(math.ceil(mat_size * mat_size / i))
  
  for i in range(3):
    print("Iniciando iteração ", i, "\nExecutando processos")
    process = subprocess.Popen([pro_prog, mat1, mat2, p])
    process.wait()
    # print("Executando threads")
    # threads = subprocess.Popen([thr_prog, mat1, mat2, p])
    # threads.wait()
    
    pro_time[iteration] += biggest_time(pro_mat)    
    # thr_time[iteration] += biggest_time(thr_mat)
  
  pro_time[iteration] = round(pro_time[iteration] / 10, 8)
  # thr_time[iteration] = round(thr_time[iteration] / 10, 8)

  iteration += 1

  print("Tamanho:    ", mat_size)
  # print("Threads:    ", thr_time)
  print("Processos:  ", pro_time, "\n")

plt.yticks([x*5 for x in range(20)])
plt.xticks(n, rotation=45)
plt.xlabel("Nº de partições da matriz")
plt.ylabel("Tempo (em segundos)")
plt.plot(n, thr_time, color='green', label='Threads')
plt.plot(n, pro_time, color='blue', label='Processos')
plt.grid(True)
plt.legend()
plt.savefig("./grafico2.png")