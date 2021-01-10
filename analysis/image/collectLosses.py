# A quick script to collect the data necesary to perform the multi objective loss analysis from the completed experiments.

import subprocess
import numpy as np
import pandas as pd
import os
import shutil

def main():
    root_directory = '/home/mitchell/multiObjectiveExperiments'
    experiment_json_root_directory = '/home/mitchell/cgp/CGPExperiments/experiments/multiObjectiveExperiments'

    losses = []

    lambda_dirs = [d for d in os.listdir(root_directory) if os.path.isdir(os.path.join(root_directory, d))]
    for lambda_dir in lambda_dirs:
        lambda_value = float(lambda_dir)
        lambda_path = os.path.join(root_directory, lambda_dir)
        experiment_dirs = [d for d in os.listdir(lambda_path) if os.path.isdir(os.path.join(lambda_path, d))]
        for experiment_dir in experiment_dirs:
            experiment_num = int(experiment_dir)
            genotype_path = os.path.join(lambda_path, experiment_dir, 'final', 'genotype.json')
            experiment_path = os.path.join(experiment_json_root_directory, lambda_dir, '{0}.json'.format(experiment_dir))

            command = './DumpBuffers {0} {1}'.format(experiment_path, genotype_path)

            print('Executing {0}...'.format(command))

            subprocess.check_call(command, shell=True)

            df = pd.read_csv('dump/losses.txt')
            df['lambda'] = lambda_value
            df['experiment'] = experiment_num

            losses.append(df)

            shutil.rmtree('dump/')

    print('Writing collected losses...')
    all_losses = pd.concat(losses, axis=0)
    print('Shape = {0}'.format(all_losses.shape))

    all_losses.to_csv('all_losses.csv', index=False)

    print('Done!')

if __name__ == '__main__':
    main()

