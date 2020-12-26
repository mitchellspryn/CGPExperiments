import argparse
import os
import subprocess
import time

def parse_args():
    parser = argparse.ArgumentParser(description='A quick automation script to run a series of CGP training experiments sequentially.')

    parser.add_argument('--experiment_list_file', required=True, type=str, help='The file with the experiments to run, one after another.')

    args = parser.parse_args()
    return args

def read_experiments(file_name):
    experiment_files = []
    with open(file_name, 'r') as f:
        for line in f:
            experiment_files.append(line.strip())

    return experiment_files

def run_experiment_file(experiment_file_name):
    with open('inprogress.txt', 'w') as f:
        f.write('{0}\n'.format(experiment_file_name))

    result = subprocess.run(['./TrainCgp', experiment_file_name])

    with open('inprogress.txt', 'w') as f:
        f.write('')

    status_filename = ''
    if (result.returncode == 0):
        status_filename = 'succeeded.txt'
    else:
        status_filename = 'failed.txt'

    with open(status_filename, 'a+') as f:
        f.write('{0}\n'.format(experiment_file_name))

def main():
    args = parse_args()

    print('Reading in experiments from {0}...'.format(args.experiment_list_file))
    experiments = read_experiments(args.experiment_list_file)

    print('Found {0} experiments. Running...'.format(len(experiments)))

    for experiment in experiments:
        run_experiment_file(experiment)
        time.sleep(5)

    print('Done!')

if __name__ == '__main__':
    main()


