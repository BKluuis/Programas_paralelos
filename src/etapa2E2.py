import os
import subprocess
import math
import matplotlib.pyplot as plt
import time

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
pro_time = []
thr_time = []
n = [32,24,18,10,8,6,4,2]

# thr_time = [0.0, 0.0, 8.7258057 , 7.3496473 , 9.1806571 , 22.9132896, 18.705125 , 19.8938228, 23.3714966, 44.7086938]
# pro_time = [0.0, 0.0, 10.6525795, 10.2134915, 11.5543804, 23.9505692, 23.8406654, 25.3647361, 35.8967163, 59.6054581]

iteration = 0
                    ### IMPORTANTE ###
# Apague qualquer arquivo txt anterior antes de rodar o programa #

for i in n:
  os.system("rm -f ./mat/threads/* ./mat/processos/*")

  pro_time.append(0)
  thr_time.append(0)
  
  p = str(math.ceil(mat_size * mat_size / i))
  
  for i in range(3):
    print("Iniciando iteração ", i, "\nExecutando processos")
    t1 = time.time()
    process = subprocess.Popen([pro_prog, mat1, mat2, p])
    process.wait()
    t2 = time.time()
    print("Executando threads")
    t3 = time.time()
    threads = subprocess.Popen([thr_prog, mat1, mat2, p])
    threads.wait()
    t4 = time.time()
    
    # pro_time[iteration] += biggest_time(pro_mat)    
    # thr_time[iteration] += biggest_time(thr_mat)
    pro_time[iteration] += t2 - t1
    thr_time[iteration] += t4 - t3

  pro_time[iteration] = round(pro_time[iteration] / 10, 8)
  thr_time[iteration] = round(thr_time[iteration] / 10, 8)

  iteration += 1

  print("Tamanho:    ", mat_size)
  print("Threads:    ", thr_time)
  print("Processos:  ", pro_time, "\n")

plt.yticks([x*5 for x in range(20)])
plt.xticks(n, rotation=45)
plt.xlabel("Nº de partições da matriz")
plt.ylabel("Tempo (em segundos)")
plt.plot(n, thr_time, color='green', label='Threads')
plt.plot(n, pro_time, color='blue', label='Processos')
plt.grid(True)
plt.legend()
plt.savefig("./grafico2_1.png")