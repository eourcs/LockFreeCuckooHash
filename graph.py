import sys
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
import matplotlib.lines as lines
import matplotlib.text as mtext

def parse_filename():
  if len(sys.argv) != 2:
    print ("Usage: [python3] {0} [filepath]".format(sys.argv[0]))
    return None
  else:
    return sys.argv[1]

def generate_graph(filename):
  # Read from file
  f = open(filename, 'r')

  graph_name = ""
  x_label = ""
  y_label = ""
  max_x = 0
  max_y = 0
  datasets = {}
  current_dataset = {}

  for line in f:
    words = line.rstrip().split(' ')
    if len(words) == 3:
      (graph_name, x_label, y_label) = tuple(words)
    elif len(words) == 2:
      try:
        current_dataset[int(words[0])] = float(words[1])
        max_x = max(int(words[0]), max_x)
      except ValueError:
        current_dataset[float(words[0])] = float(words[1])
        max_x = max(float(words[0]), max_x)
      max_y = max(float(words[1]), max_y)
    elif len(words) == 1:
      current_dataset = {}
      datasets[words[0]] = current_dataset
    else:
      raise Exception("Invalid file format")

  # Generate graphs
  colors = ['r', 'b', 'g', 'y']

  fig, ax = plt.subplots()

  patches = []
  for i, label in enumerate(datasets):
    data = datasets[label]
    x = []
    y = []
    for k in data:
      x.append(k)
      y.append(data[k])
    ax.plot(x, y, c=colors[i % len(colors)], label=label)
    patches.append(mpatches.Patch(color=colors[i % len(colors)], label=label))



  plt.title(graph_name)
  plt.xlabel(x_label)
  plt.ylabel(y_label)
  plt.axis([0, max_x, 0, 1.25 * max_y])
  plt.legend(handles=patches)
  plt.show()

if __name__ == "__main__":
  filename = parse_filename()
  if filename:
    generate_graph(filename)




