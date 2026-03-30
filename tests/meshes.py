"""
# from a file
python mesh_plot.py triangles.txt

# from stdin (pipe from your C program)
./exec | python mesh_plot.py -

# custom output name
python mesh_plot.py triangles.txt my_mesh.html

"""

import sys
import re

def parse_triangles(text):
    triangles = []
    current = []
    for line in text.strip().splitlines():
        line = line.strip()
        m = re.match(r'v\d+:\s*\(([\d.]+),\s*([\d.]+)\)', line)
        if m:
            current.append([float(m.group(1)), float(m.group(2))])
            if len(current) == 3:
                triangles.append(current)
                current = []
    return triangles

def generate_html(triangles, output_file="mesh.html"):
    all_x = [p[0] for t in triangles for p in t]
    all_y = [p[1] for t in triangles for p in t]
    pad = 0.05
    xmin = max(0, min(all_x) - pad)
    xmax = max(all_x) + pad
    ymin = max(0, min(all_y) - pad)
    ymax = max(all_y) + pad

    tri_js = str(triangles)

    html = f"""<!DOCTYPE html>
<html>
<head>
  <meta charset="utf-8">
  <title>Mesh Viewer</title>
  <style>
    body {{ margin: 0; background: #fff; display: flex; flex-direction: column; align-items: center; font-family: sans-serif; padding: 2rem; }}
    h2 {{ font-weight: 500; font-size: 18px; margin-bottom: 1rem; color: #333; }}
    .chart-wrap {{ position: relative; width: 700px; height: 600px; }}
  </style>
</head>
<body>
  <h2>Mesh — {len(triangles)} triangles</h2>
  <div class="chart-wrap">
    <canvas id="sc"></canvas>
  </div>
  <script src="https://cdnjs.cloudflare.com/ajax/libs/Chart.js/4.4.1/chart.umd.js"></script>
  <script>
    const triangles = {tri_js};

    const seen = new Set();
    const uniquePts = [];
    triangles.forEach(tri => tri.forEach(p => {{
      const key = p[0] + ',' + p[1];
      if (!seen.has(key)) {{ seen.add(key); uniquePts.push({{x: p[0], y: p[1]}}); }}
    }}));

    const ptColors = uniquePts.map(p => {{
      const hue = Math.round((p.x * 180 + p.y * 180) % 360);
      return `hsl(${{hue}}, 70%, 50%)`;
    }});

    const triPlugin = {{
      id: 'triPlugin',
      afterDraw(chart) {{
        const ctx = chart.ctx;
        const xs = chart.scales.x;
        const ys = chart.scales.y;
        triangles.forEach((tri, i) => {{
          const hue = (i * 37) % 360;
          ctx.beginPath();
          tri.forEach((p, j) => {{
            const px = xs.getPixelForValue(p[0]);
            const py = ys.getPixelForValue(p[1]);
            j === 0 ? ctx.moveTo(px, py) : ctx.lineTo(px, py);
          }});
          ctx.closePath();
          ctx.fillStyle = `hsla(${{hue}}, 60%, 65%, 0.15)`;
          ctx.fill();
          ctx.strokeStyle = `hsla(${{hue}}, 50%, 40%, 0.8)`;
          ctx.lineWidth = 1.2;
          ctx.stroke();
        }});
      }}
    }};

    new Chart(document.getElementById('sc'), {{
      type: 'scatter',
      plugins: [triPlugin],
      data: {{
        datasets: [{{
          data: uniquePts,
          backgroundColor: ptColors,
          pointRadius: 5,
          pointHoverRadius: 7
        }}]
      }},
      options: {{
        responsive: true,
        maintainAspectRatio: false,
        plugins: {{
          legend: {{ display: false }},
          tooltip: {{
            callbacks: {{
              label: ctx => `(${{ctx.parsed.x.toFixed(2)}}, ${{ctx.parsed.y.toFixed(2)}})`
            }}
          }}
        }},
        scales: {{
          x: {{ min: {xmin:.2f}, max: {xmax:.2f}, title: {{ display: true, text: 'x' }} }},
          y: {{ min: {ymin:.2f}, max: {ymax:.2f}, title: {{ display: true, text: 'y' }} }}
        }}
      }}
    }});
  </script>
</body>
</html>"""

    with open(output_file, "w") as f:
        f.write(html)
    print(f"saved to {output_file} ({len(triangles)} triangles, {len(uniquePts_count(triangles))} unique points)")

def uniquePts_count(triangles):
    seen = set()
    pts = []
    for tri in triangles:
        for p in tri:
            key = (p[0], p[1])
            if key not in seen:
                seen.add(key)
                pts.append(p)
    return pts

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("usage: python mesh_plot.py input.txt [output.html]")
        print("       cat input.txt | python mesh_plot.py -")
        sys.exit(1)

    if sys.argv[1] == "-":
        text = sys.stdin.read()
    else:
        with open(sys.argv[1]) as f:
            text = f.read()

    output = sys.argv[2] if len(sys.argv) > 2 else "mesh.html"
    triangles = parse_triangles(text)

    if not triangles:
        print("no triangles found, check input format")
        sys.exit(1)

    generate_html(triangles, output)
