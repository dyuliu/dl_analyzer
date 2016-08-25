
import sys
import os

sys.path.append('F:/v-kaijin/DL_Analyzer/analyzer/x64/Release')

import _analyzer

class Recorders(object):

    def __init__(self):
        self.__recorder = _analyzer.Recorders()

    def __get__(self):
        return self.__recorder

    def load_from_file(self, filepath):
        self.__recorder.load_from_file(filepath)
    
    def print_total_info(self):
        self.__recorder.print_total_info()

    def print_specify_info(self, record_type, iterval = 1):
        self.__recorder.print_specify_type(record_type, iterval)

    def get_all_data(self, iterval = 1):
        return self.__recorder.get_data(record_type, True, iterval)

    def get_data(self, record_type = "", iterval = 1):
        return self.__recorder.get_data(record_type, False, iterval)