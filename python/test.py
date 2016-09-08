
import analyzer
import os

import matplotlib.pyplot as plt

mpi_dir = 'F:/v-kaijin/DL_Analyzer/TEST/mpitest'
mpi_list = os.listdir(mpi_dir)

def get_mpi_list_v(mpi_list, v):
    res = []
    for fp in mpi_list:
        if int(fp.split('_')[0]) == v:
            fp_path = os.path.join(mpi_dir, fp) 
            res.append(fp_path)
    return res

def get_specify_value(filename, type):
    t = analyzer.Recorders()
    t.load_from_file(filename)
    res = t.get_data(type)
    ret = []
    for i in res:
        if i[0] == 5:
            ret.append(i)
    return ret

def test1():
    def draw_single_curve(value, curve_names = '', zoom = ''):
        k = 0
        for log in value:
            x = []
            y = []
            c = 1
            
            for i in log:
                x.append(c)
                y.append(i)
                c = c + 1
            
            plt.plot(x, y, '.-', label = curve_names[k], alpha = 0.5)
            k = k + 1
        # legend
        plt.title('')
        plt.xlabel('size of parameter ( million)')
        plt.ylabel('mpi_time')
        plt.legend()
        # grid on
        plt.grid(True)
        # for save
        plt.show()

    res111 = []
    cvs = []
    for mmm in range(1,21,2):
        fp_list = get_mpi_list_v(mpi_list, mmm)
        r =  get_specify_value(fp_list[0], 'reduce_wall')
        s = []
        for i in r:
            s.append(i[2])
        res111.append(s)
        cvs.append(str(mmm))

    draw_single_curve(res111, cvs)


def test2():
    def draw_single_curve(value, curve_names = '', zoom = ''):
        k = 0
        for log in value:
            x = []
            y = []
            c = 1
            
            for i in log:
                x.append(c)
                y.append(i)
                c = c + 1
            
            plt.plot(x, y, '.-', label = curve_names[k], alpha = 0.5)
            k = k + 1
        # legend
        plt.title('')
        plt.xlabel('machine number')
        plt.ylabel('mpi_time')
        plt.legend()

        # grid on
        plt.grid(True)
        # for save
        plt.show()

    res111 = []
    cvs = []
    for mmm in range(1,21,1):
        fp_list = get_mpi_list_v(mpi_list, mmm)
        r =  get_specify_value(fp_list[0], 'reduce_wall')
        res111.append(r)
    
    res222 = []

    for i in range(10):
        res = []
        for j in range(1,21,1):
            res.append(res111[j-1][i][2])
        res222.append(res)
        cvs.append(str(i*10+10)+'million')

    #print res222

    draw_single_curve(res222, cvs)

test2()


