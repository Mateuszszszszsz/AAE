def generate_latex_table(results, metrics, algorithms):
    """
    Generates a LaTeX table from a 2D map of results.

    :param results: dict, 2D map with algorithm names as keys and dictionaries of metrics as values
    :param metrics: list, the names of the metrics (columns)
    :param algorithms: list, the names of the algorithms (rows)
    :return: str, the LaTeX table as a string
    """
    # LaTeX table header
    latex = "\\begin{table}[h!]\n\\centering\n\\begin{tabular}{l" + "|r" * len(metrics) + "}\n"
    latex += "Algorithm & " + " & ".join(metrics) + " \\\\\n\\hline\n"

    # Add rows
    for algo in algorithms:
        latex += algo + " & " + " & ".join(f"{results[algo][metric]:.3f}" for metric in metrics) + " \\\\\n"

    # LaTeX table footer
    latex += "\\end{tabular}\n\\caption{Clustering Evaluation Metrics}\n\\label{tab:clustering_eval}\n\\end{table}"
    return latex


if __name__ == "__main__":
  # Example usage
  results = {
      'a1': {'m1': 0.85, 'm2': 0.78, 'm3': 0.80},
      'a2': {'m1': 0.88, 'm2': 0.76, 'm3': 0.82},
      'a3': {'m1': 0.80, 'm2': 0.81, 'm3': 0.79},
      'a4': {'m1': 0.87, 'm2': 0.83, 'm3': 0.85}
  }
  metrics = ['m1', 'm2', 'm3']
  algorithms = ['a1', 'a2', 'a3', 'a4']

  latex_table = generate_latex_table(results, metrics, algorithms)
  print(latex_table)
