"""
双三角旋转 demo —— 等价于 framework_two_triangles.cpp，逻辑用 Python 实现。

运行：
  cd demos/01_hello_triangle
  cmake -B build && cmake --build build
  PYTHONPATH=build/python python3 ../01_py_demo/framework_two_triangles_demo.py
"""

from pathlib import Path

import lr_framework as lr

SHADER_DIR = Path(__file__).resolve().parent.parent / "01_hello_triangle" / "shaders"


class TwoTrianglesDemo(lr.Application):
    def on_start(self):
        self.set_clear_color((0.2, 0.3, 0.3, 1.0))
        self.shader = self.load_shader("framework_triangle.vert", "framework_triangle.frag")
        mesh = lr.Mesh.triangle()
        self.mesh = mesh
        self.scene = lr.Scene()
        self.left  = self.scene.create_game_object(mesh, (-0.45, -0.10), (0.55, 0.55), (1.0, 0.5, 0.2, 1.0))
        self.right = self.scene.create_game_object(mesh, ( 0.35,  0.08), (0.75, 1.10), (0.2, 0.7, 1.0, 1.0))

    def on_update(self):
        dt = self.delta_time()
        self.left.transform.rotation  += dt * 1.4
        self.right.transform.rotation -= dt * 1.1

    def on_render(self):
        self.scene.draw(self.shader)

    def on_shutdown(self):
        self.scene.clear()


if __name__ == "__main__":
    cfg = lr.ApplicationConfig()
    cfg.title      = "LearnOpenGL - Two Triangles (Python)"
    cfg.shader_dir = str(SHADER_DIR)
    raise SystemExit(TwoTrianglesDemo(cfg).run())
