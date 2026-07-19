import csv
import sys

import matplotlib.pyplot as plt


def main() -> int:
    path = sys.argv[1] if len(sys.argv) > 1 else "output/bench.csv"

    indices, micros, path_lens, corridor_lens = [], [], [], []
    with open(path, newline="") as f:
        for row in csv.DictReader(f):
            indices.append(int(row["query_index"]))
            micros.append(float(row["micros"]))
            path_lens.append(int(row["path_len"]))
            corridor_lens.append(int(row["corridor_len"]))

    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(10, 6), sharex=True)

    ax1.plot(indices, micros, linewidth=0.8)
    ax1.set_ylabel("query time (us)")
    ax1.set_title(f"findPath timings — {path}")

    ax2.plot(indices, corridor_lens, label="corridor length (triangles)")
    ax2.plot(indices, path_lens, label="path length (waypoints)")
    ax2.set_xlabel("query index")
    ax2.set_ylabel("count")
    ax2.legend()

    fig.tight_layout()
    out = path.rsplit(".", 1)[0] + ".png"
    fig.savefig(out, dpi=150)
    print(f"wrote {out}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
