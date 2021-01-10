# A quick script that can be used to generate the image visualization used in the blog post.

import subprocess
import os
import argparse
import shutil
import decimal

import numpy as np
import cv2

experiment_json_root_directory = '/home/mitchell/cgp/CGPExperiments/experiments/multiObjectiveExperiments'
root_directory = '/home/mitchell/multiObjectiveExperiments'

def parse_node_declaration(line, existing_nodes):
    node_data = {}
    open_bracket_idx = line.find('[')
    close_bracket_idx = line.find(']')

    node_data['node_id'] = int(line[:open_bracket_idx].strip())
    node_str = line[open_bracket_idx+1:close_bracket_idx]

    tokens = node_str.split(',')
    for token in tokens:
        parts = token.split('=')
        node_data[parts[0].strip()] = parts[1][1:-1]

    existing_nodes[node_data['node_id']] = node_data

    return existing_nodes

def parse_node_connections(line, connections):
    line = line.replace(';', '').strip()

    tokens = line.split('->')
    node_from = int(tokens[0])
    node_to = int(tokens[1])

    if (node_from not in connections):
        connections[node_from] = []
    connections[node_from].append(node_to)

    return connections

def read_graph_file(graph_file_name):
    connections = {}
    nodes = {}

    with open(graph_file_name, 'r') as f:
        for line in f:
            if ('label=') in line:
                nodes = parse_node_declaration(line, nodes)
            elif (' -> ') in line:
                connections = parse_node_connections(line, connections)

    output = {}
    output['connections'] = connections
    output['nodes'] = nodes

    return output

def dump_buffers(lambda_value, experiment_number, iteration_number):
    global experiment_json_root_directory
    global root_directory

    experiment_path = os.path.join(
        experiment_json_root_directory, 
        str(lambda_value), 
        '{0}.json'.format(experiment_number))

    genotype_path = os.path.join(
        root_directory,
        str(lambda_value),
        str(experiment_number),
        str(iteration_number),
        'genotype.json')

    if (os.path.exists('dump/')):
        shutil.rmtree('dump/')

    command = './DumpBuffers {0} {1}'.format(experiment_path, genotype_path)

    print(command)

    subprocess.check_call(command, shell=True)

def prune_unused_inputs(graph_data):
    for i in range(0, 7, 1):
        if (i not in graph_data['connections']):
            del graph_data['nodes'][i]

    return graph_data

def generate_png_from_buffer_number(input_data_root, buffer_number, sample_number):
    # First seven inputs correspond to input data channels. Not in buffer dump.
    #if (buffer_number == 0):
    #    input_file = input_data_root + '_r.dat'
    #elif (buffer_number == 1):
    #    input_file = input_data_root + '_g.dat'
    #elif (buffer_number == 2):
    #    input_file = input_data_root + '_b.dat'
    #elif (buffer_number == 3):
    #    input_file = input_data_root + '_h.dat'
    #elif (buffer_number == 4):
    #    input_file = input_data_root + '_l.dat'
    #elif (buffer_number == 5):
    #    input_file = input_data_root + '_s.dat'
    #elif (buffer_number == 6):
    #    input_file = input_data_root + '_gray.dat'
    #else:
    #    input_file = 'dump/{0}.buf'.format(buffer_number - 7)

    input_file = 'dump/{0}.buf'.format(buffer_number)

    image_width = 500
    image_height = 500
    image_size = image_width * image_height
    print(input_file)
    with open(input_file, 'rb') as f:
        f.seek(image_size * sample_number, 0)
        array = np.frombuffer(f.read(image_size), dtype=np.uint8)
        return array.reshape(image_height, image_width)

def write_png_graph_file(graph_data, output_graph_file_name):
    with open(output_graph_file_name, 'w') as f:
        f.write('digraph graphname {\n')
        f.write('  size="1000, 1000"')

        headers = ['red', 'green', 'blue', 'hue', 'luminance', 'saturation', 'grayscale']
        for i in range(0, len(headers), 1):
            if i in graph_data['nodes']:
                f.write('  {0} [label="", shape="box", style="filled", fillcolor="azure", image="{2}"];\n'.format(
                    i,
                    headers[i],
                    graph_data['nodes'][i]['image_path']))

        for node in graph_data['nodes']:
            if node >= 7:
                f.write('  {0} [label="", shape="{2}", style="{3}", fillcolor="{4}", image="{5}"];\n'.format(
                    graph_data['nodes'][node]['node_id'],
                    graph_data['nodes'][node]['label'],
                    graph_data['nodes'][node]['shape'],
                    graph_data['nodes'][node]['style'],
                    graph_data['nodes'][node]['fillcolor'],
                    graph_data['nodes'][node]['image_path']))

        for connection in graph_data['connections']:
            from_node = connection
            to_nodes = graph_data['connections'][from_node]
            for to_node in to_nodes:
                f.write('  {0} -> {1};\n'.format(
                    from_node, 
                    to_node))
        
        f.write('}')

def parse_args():
    parser = argparse.ArgumentParser(description='Generates a graphviz with the images as nodes.')

    parser.add_argument('--lambda_value', type=float, help='Lambda value', required=True)
    parser.add_argument('--experiment_number', type=int, help='Experiment number', required=True)
    parser.add_argument('--output_directory', type=str, help='Output directory', required=True)
    parser.add_argument('--iteration_number', type=str, help='Iteration number', required=True)
    parser.add_argument('--sample_number', type=int, help='Sample number', required=True)
    parser.add_argument('--input_data_root', type=str, help='Input data locations', required=True)
    parser.add_argument('--scale_factor', type=float, help='Image rescale factor', required=True)

    args = parser.parse_args()

    return args

def float_to_str(f):
    ctx = decimal.Context()
    ctx.prec = 20
    d1 = ctx.create_decimal(repr(f))
    return format(d1, 'f')

def main():
    global root_directory
    global experiment_json_root_directory

    args = parse_args()

    lambda_value = ('0' if args.lambda_value == 0 else float_to_str(args.lambda_value))
    print(lambda_value)

    print('Generating gviz for lambda = {0}, experiment = {1}, iteration = {2}, sample = {3} in ouput directory {4}...'.format(
        lambda_value,
        str(args.experiment_number),
        str(args.iteration_number),
        str(args.sample_number),
        str(args.output_directory)))

    existing_graph_file = os.path.join(
        root_directory,
        str(lambda_value),
        str(args.experiment_number),
        str(args.iteration_number),
        'dotActiveGenes.gv')

    print('Parsing {0}...'.format(existing_graph_file))
    graph_data = read_graph_file(existing_graph_file)

    print('Pruning...')
    graph_data = prune_unused_inputs(graph_data)

    print('Creating output directory...')
    os.makedirs(args.output_directory, exist_ok=True)

    print('Dumping...')
    dump_buffers(lambda_value, args.experiment_number, args.iteration_number)

    print('Generating images...')
    for node in graph_data['nodes']:
        image_data = generate_png_from_buffer_number(args.input_data_root, node, args.sample_number)
        image_filepath = os.path.join(args.output_directory, '{0}.png'.format(node))

        new_width = int(image_data.shape[1] * args.scale_factor)
        new_height = int(image_data.shape[0] * args.scale_factor)
        out_image = cv2.resize(image_data, (new_width, new_height))
        cv2.imwrite(image_filepath, out_image)

        graph_data['nodes'][node]['image_path'] = image_filepath

    print('Generating gviz file...')
    gviz_file_name = os.path.join(args.output_directory, 'graph.gv')
    write_png_graph_file(graph_data, gviz_file_name)

    print('Generating gviz image...')
    image_file_name = os.path.join(args.output_directory, 'graph.png')
    command = 'dot -Tpng {0} -o {1}'.format(
        gviz_file_name,
        image_file_name)

    subprocess.check_call(command, shell=True)

    print('Done! Output generated at {0}'.format(image_file_name))

if __name__ == '__main__':
    main()

