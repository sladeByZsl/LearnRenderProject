
from pathlib import Path

import lr_framework as lr

SHADER_DIR = Path(__file__).resolve().parent.parent / "01_hello_triangle" / "shaders"

class OneDemo(lr.Application):
    def on_start(self):
        self.set_clear_color((0.2, 0.3, 0.3, 1.0))
        self.shader = self.load_shader("framework_triangle.vert", "framework_triangle.frag")
        self.mesh = lr.Mesh.triangle()
        self.scene = lr.Scene()
        self.game_object = self.scene.create_game_object(self.mesh, (0.0, 0.0), (1.0, 1.0), (0.5, 0.8, 0.5, 0.5))

    def on_update(self):
        pass

    def on_render(self):
        self.scene.draw(self.shader)

    def on_shutdown(self):
        self.scene.clear()

if __name__ == "__main__":
    cfg = lr.ApplicationConfig()
    cfg.title      = "LearnOpenGL - One (Python)"
    cfg.shader_dir = str(SHADER_DIR)
    raise SystemExit(OneDemo(cfg).run())