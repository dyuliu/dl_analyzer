
import analyzer
import os

recorder = analyzer.Recorders()
recorder.load_from_file('F:/v-kaijin/DL_Analyzer/TEST/running_info_0.recorder')
recorder.print_total_info()